// File:	VertexGeneratorO.hh
// Date:	23-Jan-2015 G.Macharashvili Dubna

#ifndef VertexGeneratorO_h
#define VertexGeneratorO_h 1

#include <iomanip> 
using namespace std ;

#include <G4ThreeVector.hh>

#include "VVertexGenerator.hh"

class VertexGeneratorO : public VVertexGenerator {

static	void	*ptr ;

private:

	VertexGeneratorO() : VVertexGenerator() { }
virtual	~VertexGeneratorO() { }

public:


static	void		*get() ;
virtual	G4ThreeVector	generateVertex() ;
virtual	void		print() { cout << "VertexGeneratorO: gauss ..." << endl ; }

	} ;

#endif

// e-o-f
