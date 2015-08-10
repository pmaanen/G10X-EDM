// File:	EdmModularCalorimeterAnalysis.hh
// Date:	28-May-2015 G.Macharashvili Juelich

#ifndef EdmModularCalorimeterAnalysis_hh
#define EdmModularCalorimeterAnalysis_hh 1

#include <iostream>
#include <iomanip> 
#include <vector>
using namespace std ;

#include <G4Event.hh>
#include <G4HCofThisEvent.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "GG4ModuleHit.hh"
#include "GG4PhiFitter.hh"

class EdmModularCalorimeterAnalysis {

public:

	EdmModularCalorimeterAnalysis() ;
virtual	~EdmModularCalorimeterAnalysis() ;

	void		Initialize() ;
virtual	void		ProcessEvent(const G4Event *evt,ModuleHitsCollection* HC) ;

protected:

	GG4PhiFitter		*phiFitter ;

	G4double		eLocal , xLocal , yLocal , eTotal ;
	G4double		asymmetry_PA ;
	G4double		beamPolarizationPhi ;
// config data ...
        G4double		sx , sy , sz ;
	G4double		frontZPosition ;

	ModuleHitsCollection	*moduleHcollection ;
	GG4ModuleHit		*hit , *hmax ;
	G4int			numActiveHits , numActiveLocal ;
	G4int			ix , iy ;
	G4int			ixMax , iyMax ;

	TH1F			*energyLocal , *posXLocal , *posYLocal , *activeMLocal , *activeM ;
	TH1F			*energyTotal ;
	TH2F			*posLocal ;
	TH1F			*phiLocal , *thetaLocal ;
	TH1F			*phiDiff , *thetaDiff ;

private:

	GG4ModuleHit		*GetHit(int ix,int iy) ;
	void			HistInit() ;
	G4double		getX(int i) ;
	G4double		getY(int i) ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
