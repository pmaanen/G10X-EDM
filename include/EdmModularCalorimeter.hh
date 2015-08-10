// File:	EdmModularCalorimeter.hh
// Date:	19-May-2015 G.Macharashvili Juelich

#ifndef EdmModularCalorimeter_hh
#define EdmModularCalorimeter_hh 1

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

class EdmModularCalorimeter {

public:

				EdmModularCalorimeter() ;
virtual				~EdmModularCalorimeter() ;

	G4LogicalVolume*	ConstructLogical(const char *name,double dx,double dy,double dz,double R1,double R2,G4Material *m) ;
	void			Build(G4LogicalVolume *logicalWorld,double z_pos,int numOfModules = 0,int *ix = 0,int *iy = 0) ;
	G4String&		GetName() const { return className ; }

protected: 

static	G4String		className ;
	G4VSolid		*solid ;
	G4LogicalVolume		*logical ;
	G4VPhysicalVolume	*physical ;

	G4double		sx , sy , sz ;
	G4double		minRad , maxRad ;
	G4double		calTotalVolume ;
	G4int			numberOfModules , nx , ny ;

	} ;

#endif

// e-o-f
