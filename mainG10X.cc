// File:	mainG10X.cc
// Date:	27-Jun-2014 GM Dubna

#include "X2_Detector.hh"
#include "G10XActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include <G4UImanager.hh>

#include <QGSP_BIC.hh>
#include <QGSP_BERT.hh>
#include <QGSP_INCLXX.hh>

#include <G4StepLimiterPhysics.hh>
#include <G4EmProcessOptions.hh>

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <TFile.h>

G4RunManager	*RunManager = 0 ;

char		rootFileName[] = { "test_xxx.root" } ;
TFile		*gRFile = 0 ;

#include <signal.h>

void	Interrupt(int signum) { (G4RunManager::GetRunManager())->AbortRun() ; }







int main(int argc,char** argv) {

	if (argc > 1) {
		cout << endl << "    drawing mode ... " << endl ;
		}
	else {
		cout << endl << "    batch mode ... " << endl ;
		gRFile = (TFile *) new TFile(rootFileName,"recreate") ;
		}


	signal(SIGTERM,&Interrupt) ;
	signal(SIGINT ,&Interrupt) ;
	signal(SIGPIPE,&Interrupt) ;

#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
#else
	G4RunManager* runManager = new G4RunManager;
#endif
    
// detector construction ...
	runManager->SetUserInitialization(new X2_Detector) ;

// physics list selection ...    
//++	G4VModularPhysicsList* physicsList = new QGSP_INCLXX(0) ;
//++	G4VModularPhysicsList* physicsList = new QGSP_BIC(0) ;
	G4VModularPhysicsList* physicsList = new QGSP_BERT(0) ;
	physicsList->RegisterPhysics(new G4StepLimiterPhysics()) ;
	physicsList->SetVerboseLevel(2) ;
	runManager->SetUserInitialization(physicsList) ;
// electromagnetic processes optimization ... 
	G4EmProcessOptions opt ;
	opt.SetVerbose(1) ;
//	opt.SetMscStepLimitation(fUseDistanceToBoundary) ;
//	opt.SetMscRangeFactor(0.02) ;
// Physics tables
	opt.SetMinEnergy(1000*CLHEP::eV) ;	// default 100*eV
	opt.SetMaxEnergy(25*CLHEP::GeV) ;	// default 100*TeV
	opt.SetDEDXBinning(400) ;	// default 12*7
	opt.SetLambdaBinning(250) ;	// default 12*7
    
// User action initialization
	runManager->SetUserInitialization(new G10XActionInitialization()) ;

// Initialize Geant4 kernel
	runManager->Initialize() ;

// =====================
	ConfigFileReader *config = new ConfigFileReader("config.dat") ;
	G4int Ntotal = 10000 ;
	config->getInt("gen_events",Ntotal) ;
// =====================

	if (1 == argc) runManager->BeamOn(Ntotal) ;

	else {


#ifdef G4VIS_USE
		G4VisManager* visManager = new G4VisExecutive("quiet") ;
//		G4VisManager* visManager = new G4VisExecutive("warnings") ; // [default]
		visManager->Initialize() ;
//--		visManager->RegisterGraphicsSystem(new G4OpenGLImmediateQt) ;
#endif

		G4UImanager* UImanager = G4UImanager::GetUIpointer();
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc,argv) ;
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute init_vis.mac") ; 
		cout << "VIS_USE command: /control/exec init_vis.mac" << endl ;
#else
		UImanager->ApplyCommand("/control/execute init.mac") ; 
		cout << "noVIS_USE  command: /control/exec init.mac" << endl ;
#endif
		if (ui->IsGUI()) {
			G4String command = "/control/execute " ;
			cout << "GUI command: " << command + argv[1] << endl ;
			UImanager->ApplyCommand(command + argv[1]) ;
			ui->SessionStart() ;
			delete ui ;
			}
#endif
#ifdef G4VIS_USE
		delete visManager ;
#endif
		}

	delete runManager ;

	if (gRFile) {
		gRFile->Write() ;
		gRFile->Close() ;
		}

	return 0;

	}

// e-o-f
