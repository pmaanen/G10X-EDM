// File:	G10XEventSave.cc
// Date:	01-Feb-2015 G.Macharashvili Dubna

#include "G10XEventSave.hh"
/*
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iomanip> 
#include <ctime>
using namespace std ;
*/
//#include <G10XRunMonitor.hh>
#include <ConfigFileReader.hh>
//#include <G4Box.hh>
#include <Randomize.hh>
#include <G4SystemOfUnits.hh>

G10XEventSave::~G10XEventSave() {
	delete Timer ;
	}
	
G10XEventSave::G10XEventSave() {
	G4EventManager::GetEventManager()->SetVerboseLevel(0) ;
//++	G10XCodesPDG::Get("uparticle_data.dat") ;
//++	G10XCodesPDG::Get() ;
//++	G10XDetectorIDCodes::Get() ;
	HCE = 0 ;
//	TE = 0 ;
	utime = time(0) ;
	Timer = (G4Timer*) new G4Timer() ;
	Timer->Start() ;

// dynamic loading of user triggering class ...
/**
	string lib_path = G10XConfiguration::Get()->GetTriggerLibPath() ;
	string trig_class_name = G10XConfiguration::Get()->GetTriggerClassName() ;
        stringstream class_name(ios::out) ;
	class_name.seekp (0,ios::beg) ;
	class_name << "_ZN" << trig_class_name.length() << trig_class_name << "3getEv\0" ;
	void *handle = dlopen(lib_path.c_str(),RTLD_NOW) ;
	if (!handle) {
		cerr << endl ;
		cerr << "G10XEventSave: user trigger class loading " << endl ;
		cerr << "    " << lib_path << ": probably lib_path error.   class: " << trig_class_name << endl ;
		cerr << "    " << dlerror() << endl ;
		abort() ;
		}
	void *tmp = dlsym(handle,class_name.str().c_str()) ;
        dlclose(handle) ;
	trigger = 0 ;
        if (tmp) {
		VirtualTrigger* (*xgen)() = reinterpret_cast<VirtualTrigger*(*)()> (tmp) ;
		trigger = (*xgen)() ;
		}
	if (!trigger) {
		cerr << "G10XEventSave: probably the class name is wrong. class: " << trig_class_name << endl << endl ;
	        abort() ;
	        }
	else {
		cout << endl ;
		cout << "G10XEventSave() uses trigger class \"" << trig_class_name << "\"" << endl ;
		cout << "new trigger classes have to be added at anke-source/modules/G10XTriggers/ ..." << endl ;
		cout << endl ;
		}
**/
	}

void	G10XEventSave::BeginOfEventAction(const G4Event* E) {
	G4int EvtNum = E->GetEventID() + 1 ;
	if (!((EvtNum)%1000)) {
		Timer->Stop() ;
		cout << setw(5) << EvtNum/1000 << "k ... " << fixed << showpoint << setw(6) << setprecision(2) << Timer->GetRealElapsed() << " s. elapsed ... \r" << flush ;
		Timer->Start() ;
		}
	trigger->initTrigger(E) ;
	}

void	G10XEventSave::EndOfEventAction(const G4Event* E) {
// get new time value ... abstract ... EVENT INTENSITY/CROSS-SECTION/ APPLY VIA CONFIG FILE ...
	utime += CLHEP::RandExponential::shoot(100.0 * ns)/1.e+9 ; // g4 standard time unit is ns. so ns = 1.0 ...
/**
// TRIGGERING ...
	accept = 1 ;
	if (trigger->amygdala(E)) accept = 0 ;
	if(!trigger->cortex(E,trig_value)) accept = 0 ;
	G10XRunMonitor::Get()->SetLastEventStatus(accept) ; // THE METHOD MUST CALLED ONLY ONCE FOR AN EVENT ...
	if (0 == accept) return ;

// THE FOLLOWING WORKS IF THE EVENT IS ACCEPTED ...

// init output file and histograms at first call ... 
	if (!TE && gRFile) {
		TE = new G10XTEvent() ;
		TE->AttachOutputFile(gRFile) ;
		}

// get event info if necessary ... e.g. Tp ...
// get track info ...
	G4int Nprimary = E->GetNumberOfPrimaryVertex() ;
	G4PrimaryParticle* PrimaryProton = E->GetPrimaryVertex(0)->GetPrimary() ;
//++	PrimaryParticleInfo *ParticleInfo = dynamic_cast<PrimaryParticleInfo *> (PrimaryParticle->GetUserInformation()) ;
// kinematics ...
	G4ThreeVector Pp = PrimaryProton->GetMomentum() ;
	G4double Tp = sqrt(Pp*Pp + PrimaryProton->GetMass()*PrimaryProton->GetMass()) - PrimaryProton->GetMass() ;
// vertex info ...
	G4double Xo = E->GetPrimaryVertex(0)->GetX0() ;
	G4double Yo = E->GetPrimaryVertex(0)->GetY0() ;
	G4double Zo = E->GetPrimaryVertex(0)->GetZ0() ;
// hits info ...
	HCE = E->GetHCofThisEvent() ;
// init output event record ...
	if (TE) {
		TE->Clear() ;
		TE->SetTime(int(utime),(utime - int(utime)) * 1.e+6) ;
		TE->SetTriggerType(trig_value) ;
		TE->SetVertex(Xo,Yo,Zo) ;
		for (int k = 0 ; k < Nprimary ; k++) {
			TP = TE->AddPrimaryTrack() ;
			G4ThreeVector mom = E->GetPrimaryVertex(k)->GetPrimary()->GetMomentum() ;
			mom.rotateY(-G10XConfiguration::Get()->GetBeamRotationAngle()) ;
			G4String particle_name = E->GetPrimaryVertex(k)->GetPrimary()->GetG4code()->GetParticleName() ;
			TP->SetParticle(G10XCodesPDG::Get()->GetPDGcode(particle_name.c_str()),mom.x(),mom.y(),mom.z()) ; // special codes ...
			//++TP->SetParticle(E->GetPrimaryVertex(k)->GetPrimary()->GetPDGcode(),mom.x(),mom.y(),mom.z()) ; // G4/PDG codes for d,t,He3, etc ...
			}
		if (HCE) {
			int NumOfHC = HCE->GetNumberOfCollections() ;
			for (int k = 0 ; k < NumOfHC ; k++) {
				TrackHitsCollection	*HC = reinterpret_cast<TrackHitsCollection *>(HCE->GetHC(k)) ;
				if (!HC->entries()) continue ; // no hits in the detector ...
				G4String HCname = HC->GetName() ;
				for (int h = 0 ; h < HC->entries() ; h++) {
					G4int dig_pos = 0 , index = 0 ;
					while (!isdigit(HCname.data()[dig_pos]) && dig_pos < HCname.length()) dig_pos++ ;
					G4String name = HCname(0,dig_pos) ;
					if (dig_pos < HCname.length()) {
						while (HCname.data()[dig_pos] == '0') dig_pos++ ;
						index = atoi(G4String(HCname(dig_pos,5))) ;
						}
					G4int detector_id = G10XDetectorIDCodes::Get()->GetDetectorID(name.c_str()) + index ;
					TH = TE->AddHit() ;
					TH->SetDetector(HCname,detector_id) ;
					TH->SetParticle(G10XCodesPDG::Get()->GetPDGcode((*HC)[h]->GetParticleName().c_str()),(*HC)[h]->GetKineticEnergy(),(*HC)[h]->GetEnergyDeposit()) ;
					//++TH->SetParticle(->GetPDGcode(),(*HC)[h]->GetKineticEnergy(),(*HC)[h]->GetEnergyDeposit()) ; // get PDG_code from "particle name" ...
					TH->SetPath((*HC)[h]->GetPath()) ;

					TH->SetTime((*HC)[h]->Time()) ;
					TH->SetPos((*HC)[h]->GetEntryX(),(*HC)[h]->GetEntryY(),(*HC)[h]->GetEntryZ()) ;
					TH->SetDir((*HC)[h]->GetDirection().x(),(*HC)[h]->GetDirection().y(),(*HC)[h]->GetDirection().z()) ;
					}
				}
			}
		TE->SetPrimaryEvents(E->GetEventID(),G10XRunMonitor::Get()->GetAccepted()) ;
		TE->Save() ;
		//++TE->Print() ;
		}
**/
	}

// e-o-f
