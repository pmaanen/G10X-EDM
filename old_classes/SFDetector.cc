// File:	SFDetector.cc
// Date:	18-Jul-2008 G.Macharashvili Dubna

#include "SFDetector.hh"

#include <string.h>
#include <iostream>
#include <sstream>

#include <G4SystemOfUnits.hh>
#include <G4PVReplica.hh>

#include <UArrow.hh>
#include <Ruller.hh>
#include "G10XModuleSD.hh"
#include "GG4TrackModuleSD.hh"
#include "BeamPipe.hh"

SFDetector::SFDetector() {
	checkOverlaps = false ;
	
	}

SFDetector::~SFDetector() {
	cout << endl ;
	cout << "SFDetector::dtor ..." << endl ;
	cout << "    calorimeter consists of " << numberOfModules << " modules " << endl ;
	cout << "    total volume:  " << calTotalVolume/CLHEP::cm3 << " [cm3] " << endl ;
	cout << "    total price:   " << calTotalVolume/CLHEP::cm3 * 50 << " $ " << endl ;
	cout << endl ;
	cout << endl ;

	}

G4VPhysicalVolume*	SFDetector::Construct() {

	ConstructMaterials();

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
// calorimeter ...
	G4double x = 0 , y = 0 , r = 0 ;
	G4double z = 400*CLHEP::mm ;
	config->getDouble("cal.distance.z",z) ;
// calorimeter module material ...
	G4Material *material = LYSO ;
	G4String materialName ;
	if (!config->getString("cal.material",materialName)) {
		if (materialName.contains("cint")) material = Scintillator ;
		else if (materialName.contains("tainles")) material = StainlessSteel ;
		else material = LYSO ;
		}
// calorimeter module solid ...
	solid = config->getG4Box("crystal") ;
	G4double sx = 2.0*((G4Box *) solid)->GetXHalfLength() ;
	G4double sy = 2.0*((G4Box *) solid)->GetYHalfLength() ;
	G4double sz = 2.0*((G4Box *) solid)->GetZHalfLength() ;

	G4VisAttributes	*VSi = new G4VisAttributes(G4Colour(0.0, 0.7, 0.1)) ;
			VSi->SetVisibility(true) ;
			VSi->SetForceSolid(true) ;
	G4double stepLimiter = 100.0 * CLHEP::mm ;
	G4UserLimits	*UserLimits_LYSO = new G4UserLimits(stepLimiter,1000*CLHEP::mm,100*CLHEP::ns,0,0) ;
	logical = new G4LogicalVolume(solid,material,"crystal") ;
	logical->SetVisAttributes(VSi) ;
	logical->SetUserLimits(UserLimits_LYSO) ;
// positioning ...
	G4double minRad = 16.0 , maxRad = 200.0 ;
	config->getDouble("cal.min_radius",minRad) ;
	config->getDouble("cal.max_radius",maxRad) ;
	numberOfModules = 0 ;
	G4int	nx = 40 , ny = 40 ;
	G4double xo = (nx%2) ? -nx/2*sx : -nx/2*sx + sx/2 ;
	G4double yo = (ny%2) ? -ny/2*sy : -ny/2*sy + sy/2 ;
	for (int i = 0 ; i < nx ; i++) {
		x = xo + sx * i ;
		for (int j = 0 ; j < ny ; j++) {
			y = yo + sy * j ;
			r = sqrt(x*x + y*y) ;
			if (r > maxRad) continue ;
			if (r < minRad) continue ;
			stringstream name(ios::out) ;
			name.seekp (0,ios::beg) ;
			name << "module_" << i << "." << j ;
			G4int index = i * 1000 + j ;			
			new G4PVPlacement(0,G4ThreeVector(x,y,z + sz/2),logical,name.str(),logicalWorld,false,index,checkOverlaps) ;
			numberOfModules++ ;
			}
		}
	calTotalVolume = numberOfModules * sx * sy * sz ;

// make sensitive the logical volume ...
//-+	new G10XSiModuleSD(logical->GetName(),logical) ;
	new GG4TrackModuleSD(logical->GetName(),logical) ;

/***
// COSY vacuum window ...
// ======================
	solid = config->getG4Tubs("cosy_window") ;
	G4VisAttributes	*VSi2 = new G4VisAttributes(G4Colour(0.7, 0.3, 0.7, 0.8)) ;
			VSi2->SetVisibility(true) ;
			VSi2->SetForceSolid(true) ;
	logical = new G4LogicalVolume(solid,G4Material::GetMaterial("G4_STAINLESS-STEEL"),"cosy_window") ;
	logical->SetVisAttributes(VSi2) ;
	z_pos = z - 4.0*CLHEP::cm ;
	new G4PVPlacement(0,G4ThreeVector(0,0,z_pos),logical,logical->GetName(),logicalWorld,false,0,checkOverlaps) ;
	new G10XModuleSD(logical->GetName(),logical) ;
***/

// sectored scintillator counter ...
// =================================
	solid = config->getG4Tubs("phi_scint") ;
	G4VisAttributes	*VSi3 = new G4VisAttributes(G4Colour(0.3, 0.5, 0.9, 0.8)) ;
			VSi3->SetVisibility(true) ;
			VSi3->SetForceSolid(true) ;
	logical = new G4LogicalVolume(solid,G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE"),solid->GetName()) ;
	logical->SetVisAttributes(VSi3) ;
	z_pos = z - 2.0*CLHEP::cm ;
	physical = new G4PVPlacement(0,G4ThreeVector(0,0,z_pos),logical,logical->GetName(),logicalWorld,false,0,checkOverlaps) ;
	new G10XModuleSD(logical->GetName(),logical) ;


// draw beam pipe ...
// ==================
	BeamPipe *bp = new BeamPipe(StainlessSteel) ;
	new G4PVPlacement(0,G4ThreeVector(0,0,0),bp->GetLogical(),bp->GetLogical()->GetName(),logicalWorld,false,0) ;
	


/** replicated on phi ... ????
	G4double phi_sector = 6.0 ;
	G4int phi_divisions = int(360.0 / phi_sector) ;
	phi_sector = 360.0 / phi_divisions ;
	config->getDouble((G4String("G4Tubs.") + solid->GetName()).c_str(),par,3) ;
	G4VSolid *solid_sector = new G4Tubs("solid_sector",par[0],par[1],par[2]/2,0.0,phi_sector*CLHEP::deg) ;
	G4LogicalVolume *sector = new G4LogicalVolume(solid_sector,G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE"),"sector") ;
	new G4PVReplica("sector",sector,physical,kPhi,phi_divisions,phi_sector*CLHEP::deg,phi_sector*CLHEP::deg/2) ;
**/

// target ...
/**
	G4VisAttributes	*targetVis = new G4VisAttributes(G4Colour(1.0, 0.2, 0.2)) ;
			targetVis->SetVisibility(true) ;
			targetVis->SetForceSolid(true) ;
	G4VSolid	*targetSol = new G4Box("target",5*mm/2,5*mm/2,10*mm/2) ;
	G4LogicalVolume	*targetLog = new G4LogicalVolume(targetSol,G4Material::GetMaterial("Vacuum_High"),targetSol->GetName()) ;
			targetLog->SetVisAttributes(targetVis) ;
	new G4PVPlacement(0,G4ThreeVector(0,0,0),targetLog,targetLog->GetName(),logicalWorld,false,0,checkOverlaps) ;
	new G4PVPlacement(0,G4ThreeVector(0,0,z-10*mm),targetLog,targetLog->GetName(),logicalWorld,false,1,checkOverlaps) ;
**/

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

// end ...
	delete config ;

	return physicalWorld ;

	}

void SFDetector::ConstructMaterials() {
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
