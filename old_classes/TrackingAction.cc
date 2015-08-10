// File:	TrackingAction.cc
// Date:	03-mar-2015 G.Macharashvili Juelich


#include <TrackingAction.hh>
#include <G4EventManager.hh>

TrackingAction::TrackingAction() {
	ListStopStatus.Reset() ;
	}

TrackingAction::~TrackingAction() {
	cout << "TrackingAction::dtor [deuteron track status] ..." << endl ;
	ListStopStatus.Print(cout,"    ") ;
	cout << endl ;
	}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
// track status types ...
const   string  TS[6] = { "Alive","AliveStop","StopKill","KillAll","Suspend","Postpone" } ;

	if (track->GetTrackID() != 1) return ;
	if (track->GetParticleDefinition()->GetParticleName() != "deuteron") return ;
	
	ListStopStatus.Register(TS[track->GetTrackStatus()].c_str()) ;
	}

// e-o-f
