// File:	AnkeG4TriggerFd.hh
// Date:	17-Oct-2012 G.Macharashvili Juelich

#ifndef AnkeG4TriggerFd_hh
#define AnkeG4TriggerFd_hh 1

#include <GG4VirtualTrigger.hh>

#include <ctime> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4Timer.hh>
#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4HCofThisEvent.hh>
#include <GG4TrackHit.hh>

class AnkeG4TriggerFd : public GG4VirtualTrigger {

public:

virtual	void	*get() { return new AnkeG4TriggerFd ; }
virtual	void	init_trigger(const G4Event*) ;
virtual	G4bool	amygdala(const G4Event*) ;
virtual	G4bool	cortex(const G4Event*,int &trig_value = *(int *)0) ;

protected:

	G4HCofThisEvent		*HCE ;

	} ;

#endif

// eof
