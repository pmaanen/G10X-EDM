// File:	G10XSiModuleSD.hh
// Date:	01-Jul-2014 GM Dubna

#include "G10XSiModuleSD.hh"
#include "G10XSiModuleHit.hh"

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

G10XSiModuleSD::G10XSiModuleSD(G4String name,G4LogicalVolume* xlogVolume) : G4VSensitiveDetector(name), logVolume(xlogVolume) , HitsCollection(0) , HitsCollectionID(-1) {
	collectionName.insert(name) ;
	logVolume->SetSensitiveDetector(this) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(this) ;

	InitHist() ;
	ListPostStepProc.Reset() ;
	ListStepStatus.Reset() ;
	}


G10XSiModuleSD::~G10XSiModuleSD() {
	cout << "G10XSiModuleSD::dtor ..." << endl ;
	ListPostStepProc.Print(cout,"    ") ;
	ListStepStatus.Print(cout,"    ") ;
	cout << endl ;
	}


void G10XSiModuleSD::Initialize(G4HCofThisEvent* hcEvent) {
	HitsCollection = new G10XSiModuleHitsCollection(SensitiveDetectorName,collectionName[0]) ;
	if (HitsCollectionID < 0) { HitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(HitsCollection) ; }
	hcEvent->AddHitsCollection(HitsCollectionID,HitsCollection) ;
	hit = 0 ;
	}


G4bool G10XSiModuleSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
// step status types ...
const   string  SS[8] = { "WorldBoundary","GeomBoundary","AtRestDoItProc","AlongStepDoItProc","PostStepDoItProc","UserDefinedLimit","ExclusivelyForcedProc","Undefined" } ; 
// track status types ...
const   string  TS[6] = { "Alive","AliveStop","StopKill","KillAll","Suspend","Postpone" } ;

	if (step->GetTotalEnergyDeposit() <= 0.0) return false ;
	if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0.) return false ;

	G4Track*		track = step->GetTrack() ;
	G4StepPoint*		preStepPoint = step->GetPreStepPoint() ;
// processes ...
	Proc = const_cast<G4VProcess*>(step->GetPostStepPoint()->GetProcessDefinedStep()) ;
	if (Proc != 0) {
		ProcType = Proc->GetProcessType() ;
		ProcName = Proc->GetProcessName() ;
		ProcTypeName = Proc->GetProcessTypeName(ProcType) ;
		ListPostStepProc.Register(ProcName,ProcType) ;
		}
// step status ...
	ListStepStatus.Register(SS[step->GetPostStepPoint()->GetStepStatus()].c_str()) ;

	G4TouchableHistory*	touchable = (G4TouchableHistory*) (preStepPoint->GetTouchable()) ;
	G4VPhysicalVolume*	motherPhysical = touchable->GetVolume(1) ; // mother // what for ?
				detectorID = preStepPoint->GetPhysicalVolume()->GetCopyNo() ;
	detectorID = 0 ; // consider the calorimeter as one detector ...
				trackID = track->GetTrackID() ;
				particleID = track->GetDefinition()->GetPDGEncoding() ;
	
	G4ThreeVector worldPos = preStepPoint->GetPosition() ;
	G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPos) ;

//++	if ((hit == GetHit(trackID,detectorID))) hit->AddEnergyDeposit(step->GetTotalEnergyDeposit(),step->GetStepLength() ) ;
	if (hit != 0) hit->AddEnergyDeposit(step->GetTotalEnergyDeposit(),step->GetStepLength() ) ;
	else {
		hit = new G10XSiModuleHit(trackID,detectorID) ;
		hit->Set(trackID,detectorID,particleID,preStepPoint->GetGlobalTime(),step->GetTotalEnergyDeposit(),localPos,worldPos) ;
		HitsCollection->insert(hit) ;
		}
	return true ;
	}


G10XSiModuleHit	*G10XSiModuleSD::GetHit(int trackID,int detectorID) {
	for (int h = 0 ; h < HitsCollection->entries() ; h++) {
		if (trackID * 1000000 + detectorID == (*HitsCollection)[h]->GetHitID()) return (*HitsCollection)[h] ;
		}
	return 0 ;
	}


void G10XSiModuleSD::EndOfEvent(G4HCofThisEvent* hcEvent) {
// here fake hits have to be added (noise, cross-talk, background, etc)

// test print ...
//	cout << GetName() << ": evt " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << endl ;
	//++for (int h = 0 ; h < HitsCollection->entries() ; h++) (*HitsCollection)[h]->Print() ;

// tmp hist test ...
	num_hits->Fill(0.01 + HitsCollection->entries()) ;
	if (0 == HitsCollection->entries()) return ;
//++??	GetHist() ;
	G4double totalELoss = 0 , totalPath = 0 ;
	for (int h = 0 ; h < HitsCollection->entries() ; h++) {
		totalELoss += (*HitsCollection)[h]->GetEnergyDeposit() ;
		totalPath += (*HitsCollection)[h]->GetPath() ; // ONLY CHARGED PARTICLES TO SUMMARIZE ...
		glob_xy->Fill((*HitsCollection)[h]->GetWorldPos().x(),(*HitsCollection)[h]->GetWorldPos().y()) ;
		}
	elossH->Fill(totalELoss) ;
	pathH->Fill(totalPath) ;
	}

/**
void G10XSiModuleSD::GetHist() {
	stringstream name(ios::out) ;
	name.seekp (0,ios::beg) ;
	name << "module_" << detectorID/1000 << "." << detectorID%1000 ;
	string sname = name.str() ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.str().c_str())) {
		gDirectory->mkdir(name.str().c_str()) ;
		gDirectory->cd(name.str().c_str()) ;
		elossH = new TH1F((name.str()+"_eloss").c_str(),"energy_deposit",500,0,500.) ;
		elossH->SetXTitle("energy deposit    [MeV]") ;
		elossH->SetYTitle("number of events") ;
		pathH = new TH1F((name.str()+"_path").c_str(),"track path",500,0,50.) ;
		pathH->SetXTitle("track path    [mm]") ;
		pathH->SetYTitle("number of events") ;
		gDirectory->cd("/") ;
		}
	else {
		gDirectory->cd(name.str().c_str()) ;
		elossH = dynamic_cast<TH1F*> (gDirectory->FindObject((name.str()+"_eloss").c_str())) ;
		pathH  = dynamic_cast<TH1F*> (gDirectory->FindObject((name.str()+"_path").c_str())) ;
		}
	}
**/

void G10XSiModuleSD::InitHist() {
	string name("calorimeter_sum") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.c_str())) {
		gDirectory->mkdir(name.c_str()) ;
		gDirectory->cd(name.c_str()) ;
		elossH = new TH1F("sum_eloss","summary energy deposit",500,0,500.) ;
		elossH->SetXTitle("energy deposit    [MeV]") ;
		elossH->SetYTitle("number of events") ;
		pathH = new TH1F("sum_path","summary track path",500,0,100) ;
		pathH->SetXTitle("track path    [mm]") ;
		pathH->SetYTitle("number of events") ;
		num_hits = new TH1F("num_hits","stored hits",20,0,20) ;
		num_hits->SetXTitle("number of stored hits") ;
		num_hits->SetYTitle("number of events") ;
		glob_xy = new TH2F("entry_xy","calorimeter entry point",200,-200,+200,200,-200,+200) ;
		glob_xy->SetXTitle("calorimeter entry x   [mm]") ;
		glob_xy->SetYTitle("calorimeter entry y   [mm]") ;
		glob_xy->SetOption("col") ;
		gDirectory->cd("/") ;
		}
	}

// e-o-f
