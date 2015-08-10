// File:	VertexGeneratorO.cc
// Date:	23-Jan-2015 G.Macharashvili Dubna

#include "VertexGeneratorO.hh"
#include <Randomize.hh>

void	*VertexGeneratorO::ptr = 0 ;

void*	VertexGeneratorO::get() { return (ptr) ? ptr : ptr = new VertexGeneratorO ; }

G4ThreeVector	VertexGeneratorO::generateVertex() {
	if (sx > 0.0) x = CLHEP::RandGauss::shoot(0,sx) ;
	if (sy > 0.0) y = CLHEP::RandGauss::shoot(0,sy) ;
	if (sz > 0.0) z = CLHEP::RandGauss::shoot(0,sz) ;
	v.set(x,y,z) ;
	return v ;
	}
	
// e-o-f
