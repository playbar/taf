
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
	if [ ! -d ${INSTALL_PATH}/include/notify ]; then mkdir -p ${INSTALL_PATH}/include/notify; fi
	cp -rf ${HFILES} ${INSTALL_PATH}/include/notify

uninstall:
	echo "do nothing"
