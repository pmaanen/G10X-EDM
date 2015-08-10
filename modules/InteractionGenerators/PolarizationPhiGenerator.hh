// File:	PolarizationPhiGenerator.hh
// Date:	01-Feb-2015 G.Macharashvili Dubna

#ifndef PolarizationPhiGenerator_hh
#define PolarizationPhiGenerator_hh

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std;

class PolarizationPhiGenerator {

protected:
	
static	const	double	Pi ;
static	double		PA ;

	double		xeqt(double a,double b,double y,double (*fun)(double)) ;
static	double		F(double x) ;

public:

	PolarizationPhiGenerator () ;
virtual	~PolarizationPhiGenerator () ;

	void		Print() const ;
	double		getRandomPhi(double PAy = 0,double beamPolarizationPhi = 0) ;
// beamPolarizationPhi is an angle with respect to y axis ...
//                     = 0       for Up 
//                     = +/- 180 for Down
//                     = -90 for Left
//                     + +90 for Right

	} ;

#endif

// e-o-f
