// File:	X_VertexGenerator.hh
// Date:	23-Jan-2015 G.Macharashvili Dubna

#ifndef X_VertexGenerator_h
#define X_VertexGenerator_h 1

#include <G4ThreeVector.hh>
#include "VVertexGenerator.hh"

class X_VertexGenerator {

public:

	X_VertexGenerator(const char * lib_name = 0,const char * generator_name = 0) ;
virtual	~X_VertexGenerator() ;

protected:

	void			*handle ;
	VVertexGenerator	*genFunct ;

public:

	VVertexGenerator	*getClass() { return genFunct ; }
	G4ThreeVector		generateVertex() { return genFunct->generateVertex() ; }

	} ;

#endif

// e-o-f
