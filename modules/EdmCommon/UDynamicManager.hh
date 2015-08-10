// File:	UDynamicManager.hh
// Date:	08-Mar-2007 G.Macharashvili Juelich

#ifndef UDynamicManager_hh
#define UDynamicManager_hh 1

#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector> 
using namespace std ;

class	UDynamicManager {

private:

class	Descriptor {

public:

	Descriptor(const char *n,int a,void *p) : name(n) , attribute(a) , counter(1) , pv(p) {}
	string		name ;
	int		attribute ;
	int		counter ;
	void		*pv ;

	} ;

	vector<Descriptor>		List ;
	vector<Descriptor>::iterator	Iterator ;
	int				k ;
static	string				undef ;

public:

	UDynamicManager() ;
virtual	~UDynamicManager() {}

	int		GetIndex(const char *n) ;
	int		GetIndex(string &n) { return GetIndex(n.c_str()) ; }

	void		Register(const char *n , int a = 0 , void *p = 0 ) ;
	void		Register(string &n , int a = 0 , void *p = 0 ) { Register(n.c_str(),a,p) ;}

	bool		Exists(const char *n) ;
	bool		Exists(string &n) { return Exists(n.c_str()) ; }

	int		GetAttribute(const char *n) ;
	int		GetAttribute(string &n) { return GetAttribute(n.c_str()) ; }

	void		*GetPointer(const char *n) ;
	void		*GetPointer(string &n) { return GetPointer(n.c_str()) ; }
	void		*GetPointer(int kx) { return List[kx].pv ; }
	
	string&		GetName(int attribute) ;
	string&		GetNameByIndex(int k) ;
	int		GetCounter(int k) const ;

	void		Reset() { List.clear() ; }
	int		Size() const { return List.size() ; }
virtual	void		Print(ostream &os = cout,const char *prefix = 0,int v = 1) ;

	} ;

#endif

// eof
