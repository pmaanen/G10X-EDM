// File:	dataFileRW.cc
// Date:	20-Apr-2015 GM Dubna

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;







int main(int argc,char** argv) {

	double x[1000] , y[1000] , dummy ;
	ifstream in ;
	if (argc < 2) {
		cerr << endl << argv[0] << " file " << endl ;
		abort() ;
		}
	in.open(argv[1]) ;
	if (!in.is_open()) {
		cerr << endl << "dataFileRW: (F) ..." << endl ;;
		cerr         << "            " << argv[1] << endl ;
		cerr         << "            not exists. " << endl << endl ;
		abort() ;
		}
	int k = 0 ;
        do {
		in >> x[k] >> y[k] ;
		if (in.eof()) break ;
		} while (k++ < 1000) ;
// theta ...
	cout << "x[" << k << "] = {" ;
	cout << fixed << showpoint ;
	for (int i = 0 ; i < k ; i++) {
		cout << setw(6) << setprecision(2) << x[i] ;
		if (i < k - 1) cout << "," ;
		}
	cout << "} ;" << endl ;
// Ay, cross-sect, or enything elase ...
	cout << "y[" << k << "] = {" ;
	cout << fixed << showpoint ;
	for (int i = 0 ; i < k ; i++) {
		cout << setw(6) << setprecision(4) << y[i] ;
		if (i < k - 1) cout << "," ;
		}
	cout << "} ;" << endl ;

        in.close() ;
	return 0;

	}

// e-o-f
