// File:	VertexGeneratorA.hh
// Date:	01-Feb-2015 G.Macharashvili Dubna

#ifndef VertexGeneratorA_hh
#define VertexGeneratorA_hh 1

#include <iomanip> 
using namespace std ;

#include <G4ThreeVector.hh>
#include <Randomize.hh>

#include "VVertexGenerator.hh"

class VertexGeneratorA : public VVertexGenerator {

static	void	*ptr ;

private:

	VertexGeneratorA() ;

virtual	~VertexGeneratorA() {}

public:

static	void		*get() ;
virtual	G4ThreeVector	generateVertex() ;
virtual	void		print() { cout << "VertexGeneratorA: parabolic profile ..." << endl ; }

protected:

	G4double 	qx , qy ;

	} ;

#endif

// e-o-f
