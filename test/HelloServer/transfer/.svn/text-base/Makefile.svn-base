
TOPDIR  := ../../..
include ${TOPDIR}/make.rules

#-------------------------------------------------------------------------------

INCLUDE   += -I../src/
LIB       += ${TAFLIB}
LOCAL_OBJ += ../src/Hello.o
#-------------------------------------------------------------------------------

TARGET  := HelloTransfer
CONFIG  :=
STRIP_FLAG := Y

#release : $(TARGET)
#	@strip $(TARGET)
#	@sz $(TARGET)

#-------------------------------------------------------------------------------

include ${TOPDIR}/inc.mk
