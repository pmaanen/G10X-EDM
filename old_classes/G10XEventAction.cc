// File:	G10XEventAction.cc
// Date:	27-Jun-2014 GM Dubna


#include "G10XEventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include <stdio.h>
#include <iomanip> 
#include <ctime>
using namespace std ;

G10XEventAction::G10XEventAction() : G4UserEventAction() {
	G4RunManager::GetRunManager()->SetPrintProgress(0) ;
	}


G10XEventAction::~G10XEventAction() {}


void G10XEventAction::BeginOfEventAction(const G4Event*E) {
//--??	G4SDManager* sdManager = G4SDManager::GetSDMpointer() ;
	G4int EvtNum = E->GetEventID() + 1 ;
	if (!((EvtNum)%1000)) cout << setw(5) << EvtNum/1000 << "k ... \r" << flush ;
	}     


void G10XEventAction::EndOfEventAction(const G4Event* event) {
	G4HCofThisEvent* hce = event->GetHCofThisEvent() ;
	if (!hce) {
		G4ExceptionDescription msg ;
        	msg << "No hits collection of this event found.\n" ;
		G4Exception("G10XEventAction::EndOfEventAction()","code001",JustWarning, msg) ;
		return ;
		}

	}

// e-o-f
