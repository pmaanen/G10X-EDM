// File:	VVertexGenerator.hh
// Date:	23-Jan-2015 G.Macharashvili Dubna

#ifndef VVertexGenerator_h
#define VVertexGenerator_h 1

#include "G4ThreeVector.hh"
#include <globals.hh>

class VVertexGenerator {

public:

	VVertexGenerator() ;
virtual	~VVertexGenerator() ;

virtual	G4ThreeVector		generateVertex() = 0 ;
virtual	void			print() {}
virtual	void			setVariation(double _x,double _y,double _z) { sx = _x ; sy = _y ; sz = _z ; }

protected:

	G4ThreeVector		v ;
	G4double 		x , y , z ;
	G4double 		sx , sy , sz ;

	} ;

#endif

// e-o-f
