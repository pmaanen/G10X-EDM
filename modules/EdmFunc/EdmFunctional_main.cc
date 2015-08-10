// File:	dataFileRW.cc
// Date:	20-Apr-2015 GM Dubna

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <TFile.h>
#include <TH1.h>


#include "EdmFunctionalAy270.hh"

EdmFunctionalAy270	exp270 ;



int main(int argc,char** argv) {

	TFile *gRFile = (TFile *) new TFile("EdmFunctional_main.root","recreate") ;
	TH1F *h = new TH1F("Ay_270","",72,0.,36.) ;
	h->SetXTitle("#vartheta_{lab}    [deg]") ;
	h->SetYTitle("A_{y}") ;
	h->SetOption("e") ;
        for (int i = 0 ; i < 72 ; i++) {
		h->SetBinContent(i+1,exp270.Ay(0.5*i)) ;
		h->SetBinError(i+1,0.01) ;
		}
	gRFile->Write() ;
        gRFile->Close() ;

	return 0;

	}

// e-o-f
