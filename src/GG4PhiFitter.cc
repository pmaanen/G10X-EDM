// File:	GG4PhiFitter.cc
// Date:	06-Mar-2015 G.Macharashvili Juelich

#include <GG4PhiFitter.hh>
#include <TMath.h>

GG4PhiFitter::GG4PhiFitter() {
	TVirtualFitter::SetDefaultFitter("Minuit") ;
	minuit = TVirtualFitter::Fitter(0,3) ;
	asymmetry = asymmetryError = 0.0 ;
	SetGeneratedParameters() ;
	}

GG4PhiFitter::~GG4PhiFitter() {
	}

void GG4PhiFitter::SetGeneratedParameters(double pol,double ay,double phi) {
	beamPolarizationGenerated = (pol) ? pol : 0.10 ;
	analyzingPowerGenerated = (ay) ? ay : 1.0 ;
	asymmetryGenerated = beamPolarizationGenerated * analyzingPowerGenerated ;
	polarizationPhiGenerated = phi ;
	}

int GG4PhiFitter::ConnectTH1(TH1F * xhist) {
	hist = xhist ;
	chisquare = -1.0 ;
	TString	fun = TString(hist->GetName()) + "_fun" ;
	if (hist->GetEntries()/hist->GetNbinsX() < 30) return 1 ;
	TF1 *fc = new TF1(fun,"[0]*(1.0+[1]*cos((x - [2])*3.14159265/180.0))",-180,180) ;

	fc->SetParName(0,"N/Nbin") ;
	fc->SetParameter(0,hist->GetEntries()/hist->GetNbinsX()) ;

	fc->SetParName(1,"asymm") ;
	fc->SetParameter(1, 0.1) ;
	fc->SetParLimits(1,-1.0,1.0) ;

	fc->SetParName(2,"phi") ;
	fc->SetParameter(2, 0.0) ;
	fc->SetParLimits(2, -180.0, +180.0) ;
	fc->FixParameter(2,0.0) ; // fixed ... 20150430 

	int status = hist->Fit(fc,"q+") ;
	chisquare = fc->GetChisquare() ;
	ndf = fc->GetNDF() ;
	cl = fc->GetProb() ;
	asymmetry = fc->GetParameter(1) ;
	asymmetryError = fc->GetParError(1) ;
	beamPolarization = asymmetry / analyzingPowerGenerated ;
	beamPolarizationError = asymmetryError / analyzingPowerGenerated ;
	polarizationPhi = fc->GetParameter(2) ;
	polarizationPhiError = fc->GetParError(2) ;
	return status ;
	}
	
	
void GG4PhiFitter::Print() {
	cout << fixed << showpoint << setprecision(0) ;
// asymm ...
	cout << endl ;
	cout << "GG4PhiFitter:  hist [" << hist->GetName() << "] statistics: " << setprecision(0) << hist->GetEntries() << endl ;
	if (chisquare < 0.0) {
		cout << "GG4PhiFitter:  low statistics. not fitted ..." << endl ;
		return  ;
		}
	if (asymmetryGenerated != 0.10) {
		cout << "polarization: " << setw(7) << setprecision(4) << beamPolarizationGenerated << " generated " << endl ;
		cout << " averaged Ay: " << setw(7) << setprecision(4) << analyzingPowerGenerated << " generated " << endl ;
		cout << "   asymmetry: " << setw(7) << setprecision(4) << asymmetryGenerated << " generated " << endl ;
		}
	cout << "   asymmetry: " ;
	cout << setw(7) << setprecision(4) << asymmetry << " +/- " ;
	cout << setw(6) << setprecision(4) << asymmetryError << " fitted.  " ;
	cout << "chi2/ndf: " ;
	cout << setw(6) << setprecision(4) << chisquare/ndf << "  " ;
	cout << "CL: " ;
	cout << setw(6) << setprecision(4) << cl << "  " ;
	cout << endl ;

	if (asymmetryGenerated != 0.10) {
		cout << "polarization: " ;
		cout << setw(7) << setprecision(4) << beamPolarization << " +/- " ;
		cout << setw(6) << setprecision(4) << beamPolarizationError << "   " ;
		cout << endl ;
		cout << "        diff: " << setw(7) << setprecision(4) << (asymmetry - asymmetryGenerated) / asymmetryError << " sigma  " << endl ;
		}
// phi ...
//	cout << endl ;
//	if (asymmetryGenerated != 0.10) cout << "  phi: " << setw(7) << setprecision(1) << polarizationPhiGenerated << " generated " << endl ;
//	cout << "  phi: " << setw(7) << setprecision(1) << polarizationPhi << " +/- " ;
//	cout << polarizationPhiError << "  [deg]   (with respect to y-axis)" ;
//	cout << endl ;
//	if (asymmetryGenerated != 0.10) cout << " diff: " << setw(7) << setprecision(1) << (polarizationPhi - polarizationPhiGenerated) / polarizationPhiError << " sigma " << endl ;

	cout << endl ;
	}

// e-o-f
