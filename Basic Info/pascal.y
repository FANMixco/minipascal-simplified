%{
/*
* pascal.y
*
* Pascal grammar in Yacc format, based originally on BNF given
* in "Standard Pascal -- User Reference Manual", by Doug Cooper.
* This in turn is the BNF given by the ANSI and ISO Pascal standards,
* and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
* The grammar has been massaged somewhat to make it LALR, and added
* the following extensions.
*
* constant expressions
* otherwise statement in a case
* productions to correctly match else's with if's
* beginnings of a separate compilation facility
*/

#include<stdio.h>

%}

%token T_IDENTIFIER T_REAL T_INTEGER T_PROGRAM T_VAR T_PROCEDURE T_FUNCTION T_BEGIN T_END T_WRITE T_READ T_DBL T_INT T_INPUT T_OUTPUT
%token T_ASSIGNMENT T_CEQ T_CNE T_CLT T_CLE T_CGT T_CGE
%token T_OPAREN T_CPAREN T_COMMA T_DOT T_DOUBLEDOT T_COLON T_SEMICOLON
%token T_PLUS T_MINUS T_MUL T_DIV T_AND T_OR T_NOT T_MOD
%token T_OF T_OSPAREN T_CSPAREN T_ARRAY
%token T_WHILE T_DO
%token T_IF T_THEN T_ELSE

%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : T_PROGRAM id T_OPAREN T_INPUT T_COMMA T_OUTPUT T_CPAREN T_SEMICOLON
		declarations
		subprogram_declarations
		compound_statement
		T_DOT
		;

identifier_list : id
		| identifier_list T_COMMA id
		;
		
declarations : declarations T_VAR identifier_list T_COLON type T_SEMICOLON
		|
		;

type : standard_type
		| T_ARRAY T_OSPAREN T_INT T_DOUBLEDOT T_INT T_CSPAREN T_OF standard_type
		;

standard_type : T_INTEGER
		| T_REAL
		;

subprogram_declarations : subprogram_declarations subprogram_declaration T_SEMICOLON
		|
		;

subprogram_declaration : subprogram_head declarations compound_statement ;

subprogram_head : T_FUNCTION id arguments T_COLON standard_type T_SEMICOLON
		| T_PROCEDURE id arguments T_SEMICOLON
		;

arguments : T_OPAREN parameter_list T_CPAREN
		|
		;

parameter_list : identifier_list T_COLON type
		| parameter_list T_SEMICOLON identifier_list T_COLON type
		;

compound_statement :
		T_BEGIN
		optional_statements
		T_END
		;

optional_statements : statement_list
		|
		;

statement_list : statement
		| statement_list T_SEMICOLON statement
		;

statement : variable T_ASSIGNMENT expression
		| procedure_statement
		| compound_statement
		| T_IF expression T_THEN statement T_ELSE statement
		| T_WHILE expression T_DO statement
		| T_READ T_OPAREN id T_CPAREN
		| T_WRITE T_OPAREN id T_CPAREN
		;

variable : id
		| id T_OSPAREN expression T_CSPAREN
		;

procedure_statement : id
		| id T_OPAREN expression_list T_CPAREN
		;

expression_list : expression
		| expression_list T_COMMA expression
		;

expression : simple_expression
		| simple_expression relop simple_expression
		;
		
relop : T_CEQ
        | T_CNE
        | T_CLT
        | T_CLE
        | T_CGT
        | T_CGE
        ;

simple_expression : term
		| sign term
		| simple_expression sign term
		| simple_expression T_OR term
		;

sign : T_PLUS
		| T_MINUS
		;
		
term : factor
		| term mulop factor
		;
		
mulop : T_MUL
		| T_DIV
		;

factor : variable
		| id T_OPAREN expression_list T_CPAREN
		| num
		| T_OPAREN expression T_CPAREN
		| T_NOT factor
		;

num : T_INT
		| T_DBL
		;
	
id : T_IDENTIFIER ;

%%

extern int line_no;
extern char *yytext;

int yyerror(s)
char *s;
{
	fprintf(stderr, "%s: at or before '%s', line %d\n", s, yytext, line_no);
}

main (void) {
	return yyparse();
}
