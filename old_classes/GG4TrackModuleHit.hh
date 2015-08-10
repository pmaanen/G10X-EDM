// File:	GG4TrackModuleHit.hh
// Date:	01-Mar-2015 G.Macharashvili Juelich


#ifndef GG4TrackModuleHit_hh
#define GG4TrackModuleHit_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <G4VHit.hh>
#include <G4VPhysicalVolume.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>
#include <G4AffineTransform.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4Step.hh>
#include <G4String.hh>

class GG4TrackModuleHit : public G4VHit {

public:

virtual	~GG4TrackModuleHit() ;
	GG4TrackModuleHit() ;
	GG4TrackModuleHit(const GG4TrackModuleHit &r) ;
	GG4TrackModuleHit(G4int track_id,G4int detectorID,G4int status,G4Step *s,const G4AffineTransform *topTransform = 0) ;

const	GG4TrackModuleHit&	operator=(const GG4TrackModuleHit &r) ;
	void		Add(G4int status,G4Step *) ;

	int		operator==(const GG4TrackModuleHit &r) const ;
inline	void		*operator new(size_t) ;
inline	void		operator delete(void *h) ;
	void		Draw() ;
	void		Print() ;

static	G4int		GetHitID(int trk,int det_copyNo) { return trk*1000000 + det_copyNo ; }
	G4int		GetHitID() const { return GetHitID(track_id,detector_id) ; }
	G4int		GetDetectorID() const { return detector_id ; }
	G4int		GetTrackID() const { return track_id ; }

	void		SetStatus(G4int stat) { status = stat ; }
	G4int		GetStatus() const { return status ; }

	G4double	GetEnergyDeposit() const { return energy_deposit ; }
	G4double	GetKineticEnergy() const { return kinetic_energy ; }

	void		SetTime(G4double t) { entry_time = t ; }
	G4double	GetTime() const { return entry_time ; }

const	G4ParticleDefinition	*GetParticle() const { return particle ; }
const	G4String&	GetParticleName() const { return particle->GetParticleName() ; }
	G4int		GetPDGEncoding() { return particle->GetPDGEncoding() ; }

	G4ThreeVector	GetEntryPoint() const { return entry_point ; }
	G4ThreeVector	GetPos() const { return pos ; }
	G4double	GetEntryX() const { return entry_point.x() ; }
	G4double	GetEntryY() const { return entry_point.y() ; }
	G4double	GetEntryZ() const { return entry_point.z() ; }

	G4double	GetPath() const { return path ; }

// physical volume usage ... ????
	G4VPhysicalVolume	*GetPhysical() const { return physical ; }
	G4LogicalVolume		*GetLogical() const { return physical->GetLogicalVolume() ; }
	G4VSolid		*GetSolid() const { return physical->GetLogicalVolume()->GetSolid() ; }
	G4int			GetCopyNo() const { return physical->GetCopyNo() ; }

private:

static	G4Navigator		*navigator ;		//!
	G4VPhysicalVolume	*physical ;		//!
	G4ParticleDefinition	*particle ;		//!

	G4int		track_id ;		// Track ID.
	G4int		detector_id ;		// Detector full ID. 
	G4int		status ;		// entry/exit/stop status ...

	G4ThreeVector	pos ;			// the step end point in GLOBAL system ...
	G4ThreeVector	entry_point ;		// entry point in GLOBAL system ...
	G4double	entry_time ;		// regardless energy deposit value ...
	G4double	path ;			// path in between entry/exit ...

	G4double	kinetic_energy ;	// Saved kinetic energy at entry point ...
	G4double	energy_deposit ;	// 0 value permited ...

	} ;

typedef	G4THitsCollection<GG4TrackModuleHit> 	TrackModuleHitsCollection ;
extern	G4Allocator<GG4TrackModuleHit>	TrackModuleHitsAllocator ;

inline	void*		GG4TrackModuleHit::operator new(size_t) { return (void *)TrackModuleHitsAllocator.MallocSingle() ; }
inline	void		GG4TrackModuleHit::operator delete(void *H) { TrackModuleHitsAllocator.FreeSingle((GG4TrackModuleHit*) H); }
	ostream&	operator<< (ostream &os, const GG4TrackModuleHit &x) ;

#endif

// e-o-f
