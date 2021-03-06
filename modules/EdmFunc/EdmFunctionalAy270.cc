// File:	EdmFunctionalAy270.cc
// Date:	21-Apr-2015 G.Macharashvili Dubna

#include "EdmFunctionalAy270.hh"

double	EdmFunctionalAy270::Ay(double thetaLab) {
const double scale_factor = 0.85 ; // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
const int numExpPoints = 52 ;
const double x[52] = {  3.17,  3.83,  4.48,  5.07,  5.73,  6.24,  6.82,  7.41,  8.14,  8.66,  9.24,  9.90, 10.42, 11.08, 11.74, 12.33, 12.77, 13.43, 14.09, 14.52, 15.25, 15.83, 16.34, 17.06, 17.72, 18.08, 18.81, 19.40, 19.99, 20.57, 21.23, 21.75, 22.48, 23.00, 23.58, 24.10, 24.83, 25.49, 25.99, 26.51, 27.16, 27.67, 28.32, 28.98, 29.49, 30.00, 30.65, 31.31, 31.82, 32.47, 32.98, 33.56} ;
const double y[52] = {0.1669,0.1628,0.1831,0.2115,0.2054,0.2257,0.2439,0.2804,0.3047,0.3676,0.4264,0.4649,0.5135,0.5723,0.6473,0.7284,0.7730,0.8115,0.8277,0.7872,0.7020,0.6392,0.5986,0.5196,0.5115,0.5155,0.5419,0.5764,0.6291,0.6615,0.7162,0.7932,0.8662,0.9331,0.9716,0.9899,1.0122,1.0264,1.0061,0.9980,0.9534,0.9270,0.8845,0.8520,0.8074,0.8014,0.7791,0.7385,0.7041,0.6797,0.6088,0.5946} ;
	if (thetaLab < x[0] || thetaLab >= x[numExpPoints-1]) {
		cerr << "EdmFunctionalAy270::Ay()  theta_lab outside of range ... " << fixed << showpoint << setprecision(2) << thetaLab << endl ;
		return 0 ;
		}
	int i = 0 ;
	while (x[++i] < thetaLab) if(i == numExpPoints - 1) return 0 ;
	return scale_factor * (y[i-1] + (y[i] - y[i-1]) / (x[i] - x[i-1]) * (thetaLab-x[i-1])) ;
	}
	
// e-o-f
