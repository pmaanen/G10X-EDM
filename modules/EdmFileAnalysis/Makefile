# File:		Makefile
# Date:		08-Apr-2015 G.Macharashvili Dubna

# Important:
#   'libEdmGeantTEvent.so' does not require any class of Geant4
#   It can be used for root-file access to extract events 
#   as well as to write events from Geant4 framework.

# Classes:
#   EdmGeantTEvent
#   EdmGeantTPTrack
#   EdmGeantTHit

LIBDIR  = ${HOME}/lib

CXXFLAGS = -g -O2 -fPIC -fno-strict-aliasing -Wall 
CXXFLAGS += `root-config --cflags` 
CXXFLAGS += -I${HOME}/include 

LDFLAGS += `root-config --libs` -L${LIBDIR} -lEdmGeantTEvent 

.PHONY: all clean

all:	Test_main
	
%.o: %.cc
	@if [ -f $< ] ; then echo "  compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

clean: 
	@-rm *.o *\~

Test_main: Test_main.o
	$(CXX) -L$(HOME)/lib -lEdmGeantTEvent $(LDFLAGS) $^ -o $@

Test_main.o: Test_main.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# e-o-f
