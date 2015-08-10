// File:	HomogenusDetector.hh
// Date:	17-Mar-2015 G.Macharashvili Juelich

#ifndef HomogenusDetector_hh
#define HomogenusDetector_hh 1

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


class HomogenusDetector : public G4VUserDetectorConstruction {

public:

				HomogenusDetector() ;
virtual				~HomogenusDetector() ;

	void			ConstructMaterials() ;
	G4VPhysicalVolume*	Construct() ;
	G4VPhysicalVolume*	World() const { return physicalWorld ; }

protected: 

	VolumeGenerator		*config ;
	G4double		par[12] , z_pos ;

	G4VSolid		*solid ;
	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;
	G4NistManager		*nistManager ;
	G4Material		*StainlessSteel , *Scintillator , *LYSO ;
	G4LogicalVolume		*logicalWorld ;
	G4VPhysicalVolume	*physicalWorld ;

	} ;

#endif

// e-o-f
