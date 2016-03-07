
TOPDIR	:= .

include ${TOPDIR}/make.rules

ifeq ($(PLATFORM),x86_64)
	lib = lib64 lib32
else
	lib = lib32
endif

DIRS            = src $(lib) 
DIRS_CLEAN      = src bin test $(lib) script
DIRS_INSTALL    = conf bin include doc $(lib) java

default:
	@echo "make libonly: only make libs, libutil.a libtaf.a libhessian.a libparse.a"
	@echo "make all: make all the libs and taf exe, such as tafregistry, tafnode, tafpatch,..."
	@echo "make clean: clean the temp files"
	@echo "make cleanall: clean all the temp files and .d .log files"
	@echo "make install: install to the "${INSTALL_PATH}
	@echo "make uninstall: uninstall taf"

libonly:
	@for dir in $(DIRS); do make -C $$dir libonly; echo; done

all:
	@for dir in $(DIRS); do make -C $$dir; echo; done

clean:
	@for dir in $(DIRS_CLEAN); do make -C $$dir clean; echo; done

cleanall:
	@for dir in $(DIRS_CLEAN); do make -C $$dir cleanall; echo; done

install:
	if [ ! -d ${INSTALL_PATH} ]; then mkdir -p ${INSTALL_PATH}; fi
	if [ ! -d ${INSTALL_PATH}/include ]; then mkdir -p ${INSTALL_PATH}/include/util; fi
	if [ ! -d ${INSTALL_PATH}/bin ]; then mkdir -p ${INSTALL_PATH}/bin; fi
	if [ ! -d ${INSTALL_PATH}/conf ]; then mkdir -p ${INSTALL_PATH}/conf; fi
	if [ ! -d ${INSTALL_PATH}/doc ]; then mkdir -p ${INSTALL_PATH}/doc; fi
	if [ ! -d ${INSTALL_PATH}/lib32 ]; then mkdir -p ${INSTALL_PATH}/lib32; fi
	if [ ! -d ${INSTALL_PATH}/lib64 ] && [ "$(PLATFORM)" == "x86_64"  ]; then mkdir -p ${INSTALL_PATH}/lib64; fi; 
	if [ "$(PLATFORM)" == "x86_64" ]; then ln -sfT ${INSTALL_PATH}/lib64 ${INSTALL_PATH}/lib; fi
	if [ "$(PLATFORM)" != "x86_64" ]; then ln -sfT ${INSTALL_PATH}/lib32 ${INSTALL_PATH}/lib; fi
	@for dir in $(DIRS_INSTALL); do make -C $$dir install; echo; done
	@echo 'TAF_PATH    :=/usr/local/taf-version/taf-'${TAF_VERSION} > makefile.taf.tmp
	@cat makefile.taf >> makefile.taf.tmp
	cp -rf makefile.taf.tmp ${INSTALL_PATH}/makefile.taf
	cp -rf make.rules.lp64 ${INSTALL_PATH}/
	cp -rf taf.mk ${INSTALL_PATH}/
	cp -rf script/create_taf_server.sh ${INSTALL_PATH}/
	cp -rf script/demo ${INSTALL_PATH}/
	if [ ! -d ${INSTALL_PATH}/taf_server_java ]; then mkdir -p ${INSTALL_PATH}/taf_server_java; fi
	@for dir in $(DIRS_INSTALL); do make -C $$dir install; echo; done
	cp -rf java/tafservercreate/* ${INSTALL_PATH}/taf_server_java/
	ln -sfT  ${INSTALL_PATH} ${LINK_PATH}
	if [  -d /home/tafjce ]; then cp -rf script/release /home/tafjce; fi
	if [  -d /home/tafjce/release ]; then /home/tafjce/release/clean_obj.sh; fi
	if [  -d /home/tafjce/release ]; then /home/tafjce/release/release_all.sh; fi

uninstall:
	rm -rf ${INSTALL_PATH}
	@for dir in $(DIRS_INSTALL); do make -C $$dir uninstall; echo; done


