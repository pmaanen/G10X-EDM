// File:	G10XModuleSD.hh
// Date:	11-mar-2015 GM Juelich
// Date:	01-Jul-2014 GM Dubna

#include "G10XModuleSD.hh"

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

G10XModuleSD::G10XModuleSD(G4String name,G4LogicalVolume* xlogVolume) : G4VSensitiveDetector(name) {
	xlogVolume->SetSensitiveDetector(this) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(this) ;
	InitHist() ;
	}


G10XModuleSD::~G10XModuleSD() {}


void G10XModuleSD::Initialize(G4HCofThisEvent*) {
	energyLoss = 0 ;
	killed = 0 ;
	}


G4bool G10XModuleSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
	track = step->GetTrack() ;
// any particle ...
	energyLoss += step->GetTotalEnergyDeposit() ;
// primary deuteron ...
	if (track->GetTrackID() == 1) {
// primary deuteron entering the detector ...
		if (step->GetPreStepPoint()->GetStepStatus() == 1) {
			h_phi->Fill(step->GetPreStepPoint()->GetMomentumDirection().phi()/CLHEP::deg) ;
			h_thetaLab->Fill(step->GetPreStepPoint()->GetMomentumDirection().theta()/CLHEP::deg) ;
			}
// primary deuteron losses identity ...
		if (step->GetPostStepPoint()->GetStepStatus() == 4 && step->GetPostStepPoint()->GetKineticEnergy() < 0.1) killed = 1 ;
		}
	return true ;
	}


void G10XModuleSD::EndOfEvent(G4HCofThisEvent*) {
	h_edep->Fill(energyLoss) ;
	if (killed) h_edepK->Fill(energyLoss) ;
	else h_edepS->Fill(energyLoss) ;
	}


void G10XModuleSD::InitHist() {
	string name(GetName()) ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.c_str())) {
		gDirectory->mkdir(name.c_str()) ;
		gDirectory->cd(name.c_str()) ;
		G4double Emax = 300.0 ;

		h_edep = new TH1F("edep_tot","summary energy deposit",200,0,Emax) ;
		h_edep->SetXTitle("energy deposit    [MeV]") ;
		h_edep->SetYTitle("number of events") ;

		h_edepS = new TH1F("edep_tot_stopped","summary energy deposit (d)",200,0,Emax) ;
		h_edepS->SetXTitle("energy deposit    [MeV]") ;
		h_edepS->SetYTitle("number of events") ;

		h_edepK = new TH1F("edep_tot_killed","summary energy deposit ",200,0,Emax) ;
		h_edepK->SetXTitle("energy deposit    [MeV]") ;
		h_edepK->SetYTitle("number of events") ;

		h_thetaLab = new TH1F("theta_lab"," #vartheta_{lab} ",60,0,60) ;
		h_thetaLab->SetXTitle("#vartheta_{lab}   [deg]") ;
		h_thetaLab->SetYTitle("num. of events") ;

		h_phi = new TH1F("phi"," #phi ",180,-180,+180) ;
		h_phi->SetXTitle("#phi   [deg]") ;
		h_phi->SetYTitle("num. of events") ;

		gDirectory->cd("/") ;
		}
	}

// e-o-f
