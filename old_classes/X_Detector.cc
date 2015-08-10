// File:	X_Detector.cc
// Date:	06-Apr-2015 G.Macharashvili Dubna

#include "X_Detector.hh"

#include <string.h>
#include <iostream>
#include <sstream>

#include <G4SystemOfUnits.hh>
#include <G4PVReplica.hh>

#include <UArrow.hh>
#include <Ruller.hh>
#include "GG4ModuleSD.hh"
#include "BeamPipe.hh"

X_Detector::X_Detector() {
	checkOverlaps = false ;
	
	}

X_Detector::~X_Detector() {

	}

G4VPhysicalVolume*	X_Detector::Construct() {

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

	physicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalWorld,"World",0,false,0,checkOverlaps) ;

// get configuration data from file ...
	config = new VolumeGenerator("config.dat") ;

// draw beam pipe ...
// ==================
	// matertial ...
	material = GetMaterial("X.vacuumWindowMaterial","G4_STAINLESS-STEEL") ;
	G4double windowZposition = 300.0 * CLHEP::mm ;
	config->getDouble("X.vacuumWindowZPosition",windowZposition) ;
	G4double windowThickness =   0.5 * CLHEP::mm ;
	config->getDouble("X.vacuumWindowThickness",windowThickness) ;
	BeamPipe *bp = new BeamPipe(material,windowZposition,windowThickness) ;
	new G4PVPlacement(0,G4ThreeVector(0,0,0),bp->GetLogical(),bp->GetLogical()->GetName(),logicalWorld,false,0) ;
	
// absorber layer ...
// ==================
	// solid ...
	solid = config->getG4Tubs("X.absorber") ;
	halfZAbs = ((G4Tubs *) solid)->GetZHalfLength() ;
	G4VisAttributes	*Vabs = new G4VisAttributes(G4Colour(0.8, 0.1, 0.0)) ;
			Vabs->SetVisibility(true) ;
			Vabs->SetForceSolid(true) ;
	G4double stepLimiter = 100.0 * CLHEP::mm ;
	G4UserLimits *UserLimits_abs = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,0,0) ;
	// matertial ...
	material = GetMaterial("X.absorberMaterial","G4_W") ;
	// logical ...
	G4LogicalVolume *logicalAbs0 = new G4LogicalVolume(solid,material,"X.absorber0") ;
	logicalAbs0->SetVisAttributes(Vabs) ;
	logicalAbs0->SetUserLimits(UserLimits_abs) ;
	G4LogicalVolume *logicalAbs1 = new G4LogicalVolume(solid,material,"X.absorber1") ;
	logicalAbs1->SetVisAttributes(Vabs) ;
	logicalAbs1->SetUserLimits(UserLimits_abs) ;
	// sensitive ...
	new GG4ModuleSD("absorber.0",logicalAbs0) ;
	new GG4ModuleSD("absorber.1",logicalAbs1) ;

// sensitive layer ...
// ===================
	// solid ...
	solid = config->getG4Tubs("X.sensitive") ;
	halfZSens = ((G4Tubs *) solid)->GetZHalfLength() ;
	G4VisAttributes	*Vsens = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9)) ;
			Vsens->SetVisibility(true) ;
			Vsens->SetForceSolid(true) ;
	stepLimiter = 100.0 * CLHEP::mm ;
	G4UserLimits *UserLimits_sens = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,0,0) ;
	// material ...
	material = GetMaterial("X.sensitiveMaterial","G4_PLASTIC_SC_VINYLTOLUENE") ;
	// logical ...
	G4LogicalVolume *logicalSens0 = new G4LogicalVolume(solid,material,"X.sensitive.0") ;
	logicalSens0->SetVisAttributes(Vsens) ;
	logicalSens0->SetUserLimits(UserLimits_sens) ;
	G4LogicalVolume *logicalSens1 = new G4LogicalVolume(solid,material,"X.sensitive.1") ;
	logicalSens1->SetVisAttributes(Vsens) ;
	logicalSens1->SetUserLimits(UserLimits_sens) ;
	// sensitive ...
	new GG4ModuleSD("sensitive.0",logicalSens0) ;
	new GG4ModuleSD("sensitive.1",logicalSens1) ;

// calorimeter ...
// ===============
	// solid ...
	solid = config->getG4Tubs("X.calorimeter") ;
	halfZCalor = ((G4Tubs *) solid)->GetZHalfLength() ;
	G4VisAttributes	*Vcalor = new G4VisAttributes(G4Colour(0.0, 0.7, 0.7)) ;
			Vcalor->SetVisibility(true) ;
			Vcalor->SetForceSolid(true) ;
	stepLimiter = 100.0 * CLHEP::mm ;
	G4UserLimits *UserLimits_calor = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,0,0) ;
	// material ...
	material = GetMaterial("X.calorimeterMaterial","LYSO") ;
	// logical ...
	G4LogicalVolume *logicalCalor = new G4LogicalVolume(solid,material,"X.calorimeter") ;
	logicalCalor->SetVisAttributes(Vcalor) ;
	logicalCalor->SetUserLimits(UserLimits_calor) ;
	// sensitive ...
	new GG4ModuleSD("sensitive.2",logicalCalor) ;

// positioning ...
// ===============
	G4double x = 0 , y = 0 ; // positioning coordinates ...
	G4double z = 400*CLHEP::mm ;     // default distance of the first volume ...
	config->getDouble("X.distance.z",z) ;

// absorber 0 ...
	z += halfZAbs ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalAbs0,logicalAbs0->GetName(),logicalWorld,false,0) ;
	z += halfZAbs ;
// scintillator 0 ...
	z += halfZSens ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalSens0,logicalSens0->GetName(),logicalWorld,false,0) ;
	z += halfZSens ;
// absorber 1 ...
	z += halfZAbs ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalAbs1,logicalAbs1->GetName(),logicalWorld,false,1) ;
	z += halfZAbs ;
// scintillator 1 ...
	z += halfZSens ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalSens1,logicalSens1->GetName(),logicalWorld,false,1) ;
	z += halfZSens ;
// calorimeter ...
	z += 20 * CLHEP::mm ;
	z += halfZCalor ;
	new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalCalor,logicalCalor->GetName(),logicalWorld,false,0) ;
	z += halfZCalor ;
// end ...

/*
// ruller and arrow ...
	G4String ruller_on("off") ;
	config->getString("ruller",ruller_on) ;
	if (ruller_on == "on") {
		G4ThreeVector Rzo(0,0,0) ;
		G4ThreeVector Rz1(0,0,z) ;
		new Ruller(logicalWorld,"z",Rzo,Rz1,1*CLHEP::cm) ;
		G4ThreeVector Rxo(-200.0/2*CLHEP::mm,-40.0,z-sz) ;
		G4ThreeVector Rx1(+200.0/2*CLHEP::mm,-40.0,z-sz) ;
		new Ruller(logicalWorld,"x",Rxo,Rx1,1*CLHEP::cm) ;
		}
*/

	delete config ;

	return physicalWorld ;

	}

G4Material *X_Detector::GetMaterial(const char *unitName,const char *defaultMaterialName) {
	G4String materialName ;
	G4Material *xmaterial = 0 ;
	if (!config->getString(unitName,materialName)) xmaterial = nistManager->FindOrBuildMaterial(materialName) ;
	if (!xmaterial) xmaterial = nistManager->FindOrBuildMaterial(defaultMaterialName) ;
	return xmaterial ;
	}


void X_Detector::ConstructMaterials() {
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
	nistManager->BuildMaterialWithNewDensity("StainlessSteel", "G4_STAINLESS-STEEL",StainlessSteel->GetDensity()) ;
// Vacuum "Galactic"
//	nistManager->FindOrBuildMaterial("G4_Galactic");
// Vacuum "Air with low density"
	G4Material* air = G4Material::GetMaterial("G4_AIR") ;
	G4double density = 1.0e-9 * air->GetDensity() ;
	nistManager->BuildMaterialWithNewDensity("Vacuum_High", "G4_AIR",density) ;
	nistManager->BuildMaterialWithNewDensity("Tungsten", "G4_W",18.25 * CLHEP::g/CLHEP::cm3) ;

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

// e-o-f
