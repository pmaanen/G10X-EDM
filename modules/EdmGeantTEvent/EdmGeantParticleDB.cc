// File:	EdmGeantParticleDB.cc
// Date:	09-Apr-2015 G.Macharashvili Dubna

#include "EdmGeantParticleDB.hh"
#include <stdlib.h>

EdmGeantParticleDB	*EdmGeantParticleDB::ptr = 0 ;
	vector<EdmGeantParticleDB::EdmGeantParticle*>		EdmGeantParticleDB::ptrVector ;
	vector<EdmGeantParticleDB::EdmGeantParticle*>::iterator	EdmGeantParticleDB::ptrIterator ;


EdmGeantParticleDB::EdmGeantParticleDB(const char * db_file_name) {
	if (ptr) return ;
	ptr = this ;
	ptrVector.reserve(200) ;
	ptrVector.clear() ;
	if (db_file_name) GetDB(db_file_name) ;
	}


void	EdmGeantParticleDB::Fatal(const char *txt,int id) {
	cout << "**** EdmGeantParticleDB: " << txt << endl ;
	if (id == 0) return ;
	cout << "     EdmGeantParticleDB: execution aborted ... " << endl << endl ;
	exit(0) ;
	}


void	EdmGeantParticleDB::GetDB(const char *db_file_name) {
	if (ptrVector.size() > 0) return ;
	ifstream in ;
	string	name ;
	int	code ;
	double	mass ;
	if (db_file_name) in.open(db_file_name) ;
	if (!in) {
		in.open("EdmGeantParticleDB.dat") ;
		if (!in) Fatal("EdmGeantParticleDB.dat - not found ... ",1) ;
		}
	while (!in.eof()) {
		in >> ws >> code >> code >> name >> mass >> ws ;
		ptrVector.push_back(new EdmGeantParticle(code,name,mass)) ;
		}
	in.close() ;
	cout << endl ;
	cout << ptrVector.size() << " particles accepted from file: " << db_file_name << endl ;
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) {
		cout << setw(16) << (*ptrIterator)->name << " " ;
		cout << setw(8) << (*ptrIterator)->code << " " ;
		cout << showpoint << fixed << setprecision(2) << setw(8) << (*ptrIterator)->mass << " " ;
		cout << endl ;
		}
	}


EdmGeantParticleDB::~EdmGeantParticleDB() {
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) delete *ptrIterator ;
	}

double	EdmGeantParticleDB::GetMass(const char *n) {
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) if ((*ptrIterator)->name == n) return (*ptrIterator)->mass ;
	return 0.0 ;
	}

double	EdmGeantParticleDB::GetMass(int c) {
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) if ((*ptrIterator)->code == c) return (*ptrIterator)->mass ;
	return 0.0 ;
	}

const char *	EdmGeantParticleDB::GetName(int c) {
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) if ((*ptrIterator)->code == c) return (*ptrIterator)->name.c_str() ;
	return "undef" ;
	}

int	EdmGeantParticleDB::GetCode(const char * n) {
	for (ptrIterator = ptrVector.begin() ; ptrIterator != ptrVector.end() ; ptrIterator++) if ((*ptrIterator)->name == n) return (*ptrIterator)->code ;
	return 0 ;
	}


/*

int main() {

	EdmGeantParticleDB *particle = EdmGeantParticleDB::Get("uparticle_data.dat") ;

cout << "proton" << " " << particle->GetCode("proton") << endl ;
cout << 2112 << " " << particle->GetName(2112) << endl ;
	return 0 ;
	}
*/

// e-o-f
