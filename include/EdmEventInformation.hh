// File:	EdmEventInformation.hh
// Date:	25-Jun-2015 G.Macharashvili Juelich

#ifndef EdmEventInformation_hh
#define EdmEventInformation_hh 1

#include "G4VUserEventInformation.hh"

class EdmEventInformation : public G4VUserEventInformation {

public:

	EdmEventInformation(int s = 0,int t = 0,int c = 0) : G4VUserEventInformation() { status = s ; type = t ; cycle = c ; }
  
inline	int	getStatus() const { return status ; }
inline	int	getType() const { return type ; }
inline	int	getCycle() const { return cycle ; }
	void	Print() const {} // necessary because the base class requires to define purely abstract Print ...

private:

	int	status ; // 0 == undef, 1 == dC -elastic, 2 == dC -inelastic, etc ...
	int	type , cycle ; // ...

	} ;

#endif

// eof
