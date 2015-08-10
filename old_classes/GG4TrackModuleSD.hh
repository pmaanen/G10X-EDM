// File:	GG4TrackModuleSD.hh
// Date:	01-Mar-2015 G.Macharashvili Juelich

#ifndef GG4TrackModuleSD_hh
#define GG4TrackModuleSD_hh 1

#include <iostream>
#include <iomanip> 
#include <vector>
using namespace std ;

#include <G4VSensitiveDetector.hh>
#include <G4HCofThisEvent.hh>
#include <G4SDManager.hh>
#include <G4EventManager.hh>
#include <G4Event.hh>

#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "GG4TrackModuleHit.hh"
#include "GG4ModuleHit.hh"
#include "StepAnalysis.hh"
#include "CalorimeterAnalysis.hh"

class G4Step ;
class G4HCofThisEvent ;
class G4TouchableHistory ;

class GG4TrackModuleSD : public G4VSensitiveDetector {

public:

	GG4TrackModuleSD(G4String,G4LogicalVolume *) ;
virtual	~GG4TrackModuleSD() ;

virtual	void		Initialize(G4HCofThisEvent*) ;
virtual	G4bool		ProcessHits(G4Step*,G4TouchableHistory*) ;
virtual	void		EndOfEvent(G4HCofThisEvent*) ;

virtual	void		Clear() {}
virtual	void		DrawAll() {}
virtual	void		PrintAll() {}

protected:

	G4LogicalVolume		*LogVol ;
const	G4VTouchable		*touchable ;
	G4AffineTransform	*topTransform ;
const	G4Event			*E ;
	G4int			eventID ;
	G4int			trackID ;
	G4int			detectorID ;

	G4int				hitsCollectionID ;
	TrackModuleHitsCollection	*hitsCollection ;
	GG4TrackModuleHit		*H , *V ;

	G4int				moduleHcollectionID ;
	ModuleHitsCollection		*moduleHcollection ;
	GG4ModuleHit			*mH ;

	StepAnalysis		stepAnalysis ;
	CalorimeterAnalysis	calorimeterAnalysis ;
	CalorimeterEntry	entry ;

        G4ProcessVector		*ProcVect ;
	G4VProcess		*Proc ;  
	G4ProcessType		ProcType ;  
	G4String		ProcTypeName , ProcName ;
	G4double		Eo , xo , yo ; // very first entry parameters ...


	TH1F			*elossH , *pathH , *num_hits , *activeMod ;
	TH1F			*eloss1 ;
	TH2F			*glob_xy ;

private:

	GG4TrackModuleHit	*GetHit(int trackID,int detectorID) ;
	GG4ModuleHit		*GetHit(int det) ;
	void			HistInit() ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
