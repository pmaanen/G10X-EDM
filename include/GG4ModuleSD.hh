// File:	GG4ModuleSD.hh
// Date:	10-Apr-2015 GM Juelich

#ifndef GG4ModuleSD_h
#define GG4ModuleSD_h 1

#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4SDManager.hh>
#include <G4VSensitiveDetector.hh>

#include <TFile.h>
#include <TH2.h>
#include <TH1.h>

#include "GG4ModuleHit.hh"

class GG4ModuleSD : public G4VSensitiveDetector {

public:

	GG4ModuleSD(G4LogicalVolume *log) ;
virtual	~GG4ModuleSD() ;
    
virtual	void			Initialize(G4HCofThisEvent*HCE) ;
virtual	G4bool			ProcessHits(G4Step* Step,G4TouchableHistory* ROhist) ;
virtual void			EndOfEvent(G4HCofThisEvent*) ;

private:

	GG4ModuleHit		*GetHit(int det_id) ;

protected:

	G4Track			*track ;
	G4LogicalVolume		*logicalVolume ;
	G4String		particleName ;

	G4int			detectorID ;
	G4int			postStepStatus ;
	G4int			moduleHcollectionID ;
	ModuleHitsCollection	*moduleHcollection ;
	GG4ModuleHit		*moduleH ;
	G4double		d_total_path ;

	TH1F			*e_0 , *e_1 , *e_2 , *tent , *partIndex , *path ;

private:

	void			HistInit() ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
