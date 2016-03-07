
TOPDIR	:= ..

include ${TOPDIR}/make.rules

BINFILES = hce2cpp jce2c jce2cpp jce2java jce2kjava jce2symcpp tafpatch tafnode tafregistry taflog tafpatchclient tafadminclient tafstat tafproperty tafconfig tafnotify jutil-1.0.2.jar

all:
	@echo "do all nothing"

clean:
	@echo "do clean nothing"

cleanall:
	@echo "do cleanall nothing"

install:
	echo "java -classpath .:${INSTALL_PATH}/bin Hessian2hce \$$*" > hessian2hce.sh
	cp hessian2hce.sh ${INSTALL_PATH}/bin/hessian2hce.sh
	cp -rf ${BINFILES} ${INSTALL_PATH}/bin

uninstall:
	@echo "do uninstall nothing"

touch:
	@echo "do touch nothing"
