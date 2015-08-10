# Date:		19-Jun-2015 G.Macharashvili Juelich
# Edm common classes ...

MODULES+= UDynamicManager
MODULES+= UArrow
MODULES+= Ruller

LIB_NAME = EdmCommon
LIB_DIR  = ${HOME}/lib


CPPFLAGS+= -Wall -Wunused-parameter -pthread -I. -I${HOME}/include `geant4-config --cflags` `root-config --cflags` 
CXXFLAGS+= -g -O2 -fPIC -fno-strict-aliasing -Wall -Wunused-parameter

LDFLAGS += -L${LIB_DIR} -L/usr/local/lib `clhep-config --libs` `geant4-config --libs` `root-config --libs` 

LIBNAME  = $(LIB_DIR)/lib$(LIB_NAME).so

.PHONY: all lib include clean 

all:	include lib 
	@echo -e "\033[31m"   lib${LIB_NAME}.so ... ok "\033[0m"
	
lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) 
	@echo "Linking shared library $@ ..."
	@$(CXX) ${LDFLAGS} -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

clean: 
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@-rm $(LIBNAME)

%.o: %.cc
	@if [ -f $< ] ; then echo "Compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

$(HOME)/include/%.hh: %.hh
	@echo "Copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

# e-o-f
