// File:	Ruller.cc
// Date:	22-May-2009 G.Macharashvili Dubna

#include <G4SystemOfUnits.hh>

#include "Ruller.hh"

Ruller::Ruller(G4LogicalVolume *MotherVol,const char *xname,G4ThreeVector &po,G4ThreeVector &p1,G4double dz) {
	string name(xname) ;
	G4Colour C0(1., 1., .0) ;
	G4Colour C1(.1, .1, .1) ;
	G4Colour C2(1., .1, .1) ;
	G4VisAttributes	*V0=new G4VisAttributes(true,C0) ;
	V0->SetForceSolid(true) ;
	V0->SetVisibility(true) ;
	G4VisAttributes	*V1=new G4VisAttributes(true,C1) ;
	V1->SetForceSolid(true) ;
	V1->SetVisibility(true) ;
	G4VisAttributes	*V2=new G4VisAttributes(true,C2) ;
	V2->SetForceSolid(true) ;
	V2->SetVisibility(true) ;

	G4int	Ncm = ((p1 - po).mag() + 0.5*dz) / dz ;
	G4RotationMatrix *R = new G4RotationMatrix ;
	G4double phi = (p1 - po).phi() ;
	G4double theta = (p1 - po).theta() ;
	R->rotateZ(-phi) ;
	R->rotateY(-theta) ;
	G4Box *Bcm = new G4Box(name + "Bcm",5*CLHEP::mm,5*CLHEP::mm,dz/2) ;
	G4LogicalVolume *L0 = new G4LogicalVolume(Bcm,MotherVol->GetMaterial(),name + "L0") ;
	L0->SetVisAttributes(V0) ;
	G4LogicalVolume *L1 = new G4LogicalVolume(Bcm,MotherVol->GetMaterial(),name + "L1") ;
	L1->SetVisAttributes(V1) ;
	G4LogicalVolume *L2 = new G4LogicalVolume(Bcm,MotherVol->GetMaterial(),name + "L2") ;
	L2->SetVisAttributes(V2) ;

	G4ThreeVector d(dz*sin(theta)*cos(phi), dz*sin(theta)*sin(phi), dz*cos(theta)) ;
	G4ThreeVector p = po + d/2 ;
	for (int k = 1 ; k <= Ncm ; k += 2) {
		new G4PVPlacement(R,p,L0,L0->GetName(),MotherVol,false,k) ;
		if (!((k + 1)%10)) new G4PVPlacement(R,p += d,L2,L2->GetName(),MotherVol,false,k + 1) ;
		else new G4PVPlacement(R,p += d,L1,L1->GetName(),MotherVol,false,k + 1) ;
		p += d ;
		}
	}

// eof
