
TOPDIR	:= ../..

include ${TOPDIR}/make.rules

HFILES = $(wildcard *.h)

all:
	echo "do nothing"

clean:
	echo "do nothing"

cleanall:
	echo "do nothing"

install:
	if [ ! -d ${INSTALL_PATH}/include/jce ]; then mkdir -p ${INSTALL_PATH}/include/jce; fi
	cp -rf ${HFILES} ${INSTALL_PATH}/include/jce

uninstall:
	echo "do nothing"

patch:
	tar -zcvf wup-linux-c++.tgz JceDisplayer.h Jce.h  JceType.h RequestF.h wup.h
	tar -zcvf wup-symbian-c++.tgz RequestF_sym.h Jce_sym.h wup_sym.h
	tar -zcvf wup-c.tgz Jce_c.h wup_c.h Jce_c.c wup_c.c 
