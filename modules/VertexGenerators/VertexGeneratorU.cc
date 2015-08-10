// File:	VertexGeneratorU.cc
// Date:	09-Feb-2015 G.Macharashvili Dubna

#include "VertexGeneratorU.hh"
#include <Randomize.hh>

void*	VertexGeneratorU::ptr = 0 ;

void*	VertexGeneratorU::get() { return (ptr) ? ptr : ptr = new VertexGeneratorU ; }

G4ThreeVector	VertexGeneratorU::generateVertex() {
	if (sx > 0.0) x = CLHEP::RandFlat::shoot(-sx/2,sx/2) ;
	if (sy > 0.0) y = CLHEP::RandFlat::shoot(-sy/2,sy/2) ;
	if (sz > 0.0) z = CLHEP::RandFlat::shoot(-sz/2,sz/2) ;
	v.set(x,y,z) ;
	return v ;
	}
	
// e-o-f
