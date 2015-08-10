# Date:		19-Jun-2015 G.Macharashvili Juelich
# Edm generated event analysis and persistency library generation ...

MODULES = EdmGeantEventSave
#MODULES+= 
#MODULES+= 

LIB_DIR  = ${HOME}/lib
LIB_NAME = EdmGeantAnalysis
LIBNAME  = $(LIB_DIR)/lib$(LIB_NAME).so

## IMPORTANT: add option "-m64" for 64-bit cpu' ...
CPPFLAGS= -pthread -I. -I${HOME}/include `geant4-config --cflags` `root-config --cflags`
CXXFLAGS= -g -O2 -fPIC -fno-strict-aliasing -Wall -Wunused-parameter 
CXXFLAGS += `geant4-config --cflags` 
CXXFLAGS += `root-config --cflags`
CXXFLAGS += -I${HOME}/include

LDFLAGS += `root-config --libs` 
LDFLAGS += `geant4-config --libs` 
LDFLAGS +=-L${LIB_DIR} # -lEdmCommon
#LDFLAGS = 



.PHONY: all lib include clean

all:	include lib
	@echo -e "\033[31m"   lib${LIB_NAME}.so ... ok "\033[0m"

lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) 
	@echo "**** Linking shared library $@ ..."
	@echo "**** LDFLAGS = ${LDFLAGS}"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

%.o: %.cc
	@if [ -f $< ] ; then echo "Compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

#-- %.so: %.cc
#--	$(CXX) -fPIC -O2 -shared $(CPPFLAGS) -o $@

$(HOME)/include/%.hh: %.hh
	@echo "Copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

clean:
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@-rm $(LIBNAME)

# e-o-f
