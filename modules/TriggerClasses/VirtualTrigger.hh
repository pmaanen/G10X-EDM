// File:	VirtualTrigger.hh
// Date:	01-Feb-2015 G.Macharashvili Dubna

#ifndef VirtualTrigger_hh
#define VirtualTrigger_hh 1

#include <G4Event.hh>

class VirtualTrigger {

public:

		VirtualTrigger() ;
virtual		~VirtualTrigger() ;

virtual	void	initTrigger(const G4Event*) = 0 ;
// sometimes it is needed to initialize environment ...

virtual	G4bool	amygdala(const G4Event*) { return false ; }
// if returns "true" - event rejected independent of "cortex" value ...

virtual	G4bool	cortex(const G4Event*,int &trig_value = *(int *)0) { return true ; }
// if returns "true" - event accepted and stored in the output file. 
// trig_value if used returns a trigger type which will be saved at the event header.
// different trigger types can be used for different topology.

	} ;

#endif

// e-o-f
