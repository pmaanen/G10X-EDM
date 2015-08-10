// File:	G10XEventAction.hh
// Date:	27-Jun-2014 GM Dubna


#ifndef G10XActionInitialization_h
#define G10XActionInitialization_h 1

#include "G4VUserActionInitialization.hh"


class G10XActionInitialization : public G4VUserActionInitialization {

public:

	G10XActionInitialization() ;
virtual ~G10XActionInitialization() ;

virtual void	BuildForMaster() const ;
virtual void	Build() const ;

	} ;

#endif

// e-o-f
