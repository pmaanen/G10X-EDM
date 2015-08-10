// File:	X_InteractionGenerator.cc
// Date:	23-Jan-2015 G.Macharashvili Dubna

#include "X_InteractionGenerator.hh"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <iomanip>

#include <string.h>
#include <iostream>
#include <sstream>

#include <iomanip>
using namespace std ;

X_InteractionGenerator::X_InteractionGenerator(const char * lib_name, const char *generator_name) {
// lib ...
	string lib_path ;
	if (!lib_name) lib_path = string("libInteractionGenerators.so") ;
	else if (0 == strlen(lib_name)) lib_path = string("libInteractionGenerators.so") ;
	else lib_path = string(lib_name) ;
// class ...
	string vg_class ;
	if (!generator_name) vg_class = string("UniformThetaGenerator") ;
	else if (0 == strlen(generator_name)) vg_class = string("UniformThetaGenerator") ;
	else vg_class = string(generator_name) ;
// mangle ...
	stringstream class_name(ios::out) ;
	class_name.seekp (0,ios::beg) ;
	class_name << "_ZN" << vg_class.length() << vg_class << "3getEv" ;
// show lib/class ...
        cout << "InteractionGenerator " << endl ;
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
	VInteractionGenerator* (*xgen)() = reinterpret_cast<VInteractionGenerator*(*)()> (tmp) ;
//++	(*xgen)()->print() ;
	genFunct = (*xgen)() ;
// error processiong ...
	if (0 == genFunct) {
		cerr << "InteractionGenerator: probably the generator class name is wrong \"" << class_name.str() << "\"" << endl ;
		abort() ;
		}
	cout << endl ;
	}

X_InteractionGenerator::~X_InteractionGenerator() {
	dlclose(handle) ;
	delete genFunct ;
	}

// e-o-f
