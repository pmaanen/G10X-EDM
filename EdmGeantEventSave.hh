// File:	EdmGeantEventSave.hh
// Date:	09-Apr-2011 G.Macharashvili Dubna

#ifndef EdmGeantEventSave_hh
#define EdmGeantEventSave_hh 1

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

#include <EdmGeantTEvent.hh>
#include <EdmGeantParticleDB.hh>

#include <TFile.h>

//#include <EdmGeantDetectorIDCodes.hh>
//#include <GG4VirtualTrigger.hh>
//#include <GG4TrackHit.hh>

class EdmGeantEventSave : public G4UserEventAction {

public:

		EdmGeantEventSave() ;
virtual		~EdmGeantEventSave() ;

virtual	void	BeginOfEventAction(const G4Event*) ;
virtual	void	EndOfEventAction(const G4Event*) ;

protected:

	G4HCofThisEvent		*HCE ;
	EdmGeantTEvent		*TE ;
	EdmGeantTTrack		*TP ;
	EdmGeantTHit		*TH ;
//++	GG4VirtualTrigger	*trigger ;
//++	G4int			trig_value , accept ;
	G4Timer			*Timer ;
	G4double		utime ; // absolute time has to be sent to triggering routines ...

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
