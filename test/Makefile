
TOPDIR	:= ..

include ${TOPDIR}/make.rules

DIRS = util jce2cpp jce2java hessian hce2cpp server node client async_client log jmem TCServer TestServer TestClient stat property admin registry AServer

all:
	@for dir in $(DIRS); do make -C $$dir; echo; done

clean:
	@for dir in $(DIRS); do make -C $$dir clean; echo; done

cleanall:
	@for dir in $(DIRS); do make -C $$dir cleanall; echo; done

install:
	@for dir in $(DIRS); do make -C $$dir install; echo; done

uninstall:
	@for dir in $(DIRS); do make -C $$dir uninstall; echo; done
