
TOPDIR	:= ..

include ${TOPDIR}/make.rules

DIRS = servant jce log util hessian patch config notify nodeF jmem parse

all:
	@for dir in $(DIRS); do make -C $$dir; echo; done

clean:
	@for dir in $(DIRS_CLEAN); do make -C $$dir clean; echo; done

cleanall:
	@for dir in $(DIRS_CLEAN); do make -C $$dir cleanall; echo; done

install:
	@for dir in $(DIRS); do make -C $$dir install; echo; done

uninstall:
	@for dir in $(DIRS); do make -C $$dir uninstall; echo; done


