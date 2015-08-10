// File:	X_Detector.hh
// Date:	06-Apr-2015 G.Macharashvili Dubna

#ifndef X_Detector_hh
#define X_Detector_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

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


class X_Detector : public G4VUserDetectorConstruction {

public:

				X_Detector() ;
virtual				~X_Detector() ;

	void			ConstructMaterials() ;
	G4Material*		GetMaterial(const char *unitName,const char *defaultMaterialName) ;
	G4VPhysicalVolume*	Construct() ;
	G4VPhysicalVolume*	World() const { return physicalWorld ; }

protected: 

	VolumeGenerator		*config ;
	G4double		par[12] , z_pos ;

	G4VSolid		*solid ;
	G4double		halfZAbs , halfZSens , halfZCalor ;
	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;

	G4NistManager		*nistManager ;
	G4Material		*material ;
	G4Material		*StainlessSteel , *Scintillator , *LYSO ;

	G4bool			checkOverlaps ;
	G4LogicalVolume		*logicalWorld ;
	G4VPhysicalVolume	*physicalWorld ;

	} ;

#endif

// e-o-f
