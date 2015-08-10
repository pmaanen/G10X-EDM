// File:	GG4ModuleEventAction.cc
// Date:	10-Apr-2014 GM Dubna

#include "GG4ModuleEventAction.hh"

#include <G4Event.hh>
#include <G4RunManager.hh>
#include <G4EventManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4VHitsCollection.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

#include <stdio.h>
#include <iomanip> 
#include <ctime>
using namespace std ;

#include "GG4PhiFitter.hh"

GG4ModuleEventAction::GG4ModuleEventAction() : G4UserEventAction() {
	ellipseCut01 = TCutGCirc("e01",6.3,6.5,1.2,1.2) ;
	if (gRFile) {
	        calorAnalysis = new EdmModularCalorimeterAnalysis ;
	        calorAnalysis->Initialize() ;

		HistInit() ;
		}
	}


GG4ModuleEventAction::~GG4ModuleEventAction() {
	if (gRFile) {
		delete calorAnalysis ;

		GG4PhiFitter *phiFitter = new GG4PhiFitter() ;
		phiFitter->ConnectTH1(Xphi) ;
		phiFitter->Print() ;
		delete phiFitter ;
		}
	}


void GG4ModuleEventAction::BeginOfEventAction(const G4Event*E) {
	eventID = E->GetEventID() + 1 ;
	if (!((eventID)%1000)) cout << setw(5) << eventID/1000 << "k ...         \r" << flush ;
	}     


void GG4ModuleEventAction::EndOfEventAction(const G4Event* event) {
// for graphical version no analysis needed ...
	if (!gRFile) return ;
// start analysis ...
	G4HCofThisEvent* HCE = event->GetHCofThisEvent() ;
	if (!HCE) return ;
	int numOfHC = HCE->GetNumberOfCollections() ;
	if (numOfHC < 1) return ;
	for (int k = 0 ; k < 20 ; k++) moduleH[k] = 0 ;
	for (int k = 0 ; k < numOfHC ; k++) {
		HC = dynamic_cast<ModuleHitsCollection *>(HCE->GetHC(k)) ;
		G4String HCname = HC->GetName() ;
		if (HC->entries() > 0) {
			if (HCname == "abs.0") moduleH[10] = HC ;
			else if (HCname == "abs.1") moduleH[11] = HC ;
			else if (HCname == "sens.0") moduleH[0] = HC ;
			else if (HCname == "sens.1") moduleH[1] = HC ;
			else if (HCname == "calor.M") moduleH[2] = HC ; // it is calorimeter ...
			}
		}
	if (!moduleH[0] || !moduleH[1] || !moduleH[2]) return ;
	numH[0] = moduleH[0]->entries() ;
	numH[1] = moduleH[1]->entries() ;
	numH[2] = moduleH[2]->entries() ;
// good event selection ...
	if (!numH[0] || !numH[1] || !numH[2]) return ;
// fill in histograms ...
	G4int scintOK = 0 ;
	for (G4int ho = 0 ; ho < numH[0] ; ho++) {
		for (G4int h1 = 0 ; h1 < numH[1] ; h1++) {
			G4double e0 = (*moduleH[0])[ho]->GetEnergyDeposit() ;
			G4double e1 = (*moduleH[1])[h1]->GetEnergyDeposit() ;
			G4double e2 = (*moduleH[2])[0]->GetEnergyDeposit() ; // fixed 1 hit only for DISK calorimeter ...
			if ((*moduleH[0])[ho]->GetDetectorID() == (*moduleH[1])[h1]->GetDetectorID()) { // the same sectors ...
				ee_0_1->Fill(e1,e0) ;
				ee_0_calor->Fill(e2,e0) ;
				ee_1_calor->Fill(e2,e1) ;

				if (ellipseCut01->IsInside(e0,e1)) {
					scintOK = 1 ;
					ee_sel->Fill(e0+e1+e2) ;
					ee_0_1_s->Fill(e1,e0) ;
					G4double phi = (*moduleH[0])[ho]->GetDetectorID() *10 + 0.1 ;
					if (phi > 180) phi -= 360.0 ;
					Xphi->Fill(phi) ;
					}
				}
			}
		}


// ====================================
	if (scintOK) calorAnalysis->ProcessEvent(event,moduleH[2]) ;
// ====================================



	}

void GG4ModuleEventAction::HistInit() {
	TString dir("GG4ModuleEventAction") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir) ;
	gDirectory->cd(dir) ;

	ee_0_1 = new TH2F("ee_0_1","[0] vs [1]",200,0,60,200,0,60) ;
	ee_0_1->SetXTitle("sensitive [1]  #Delta E    [MeV]") ;
	ee_0_1->SetYTitle("sensitive [0]  #Delta E    [MeV]") ;
	ee_0_1->SetOption("col") ;

	ee_0_1_s = new TH2F("ee_0_1_s","[0] vs [1] selected",120,0,12,120,0,12) ;
	ee_0_1_s->SetXTitle("sensitive [1]  #Delta E    [MeV]") ;
	ee_0_1_s->SetYTitle("sensitive [0]  #Delta E    [MeV]") ;
	ee_0_1_s->SetOption("col") ;

	ee_0_calor = new TH2F("ee_0_calor","[0] vs [calor]",200,0,300,200,0,20) ;
	ee_0_calor->SetXTitle("calorimeter #Delta E    [MeV]") ;
	ee_0_calor->SetYTitle("sensitive [0]  #Delta E    [MeV]") ;
	ee_0_calor->SetOption("col") ;

	ee_1_calor = new TH2F("ee_1_calor","[1] vs [calor]",200,0,300,200,0,20) ;
	ee_1_calor->SetXTitle("calorimeter #Delta E    [MeV]") ;
	ee_1_calor->SetYTitle("sensitive [1]  #Delta E    [MeV]") ;
	ee_1_calor->SetOption("col") ;

	ee_sel = new TH1F("ee_sel","total #DeltaE [selected]",300,0,300) ;
	ee_sel->SetXTitle("#Sigma #DeltaE    [MeV]") ;
	ee_sel->SetYTitle("num of events") ;

	Xphi   = new TH1F("phi",  "phi",  36,-180.0,+180.0) ;
	Xphi->SetXTitle("#phi    [deg]") ;
	Xphi->SetYTitle("number of events") ;
	Xphi->SetOption("e") ;

/*
	h_theta = new TH1F("theta_lab","theta lab",  90,   0.0,  90.0) ;
	h_theta->SetXTitle("#vartheta_{lab}    [deg]") ;
	h_theta->SetYTitle("number of events") ;
	h_theta->SetOption("e") ;

*/

	gDirectory->cd("/") ;

	}

TCutG	*GG4ModuleEventAction::TCutGCirc(const char *name,double xc,double yc,double xr,double yr) {
	const	int	N=18 ;
	const	double	sa=2*acos(-1.0)/N ;
	double	x[N+1] , y[N+1] ;
	if (yr <= 0.0) yr=xr ;
	for (int k = 0 ; k <= N ; k++) {
		x[k] = xc+xr*cos(k*sa) ;
		y[k] = yc+yr*sin(k*sa) ;
		}
	return new TCutG(name,N+1,x,y) ;
	}


// e-o-f
