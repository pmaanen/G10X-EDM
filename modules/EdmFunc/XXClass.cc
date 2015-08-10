

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
		 - beamMass*beamMass - targetMass*targetMass + particle2Mass*particle2Mass - particle1Mass*particle1Mass ;
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
	( beamMomentumm*( targetMass*(Tp + beamMass) + 0.5*(beamMass*beamMass + targetMass*targetMass + particle1Mass2 - particle2Mass2) ) * co
	+ ee * sqrt( uu*uu - particle1Mass2*particle2Mass2 - particle1Mass2*beamMomentumm*beamMomentumm*si2 ) )
	/ (ee*ee - beamMomentumm*beamMomentumm*co*co) ;
	return particle1Momentum ;
	}




// e-o-f
