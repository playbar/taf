TOPDIR	:= ../..
include ${TOPDIR}/make.rules

#-------------------------------------------------------------------------------

YACC_Y	:= jce.y
LEX_L	:= jce.l

YACC_H 	:= jce.tab.hpp
YACC_C 	:= jce.tab.cpp
YACC_O	:= jce.tab.o
LEX_C	:= lex.yy.cpp
LEX_O	:= lex.yy.o


#-------------------------------------------------------------------------------

INCLUDE 	+=
LIB	    	+= -lutil
LOCAL_OBJ	:= ${LEX_O} ${YACC_O} $(LOCAL_OBJ) 
CLEANFILE	+=${YACC_H} ${YACC_C} ${LEX_C}

TARGET		:=${PARSELIB}

all : ${YACC_H} ${LEX_C}

#-------------------------------------------------------------------------------

${YACC_H} ${YACC_C} : ${YACC_Y}
	rm -vf $(YACC_H) $(YACC_C) $(YACC_O)
	${YACC} $^ -o ${YACC_C}


${LEX_C} : ${LEX_L}
	rm -vf ${LEX_C} ${LEX_O}
	${LEX} $^

include ${TOPDIR}/inc.mk
#-------------------------------------------------------------------------------

install:
	    echo "do nothing"
		
#-------------------------------------------------------------------------------

