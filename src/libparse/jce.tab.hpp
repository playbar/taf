/* A Bison parser, made by GNU Bison 2.5.1.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     JCE_VOID = 258,
     JCE_STRUCT = 259,
     JCE_BOOL = 260,
     JCE_BYTE = 261,
     JCE_SHORT = 262,
     JCE_INT = 263,
     JCE_DOUBLE = 264,
     JCE_FLOAT = 265,
     JCE_LONG = 266,
     JCE_STRING = 267,
     JCE_VECTOR = 268,
     JCE_MAP = 269,
     JCE_NAMESPACE = 270,
     JCE_INTERFACE = 271,
     JCE_IDENTIFIER = 272,
     JCE_OUT = 273,
     JCE_OP = 274,
     JCE_KEY = 275,
     JCE_ROUTE_KEY = 276,
     JCE_REQUIRE = 277,
     JCE_OPTIONAL = 278,
     JCE_INTEGER_LITERAL = 279,
     JCE_FLOATING_POINT_LITERAL = 280,
     JCE_FALSE = 281,
     JCE_TRUE = 282,
     JCE_STRING_LITERAL = 283,
     JCE_SCOPE_DELIMITER = 284,
     JCE_CONST = 285,
     JCE_ENUM = 286,
     JCE_UNSIGNED = 287,
     BAD_CHAR = 288
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


