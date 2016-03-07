
TOPDIR	:= ../..

include ${TOPDIR}/make.rules

DIRS = src srcB client4c++

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

