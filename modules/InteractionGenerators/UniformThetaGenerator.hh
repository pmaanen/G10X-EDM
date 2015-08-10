// File:	UniformThetaGenerator.hh
// Date:	13-Apr-2015 G.Macharashvili Dubna

#ifndef UniformThetaGenerator_hh
#define UniformThetaGenerator_hh 1

#include <iomanip> 
using namespace std ;

#include <VInteractionGenerator.hh>

class UniformThetaGenerator : public VInteractionGenerator {

static	void	*ptr ;

private:

	G4double		kineticEnergy ;		// external. initialized with Init() ...
	G4ParticleDefinition	*particle ;		// external. initialized with Init() ...
	G4double		theta1 , theta2 ;	// external. initialized with Init() ...

private:

public:

	UniformThetaGenerator(const char * = "UniformThetaGenerator") ;
virtual	~UniformThetaGenerator() {}

virtual	void	GenerateInteraction() ;
virtual	void	Init(G4ParticleDefinition *particleDefinition = 0,G4double kineticEnergy = 0,G4double t1 = 0,G4double t2 = 0 ) ;
static	void	*get() ;

protected:

	} ;

#endif

// e-o-f
