// File:	EdmModularCalorimeter.cc
// Date:	19-May-2015 G.Macharashvili Juelich

#include "EdmModularCalorimeter.hh"

#include <G4SystemOfUnits.hh>

G4String	EdmModularCalorimeter::className = "M.calorimeter" ;

EdmModularCalorimeter::EdmModularCalorimeter() {
	numberOfModules = 0 ;
	nx = 40 , ny = 40 ;
	}

EdmModularCalorimeter::~EdmModularCalorimeter() {
	if (numberOfModules > 0) {
		cout << endl ;
		cout << fixed << showpoint << setprecision(1) ;
		cout << "EdmModularCalorimeter::dtor ..." << endl ;
		cout << "    calorimeter consists of " << numberOfModules << " modules " ;
		cout << "[" << sx << "x" << sy << "x" << sz << "] " ;
		cout << endl ;
		cout << "    total volume:  " << calTotalVolume/CLHEP::cm3 << " [cm3] " << endl ;
		cout << "    total price:   " << calTotalVolume/CLHEP::cm3 * 50 << " $ " << endl ;
		cout << endl ;
		cout << endl ;
		}
	}

G4LogicalVolume* EdmModularCalorimeter::ConstructLogical(const char *name,double dx,double dy,double dz,double R1,double R2,G4Material *material) {
	sx = dx , sy = dy , sz = dz ;
	minRad = R1 ;
	maxRad = R2 ;
// calorimeter module solid ...
	solid = new G4Box(name,dx/2,dy/2,dz/2) ;
// vis attributes ...
	G4VisAttributes	*VSi = new G4VisAttributes(G4Colour(0.0, 0.7, 0.1)) ;
			VSi->SetVisibility(true) ;
			VSi->SetForceSolid(true) ;
// create logical ...
	logical = new G4LogicalVolume(solid,material,name) ;
	logical->SetUserLimits(new G4UserLimits(100.0 * CLHEP::mm,1000*CLHEP::mm,100*CLHEP::ns,0,0)) ;
	logical->SetVisAttributes(VSi) ;
	return logical ;
	}

void EdmModularCalorimeter::Build(G4LogicalVolume *logicalWorld,double z_pos,int numOfModules,int *ix,int *iy) {
	G4double x , y , r ;
	G4double xo = (nx%2) ? -nx/2*sx : -nx/2*sx + sx/2 ;
	G4double yo = (ny%2) ? -ny/2*sy : -ny/2*sy + sy/2 ;
// radius controlled placement ...
	if (0 == numOfModules) {
		for (int i = 0 ; i < nx ; i++) {
			x = xo + sx * i ;
			for (int j = 0 ; j < ny ; j++) {
				y = yo + sy * j ;
				r = sqrt(x*x + y*y) ;
				if (r > maxRad) continue ;
				if (r < minRad) continue ;
				stringstream name(ios::out) ;
				name.seekp (0,ios::beg) ;
				name << "m." << i << "." << j ;
				G4int index = j * 1000 + i ;
				new G4PVPlacement(0,G4ThreeVector(x,y,z_pos + sz/2),logical,name.str(),logicalWorld,false,index,false) ;
				numberOfModules++ ;
				}
			}
		}
// individual crystal placement NOT READY ... GM-20150522
	else {
		for (int c = 0 ; c < numOfModules ; c++) {
			x = xo + sx * ix[c] ;
			y = yo + sy * iy[c] ;
			// add here inner radius restriction only ... ??
			stringstream name(ios::out) ;
			name.seekp (0,ios::beg) ;
			name << "m." << ix[c] << "." << iy[c] ;
			G4int index = ix[c] * 1000 + iy[c] ;
			new G4PVPlacement(0,G4ThreeVector(x,y,z_pos + sz/2),logical,name.str(),logicalWorld,false,index,false) ;
			numberOfModules++ ;
			}
		}

	calTotalVolume = numberOfModules * sx * sy * sz ;
// make sensitive the logical volume ...
// MAKE SENSITIVE IN THE CALLING CLASS. X2_Detector.cc ... GM-20150522
// e.g.:	new GG4ModuleSD(logical) ;
//	cout << "EdmModularCalorimeter: constructed ..." << endl ;
	}

// e-o-f
