// File:	G10XModuleSD.hh
// Date:	11-mar-2015 GM Juelich

#ifndef G10XModuleSD_h
#define G10XModuleSD_h 1

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include <G4Event.hh>
//#include <G4EventManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4SDManager.hh>
#include <G4VSensitiveDetector.hh>

class G10XModuleSD : public G4VSensitiveDetector {

public:

	G10XModuleSD(G4String name,G4LogicalVolume *logVolume = 0) ;
virtual	~G10XModuleSD() ;
    
virtual	void			Initialize(G4HCofThisEvent*HCE) ;
virtual	G4bool			ProcessHits(G4Step* S,G4TouchableHistory* ROhist) ;
virtual void			EndOfEvent(G4HCofThisEvent*) ;

private:

	void			InitHist() ;

protected:

	G4Track			*track ;
	G4int			killed ;
	G4double		energyLoss ;
	TH1F			*h_edep , *h_edepS , *h_edepK ;
	TH1F			*h_phi , *h_thetaLab ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
