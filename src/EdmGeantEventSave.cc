// File:	EdmGeantEventSave.cc
// Date:	19-Jun-2015 G.Macharashvili Juelich

#include "EdmGeantEventSave.hh"
#include "EdmEventInformation.hh"
#include <ctime>
#include <Randomize.hh>
#include <VolumeGenerator.hh>

EdmGeantEventSave::~EdmGeantEventSave() {
	delete Timer ;
	if (gRFile && TE) delete TE ;
	}
	
EdmGeantEventSave::EdmGeantEventSave() {
	HCE = 0 ;
	TE = 0 ;
	utime = time(0) ;
	Timer = (G4Timer*) new G4Timer() ;
	Timer->Start() ;

// dynamic loading of user triggering class ...
// get from: AnkeG4EventSave

	}

void	EdmGeantEventSave::BeginOfEventAction(const G4Event* E) {
	G4int EvtNum = E->GetEventID() + 1 ;
	if (!((EvtNum)%1000)) {
		Timer->Stop() ;
		cout << setw(5) << EvtNum/1000 << "k ... " << fixed << showpoint << setw(6) << setprecision(2) << Timer->GetRealElapsed() << " s. elapsed ... \r" << flush ;
		Timer->Start() ;
		}
//++	trigger->init_trigger(E) ;
	}

void	EdmGeantEventSave::EndOfEventAction(const G4Event* E) {
// if output file exists ...
	if (!gRFile) return ;
	if (!TE) {
		TE = new EdmGeantTEvent() ;
		TE->AttachOutputFile(gRFile) ;
		InitRunProfile(TE) ;
		}

// get new time value ... abstract ... EVENT INTENSITY/CROSS-SECTION/ APPLY VIA CONFIG FILE ...
	utime += CLHEP::RandExponential::shoot(10000.0 * CLHEP::ns)/1.e+9 ; // g4 standard time unit is ns. so ns = 1.0 ...
// TRIGGERING ...
// get from: AnkeG4EventSave

// vertex info and tracks ...
	G4int Nprimary = E->GetNumberOfPrimaryVertex() ;
	G4double xo = 0 , yo = 0 , zo = 0 ;
	if (E->GetPrimaryVertex(0)) {
		xo = E->GetPrimaryVertex(0)->GetX0() ;
		yo = E->GetPrimaryVertex(0)->GetY0() ;
		zo = E->GetPrimaryVertex(0)->GetZ0() ;
		}
// hits info ...
	HCE = E->GetHCofThisEvent() ;
// initial event info to save ...
        EdmEventInformation *userInfo = dynamic_cast<EdmEventInformation *> (E->GetUserInformation()) ;
//--cout << userInfo << "  " << userInfo->getStatus() << endl ;
// init output event record ...
	TE->Clear() ;
	TE->SetTime(int(utime),(utime - int(utime)) * 1.e+6) ;
	TE->SetTriggerType(44) ; // xxxxxxxxxxxxx
	TE->SetWeight(1.0) ;
	TE->SetStatus(userInfo->getStatus()) ; // 0 - undef, 1 - elstic, 2- inelastic ...
	TE->SetStatus2(0) ;
// set initial beam direction (p) and vertex ...
	TE->SetVertex(xo,yo,zo) ;
// here primary info needed beam_id, target_id(?), Tp, etc ?? xxxxxxxxxxxxxxxx
	TE->SetBeamParticle(1000020040,0.0,0.0,1.0) ;
// primary tracks (originating from vertex) ...
	for (int k = 0 ; k < Nprimary ; k++) {
		TP = TE->AddTrack() ;
		G4ThreeVector mom = E->GetPrimaryVertex(k)->GetPrimary()->GetMomentum() ;
		//++G4String particle_name = E->GetPrimaryVertex(k)->GetPrimary()->GetG4code()->GetParticleName() ;
		TP->SetParticle(E->GetPrimaryVertex(k)->GetPrimary()->GetPDGcode(),mom.x(),mom.y(),mom.z()) ; // G4/PDG codes for d,t,He3, etc ...
		}
// hits from all detectors ...
	if (HCE) {
		int NumOfHC = HCE->GetNumberOfCollections() ;
		for (int k = 0 ; k < NumOfHC ; k++) {
			ModuleHitsCollection*HC = reinterpret_cast<ModuleHitsCollection *>(HCE->GetHC(k)) ;
			if (!HC->entries()) continue ; // no hits for the detector ...
			for (int h = 0 ; h < HC->entries() ; h++) TH = TE->AddHit(HC->GetName(),(*HC)[h]->GetDetectorID(),(*HC)[h]->GetEnergyDeposit(),(*HC)[h]->GetTime()) ;
			}
		}
	TE->SetAccumulatedEvents(E->GetEventID(),1) ;
	TE->Save() ;
//++	TE->Print() ;
	}

void EdmGeantEventSave::InitRunProfile(EdmGeantTEvent*rootEvent) {
// config data ...
        double beamEnergy = 270.0 ;
        double beamPolarization = 0 , beamPolarizationPhi = 0 ;
        double calorimeterZpos = 460.0 ;
	double sx = 15.0 , sy = 15.0 , sz = 100.0 ;
	VolumeGenerator *config = new VolumeGenerator("config.dat") ;
       	config->setVerboseLevel(0) ; 
	config->getDouble("kinetic_energy",beamEnergy) ;
	config->getDouble("beam_polarization_phi",beamPolarizationPhi) ;
	config->getDouble("beam_polarization",beamPolarization) ;
	config->getDouble("cal.distance.z",calorimeterZpos) ;
	G4VSolid *solid = config->getG4Box("crystal") ;
	sx = 2.0*((G4Box *) solid)->GetXHalfLength() ;
	sy = 2.0*((G4Box *) solid)->GetYHalfLength() ;
	sz = 2.0*((G4Box *) solid)->GetZHalfLength() ;
	delete config ;
// fill in profile data ...
	rootEvent->getProfile()->setInitialKinematics(beamEnergy) ;
	rootEvent->getProfile()->setBeamPolarization(beamPolarization) ;
	rootEvent->getProfile()->setBeamPolarizationPhi(beamPolarizationPhi) ;
	rootEvent->getProfile()->setModuleSize(sx,sy,sz) ;
	rootEvent->getProfile()->setGeometryParameters(calorimeterZpos) ;
	}

// e-o-f
