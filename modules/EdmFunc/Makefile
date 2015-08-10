# File:		Makefile
# Date:		21-Apr-2015 G.Macharashvili Dubna

LIB_DIR  = ${HOME}/lib
LIB_NAME = EdmFunc

MODULES = EdmFunctionalAy270 

CPPFLAGS= -pthread -I. -I${HOME}/include 

CXXFLAGS= -g -O2 -fPIC -fno-strict-aliasing -Wall `root-config --cflags` 
LDFLAGS = `root-config --libs`

LIBNAME  = $(LIB_DIR)/lib$(LIB_NAME).so

ExtraCPPFLAGS = 
ExtraLDFLAGS = 

%.o: %.cc
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%: %.o
	@$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: all lib include clean 

all:	include lib 
	@echo -e "\033[31m lib${LIB_NAME}.so ... ok \033[0m"
	
EdmFunctional_main: EdmFunctional_main.o EdmFunctionalAy270.o
	@$(CXX) $(CXXFLAGS) ${LDFLAGS} $^ -o $@

lib:	$(LIBNAME)
$(LIBNAME): $(addsuffix .o,$(MODULES)) 
	@echo "Linking shared library $@ ..."
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

include: $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES))) 

clean:
	@-rm $(LIBNAME)
	@-rm $(addprefix $(HOME)/include/,$(addsuffix .hh,$(MODULES)))
	@-rm *.o *\~
	@if [ -f EdmFunctional_main ] ; then rm EdmFunctional_main ; fi ;

#%.o: %.cc
#	@if [ -f $< ] ; then echo "Compiling $< ..." ; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ ; fi ;

%.so: %.cc
	$(CXX) -fPIC -O2 -shared $(CPPFLAGS) -o $@

$(HOME)/include/%.hh: %.hh
	@echo "Copying $< to $(HOME)/include ..."
	@-cp $< $(HOME)/include/$(notdir $@)

# eof
