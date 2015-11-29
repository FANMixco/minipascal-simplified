%{
/*
 * pascal.l
 *
 * lex input file for pascal scanner
 *
 * extensions: to ways to spell "external" and "->" ok for "^".
 */

#include <stdio.h>
#include "pascal.tab.h"
int line_no = 1;

%}

%%

":="                    return(T_ASSIGNMENT);
"="	                    return(T_CEQ);
"<>"                    return(T_CNE);
"<"                     return(T_CLT);
"<="                    return(T_CLE);
">"                     return(T_CGT);
">="                    return(T_CGE);
"("                     return(T_OPAREN);
")"                     return(T_CPAREN);
"["                     return(T_OSPAREN);
"]"                     return(T_CSPAREN);
"."                     return(T_DOT);
".."                    return(T_DOUBLEDOT);
","                     return(T_COMMA);
"+"                     return(T_PLUS);
"-"                     return(T_MINUS);
"*"                     return(T_MUL);
"/"                     return(T_DIV);
"mod"                   return(T_MOD);
"and"                   return(T_AND);
"or"                    return(T_OR);
"array"                 return(T_ARRAY);
"of"                    return(T_OF);
"not"                   return(T_NOT);
"input"                 return(T_INPUT);
"output"                return(T_OUTPUT);
";"                     return(T_SEMICOLON);
":"                     return(T_COLON);
"if"                    return(T_IF);
"then"                  return(T_THEN);
"else"                  return(T_ELSE);
"while"                 return(T_WHILE);
"do"                    return(T_DO);
"program"               return(T_PROGRAM);
"procedure"				      return(T_PROCEDURE);
"function"				      return(T_FUNCTION);
"end"					          return(T_END);
"begin"					        return(T_BEGIN);
"var"					          return(T_VAR);
"integer"				        return(T_INTEGER);
"real"					        return(T_REAL);
"readln"				        return(T_READ);
"writeln"				        return(T_WRITE);
[a-zA-Z_][a-zA-Z0-9_]*  return(T_IDENTIFIER);
[0-9]+\.[0-9]*          return(T_DBL);
[0-9]+                  return(T_INT);
\n                      line_no++;
[\t\f " "]              ;
.                               { fprintf (stderr,
                                    "'%c' (0%o): illegal character at line %d\n",
                                    yytext[0], yytext[0], line_no);
                                }
%%

yywrap ()
{
  return (1);
}
