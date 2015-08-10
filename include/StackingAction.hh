// File:	StackingAction.hh
// Date:	18-Mar-2015 G.Macharashvili Juelich

#ifndef StackingAction_H
#define StackingAction_H 1

#include <G4Track.hh>
#include <G4TrackStatus.hh>
#include <G4UserStackingAction.hh>

class StackingAction : public G4UserStackingAction {

public:

	StackingAction() {}
virtual	G4ClassificationOfNewTrack	ClassifyNewTrack(const G4Track *Track) ;

private:

	} ;

#endif

// e-o-f
