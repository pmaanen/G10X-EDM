// File:	AnalysisManager.cc
// Date:	01-Jul-2014 GM Dubna

#include "AnalysisManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

	AnalysisManager*	AnalysisManager::self_pointer = 0 ;


AnalysisManager* AnalysisManager::Instance() { 
	if (0 == self_pointer) self_pointer = new AnalysisManager ;
	return self_pointer ;
	}


AnalysisManager::AnalysisManager() {}


AnalysisManager::~AnalysisManager() {}


int AnalysisManager::H1(const char *dir,const char *name,int nx,double x1,double x2) {
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;
	h1.push_back(new TH1F(name,name,nx,x1,x2)) ;
	return h1.size() - 1 ;
	}
 
int AnalysisManager::H2(const char *dir,const char *name,int nx,double x1,double x2,int ny,double y1,double y2) {
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;
	h2.push_back(new TH2F(name,name,nx,x1,x2,ny,y1,y2)) ;
	h2[h2.size() - 1]->SetOption("col") ;
	return h2.size() - 1 ;
	}
 
void AnalysisManager::BeginOfRun() { 
	rootFile = (TFile *) new TFile(rootFileName,"recreate") ;

//++	if (!rootEvent && rootFile) {
//++		rootEvent = new AnkeG4TEvent ;
//++		rootEvent->AttachOutputFile(rootFile) ;
//++		}

	}


void AnalysisManager::EndOfRun() {

	rootFile->Write() ;
	rootFile->Close() ;

	}

// e-o-f
