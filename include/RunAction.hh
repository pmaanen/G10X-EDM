// File:	RunAction.hh
// Date:	01-Jul-2014 GM Dubna

#ifndef RunAction_hh
#define RunAction_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Run.hh"
#include "G4Timer.hh"
#include "G4EventManager.hh"

class RunAction : public G4UserRunAction {

public:

	RunAction() ;
virtual	~RunAction() ;

virtual	void	BeginOfRunAction(const G4Run*) ;
virtual	void	EndOfRunAction(const G4Run*) ;

protected:

	G4Timer*		Time ;

	} ;

#endif

// e-o-f
