// File:	EdmGeantTEvent.hh
// Date:	08-Apr-2015 G.Macharashvili Dubna

#ifndef EdmGeantTEvent_hh
#define EdmGeantTEvent_hh

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include <TMath.h>
#include <TObject.h>
#include <TClass.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#include "EdmGeantParticleDB.hh"

// - COMMENTS ...
// - particle data base needed to get PDG_id, geant_name, PDG_mass ...
// - class G4TPrimaryTrack: add converter to TLorentzVector ...
// - 

class EdmGeantTRunProfile : public TObject { // some dictionary/data-base ...

static	const	Int_t	nx ;		//!   max 40 raws
static	const	Int_t	ny ;		//!   max 40 columns
static	const	Double_t	protonMass ;		//!
static	const	Double_t	neutronMass ;		//!
static	const	Double_t	deuteronMass ;		//!
static	const	Int_t		maxNumOfNames = 50 ;	//!

public:

static	Double_t	statx , staty , statz ;	//!

	EdmGeantTRunProfile() ;
virtual	~EdmGeantTRunProfile() ;
	EdmGeantTRunProfile	&operator=(const EdmGeantTRunProfile &) ;

const	TString		&GetDetectorName(int id) const { return arbitraryNames[id] ; }
const	TString		&getName(int id) const { return arbitraryNames[id] ; }

	void		Print(Option_t * = "") const ;
	int		getID(const char * _name) ; // any name <-> id conversion ...

protected:
// saved datat ...

	Double_t	beamEnergy , beamMass , targetMass , systemBeta ;
	Double_t	beamPolarization , beamPolarizationPhi , asymmetry ;
	Double_t	calorimeterZpos ;
	Double_t	sx , sy , sz ;

	Int_t		numOfEntities ;
	TString		arbitraryNames[maxNumOfNames] ; //[numOfEntities]

public:

	void		setInitialKinematics(double x , double y=deuteronMass , double z=6*deuteronMass) ;
inline	void		setModuleSize(double x , double y , double z) { sx = x ; sy = y ; sz = z ; }
inline	void		setGeometryParameters(double x) { calorimeterZpos = x ; }
inline	void		setBeamPolarization(double x) { beamPolarization = x ; }
inline	void		setBeamPolarizationPhi(double x) { beamPolarizationPhi = x ; }

inline	double		getBeamEnergy() const { return beamEnergy ; }
inline	double		getBeamMass() const { return beamMass ; }
inline	double		getTargetMass() const { return targetMass ; }
inline	double		getSystemBeta() const { return systemBeta ; }
inline	double		getCalorimeterZpos() const { return calorimeterZpos ; }
inline	double		getBeamPolarization() const { return beamPolarization ; }
inline	double		getBeamPolarizationPhi() const { return beamPolarizationPhi ; }

static	double		getXsize() { return statx ; }
static	double		getYsize() { return staty ; }
static	double		getZsize() { return statz ; }
static	int		getXn() { return nx ; }
static	int		getYn() { return ny ; }

/*
	double		getXcenter(int i) const { return double((nx%2) ? -nx/2*sx : -nx/2*sx + sx/2) + sx * i ; }
	double		getYcenter(int i) const { return double((ny%2) ? -ny/2*sy : -ny/2*sy + sy/2) + sy * i ; }
	double		getXc(int module_id) const { return double((nx%2) ? -nx/2*sx : -nx/2*sx + sx/2) + sx * (module_id%1000) ; }
	double		getYc(int module_id) const { return double((ny%2) ? -ny/2*sy : -ny/2*sy + sy/2) + sy * (module_id/1000) ; }
*/

	ClassDef(EdmGeantTRunProfile,1) ;

	} ;


class EdmGeantTHit : public TObject {

private:

	UShort_t	detector_id ;			// internally generated: 0,1,2,3 ...)
	UShort_t	module_id ;			// module is the same as module e.g.[0 - 36]
	Float_t		energy_deposit ;		// [MeV]
	Float_t		entry_time ;			// [ns]

public:

	EdmGeantTHit() ;
	EdmGeantTHit(int det_id,int mod,double ee,double tt) ;
	EdmGeantTHit(const EdmGeantTHit &) ;
virtual	~EdmGeantTHit() ;
	EdmGeantTHit	&operator=(const EdmGeantTHit &) ;

	void		SetEnergyDeposit(double e) { energy_deposit = e ; }
	void		SetTime(double t) { entry_time = t ; }
	double		GetEnergyDeposit() const { return energy_deposit ; }
	double		GetTime() const { return entry_time ; }

	int		GetDetectorID() const { return detector_id ; }
	int		GetModuleID() const { return module_id ; }

	double		getXc() const {
	return double((EdmGeantTRunProfile::getXn()%2) ? -EdmGeantTRunProfile::getXn()/2*EdmGeantTRunProfile::statx : -EdmGeantTRunProfile::getXn()/2*EdmGeantTRunProfile::statx + EdmGeantTRunProfile::statx/2) + EdmGeantTRunProfile::statx * (module_id%1000) ;
	}

	double		getYc() const {
	return double((EdmGeantTRunProfile::getYn()%2) ? -EdmGeantTRunProfile::getYn()/2*EdmGeantTRunProfile::staty : -EdmGeantTRunProfile::getYn()/2*EdmGeantTRunProfile::staty + EdmGeantTRunProfile::staty/2) + EdmGeantTRunProfile::staty * (module_id/1000) ;
	}

private:

	ClassDef(EdmGeantTHit,1) ;

	} ;


class EdmGeantTTrack : public TObject {

	Int_t		particle_PDG_id ;		// 
	Float_t		px , py , pz ;			// momentum [MeV/c] 
//	Float_t		ox , oy , oz ;			// track origin in glob system [mm] 
// Very important: Momentum is given in the PHYSICAL (right handed) lab system 
// with z - along the beam.

public:

	double		mass , Ekin ;			//! not to write on stream ... Get/Set needed ...
	string		particleName ;			//! 

public:

	EdmGeantTTrack() ;
	EdmGeantTTrack(const EdmGeantTTrack &) ;
virtual	~EdmGeantTTrack() ;
	EdmGeantTTrack	&operator=(const EdmGeantTTrack &) ;

	void		SetParticle(int PDG_code,double x = 0,double y = 0,double z = 0) 
	{ particle_PDG_id = PDG_code, px = x ; py = y ; pz = z ; }
	void		SetMomentum(double x,double y,double z) { px = x ; py = y ; pz = z ; }
//	void		SetOrigin(double x,double y,double z) { ox = x ; oy = y ; oz = z ; }

	double		Px() const { return px ; }
	double		Py() const { return py ; }
	double		Pz() const { return pz ; }
	TVector3	P()  const { return TVector3(px,py,pz) ; }
	TLorentzVector	P4() const { return TLorentzVector(px,py,pz,Ekin + mass) ; } // ??

	int		GetParticlePDGcode() const { return particle_PDG_id ; }
	int		GetParticleID() const { return particle_PDG_id ; }

	ClassDef(EdmGeantTTrack,1) ;

	} ;


class EdmGeantTEvent : public TObject {

private:

static	const	int	MaxHit = 200 ;
static	const	int	MaxTrack = 20 ;
static	const	int	MaxPar = 256 ;

protected:

	EdmGeantTEvent	*gSelfPtr ;		//! The object address 

	TFile		*gInputFile ;		//! Input root file 
	TTree		*gInputTree ; 		//!
	TBranch		*gInputBranch ;		//!

	TFile		*gOutputFile ;		//! Output root file 
	TTree		*gOutputTree ; 		//!
	TBranch		*gOutputBranch ;	//!

	Long64_t	CurrentEntry ; 		//! active if I/O initialized
	Long64_t	TotalEntries ; 		//! >0 if read from file

	EdmGeantTRunProfile	*runProfile ;	//!

// event header ...
	UInt_t		TimeSec , TimeUSec ;	// Time for event connection 
	UInt_t		TriggerType ;		// standard EMS trigger word copy 
	UInt_t		status , status2 ;	// PI and FI and TG and BCT stored 
	Float_t		weight ;		// event weight or something else 
// primary event information ...
	Int_t		generated , accepted ;	// generated/accepted so far ...
// global pointers ...
	TClonesArray	*gHitList ;		//!
	TClonesArray	*gTrackList ;		//!
// list of hits ...
	TClonesArray	*HitList ;		//->
	Int_t		NumberOfHits ;		// Number of stored hits ...
// primary track list ...
	TClonesArray	*TrackList ;		//->
	Int_t		NumberOfTracks ;	// Number of primary tracks ...
// primary vertex ...
//	Int_t		targetType ;		// needs to be defined ...
	Float_t		vx , vy , vz ;		// primary interaction point ...
// beam particle ... 
	Int_t		beam_particle_PDG_id ;	// beam particle id and momentum vector ...
	Float_t		beam_px , beam_py , beam_pz ;		// momentum [MeV/c] 
// additional user controlled data ...
	Int_t		udata_items ;		// arbitrary data set ...
	Float_t		udata[MaxPar] ;		//[udata_items]
// tmp variables ...
	EdmGeantTTrack	*track ;		//! 
	EdmGeantTHit	*hit ;			//! 


private:

public:

	EdmGeantTEvent() ;
virtual	~EdmGeantTEvent() ;

	void	Clear(Option_t * = "" ) ;
	void	Print(Option_t * = "" ) const ;

public:

	void		BeginOfRun() ;
	void		EndOfRun() ;

	void		SetTime(unsigned ts,unsigned tu) { TimeSec = ts ; TimeUSec = tu ; }
	void		SetTriggerType(int t) { TriggerType = t ; }
	void		SetAccumulatedEvents(int _gen,int _acc) { generated = _gen ; accepted = _acc ; }
	void		SetVertex(double x,double y,double z) { vx = x ; vy = y ; vz = z ; }

	void		SetBeamParticle(int PDG_code,double x = 0,double y = 0,double z = 0) 
	{ beam_particle_PDG_id = PDG_code, beam_px = x ; beam_py = y ; beam_pz = z ; }
	void		SetBeamMomentum(double x,double y,double z) { beam_px = x ; beam_py = y ; beam_pz = z ; }

	void		SetWeight(double w) { weight = w ; }
	void		SetStatus(double s) { status = s ; } // 0 - undef, 1 - elastic, 2 - inelastic ...
	void		SetStatus2(double s) { status2 = s ; }
	void		SetUserData(int num,double *d) ;

	double		GetTime() const { return double(TimeSec) + double(TimeUSec)/1000000.0 ; }
	unsigned	GetTimeSec() const { return TimeSec ; }
	unsigned	GetTimeUSec() const { return TimeUSec ; }
	int		GetTrigger() const { return (TriggerType) ; }
	int		GetGenerated() const { return (generated) ; }
	int		GetAccepted() const { return (accepted) ; }
	double		GetVx() const { return (vx) ; }
	double		GetVy() const { return (vy) ; }
	double		GetVz() const { return (vz) ; }

	double		GetBeamPx() const { return (beam_px) ; }
	double		GetBeamPy() const { return (beam_py) ; }
	double		GetBeamPz() const { return (beam_pz) ; }
	TVector3	GetBeamP() const { return TVector3(beam_px,beam_py,beam_pz) ; }
	int		GetBeamParticleID() const { return (beam_particle_PDG_id) ; }

	double		GetWeight() const { return weight ; }
	int		GetStatus() const { return status ; }
	int		GetStatus2() const { return status2 ; }

	int		GetUserData(double *d) ;

// detector hits ...
private:

	char*		TimeStamp() const ;
	EdmGeantTHit*	CreateHit(const char *,int s = 0,double ee = 0,double tt = 0) ;

public:

	EdmGeantTHit*	AddHit(const char *,int s = 0,double ee = 0,double tt = 0) ;
	EdmGeantTHit*	GetHit(int nseq) ;
const	EdmGeantTHit*	GetHit(int nseq) const ;
	int		GetNumberOfHits() { return NumberOfHits ; }

// primary track I/O ...
private:

public:

	EdmGeantTTrack*	AddTrack() ;
	EdmGeantTTrack*	GetTrack(int k) ;
const	EdmGeantTTrack*	GetTrack(int k) const ;
	int		GetNumberOfTracks() { return NumberOfTracks ; }

// common ...
	int		Save() ;
	int		Get(int entry = -1) ;
virtual	void		Exception(const char * = 0) ;
	void		AttachInputFile(TFile* F) ;
	void		AttachOutputFile(TFile* F) ;
	Long64_t	GetNumOfEntries() { return TotalEntries ; }

// run profile data I/O ...
/*	void		setSystem(double beamEk,double beamMass,double targetMass) { runProfile->
	void		setPolarizationParameters(double beamPol,double beamPolPhi = 0,double asymm = 0) { runProfile->
	void		setGeometryParameters(double Zpos) { runProfile->setCalorimeterZpos(Zpos) ; }
	void		setModuleSize(double x,double y,double z) { runProfile->setModuleSize(x,y,z) ; }
	
	
	double		getBeamEnergy() const { return runProfile->getBeamEnergy() ; }
	double		getBeamMass() const { return runProfile->getBeamMass() ; }
	double		getTargetMass() const { return runProfile->getTargetMass() ; }
	double		getBeamPolarization() const { return runProfile->getBeamPolarization() ; }
//++	double		getModulePhi(int id) ;
	double		getModuleXcenter(int ix) const { return runProfile->getXcenter(ix) ; }
	double		getModuleYcenter(int iy) const { return runProfile->getYcenter(iy) ; }
	double		getModuleZpos() const { return runProfile->getCalorimeterZpos() ; }
*/

	EdmGeantTRunProfile*getProfile() { return runProfile ; }
const	TString		&getName(int id) const { return runProfile->getName(id) ; }
	void		PrintProfile() const { runProfile->Print() ; }

	ClassDef(EdmGeantTEvent,1) ;
		
	} ;

#endif

// e-o-f
