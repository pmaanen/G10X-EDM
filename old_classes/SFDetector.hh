// File:	SFDetector.hh
// Date:	18-Jul-2008 G.Macharashvili Dubna

#ifndef SFDetector_hh
#define SFDetector_hh 1

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


class SFDetector : public G4VUserDetectorConstruction {

public:

				SFDetector() ;
virtual				~SFDetector() ;

	void			ConstructMaterials() ;
	G4VPhysicalVolume*	Construct() ;
	G4VPhysicalVolume*	World() const { return physicalWorld ; }

protected: 

	VolumeGenerator		*config ;
	G4double		par[12] , z_pos ;

	G4int			numberOfModules ;
	G4double		calTotalVolume ;
	G4VSolid		*solid ;
	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;
	G4NistManager		*nistManager ;
	G4Material		*StainlessSteel , *Scintillator , *LYSO ;
	G4bool			checkOverlaps ;
	G4LogicalVolume		*logicalWorld ;
	G4VPhysicalVolume	*physicalWorld ;

	} ;

#endif

// e-o-f
