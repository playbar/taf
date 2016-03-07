
TOPDIR := ../..
include ${TOPDIR}/make.rules
#-------------------------------------------------------------------------------

INCLUDE +=
LIB     += -lutil -lz /usr/local/mysql/lib/mysql/libmysqlclient.a

#-------------------------------------------------------------------------------
TARGETS = $(shell ls *.cpp |cut  -d "." -f 1)
#TARGETS = test_tc_common test_tc_config test_tc_log test_tc_socket test_tc_http test_tc_option test_tc_file test_tc_file_lock test_tc_mem_queue test_tc_mmap test_tc_hashmap test_tc_bitmap test_tc_cgi test_tc_mysql test_tc_thread_lock test_tc_thread_queue test_tc_functor test_tc_thread_pool test_tc_mem_chunk test_tc_des test_tc_encoder test_tc_pack test_tc_singleton test_tc_loki test_tc_thread test_tc_autoptr 

include $(TOPDIR)/inc.mk

#-------------------------------------

install:
	mkdir -p ${INSTALL_PATH}/test/util/
	cp -rf *.cpp ${INSTALL_PATH}/test/util/

