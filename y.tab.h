/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_OR = 258,
    T_AND = 259,
    T_CEQ = 260,
    T_CNE = 261,
    T_CLT = 262,
    T_CGT = 263,
    T_CLE = 264,
    T_CGE = 265,
    T_PLUS = 266,
    T_MINUS = 267,
    T_MUL = 268,
    T_DIV = 269,
    T_NOT = 270,
    T_PROGRAM = 271,
    T_VAR = 272,
    T_PROCEDURE = 273,
    T_FUNCTION = 274,
    T_BEGIN = 275,
    T_END = 276,
    T_IF = 277,
    T_THEN = 278,
    T_ELSE = 279,
    T_WHILE = 280,
    T_FOR = 281,
    T_TO = 282,
    T_DO = 283,
    T_ASSIGNMENT = 284,
    T_OBRACKET = 285,
    T_CBRACKET = 286,
    T_SEMICOLON = 287,
    T_COLON = 288,
    T_COMMA = 289,
    T_DOT = 290,
    T_PRINT_INT = 291,
    T_PRINT_CHAR = 292,
    T_PRINT_BOOL = 293,
    T_PRINT_LINE = 294,
    TYPE_IDENTIFIER = 295,
    IDENTIFIER = 296,
    T_INT = 297,
    T_BOOL = 298,
    T_CHAR = 299
  };
#endif
/* Tokens.  */
#define T_OR 258
#define T_AND 259
#define T_CEQ 260
#define T_CNE 261
#define T_CLT 262
#define T_CGT 263
#define T_CLE 264
#define T_CGE 265
#define T_PLUS 266
#define T_MINUS 267
#define T_MUL 268
#define T_DIV 269
#define T_NOT 270
#define T_PROGRAM 271
#define T_VAR 272
#define T_PROCEDURE 273
#define T_FUNCTION 274
#define T_BEGIN 275
#define T_END 276
#define T_IF 277
#define T_THEN 278
#define T_ELSE 279
#define T_WHILE 280
#define T_FOR 281
#define T_TO 282
#define T_DO 283
#define T_ASSIGNMENT 284
#define T_OBRACKET 285
#define T_CBRACKET 286
#define T_SEMICOLON 287
#define T_COLON 288
#define T_COMMA 289
#define T_DOT 290
#define T_PRINT_INT 291
#define T_PRINT_CHAR 292
#define T_PRINT_BOOL 293
#define T_PRINT_LINE 294
#define TYPE_IDENTIFIER 295
#define IDENTIFIER 296
#define T_INT 297
#define T_BOOL 298
#define T_CHAR 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 34 "parser.y" /* yacc.c:1909  */

    char* opiden;
    int integer;
    int boolean;
    char character;
    int type;
    struct Node *node;

#line 151 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
