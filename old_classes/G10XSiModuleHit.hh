// File:	G10XSiModuleHit.hh
// Date:	01-Jul-2014 GM Dubna


#ifndef G10XSiModuleHit_h
#define G10XSiModuleHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef ;
class G4AttValue ;

/// - the layer ID
/// - the particle time
/// - the deposited energy
/// - the particle local and global positions

class G10XSiModuleHit : public G4VHit {

public:

	G10XSiModuleHit() ;
	G10XSiModuleHit(G4int track_id,G4int det_id) ;
	G10XSiModuleHit(const G10XSiModuleHit &right) ;
virtual ~G10XSiModuleHit();

const	G10XSiModuleHit& operator=(const G10XSiModuleHit &right) ;
int	operator==(const G10XSiModuleHit &right) const ;
    
inline	void	*operator new(size_t) ;
inline	void	operator delete(void *aHit) ;
    
virtual	void	Draw();
//--virtual	const	std::map<G4String,G4AttDef>*	GetAttDefs() const ;
//--virtual	std::vector<G4AttValue>*	CreateAttValues() const ;
virtual	void	Print() ;


//--void	SetLayerID(G4int det_id = -1) { detectorID = det_id ; }
G4int	GetTrackID() const { return trackID; }
G4int	GetDetectorID() const { return detectorID; }
G4int	GetParticleID() const { return particleID; }

//--void	SetTime(G4double t) { globalTime = t ; }
G4double	GetTime() const { return globalTime ; }

//--void	SetEnergyDeposit(G4double e) { energyDeposit = e ; }
G4double	GetEnergyDeposit() const { return energyDeposit ; }

//--void	SetLocalPos(G4ThreeVector xyz) { localPos = xyz ; }
G4ThreeVector	GetLocalPos() const { return localPos ; }

//--void	SetWorldPos(G4ThreeVector xyz) { worldPos = xyz ; }
G4ThreeVector	GetWorldPos() const { return worldPos ; }
    
G4double	GetPath() const { return path ; }
    
void	Set(G4int track_id,G4int det_id,G4int particle_id,G4double t,G4double e,G4ThreeVector loc,G4ThreeVector glob) {
	trackID = track_id ;
	detectorID = det_id ;
	particleID = particle_id ;
	globalTime = t ;
	energyDeposit = e ;
	localPos = loc ;
	worldPos = glob ;
theta = 0 ;
phi = 0 ;
path = 0 ; 
	}
    
void	AddEnergyDeposit(G4double e,G4double step) { energyDeposit += e ; path += step ; }
    
G4int	GetHitID() const { return detectorID * 1000 + trackID ; }

private:

	G4int		trackID ;		//!
	G4int		detectorID ;		//
	G4int		particleID ;		//
	G4double	globalTime ;		//
	G4double	energyDeposit ;		//
	G4ThreeVector	localPos ;		//
	G4ThreeVector	worldPos ;		//
	G4double	theta , phi , path ;	// direction ...

	} ;

typedef G4THitsCollection<G10XSiModuleHit> G10XSiModuleHitsCollection ;

extern G4ThreadLocal G4Allocator<G10XSiModuleHit>* G10XSiModuleHitAllocator ;

inline void* G10XSiModuleHit::operator new(size_t) {
	if (!G10XSiModuleHitAllocator) G10XSiModuleHitAllocator = new G4Allocator<G10XSiModuleHit> ;
	return (void*)G10XSiModuleHitAllocator->MallocSingle() ;
	}

inline void G10XSiModuleHit::operator delete(void* aHit) { G10XSiModuleHitAllocator->FreeSingle((G10XSiModuleHit*) aHit) ; }

#endif

// e-o-f
