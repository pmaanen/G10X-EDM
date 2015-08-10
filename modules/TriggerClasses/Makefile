# Date:		01-Oct-2015 G.Macharashvili Dubna
# anke geant4 trigger classes library generation ...

MODULES = VirtualTrigger
MODULES+= Trigger0
#MODULES+= Trigger4

LIB_DIR  = ${HOME}/lib
LIB_NAME  = TriggerClasses

## IMPORTANT: add option "-m64" for 64-bit cpu' ...
CPPFLAGS= -pthread -I. `geant4-config --cflags` `root-config --cflags`
CXXFLAGS= -g -O2 -fPIC -fno-strict-aliasing -Wall -Wunused-parameter `geant4-config --cflags` `root-config --cflags`
LDFLAGS = `clhep-config --libs`

LIBNAME  = $(LIB_DIR)/lib$(LIB_NAME).so

.PHONY: all lib include clean

all:	include lib
	@echo -e "\033[31m"   lib${LIB_NAME}.so ... ok "\033[0m"

lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) 
	@echo "Linking shared library $@ ..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

clean:
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@-rm $(LIBNAME)

%.o: %.cc
	@if [ -f $< ] ; then echo "Compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

%.so: %.cc
	$(CXX) -fPIC -O2 -shared $(CPPFLAGS) -o $@

$(HOME)/include/%.hh: %.hh
	@echo "Copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

# e-o-f
