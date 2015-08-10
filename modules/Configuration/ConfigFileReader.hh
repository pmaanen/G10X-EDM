// File:	ConfigFileReader.hh
// Date:	30-Mar-2011 G.Macharashvili Dubna
// Date:	30-Jun-2014 G.Macharashvili Dubna

#ifndef ConfigFileReader_hh
#define ConfigFileReader_hh

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
using namespace std ;

class ConfigFileReader {

protected:

		int		verbosity ;
static	const	int		max_buf_len = 2048 ;
		int		par_len ;
		int		comment_line_marker ;
		string		name ;
		ifstream	input_stream ;
		char		buffer[max_buf_len] ;

public:

	ConfigFileReader(const char *file_name = "config.dat",int commented_line_marker = '#') ;
virtual	~ConfigFileReader() { if (input_stream.is_open()) input_stream.close() ; }

	void	open(const char *file_name) { input_stream.open(file_name) ; }
	void	close() { input_stream.close() ; }
	int	isOpen() { return input_stream.is_open() ; }
	void	setVerboseLevel(int v = 0) { verbosity = v ; }
	int	getVerboseLevel() const { return verbosity ; }
	int	getDouble(const char *par_name,double &x) ;
	int	getDouble(const char *par_name,double *x,int nx) ;
	int	getInt(const char *par_name,int &x) ;
	int	getInt(const char *par_name,int *x,int nx) ;
	int	getString(const char *par_name,char *x,int max) ;
	int	getString(const char *par_name,string &x) ;
	int	getClass(const char *par_name,char *x,int max) ;

protected:

	int	getPosition(const char *par_name) ;
	void	warning(const char *par_name,const char * = 0) ;

	} ;

#endif

// e-o-f
