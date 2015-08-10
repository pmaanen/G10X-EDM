// File:	GG4ModuleHit.hh
// Date:	02-Mar-2015 G.Macharashvili Juelich


#ifndef GG4ModuleHit_hh
#define GG4ModuleHit_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4VHit.hh>
#include <G4VPhysicalVolume.hh>
#include <G4THitsCollection.hh>

class GG4ModuleHit : public G4VHit {

public:

virtual	~GG4ModuleHit() ;
	GG4ModuleHit(const GG4ModuleHit &r) ;
	GG4ModuleHit(G4int detectorID = 0,G4int status = 0,G4double tt = 0,G4double ee = 0) ;

const	GG4ModuleHit&	operator=(const GG4ModuleHit &r) ;
	void		Add(G4int Xstatus,G4double de) ;

	int		operator==(const GG4ModuleHit &r) const ;
inline	void		*operator new(size_t) ;
inline	void		operator delete(void *h) ;
	void		Print() ;

	G4int		GetHitID() const { return detector_id ; }
	G4int		GetDetectorID() const { return detector_id ; }
//	void		SetStatus(G4int stat) { status = stat ; }

	G4int		GetStatus() const { return status ; }
//	G4int		GetXindex() const { return (detector_id % 1000) ; }
//	G4int		GetYindex() const { return (detector_id / 1000) ; }
//static	G4int		GetDetectorID(int ix,int iy) { return iy * 1000 + ix ; }

	G4double	GetEnergyDeposit() const { return energy_deposit ; }
	G4double	GetTime() const { return entry_time ; }

private:

	G4int		detector_id ;		// Detector full ID. as defuned by <AnkeG4DetectorIDCodes>
	G4int		status ;		// 0 - no primary particle, otherwise == PostStepStatus 
						// 3 - stop, 4 - hadronic interaction (killed) ...
	G4double	entry_time ;		// regardless energy deposit value ...
	G4double	energy_deposit ;	// 0 value permited ...

	} ;

typedef	G4THitsCollection<GG4ModuleHit> 	ModuleHitsCollection ;
extern	G4Allocator<GG4ModuleHit>		ModuleHitsAllocator ;

inline	void*		GG4ModuleHit::operator new(size_t) { return (void *)ModuleHitsAllocator.MallocSingle() ; }
inline	void		GG4ModuleHit::operator delete(void *H) { ModuleHitsAllocator.FreeSingle((GG4ModuleHit*) H); }
	ostream&	operator<< (ostream &os, const GG4ModuleHit &x) ;

#endif

// e-o-f
