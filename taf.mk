#----------------------------------------------------------------------------------

HCE2CPP      = /usr/local/taf/bin/hce2cpp
JCE2CPP      = /usr/local/taf/bin/jce2cpp

#----------------------------------------------------------------------------------

INCLUDE     += -I/usr/local/taf/include
LIB_DIR     += -L/usr/local/taf/lib
LIB         += -lhessian -lutil

#----------------------------------------------------------------------------------

JCE_SRC     := $(wildcard *.jce)
JCE_H       := $(patsubst %.jce,%.h, $(JCE_SRC))
JCE_INTER   := $(if $(JCE_SRC), $(shell grep -ls interface $(JCE_SRC)))
JCE_CPP     := $(patsubst %.jce,%.cpp, $(JCE_INTER))
JCE_OBJ     := $(patsubst %.jce,%.o, $(JCE_INTER))

HCE_SRC     := $(wildcard *.hce)
HCE_H       := $(patsubst %.hce,%.h, $(HCE_SRC))
HCE_CPP     := $(patsubst %.hce,%.cpp, $(HCE_SRC))
HCE_OBJ     := $(patsubst %.hce,%.o, $(HCE_SRC))

#----------------------------------------------------------------------------------

ifneq ($(JCE_SRC),)
$(JCE_H) $(JCE_CPP) : $(JCE_SRC)
	rm -vf $(JCE_H) $(JCE_CPP) $(JCE_OBJ)
	$(JCE2CPP) $(JCE_SRC)
endif
#----------------------------------------------------------------------------------

ifneq ($(HCE_SRC),)
$(HCE_H) $(HCE_CPP) : $(HCE_SRC)
	rm -vf $(HCE_H) $(HCE_CPP) $(HCE_OBJ)
	$(HCE2CPP) $(HCE_SRC)
endif

#----------------------------------------------------------------------------------

one:
	$(JCE2CPP) $(JCE_SRC)
	$(HCE2CPP) $(HCE_SRC)

oneclean:
	rm -vf $(JCE_CPP) $(JCE_H) $(HCE_CPP) $(HCE_H)
