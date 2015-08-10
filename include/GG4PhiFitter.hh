// File:	GG4PhiFitter.hh
// Date:	06-Mar-2015 G.Macharashvili Juelich

#ifndef GG4PhiFitter_hh
#define GG4PhiFitter_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <TFile.h>
#include <TVirtualFitter.h>
#include <TF1.h>
#include <TH1.h>

class GG4PhiFitter {

public:  

	GG4PhiFitter() ;
	~GG4PhiFitter() ;

void	SetGeneratedParameters(double generatedBeamPolarization = 0.0,double averageAy = 0.0,double Phi = 0) ;
int	ConnectTH1(TH1F *) ;
void	Print() ;

double	GetPolarizationPhi() ;
double	GetPolarizationPhiError() ;
double	GetAsymmetry() ;
double	GetAsymmetryError() ;

	double	Chisquare() const { return chisquare ; }
	double	ChisquareNorm() const { return  ndf ; }

    
private:

protected:


	TVirtualFitter	*minuit ;
	double		chisquare , ndf , cl ;
	TH1F		*hist ;

	double		beamPolarizationGenerated , analyzingPowerGenerated , asymmetryGenerated ;
	double		asymmetry , asymmetryError ;
	double		beamPolarization , beamPolarizationError ;

	double		polarizationPhiGenerated ;
	double		polarizationPhi , polarizationPhiError ;

	} ;

#endif

// e-o-f
