// File:	dataGenerator4Plot.cc
// Date:	10-May-2015 GM Dubna

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;


#include <TFile.h> 
#include <TH1.h> 
#include <TF1.h> 
#include <TGraph.h> 
#include <TMath.h> 
#include <TVirtualFitter.h> 
#include <TLorentzVector.h> 
using namespace TMath ;


const double Tp = 270.0 ; // [MeV]
const double d_mass = 1875.612763 ; // [MeV]
const double c_mass = 6 * d_mass ;

double  getAngle(double m1,double m2,double Tp,double ms,double mr,double Eks) {
// theta_lab = fun(Ek_lab) ;
// m1 - beam mass , m2 - target mass ...
// ms - mass scattered, mr - mass recoil ...
	double E1 = Tp + m1 ;
	double P1 = sqrt(E1*E1 - m1*m1) ;
	double Es = Eks + ms ;
	double Ps = sqrt(Es*Es - ms*ms) ;
	double cosThetaLab = 1.0/(2*P1*Ps) * (2.0*((E1 + m2)*Es - E1*m2) + mr*mr - m2*m2 - ms*ms - m1*m1) ;
	return acos(cosThetaLab) ;
	}

double  angle1(double Tp,double beamMass,double targetMass,double particle1Mass,double particle2Mass,double particle1Ek) {
// all parameters in LAB system ...
	double beamMomentum = sqrt((Tp + beamMass) * (Tp + beamMass) - beamMass*beamMass) ;
	double particle1E = particle1Ek + particle1Mass ;
	double particle1Momentum = sqrt(particle1E*particle1E - particle1Mass*particle1Mass) ;  
	double cosTheta = 1.0/(2*beamMomentum*particle1Momentum)*(2.0*(((Tp + beamMass)+targetMass)*particle1E - (Tp + beamMass)*targetMass) \
		 - beamMass*beamMass - targetMass*targetMass + particle2Mass*particle2Mass - particle1Mass*particle1Mass ) ;
	return acos(cosTheta) ;
	}
                                        
double  kineticEnergy1(double Tp,double beamMass,double targetMass,double particle1Mass,double particle2Mass,double particle1Theta) {
// all parameters in LAB system ...
	double beamMomentum = sqrt((Tp + beamMass) * (Tp + beamMass) - beamMass*beamMass) ;
// particle1Mass - scattered (deuteron) particle1Mass2 - squared ...
// particle2Mass - recoil (carbon) 
	double particle1Mass2 = particle1Mass * particle1Mass ;
	double particle2Mass2 = particle2Mass * particle2Mass ;
	double co = cos(particle1Theta) ;
	double si2 = 1.0 - co*co ;
	double ee = (Tp + beamMass) + targetMass ;
	double uu = targetMass*(Tp + beamMass) + 0.5*(beamMass*beamMass + targetMass*targetMass - particle1Mass2 - particle2Mass2) ;
	double particle1Momentum = 
	( beamMomentum*( targetMass*(Tp + beamMass) + 0.5*(beamMass*beamMass + targetMass*targetMass + particle1Mass2 - particle2Mass2) ) * co
	+ ee * sqrt( uu*uu - particle1Mass2*particle2Mass2 - particle1Mass2*beamMomentum*beamMomentum*si2 ) )
	/ (ee*ee - beamMomentum*beamMomentum*co*co) ;
	return sqrt(particle1Momentum*particle1Momentum + particle1Mass2)  - particle1Mass ;
	}

double cm2lab(double thetaCMdeg) {
	TLorentzVector	L = TLorentzVector(0,0,sqrt((d_mass+Tp)*(d_mass+Tp)-d_mass*d_mass),d_mass+Tp) + TLorentzVector(0,0,0,c_mass) ;
	TLorentzVector  Slab , Scm ;
	double thetaCM = thetaCMdeg * DegToRad() ;
	double cc = Cos(thetaCM) , ss = Sin(thetaCM) ;
	double s = d_mass*d_mass + c_mass*c_mass + 2.0 * (Tp+d_mass) * c_mass ;
	double e = (s+d_mass*d_mass-c_mass*c_mass)/Sqrt(2.0*s) ;
	double p = Sqrt(e*e-d_mass*d_mass) ;
	Scm.SetVectM(TVector3(p*ss,0.0,p*cc),d_mass) ;
	Slab = Scm ;
	Slab.Boost(0,0,L.Beta()) ;
	return Slab.Vect().Theta() * RadToDeg() ;
	}


int main(int argc,char** argv) {

	double x[1000] , y[1000] ;
	ifstream in ;
	if (argc < 2) {
		cerr << endl << argv[0] << " file:dC_crossX_270_MeV.dat " << endl ;
		abort() ;
		}
	in.open(argv[1]) ;
	if (!in.is_open()) {
		cerr << endl << "dataGenerator4Plot: (F) ..." << endl ;;
		cerr         << "                    " << argv[1] << endl ;
		cerr         << "                    not exists. " << endl << endl ;
		abort() ;
		}
	int k = 0 ;
        do {
		in >> x[k] >> y[k] ;
		if (in.eof()) break ;
		} while (k++ < 1000) ;
        in.close() ;

	double t1 = 4.0 ,t2 = 25.0 ;
//	============================
        double numExpPoints = k ;
	TFile *wFile = TFile::Open("dataGenerator4Plot.root","recreate") ;
	TGraph *graphX = new TGraph(numExpPoints,x,y) ;
	graphX->Write() ;
	TVirtualFitter::SetDefaultFitter("Minuit") ;
	const char *fname = "pol7" ;
	int nx = 4 * int(t2-t1+0.01) ;
	TH1F *histXcm = new TH1F("histXcm","  ",nx,0,25) ;
	TH1F *histXcm2 = new TH1F("histXcm2","  ",nx,0,25) ;
	TH1F *histXlab2 = new TH1F("histXlab2","  ",nx,0,25) ;
	for (int b = 1 ; b <= histXcm->GetNbinsX() ; b++) {
		histXcm->SetBinContent(b,graphX->Eval(histXcm->GetBinCenter(b))) ;
		histXcm->SetBinError(b,0.05 * graphX->Eval(histXcm->GetBinCenter(b))) ;
		}
	histXcm->Fit(fname,"R+","",t1,t2) ;
	TF1 *funGen = histXcm->GetFunction(fname) ;
	for (int i = 0 ; i < 1000000 ; i++) {
		double thetaCM = funGen->GetRandom() ;
		double thetaLAB = cm2lab(thetaCM) ;
		histXcm2->Fill(thetaCM) ;
		histXlab2->Fill(thetaLAB) ;
		cout << fixed << showpoint << setprecision(2) ;
		cout << setw(6) << kineticEnergy1(Tp,d_mass,c_mass,d_mass,c_mass,thetaLAB*DegToRad()) << " " ;
		cout << setw(5) << thetaLAB ;
//		cout << "  " << setw(5) << angle1(Tp,d_mass,c_mass,d_mass,c_mass,particle1Ek)*RadToDeg() << "  " ;
		cout << endl ;
		}
	wFile->Write() ;
	wFile->Close() ;

	return 0;

	}

// e-o-f
