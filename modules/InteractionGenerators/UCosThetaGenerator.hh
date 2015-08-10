// File:	UCosThetaGenerator.hh
// Date:	12-Dec-2006 G.Macharashvili Juelich

#ifndef UCosThetaGenerator_hh
#define UCosThetaGenerator_hh

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std;

#define SPLINE

#include <TRandom.h>
#include <TMath.h>
#ifdef SPLINE
#include <TGraph.h>
#include <TMVA/Factory.h>
#include <TMVA/TSpline2.h>
#endif

class UCosThetaGenerator {

protected:
	
	int		Npoints ;
	int		CosThetaGenPoints ;
	double		Tp ;
	double		CosThetaBinWidth ;
	double		thetaCM[2000] , Sigma_thetaCM[2000] , Ay_thetaCM[2000] ;
	double		Fx[402] ;
	TRandom		Uniform ;

#ifdef SPLINE
	TGraph		*Graph_cs , *Graph_ay ;
	TMVA::TSpline2	*Spline_cs , *Spline_ay ;
#endif

static	double		AyTheta ;

//+	TVector3	P ;
//+	TVector3	Q ;

	void		Parab(double x1,double y1,double x2,double y2,double x3,double y3,double &a, double &b, double &c) ;
	double		xeqt(double a,double b,double y,double (*fun)(double)) ;
static	double		F(double x) ;
	void		Fatal() ;

public:

	UCosThetaGenerator () ;
	UCosThetaGenerator (const char *filename) ;
virtual	~UCosThetaGenerator () ;

	int	Set(const char *filename) ;
	void	Print() const ;
	void	PrintTotalCrossSection() ;

	double		GetTp() const { return Tp/1000.0 ; }
	double		GetTpMeV() const { return Tp ; }
	double		GenerateThetaCM() ;
	double		GeneratePhi(double theta_cm_deg) ;
	void		GenerateThetaPhi(double &t,double &p,int type = 0) ;

//+	TVector3&	GetBeamPolarization() const { return P ; }
//+	TVector3&	GetTargetPolarization() const { return Q ; }

	double		DSigmaDCos_CosThetaCM(double VCosTheta) ;
	double		DSigmaDOmega_ThetaCM(double ThetaDeg) ;
	double		Ay_ThetaCM(double ThetaDeg) ;

	} ;

#endif

// eof
