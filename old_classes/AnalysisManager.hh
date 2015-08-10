// File:	AnalysisManager.hh
// Date:	01-Jul-2014 GM Dubna

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector> 
using namespace std ;

#include "globals.hh"

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>

extern	char	*rootFileName ;


class AnalysisManager {

protected:

	AnalysisManager() ;

public:

static	AnalysisManager*	Instance() ;

virtual	~AnalysisManager() ;

	void	BeginOfRun() ;
	void	EndOfRun() ;

	int	H1(const char *dir,const char *name,int nx,double x1,double x2) ;
	TH1F*	getH1(int i) { return h1[i] ; }
	void	FillH1(int i,double x,double w = 1.0)  { h1[i]->Fill(x,w) ; }

	int	H2(const char *dir,const char *name,int nx,double x1,double x2,int ny,double y1,double y2) ;
	TH2F*	getH2(int i) { return h2[i] ; }
	void	FillH2(int i,double x,double y,double w = 1.0)  { h2[i]->Fill(x,y,w) ; }

//++	AnkeG4TEvent*	getRootEventPtr() { return rootEvent ; }
	TFile*		getRootFilePtr() { return rootFile ; }

private:

static	AnalysisManager*	self_pointer ;

//++	AnkeG4TEvent	*rootEvent ;
	TFile		*rootFile ;

	vector<TH1F*>		h1 ;
	vector<TH2F*>		h2 ;
	vector<TProfile*>	hp ;

	} ;

#endif

// e-o-f
