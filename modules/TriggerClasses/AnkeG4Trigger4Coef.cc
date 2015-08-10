// File:	AnkeG4Trigger4Coef.cc
// Date:	13-Mar-2012 G.Macharashvili Juelich
// Date:	15-Oct-2012 G.Macharashvili Juelich

#include "AnkeG4Trigger4Coef.hh"

	AnkeG4Trigger4Coef::AnkeG4Trigger4Coef() {
	if (gRFile) {
		gRFile->cd() ;
	        if (!gDirectory->GetDirectory("Trigger4Coef")) gDirectory->mkdir("Trigger4Coef") ;
                gDirectory->cd("Trigger4Coef") ;

        	d2plateXY = new TH2F("d2plateXY","d2plateXY",140,-140,+140,50,-50,+50) ;
        	d2plateXY->SetXTitle("x_{local}     mm") ;
        	d2plateXY->SetYTitle("y_{local}     mm") ;
        	d2plateXY->SetOption("col") ;

        	FdWindowXY = new TH2F("FdWindowXY","FdWindowXY",125,-125,+125,110,-110,+110) ;
        	FdWindowXY->SetXTitle("x_{local}     mm") ;
        	FdWindowXY->SetYTitle("y_{local}     mm") ;
        	FdWindowXY->SetOption("col") ;

        	gRFile->cd() ;

	        }
        }

// called before the event is generated ...
void	AnkeG4Trigger4Coef::init_trigger(const G4Event*) {	}

// "true" discards the event on some reason ...
G4bool	AnkeG4Trigger4Coef::amygdala(const G4Event* E) {
	return false ;
	}

G4bool	AnkeG4Trigger4Coef::cortex(const G4Event* E,int &trig_value) {
// analyse hits in vacWindow, pc1, pc2, pc3, pc4 in Forward system ....
	G4int	detected = 0x0 ;
	if (&trig_value) trig_value = detected ;
	HCE = E->GetHCofThisEvent() ;
	if (!HCE) return false ;
	int NumOfHC = HCE->GetNumberOfCollections() ;
	for (int k = 0 ; k < NumOfHC ; k++) {
		TrackHitsCollection	*HC = reinterpret_cast<TrackHitsCollection *>(HCE->GetHC(k)) ;
		if (0 == HC->entries()) continue ;
		G4String HCname = HC->GetName() ;
		if (HCname.find("FdMWPC") == 0) {
			detected |= 0x1 ;
			}
		else if (HCname.find("FdStop") == 0) {
			detected |= 0x2 ;
			}
		else if (HCname.find("FdWindow") == 0) {
			HCFdWindow = HC ;
			detected |= 0x4 ;
			}
		else if (HCname.find("DXplateVD") == 0) {
			HCd2plate = HC ;
			detected |= 0x8 ;
			}
		} // NumOfHC ;

// set event status ...
	if (&trig_value) trig_value = detected ;
// if good (acceptable) event topology ...
	if (gRFile && detected == 0xf) {
		G4ThreeVector local = (*HCd2plate)[0]->GetLocal() ;
	        d2plateXY->Fill(local[0],local[1]) ;
       	        local = (*HCFdWindow)[0]->GetLocal() ;
                FdWindowXY->Fill(local[0],local[1]) ;
	        }
// end ...
	return (detected == 0xf) ? true : false ;
	}

// eof
