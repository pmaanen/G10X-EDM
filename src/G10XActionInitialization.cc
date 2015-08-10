// File:	G10XActionInitialization.cc
// Date:	27-Jun-2014 GM Dubna


#include "G10XActionInitialization.hh"

#include "G10XGenerator.hh"
#include "RunAction.hh"
//#include "G10XEventAction.hh"
#include "StackingAction.hh"

#include "GG4ModuleEventAction.hh"
#include "EdmGeantEventSave.hh"

G10XActionInitialization::G10XActionInitialization() : G4VUserActionInitialization() {}


G10XActionInitialization::~G10XActionInitialization() {}


void G10XActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction) ;
	}


void G10XActionInitialization::Build() const {
	SetUserAction(new G10XGenerator) ;
	SetUserAction(new RunAction) ;
	SetUserAction(new StackingAction) ;

//++	SetUserAction(new G10XEventAction) ;
//++	SetUserAction(new GG4ModuleEventAction) ;
	SetUserAction(new EdmGeantEventSave) ;
	}

// e-o-f
