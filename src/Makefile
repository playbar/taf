
TOPDIR	:= ..

include ${TOPDIR}/make.rules

DIRS = libutil libparse jce2cpp libhessian ${TOPDIR}/jce libservant liblog libpatch libconfig libnotify libnodeF  
DIRS_LIB = libutil libparse jce2cpp  libhessian  ${TOPDIR}/jce libservant liblog libpatch libconfig libnotify libnodeF

all:
	@for dir in $(DIRS); do make -C $$dir; echo; done

clean:
	@for dir in $(DIRS); do make -C $$dir clean; echo; done

cleanall:
	@for dir in $(DIRS); do make -C $$dir cleanall; echo; done

install:
	@echo "do nothing"

uninstall:
	@echo "do nothing"

libonly:
	@for dir in $(DIRS_LIB); do make -C $$dir; echo; done

