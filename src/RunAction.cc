// File:	RunAction.cc
// Date:	01-Jul-2014 GM Dubna

#include "RunAction.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) { 
	Time = (G4Timer*) new G4Timer() ;
	Time->Start() ;
	long t = time(0) ;
	char s[30] ;
	strftime(s,29,"%A %k:%M %d-%b-%Y",localtime(&t)) ;
	cout << endl ;
	cout << "    RunAction: " << s << " " ;
	cout << endl ;
	cout << endl ;
	}

void RunAction::EndOfRunAction(const G4Run* run) {
	Time->Stop() ;
	cout << endl << fixed << showpoint ;
	cout << "    G10X (c) 26-May-2015 for Geant4.10.01.p01 ... EDM run completed ..." << endl ;
	cout << "    events processed: " << setprecision(0) << run->GetNumberOfEvent() << endl  ;
	cout << "    elapsed real time: " << setprecision(1) << Time->GetRealElapsed() << " sec" << endl ;
	cout << "    rate: " << setprecision(1) << run->GetNumberOfEvent()/Time->GetRealElapsed() << " evt/sec" << endl ;
	cout << "    ..." << endl ;
	delete Time ;
	}

// e-o-f
