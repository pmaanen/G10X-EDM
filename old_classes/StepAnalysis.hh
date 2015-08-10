// File:	StepAnalysis.hh
// Date:	03-Mar-2015 G.Macharashvili Juelich

#ifndef StepAnalysis_h
#define StepAnalysis_h 1

#include <iostream>
#include <iomanip> 
using namespace std ;

#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "G4SteppingManager.hh"
#include "G4StepStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "UDynamicManager.hh"

class StepAnalysis {

public:

	StepAnalysis() ;
virtual	~StepAnalysis() ;

	void	Diagnostics(const G4Step*,const char *particle) ;
	int	EntryStatus() const { return stepEntryStatus ; }
	int	EndStatus() const { return stepEndStatus ; }

enum {              energyLoss = 0 , exitVolume ,       stopped ,       identityLoss } ;
// post step stat:  3                1                  3               4
//                  hIoni(3)         Transportation(1)  Ekin = 0.0(3)   dInelastic(4)

private:

	G4VPhysicalVolume	*pvol ;
	G4LogicalVolume		*lvol ;

const	G4Step			*step ;
const	G4StepPoint		*sp1 , *sp2 ;
	G4ThreeVector		p1 , p2 ;

	G4Track			*track ;

	G4VProcess		*proc ;
	G4String		procName ;
	G4int			procType ;

	UDynamicManager		ListPostStepProc ;
        UDynamicManager		ListStepStatus ;

	G4int			postStepStatus ;
	G4int			stepEntryStatus , stepEndStatus ;

	} ;

#endif

// e-o-f
