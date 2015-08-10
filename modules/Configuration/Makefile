# Makefile
# 29-Jan-2015 G.Macharashvili Dubna

LIB_DIR  = ${HOME}/lib
LIB_NAME = Configuration

MODULES = ConfigFileReader
MODULES += VolumeGenerator


CPPFLAGS+= -pthread -I.  `geant4-config --cflags` `root-config --cflags` 
CXXFLAGS+= -g -O2 -fPIC -fno-strict-aliasing -Wall `geant4-config --cflags` `root-config --cflags` 
LDFLAGS += `clhep-config --libs` `geant4-config --libs`

LIBNAME  = $(LIB_DIR)/lib$(LIB_NAME).so

.PHONY: all lib include clean 

all:	include lib 
	@echo -e "\033[31m"   lib${LIB_NAME}.so ... ok "\033[0m"

lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) 
	@echo "Linking shared library $@ ..."
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

clean: 
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@-rm $(LIBNAME)

%.o: %.cc
	@if [ -f $< ] ; then echo "Compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

%.so: %.cc
	$(CXX) -dynamiclib -fPIC -O2 -shared $(CPPFLAGS) -o $@

$(HOME)/include/%.hh: %.hh
	@echo "Copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

# eof
