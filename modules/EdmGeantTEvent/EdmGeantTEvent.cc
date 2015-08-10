// File:	EdmGeantTEvent.cc
// Date:	08-Apr-2015 G.Macharashvili Dubna

#include "EdmGeantTEvent.hh"

ClassImp(EdmGeantTRunProfile)

const	Int_t		EdmGeantTRunProfile::nx = 40 ;
const	Int_t		EdmGeantTRunProfile::ny = 40 ;
const	Double_t	EdmGeantTRunProfile::protonMass = 938.272 ;
const	Double_t	EdmGeantTRunProfile::neutronMass = 939.565 ;
const	Double_t	EdmGeantTRunProfile::deuteronMass = 1875.613 ;
	Double_t	EdmGeantTRunProfile::statx = 15.0 ; // [mm]
	Double_t	EdmGeantTRunProfile::staty = 15.0 ; // [mm]
	Double_t	EdmGeantTRunProfile::statz = 100.0 ; // [mm]

	EdmGeantTRunProfile::EdmGeantTRunProfile() : TObject() , numOfEntities(0) {
		sx = statx ;  // [mm]
		sy = staty ;  // [mm]
		sz = statz ; // [mm]
		beamEnergy = 270.0 ; // [MeV]
		beamMass = deuteronMass ; // deuteron [MeV]
		targetMass = 6 * deuteronMass ; // carbon [MeV]
		}

	EdmGeantTRunProfile::~EdmGeantTRunProfile() {}


EdmGeantTRunProfile& EdmGeantTRunProfile::operator=(const EdmGeantTRunProfile &r) {
	if (this != &r) {
		*(TObject *) this = *(TObject *) &r ;
		}
	return *this ;
	}

void EdmGeantTRunProfile::setInitialKinematics(double e,double beamM,double targetM) {
	double beamP = sqrt(e*(e+2*beamM)) ;
	systemBeta = beamP/(e+beamM+targetM) ;
	}

int EdmGeantTRunProfile::getID(const char * _name) {
	if (numOfEntities == maxNumOfNames) {
		cerr << "EdmGeantTRunProfile::getID: buffer overflow. Fatal ... " << endl ;
		abort() ;
		}
	for (int id = 0 ; id < numOfEntities ; id++) {
		if (arbitraryNames[id] == TString(_name)) return id ;
		}
	arbitraryNames[numOfEntities] = _name ;
	++numOfEntities ;
	return numOfEntities - 1 ;
	}

void EdmGeantTRunProfile::Print(Option_t *) const {
	cout << fixed << showpoint << setprecision(2) ;
	cout << endl ;
	cout << "EdmGeantTRunProfile::Print() " << endl ;
	cout << "detector dictionary: [" << numOfEntities << "]" << endl ;
	for (int id = 0 ; id < numOfEntities ; id++) {
		cout << "        " ;
		cout << setw(10) << arbitraryNames[id] << "  " ;
		cout << setw(2) << id << " " ;
		cout << endl ;
		}
	cout << "initial conditions: " << endl ;
	cout << "             beam energy: [MeV] " << setw(7) << beamEnergy << endl ;
	cout << "      beam particle mass: [MeV] " << setw(7) << beamMass << endl ;
	cout << "       beam polarization:       " << setw(7) << beamPolarization << endl ;
	cout << "  calorimeter z-position: [mm]  " << setw(7) << calorimeterZpos << endl ;
	cout << "            crystal size: [mm]  " << sx << " x " << sy << " x " << sz << endl ;
	cout << endl ;
	}

ClassImp(EdmGeantTHit)

	EdmGeantTHit::EdmGeantTHit() : TObject() {
//--??		Class()->IgnoreTObjectStreamer() ;
		detector_id = 0 ;
		module_id = 0 ;
		entry_time = 0 ;
		energy_deposit = 0 ;
		}

	EdmGeantTHit::EdmGeantTHit(int det_id,int mod,double ee,double tt) : TObject() {
//--??		Class()->IgnoreTObjectStreamer() ;
		detector_id = det_id ;
		module_id = mod ;
		entry_time = tt ;
		energy_deposit = ee ;
		}

	EdmGeantTHit::EdmGeantTHit(const EdmGeantTHit &t) : TObject() {
//--??		Class()->IgnoreTObjectStreamer() ;
		*this = t ;
		}

	EdmGeantTHit::~EdmGeantTHit() {}

EdmGeantTHit& EdmGeantTHit::operator=(const EdmGeantTHit &r) {
	if (this != &r) {
		*(TObject *) this = *(TObject *) &r ;
		detector_id = r.detector_id ;
		module_id = r.module_id ; 
		energy_deposit = r.energy_deposit ; 
		entry_time = r.entry_time ; 
		}
	return *this ;
	}

ClassImp(EdmGeantTEvent)

EdmGeantTEvent::EdmGeantTEvent() {
//--??	Class()->IgnoreTObjectStreamer() ;
	gHitList = new TClonesArray("EdmGeantTHit",MaxHit) ;
	HitList = gHitList ;
	gTrackList = new TClonesArray("EdmGeantTTrack",MaxTrack) ;
	TrackList = gTrackList ;
	Clear() ;
	gSelfPtr = this ;
	gInputFile = gOutputFile = 0 ;
	gInputTree = gOutputTree = 0 ;
	gInputBranch = gOutputBranch = 0 ;
	CurrentEntry = -1 ;
	TotalEntries = 0 ;
// create runProfile ...
	runProfile = new EdmGeantTRunProfile ;
	}

EdmGeantTEvent::~EdmGeantTEvent() {
//	runProfile->Print() ;
	if (gOutputFile) {
		gOutputFile->cd() ;
		runProfile->Write() ; // save runProfile 
		}
	delete runProfile ;
	}

// detector hits I/O ...

EdmGeantTHit* EdmGeantTEvent::AddHit(const char * detName,int s,double ee,double tt) {
	if (MaxHit == NumberOfHits) return 0 ;
	return new ((*HitList)[NumberOfHits++]) EdmGeantTHit(runProfile->getID(detName),s,ee,tt) ;
	}

EdmGeantTHit* EdmGeantTEvent::GetHit(int k) {
	if (k >= NumberOfHits || k < 0) return 0 ;
	hit = dynamic_cast<EdmGeantTHit*> ((*HitList)[k]) ;
	return hit ;
	}

const EdmGeantTHit* EdmGeantTEvent::GetHit(int k) const {
	if (k >= NumberOfHits || k < 0) return 0 ;
	return dynamic_cast<EdmGeantTHit*> ((*HitList)[k]) ;
	}

// primary track I/O ...
EdmGeantTTrack::EdmGeantTTrack() : TObject() {
//--??	Class()->IgnoreTObjectStreamer() ;
	particle_PDG_id = 1234 ;
	px = py = pz = 0 ;
//	ox = oy = oz = 0 ;
	}

EdmGeantTTrack::EdmGeantTTrack(const EdmGeantTTrack &t) : TObject() { *this = t ; }

EdmGeantTTrack::~EdmGeantTTrack() {}

EdmGeantTTrack& EdmGeantTTrack::operator=(const EdmGeantTTrack &r) {
	if (this != &r) {
		*(TObject *) this = *(TObject *) &r ;
		particle_PDG_id = r.particle_PDG_id ; 
		px = r.px ;
		py = r.py ;
		pz = r.pz ;
		// o = r.o ;
		}
	return *this ;
	}

EdmGeantTTrack* EdmGeantTEvent::AddTrack() {
	if (MaxTrack == NumberOfTracks) return 0 ;
	return new ((*TrackList)[NumberOfTracks++]) EdmGeantTTrack() ;
	}

EdmGeantTTrack* EdmGeantTEvent::GetTrack(int k) {
	if (k >= NumberOfTracks || k < 0) return 0 ;
	track = dynamic_cast<EdmGeantTTrack*> ((*TrackList)[k]) ;
	track->mass = EdmGeantParticleDB::Get()->GetMass(track->GetParticlePDGcode()) ;
	track->particleName = EdmGeantParticleDB::Get()->GetName(track->GetParticlePDGcode()) ;
	track->Ekin = sqrt(track->P().Mag2() + track->mass*track->mass) - track->mass ;
	return track ;
	}

const EdmGeantTTrack* EdmGeantTEvent::GetTrack(int k) const {
	if (k >= NumberOfTracks || k < 0) return 0 ;
	return dynamic_cast<EdmGeantTTrack*> ((*TrackList)[k]) ;
	}

// common ...
void EdmGeantTEvent::Clear(Option_t *) {
	NumberOfHits = 0 ;
	HitList->Clear() ;
	NumberOfTracks = 0 ;
	TrackList->Clear() ;
	status = TriggerType = TimeSec = TimeUSec = 0 ;
	weight = 1.0 ;
	generated = accepted = 0 ;
	vx = vy = vz = 0 ;
	udata_items = 0 ;
	for (int k = 0 ; k < MaxPar ; k++) udata[k] = 0 ;
	}

char* EdmGeantTEvent::TimeStamp() const {
	time_t t = TimeSec ;
	char *s = ctime(&t) ;
	char *s1 = s ;
	while (*s!='\n') s++ ;
	*s = '\0' ;
	return s1 ;
	}

void EdmGeantTEvent::SetUserData(int num,double *d) {
	udata_items = (num < MaxPar) ? num : MaxPar ;
	for (int k = 0 ; k < udata_items ; k++) udata[k] = *d++ ;
	}

int EdmGeantTEvent::GetUserData(double *d) {
	for (int k = 0 ; k < udata_items ; k++) *d++ = udata[k] ;
	return udata_items ;
	}

void EdmGeantTEvent::Print(Option_t *) const {
	cout << fixed << showpoint << setprecision(6) ;
	cout << endl ;
	cout << "EdmGeantTEvent: " ;
	cout << ".." << setw(3) << setfill('0') << GetTimeSec()%1000 << "." << setw(6) << GetTimeUSec() << setfill(' ') << "  " ;
	cout << "Trig: 0x" << hex << GetTrigger() << dec << "  " ;
	cout << TimeStamp() << "  " ;
	cout << "g:" << GetGenerated() << " " ;
	cout << "a:" << GetAccepted() << " " ;
	cout << endl ;

	cout << "primary.vertex:     [mm]: " ;
	cout << setw(7) << setprecision(2) << GetVx() << " " ;
	cout << setw(7) << setprecision(2) << GetVy() << " " ;
	cout << setw(7) << setprecision(2) << GetVz() << " " ;
	cout << "  (edm sys)  " ;
	cout << endl ;

	cout << "primary.tracks: " << NumberOfTracks << "  " ;
	cout << " (target/lab sys. z-along beam)  " ;
	cout << endl ;
	for (int t = 0 ; t < NumberOfTracks ; t++) {
		const EdmGeantTTrack *track = GetTrack(t) ;
		cout << setprecision(2) << "    " ;
		cout << setw(10) << track->GetParticlePDGcode() << " " ;
		cout << " p[MeV/c]: " ;
		cout << setw(7) << track->Px() << " " ;
		cout << setw(7) << track->Py() << " " ;
		cout << setw(7) << track->Pz() << " " ;
		cout << "  th-phi[deg]: " ;
		cout << setw(5) << track->P().Theta()*TMath::RadToDeg() << " " ;
		cout << setw(6) << track->P().Phi()*TMath::RadToDeg() << " " ;
		cout << endl ;
		}
	cout << " detector.hits: " << NumberOfHits << " " ;
	cout << "  (global sys)  " ;
	cout << endl ;
	for (int t = 0 ; t < NumberOfHits ; t++) {
		const EdmGeantTHit *hit = GetHit(t) ;
		cout << "    " ;
		cout << setw(2) << hit->GetDetectorID() << " " ;
		cout << setw(10) << left << getName(hit->GetDetectorID()) << right << " " ;
		cout << setw(6) << hit->GetModuleID() << " " ;
		cout << "    " ;
		cout << "e:" << setw(6) << setprecision(2) << hit->GetEnergyDeposit() << "  " ;
		cout << "t:" << setw(5) << setprecision(2) << hit->GetTime() << "  " ;
//++		cout << "x:" << setw(7) << setprecision(2) << hit->getXc() << "  " ;
//++		cout << "y:" << setw(7) << setprecision(2) << hit->getYc() << "  " ;
		cout << endl ;
		}
	}

void EdmGeantTEvent::AttachOutputFile(TFile* F) {
	gOutputFile = F ;
	if (gOutputFile) {
		gOutputFile->SetCompressionLevel(9) ;
		gOutputFile->cd() ;
		}
	else Exception("output file not attached ...") ;
	if (!gOutputTree) gOutputTree = (TTree *) new TTree("EdmGeantTEventTree","EdmGeantTEventTree") ;
	if (!gOutputTree) Exception("tree not exists on output file ...") ;
	if (!gOutputBranch) gOutputBranch = gOutputTree->Bronch("EdmGeantTEvent","EdmGeantTEvent",&gSelfPtr,32000,99) ;
	if (!gOutputBranch) Exception("event not exists in output Tree ...") ;
	}

int EdmGeantTEvent::Save() {
	if (gOutputFile) gOutputFile->cd() ;
	else Exception("output file not attached ...") ;
	return gOutputTree->Fill() ;
	}

void EdmGeantTEvent::AttachInputFile(TFile* F) {
	gInputTree = 0 ;
	gInputBranch = 0 ;
	gInputFile = F ;
	if (gInputFile) gInputFile->cd() ;
	else Exception("input file not attached ...") ;
	if (!gInputTree) {
		gInputTree = (TTree *) gInputFile->Get("EdmGeantTEventTree") ;
		if (!gInputTree) Exception("tree not exists on input file ...") ;
		TotalEntries = int (gInputTree->GetEntries()) ;
		if (!gInputBranch) gInputBranch = gInputTree->GetBranch("EdmGeantTEvent") ;
		if (!gInputBranch) Exception("event not exists in input Tree ...") ;
		gInputBranch->SetAddress(&gSelfPtr) ;
		}
	int stat = gInputFile->ReadTObject(runProfile,"EdmGeantTRunProfile") ;
	if (!stat) Exception("EdmGeantTEvent::AttachInputFile: runProfile can not read ...") ;
	EdmGeantTRunProfile::statx = runProfile->getXsize() ;
	EdmGeantTRunProfile::staty = runProfile->getYsize() ;
	EdmGeantTRunProfile::statz = runProfile->getZsize() ;
//	runProfile->Print() ;
	}

int EdmGeantTEvent::Get(int entry) {
	if (gInputFile) gInputFile->cd() ;
	else Exception("input file not attached ...") ;
	if (entry > TotalEntries) Exception("entry number error ...") ;
	if (entry >= 0 && entry != CurrentEntry + 1) CurrentEntry = entry ;
	else CurrentEntry++ ;
	return	gInputBranch->GetEntry(CurrentEntry) ;
	}

void EdmGeantTEvent::BeginOfRun() {
	if (gInputFile) {
		gInputFile->cd() ;
		int stat = gInputFile->ReadTObject(runProfile,"EdmGeantTRunProfile") ;
		if (!stat) Exception("EdmGeantTEvent::BeginOfRun: runProfile read unsuccessful ...") ;
//		runProfile->Print() ;
		}
	}

void EdmGeantTEvent::EndOfRun() {
	if (gOutputFile) {
		gOutputFile->cd() ;
		runProfile->Write() ;
		}
	}

#include <exception>
void EdmGeantTEvent::Exception(const char *s) {
	cerr << endl << "**** " << "EdmGeantTEvent: " ;
	if (s) cerr << s ;
	cerr << endl ;
	abort() ;
	}

// e-o-f
