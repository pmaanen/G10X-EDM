// File:	UArrow.cc
// Date:	17-Jul-2006 G.M. Juelich

#include <G4SystemOfUnits.hh>

#include "UArrow.hh"

UArrow::UArrow(G4double Len,G4LogicalVolume *MotherVol,G4ThreeVector Vc) {
	G4VisAttributes	*V = new G4VisAttributes(G4Colour(0.96,0.41,0.41)) ;
	V->SetForceSolid(true) ;
	V->SetVisibility(true) ;
	G4double	ZB[4] = {    0,   -7,   -6,    0 } ;
	G4double	RB[4] = {    0,    0,    0,    0 } ;
	G4double	RE[4] = {    1,    1,  1.2,  0.1 } ;
	ZB[0] = -Len ;
	G4Polycone		*PZ = new G4Polycone("Arrow_Long",0,360*CLHEP::deg,4,ZB,RB,RE) ;
 	G4LogicalVolume		*LZ = new G4LogicalVolume(PZ,MotherVol->GetMaterial(),"Arrow_Z") ;
//	G4VPhysicalVolume 	*WZ = 
	                        new G4PVPlacement(0,Vc+G4ThreeVector(0,0,Len/2),LZ,LZ->GetName(),MotherVol,false,0) ;
	LZ->SetVisAttributes(V) ;
	G4double	LenV = 10.0 * CLHEP::mm ;
	ZB[0] = -LenV ;
	G4RotationMatrix	*RY = new G4RotationMatrix ;
	RY->rotateX(90*CLHEP::deg) ;
	G4Polycone		*PY = new G4Polycone("Arrow_Short",0,360*CLHEP::deg,4,ZB,RB,RE) ;
 	G4LogicalVolume		*LY = new G4LogicalVolume(PY,MotherVol->GetMaterial(),"Arrow_Y") ;
//	G4VPhysicalVolume 	*WY = 
                                new G4PVPlacement(RY,Vc+G4ThreeVector(0,LenV/2,0),LY,LY->GetName(),MotherVol,false,0) ;
	LY->SetVisAttributes(V) ;
	}

// eof
