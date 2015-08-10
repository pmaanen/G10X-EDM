// File:	X_VertexGenerator.cc
// Date:	23-Jan-2015 G.Macharashvili Dubna

#include "X_VertexGenerator.hh"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iomanip>

#include <string.h>
#include <iostream>
#include <sstream>

#include <iomanip>
using namespace std ;

X_VertexGenerator::X_VertexGenerator(const char * lib_name, const char *generator_name) {
// lib ...
	string lib_path ;
	if (!lib_name) lib_path = string("libVertexGenerators.so") ;
	else if (0 == strlen(lib_name)) lib_path = string("libVertexGenerators.so") ;
	else lib_path = string(lib_name) ;
// class ...
	string vg_class ;
	if (!generator_name) vg_class = string("VertexGeneratorO") ;
	else if (0 == strlen(generator_name)) vg_class = string("VertexGeneratorO") ;
	else vg_class = string(generator_name) ;
// mangle ...
	stringstream class_name(ios::out) ;
	class_name.seekp (0,ios::beg) ;
	class_name << "_ZN" << vg_class.length() << vg_class << "3getEv" ;
// show lib/class ...
        cout << "VertexGenerator " << endl ;
	cout << "    lib:    " << lib_path << endl ;
	cout << "    class:  " << vg_class << endl ;
// open lib ...
	handle = dlopen(lib_path.c_str(),RTLD_LAZY) ;
	if (!handle) { cerr << dlerror() << endl ; exit(EXIT_FAILURE) ; }
// load class ...
	void *tmp = dlsym(handle,class_name.str().c_str()) ;
        if (!tmp) { cerr << dlerror() << endl ; exit(EXIT_FAILURE) ; }
	genFunct = 0 ;
// get class pionter ...
	VVertexGenerator* (*xgen)() = reinterpret_cast<VVertexGenerator*(*)()> (tmp) ;
//++	(*xgen)()->print() ;
	genFunct = (*xgen)() ;
// error processiong ...
	if (0 == genFunct) {
		cerr << "VertexGenerator: probably the generator class name is wrong \"" << class_name.str() << "\"" << endl ;
		abort() ;
		}
	else {
		cout << endl ;
//++		cout << "new generator classes have to be added at source/modules/Generators/ ..." << endl ;
//++		cout << endl ;
		}
	}

X_VertexGenerator::~X_VertexGenerator() {
	dlclose(handle) ;
	}

// e-o-f
