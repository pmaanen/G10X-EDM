// File:	HadronicInteractionGenerator.cc
// Date:	09-May-2015 G.Macharashvili Dubna

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// in progress. problems   
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "HadronicInteractionGenerator.hh"

#include <G4HadronElasticProcess.hh>
#include <G4HadronElastic.hh>
#include <G4SystemOfUnits.hh>
#include <G4DeuteronInelasticProcess.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4NistManager.hh>

G4ProcessManager *HadronicInteractionGenerator::processManager = 0 ;

void*	HadronicInteractionGenerator::ptr = 0 ;

void*	HadronicInteractionGenerator::get() { return (ptr) ? ptr : ptr = new HadronicInteractionGenerator ; }

HadronicInteractionGenerator::~HadronicInteractionGenerator() {

	cout << "**** HadronicInteractionGenerator: generated " << readEvents << " accepted " << acceptedEvents << " events ... " << endl ;

	}
	
HadronicInteractionGenerator::HadronicInteractionGenerator() {
	num_of_tracks = 1 ;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable() ;
	particle = particleTable->FindParticle("deuteron") ;
	def[0] = particle ;
	kineticEnergy = 270.0 * CLHEP::MeV ;
	minTheta =  5.0 * CLHEP::deg ;
	maxTheta = 20.0 * CLHEP::deg ;
	generatedEvents = acceptedEvents = 0 ;
	}

void	HadronicInteractionGenerator::Init(G4ParticleDefinition *Xparticle,G4double XkineticEnergy,G4double t1,G4double t2) {
// Xparticle: deuteron, proton, ... ?
	particle = Xparticle ;
        kineticEnergy = XkineticEnergy ;
        if (t1 >= 0.0) minTheta = t1 ;
        if (t2 >= 0.0) {
                if (t2 > minTheta) maxTheta = t2 ;
                else maxTheta = minTheta + 0.001 ;
                }
	if (XkineticEnergy < 10.0 * MeV) {
		cerr << endl << "**** HadronicInteractionGenerator::Init() [F] very low energy: " ;
		cerr << fixed << showpoint << setprecision(2) << setw(5) << XkineticEnergy << " [MeV]  ... " << endl << endl ;
		abort() ;
		}
	}




void	HadronicInteractionGenerator::GenerateInteraction() {
        do {
		in >> ek[0] >> theta[0] ;
        	theta[0] += CLHEP::RandFlat::shoot(-0.1,+0.1) ;
		theta[0] *= CLHEP::deg ;
		readEvents++ ;
		} while (in.good() && (theta[0] < minTheta || theta[0] > maxTheta)) ;
	if (in.eof()) G4RunManager::GetRunManager()->AbortRun() ;
	else acceptedEvents++ ;
	}
	

void	HadronicInteractionGenerator::initialization() {


	processManager = T->GetDefinition()->GetProcessManager() ;
// particle ...
//--	G4double kineticEnergy = 270.0 * CLHEP::MeV ;
//--	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable() ;
//--	G4ParticleDefinition *deuteron = particleTable->FindParticle("deuteron") ;
	G4DynamicParticle *dynamicParticle = new G4DynamicParticle(particle,G4ThreeVector(0,0,1.0),kineticEnergy) ;
// set material ...
	G4NistManager *nistManager = G4NistManager::Instance() ;
	nistManager->SetVerbose(0) ;
	G4Element *targetMaterial = nistManager->FindOrBuildElement(6) ;
// set processes ...
	activeProcessForced = "elastic" ;
//++	activeProcessForced = "inelastic" ;
// print ...
	cout << fixed << showpoint << setprecision(2) ;
	cout << "HadronicInteractionGenerator::initialization() - get cross sections ..." << endl ;
	cout << particle->GetParticleName() << "  kineticEnergy: " << kineticEnergy << " [MeV] " << endl ;
// cycle over all available hadrinic processes ...
	ProcVect = processManager->GetPostStepProcessVector() ;
	for (G4int k = 0 ; k < processManager->GetProcessListLength() ; k++) {
		Proc = (*ProcVect)(k) ;
//--		ProcType = Proc->GetProcessType() ;
		ProcName = Proc->GetProcessName() ;
//--		ProcTypeName = Proc->GetProcessTypeName(ProcType) ;
		if (ProcTypeName != "Hadronic") continue ; // ignore ...
//--		if (ProcName == "Transportation") continue ; // ignore ...
		if (ProcName == "hadElastic") {
			if (activeProcessForced == "elastic") activeProc = Proc ;
			G4HadronElasticProcess *dElastic = dynamic_cast<G4HadronElasticProcess*>(Proc) ;
			cout << "elastic:   " << particle->GetParticleName() << " + " << targetMaterial->GetName() << "  cross section: " ;
			cout << dElastic->GetElementCrossSection(dynamicDeuteron,targetMaterial)/CLHEP::millibarn << " [mb]" ;
			cout << endl ;
			}
		if (ProcName == "dInelastic") {
			if (activeProcessForced == "inelastic") activeProc = Proc ;
			G4HadronInelasticProcess *dInelastic = dynamic_cast<G4HadronInelasticProcess*>(Proc) ;
			cout << "inelastic: " << particle->GetParticleName() << " + " << targetMaterial->GetName() << "  cross section: " ;
			cout << dInelastic->GetElementCrossSection(dynamicDeuteron,targetMaterial)/CLHEP::millibarn << " [mb]" ;
			cout << endl ;
			}
		}

// get event manager ...
	eventManager = G4EventManager::GetEventManager() ;
// Initialize hists ...
	HistInit() ;
// init elasyics counter ...
	NInelastic = NElastic = 0 ;
	}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// PROBLEM IS TO SIMULATE STEP AND TRACK ... see StepManager for new/delete ...
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void	HadronicInteractionGenerator::UserInteractionGenerator(const G4Step* const_S) {
	G4Step *S = const_cast<G4Step *>(const_S) ;
	G4Track	*T = S->GetTrack() ;
	
	if (!processManager) initialization(T) ;

// open file to write ...
	if (0 == w_event) {
		of.open(activeProcessForced + ".dat") ;
		w_event++ ;
		}
// activate the forced process ...
	G4VParticleChange* c = activeProc->PostStepDoIt(*T,*S) ;
	G4int cNumOfSecondaries = c->GetNumberOfSecondaries() ;

// if hadElastic: TrackStatus == 0 (alive) 
// if dInelastic: TrackStatus == 2 (stopKill) 
//=============================================================== Elastic
	if (activeProc->GetProcessName() == "hadElastic") {
		NElastic++ ;
// modify current track for elastic scattering ...
		c->UpdateStepForPostStep(S) ;
		S->UpdateTrack() ;
		T = S->GetTrack() ;
// xxxxxxxxxxx
cout << activeProcessForced << "  " << setw(2) << cNumOfSecondaries << "  " << c << "  " << T << "  " ;
cout << fixed << showpoint << setprecision(2) << setw(6) << T->GetMomentum().theta()/deg << endl ;
// track pars ...
		G4double d_theta = T->GetMomentum().theta()/deg ;
		G4double d_Ek = T->GetKineticEnergy() ;
		G4double d_phi = T->GetMomentum().phi()/deg ;
// elastic data write ..
		if (d_theta > 3.0 && d_theta < 28.0) {
			if (of.is_open()){
				of << showpoint << fixed ;
				of << setprecision(1) ;
				of << setw(5) << d_Ek ;
				of << " " << setw(5) << d_theta ;
				of << endl ;
				if (w_event++ == 3000000 - 1) of.close() ;
				}
			}
		h_elasticTE->Fill(d_theta,d_Ek) ;
		h_elasticThetaLab->Fill(d_theta) ;
		h_elasticPhi->Fill(d_phi) ;
		}

//=============================================================== Inelastic
	else {
		NInelastic++ ;
// generate secondaries ...
		h_numOfSecondaries->Fill(0.01 + cNumOfSecondaries) ;
		G4int c_Code[20] , c_save = 0 ;
		G4double c_Ek[20] , c_theta[20] , c_phi[20] ;
		for (G4int n = 0 ; n < cNumOfSecondaries && n < 20 ; n++) { // scan secondaries ...
			G4Track* Ts = c->GetSecondary(n) ;
			G4String p_name = Ts->GetParticleDefinition()->GetParticleName() ;
			c_Ek[c_save] = Ts->GetKineticEnergy() ;
			c_theta[c_save] = Ts->GetMomentum().theta()/deg ;
			c_phi[c_save] = Ts->GetMomentum().phi()/deg ;
			c_Code[c_save] = ' ' ;
			if (p_name == "deuteron") c_Code[c_save] = 'd' ;
			if (p_name == "proton") c_Code[c_save] = 'p' ;
			if (p_name == "neutron") c_Code[c_save] = 'n' ;
			if (p_name == "alpha") c_Code[c_save] = 'a' ;

			if (p_name == "deuteron") h_particleTE->Fill(c_theta[c_save],c_Ek[c_save]) ;
			if (p_name == "proton") h_protonTE->Fill(c_theta[c_save],c_Ek[c_save]) ;
			if (p_name == "neutron") h_neutronTE->Fill(c_theta[c_save],c_Ek[c_save]) ;
			if (p_name == "alpha") h_alphaTE->Fill(c_theta[c_save],c_Ek[c_save]) ;

			if (c_Code[c_save] != ' ' && c_Ek[c_save] > 1.0) ++c_save ;
			delete Ts ;
			}
// inelastic ...
		if (of.is_open()){
			of << showpoint << fixed ;
			of << setprecision(1) ;
			of << "e " << cNumOfSecondaries << " " << c_save << endl ;
			for (G4int n = 0 ; n < c_save ; n++) {
				of << showpoint << fixed ;
				of << setprecision(1) ;
				of << setw(1) << char(c_Code[n]) << " " ;
				of << setw(5) << c_Ek[n] << " " ;
				of << setw(5) << c_theta[n] << " " ;
				of << setw(6) << c_phi[n] << " " ;
				of << endl ;
				}
			if (w_event++ == 1000000) of.close() ;
			}
		}

	c->Clear() ;

	eventManager->AbortCurrentEvent() ;

	return ;

	}

void	HadronicInteractionGenerator::HistInit() {
	gRFile->cd() ;
	gRFile->mkdir("Elastic") ;
	gRFile->cd("Elastic") ;

	h_elasticThetaLab = new TH1F("elastic_theta"," #vartheta_{lab} elastic",80,0,80) ;
	h_elasticThetaLab->SetXTitle("#vartheta_{lab} elastic   [deg]") ;
	h_elasticThetaLab->SetYTitle("num. of events") ;

	h_elasticPhi = new TH1F("elastic_phi"," #phi elastic",180,-180,+180) ;
	h_elasticPhi->SetXTitle("#phi elastic   [deg]") ;
	h_elasticPhi->SetYTitle("num. of events") ;

	h_elasticTE = new TH2F("elastic_theta_Ek","",90,0,90,   150,150,300) ;
	h_elasticTE->SetXTitle("#vartheta_{lab} elsetic  [deg]") ;
	h_elasticTE->SetYTitle("E_{k}  elastic   [MeV]") ;
	h_elasticTE->SetOption("col") ;

// Inelastic ...

	gRFile->cd() ;
	gRFile->mkdir("Inlastic") ;
	gRFile->cd("Inlastic") ;

	h_numOfSecondaries = new TH1F("num_secondaries"," number of secondaries (inelastic)",20,0,20) ;
	h_numOfSecondaries->SetXTitle("num. of secondaries") ;
	h_numOfSecondaries->SetYTitle("num. of events") ;

	h_protonTE = new TH2F("proton_theta_Ek"," #vartheta_{lab} proton (break-up)",180,0.,180., 160,0,160) ;
	h_protonTE->SetXTitle("#vartheta_{lab}  (d C #rightarrow p X) inelsetic  [deg]") ;
	h_protonTE->SetYTitle("E_{k}  elastic   [MeV]") ;
	h_protonTE->SetOption("col") ;

	h_deuteronTE = new TH2F("deuteron_theta_Ek"," #vartheta_{lab} deuteron ",180,0.,180., 160,0,160) ;
	h_deuteronTE->SetXTitle("#vartheta_{lab}  (d C #rightarrow d X) inelsetic  [deg]") ;
	h_deuteronTE->SetYTitle("E_{k}  elastic   [MeV]") ;
	h_deuteronTE->SetOption("col") ;

	h_alphaTE = new TH2F("alpha_theta_Ek"," #vartheta_{lab} alpha (He4)",180,0.,180., 160,0,160) ;
	h_alphaTE->SetXTitle("#vartheta_{lab}  (d C #rightarrow #alpha X) inelsetic  [deg]") ;
	h_alphaTE->SetYTitle("E_{k}  elastic   [MeV]") ;
	h_alphaTE->SetOption("col") ;

	h_neutronTE = new TH2F("neutron_theta_Ek"," #vartheta_{lab} neutron ",180,0.,180., 160,0,160) ;
	h_neutronTE->SetXTitle("#vartheta_{lab}  (d C #rightarrow n X) inelsetic  [deg]") ;
	h_neutronTE->SetYTitle("E_{k}  elastic   [MeV]") ;
	h_neutronTE->SetOption("col") ;

	gRFile->cd() ;
	}

// e-o-f
