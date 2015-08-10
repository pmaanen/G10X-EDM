// File:	GG4TrackModuleHit.cc
// Date:	01-Mar-2015 G.Macharashvili Juelich

#include "GG4TrackModuleHit.hh"
#include <G4TouchableHandle.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Circle.hh>
#include <G4VVisManager.hh>


G4Allocator<GG4TrackModuleHit>	TrackModuleHitsAllocator ;

G4Navigator			*GG4TrackModuleHit::navigator = 0 ;

GG4TrackModuleHit::~GG4TrackModuleHit() {}

GG4TrackModuleHit::GG4TrackModuleHit() {
	physical = 0 ;
        particle = 0 ;

	track_id = 0 ;
	detector_id = 0 ;
	status = 0 ;

	pos = G4ThreeVector(0,0,0) ;
	entry_point = G4ThreeVector(0,0,0) ;
	entry_time = 0 ;
	path = 0.0 ;

	kinetic_energy = 0 ;
	energy_deposit = 0 ;
	}

GG4TrackModuleHit::GG4TrackModuleHit(G4int trackID,G4int detectorID,G4int Xstatus,G4Step *S,const G4AffineTransform *topTransform) {
	physical =		S->GetPreStepPoint()->GetPhysicalVolume() ;
	particle = 		S->GetTrack()->GetDefinition() ;

	track_id = 		trackID ;
	detector_id =		detectorID ;
        status =		10*Xstatus ;

	pos =			S->GetPostStepPoint()->GetPosition() ;
	if (1 == Xstatus) entry_point = S->GetPreStepPoint()->GetPosition() ;
	entry_time =		S->GetPreStepPoint()->GetGlobalTime() ;
        path = 			S->GetStepLength() ;

	kinetic_energy =	S->GetPreStepPoint()->GetKineticEnergy() ;
	energy_deposit =	S->GetTotalEnergyDeposit() ;

// see in GG4SensitiveDetector how 'topTransform' is defined ... 12-Mar-2013 GM
//        if (topTransform) {
//		entry_local = topTransform->TransformPoint(entry_point) ;
//		entry_dir_local = topTransform->TransformAxis(entry_dir) ;
//		}
// this works absolutely in the same way but the previous one is speed optimized ...
//	G4TouchableHandle theTouchable = S->GetPreStepPoint()->GetTouchableHandle() ;
//	entry_local = theTouchable->GetHistory()->GetTopTransform().TransformPoint(entry_point);
//	entry_dir_local = theTouchable->GetHistory()->GetTopTransform().TransformAxis(entry_dir) ;

	}

GG4TrackModuleHit::GG4TrackModuleHit(const GG4TrackModuleHit &r) { *this = r ;	}

void	GG4TrackModuleHit::Add(G4int Xstatus,G4Step *S) {
        status /= 10 ;
        status *= 10 ;
        status += Xstatus ;
        energy_deposit += S->GetTotalEnergyDeposit() ;
        path += S->GetStepLength() ;
        }

const	GG4TrackModuleHit& GG4TrackModuleHit::operator=(const GG4TrackModuleHit &r) {
	if (this != &r) {
		physical	= r.physical ;
		particle	= r.particle ;

		track_id	= r.track_id ;
		detector_id	= r.detector_id ;
		status		= r.status ;

		pos		= r.pos ;
		entry_point	= r.entry_point ;
		entry_time	= r.entry_time ;
		path		= r.path ;

        	kinetic_energy	= r.kinetic_energy ;
		energy_deposit	= r.energy_deposit ;

		}
	return *this ;
	}

int	GG4TrackModuleHit::operator==(const GG4TrackModuleHit &r) const { return ( this == &r) ; }

void	GG4TrackModuleHit::Print() { cout << *this << endl ; }

ostream&   operator<< (ostream &os,const GG4TrackModuleHit &x) {
	os << fixed << showpoint << right ;
	os << "    " << x.GetPhysical()->GetName() << " " ;
	os << setw(3) << x.GetDetectorID() << " " ;
	os << setw(2) << x.GetTrackID() << " " ;
	os << setw(10) << x.GetParticleName() << " " ;
	os << "ss: " << setw(2) << x.GetStatus() << "  " ;
	os << "Ek:" << setw(6) << setprecision(1) << x.GetKineticEnergy() << "  " ;
	os << "t:" << setw(5) << setprecision(2) << x.GetTime() << "  " ;
	os << "de:" << setw(7) << setprecision(3) << x.GetEnergyDeposit() << "  " ;
	os << "pos:" ;
	os << setw(7) << setprecision(1) << x.GetEntryX() << "  " ;
	os << setw(7) << setprecision(1) << x.GetEntryY() << "  " ;
	os << setw(7) << setprecision(1) << x.GetEntryZ() << "  " ;
	os << "d: " << setw(4) << setprecision(1) << x.GetPath() << "  " ;
	os << flush ;
        return os ;
        }

void GG4TrackModuleHit::Draw() {
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance() ;
	if(pVVisManager) {
		G4Circle circle(pos) ;
		circle.SetScreenSize(2) ;
		circle.SetFillStyle(G4Circle::filled) ;
		G4Colour colour(1.,1.,0.) ; // yellow ...
		G4VisAttributes attribs(colour) ;
		circle.SetVisAttributes(attribs) ;
		pVVisManager->Draw(circle) ;
		}
	}

// e-o-f
