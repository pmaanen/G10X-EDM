// File:	TH2FSelector.hh
// Date:	28-Oct-2013 G.Macharashvili Dubna

#ifndef TH2FSelector_hh
#define TH2FSelector_hh

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std ;

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>

class TH2FSelector {

protected:

	TH2F		*h ;
	double		MaxValue ;
	float		*MaxValueX ;

private:
	
public:

	TH2FSelector() ;
virtual	~TH2FSelector() ;

	TH2F		*GetHist() const { return h ; }
	int		Save(const char *dir,TH2F *xh) ;
	int		Save(TH2F *xh) { return Save(".",xh) ; }
	void		Init(const char *dir,const char *name) ;
	void		Init(const char *name) { return Init(".",name) ; }
	bool		IsAboveLevel(double x,double y,double level) ;
	bool		IsAboveLevelX(double x,double y,double level) ;

	} ;

#endif

// e-o-f
