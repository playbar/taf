
TOPDIR  := ../..
include ${TOPDIR}/make.rules

#-------------------------------------------------------------------------------

INCLUDE   += -I../../include/servant
LIB       += ${TAFLIB}

READLINE_FLAG := 0

ifeq ($(READLINE_FLAG),1)
    LIB     += -lreadline
    CFLAGS  += -DHAVE_READLINE
endif

YACC_Y  := Grammar.y
LEX_L   := Scanner.l

YACC_H  := Grammar.hpp
YACC_C  := Grammar.cpp
YACC_O  := Grammar.o
LEX_C   := Scanner.cpp
LEX_O   := Scanner.o

CFLAGS  += -DYYDEBUG

#-------------------------------------------------------------------------------

TARGET  	:= $(ADMINCLIENT)
LOCAL_OBJ 	+= ${LEX_O} ${YACC_O}

#-------------------------------------------------------------------------------

all : ${YACC_H} ${LEX_C} ${TARGET}

${YACC_H} ${YACC_C} : ${YACC_Y}
	rm -vf $(YACC_H) $(YACC_C) $(YACC_O); \
	${YACC} $^ -o ${YACC_C} -d

${LEX_C} : ${LEX_L}
	rm -vf ${LEX_C} ${LEX_O}; \
	${LEX} -o$@ $^

CLEANFILE +=  Grammar.cpp  Grammar.hpp Scanner.cpp

include ${TOPDIR}/jce/registry/Registry.mk
include ${TOPDIR}/jce/node/Node.mk
include ${TOPDIR}/inc.mk

