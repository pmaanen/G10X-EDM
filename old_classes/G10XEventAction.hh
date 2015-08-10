// File:	G10XEventAction.hh
// Date:	27-Jun-2014 GM Dubna


#ifndef G10XEventAction_h
#define G10XEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

class G10XEventAction : public G4UserEventAction {

public:

	G10XEventAction();
virtual ~G10XEventAction() ;
    
virtual	void BeginOfEventAction(const G4Event*) ;
virtual	void EndOfEventAction(const G4Event*) ;
    
private:

    } ;

#endif

// e-o-f
