// File:	EdmPrimaryParticleInformation.hh
// Date:	29-May-2015 G.Macharashvili Juelich

#ifndef EdmPrimaryParticleInformation_hh
#define EdmPrimaryParticleInformation_hh 1

#include "G4VUserPrimaryParticleInformation.hh"

class EdmPrimaryParticleInformation : public G4VUserPrimaryParticleInformation {

public:

	EdmPrimaryParticleInformation(int s = 0,int t = 0) : G4VUserPrimaryParticleInformation() { status = s ; type = t ; }
  
inline	int	getStatus() const { return status ; }
inline	int	getType() const { return type ; }
	void	Print() const {}

private:

	int	status ; // ...
	int	type ; // ...

	} ;

#endif

// eof
