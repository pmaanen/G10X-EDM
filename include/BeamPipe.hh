// File:	BeamPipe.hh
// Date:	18-Mar-2015 G.M. Juelich

#ifndef BeamPipe_h
#define BeamPipe_h 1

#include <G4Polycone.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Material.hh>
#include <globals.hh>

class BeamPipe {

public:

	BeamPipe(G4double = 0.0,G4double = 0.0) ;

	G4LogicalVolume*	BuildLogicalBeam(G4Material *) ;
	G4LogicalVolume*	BuildLogicalBeamPipe(G4Material*) ;
	G4LogicalVolume*	BuildLogicalVacuum(G4Material *) ;

protected:

	G4double		Zs[20] , Ri[20] , Ro[20] ;
	G4double		zw , wt ;

protected:

	void			Init() ;

	} ;

#endif

// e-o-f
