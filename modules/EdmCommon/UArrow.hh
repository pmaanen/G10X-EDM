// File:	UArrow.hh
// Date:	17-Jul-2006 G.M. Juelich

#ifndef UArrow_h
#define UArrow_h 1

#include <G4Polycone.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Material.hh>

class UArrow {

public:

	UArrow(G4double Len,G4LogicalVolume *MotherVol,G4ThreeVector Vc = G4ThreeVector(0,0,0)) ;

	} ;

#endif

// eof
