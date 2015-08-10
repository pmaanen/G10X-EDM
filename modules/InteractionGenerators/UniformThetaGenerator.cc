// File:	UniformThetaGenerator.cc
// Date:	13-Apr-2015 G.Macharashvili Dubna

#include "UniformThetaGenerator.hh"
#include <CLHEP/Random/RandFlat.h>
#include <G4ParticleTable.hh>

void*	UniformThetaGenerator::ptr = 0 ;

void*	UniformThetaGenerator::get() { return (ptr) ? ptr : ptr = new UniformThetaGenerator ; }

UniformThetaGenerator::UniformThetaGenerator(const char *name) : VInteractionGenerator(name) {
	num_of_tracks = 1 ;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable() ;
// defaults
	particle = particleTable->FindParticle("deuteron") ;
	kineticEnergy = 270 * CLHEP::MeV ;
	theta1 =  5.0 * CLHEP::deg ;
	theta2 = 20.0 * CLHEP::deg ;
	}

void	UniformThetaGenerator::Init(G4ParticleDefinition *Xparticle,G4double XkineticEnergy,G4double t1,G4double t2) {
        particle = Xparticle ;
        kineticEnergy = XkineticEnergy ;
        theta1 = t1 ;
        theta2 = t2 ;
        }

void	UniformThetaGenerator::GenerateInteraction() {
	theta[0] = CLHEP::RandFlat::shoot(theta1,theta2) ;
	ek[0] = kineticEnergy ;
	def[0] = particle ;
	}
	
// e-o-f
