// File:	PolarizationPhiGenerator.cc
// Date:	01-Feb-2015 G.Macharashvili Dubna

#include <Randomize.hh>

#include "PolarizationPhiGenerator.hh"

const	double	PolarizationPhiGenerator::Pi = acos(-1.0) ;
	double	PolarizationPhiGenerator::PA = 0 ;

PolarizationPhiGenerator::~PolarizationPhiGenerator () {
	}

PolarizationPhiGenerator::PolarizationPhiGenerator () {
	}

double	PolarizationPhiGenerator::F(double x) { return 1.0/Pi*(x + PA*sin(x)) ; }

double	PolarizationPhiGenerator::getRandomPhi(double xPA,double beamPolarizationPhi) {
	if (0.0 == xPA) return CLHEP::RandFlat::shoot(-Pi,Pi) ;
	PA = xPA ;
	double	xPhi = xeqt(0.0,Pi,CLHEP::RandFlat::shoot(),&F) ;
	xPhi = (CLHEP::RandFlat::shoot() > 0.5) ? xPhi : -xPhi ;
	xPhi += beamPolarizationPhi*Pi/180.0 ;
	while (xPhi > +Pi) xPhi = -2*Pi + xPhi ;
	while (xPhi < -Pi) xPhi = +2*Pi + xPhi ;
	return xPhi ;
	}

double	PolarizationPhiGenerator::xeqt(double a,double b,double y,double (*fun)(double)) {
	double s = -1. , x , funx ;
	if ((*fun)(a) <= (*fun)(b)) s = 1. ;
	if (s*y < s*(*fun)(a)) return 1.e+12 ;
	if (s*y > s*(*fun)(b)) return 1.e+12 ;
	do {
		x = 0.5*(a+b) ;
		funx = (*fun)(x) ;
		if (s*funx < s*y) a=x ;
		else b = x ;
		} while (fabs(y-funx) > 1e-5) ;
	return (0.5*(a+b)) ;
	}

// eof
