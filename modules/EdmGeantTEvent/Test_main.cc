// File:	Test_main.cc
// Date:	19-Jun-2015 GM Juelich

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <TFile.h>
#include "EdmGeantTEvent.hh"

EdmGeantTEvent	evt ;

int main(int argc,char** argv) {

	TFile *gRFile = (TFile *) new TFile(argv[1]) ;
	evt.AttachInputFile(gRFile) ;
	evt.PrintProfile() ;

	for (int i = 0 ; i < evt.GetNumOfEntries() ; i++) {
        	evt.Get() ;
	        evt.Print() ;
	        }
        gRFile->Close() ;

	return 0;

	}

// e-o-f
