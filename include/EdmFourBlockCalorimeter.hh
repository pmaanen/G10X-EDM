// File:	EdmFourBlockCalorimeter.hh
// Date:	12-Jun-2015 G.Macharashvili Juelich

#ifndef EdmFourBlockCalorimeter_hh
#define EdmFourBlockCalorimeter_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4ThreeVector.hh>
#include <G4UserLimits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Box.hh>
#include <G4RotationMatrix.hh>
#include <G4AffineTransform.hh>

class EdmFourBlockCalorimeter {

public:

				EdmFourBlockCalorimeter() ;
virtual				~EdmFourBlockCalorimeter() ;

	G4LogicalVolume*	ConstructLogical(const char *name,G4Material *m) ;
	void			Build(G4LogicalVolume *logicalWorld,double z_pos,int NBradial,int NBtangential,double R1 = 50.0 * CLHEP::mm) ;
	G4String&		GetName() const { return className ; }

protected: 

static	G4String		className ;
	G4VSolid		*solid ;
	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;

	G4double		sx , sy , sz ;
	G4double		calTotalVolume ;
	G4int			numberOfModules ;

	} ;

#endif

// e-o-f
