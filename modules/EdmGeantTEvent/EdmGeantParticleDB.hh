// File:	EdmGeantParticleDB.hh
// Date:	09-Apr-2015 G.Macharashvili Dubna

#ifndef EdmGeantParticleDB_hh
#define EdmGeantParticleDB_hh

// ***************************
// Energy/Momentum/Mass in MeV
// ***************************

#include <fstream>
#include <iostream>
#include <iomanip> 
#include <cstring>
#include <vector>
using namespace std ;

class EdmGeantParticleDB {

	class EdmGeantParticle {

	public:

		EdmGeantParticle(int xcode,string &xname,double xmass) : code(xcode) , name(xname) , mass(xmass) {}
		int		code ;
		string		name ;
		double		mass ;

		} ;

private:

static	EdmGeantParticleDB			*ptr ;
static	vector<EdmGeantParticle*>		ptrVector ;
static	vector<EdmGeantParticle*>::iterator	ptrIterator ;

public:

static	EdmGeantParticleDB *Get(const char *f = "uparticle_data.dat") { return (ptr) ? ptr : new EdmGeantParticleDB(f) ; }
	double		GetMass(const char * xname) ;
	double		GetMass(int xcode) ;
	const char*	GetName(int xcode) ;
	int		GetCode(const char * xname) ;

private:

virtual			~EdmGeantParticleDB() ;

			EdmGeantParticleDB(const char * = 0) ;
static	void		Fatal(const char *txt,int id = 0) ;
static	void		GetDB(const char * = 0) ;

	} ;

#endif

// e-o-f
