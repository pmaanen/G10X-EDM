// File:	UCosThetaGenerator.cc
// Date:	12-Dec-2006 G.Macharashvili Juelich

#include "UCosThetaGenerator.hh"

UCosThetaGenerator::~UCosThetaGenerator () {
#ifdef SPLINE
	if (!Graph_cs) delete Graph_cs ;
	if (!Graph_ay) delete Graph_ay ;
	if (!Spline_cs) delete Spline_cs ;
	if (!Spline_ay) delete Spline_ay ;
#endif
	}

UCosThetaGenerator::UCosThetaGenerator () {
#ifdef SPLINE
	Graph_cs = Graph_ay = 0 ;
	Spline_cs = Spline_ay = 0 ;
#endif
	}

UCosThetaGenerator::UCosThetaGenerator(const char *filename) {
	int	status = Set(filename) ;
	if (status) Fatal() ;
	}

int	UCosThetaGenerator::Set(const char *filename) {
	ifstream	is ;
	if (filename) is.open(filename) ;
	if (!is.is_open()) return 1 ;
	is >> Tp >> Npoints ;
	if (!is.good()) return 1 ;
	int	k = Npoints ;
	double	*x = &thetaCM[0] ;
	double	*y = &Sigma_thetaCM[0] ;
	double	*a = &Ay_thetaCM[0] ;
	while (k--) is >> *x++ >> *y++ >> *a++ ;
	is.close() ;
// Temp: for Ay enhancign at 40 MeV ...
//	for (int k = 0 ; k < Npoints ; k++) {
//		Ay_thetaCM[k] *= 20.0 ;
//		if (Ay_thetaCM[k] > 0.8) Ay_thetaCM[k] = 0.8 ;
//		}
// End Tmp ...
	Graph_cs = new TGraph(Npoints,thetaCM,Sigma_thetaCM) ;
	Spline_cs = new TMVA::TSpline2("cs",Graph_cs) ;
	Graph_ay = new TGraph(Npoints,thetaCM,Ay_thetaCM) ;
	Spline_ay = new TMVA::TSpline2("cs",Graph_ay) ;
//
	int	c = 0 ;
	CosThetaGenPoints = 400 ;
	CosThetaBinWidth = 2.0 / CosThetaGenPoints ;
	Fx[0] = 0 ;
	for (int k = 0 ; k < CosThetaGenPoints ; k++) {
		Fx[k+1] = Fx[k] + DSigmaDCos_CosThetaCM(-1.0 + (0.5+k)*CosThetaBinWidth) ;
		}
	for (int k = 1 ; k <= CosThetaGenPoints ; k++) Fx[k] = Fx[k] / Fx[CosThetaGenPoints] ;
	PrintTotalCrossSection()  ;

	return	0 ;
	}

// type&1 == 1  - cos(theta) flat (0,1) ...
// type&2 == 2  - Ay = 0 ... 
void	UCosThetaGenerator::GenerateThetaPhi(double &t,double &p,int type) {
	t = ( (type & 0x1) == 1) ? acos(Uniform.Rndm())*TMath::RadToDeg() : GenerateThetaCM() ;
	p = ( (type & 0x2) == 2) ? (-1. + 2. * Uniform.Rndm()) * 180. : GeneratePhi(t) ;
	}
	
double	UCosThetaGenerator::GenerateThetaCM() {
	double	r = Uniform.Rndm() ;
	int	bin = TMath::BinarySearch(CosThetaGenPoints,Fx,r) ;
	double	x = -1.0 + CosThetaBinWidth * bin ;
	if (r > Fx[bin]) x += CosThetaBinWidth * (r - Fx[bin]) / (Fx[bin+1] - Fx[bin]) ;
	return acos(x)*TMath::RadToDeg() ;
	}

double	UCosThetaGenerator::GeneratePhi(double theta_cm) {
	AyTheta = Ay_ThetaCM(theta_cm) ;
	double	xPhi = xeqt(0.0,TMath::Pi(),Uniform.Rndm(),&F) ;
		xPhi *= (Uniform.Rndm() > 0.5) ? +180./TMath::Pi() : -180./TMath::Pi() ;
	return	xPhi ;
	}
	
void	UCosThetaGenerator::Print() const {
	cout << "UCosThetaGenerator: Tp: " ;
	cout << fixed << showpoint << setw(5) << setprecision(1) ;
	cout << Tp << " MeV. " << Npoints << " data points. " << endl ;
	for (int k = 0 ; k < Npoints ; k++) {
		cout << fixed << showpoint << scientific << setprecision(6) ;
		cout << setw(3) << k+1 << "  " ;
		cout << setw(14) << thetaCM[k] << " " ;
		cout << setw(14) << Sigma_thetaCM[k] << " " ;
		cout << setw(14) << Ay_thetaCM[k] << " " ;
		cout << endl ;
		}
	}

double	UCosThetaGenerator::DSigmaDCos_CosThetaCM(double VCosTheta) { return 2*TMath::Pi()*DSigmaDOmega_ThetaCM(acos(VCosTheta)*TMath::RadToDeg()) ; }

double	UCosThetaGenerator::DSigmaDOmega_ThetaCM(double ThetaDeg) {
#ifdef SPLINE
	return Spline_cs->Eval(ThetaDeg) ;
#else
	int	i = TMath::BinarySearch(Npoints,thetaCM,ThetaDeg) ;
	if (ThetaDeg >= thetaCM[i]) return ((Sigma_thetaCM[i+1] - Sigma_thetaCM[i])/(thetaCM[i+1] - thetaCM[i]) * (ThetaDeg - thetaCM[i]) + Sigma_thetaCM[i]) ;
	return 0.0 ;
#endif
	}

double	UCosThetaGenerator::Ay_ThetaCM(double ThetaDeg) {
#ifdef SPLINE
	return Spline_ay->Eval(ThetaDeg) ;
#else
	double	a , b , c ;
	if (ThetaDeg < 0.0) return 0.0 ;
	if (ThetaDeg > 180.0) return 0.0 ;
	if (ThetaDeg <= thetaCM[1]) Parab(thetaCM[0],Ay_thetaCM[0],thetaCM[1],Ay_thetaCM[1],thetaCM[2],Ay_thetaCM[2],a,b,c) ;
	else if (ThetaDeg >= thetaCM[Npoints-2]) Parab(thetaCM[Npoints-3],Ay_thetaCM[Npoints-3],thetaCM[Npoints-2],Ay_thetaCM[Npoints-2],thetaCM[Npoints-1],Ay_thetaCM[Npoints-1],a,b,c) ;
	else {
		int i = TMath::BinarySearch(Npoints,thetaCM,ThetaDeg) - 2 ;
		for ( ; i < Npoints-1 ; i++) {
			if ( (ThetaDeg-thetaCM[i-1] >= (thetaCM[i]-thetaCM[i-1])*0.5) && (ThetaDeg-thetaCM[i] < (thetaCM[i+1]-thetaCM[i])*0.5)) break ;
			}
		Parab(thetaCM[i-1],Ay_thetaCM[i-1],thetaCM[i],Ay_thetaCM[i],thetaCM[i+1],Ay_thetaCM[i+1],a,b,c) ;
		}
	return a*ThetaDeg*ThetaDeg + b*ThetaDeg + c ;
#endif
	}

void	UCosThetaGenerator::Fatal() {
	cout << "UCosThetaGenerator: Data filename not specified ..." << endl ;
	exit(0) ;
	}

void	UCosThetaGenerator::Parab(double x1,double y1,double x2,double y2,double x3,double y3,double &a, double &b, double &c) {
	double v1=x1-x2 , v2=x1-x3 , v3=x2-x3 ;
	a = y1/v1/v2 - y2/v1/v3 + y3/v2/v3 ;
	b = -(y1*(x2+x3)/v1/v2 - y2*(x1+x3)/v1/v3 + y3*(x1+x2)/v2/v3) ;
	c = y1*x2*x3/v1/v2 - y2*x1*x3/v1/v3 + y3*x1*x2/v2/v3 ;
	}

/*  
// **************************************************************
// in the interval (a,b) x will be found, for which: fun(x)==max
// returns x and ymax
// **************************************************************
double  xfmx(double a,double b,double *yout,double (*fun)(double)) {
	double x , y , dx = (b-a)/2. , xmx = (a+b)/2. , ymxo , ymx = -1.e+12 ;
	do {
		ymxo=ymx ;
		if(xmx-dx>a) a=xmx-dx ;
		if(xmx+dx<b) b=xmx+dx ;
		dx=(b-a)/50. ;
		x=a ;
		while(x<=b) {
			y=(*fun)(x) ;
			if(y>ymx) {
				ymx=y ;
				xmx=x ;
				}
			x+=dx ;
			}
		} while (ymx-ymxo > 1e-5) ;
	if (yout!=(double *)0) *yout=ymx ;
	return xmx ;
	}
// **************************************************************
// in the interval (a,b) x will be found, for which: fun(x)==y
// returns x
// **************************************************************
*/
double	UCosThetaGenerator::F(double x) { return 1.0/TMath::Pi()*(x + AyTheta*sin(x)) ; }

double	UCosThetaGenerator::xeqt(double a,double b,double y,double (*fun)(double)) {
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

void	UCosThetaGenerator::PrintTotalCrossSection() {
	double	CosThetaStep = 0.001 ;
	int	CosThetaBins = int((2.0+1.e-8)/CosThetaStep) ;
	double	cos1 = -1.0 , cos2 = -1.0 , t1 = TMath::Pi() , t2 = TMath::Pi() , s1 = 0 , s2 = 0 ;
	double	STotal1 = 0 , STotal2 = 0 ; // 
	for (int k = 0 ; k < CosThetaBins ; k++) {
		cos1 = cos2 ;
		t1 = t2 ;
		s1 = s2 ;
		cos2 = cos1 + CosThetaStep ;
		if (cos2 > 1.0) cos2 = 1.0 ;
		t2 = acos(cos2) ;
		s2 = DSigmaDOmega_ThetaCM(t2*TMath::RadToDeg()) ;
//cout << fixed << showpoint << setprecision(2) ;
//cout << setw(9) << t1*TMath::RadToDeg() << "  " ;
//cout << setw(9) << t2*TMath::RadToDeg() << "  " ;
//cout << setw(9) << (s1+s2)/2 << "  " ;
//cout << setw(9) << (t1 - t2) << "  " ;
//cout << endl ;
		STotal1 += 2*TMath::Pi()*sin((t2 + t1)*0.5)*(t1 - t2)*(s2 + s1)*0.5 ;
		STotal2 += DSigmaDCos_CosThetaCM((cos1 + cos2)*0.5)*CosThetaStep ;
		}
	cout << endl << showpoint << fixed << setprecision(3) ;
	cout << "       UCosThetaGenerator: Total cross-section " << endl ;
	cout << "                  Integrated DSigma/DOmega:    " << STotal1 << " mb. " << endl ;
	cout << "                  Integrated DSigma/DCosTheta: " << STotal2 << " mb. " << endl ;
	cout << endl ;
	}

double	UCosThetaGenerator::AyTheta = 0 ;

// eof
