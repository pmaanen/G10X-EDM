// File:	Ruller.hh
// Date:	22-May-2009 G.Macharashvili Dubna

#ifndef Ruller_hh
#define Ruller_hh 1

#include <iostream>
#include <iomanip> 
using namespace std ;

#include <globals.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Box.hh>
#include <G4RotationMatrix.hh>

class Ruller {

public:

	Ruller(G4LogicalVolume *mother,const char *name,G4ThreeVector &p1,G4ThreeVector &p2,G4double dz = 10*mm) ;

	} ;

#endif

// eof
