// File:	Test_main.cc
// Date:	19-Jun-2015 GM Juelich

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <TFile.h>
#include <TH1.h>
#include "EdmGeantTEvent.hh"

EdmGeantTEvent	evt ;

int main(int argc,char** argv) {

	TFile *FW = (TFile *) new TFile("Test_output.root","recreate") ;
	TH1F *histSens0 = new TH1F("histSens0","",36,0,36) ;
	TFile *gRFile = (TFile *) new TFile(argv[1]) ;
	evt.AttachInputFile(gRFile) ;
	evt.Profile() ;

	for (int i = 0 ; i < evt.GetNumOfEntries() ; i++) {
        	evt.Get() ;
        	for (int k = 0 ; k < evt.GetNumberOfHits() ; k++) {
        	        EdmGeantTHit *h = evt.GetHit(k) ;
        	        if (evt.getProfile()->GetDetectorName(h->GetDetectorID()) == "sens.0") histSens0->Fill(h->GetModuleID()) ;
        	        
        	        }
        	for (int k = 0 ; k < evt.GetNumberOfTracks() ; k++) {
        	        }
//	        evt.Print() ;
	        }
        gRFile->Close() ;
        FW->Write() ;
        FW->Close() ;

	return 0;

	}

// e-o-f
