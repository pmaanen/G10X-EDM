// File:	AnkeG4Trigger4Coef.hh
// Date:	13-Mar-2012 G.Macharashvili Juelich
// Date:	15-Oct-2012 G.Macharashvili Juelich

#ifndef AnkeG4Trigger4Coef_hh
#define AnkeG4Trigger4Coef_hh 1

#include <GG4VirtualTrigger.hh>

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
#include <GG4TrackHit.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

class AnkeG4Trigger4Coef : public GG4VirtualTrigger {

	AnkeG4Trigger4Coef() ;

public:

virtual	void	*get() { return new AnkeG4Trigger4Coef ; }

// called from the derived classes at the BeginOfEventAction ...
virtual	void	init_trigger(const G4Event*) ;

// called from the derived classes at the EndOfEventAction ...
virtual	G4bool	amygdala(const G4Event*) ;
virtual	G4bool	cortex(const G4Event*,int &trig_value = *(int *)0) ;

protected:

	G4HCofThisEvent		*HCE ;
	TrackHitsCollection	*HCd2plate , *HCFdWindow ;
	TH2F			*d2plateXY , *FdWindowXY ;

	} ;

extern	TFile	*gRFile ;

#endif

// eof
