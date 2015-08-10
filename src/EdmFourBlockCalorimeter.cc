// File:	EdmFourBlockCalorimeter.cc
// Date:	12-Jun-2015 G.Macharashvili Juelich

#include "EdmFourBlockCalorimeter.hh"

#include <G4SystemOfUnits.hh>

G4String	EdmFourBlockCalorimeter::className = "FB.calorimeter" ;

EdmFourBlockCalorimeter::EdmFourBlockCalorimeter() {
	numberOfModules = 0 ;
	sx = 30.0 * CLHEP::mm , sy = 30.0 * CLHEP::mm , sz = 100.0 * CLHEP::mm ;
	}

EdmFourBlockCalorimeter::~EdmFourBlockCalorimeter() {
	if (numberOfModules > 0) {
		cout << endl ;
		cout << fixed << showpoint << setprecision(1) ;
		cout << "EdmFourBlockCalorimeter::dtor ..." << endl ;
		cout << "    calorimeter consists of " << numberOfModules << " modules " ;
		cout << "[" << sx << "x" << sy << "x" << sz << "] " ;
		cout << endl ;
		cout << "    total volume:  " << calTotalVolume/CLHEP::cm3 << " [cm3] " << endl ;
		cout << "    total price:   " << calTotalVolume/CLHEP::cm3 * 50.0 << " $ " << endl ;
		cout << endl ;
		cout << endl ;
		}
	}

G4LogicalVolume* EdmFourBlockCalorimeter::ConstructLogical(const char *name,G4Material *material) {
	solid = new G4Box(name,sx/2,sy/2,sz/2) ;
	G4VisAttributes	*VSi = new G4VisAttributes(G4Colour(0.0, 0.7, 0.1)) ;
			VSi->SetVisibility(true) ;
			VSi->SetForceSolid(true) ;
	logical = new G4LogicalVolume(solid,material,name) ;
	logical->SetUserLimits(new G4UserLimits(100.0 * CLHEP::mm,1000*CLHEP::mm,100*CLHEP::ns,0,0)) ;
	logical->SetVisAttributes(VSi) ;
	return logical ;
	}

void EdmFourBlockCalorimeter::Build(G4LogicalVolume *logicalWorld,double z_pos,int NBradX,int NBtanY,double R1) {
	G4double x , y ;
	G4double xo = (R1 > 50.0 * CLHEP::mm) ? R1 : 50.0 * CLHEP::mm ;
	G4double yo = -sx * NBtanY/2 ;
	for (int ix = 0 ; ix < NBradX ; ix++) {
		x = xo + sx * (ix + 0.5) ;
		for (int iy = 0 ; iy < NBtanY ; iy++) {
			y = yo + sy * (iy + 0.5) ;
			G4int index = iy * 10 + ix ;
			stringstream name(ios::out) ;
			name.seekp (0,ios::beg) ;
			name << iy << "." << ix ;
			G4String nameStr = name.str() ;
			new G4PVPlacement(0,G4ThreeVector( x, y,z_pos + sz/2),logical,G4String("m.0.") + nameStr,logicalWorld,false,index    ,false) ;
			new G4PVPlacement(0,G4ThreeVector( y,-x,z_pos + sz/2),logical,G4String("m.1.") + nameStr,logicalWorld,false,index+100,false) ;
			new G4PVPlacement(0,G4ThreeVector(-x,-y,z_pos + sz/2),logical,G4String("m.2.") + nameStr,logicalWorld,false,index+200,false) ;
			new G4PVPlacement(0,G4ThreeVector(-y, x,z_pos + sz/2),logical,G4String("m.3.") + nameStr,logicalWorld,false,index+300,false) ;
			numberOfModules+=4 ;
			}
		}

	calTotalVolume = numberOfModules * sx * sy * sz ;
	cout << "EdmFourBlockCalorimeter: constructed ..." << endl ;
	}

// e-o-f
