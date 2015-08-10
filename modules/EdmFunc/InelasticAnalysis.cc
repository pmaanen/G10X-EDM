// File:	InelasticAnalysis.cc
// Date:	15-Jun-2015 GM juelich

#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;


#include <TFile.h> 
#include <TH1.h> 
#include <TH2.h> 
#include <TF1.h> 
#include <TGraph.h> 
#include <TMath.h> 
#include <TVirtualFitter.h> 
#include <TLorentzVector.h> 
using namespace TMath ;

const double Tp = 270.0 ; // [MeV]
const double d_mass = 1875.612763 ; // [MeV]
const double c_mass = 6 * d_mass ;

const double protonMass = 938.272000  , neutronMass = 939.565330 ;

int	trk = 0 ;
double ek[20] , theta[20] , phi[20] , m[20] ;
double minTheta = 4.0 * DegToRad() , maxTheta = 22.0 * DegToRad() ;

int	GetEvent(ifstream &in) {
	trk = 0 ;
	char c ;
	int Ntotal , Nrecorded ;
	if (in.eof()) return -1 ;
	in >> c >> Ntotal >> Nrecorded ;
	for (int t = 0 ; t < Nrecorded ; t++) {
		in >> c >> ek[trk] >> theta[trk] >> phi[trk] ;
		if (in.eof()) return -1 ;
		theta[trk] *= DegToRad() ;
		if (theta[trk] < minTheta || theta[trk] > maxTheta) continue ;
		phi[trk] *= DegToRad() ;
		if (c == 'p') m[trk] = protonMass ;
		else if (c == 'n') m[trk] = neutronMass ;
		else continue ;
		trk++ ;
		}
	return trk ;
	}
	


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
					
double  momentum1(double Tp,double beamMass,double targetMass,double particle1Mass,double particle2Mass,double particle1Theta) {
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


int main(int argc,char** argv) {

// input data file ...
	ifstream in ;
// get data file ...
	TString home = getenv("HOME") ;
	in.open("dC_Inelastic.dat") ;
	if (!in.is_open()) {
		in.open(home + "/dC_Inelastic.dat") ;
		if (!in.is_open()) {
			in.open(home + "/edm-data/dC_Inelastic.dat") ;
			if (!in.is_open()) {
				cerr << endl << "**** InelasticAnalysis (F) ..." << endl ;;
				cerr << "    ./dC_Inelastic.dat " << endl ;
				cerr << "    ~/dC_Inelastic.dat " << endl ;
				cerr << "    ~/edm-data/dC_Inelastic.dat " << endl ;
				cerr << "**** data file not exists. " << endl << endl ;
				abort() ;
				}
			else cout << "data file: " << home+"/edm-data/dC_Inelastic.dat" << endl ;
			}
		else cout << "data file: " << home+"/dC_Inelastic.dat" << endl ;
		}
	else cout << "data file: " << "./dC_Inelastic.dat" << endl ;

// output file ...
	TFile *wFile = TFile::Open("InelasticAnalysis.root","recreate") ;
	TH1F *histPcmN = new TH1F("p_cm_neutron","",200,0,600.) ;
	TH1F *histPcmP = new TH1F("p_cm_proton" ,"",200,0,600.) ;
	TH1F *histThetaLab = new TH1F("T_lab","",50,0,50.) ;
	TH1F *histThetaCM = new TH1F("T_cm","",50,0,50.) ;
	TH1F *effectiveMass = new TH1F("effective_mass_pn","",500,1000,2200.) ;
	TH1F *excitationEnergy = new TH1F("excitation_pn","",500,0,200.) ;
	TH2F *energyN2P = new TH2F("energy_pn","",100,0.,150.,100,0.,150.) ;
	energyN2P->SetOption("col") ;
// kinematics ...
	TLorentzVector	L = TLorentzVector(0,0,sqrt((d_mass+Tp)*(d_mass+Tp)-d_mass*d_mass),d_mass+Tp) + TLorentzVector(0,0,0,c_mass) ;
	TLorentzVector  S[4] , Scm[4] , Pmax , Nmax ;
	Pmax.SetXYZM(0.0,0.0,400.0,protonMass) ;
	Nmax.SetXYZM(0.0,0.0,400.0,neutronMass) ;
	double p[4] ;
cout << fixed << showpoint << setprecision(4) ;
cout << " sys.beta: " << setw(7) << L.Beta() ;
cout << endl ;
// read data file ...
	int acc = 0 ;
	int num_tracks = 0 ;
	int ix = -1 , iy = -1 ;
	while ((num_tracks = GetEvent(in)) >= 0) {

		if (num_tracks > 1 && num_tracks < 5) {
			acc++ ;
			for (int i = 0 ; i < num_tracks ; i++) p[i] = 0 ;
			for (int i = 0 ; i < num_tracks ; i++) {
				if (m[i] != protonMass && m[i] != neutronMass) continue ;
				p[i] = sqrt((ek[i]+m[i])*(ek[i]+m[i]) - m[i]*m[i]) ;
				S[i].SetVectM(TVector3(p[i]*sin(theta[i])*cos(phi[i]),p[i]*sin(theta[i])*sin(phi[i]),p[i]*cos(theta[i])),m[i]) ;
				Scm[i] = S[i] ;
				Scm[i].Boost(0,0,-L.Beta()) ;
				if (m[i] == protonMass) histPcmP->Fill(p[i]) ;
				if (m[i] == neutronMass) histPcmN->Fill(p[i]) ;
				histThetaLab->Fill(theta[i]*RadToDeg()) ;
				histThetaCM->Fill(Scm[i].Theta()*RadToDeg()) ;

				if (p[i] > 400.0) {
					if (m[i] == protonMass && Scm[i].Vect().Mag() > Pmax.Vect().Mag()) { ix = i ; Pmax = Scm[i] ; }
					if (m[i] == neutronMass && Scm[i].Vect().Mag() > Nmax.Vect().Mag()){ iy = i ; Nmax = Scm[i] ; }
					}
				}
                        if (ix >= 0 && iy >= iy) energyN2P->Fill(ek[ix],ek[iy]) ;
			if (Pmax.Vect().Mag() > 400.0 && Nmax.Vect().Mag() > 400.0) { // breakup ???
				
				effectiveMass->Fill((Pmax+Nmax).M()) ;
				excitationEnergy->Fill((Pmax+Nmax).M()-protonMass-neutronMass) ;
			        }
			}
		}

	cout << acc << " events accepted ..." << endl << endl ;

	in.close() ;

	wFile->Write() ;
	wFile->Close() ;

	return 0;

	}

// e-o-f
