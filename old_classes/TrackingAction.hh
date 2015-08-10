// File:	TrackingAction.hh
// Date:	03-mar-2015 G.Macharashvili Juelich

#ifndef TrackingAction_hh
#define TrackingAction_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <globals.hh>
#include <G4UserTrackingAction.hh>

#include "UDynamicManager.hh"

class TrackingAction : public G4UserTrackingAction {

// keeps a profile and traces of the primary deuteron track ...

public:  

	TrackingAction() ;
	~TrackingAction() ;
   
void	PreUserTrackingAction(const G4Track*) ;
void	PostUserTrackingAction(const G4Track*) ;
    
private:

protected:

	UDynamicManager		ListStopStatus ;

	} ;

#endif

// e-o-f
