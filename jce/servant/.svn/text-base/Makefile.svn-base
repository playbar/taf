
TOPDIR	:= ../..
include ${TOPDIR}/make.rules

#------------------------------------------------

all : $(JCE_H) $(JCE_CPP)

ifneq ($(JCE_SRC),)
$(JCE_H) $(JCE_CPP) : $(JCE_SRC)
	rm -vf $(JCE_H) $(JCE_CPP)
	$(JCE2CPP) $(JCE_SRC) --header=servant --with-taf 
	cp -rf ${JCE_H} ${SERVANT_H}
	cp -rf ${JCE_CPP} ${SERVANT_C}
endif

#------------------------------------------------

clean:
	rm -vf $(JCE_H) $(JCE_CPP)

cleanall:
	rm -vf $(JCE_H) $(JCE_CPP)

install:
	echo "do nothing"
