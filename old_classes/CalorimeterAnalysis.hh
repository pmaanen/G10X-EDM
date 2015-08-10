// File:	CalorimeterAnalysis.hh
// Date:	01-Mar-2015 G.Macharashvili Juelich

#ifndef CalorimeterAnalysis_hh
#define CalorimeterAnalysis_hh 1

#include <iostream>
#include <iomanip> 
#include <vector>
using namespace std ;

#include <G4HCofThisEvent.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "GG4ModuleHit.hh"
#include "GG4PhiFitter.hh"

struct	CalorimeterEntry		{ double E , x , y ; } ;

class CalorimeterAnalysis {

public:

	CalorimeterAnalysis() ;
virtual	~CalorimeterAnalysis() ;

	void		Initialize() ;
        void		ProcessEvent(ModuleHitsCollection* HC,CalorimeterEntry *entry) ;

protected:

	G4int		detectorID , ix , iy ;
	G4double	e3x3 , x3x3 , y3x3 , etot ;
	G4double	asymmetry_PA ;
	G4double	beamPolarizationPhi ;
	G4double	z_pos ;

	ModuleHitsCollection	*moduleHcollection ;
	GG4ModuleHit		*hit , *hmax , *hvar ;
	GG4PhiFitter		*phiFitter ;

	TH1F			*He3x3 , *Hx3x3 , *Hy3x3 , *activeM3x3 , *activeM ;
	TH1F			*Hetot , *Hstat ;
	TH1F			*Hd_tot , *Hd_stop , *Hd_kill , *Hd_unkn ;
	TH2F			*hit_xy , *HstatE ;
	TH1F			*Hd_phi , *Hd_thetaLab ;

private:

	GG4ModuleHit		*GetHit(int det) ;
	void			HistInit() ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
