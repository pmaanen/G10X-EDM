// File:	G10XSiModuleSD.hh
// Date:	01-Jul-2014 GM Dubna

#ifndef G10XSiModuleSD_h
#define G10XSiModuleSD_h 1

#include <G4VSensitiveDetector.hh>
#include <G4ParticleChange.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "UDynamicManager.hh"
#include "G10XSiModuleHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class G10XSiModuleSD : public G4VSensitiveDetector {

public:

	G10XSiModuleSD(G4String name,G4LogicalVolume *logVolume = 0) ;
virtual	~G10XSiModuleSD() ;
    
virtual	void			Initialize(G4HCofThisEvent*HCE) ;
virtual	G4bool			ProcessHits(G4Step* S,G4TouchableHistory* ROhist) ;
virtual void			EndOfEvent(G4HCofThisEvent*) ;

private:

	G10XSiModuleHit*	GetHit(int trackID,int detectorID) ;
	void			InitHist() ;
//++??	void			GetHist() ;

protected:

	G4LogicalVolume*		logVolume ;
	G10XSiModuleHitsCollection	*HitsCollection ;
	G4int				HitsCollectionID ;
	G10XSiModuleHit			*hit ;
	G4int				trackID , detectorID , particleID ;
// processes registration/analysis ...
        G4ProcessVector			*ProcVect ;
	G4VProcess			*Proc ;  
	G4ProcessType			ProcType ;  
	G4String			ProcTypeName , ProcName ;
	UDynamicManager			ListPostStepProc ;
// step status ...
        UDynamicManager			ListStepStatus ;

	TH1F				*elossH , *pathH , *num_hits ;
	TH2F				*glob_xy ;
	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
