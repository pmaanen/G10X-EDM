// File:	HomogenusDetector.cc
// Date:	17-Mar-2015 G.Macharashvili Juelich

#include "HomogenusDetector.hh"

#include <string.h>
#include <iostream>
#include <sstream>

#include <G4SystemOfUnits.hh>
#include <G4PVReplica.hh>

#include <UArrow.hh>
#include <Ruller.hh>
#include "G10XModuleSD.hh"
#include "GG4TrackModuleSD.hh"

HomogenusDetector::HomogenusDetector() {
	
	}

HomogenusDetector::~HomogenusDetector() {

	}

G4VPhysicalVolume*	HomogenusDetector::Construct() {

	ConstructMaterials() ;

// World ...
	G4UserLimits	*UserLimitsWorld = new G4UserLimits(400*CLHEP::cm,800*CLHEP::cm,100*CLHEP::ns,10.0*CLHEP::keV,10.0*CLHEP::mm) ;

	G4VisAttributes	*WorldVisAtt=new G4VisAttributes(G4Colour(0.0,0.0,1.0)) ;
	WorldVisAtt->SetForceWireframe(true) ; // ?
	WorldVisAtt->SetVisibility(false) ;

	G4Box *solidWorld = new G4Box("Box",100*CLHEP::cm,100*CLHEP::cm,400*CLHEP::cm) ;
	logicalWorld = new G4LogicalVolume(solidWorld,G4Material::GetMaterial("Vacuum_High"),"World",0,0,0) ;
	logicalWorld->SetVisAttributes(WorldVisAtt) ;
	logicalWorld->SetUserLimits(UserLimitsWorld) ;

	physicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalWorld,"World",0,false,0) ;

// get configuration data from file ...
	config = new VolumeGenerator("config.dat") ;
// calorimeter material ...
	G4Material *material = LYSO ;
	G4String materialName ;
	if (!config->getString("HomogenusDetector.material",materialName)) {
		if (materialName.contains("cint")) material = Scintillator ;
		else if (materialName.contains("tainles")) material = StainlessSteel ;
		else material = LYSO ;
		}
// calorimeter solid solid ...
	solid = config->getG4Tubs("HomogenusDetector") ;
	G4VisAttributes	*VSi = new G4VisAttributes(G4Colour(0.0, 0.7, 0.1)) ;
			VSi->SetVisibility(true) ;
			VSi->SetForceSolid(true) ;
	G4double stepLimiter = 100.0 * CLHEP::mm ;
	G4UserLimits	*UserLimits_LYSO = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,0,0) ;
	logical = new G4LogicalVolume(solid,material,"Homogenus") ;
	logical->SetVisAttributes(VSi) ;
	logical->SetUserLimits(UserLimits_LYSO) ;
// positioning ...
	G4double x = 0 , y = 0 , z = 600*CLHEP::mm ;
	config->getDouble("HomogenusDetector.frontZpos",z) ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z + ((G4Tubs*)solid)->GetZHalfLength()),logical,logical->GetName(),logicalWorld,false,0) ;
// sensitive detector ...
	new G10XModuleSD(logical->GetName(),logical) ;

// ruller and arrow ...
	G4String ruller_on("off") ;
	config->getString("ruller",ruller_on) ;
	if (ruller_on == "on") {
		G4ThreeVector Rzo(0,0,0) ;
		G4ThreeVector Rz1(0,0,z) ;
		new Ruller(logicalWorld,"z",Rzo,Rz1,1*CLHEP::cm) ;
		G4ThreeVector Rxo(-200.0/2*CLHEP::mm,-40.0,z) ;
		G4ThreeVector Rx1(+200.0/2*CLHEP::mm,-40.0,z) ;
		new Ruller(logicalWorld,"x",Rxo,Rx1,1*CLHEP::cm) ;
		}

// end ...

	delete config ;

	return physicalWorld ;

	}

void HomogenusDetector::ConstructMaterials() {
	nistManager = G4NistManager::Instance() ;
	nistManager->SetVerbose(0) ;
// Air 
	nistManager->FindOrBuildMaterial("G4_AIR");
// Scintillator (PolyVinylToluene, C_9H_10)
	Scintillator = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
// Silicon
	nistManager->FindOrBuildMaterial("G4_Si");
	nistManager->FindOrBuildMaterial("G4_Al");
// StainlessSteel ...
	StainlessSteel = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL") ;
// Vacuum "Galactic"
//	nistManager->FindOrBuildMaterial("G4_Galactic");
// Vacuum "Air with low density"
	G4Material* air = G4Material::GetMaterial("G4_AIR") ;
	G4double density = 1.0e-9 * air->GetDensity() ;
	nistManager->BuildMaterialWithNewDensity("Vacuum_High", "G4_AIR",density) ;

	vector<G4double> fractionMass ;
	vector<G4String> elements ;

	elements.push_back("Lu") ;  fractionMass.push_back(0.7145) ;
	elements.push_back( "Y") ;  fractionMass.push_back(0.0403) ;
	elements.push_back("Si") ;  fractionMass.push_back(0.0637) ;
	elements.push_back( "O") ;  fractionMass.push_back(0.1815) ;

	LYSO = nistManager->ConstructNewMaterial("LYSO",elements, fractionMass, 7.40*CLHEP::g/CLHEP::cm3) ;
	elements.clear() ;
	fractionMass.clear() ;

	} 

// eof
