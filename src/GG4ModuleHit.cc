// File:	GG4ModuleHit.cc
// Date:	02-Mar-2015 G.Macharashvili Juelich

#include "GG4ModuleHit.hh"

G4Allocator<GG4ModuleHit>	ModuleHitsAllocator ;

GG4ModuleHit::~GG4ModuleHit() {}

GG4ModuleHit::GG4ModuleHit(G4int detectorID,G4int Xstatus,G4double tt,G4double ee) {
	detector_id =		detectorID ;
	status =		Xstatus ;
	entry_time =		tt ;
	energy_deposit =	ee ;
	}

GG4ModuleHit::GG4ModuleHit(const GG4ModuleHit &r) { *this = r ;	}

void	GG4ModuleHit::Add(G4int Xstatus,G4double de) {
	energy_deposit += de ;
	if (Xstatus == 0 || Xstatus > 4) return ;
	if ( status == 4) return ;
	if ( status == 0 || status == 3) status = Xstatus ;
	}

const	GG4ModuleHit& GG4ModuleHit::operator=(const GG4ModuleHit &r) {
	if (this != &r) {
		detector_id	= r.detector_id ;
		status		= r.status ;
		entry_time	= r.entry_time ;
		energy_deposit	= r.energy_deposit ;
		}
	return *this ;
	}

int	GG4ModuleHit::operator==(const GG4ModuleHit &r) const { return ( this == &r) ; }

void	GG4ModuleHit::Print() { cout << *this << endl ; }

ostream&   operator<< (ostream &os,const GG4ModuleHit &x) {
	os << fixed << showpoint << right ;
	os << "    [module] id: " << setw(2) << x.GetDetectorID() << "  " ;
	os << "ss: " << setw(1) << x.GetStatus() << "  " ;
	os << "t:" << setw(6) << setprecision(2) << x.GetTime() << "  " ;
	os << "e:" << setw(7) << setprecision(3) << x.GetEnergyDeposit() << "  " ;
	os << flush ;
        return os ;
        }

// e-o-f
