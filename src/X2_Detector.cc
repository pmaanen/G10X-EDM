// File:	X2_Detector.cc
// Date:	16-Apr-2015 G.Macharashvili Dubna

#include "X2_Detector.hh"

#include <string.h>
#include <iostream>
#include <sstream>

#include <G4SystemOfUnits.hh>
//--#include <G4PVReplica.hh>

//#include <UArrow.hh>
//#include <Ruller.hh>
#include "GG4ModuleSD.hh"
#include "BeamPipe.hh"


X2_Detector::X2_Detector() {
	checkOverlaps = false ;
	}

X2_Detector::~X2_Detector() {
	}

G4VPhysicalVolume*	X2_Detector::Construct() {

const	G4double minEnergyCut = 0.100 * CLHEP::MeV ;

// new materials if needed ...
	ConstructMaterials() ;
	G4Material *air = nistManager->FindOrBuildMaterial("G4_AIR") ;
	G4Material *vacuum = nistManager->FindOrBuildMaterial("Vacuum_High") ;
	G4Material *steel = nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL") ;

// get configuration data from file ...
	config = new VolumeGenerator("config.dat") ;
	config->setVerboseLevel(0) ; 
// World ...
	G4UserLimits	*UserLimitsWorld = new G4UserLimits(1000*CLHEP::mm,1000*CLHEP::mm,100*CLHEP::ns,10.0*CLHEP::keV,0) ;
	G4Box *solidWorld = new G4Box("Box",100*CLHEP::cm,100*CLHEP::cm,400*CLHEP::cm) ;

	G4VisAttributes	*WorldVisAtt=new G4VisAttributes(G4Colour(0.0,0.0,1.0)) ;
	WorldVisAtt->SetForceWireframe(true) ; // ?
	WorldVisAtt->SetVisibility(false) ;

	logicalWorld = new G4LogicalVolume(solidWorld,air,"World",0,0,0) ;
	logicalWorld->SetVisAttributes(WorldVisAtt) ;
	logicalWorld->SetUserLimits(UserLimitsWorld) ;

	physicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalWorld,"World",0,false,0,checkOverlaps) ;

// place and draw beam pipe ...
// ============================
	G4String windowOn = "off" ;
	if (!config->getString("X.vacuumWindow",windowOn) && windowOn == "on") {
		G4double windowZposition = 400.0 * CLHEP::mm ;
		config->getDouble("X.vacuumWindowZPosition",windowZposition) ;
		G4double windowThickness = 0.5 * CLHEP::mm ;
		config->getDouble("X.vacuumWindowThickness",windowThickness) ;
// build beam pipe ...
		BeamPipe *bp = new BeamPipe(windowZposition,windowThickness) ;
		G4LogicalVolume *logicalBeamPipe = bp->BuildLogicalBeamPipe(steel) ;
		G4LogicalVolume *logicalVacuum = bp->BuildLogicalVacuum(vacuum) ;
		G4LogicalVolume *logicalBeam = bp->BuildLogicalBeam(vacuum) ;
// make sensitive beam_pipe ...
//		G4VSensitiveDetector* beamPipeSD = new GG4ModuleSD(logicalBeamPipe) ;
//		G4SDManager::GetSDMpointer()->AddNewDetector(beamPipeSD) ;
//		logicalBeamPipe->SetSensitiveDetector(beamPipeSD) ; // induces error
// make sensitive internal vacuum ...
//		G4VSensitiveDetector* vacuumSD = new GG4ModuleSD(logicalVacuum) ;
//		G4SDManager::GetSDMpointer()->AddNewDetector(vacuumSD) ;
//		logicalVacuum->SetSensitiveDetector(vacuumSD) ;
// placement ...
		new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalBeamPipe,logicalBeamPipe->GetName(),logicalWorld,false,0,checkOverlaps) ;
		new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalVacuum,logicalVacuum->GetName(),logicalWorld,false,0,checkOverlaps) ;
		new G4PVPlacement(0,G4ThreeVector(0,0,0),logicalBeam,logicalBeam->GetName(),logicalWorld,false,0,checkOverlaps) ;
		}
	
// absorber layer ...
// ==================
	// solid ...
	solidAbs = config->getG4Tubs("X.absorber") ;
	if (solidAbs) {
		halfZAbs = ((G4Tubs *) solidAbs)->GetZHalfLength() ;
		visAbs = new G4VisAttributes(G4Colour(0.8, 0.1, 0.0)) ;
			visAbs->SetVisibility(true) ;
			visAbs->SetForceSolid(true) ;
		G4double stepLimiter = 100.0 * CLHEP::mm ;
		UserLimits_abs = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,minEnergyCut,0) ;
	// matertial ...
		material = GetMaterial("X.absorberMaterial","Tungsten") ;
	// logical ...
		logicalAbs0 = new G4LogicalVolume(solidAbs,material,"abs.0") ;
		logicalAbs0->SetVisAttributes(visAbs) ;
		logicalAbs0->SetUserLimits(UserLimits_abs) ;

		logicalAbs1 = new G4LogicalVolume(solidAbs,material,"abs.1") ;
		logicalAbs1->SetVisAttributes(visAbs) ;
		logicalAbs1->SetUserLimits(UserLimits_abs) ;
	// sensitive ...
		G4VSensitiveDetector* a0 = new GG4ModuleSD(logicalAbs0) ;
		G4SDManager::GetSDMpointer()->AddNewDetector(a0) ;
		logicalAbs0->SetSensitiveDetector(a0) ;

		G4VSensitiveDetector* a1 = new GG4ModuleSD(logicalAbs1) ;
		G4SDManager::GetSDMpointer()->AddNewDetector(a1) ;
		logicalAbs1->SetSensitiveDetector(a1) ;

		}

// sensitive layer ...
// ===================
	// solid ...
	G4double sectorAngle = 10.0 ; // possible angles: 4(90), 5(72), 6(60), 8(45), 9(40), 10(36),12(30), 15(24) [deg]
	G4int numSectors = int(360.01/sectorAngle) ;
	sectorAngle *= CLHEP::deg ;
	G4double parSens[3] = {50*CLHEP::mm , 200*CLHEP::mm , 10*CLHEP::mm} ;
	solidSens = 0 ;
	if (!config->getDouble("G4Tubs.X.sensitive",parSens,3)) {
		solidSens = new G4Tubs("X.sensitive",parSens[0],parSens[1],parSens[2]/2,0,sectorAngle) ;
		halfZSens = parSens[2]/2.0 ;
		visSens = new G4VisAttributes(G4Colour(0.4, 0.7, 0.9, 0.8)) ;
			visSens->SetVisibility(true) ;
			visSens->SetForceSolid(true) ;
		G4double stepLimiter = 100.0 * CLHEP::mm ;
		UserLimits_sens = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,minEnergyCut,0) ;
	// material ...
		material = GetMaterial("X.sensitiveMaterial","G4_PLASTIC_SC_VINYLTOLUENE") ;
	// logical ...
		logicalSens0 = new G4LogicalVolume(solidSens,material,"sens.0") ;
		logicalSens0->SetVisAttributes(visSens) ;
		logicalSens0->SetUserLimits(UserLimits_sens) ;
	// sensitive ...
		G4VSensitiveDetector* s0 = new GG4ModuleSD(logicalSens0) ;
		G4SDManager::GetSDMpointer()->AddNewDetector(s0) ;
		logicalSens0->SetSensitiveDetector(s0) ;
	// logical ...
		logicalSens1 = new G4LogicalVolume(solidSens,material,"sens.1") ;
		logicalSens1->SetVisAttributes(visSens) ;
		logicalSens1->SetUserLimits(UserLimits_sens) ;
	// sensitive ...
		G4VSensitiveDetector* s1 = new GG4ModuleSD(logicalSens1) ;
		G4SDManager::GetSDMpointer()->AddNewDetector(s1) ;
		logicalSens1->SetSensitiveDetector(s1) ;
		}

// calorimeter ...
// ===============
	// material ...
	material = GetMaterial("X.calorimeterMaterial","LYSO") ;
	G4String fourBlocksOn = "off" ;
	// solid ...
	solidCalor = config->getG4Tubs("X.calorimeter") ;
	if (solidCalor) {
		halfZCalor = ((G4Tubs *) solidCalor)->GetZHalfLength() ;
		visCalor = new G4VisAttributes(G4Colour(0.0, 0.7, 0.7)) ;
				visCalor->SetVisibility(true) ;
				visCalor->SetForceSolid(true) ;
		UserLimits_calor = new G4UserLimits(100.0 * CLHEP::mm,1000*CLHEP::mm,100*CLHEP::ns,minEnergyCut,0) ;
	// logical ...
		logicalCalor = new G4LogicalVolume(solidCalor,material,"calor.O") ;
		logicalCalor->SetVisAttributes(visCalor) ;
		logicalCalor->SetUserLimits(UserLimits_calor) ;
		}

	else if (!config->getString("FB.calorimeter",fourBlocksOn) && fourBlocksOn == "on") {
		logicalCalor = FBcalorimeter.ConstructLogical("calor.F",material) ;
		}
	else {
/*
	add here ...
		G4double minRad = 60.0 * CLHEP::mm , maxRad = 220.0 * CLHEP::mm ;
		config->getDouble("cal.min_radius",minRad) ;
		config->getDouble("cal.max_radius",maxRad) ;

		G4double sx = 15.0*CLHEP::mm , sy = 15.0 * CLHEP::mm , sz = 100.0 * CLHEP::mm ;
		G4VSolid *solid = config->getG4Box("crystal") ;
		sx = 2.0*((G4Box *) solid)->GetXHalfLength() ;
		sy = 2.0*((G4Box *) solid)->GetYHalfLength() ;
		sz = 2.0*((G4Box *) solid)->GetZHalfLength() ;
*/
		logicalCalor = calorimeter.ConstructLogical("calor.M",15.0,15.0,100.0,  60.0,220.0,   material) ;
		// ==================== get sizes and radiuses from config =============================== ^
		}
// make calorimeter sensitive ...
	G4VSensitiveDetector* cc = new GG4ModuleSD(logicalCalor) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(cc) ;
	logicalCalor->SetSensitiveDetector(cc) ;


// positioning ...
// ===============
	G4RotationMatrix *RS = 0 ;
	G4double x = 0 , y = 0 ; // positioning coordinates ...
	G4double z = 400*CLHEP::mm ;     // default distance of the first volume ...
	config->getDouble("X.distance.z",z) ;
// absorber 0 ...
	if (solidAbs) {
		z += halfZAbs ;
		new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalAbs0,logicalAbs0->GetName(),logicalWorld,false,0,checkOverlaps) ;
		z += halfZAbs ;
		z += 2.0 * CLHEP::mm ;
		}
// scintillator 0 ...
	if (solidSens) {
		z += halfZSens ;
		for (int k = 0 ; k < numSectors ; k++) {
			RS = new G4RotationMatrix ;
			RS->rotateZ(-sectorAngle*k - sectorAngle/2.0) ;// NEGATIVE. 'cause system rotates not the object ...
			new G4PVPlacement(RS,G4ThreeVector(x,y,z),logicalSens0,logicalSens0->GetName(),logicalWorld,false,k,checkOverlaps) ;
			}
		z += halfZSens ;
		z += 2.0 * CLHEP::mm ;
		}
// absorber 1 ...
	if (solidAbs) {
		z += halfZAbs ;
		new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalAbs1,logicalAbs1->GetName(),logicalWorld,false,1,checkOverlaps) ;
		z += halfZAbs ;
		z += 2.0 * CLHEP::mm ;
		}
// scintillator 1 ...

	if (solidSens) {
		z += halfZSens ;
		for (int k = 0 ; k < numSectors ; k++) {
			RS = new G4RotationMatrix ;
			RS->rotateZ(-sectorAngle*k - sectorAngle/2.0) ;
			new G4PVPlacement(RS,G4ThreeVector(x,y,z),logicalSens1,logicalSens1->GetName(),logicalWorld,false,k,checkOverlaps) ;
			}
		z += halfZSens ;
		z += 2.0 * CLHEP::mm ;
		}

// calorimeter ...
	z += 15.0 * CLHEP::mm ;
	cout << fixed << showpoint << setprecision(2) ;
	cout << endl << "X2_Detector::EdmCalorimeter: frontZPosition: " << z << endl ;
	if (solidCalor) {
		z += halfZCalor ;
		new G4PVPlacement(0,G4ThreeVector(x,y,z),logicalCalor,logicalCalor->GetName(),logicalWorld,false,0,checkOverlaps) ;
		z += halfZCalor ;
		z += 2.0 * CLHEP::mm ;
		}
	if (logicalCalor->GetName() == "calor.F") FBcalorimeter.Build(logicalWorld,z,2,2) ;
	else calorimeter.Build(logicalWorld,z) ;
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

	cout << endl << "X2_Detector: geometry and SD generated ... " << endl << endl ;

	delete config ;

	return physicalWorld ;

	}

G4Material *X2_Detector::GetMaterial(const char *unitName,const char *defaultMaterialName) {
	G4String materialName ;
	G4Material *xmaterial = 0 ;
	if (!config->getString(unitName,materialName)) xmaterial = nistManager->FindOrBuildMaterial(materialName) ;
	if (!xmaterial) xmaterial = nistManager->FindOrBuildMaterial(defaultMaterialName) ;
	if (!xmaterial) {
		cerr << "    GetMaterial: " << defaultMaterialName << " not found or built ... " << endl ;
		abort() ;
		}
	if (config->getVerboseLevel() > 0) cout << "    GetMaterial: [" << unitName << "] \"" << xmaterial->GetName() << "\" ... ok " << endl ;
	return xmaterial ;
	}


void X2_Detector::ConstructMaterials() {
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
