// File:	GG4ModuleEventAction.hh
// Date:	10-Apr-2014 GM Dubna

#ifndef GG4ModuleEventAction_hh
#define GG4ModuleEventAction_hh 1

#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TCutG.h>

#include <G4UserEventAction.hh>
#include <globals.hh>

#include <GG4ModuleHit.hh>

#include "EdmModularCalorimeterAnalysis.hh"

class GG4ModuleEventAction : public G4UserEventAction {

public:

	GG4ModuleEventAction();
virtual ~GG4ModuleEventAction() ;
    
virtual	void BeginOfEventAction(const G4Event*) ;
virtual	void EndOfEventAction(const G4Event*) ;
    
protected:

	G4int			eventID ;
	ModuleHitsCollection	*HC , *moduleH[20] ;
	G4int			numH[20] ;

	TH2F	*ee_0_1 , *ee_0_1_s , *ee_0_calor , *ee_1_calor ;
	TH1F	*ee_sel , *Xphi;
	TCutG	*ellipseCut01 ;
	
        EdmModularCalorimeterAnalysis	*calorAnalysis ;

private:

	void	HistInit() ;
	TCutG	*TCutGCirc(const char *name,double xc,double yc,double xr,double yr) ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
