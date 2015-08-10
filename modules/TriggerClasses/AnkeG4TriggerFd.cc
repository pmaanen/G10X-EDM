// File:	AnkeG4TriggerFd.cc
// Date:	17-Oct-2012 G.Macharashvili Juelich

// HAS TO MODIFIED FOR SPECIFIC PURPOSES ...

#include "AnkeG4TriggerFd.hh"

void	AnkeG4TriggerFd::init_trigger(const G4Event*) {	}

G4bool	AnkeG4TriggerFd::amygdala(const G4Event* E) {	return false ; }

G4bool	AnkeG4TriggerFd::cortex(const G4Event* E,int &trig_value) {
	G4int	detected = 0x0 ;
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
			detected |= 0x4 ;
			}
		else if (HCname.find("DXplateVD") == 0) {
			detected |= 0x8 ;
			}
		} // NumOfHC ;

// set event status ...
	if (detected == 0xf) return true ;
	return false ;
	}

// eof
