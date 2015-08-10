// File:	G10XSiModuleHit.hh
// Date:	01-Jul-2014 GM Dubna

#include "G10XSiModuleHit.hh"

#include <fstream>
#include <iostream>
#include <iomanip> 
using namespace std ;

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


G4ThreadLocal G4Allocator<G10XSiModuleHit>* G10XSiModuleHitAllocator;


G10XSiModuleHit::G10XSiModuleHit(G4int track_id,G4int det_id) : G4VHit(), trackID(track_id), detectorID(det_id), particleID(-1), globalTime(0.), energyDeposit(0.), localPos(0), worldPos(0) {}


G10XSiModuleHit::~G10XSiModuleHit() {}


G10XSiModuleHit::G10XSiModuleHit(const G10XSiModuleHit &right) : G4VHit() {
    trackID = right.trackID;
    detectorID = right.detectorID;
    particleID = right.particleID;
    worldPos = right.worldPos;
    localPos = right.localPos;
    theta = right.theta;
    phi = right.phi;
    path = right.path;
    globalTime = right.globalTime;
    energyDeposit = right.energyDeposit ;
}


const G10XSiModuleHit& G10XSiModuleHit::operator=(const G10XSiModuleHit &right) {
    trackID = right.trackID;
    detectorID = right.detectorID;
    particleID = right.particleID;
    worldPos = right.worldPos;
    localPos = right.localPos;
    theta = right.theta;
    phi = right.phi;
    path = right.path;
    globalTime = right.globalTime;
    energyDeposit = right.energyDeposit ;
    return *this;
}


int G10XSiModuleHit::operator==(const G10XSiModuleHit &) const { return 0 ; }


void G10XSiModuleHit::Draw() {
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if(pVVisManager) {
		G4Circle circle(worldPos) ;
		circle.SetScreenSize(2) ;
		circle.SetFillStyle(G4Circle::filled) ;
		G4Colour colour(1.,1.,0.) ; // yellow ...
		G4VisAttributes attribs(colour) ;
		circle.SetVisAttributes(attribs) ;
		pVVisManager->Draw(circle) ;
		}
	}

/*
const std::map<G4String,G4AttDef>* G10XSiModuleHit::GetAttDefs() const {
    G4bool isNew ;
    std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("G10XSiModuleHit",isNew) ;
    if (isNew) {
        (*store)["HitType"] = G4AttDef("HitType","Hit Type","Physics","","G4String");
        (*store)["ID"]      = G4AttDef("ID","ID","Physics","","G4int");
        (*store)["Time"]    = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");
        (*store)["EDep"]    = G4AttDef("EDep","EDep","Physics","G4BestUnit","G4double");
        (*store)["Pos"]     = G4AttDef("Pos","Position","Physics","G4BestUnit","G4ThreeVector");
    }
    return store ;
}
*/

/*
std::vector<G4AttValue>* G10XSiModuleHit::CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue> ;
    
    values->push_back(G4AttValue("HitType","DriftChamberHit",""));
    values->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(detectorID),""));
    values->push_back(G4AttValue("Time",G4BestUnit(globalTime,"Time"),""));
    values->push_back(G4AttValue("EDep",G4BestUnit(energyDeposit,"EDep"),""));
    values->push_back(G4AttValue("Pos",G4BestUnit(worldPos,"Length"),""));
    
    return values;
}
*/

void G10XSiModuleHit::Print() {
	cout << fixed << showpoint << setprecision(2) ;
	cout << "  detector[" << detectorID << "] " ;
	cout << " t[ns]: " << setw(6) << globalTime/ns << "  " ;
	cout << " e[MeV]: " << setw(6) << energyDeposit << "  " ;
	cout << " local (x,y)[mm]: " ;
	cout << setw(6) << localPos.x() << " " ;
	cout << setw(6) << localPos.y() << " " ;
cout <<  " th: " << " " ;
cout << setw(7) << theta / deg << " " ;
cout <<  " phi: " << " " ;
cout << setw(7) << phi / deg << " " ;
	cout << endl ;
	}

// e-o-f
