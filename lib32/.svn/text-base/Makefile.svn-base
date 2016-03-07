
TOPDIR	:= ..

include ${TOPDIR}/make.rules

LIBFILES = $(wildcard *.a)
LIBTAF = libnodeF.a libpatch.a libconfig.a libnotify.a liblog.a libparse.a libservant.a

all:
	if [ "$(PLATFORM)" != "x86_64" ]; then ln -sfT ./lib32 ${TOPDIR}/lib; fi
	echo "do nothing"

clean:
	rm -rf libtaf.a ${LIBFILES}

cleanall:
	rm -rf libtaf.a ${LIBFILES}

install:
	ls $(LIBTAF) | xargs -n1 ar -x ; rm -rf libtaf.a ; ar -q libtaf.a *.o; rm *.o ; \
	cp -rf libparse.a libhessian.a libutil.a libtaf.a ${INSTALL_PATH}/lib32

uninstall:
	echo "do nothing"

arlib:
	ls $(LIBTAF) | xargs -n1 ar -x ; rm -rf libtaf.a ; ar -q libtaf.a *.o; rm *.o
 
