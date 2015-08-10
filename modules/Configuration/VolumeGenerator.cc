// File:        VolumeGenerator.cc
// Date:        30-Jan-2015 G.Macharashvili Dubna

#include "VolumeGenerator.hh"

VolumeGenerator::VolumeGenerator(const char *file_name,int clm) : ConfigFileReader(file_name,clm) {}

G4VSolid *VolumeGenerator::getG4Box(const char *obj_name) {
	string xclass("G4Box.") ;
	xclass += obj_name ;
	if (getDouble(xclass.c_str(),par,3)) {
		if (verbosity > 0) {
			cerr << "VolumeGenerator:getG4Box() format: G4Box.name  x  y  z (full sizes)" << endl ;
			cerr << "VolumeGenerator: \"" << xclass << "\" not found ... " << endl ;
			}
		return 0 ;
		}
	else return new G4Box(obj_name,par[0]/2,par[1]/2,par[2]/2) ;
	}

G4VSolid *VolumeGenerator::getG4Tubs(const char *obj_name) {
	string xclass("G4Tubs.") ;
	xclass += obj_name ;
	if (getDouble(xclass.c_str(),par,5)) {
		if (verbosity > 0) {
			cerr << "VolumeGenerator:getG4Tubs() format: G4Tubs.name  inRad outRad totalZsize phi1 phi2" << endl ;
			cerr << "VolumeGenerator: \"" << xclass << "\" not found ... " << endl ;
			}
		return 0 ;
		}
	else return new G4Tubs(obj_name,par[0],par[1],par[2]/2,par[3],par[4]) ;
	}

// e-o-f
