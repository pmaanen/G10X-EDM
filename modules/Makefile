# Date:		28-Jan-2015 G.Macharashvili Dubna

# the sequence matters ...
LIBDIRS += Configuration
LIBDIRS += VertexGenerators 
LIBDIRS += InteractionGenerators
LIBDIRS += TriggerClasses
LIBDIRS += EdmCommon
LIBDIRS += EdmFunc
LIBDIRS += EdmGeantTEvent

.PHONY: all lib clean

all: lib
	@echo 
	@echo -e "\033[31m"... libraries generated ..."\033[0m"
	@echo -e "\033[31m"... G4INSTALL = ${G4INSTALL} ..."\033[0m"
	@echo -e "\033[31m"... contains: `ls -l ${G4INSTALL} | awk '{ print $$9 }'` ..."\033[0m"
	@echo 
	
lib:
	@if test "x${G4INCLUDE}" == "x" ; then echo -e "\n\033[31m" ... global variable G4INCLUDE not exists ... fatal"\033[0m\n" ; exit 2 ; else echo "  " ; fi

	@(test -e $(INC) || mkdir $(INC))	 
	@(test -e $(LIB) || mkdir $(LIB))
	@for dir in $(LIBDIRS); do (cd $$dir ; $(MAKE) ; echo " " ) ; done

clean:
	@for dir in $(LIBDIRS); do (cd $$dir; $(MAKE) clean); done

## eof
