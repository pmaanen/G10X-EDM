// File:	GG4TrackModuleSD.cc
// Date:	01-Mar-2015 G.Macharashvili Juelich

#include "GG4TrackModuleSD.hh"
#include <G4VisExtent.hh>

GG4TrackModuleSD::GG4TrackModuleSD(G4String detectorName,G4LogicalVolume *LogV) : G4VSensitiveDetector(detectorName) {
	collectionName.push_back("track_module_HC") ;
	collectionName.push_back("module_HC") ;
	LogVol = LogV ;
	LogVol->SetSensitiveDetector(this) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(this) ;
	hitsCollectionID = -1 ;
	moduleHcollectionID = -1 ;
        topTransform = 0 ;
        HistInit() ;
// Calorimeter analysis at EndOfEvent() ...
	calorimeterAnalysis.Initialize() ;
// ...
	}

GG4TrackModuleSD::~GG4TrackModuleSD() {
        if (topTransform) delete topTransform ;
	}

void	GG4TrackModuleSD::Initialize(G4HCofThisEvent* HCE) {
// track-module HC ...
	hitsCollection = new TrackModuleHitsCollection(GetName(),collectionName[0]) ;
	if (hitsCollectionID < 0) detectorID = hitsCollectionID = GetCollectionID(0) ;
	HCE->AddHitsCollection(hitsCollectionID,hitsCollection) ;
// module HC ...
	moduleHcollection = new ModuleHitsCollection(GetName(),collectionName[1]) ;
	if (moduleHcollectionID < 0) moduleHcollectionID = GetCollectionID(1) ;
	HCE->AddHitsCollection(moduleHcollectionID,moduleHcollection) ;
// save event seq number ...
	E = G4EventManager::GetEventManager()->GetConstCurrentEvent() ;
	eventID = E->GetEventID() ;
// clear before particle enters the detector ...
	Eo = xo = yo = 0.0 ;
	}


G4bool GG4TrackModuleSD::ProcessHits(G4Step* step,G4TouchableHistory* ROhist) {
// these definitions are needed to keep Eo/xo/yo etc data for analysis at G4EventAction ...
//	E->SetUserInformation(new GG4PrimaryEventInformation(eventStatus,Tp,eventWeight,Eo,xo,yo,...)) ;
//	E->GetUserInformation() ... ???
//	E->GetPrimaryVertex(t)->GetPrimary()->SetUserInformation(new GG4PrimaryParticleInformation(0,0)) ;
// ==========================================
// get top transform for the detector physical volume ... (is not needed at the moment) 
        if (!topTransform) {
                touchable = step->GetTrack()->GetTouchable() ;
		topTransform = new G4AffineTransform ;
		topTransform->SetNetTranslation(touchable->GetTranslation()) ;
		topTransform->SetNetRotation(*touchable->GetRotation()) ;
		topTransform->Invert() ;
		}
// ==========================================
// step analysis ...
	stepAnalysis.Diagnostics(step,"deuteron") ;
	if (stepAnalysis.EntryStatus()) {}
	if (stepAnalysis.EndStatus()) {}
// ==========================================
// check if very first entry ...
	if (0.0 == Eo) {
		entry.x = step->GetPreStepPoint()->GetPosition().x() ;
		entry.y = step->GetPreStepPoint()->GetPosition().y() ;
		entry.E = step->GetPreStepPoint()->GetKineticEnergy() ;
		}
// ==========================================
	if (step->GetTotalEnergyDeposit() <= 0.0) return false ;
	if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0.) return false ;
// define module and track to identify a hit ...
	trackID = step->GetTrack()->GetTrackID() ;
	detectorID = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() ;
// generate new hit or update the existing ...
	if ((H = GetHit(trackID,detectorID))) H->Add(stepAnalysis.EndStatus(),step) ;
	else hitsCollection->insert(new GG4TrackModuleHit(trackID,detectorID,stepAnalysis.EntryStatus(),step,topTransform)) ;
	return true ;
	}

void	GG4TrackModuleSD::EndOfEvent(G4HCofThisEvent* HCE) {
	if (0 == gRFile) return ;
// tmp hist test ...
	num_hits->Fill(0.01 + hitsCollection->entries()) ;

	if (0 == hitsCollection->entries()) return ;
// generate module hits collection ...
	G4double totalELoss = 0 , totalPath = 0 , activeModules = 0 ;
	for (int h = 0 ; h < hitsCollection->entries() ; h++) {
	        GG4TrackModuleHit *hit = (*hitsCollection)[h] ;
//++	        hit->Print() ;
// generate new moduleH or update the existing ...
        	if ((mH = GetHit(hit->GetDetectorID()))) mH->Add(hit->GetStatus(),hit->GetEnergyDeposit()) ;
        	else moduleHcollection->insert(new GG4ModuleHit(hit->GetDetectorID(),hit->GetStatus(),hit->GetTime(),hit->GetEnergyDeposit(),hit->GetPhysical())) ;
// moduleH =======================================
		eloss1->Fill(hit->GetEnergyDeposit()) ;
	        if (hit->GetEnergyDeposit() > 0.001) {
	                activeModules++ ;
	                totalELoss += hit->GetEnergyDeposit() ;
			if (hit->GetParticle()->GetPDGCharge() != 0.0) totalPath += hit->GetPath() ;
	                glob_xy->Fill(hit->GetEntryX(),hit->GetEntryY()) ;
	                }
		}
	activeMod->Fill(0.01 + activeModules) ;
	elossH->Fill(totalELoss) ;
	pathH->Fill(totalPath) ;
//++cout << "  HCollection: " << moduleHcollection->GetName() << endl ;
//++	for (int h = 0 ; h < moduleHcollection->entries() ; h++) {
//++		(*moduleHcollection)[h]->Print() ;
//++		}

// analyse moduleHC 
	calorimeterAnalysis.ProcessEvent(moduleHcollection,&entry) ;

	}

GG4TrackModuleHit	*GG4TrackModuleSD::GetHit(int t,int d) {
	G4int hitID = GG4TrackModuleHit::GetHitID(t,d) ;
	for (int h = 0 ; h < hitsCollection->entries() ; h++) {
		if (hitID == (*hitsCollection)[h]->GetHitID()) return (*hitsCollection)[h] ;
		}
	return 0 ;
	}

GG4ModuleHit	*GG4TrackModuleSD::GetHit(int d) {
	for (int h = 0 ; h < moduleHcollection->entries() ; h++) {
		if (d == (*moduleHcollection)[h]->GetHitID()) return (*moduleHcollection)[h] ;
		}
	return 0 ;
	}

void GG4TrackModuleSD::HistInit() {
	string name("GG4TrackModuleSD") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.c_str())) {
		gDirectory->mkdir(name.c_str()) ;
		gDirectory->cd(name.c_str()) ;
		elossH = new TH1F("eloss_sum","summary energy deposit",500,0,500.) ;
		elossH->SetXTitle("energy deposit    [MeV]") ;
		elossH->SetYTitle("number of events") ;
		eloss1 = new TH1F("eloss_mod","energy deposit in modules",500,0,500.) ;
		eloss1->SetXTitle("energy deposit    [MeV]") ;
		eloss1->SetYTitle("number of events") ;
		pathH = new TH1F("sum_path","summary track path",1000,0,500) ;
		pathH->SetXTitle("track path    [mm]") ;
		pathH->SetYTitle("number of events") ;
		num_hits = new TH1F("num_hits","stored hits",40,0,40) ;
		num_hits->SetXTitle("number of stored hits") ;
		num_hits->SetYTitle("number of events") ;
		glob_xy = new TH2F("entry_xy","calorimeter entry point",200,-250,+250,200,-250,+250) ;
		glob_xy->SetXTitle("calorimeter entry x   [mm]") ;
		glob_xy->SetYTitle("calorimeter entry y   [mm]") ;
		glob_xy->SetOption("col") ;
		activeMod = new TH1F("active_modules","active modules",60,0,60) ;
		activeMod->SetXTitle("number of active modules") ;
		activeMod->SetYTitle("number of events") ;

		gDirectory->cd("/") ;
		}

        }
        
// e-o-f
