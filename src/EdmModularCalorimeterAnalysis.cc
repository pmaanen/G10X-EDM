// File:	EdmModularCalorimeterAnalysis.cc
// Date:	28-May-2015 G.Macharashvili Juelich

#include "EdmModularCalorimeterAnalysis.hh"
#include "VolumeGenerator.hh"

#include <Randomize.hh>

#include <G4Box.hh>

EdmModularCalorimeterAnalysis::EdmModularCalorimeterAnalysis() {
       	sx = 15.0*CLHEP::mm , sy = 15.0 * CLHEP::mm , sz = 100.0 * CLHEP::mm ;
       	frontZPosition = 460.0 * CLHEP::mm ;
       	asymmetry_PA = 0.0 ;
       	beamPolarizationPhi = 0.0 ;
	}

EdmModularCalorimeterAnalysis::~EdmModularCalorimeterAnalysis() {
	cout << "ModularCalorimeterAnalysis::dtor ..." << endl ;
	cout << endl ;
// do fit to extract beam parameters ...
        if (gRFile) {
        	phiFitter = new GG4PhiFitter() ;
        	phiFitter->SetGeneratedParameters(asymmetry_PA,beamPolarizationPhi) ;
        	phiFitter->ConnectTH1(phiLocal) ;
        	phiFitter->Print() ;
        	delete phiFitter ;
        	}
	}

void	EdmModularCalorimeterAnalysis::Initialize() {
        if (gRFile) {
// get beam parameters ...
        	VolumeGenerator *config = new VolumeGenerator("config.dat") ;
        	config->setVerboseLevel(0) ; 
        	config->getDouble("asymmetry_PA",asymmetry_PA) ;
        	config->getDouble("beam_polarization_phi",beamPolarizationPhi) ;
        	config->getDouble("cal.distance.z",frontZPosition) ;
        	G4VSolid *solid = config->getG4Box("crystal") ;
        	sx = 2.0*((G4Box *) solid)->GetXHalfLength() ;
        	sy = 2.0*((G4Box *) solid)->GetYHalfLength() ;
        	sz = 2.0*((G4Box *) solid)->GetZHalfLength() ;
        	delete config ;
// histograms ...
                HistInit() ;
                }
	}

void	EdmModularCalorimeterAnalysis::ProcessEvent(const G4Event *evt,ModuleHitsCollection* HC) {
	if (0 == HC || 0 == gRFile) return ;
	if (0 == HC->entries()) return ;
	if (HC->GetName() != "calor.M") return ;

	moduleHcollection = HC ;
	hmax = 0 ;
	numActiveHits = HC->entries() ;
	eTotal = 0 ;
	for (int h = 0 ; h < numActiveHits ; h++) {
        	hit = (*HC)[h] ;
		eTotal += hit->GetEnergyDeposit() ;
	        if (!hmax) hmax = hit ;
	        else if (hmax->GetEnergyDeposit() < hit->GetEnergyDeposit()) hmax = hit ;
	        }
        if (!hmax) return ;

// analysis goes on ...

        activeM->Fill(0.01 + numActiveHits) ;
	energyTotal->Fill(eTotal) ;


// main energy cut to distinguish signal/background ... mainThresholdEnergy = 
//++	if (hmax->GetEnergyDeposit() < 150.0 * CLHEP::MeV) return ;

        ixMax = hmax->GetDetectorID()%1000 ;
        iyMax = hmax->GetDetectorID()/1000 ;

// local analysis ...
	eLocal = 0 , xLocal = 0 , yLocal = 0 ;
	G4double  e , x , y ;
	numActiveLocal = 0 ;
	for (int i = ixMax - 1 ; i <= ixMax + 1 ; i++) {
                for (int j = iyMax - 1 ; j <= iyMax + 1 ; j++) {
			hit = GetHit(i,j) ;
			if (!hit) continue ;
//			hit->Print() ;
                        e = hit->GetEnergyDeposit() ;
// noise-cut ?? ... -> Plot rejected noise spectrum ...
//++			if (e < 0.01 * CLHEP::MeV) continue ;
                        x = getX(i) ;
                        y = getY(j) ;
			eLocal += e ;
			xLocal += e*x ;
			yLocal += e*y ;
			numActiveLocal++ ;
                        }
                }
	x = xLocal /= eLocal ;
	y = yLocal /= eLocal ;
//	if (numActiveLocal > 3) return ;
        activeMLocal->Fill(0.01 + numActiveLocal) ;
	energyLocal->Fill(eLocal) ;
	posLocal->Fill(xLocal,yLocal) ;
// variate position (??) 
	double sigma = 5.0 * CLHEP::mm ;
//	do { x = CLHEP::RandGauss::shoot(xLocal,sigma) ; } while (x > 2.0 * sigma) ;
//	do { y = CLHEP::RandGauss::shoot(yLocal,sigma) ; } while (y > 2.0 * sigma) ;
// select stopped deuteron and plot phi/theta ...
	double phi = atan2(y,x) / CLHEP::deg ;
	double thetaLab = atan(sqrt(x*x+y*y)/(frontZPosition - 5.0)) / CLHEP::deg ;
	phiLocal->Fill(phi) ;
	thetaLocal->Fill(thetaLab) ;

// check ...
	G4ThreeVector momGen = evt->GetPrimaryVertex(0)->GetPrimary()->GetMomentum() ;
	double thetaGen = momGen.getTheta() / CLHEP::deg ;
	double phiGen = momGen.getPhi() / CLHEP::deg ;
	thetaDiff->Fill(thetaLab - thetaGen) ;
	phiDiff->Fill(phi - phiGen) ;

	}


GG4ModuleHit	*EdmModularCalorimeterAnalysis::GetHit(int x,int y) {
	for (int h = 0 ; h < moduleHcollection->entries() ; h++) {
		if (y*1000 + x == (*moduleHcollection)[h]->GetHitID()) return (*moduleHcollection)[h] ;
		}
	return 0 ;
	}


G4double EdmModularCalorimeterAnalysis::getX(int i) {
	G4int nx = 40 ;
	G4double xo = (nx%2) ? -nx/2*sx : -nx/2*sx + sx/2 ;
	return xo + sx * i ;
	}


G4double EdmModularCalorimeterAnalysis::getY(int j) {
	G4int ny = 40 ;
	G4double yo = (ny%2) ? -ny/2*sy : -ny/2*sy + sy/2 ;
	return yo + sy * j ;
	}


void EdmModularCalorimeterAnalysis::HistInit() {
	string name("EdmModularCalorimeterAnalysis") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.c_str())) {
		gDirectory->mkdir(name.c_str()) ;
		gDirectory->cd(name.c_str()) ;

		energyTotal = new TH1F("energy_total","summary energy deposit",200,0,300.) ;
		energyTotal->SetXTitle("energy deposit    [MeV]") ;
		energyTotal->SetYTitle("number of events") ;

		energyLocal = new TH1F("energy_local","energy deposit in modules around max",200,0,300.) ;
		energyLocal->SetXTitle("energy deposit    [MeV]") ;
		energyLocal->SetYTitle("number of events") ;
		
		activeM = new TH1F("active_M","stored hits",20,0,20) ;
		activeM->SetXTitle("number of stored hits") ;
		activeM->SetYTitle("number of events") ;

		activeMLocal = new TH1F("active_M_Local","active modules",20,0,20) ;
		activeMLocal->SetXTitle("number of active modules") ;
		activeMLocal->SetYTitle("number of events") ;

		posLocal = new TH2F("pos_Local","calorimeter entry point",100,-250,+250,100,-250,+250) ;
		posLocal->SetXTitle("calorimeter entry x   [mm]") ;
		posLocal->SetYTitle("calorimeter entry y   [mm]") ;
		posLocal->SetOption("col") ;

		phiLocal = new TH1F("phi_Calor"," #phi angle of detected deuteron ",  36,-180.0,+180.0) ;
		phiLocal->SetXTitle("#phi    [deg]") ;
		phiLocal->SetYTitle("number of events") ;
		phiLocal->SetOption("e") ;

		thetaLocal = new TH1F("theta_Calor"," #vartheta_{lab} ",70,0,30) ;
		thetaLocal->SetXTitle("#vartheta_{lab}   [deg]") ;
		thetaLocal->SetYTitle("num. of events") ;
		thetaLocal->SetOption("e") ;

		phiDiff = new TH1F("phi_Diff"," #phi measurement uncertainty ",  100,-10.0,+10.0) ;
		phiDiff->SetXTitle("#phi - #phi_{gen}    [deg]") ;
		phiDiff->SetYTitle("number of events") ;

		thetaDiff = new TH1F("theta_Diff"," #vartheta measurement uncertainty ",  100,-10.0,+10.0) ;
		thetaDiff->SetXTitle("#vartheta - #vartheta_{gen}    [deg]") ;
		thetaDiff->SetYTitle("number of events") ;

		gDirectory->cd("/") ;
		}

        }
        
// e-o-f
