// File:	X2_Detector.hh
// Date:	16-Apr-2015 G.Macharashvili Dubna

#ifndef X2_Detector_hh
#define X2_Detector_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4UserLimits.hh>
#include <G4ThreeVector.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Box.hh>
#include <G4RotationMatrix.hh>
#include <G4AffineTransform.hh>
#include <G4SubtractionSolid.hh>
#include <G4NistManager.hh>

#include <VolumeGenerator.hh>
#include "EdmModularCalorimeter.hh"
#include "EdmFourBlockCalorimeter.hh"


class X2_Detector : public G4VUserDetectorConstruction {

public:

				X2_Detector() ;
virtual				~X2_Detector() ;

	void			ConstructMaterials() ;
	G4Material*		GetMaterial(const char *unitName,const char *defaultMaterialName) ;
	G4VPhysicalVolume*	Construct() ;
	G4VPhysicalVolume*	World() const { return physicalWorld ; }

protected: 

	VolumeGenerator		*config ;
	G4double		z_pos ;

	G4VSolid		*solidAbs , *solidSens , *solidCalor ;
	G4double		halfZAbs , halfZSens , halfZCalor ;
	G4LogicalVolume		*logicalAbs0 , *logicalAbs1 ;
	G4LogicalVolume		*logicalSens0 , *logicalSens1 ;
	G4LogicalVolume		*logicalCalor ;
	G4VisAttributes		*visAbs , *visSens , *visCalor ;
	G4UserLimits		*UserLimits_abs , *UserLimits_sens , *UserLimits_calor ;

	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;

	G4NistManager		*nistManager ;
	G4Material		*material ;
	G4Material		*StainlessSteel , *Scintillator , *LYSO ;

	G4bool			checkOverlaps ;
	G4LogicalVolume		*logicalWorld ;
	G4VPhysicalVolume	*physicalWorld ;

	EdmModularCalorimeter	calorimeter ;
	EdmFourBlockCalorimeter	FBcalorimeter ;

	} ;

#endif

// e-o-f
