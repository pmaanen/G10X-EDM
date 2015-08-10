// File:	VolumeGenerator.hh
// Date:	31-Jan-2015 G.Macharashvili Dubna

#ifndef VolumeGenerator_hh
#define VolumeGenerator_hh

#include <ConfigFileReader.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>

class VolumeGenerator : public ConfigFileReader {

protected:

	double		par[12] ;

public:

	VolumeGenerator(const char *file_name = "config.dat",int commented_line_marker = '#') ;
virtual	~VolumeGenerator() { if (input_stream.is_open()) input_stream.close() ; }

	G4VSolid	*getG4Box(const char *obj_name) ;
	G4VSolid	*getG4Tubs(const char *obj_name) ;

	} ;

#endif

// e-o-f
