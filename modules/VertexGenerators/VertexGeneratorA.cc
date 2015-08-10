// File:	VertexGeneratorA.cc
// Date:	01-Feb-2015 G.Macharashvili Dubna

#include "VertexGeneratorA.hh"

void	*VertexGeneratorA::ptr = 0 ;

	VertexGeneratorA::VertexGeneratorA() : VVertexGenerator() {
        }

void*	VertexGeneratorA::get() { return (ptr) ? ptr : ptr = new VertexGeneratorA ; }

G4ThreeVector	VertexGeneratorA::generateVertex() {
       	x = y = z = 0 ;
// parabolic x distributions ...
	if (sx > 0) { do { x = 2*CLHEP::RandFlat::shoot() - 1 ; } while (CLHEP::RandFlat::shoot() > 1.0 - x*x) ; }
	qx = sqrt(1.0 - x*x) ;
	x *= sx/2 ;
// parabolic y distributions ...
        if (sy > 0) { do { y = 2*CLHEP::RandFlat::shoot() - 1 ; } while (CLHEP::RandFlat::shoot() > 1.0 - y*y) ; }
	qy = sqrt(1.0 - y*y) ;
	y *= qx ;
	y *= sy/2 ;
// parabolic z distributions ...
        if (sz > 0) { do { z = 2*CLHEP::RandFlat::shoot() - 1 ; } while (CLHEP::RandFlat::shoot() > 1.0 - z*z) ; }
	z *= qx*qy ;
	z *= sz/2 ;
// set ...
	v.set(x,y,z) ;
	return v ;
	}
	
// e-o-f
