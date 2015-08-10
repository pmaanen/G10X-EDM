// File:	TH2FSelector.cc
// Date:	28-Oct-2013 G.Macharashvili Dubna

#include "TH2FSelector.hh"

using namespace TMath ;

TH2FSelector::~TH2FSelector() {
	if (MaxValueX) delete [] MaxValueX ;
	}

TH2FSelector::TH2FSelector() {
	MaxValue = 1.e+32 ;
	h = 0 ;
	MaxValueX = 0 ;
	}

void	TH2FSelector::Init(const char *xdir,const char *xname) {
	string fname = xdir ;
	fname += "/TH2F_" ;
	fname += xname ;
	fname += ".root" ;
	TFile *rootFile = TFile::Open((fname).c_str()) ;
	if (!rootFile) {
		cout << "TH2FSelector::Init() file \"" << fname << "\" cannot open ... " << endl ;
		}
	else rootFile->GetObject(xname,h) ;
	if (!h) {
		cout << "TH2FSelector::Init() file \"" << fname << "\" does not contain hist \"" << xname << "\" ... NO SELECTION ..." << endl ;
		}
	else {
		h->SetDirectory(0) ;
//
		if (h->GetRMS(1) > 0.0 && h->GetRMS(2) > 0.0) MaxValue = h ->Integral("width")/(2.0*h->GetRMS(1))/(2.0*h->GetRMS(2)) ;
		else MaxValue = h->GetBinContent(h->GetMaximumBin()) ;
//
		MaxValueX = new float[h->GetNbinsX()] ;
		TH1D *hy = 0 ;
		for (int b = 1 ; b <= h->GetNbinsX() ; b++) {
			hy = h->ProjectionY("tmp_y",b,b) ;
			hy->SetDirectory(0) ;
			if (hy && hy->GetRMS() > 0.0) MaxValueX[b-1] = hy->Integral("width")/(2.0*hy->GetRMS()) ;
			else MaxValueX[b-1] = 0.0 ;
			}
		if (!hy) hy->Delete() ;
		}
	if (rootFile) {
		rootFile->Close() ;
		rootFile->Delete() ;
		}
	}

bool	TH2FSelector::IsAboveLevel(double x,double y,double level) {
	if (!h) return true ;
	if (x < h->GetXaxis()->GetXmin() || x > h->GetXaxis()->GetXmax()) return false ;
	if (y < h->GetYaxis()->GetXmin() || y > h->GetYaxis()->GetXmax()) return false ;
	return (h->GetBinContent(h->FindBin(x,y))/MaxValue > level) ;
	}

bool	TH2FSelector::IsAboveLevelX(double x,double y,double level) {
	if (!h) return true ;
	if (x < h->GetXaxis()->GetXmin() || x > h->GetXaxis()->GetXmax()) return false ;
	if (y < h->GetYaxis()->GetXmin() || y > h->GetYaxis()->GetXmax()) return false ;
	int b = h->GetXaxis()->FindBin(x) - 1 ;
	if (MaxValueX[b] <= 0.0) return false ;
	return (h->GetBinContent(h->FindBin(x,y))/MaxValueX[b] > level) ;
	}

int	TH2FSelector::Save(const char *xdir,TH2F *xh) {
	string fname = xdir ;
	fname += "/TH2F_" ;
	fname += xh->GetName() ;
	fname += ".root" ;
	TFile *rootFile = TFile::Open(fname.c_str(),"recreate") ;
	if (!rootFile) {
		cout << "TH2FSelector::Save() file \"" << fname << "\" cannot open ..." << endl ;
		exit(0) ;
		}
	int state = xh->Write() ;
	rootFile->Close() ;
	rootFile->Delete() ;
	return state ;
	}

// e-o-f
