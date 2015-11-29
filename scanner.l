%{
#include <stdio.h>
#include "Structures.h"
#include "parser.h"

#define SAVE_TOKEN_LOC   { yylloc->first_column = yylloc->last_column + 1; yylloc->last_column += yyleng; }

#define CAST_BOOLEAN(strb)  strcasecmp(strb, "False") ? TRUE : FALSE
%}

%option yylineno
%option bison-bridge
%option bison-locations
%option noyywrap
%option nounput

%%
"program"                   { SAVE_TOKEN_LOC; return T_PROGRAM; }
"var"                       { SAVE_TOKEN_LOC; return T_VAR; }
"integer"|"boolean"|"char"  { SAVE_TOKEN_LOC; yylval->type = Get_Type_From_OpIden(yytext); return T_STANDARD_TYPE; }
"procedure"                 { SAVE_TOKEN_LOC; return T_PROCEDURE; }
"function"                  { SAVE_TOKEN_LOC; return T_FUNCTION; }
"begin"                     { SAVE_TOKEN_LOC; return T_BEGIN; }
"end"                       { SAVE_TOKEN_LOC; return T_END; }
"writeInt"                  { SAVE_TOKEN_LOC; return T_WRITE_INT; }
"writeBool"                 { SAVE_TOKEN_LOC; return T_WRITE_BOOL; }
"writeChar"                 { SAVE_TOKEN_LOC; return T_WRITE_CHAR; }
"writeLn"                   { SAVE_TOKEN_LOC; return T_WRITE_LN; }
"if"                        { SAVE_TOKEN_LOC; return T_IF; }
"then"                      { SAVE_TOKEN_LOC; return T_THEN; }
"else"                      { SAVE_TOKEN_LOC; return T_ELSE; }
"while"                     { SAVE_TOKEN_LOC; return T_WHILE; }
"for"                       { SAVE_TOKEN_LOC; return T_FOR; }
"to"                        { SAVE_TOKEN_LOC; return T_TO; }
"do"                        { SAVE_TOKEN_LOC; return T_DO; }
":="                        { SAVE_TOKEN_LOC; return T_ASSIGNOP; }
"("                         { SAVE_TOKEN_LOC; return T_OBRACKET; }
")"                         { SAVE_TOKEN_LOC; return T_CBRACKET; }
"+"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_PLUS; }
"-"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_MINUS; }
"*"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_MUL; }
"/"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_DIV; }
"mod"                       { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_MOD; }
"and"                       { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_AND; }
"or"                        { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_OR; }
"not"                       { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_NOT; }
">"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CGT; }
"<"                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CLT; }
"="                         { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CEQ; }
"<>"                        { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CNE; }
">="                        { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CGE; }
"<="                        { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_CLE; }
";"                         { SAVE_TOKEN_LOC; return T_SEMICOLON; }
":"                         { SAVE_TOKEN_LOC; return T_COLON; }
"."                         { SAVE_TOKEN_LOC; return T_DOT; }
","                         { SAVE_TOKEN_LOC; return T_COMMA; }
"true"|"false"              { SAVE_TOKEN_LOC; yylval->boolean=CAST_BOOLEAN(yytext); return T_BOOL; }
"'"\\n"'"                   { SAVE_TOKEN_LOC; yylval->character='\n'; return T_CHAR; }
"'"."'"                     { SAVE_TOKEN_LOC; yylval->character=yytext[1]; return T_CHAR; }
[0-9]+                      { SAVE_TOKEN_LOC; yylval->integer=atoi(yytext); return T_INT; }
[0-9]+\.[0-9]+              { SAVE_TOKEN_LOC; yylval->real=atoi(yytext); return T_REAL; }
[A-Za-z][A-Za-z0-9_]*       { SAVE_TOKEN_LOC; yylval->opiden=strdup(yytext); return T_IDENTIFIER; }
[ \t]+                      /* eat all whitespaces */;
[\n]                        { yylloc->first_line = yylloc->last_line = yylineno; yylloc->first_column = 1; yylloc->last_column = 0; }
.                           { SAVE_TOKEN_LOC; fprintf (stderr, "Syntax Error: unexpected character '%s' at line %d\n", yytext, yylloc->first_line); }

%%
