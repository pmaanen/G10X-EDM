// File:	BeamPipe.cc
// Date:	18-Mar-2015 G.M. Juelich

#include <G4SystemOfUnits.hh>

#include "BeamPipe.hh"

BeamPipe::BeamPipe(G4double winPosZ,G4double XwThickness) {
	zw = (winPosZ > 0.0) ? winPosZ : +400.0*CLHEP::mm ; // vacuum window begining 
	wt = (XwThickness > 0.0) ? XwThickness : 0.5*CLHEP::mm ;
	}

void BeamPipe::Init() {
	G4double po = 50.0*CLHEP::mm ;    // beam pipe outer radius
	G4double pi = po - 2*CLHEP::mm ;  // beam pipe inner radius
	G4double bo = 80.0*CLHEP::mm ;    // drum outer radius 
	G4double zo = -200.0*CLHEP::mm ;  // begining of the beam pipe
	G4double z1 = -100.0*CLHEP::mm ;  // drum begining
	G4double z2 = +100.0*CLHEP::mm ;  // drum ending
	G4double zend = +800.0*CLHEP::mm ;// very end of the beam pipe 
	G4double dz = 5.0*CLHEP::mm ;     // longitudinal thicknesss
	G4double dr = 5.0*CLHEP::mm ;     // radial thickness
	G4double tg = tan(25*CLHEP::deg) ;// beap pipe opening angle 
	
	Zs[ 0]=zo ;               Ri[ 0]=pi ;                            Ro[ 0]=po ;
	Zs[ 1]=Zs[ 2]=z1 ;        Ri[ 1]=Ri[ 2]=pi ;                     Ro[ 1]=po ; Ro[ 2]=bo ;
	Zs[ 3]=Zs[ 4]=z1+dz ;     Ri[ 3]=pi ; Ri[ 4]=bo-dr ;             Ro[ 3]=Ro[ 4]=bo ;
	Zs[ 5]=Zs[ 6]=z2-dz ;     Ri[ 5]=bo-dr ; Ri[ 6]=tg*(z2-dz) ;     Ro[ 5]=Ro[ 6]=bo ;
	Zs[ 7]=Zs[ 8]=z2 ;        Ri[ 7]=Ri[ 8]=tg*z2 ;                  Ro[ 7]=bo ; Ro[ 8]=tg*z2+dr ;
	Zs[ 9]=Zs[10]=zw ;        Ri[ 9]=tg*zw ; Ri[10]=pi ;             Ro[ 9]=Ro[10]=tg*zw+dr ;
	Zs[11]=Zs[12]=zw+wt ;     Ri[11]=Ri[12]=pi ;                     Ro[11]=tg*(zw+wt)+dr ; Ro[12]=po ;
	Zs[13]=zend ;             Ri[13]=pi ;                            Ro[13]=po ;
	}

G4LogicalVolume* BeamPipe::BuildLogicalBeamPipe(G4Material *material) {
	G4VisAttributes	*visAttr = new G4VisAttributes(G4Colour(0.6,0.6,0.6)) ;
	visAttr->SetVisibility(true) ;
	Init() ;
 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("beam_pipe",90*CLHEP::deg,270*CLHEP::deg,14,Zs,Ri,Ro),material,"beam.W") ;
//++ 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("beam_pipe",0*CLHEP::deg,360*CLHEP::deg,14,Zs,Ri,Ro),material,"beam.W") ;
	logical->SetVisAttributes(visAttr) ;
	return logical ;
	}

G4LogicalVolume* BeamPipe::BuildLogicalBeam(G4Material *material) {
	G4VisAttributes	*visAttr = new G4VisAttributes(G4Colour(0.8,0.4,0.4,0.6)) ;
	visAttr->SetVisibility(true) ;
	for (int i = 0 ; i < 14 ; i++) Ro[i] = 4.0 * CLHEP::mm ;
	for (int i = 0 ; i < 14 ; i++) Ri[i] = 0.0 ;
//++ 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("beam_pipe",90*CLHEP::deg,270*CLHEP::deg,14,Zs,Ri,Ro),material,"beam.W") ;
 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("beam_pipe",0*CLHEP::deg,360*CLHEP::deg,14,Zs,Ri,Ro),material,"beam.W") ;
	logical->SetVisAttributes(visAttr) ;
	return logical ;
	}

G4LogicalVolume* BeamPipe::BuildLogicalVacuum(G4Material *material) {
	G4VisAttributes	*visAttr = new G4VisAttributes(G4Colour(0.8,0.8,0.4)) ;
	visAttr->SetVisibility(true) ;
	Init() ;
	for (int i = 0 ; i < 14 ; i++) Ro[i] = Ri[i] ;
	for (int i = 0 ; i < 14 ; i++) Ri[i] = 4.0 * CLHEP::mm ;
 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("COSY.vac",90*CLHEP::deg,270*CLHEP::deg,14,Zs,Ri,Ro),material,"COSY.vac") ;
//++ 	G4LogicalVolume *logical = new G4LogicalVolume(new G4Polycone("COSY.vac",0*CLHEP::deg,360*CLHEP::deg,14,Zs,Ri,Ro),material,"COSY.vac") ;
	logical->SetVisAttributes(visAttr) ;
	return logical ;
	}

// e-o-f
