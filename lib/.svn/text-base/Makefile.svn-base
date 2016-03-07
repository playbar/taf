
TOPDIR	:= ..

include ${TOPDIR}/make.rules

LIBFILES = $(wildcard *.a)
LIBTAF =  libnodeF.a libpatch.a libconfig.a libnotify.a liblog.a libparse.a libservant.a

all:
	if [ "$(PLATFORM)" == "x86_64" ]; then ln -sfT ./lib64 ${TOPDIR}/lib; fi
	echo "do nothing"

clean:
	rm -rf ${LIBFILES} 

cleanall:
	rm -rf ${LIBFILES} 

install:
	ls $(LIBTAF) | xargs -n1 ar -x ; rm -rf libtaf.a ; ar -q libtaf.a *.o; rm *.o ; \
	cp -rf libparse.a libhessian.a libutil.a libtaf.a ${INSTALL_PATH}/lib64
	echo "do nothing"

uninstall:
	echo "do nothing"

arlib:
	ls $(LIBTAF) | xargs -n1 ar -x ; rm -rf libtaf.a ; ar -q libtaf.a *.o; rm *.o
 
