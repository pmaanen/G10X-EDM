// File:	X_InteractionGenerator.hh
// Date:	23-Jan-2015 G.Macharashvili Dubna

#ifndef X_InteractionGenerator_h
#define X_InteractionGenerator_h 1

#include <G4ThreeVector.hh>
#include "VInteractionGenerator.hh"

class X_InteractionGenerator {

public:

	X_InteractionGenerator(const char * lib_name = 0,const char * generator_name = 0) ;
virtual	~X_InteractionGenerator() ;

protected:

	void			*handle ;
	VInteractionGenerator	*genFunct ;

public:

	VInteractionGenerator	*getClass() { return genFunct ; }
	void			generateInteraction() { return genFunct->GenerateInteraction() ; }

	} ;

#endif

// e-o-f
