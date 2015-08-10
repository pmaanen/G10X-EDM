// File:	StackingAction.cc
// Date:	18-Mar-2015 G.Macharashvili Juelich

#include <iostream>
#include <iomanip> 
using namespace std ;

#include "StackingAction.hh"

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* T) {
	if (T->GetKineticEnergy() < 100 * CLHEP::keV) return fKill ;
	if (T->GetDefinition()->GetPDGCharge() == 0 && T->GetKineticEnergy() < 5 * CLHEP::MeV) return fKill ;
	return fUrgent ;
	}

// e-o-f
