// File:	G10XEventSave.hh
// Date:	01-Feb-2015 G.Macharashvili Dubna

#ifndef G10XEventSave_hh
#define G10XEventSave_hh 1

#include <ctime> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4Timer.hh>
#include <G4Event.hh>
#include <G4UserEventAction.hh>
#include <G4EventManager.hh>
#include <G4HCofThisEvent.hh>
//#include <G10XTEvent.hh>

//#include <G10XCodesPDG.hh>
//#include <G10XDetectorIDCodes.hh>
#include <VirtualTrigger.hh>
//#include <GG4TrackHit.hh>

#include <TFile.h>

class G10XEventSave : public G4UserEventAction {

public:

		G10XEventSave() ;
virtual		~G10XEventSave() ;

virtual	void	BeginOfEventAction(const G4Event*) ;
virtual	void	EndOfEventAction(const G4Event*) ;

protected:

	G4HCofThisEvent		*HCE ;
//	G10XTEvent		*TE ;
//	G10XTPrimaryTrack 	*TP ;
//	G10XTHit		*TH ;
	VirtualTrigger		*trigger ;
	G4int			trig_value , accept ;
	G4Timer			*Timer ;
	G4double		utime ; // absolute time has to be sent to triggering routines ...

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
