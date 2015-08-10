// File:	G10XGenerator.hh
// Date:	16-Jul-2014 GM Tbilisi

#ifndef G10XGenerator_h
#define G10XGenerator_h 1

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

#include <TFile.h>
#include <TVirtualFitter.h>
#include <TF1.h>
#include <TH2.h>
#include <TProfile.h>

#include "X_VertexGenerator.hh"
#include "X_InteractionGenerator.hh"
#include "GG4PhiFitter.hh"
#include "PolarizationPhiGenerator.hh"
#include "EdmFunctionalAy270.hh"

class G4ParticleGun ;
class G4Event ;
class G4ParticleDefinition ;

class G10XGenerator : public G4VUserPrimaryGeneratorAction {

private:

	void	HistInit() ;

public:

	G10XGenerator() ;
virtual	~G10XGenerator() ;
    
virtual	void	GeneratePrimaries(G4Event*) ;

protected:

	EdmFunctionalAy270		exp270 ;
	PolarizationPhiGenerator	*phiGenerator ;
	X_InteractionGenerator		*interactionGenerator ;
	X_VertexGenerator		*vertexGenerator ;

	G4ParticleGun		*particleGun ;
	G4ParticleDefinition	*particleDef ;
	GG4PhiFitter		*phiFitter ;
	
	G4double		kineticEnergy ;
	G4double		beamPolarization ;
	G4double		beamPolarizationPhi ;
	G4double		averageAnalyzingPower , averageAy , acceptedEvents , Ay ;
	G4double		minTheta , maxTheta ;

	TH1F			*h_theta , *h_phi , *h_part , *h_mult ;
	TH2F			*vertex_xy , *vertex_zy ;
	TProfile		*p_Ay ;

	} ;

extern	TFile	*gRFile ;

#endif

// e-o-f
