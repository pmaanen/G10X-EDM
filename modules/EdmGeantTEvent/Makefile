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

LIB_NAME  = EdmGeantTEvent

DICTNAME = $(LIB_NAME)Dict
LINKNAME = $(LIB_NAME)LinkDef.hh

MODULES = EdmGeantTEvent EdmGeantParticleDB 

LIBDIR  = ${HOME}/lib

CXXFLAGS = -g -O2 -fPIC -fno-strict-aliasing -Wall 
CXXFLAGS += `root-config --cflags` 
CXXFLAGS += -I${HOME}/include 

LDFLAGS += `root-config --libs`

LIBNAME  = $(LIBDIR)/lib$(LIB_NAME).so
.PHONY: all lib include clean

all:	include lib
	@echo -e "\033[31m"   lib${LIB_NAME}.so ... ok "\033[0m"
	
$(DICTNAME).cc: $(addsuffix .hh,$(MODULES)) $(LINKNAME)
	@echo "  generating one dictionary for all classes: $@"
	@`root-config --prefix`/bin/rootcint -f $@ -c $(CXXFLAGS) -p $^

lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) $(addsuffix .o,$(DICTNAME)) 
	@echo "  linking shared library with dictionary $@ ..."
	@$(CXX) -fPIC $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

%.o: %.cc
	@if [ -f $< ] ; then echo "  compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

%.so: %.cc
	$(CXX) -fPIC -O2 -shared $(LDFLAGS) -o $@

$(HOME)/include/%.hh: %.hh
	@if [ ! -d ${INC} ] ; then mkdir ${INC} ; fi
	@echo "  copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

clean: 
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@-rm $(DICTNAME)*
	@-rm $(LIBNAME)

Test_main: Test_main.o
	$(CXX) -L$(HOME)/lib -lEdmGeantTEvent $(LDFLAGS) $^ -o $@

Test_main.o: Test_main.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# e-o-f
