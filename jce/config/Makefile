
TOPDIR	:= ../..
include ${TOPDIR}/make.rules

#------------------------------------------------

all : $(JCE_H) $(JCE_CPP)

ifneq ($(JCE_SRC),)
$(JCE_H) $(JCE_CPP) : $(JCE_SRC)
	rm -vf $(JCE_H) $(JCE_CPP)
	$(JCE2CPP) --with-taf $(JCE_SRC)
	cp -rf ${JCE_H} ${CONFIG_H}
	cp -rf ${JCE_CPP} ${CONFIG_C}
endif

#------------------------------------------------

clean:
	rm -vf $(JCE_H) $(JCE_CPP)

cleanall:
	rm -vf $(JCE_H) $(JCE_CPP)

install:
	echo "do nothing"
