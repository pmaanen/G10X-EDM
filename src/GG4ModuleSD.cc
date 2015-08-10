// File:	GG4ModuleSD.hh
// Date:	10-Apr-2015 GM Juelich

#include "GG4ModuleSD.hh"

#include <G4VSolid.hh> 

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

//#define Verb 1

GG4ModuleSD::GG4ModuleSD(G4LogicalVolume *log) : G4VSensitiveDetector(log->GetName()) {
	logicalVolume = log ;
	moduleHcollection = 0 ;
	collectionName.push_back(log->GetName()) ;
	moduleHcollectionID = -1 ;

	if (gRFile) HistInit() ;

	cout << showpoint << fixed ;
	cout << left ;
	cout << setw(10) << logicalVolume->GetSolid()->GetEntityType() << " " ;
	cout << " det: " << setw(8) << GetName() << " " ;
	cout << " mat: " << setw(26) << logicalVolume->GetMaterial()->GetName() << "  " ;
	cout << " density: " << setw(5) << setprecision(2) << logicalVolume->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3) << "  " ;
	cout << " Xo: " << scientific << setw(8) << setprecision(3) << logicalVolume->GetMaterial()->GetRadlen() << "  " ;
	cout << right << endl ;

	}


GG4ModuleSD::~GG4ModuleSD() {}


void GG4ModuleSD::Initialize(G4HCofThisEvent *eventHC) {
// module H Collection ctor ... called in each event ...
	moduleHcollection = new ModuleHitsCollection(GetName(),collectionName[0]) ;
	if (moduleHcollectionID < 0) moduleHcollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(moduleHcollection) ;
	eventHC->AddHitsCollection(moduleHcollectionID,moduleHcollection) ;
	d_total_path = 0 ;
	}


G4bool GG4ModuleSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
	if (step->GetTotalEnergyDeposit() < 0.001 * CLHEP::MeV) return false ;
	detectorID = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() ;
	track = step->GetTrack() ;
	particleName = track->GetParticleDefinition()->GetParticleName() ;

#ifdef Verb
	cout << left ;
	cout << "SD: " << setw(9) << GetName() << " " << flush ;
	cout << "id:" << setw(6) << detectorID << " " ;
	cout << fixed << showpoint << setprecision(2) ;
	cout <<	setw(9) << particleName << " " << flush ;
	cout <<	setw(9) << step->GetPreStepPoint()->GetPhysicalVolume()->GetName() << " " << flush ;
	cout << "t: " << setw(2) << track->GetTrackID() << "  " ;
	cout << "Ek: " << setw(6) << step->GetPreStepPoint()->GetKineticEnergy() << "  " ;
	cout << "ee: " << setw(6) << step->GetTotalEnergyDeposit() << "  " ;
#endif

/*
fWorldBoundary          0 - Step reached the world boundary
fGeomBoundary           1 - Step defined by a geometry boundary
fAtRestDoItProc         2 - Step defined by a PreStepDoItVector
fAlongStepDoItProc      3 - Step defined by a AlongStepDoItVector (hIoni)
fPostStepDoItProc       4 - Step defined by a PostStepDoItVector (Hadronic ?)
fUserDefinedLimit       5 - Step defined by the user Step limit in the logical volume
fExclusivelyForcedProc  6 - Step defined by an exclusively forced PostStepDoIt process 
fUndefined              7 - Step not defined yet 
*/

// define elastic deuteron parameters ...
	postStepStatus = step->GetPostStepPoint()->GetStepStatus() ;
	if (track->GetTrackID() != 1) postStepStatus = 0 ;
	else d_total_path += step->GetStepLength() ;
// generate new hit or update the existing ...
	int newHitNeeded = (!(moduleH = GetHit(detectorID))) ? 1 : 0 ;
	if (newHitNeeded) moduleHcollection->insert(new GG4ModuleHit(detectorID,postStepStatus,step->GetPreStepPoint()->GetGlobalTime(),step->GetTotalEnergyDeposit())) ;
	else moduleH->Add(postStepStatus,step->GetTotalEnergyDeposit()) ;

#ifdef Verb
	cout << "newH: " << newHitNeeded << " " ;
#endif

	if (gRFile != 0 && step->GetPreStepPoint()->GetStepStatus() == 1) {

#ifdef Verb
	cout << "h.1 " ;
#endif
// note: GM-20150602
// Add here: plot energy deposits for havy ions (with masses >= alpha mass) ...
		G4int particleIndex = 0 ;
		if (particleName == "deuteron") particleIndex = 1 ;
		else if (particleName == "proton") particleIndex = 2 ;
		else if (particleName == "neutron") particleIndex = 3 ;
		else if (particleName == "alpha") particleIndex = 4 ;
		partIndex->Fill(particleIndex + 0.1) ;
		}

#ifdef Verb
	cout << "e  " << right << endl ;
#endif

	return true ;
	}

GG4ModuleHit	*GG4ModuleSD::GetHit(int det_id) {
	for (int h = 0 ; h < moduleHcollection->entries() ; h++) {
		if (det_id == (*moduleHcollection)[h]->GetHitID()) return (*moduleHcollection)[h] ;
		}
	return 0 ;
	}

void GG4ModuleSD::EndOfEvent(G4HCofThisEvent *eventHC) {
//++	cout << endl ;
//++	for (int h = 0 ; h < moduleHcollection->entries() ; h++) (*moduleHcollection)[h]->Print() ;
	if (gRFile != 0  && moduleHcollection->entries() > 0) {
		for (G4int h = 0 ; h < moduleHcollection->entries() ; h++) {
			tent->Fill((*moduleHcollection)[h]->GetTime()) ;
			path->Fill(d_total_path) ;
			e_0->Fill((*moduleHcollection)[h]->GetEnergyDeposit()) ;
			if ((*moduleHcollection)[h]->GetStatus() == 4) e_1->Fill((*moduleHcollection)[h]->GetEnergyDeposit()) ;
			if ((*moduleHcollection)[h]->GetStatus() == 3) e_2->Fill((*moduleHcollection)[h]->GetEnergyDeposit()) ;
			}
		}
	}


void GG4ModuleSD::HistInit() {
	if (e_0) return ;
	G4int nx = 200 ;
	G4double eDepMax = 300.0 * CLHEP::MeV ;
	G4double pathMax = 100.0 * CLHEP::mm ;
	TString dir("GG4ModuleSD") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;

	dir = collectionName[0] ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;
	     if (dir.Contains("beam")) { nx = 200 ; eDepMax =  50.0 * CLHEP::MeV ; pathMax = 20.0 * CLHEP::mm ; }
	else if (dir.Contains("sens")) { nx = 100 ; eDepMax = 100.0 * CLHEP::MeV ; pathMax = 20.0 * CLHEP::mm ; }
	else if (dir.Contains("abs"))  { nx = 100 ; eDepMax = 100.0 * CLHEP::MeV ; pathMax =  5.0 * CLHEP::mm ; }

	e_0 = new TH1F(dir+"_e","total #DeltaE [all]",nx,0,eDepMax) ;
	e_0->SetXTitle("#DeltaE    [MeV]") ;
	e_0->SetYTitle("num of events") ;

	e_1 = new TH1F(dir+"_e_1","total #DeltaE [kill]",nx,0,eDepMax) ;
	e_1->SetXTitle("#DeltaE    [MeV]") ;
	e_1->SetYTitle("num of events") ;

	e_2 = new TH1F(dir+"_e_2","total #DeltaE [stop]",nx,0,eDepMax) ;
	e_2->SetXTitle("#DeltaE    [MeV]") ;
	e_2->SetYTitle("num of events") ;

	tent = new TH1F(dir+"_time","entry time ",500,0,50) ;
	tent->SetXTitle("global time    [ns]") ;
	tent->SetYTitle("num of events") ;

	partIndex = new TH1F(dir+"_particle","particle index ",10,0,10) ;
	partIndex->SetXTitle("particle index") ;
	partIndex->SetYTitle("num of events") ;
	partIndex->GetXaxis()->SetBinLabel(1,"X") ;
	partIndex->GetXaxis()->SetBinLabel(2,"d") ;
	partIndex->GetXaxis()->SetBinLabel(3,"p") ;
	partIndex->GetXaxis()->SetBinLabel(4,"n") ;
	partIndex->GetXaxis()->SetBinLabel(5,"#alpha") ;

	path = new TH1F(dir+"_path","particle path ",100,0,pathMax) ;
	path->SetXTitle("total path   [mm]") ;
	path->SetYTitle("num of events") ;

	gDirectory->cd("/") ;

	}


// e-o-f
