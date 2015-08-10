// File:	StepAnalysis.cc
// Date:	03-Mar-2015 G.Macharashvili Juelich

#include "StepAnalysis.hh"

StepAnalysis::StepAnalysis() {
	ListPostStepProc.Reset() ;
	ListStepStatus.Reset() ;

	}

StepAnalysis::~StepAnalysis() {
	cout << "GG4StepAnalysis::dtor ..." << endl ;
	ListPostStepProc.Print(cout,"    ") ;
	ListStepStatus.Print(cout,"    ") ;
	cout << endl ;
	}

void	StepAnalysis::Diagnostics(const G4Step* xstep,const char *particle) {
// step status types ... HAS TO BE CALLED AT SensitiveDetector::ProcessHits(...) 
const   string  SS[8] = { "WorldBoundary","GeomBoundary","AtRestDoItProc","AlongStepDoItProc","PostStepDoItProc","UserDefinedLimit","ExclusivelyForcedProc","Undefined" } ; 
	step = xstep ;
	track = step->GetTrack() ;
	if (track->GetTrackID() != 1) return ;
	if (track->GetParticleDefinition()->GetParticleName() != particle) return ;
// volume ...
	pvol = track->GetVolume() ;
	lvol = pvol->GetLogicalVolume() ;
// step points ...
	sp1 = step->GetPreStepPoint() ;
	sp2 = step->GetPostStepPoint() ;
	p1 = sp1->GetPosition() ;
// process defined step ...
	procName = "undef" ;
	proc = const_cast<G4VProcess*>(step->GetPostStepPoint()->GetProcessDefinedStep()) ;
	if (proc != 0) {
		procType = proc->GetProcessType() ;
		procName = proc->GetProcessName() ;
		ListPostStepProc.Register(procName,procType) ;
		}
// register all step status ...
	ListStepStatus.Register(SS[sp2->GetStepStatus()].c_str()) ;

// exactly entering the physical volume ... 
// but unknown entry side (front ?) ...
	if (1 == sp1->GetStepStatus()) {
/*
		cout << endl ;
		cout << sp1->GetStepStatus() << "-" ;
		cout << sp2->GetStepStatus() << "  " ;
		cout << "entry:  " ;
		cout << fixed << showpoint << setprecision(2) << setw(6) << sp1->GetKineticEnergy() << "  " ;
		cout << " " << pvol->GetName() << "  " << procName << "  " ;
		cout << endl ;
*/
// G4VPhysicalVolume placement and G4VSolid data is needed  for frontal entry check ...
//		if (p1.x() ...
		stepEntryStatus = 1 ;
		}
	else stepEntryStatus = 0 ;

// placement ... done. 
/*
cout << endl ;
cout << fixed << showpoint << setprecision(2) ;
cout << setw(7) << pvol->GetTranslation().x() << " " ;
cout << setw(7) << pvol->GetTranslation().y() << " " ;
cout << setw(7) << pvol->GetTranslation().z() << " " ;
cout << endl ;
*/

// if post step status == 1 - Transportation - also means unconditionaly exit
//                        3 - hIoni          - happens any time along track
//                        4 - dInelastic     - 

	postStepStatus = step->GetPostStepPoint()->GetStepStatus() ;
// exactly exit ...
	if (1 == postStepStatus) {
		stepEndStatus = exitVolume ;
		}
// post: kill (dInelastic - ONLY) ...
	if (4 == postStepStatus) {
		stepEndStatus = identityLoss ;
		}
// along: (hIoni)  ...
	if (3 == postStepStatus) {
		if (sp2->GetKineticEnergy() < 1.e-6) stepEndStatus = stopped ;
		else stepEndStatus = energyLoss ;
		}
	
	return ;

	}

// e-o-f
