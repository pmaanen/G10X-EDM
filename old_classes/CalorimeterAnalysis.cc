// File:	CalorimeterAnalysis.cc
// Date:	01-Mar-2015 G.Macharashvili Juelich

#include "CalorimeterAnalysis.hh"
#include <ConfigFileReader.hh>
#include <Randomize.hh>

CalorimeterAnalysis::CalorimeterAnalysis() {
// get beam parameters ...
	ConfigFileReader *config = new ConfigFileReader("config.dat") ;
	asymmetry_PA = 0.0 ;
	beamPolarizationPhi = 0.0 ;
	config->getDouble("asymmetry_PA",asymmetry_PA) ;
	config->getDouble("beam_polarization_phi",beamPolarizationPhi) ;
	z_pos = 400.0 ;
	config->getDouble("cal.distance.z",z_pos) ;
	delete config ;
	}

CalorimeterAnalysis::~CalorimeterAnalysis() {
	cout << "GG4calorimeterAnalysis::dtor ..." << endl ;
	cout << endl ;
// do fit to extract beam parameters ...
	phiFitter = new GG4PhiFitter() ;
	phiFitter->SetGeneratedParameters(asymmetry_PA,beamPolarizationPhi) ;
	phiFitter->ConnectTH1(Hd_phi) ;
	phiFitter->Print() ;
	delete phiFitter ;
	}

void	CalorimeterAnalysis::Initialize() {
	HistInit() ;
	}

void	CalorimeterAnalysis::ProcessEvent(ModuleHitsCollection* HC,CalorimeterEntry *entry) {
	if (0 == HC || 0 == gRFile) return ;
	moduleHcollection = HC ;
//++??	activeAll->Fill(0.01 + HC->entries()) ;
	if (0 == HC->entries()) return ;
// -----------------
	hmax = 0 ;
	int Nactive = 0 ;
	etot = 0 ;
	for (int h = 0 ; h < HC->entries() ; h++) {
        	hit = (*HC)[h] ;
	        if (hit->GetEnergyDeposit() < 0.01) continue ;
	        Hstat->Fill(hit->GetStatus()) ;
	        HstatE->Fill(hit->GetEnergyDeposit(),hit->GetStatus()) ;
                etot += hit->GetEnergyDeposit() ;
	        Nactive++ ;
	        if (!hmax) hmax = hit ;
	        else if (hmax->GetEnergyDeposit() < hit->GetEnergyDeposit()) hmax = hit ;
	        }
        if (!hmax) return ;
        activeM->Fill(0.01 + Nactive) ;
	Hetot->Fill(etot) ;
// -----------------
//	if (hmax->GetEnergyDeposit() < 50) return ;
	e3x3 = 0 , x3x3 = 0 , y3x3 = 0 ;
	G4double  e , x , y ;
//--        ix = hmax->GetXindex() ; iy = hmax->GetYindex() ;
        Nactive = 0 ;
        G4int d_enter = 0 , d_killed = 0 , d_stopped = 0 ;
        for (int i = ix - 1 ; i <= ix + 1 ; i++) {
                for (int j = iy - 1 ; j <= iy + 1 ; j++) {
//--			hit = GetHit(GG4ModuleHit::GetDetectorID(i,j)) ;
			hit = 0 ; // ???????????
			if (!hit) continue ;
//--hit->Print() ;
                        G4int ss = hit->GetStatus() ;
                        if (ss/10 == 1) d_enter = 1 ;
                        if (ss%10 == 2) d_stopped = 1 ;
                        if (ss%10 == 3) d_killed = 1 ;
                        e = hit->GetEnergyDeposit() ;
//--x = hit->GetPhysical()->GetTranslation().x() ;
//--y = hit->GetPhysical()->GetTranslation().y() ;
			e3x3 += e ;
			x3x3 += e*x ;
			y3x3 += e*y ;
			Nactive++ ;
                        }
                }
	x3x3 /= e3x3 ;
	y3x3 /= e3x3 ;
//	if (Nactive > 6) return ;
        activeM3x3->Fill(0.01 + Nactive) ;
	He3x3->Fill(e3x3) ;
	hit_xy->Fill(x,y) ;
	if (1 == d_enter) {
		Hd_tot->Fill(e3x3) ;
		if (d_stopped) Hd_stop->Fill(e3x3) ;
		if (d_killed) Hd_kill->Fill(e3x3) ;
		}
        else Hd_unkn->Fill(e3x3) ;
// select stopped deuteron and plot phi/theta ...
	if (e3x3 > 260.0 && e3x3 < 280.0) {
		double sigma = 4.0 ;
		x = CLHEP::RandGauss::shoot(entry->x,sigma) ;
		y = CLHEP::RandGauss::shoot(entry->y,sigma) ;
		double phi = atan2(y,x) / acos(-1.0) * 180.0 ;
		double thetaLab = atan(sqrt(x*x+y*y)/z_pos) / acos(-1.0) * 180.0 ;
		Hd_phi->Fill(phi) ;
		Hd_thetaLab->Fill(thetaLab) ;
		}

	}

GG4ModuleHit	*CalorimeterAnalysis::GetHit(int d) {
	for (int h = 0 ; h < moduleHcollection->entries() ; h++) {
		if (d == (*moduleHcollection)[h]->GetHitID()) return (*moduleHcollection)[h] ;
		}
	return 0 ;
	}

void CalorimeterAnalysis::HistInit() {
	string name("CalorimeterAnalysis") ;
	gDirectory->cd("/") ;
	if (!gDirectory->GetDirectory(name.c_str())) {
		gDirectory->mkdir(name.c_str()) ;
		gDirectory->cd(name.c_str()) ;

		Hetot = new TH1F("energy_tot","summary energy deposit",500,0,500.) ;
		Hetot->SetXTitle("energy deposit    [MeV]") ;
		Hetot->SetYTitle("number of events") ;
		He3x3 = new TH1F("energy_3x3","energy deposit in modules around max",500,0,500.) ;
		He3x3->SetXTitle("energy deposit    [MeV]") ;
		He3x3->SetYTitle("number of events") ;
		
		activeM = new TH1F("active_M","stored hits",40,0,40) ;
		activeM->SetXTitle("number of stored hits") ;
		activeM->SetYTitle("number of events") ;
	
		hit_xy = new TH2F("hit_xy","calorimeter entry point",100,-250,+250,100,-250,+250) ;
		hit_xy->SetXTitle("calorimeter entry x   [mm]") ;
		hit_xy->SetYTitle("calorimeter entry y   [mm]") ;
		hit_xy->SetOption("col") ;

		activeM3x3 = new TH1F("active_M_3x3","active modules",20,0,20) ;
		activeM3x3->SetXTitle("number of active modules") ;
		activeM3x3->SetYTitle("number of events") ;

		HstatE = new TH2F("H_status_E","status vs #DeltaE",200,0,500,20,0,20.) ;
		HstatE->SetXTitle("status") ;
		HstatE->SetYTitle("number of events") ;
		HstatE->SetOption("col") ;

		Hstat = new TH1F("H_status","status versions",20,0,20.) ;
		Hstat->SetXTitle("status") ;
		Hstat->SetYTitle("number of events") ;

		Hd_tot = new TH1F("d_tot","energy deposit in modules around max",550,0,550.) ;
		Hd_tot->SetXTitle("energy deposit    [MeV]") ;
		Hd_tot->SetYTitle("number of events") ;
		Hd_stop = (TH1F *)Hd_tot->Clone("d_stop") ;
		Hd_kill = (TH1F *)Hd_tot->Clone("d_kill") ;
		Hd_unkn = (TH1F *)Hd_tot->Clone("d_unkn") ;
		
		Hd_phi = new TH1F("Ad_phi"," #phi angle of detected deuteron ",  40,-180.0,+180.0) ;
		Hd_phi->SetXTitle("#phi    [deg]") ;
		Hd_phi->SetYTitle("number of events") ;
		Hd_phi->SetOption("e") ;

		Hd_thetaLab = new TH1F("theta_lab"," #vartheta_{lab} ",70,0,30) ;
		Hd_thetaLab->SetXTitle("#vartheta_{lab}   [deg]") ;
		Hd_thetaLab->SetYTitle("num. of events") ;

		gDirectory->cd("/") ;
		}

        }
        
// e-o-f
