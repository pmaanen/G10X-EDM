// File:	G10XGenerator.cc
// Date:	16-Jul-2014 GM Tbilisi

#include "G10XGenerator.hh"
#include "EdmEventInformation.hh"
#include "EdmPrimaryParticleInformation.hh"
#include <ConfigFileReader.hh>

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <sstream>

G10XGenerator::G10XGenerator() : G4VUserPrimaryGeneratorAction() {
	ConfigFileReader *config = new ConfigFileReader("config.dat") ;
// get library location ...
// initialization of vertex generator selection ...
// DEFAULT GENERATOR IS NEEDED ... xxxxxxxxxxxxxxxxxxxxxxxxxxx
	string lib("") , xclass("") ;
	G4double var[3] = {0,0,0} ;
	config->getString("vertex_generator_lib",lib) ;
	config->getString("vertex_generator_class",xclass) ;
	config->getDouble("vertex_variation",var,3) ;
	vertexGenerator = new X_VertexGenerator(lib.c_str(),xclass.c_str()) ;
	vertexGenerator->getClass()->setVariation(var[0],var[1],var[2]) ;

//	transverse emittance, beta_x, beta_y, etc ...
// ???????????????????           
// initialization of secondaries generator ...

// ================================================ concerning beam polarization ...
// initialization interaction azimuthal asymmetry ...
	beamPolarization = 0.0 ;
	config->getDouble("beam_polarization",beamPolarization) ;
// beam polarization orientation (\phi_{o}) ...
	beamPolarizationPhi = 0.0 ;
	config->getDouble("beam_polarization_phi",beamPolarizationPhi) ;
// accept analysing power ... if zero: exp270.Ay(theta) is used ...
	averageAy = acceptedEvents = averageAnalyzingPower = 0.0 ;
	config->getDouble("average_analyzing_power",averageAnalyzingPower) ;
// initialize phi generator ...
	phiGenerator = new PolarizationPhiGenerator() ;
// ================================================

// initialization of luminosity (event time generator) ...
// ??????????????????

// get particle from config.dat. default [deuteron] ...
	G4String particle_name("deuteron") ;
	config->getString("particle_name",particle_name) ;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable() ;
	particleDef = particleTable->FindParticle(particle_name) ;
	if (!particleDef) particleDef = particleTable->FindParticle("deuteron") ;
// get particle kinetic energy ...
	kineticEnergy = 270 * CLHEP::MeV ;
	config->getDouble("kinetic_energy",kineticEnergy) ;
// theta limits ...
	G4double thetaLimits[2] ;
	minTheta =  4.0 * CLHEP::deg ;
	maxTheta = 22.0 * CLHEP::deg ;
	if (!config->getDouble("theta_limits",thetaLimits,2)) {
		minTheta = thetaLimits[0] * CLHEP::deg ;
		maxTheta = thetaLimits[1] * CLHEP::deg ;
		}
// theta generator ...
	lib = "libInteractionGenerators.so" ;
	xclass = "UniformThetaGenerator" ;
	config->getString("interaction_generator_lib",lib) ;
	config->getString("interaction_generator_class",xclass) ;
	interactionGenerator = new X_InteractionGenerator(lib.c_str(),xclass.c_str()) ;
	interactionGenerator->getClass()->Init(particleDef,kineticEnergy,minTheta,maxTheta) ;
// config is not needed ...
	delete config ;

	particleGun  = new G4ParticleGun() ;

	if (gRFile) HistInit() ;

	}


G10XGenerator::~G10XGenerator() {
	delete interactionGenerator ;
	delete phiGenerator ;
	delete vertexGenerator ;
	delete particleGun ;
	if (gRFile) {
		phiFitter = new GG4PhiFitter() ;
		phiFitter->SetGeneratedParameters(beamPolarization , averageAy / acceptedEvents , beamPolarizationPhi) ;
		phiFitter->ConnectTH1(h_phi) ;
		phiFitter->Print() ;
		delete phiFitter ;
		}
	}


void G10XGenerator::GeneratePrimaries(G4Event* event) {
	G4ThreeVector v = vertexGenerator->generateVertex() ;
	interactionGenerator->getClass()->GenerateInteraction() ;
	particleGun->SetNumberOfParticles(interactionGenerator->getClass()->GetNumberOfTracks()) ;
// if elastic ...
	if (interactionGenerator->getClass()->GetName() == "InteractionElasticF") {
		Ay = exp270.Ay(interactionGenerator->getClass()->GetTheta(0) / CLHEP::deg) ;
		}
// all other possible generators including inelastic ...
	else {
		Ay = averageAnalyzingPower ;
		}
// save asymmetric phi for the first track ...
	interactionGenerator->getClass()->SetPhi(phiGenerator->getRandomPhi(beamPolarization * Ay,beamPolarizationPhi),0) ;
// user information for the event ...
		G4int eventStatus = 1 ;
		if (interactionGenerator->getClass()->GetName() == "InteractionInelasticF") eventStatus = 2 ;
		event->SetUserInformation(new EdmEventInformation(eventStatus)) ;
// ...
	for (G4int i = 0 ; i < interactionGenerator->getClass()->GetNumberOfTracks() ; i++) {
		G4double theta = interactionGenerator->getClass()->GetTheta(i) ;
		G4double phi = interactionGenerator->getClass()->GetPhi(i) ;
		G4String particleName = interactionGenerator->getClass()->GetParticleDefinition(i)->GetParticleName() ;
		particleGun->SetParticleDefinition(interactionGenerator->getClass()->GetParticleDefinition(i)) ;
		particleGun->SetParticleEnergy(interactionGenerator->getClass()->GetKineticEnergy(i)) ;
		particleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta))) ;
		particleGun->SetParticlePosition(v) ;
		particleGun->SetParticleTime(0) ;
		particleGun->GeneratePrimaryVertex(event) ;
// plot FOM in lab ...
		if (gRFile) {
			h_theta->Fill(theta/CLHEP::deg) ;
			h_phi->Fill(phi/CLHEP::deg) ;
			p_Ay->Fill(theta/CLHEP::deg,Ay) ;
			G4int particleIndex = 0 ;
			if (particleName == "deuteron") particleIndex = 1 ;
			else if (particleName == "proton") particleIndex = 2 ;
			else if (particleName == "neutron") particleIndex = 3 ;
			else if (particleName == "alpha") particleIndex = 4 ;
			h_part->Fill(particleIndex + 0.1) ;
			}
		}
//	cout << event->GetEventID() << endl ;
	if (gRFile) {
		h_mult->Fill(interactionGenerator->getClass()->GetNumberOfTracks() + 0.1) ;
		vertex_xy->Fill(v.x(),v.y()) ;
		vertex_zy->Fill(v.z(),v.y()) ;
		}
	averageAy += Ay ;
	acceptedEvents++ ;
	}

void G10XGenerator::HistInit() {
	TString dir("G10XGenerator") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;

	h_theta = new TH1F("theta_lab","theta lab",  100,   0.0,  25.0) ;
	h_theta->SetXTitle("#vartheta_{lab}    [deg]") ;
	h_theta->SetYTitle("number of events") ;
	h_theta->SetOption("e") ;

	p_Ay = new TProfile("Ay_profile","analysing power distribution",  100,   0.0,  25.0, "") ;
	p_Ay->SetXTitle("#vartheta_{lab}    [deg]") ;
	p_Ay->SetYTitle("Ay") ;
	p_Ay->SetAxisRange(0.0,1.1,"y") ;
	p_Ay->SetOption("e") ;

	h_phi   = new TH1F("phi_gen",  "phi generated",  40,-180.0,+180.0) ;
	h_phi->SetXTitle("#phi    [deg]") ;
	h_phi->SetYTitle("number of events") ;
	h_phi->SetOption("e") ;

	h_part   = new TH1F("particle",  "particle index",  10,0,10.0) ;
	h_part->SetXTitle("particle index") ;
	h_part->SetYTitle("number of events") ;
	h_part->GetXaxis()->SetBinLabel(1,"X") ;
	h_part->GetXaxis()->SetBinLabel(2,"d") ;
	h_part->GetXaxis()->SetBinLabel(3,"p") ;
	h_part->GetXaxis()->SetBinLabel(4,"n") ;
	h_part->GetXaxis()->SetBinLabel(5,"#alpha") ;

	h_mult   = new TH1F("multiplicity",  "particle multiplicity",  10,0,10.0) ;
	h_mult->SetXTitle("particle multiplicity") ;
	h_mult->SetYTitle("number of events") ;

	vertex_xy = new TH2F("vertex_xy","interaction point (x,y)",200,-10,+10,200,-10,+10) ;
	vertex_xy->SetXTitle("vertex x-coordinate    [mm]") ;
	vertex_xy->SetYTitle("vertex y-coordinate    [mm]") ;
	vertex_xy->SetOption("col") ;

	vertex_zy = new TH2F("vertex_zy","interaction point (z,y)",200,-10,+10,200,-10,+10) ;
	vertex_zy->SetXTitle("vertex z-coordinate    [mm]") ;
	vertex_zy->SetYTitle("vertex y-coordinate    [mm]") ;
	vertex_zy->SetOption("col") ;

	gDirectory->cd("/") ;

	}

// e-o-f
