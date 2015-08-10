// File:	ConfigFileReader.cc
// Date:	30-Mar-2011 G.Macharashvili Dubna
// Date:	30-Jun-2014 G.Macharashvili Dubna

#include "ConfigFileReader.hh"

	ConfigFileReader::ConfigFileReader(const char *file_name,int clm) {
	name = string(file_name) ;
	input_stream.open(file_name) ;
	comment_line_marker = clm ;
	verbosity = 1 ;
// Example ...
/*
	if (input_stream.open(FileName)) {}
	else if (input_stream.open(".SetupDB")) {}
	else if (input_stream.open("~/.SetupDB")) {}
	else if (input_stream.open("/usr/local/SetupDB")) {}
	else {	cout << " File: " << FileName ;
		cout << " - no one exist: " << endl ;
		cout << "    .Config.DB " << endl ;
		cout << "    ~/.Config.DB " << endl ;
		cout << "    /usr/local/Config.DB " << endl ;
		cout << "    ... Fatal " << endl ;
		exit(1) ;		
		}
*/
	}

void	ConfigFileReader::warning(const char *par_name,const char *msg) {
	if (0 == msg) {
		if (verbosity > 0) cerr << "(W) ConfigFileReader:  warning. parameter [" << par_name << "] not found ... " << endl ;
		}
	else {
		cerr << "(E) ConfigFileReader: fatal error. [" << par_name << "] " << msg << endl << endl ;
		abort() ;
		}
	}

int	ConfigFileReader::getPosition(const char *par_name) {
	if (!isOpen()) {
		cerr << "(E) ConfigFileReader: error. file: [" << name << "] could not open ... FATAL" << endl ;
		abort() ;
		}
	input_stream.clear() ;
	input_stream.seekg(0,ios::beg) ;
	par_len = strlen(par_name) ;
	do {
		input_stream >> ws ;
		if (input_stream.peek() == comment_line_marker || input_stream.peek() == '\n') {
			input_stream.ignore(max_buf_len,'\n') ;
			continue ;
			}
		input_stream.get(buffer,par_len + 1) ;
		//++??++for (unsigned int k = 0 ; k < par_len ; k++) tolower(buffer[k]) ;
		if (!strncmp(buffer,par_name,par_len)) {
			if (input_stream.peek() == ':') input_stream.get() ; // remove trailing ":" 
			return 0 ;
			}
		else input_stream.ignore(max_buf_len,'\n') ;
		} while(input_stream.good()) ;
	warning(par_name) ;
	return -1 ;
	}
		
int	ConfigFileReader::getDouble(const char *par_name,double &x) {
		if (getPosition(par_name)) return -2 ;
		input_stream >> ws >> x ;
		return 0 ;
		}

int	ConfigFileReader::getInt(const char *par_name,int &x) {
		if (getPosition(par_name)) return -2 ;
		input_stream >> ws >> x ;
		return 0 ;
		}

int	ConfigFileReader::getDouble(const char *par_name,double *x,int nx) {
		if (getPosition(par_name)) return -2 ;
		for (int i = 0 ; i < nx ; i++) input_stream >> ws >> *x++ ;
		return 0 ;
		}

int	ConfigFileReader::getInt(const char *par_name,int *x,int nx) {
		if (getPosition(par_name)) return -2 ;
		for (int i = 0 ; i < nx ; i++) input_stream >> ws >> *x++ ;
		return 0 ;
		}

int	ConfigFileReader::getString(const char *par_name,string &x) {
		if (getPosition(par_name)) return -2 ;
		input_stream.getline(buffer,max_buf_len) ;
		stringstream sg(buffer) ;
		if (!sg.eof()) {
			sg >> ws ;
			if (sg.peek() == '\"') {
				sg.get() ;
				sg.getline(buffer,max_buf_len,'\"') ;
				x = string(buffer) ;
				if (x[x.length()-1] == '\"') x=x.substr(0,x.length()-1) ;
				}
			else sg >> x ;
			}
		return 0 ;
		}

int	ConfigFileReader::getString(const char *par_name,char *x,int max) {
		if (getPosition(par_name)) return -2 ;
		input_stream.getline(buffer,max_buf_len) ;
		stringstream	sg(buffer) ;
		if (!sg.eof()) {
			sg >> ws ;
			if (sg.peek() == '\"') {
				sg.get() ;
				sg.getline(x,max,'\"') ;
				}
			else sg.getline(x,max,'\0') ;
			}
		return 0 ;
		}

// no commented strings inside {...} 
// returned *x is zero terminated string. 
// example:
//            #include <string>       // std::string
//            #include <iostream>     // std::cout
//            #include <sstream>      // std::stringstream
//            char x[400] ;
//            getClass("someClassName",x,400) ;
//            stringstream Xclass(ios::in | ios::out) ;
//            Xclass << x ;
//            Xclass >> intA >> doubleB >> stringC >> etc ; // depending on known class members ...
//
int	ConfigFileReader::getClass(const char *par_name,char *x,int max) {
		if (getPosition(par_name)) return -2 ;
		int b = 0 , c = 0 , i = 0 ;
		while (!input_stream.eof()) {
			if ( i > max - 1) warning(par_name,"too long class (i > max_buff) ...") ;
			c = input_stream.get() ;
			if (0 == b) {
				if (c == '{') {
					b = 1 ;
					continue ;
					}
				else if (c == ' ' || c == '\n' || c == '\t') continue ;
				else warning(par_name,"non ws before \"{\" ") ;
				}
			else if (1 == b) {
				if (c == '}') {
					b = 2 ;
					break ;
					}
				if (c == '\n' || c == '\t' || c == ' ') {
					if (i > 0 && x[i-1] != ' ') x[i++] = ' ' ;
					}
				else x[i++] = c ;
				}
			}
		if (2 != b) warning(par_name,"eof before \"}\"") ;
		x[i] = '\0' ;
		return 0 ;
		}

/*

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
int main() {
	ConfigFileReader cfg ;
	cfg.open("test.cfg") ;

	int buf[10] ;
	char x[400] ;
	int a = 0 ;
	double size[3] , xx , yy , zz ;
	string str ;

	cout << cfg.getInt("int.set.4",buf,10) << endl ;
	cout << buf[0] << " " << buf[1] << " " << buf[2] << " " << buf[3] << endl ;

	cfg.getClass("someClassName.object.2",x,400) ;
	stringstream Xclass(ios::in | ios::out) ;
	Xclass << x ;
	cout << "x: {" << x << "}" << endl ;
	Xclass >> a >> xx >> yy >> zz ;
	Xclass >> str >> size[0] >> size[1] >> size[2] ;
	cout << "a:   " << a << endl ;
	cout << fixed << showpoint << setprecision(4) ;
	cout << "xyz: " ;
	cout << setw(7) << xx << " " ;
	cout << setw(7) << yy << " " ;
	cout << setw(7) << zz << " " ;
	cout << endl ;	
	cout << str << "  " ;
	cout << setw(7) << size[0] << " " ;
	cout << setw(7) << size[1] << " " ;
	cout << setw(7) << size[2] << " " ;
	cout << endl ;	

	cfg.close() ;
	return 0 ;
	}
*/

/*
# test.cfg

int.set.4 1 2 3 
int.set.4   1 2 3   5

someClassName.object.2 {
	1223       0.987 0.123 0.444
	xdet.size 0.111 0.222 0.333
	}

# eof
*/

// e-o-f
