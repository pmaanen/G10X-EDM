// File:	UDynamicManager.cc
// Date:	08-Mar-2007 G.Macharashvili Juelich

#include "UDynamicManager.hh"

string	UDynamicManager::undef = "undef" ;
UDynamicManager::UDynamicManager() {
	List.reserve(100) ;
	List.clear() ;
	}
void	UDynamicManager::Register(const char *n,int a,void *p) {
	if (Exists(n)) List[k].counter++ ;
	else List.push_back(Descriptor(n,a,p)) ;
	}
bool	UDynamicManager::Exists(const char *n) { return (GetIndex(n) != -1) ; }
int	UDynamicManager::GetAttribute(const char *n) { return (Exists(n)) ? List[k].attribute : 0 ; }
string&	UDynamicManager::GetName(int attr) {
	for (k = 0 ; k < Size() ; k++) {
		if (List[k].attribute == attr) return List[k].name ;
		}
	return undef ;
	}
string&	UDynamicManager::GetNameByIndex(int k) {
	if (k >= 0 && k < Size()) return List[k].name ;
	else return undef ;
	}
int	UDynamicManager::GetCounter(int k) const {
	if (k >= 0 && k < Size()) return List[k].counter ;
	else return 0 ;
	}
void*	UDynamicManager::GetPointer(const char *n) { return (Exists(n)) ? List[k].pv : 0 ; }
int	UDynamicManager::GetIndex(const char *n) {
	for (k = 0 ; k < Size() ; k++) {
		if (List[k].name == n) return k ;
		}
	return -1 ;
	}
void	UDynamicManager::Print(ostream &os,const char *prefix,int v) {
	if (!v) return ;
	if (prefix) os << prefix ;
	os << "UDynamicManager: " << Size() << " descriptors registered." << endl ;
	for (Iterator = List.begin() , k = 0 ; Iterator != List.end() ; Iterator++ , k++) {
		os << setw(12) << k << " " ;
		os << setw(20) << left << Iterator->name << right << " " ;
		os << setw(6) << Iterator->attribute << "  " ;
		os << setw(7) << Iterator->counter << "  " ;
		if (Iterator->pv) os << Iterator->pv << " " ;
		os << endl ;
		}
	}

// eof
