// File:	HadronicInteractionGenerator.hh
// Date:	09-May-2015 G.Macharashvili Dubna

#ifndef HadronicInteractionGenerator_hh
#define HadronicInteractionGenerator_hh 1

#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4EventManager.hh>
#include <G4Event.hh>
#include <G4UserInteractionGenerator.hh>
#include <G4ParticleChange.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4ForceCondition.hh>

#include <UDynamicManager.hh>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>

class HadronicInteractionGenerator : public VInteractionGenerator {

static	void	*ptr ;

public:

	HadronicInteractionGenerator(const char * = "HadronicInteractionGenerator") ;
virtual	~HadronicInteractionGenerator() ;

virtual	void	GenerateInteraction() ;
virtual	void	Init(G4ParticleDefinition *particleDefinition = 0,G4double kineticEnergy = 270.0,G4double t1 = 0.0,G4double t2 = 0.0 ) ;
static	void	*get() ;

	void	HistInit() ;

protected:

	G4int			generatedEvents , acceptedEvents ;

	G4EventManager		*eventManager ;
static	G4ProcessManager	*processManager ;
	G4int			NumOfProc ;
	G4ProcessVector		*ProcVect ;
	G4VProcess		*Proc , *activeProc ;
	G4ProcessType		ProcType ;
	G4String		ProcName ;
	G4String		ProcTypeName ;
	G4String		activeProcessForced ;

	G4int			NInelastic , NElastic ;
	G4ForceCondition	Condition ;

	TH1F			*h_elasticThetaLab , *h_elasticPhi , *h_numOfSecondaries ;
	TH2F			*h_elasticTE ;
	TH2F			*h_protonTE , *h_deuteronTE , *h_gammaTE , *h_alphaTE , *h_neutronTE ;	

	ofstream		of ;
	G4int			w_event ;

	} ;

extern	TFile*		gRFile ;

#endif

// e-o-f
