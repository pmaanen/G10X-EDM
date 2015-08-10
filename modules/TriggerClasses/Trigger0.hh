// File:	Trigger0.hh
// Date:	01-Feb-2015 G.Macharashvili Dubna

//==========================================================
// This trigger accepts all events ...
//==========================================================

#ifndef Trigger0_hh
#define Trigger0_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <VirtualTrigger.hh>

class Trigger0 : public VirtualTrigger {

static	Trigger0	*ptr ;

		Trigger0() ;
virtual		~Trigger0() ;

public:

static	void	*get() { return (ptr) ? ptr : ptr = new Trigger0 ; }

// called at G4UserAction::BeginOfEventAction ...
virtual	void	initTrigger(const G4Event*) { ; }

// called at G4UserAction::EndOfEventAction ...
virtual	G4bool	amygdala(const G4Event*) { return false ; }
virtual	G4bool	cortex(const G4Event*,int &trig_value = *(int *)0) { return true ; }

	} ;

#endif

// eof
