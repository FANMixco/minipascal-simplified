%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Structures.h"
#include "parser.h"
#include "Code_Gen.h"

extern FILE *yyin;

static void yyerror (const char *msg);

static Node *global_node;
%}

%locations
%pure-parser
%error-verbose

/*
This union define the basic variables that we're gonna represent such as opvars (keyword explained above), integer, real numbers, boolean results, types such as VOID, INTEGER, etc. (if we need more information about it, it's in the node.h file), Node the entire program (finally it looks like a kind of binary tree!).
*/
%union {
    char* opiden;
    int integer;
    int boolean;
    char character;
	double real;
    int type;
    struct Nodes *node;
}

//This collection of tokens are related to the keywords reserved by Pascal such as begin, end, etc. More information pascal.l file
%left <opiden> T_OR
%left <opiden> T_AND
%left <opiden> T_CEQ T_CNE
%left <opiden> T_CLT T_CGT T_CLE T_CGE
%left <opiden> T_PLUS T_MINUS
%left <opiden> T_MUL T_DIV T_MOD
%left <opiden> T_NOT

//This collection of tokens are related to the keywords reserved by Pascal such as begin, end, etc. More information pascal.l file
%token T_PROGRAM T_VAR T_PROCEDURE T_FUNCTION T_BEGIN T_END
%token T_IF T_THEN T_ELSE T_WHILE T_FOR T_TO T_DO
%token T_ASSIGNOP T_OBRACKET T_CBRACKET T_SEMICOLON T_COLON T_COMMA T_DOT
%token T_WRITE_INT T_WRITE_CHAR T_WRITE_BOOL T_WRITE_LN

//This tokens has special types because they are related to real information to use and they are not constants like IF or OR, they mean Regular Expressions
%token <type> T_STANDARD_TYPE
%token <opiden> T_IDENTIFIER
%token <integer> T_INT
%token <boolean> T_BOOL
%token <character> T_CHAR
%token <real> T_REAL

//This are the keywords that we're gonna use accross the grammar
%type <node> program program_decl declarations multi_var_decl var_decl list_id multi_id id
%type <node> subprogram_declarations multi_subprogram_decl subprogram_decl procedure_decl function_decl arg arg_list multi_arg
%type <node> compound_statement statements optional_statements multi_statement statement matched_statement unmatched_statement
%type <node> if matched_if while for
%type <node> write_statement write_char write_int write_bool write_ln
%type <node> expression simple_expression not_element element term
%type <node> call call_arg_list call_arg call_multi_arg
%type <node> assignop identifier standard_type
%type <node> add_op mul_op rel_op not_op

//This expression indicates to Bison where to start the translation
%start program

%%
program:
    program_decl declarations subprogram_declarations compound_statement
    {
        Node *temp;
        temp = New_Node("program", PROGRAM, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $1); // program_decl
        Add_Child(temp, $2); // declarations
        Add_Child(temp, $3); // subprogram_declarations
        Add_Child(temp, $4); // compound_statement
        $$ = temp;
        global_node = temp;
    }
    ;

program_decl:
    T_PROGRAM identifier T_SEMICOLON
    {
        Node *temp;
        temp = New_Node("program_decl", PROGRAM_DECL, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        $$ = temp;
    }
    ;

declarations:
    multi_var_decl
    {
        Node *temp;
        temp = New_Node("declarations", DECLARATIONS, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        $$ = temp;
    }
    | /* E */ { $$ = NULL; }
    ;

multi_var_decl:
    var_decl multi_var_decl
    {
        Add_Sibling($1, $2);
        $$ = $1;
    }
    | /* E */ { $$ = NULL; }
    ;

var_decl:
    T_VAR list_id T_COLON T_STANDARD_TYPE T_SEMICOLON
    {
        Node *temp;
        temp = New_Node("var_decl", VAR_DECL, $4, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        $$ = temp;
    }
    ;

list_id:
    id multi_id
    {
        Node *temp;
        temp = New_Node("list_id", LIST_ID, VOID, yylloc.last_line, NULL);
        Add_Sibling($1, $2);
        Add_Child(temp, $1);
        $$ = temp;
    }
    ;

multi_id:
    T_COMMA id multi_id
    {
        Add_Sibling($2, $3);
        $$ = $2;
    }
    | /* E */ { $$ = NULL; }
    ;

id:
    identifier { $$ = $1; }
    ;


subprogram_declarations:
	subprogram_decl multi_subprogram_decl
    {
        Node *temp;
        temp = New_Node("subprogram_declarations", SUBPROGRAM_DECLARATIONS, VOID, yylloc.last_line, NULL);
        Add_Sibling($1, $2);
        Add_Child(temp, $1);
        $$ = temp;
    }
    | /* E */ { $$ = NULL; }
    ;

multi_subprogram_decl:
    subprogram_decl multi_subprogram_decl
    {
        Add_Sibling($1, $2);
        $$ = $1;
    }
    | /* E */ { $$ = NULL; }
    ;

subprogram_decl:
    procedure_decl { $$ = $1; }
    | function_decl { $$ = $1; }
    ;

procedure_decl:
    T_PROCEDURE identifier T_OBRACKET arg_list T_CBRACKET T_SEMICOLON declarations
    T_BEGIN optional_statements T_END T_SEMICOLON
    {
        Node *temp;
        temp = New_Node("procedure_decl", PROCEDURE_DECL, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);  // identifier
        Add_Child(temp, $4);  // arg_list
        Add_Child(temp, $7);  // declarations
        Add_Child(temp, $9);  // statements
        // Symbol Table
        temp->symbol = New_Symbol(NULL);
        $$ = temp;
    }
    ;

function_decl:
    T_FUNCTION identifier T_OBRACKET arg_list T_CBRACKET T_COLON T_STANDARD_TYPE
    T_SEMICOLON declarations T_BEGIN optional_statements T_END T_SEMICOLON
    {
        Node *temp;
        temp = New_Node("function_decl", FUNCTION_DECL, $7, yylloc.last_line, NULL);
        Add_Child(temp, $2);  // identifier
        Add_Child(temp, $4);  // arg_list
        Add_Child(temp, $9);  // declarations
        Add_Child(temp, $11); // statements
        $2->symbol->type = $7;
        // Symbol Table
        temp->symbol = New_Symbol(NULL);
        $$ = temp;
    }
    ;

arg_list:
    arg multi_arg
    {
        Node *temp;
        temp = New_Node("arg_list", ARG_LIST, VOID, yylloc.last_line, NULL);
        Add_Sibling($1, $2);
        Add_Child(temp, $1);
        $$ = temp;
    }
    | /* E */ { $$ = NULL; }
    ;

multi_arg:
    T_COMMA arg multi_arg
    {
        Add_Sibling($2, $3);
        $$ = $2;
    }
    | /* E */ { $$ = NULL; }
    ;

arg:
    identifier T_COLON T_STANDARD_TYPE
    {
        Node *temp;
        temp = New_Node("arg", ARG, $3, yylloc.last_line, NULL);
        Add_Child(temp, $1);  // identifier
        $$ = temp;
    }
    ;

compound_statement:
    T_BEGIN optional_statements T_END T_DOT { $$ = $2; }
    | /* E */ { $$ = NULL; }
    ;

statements:
    statement { $$ = $1; }
    | T_BEGIN optional_statements T_END { $$ = $2; }
    ;

optional_statements:
    statement multi_statement
    {
        Node *temp;
        temp = New_Node("optional_statements", OPTIONAL_STATEMENTS, VOID, yylloc.last_line, NULL);
        Add_Sibling($1, $2);
        Add_Child(temp, $1);
        $$ = temp;
    }
    | /* E */ { $$ = NULL; }
    ;

multi_statement:
    T_SEMICOLON statement multi_statement
    {
        Add_Sibling($2, $3);
        $$ = $2;
    }
    | /* E */ { $$ = NULL; }
    ;

statement:
    matched_statement { $$ = $1; }
    | unmatched_statement { $$ = $1; }
    ;

matched_statement:
    assignop { $$ = $1; }
    | matched_if { $$ = $1; }
    | while { $$ = $1; }
    | for { $$ = $1; }
    | call { $$ = $1; }
    | write_statement { $$ = $1; }
    ;

unmatched_statement:
    if { $$ = $1; }
    | T_IF expression T_THEN matched_statement T_ELSE unmatched_statement
    {
        Node *temp;
        temp = New_Node("if", IF, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        Add_Child(temp, $4);
        Add_Child(temp, $6);
        $$ = temp;
    }
    ;

if:
    T_IF expression T_THEN statements
    {
        Node *temp;
        temp = New_Node("if", IF, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        Add_Child(temp, $4);
        $$ = temp;
    }
    ;

matched_if:
    T_IF expression T_THEN matched_statement T_ELSE matched_statement
    {
        Node *temp;
        temp = New_Node("if", IF, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        Add_Child(temp, $4);
        Add_Child(temp, $6);
        $$ = temp;
    }
    ;

write_statement:
    write_int { $$ = $1; }
    | write_char { $$ = $1; }
    | write_bool { $$ = $1; }
    | write_ln { $$ = $1; }
    ;

write_int:
    T_WRITE_INT T_OBRACKET expression T_CBRACKET
    {
        Node *temp;
        temp = New_Node("write_int", WRITE_INT, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

write_char:
    T_WRITE_CHAR T_OBRACKET expression T_CBRACKET
    {
        Node *temp;
        temp = New_Node("write_char", WRITE_CHAR, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

write_bool:
    T_WRITE_BOOL T_OBRACKET expression T_CBRACKET
    {
        Node *temp;
        temp = New_Node("write_bool", WRITE_BOOL, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

write_ln:
    T_WRITE_LN T_OBRACKET T_CBRACKET
    {
        Node *temp;
        temp = New_Node("write_ln", WRITE_LN, VOID, yylloc.last_line, NULL);
        $$ = temp;
    }
    ;

assignop:
    identifier T_ASSIGNOP expression
    {
        Node *temp;
        temp = New_Node("assignop", ASSIGNOP, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

while:
    T_WHILE expression T_DO statements
    {
        Node *temp;
        temp = New_Node("while", WHILE, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        Add_Child(temp, $4);
        $$ = temp;
    }
    ;

for:
    T_FOR assignop T_TO expression T_DO statements
    {
        Node *temp;
        temp = New_Node("for", FOR, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $2); // assignop
        Add_Child(temp, $4); // expression
        Add_Child(temp, $6); // statements
        $$ = temp;
    }
    ;

expression:
    simple_expression { $$ = $1; }
    | simple_expression rel_op simple_expression
    {
        Node *temp;
        temp = New_Node("rel_expr", REL_EXPR, BOOLEAN, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        Add_Child(temp, $2);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

simple_expression:
    term { $$ = $1; }
    | simple_expression add_op term
    {
        Node *temp;
        Type type = ((Node *) $2)->type;
        temp = New_Node("add_expr", ADD_EXPR, type, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        Add_Child(temp, $2);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

term:
    not_element { $$ = $1; }
    | term mul_op not_element
    {
        Node *temp;
        Type type = ((Node *) $2)->type;
        temp = New_Node("mul_expr", MUL_EXPR, type, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        Add_Child(temp, $2);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

not_element:
    element { $$ = $1; }
    | not_op element
    {
        Node *temp;
        temp = New_Node("not_element", NOT_ELEMENT, BOOLEAN, yylloc.last_line, NULL);
        Add_Child(temp, $2);
        $$ = temp;
    }
    ;

element:
    identifier { $$ = $1; }
    | standard_type { $$ = $1; }
    | call { $$ = $1; }
    | T_OBRACKET expression T_CBRACKET { $$ = $2; }
    ;

call:
    identifier T_OBRACKET call_arg_list T_CBRACKET
    {
        Node *temp;
        temp = New_Node("call", CALL, VOID, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        Add_Child(temp, $3);
        $$ = temp;
    }
    ;

call_arg_list:
    /* E */ { $$ = NULL; }
    | call_arg call_multi_arg
    {
        Node *temp;
        temp = New_Node("call_arg_list", CALL_ARG_LIST, VOID, yylloc.last_line, NULL);
        Add_Sibling($1, $2);
        Add_Child(temp, $1);
        $$ = temp;
    }
    ;

call_multi_arg:
    /* E */ { $$ = NULL; }
    | T_COMMA call_arg call_multi_arg
    {
        Add_Sibling($2, $3);
        $$ = $2;
    }
    ;

call_arg:
    expression
    {
        Node *temp;
        temp = New_Node("call_arg", CALL_ARG, ((Node *) $1)->type, yylloc.last_line, NULL);
        Add_Child(temp, $1);
        $$ = temp;
    }
    ;

add_op:
    T_PLUS
    {
        Node *temp;
        temp = New_Node($1, T_PLUS, INTEGER, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_MINUS
    {
        Node *temp;
        temp = New_Node($1, T_MINUS, INTEGER, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_OR
    {
        Node *temp;
        temp = New_Node($1, T_OR, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    ;

mul_op:
    T_MUL
    {
        Node *temp;
        temp = New_Node($1, T_MUL, INTEGER, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_DIV
    {
        Node *temp;
        temp = New_Node($1, T_DIV, INTEGER, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_AND
    {
        Node *temp;
        temp = New_Node($1, T_AND, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    ;

rel_op:
    T_CLT
    {
        Node *temp;
        temp = New_Node($1, T_CLT, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_CLE
    {
        Node *temp;
        temp = New_Node($1, T_CLE, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_CGT
    {
        Node *temp;
        temp = New_Node($1, T_CGT, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_CGE
    {
        Node *temp;
        temp = New_Node($1, T_CGE, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_CEQ
    {
        Node *temp;
        temp = New_Node($1, T_CEQ, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    | T_CNE
    {
        Node *temp;
        temp = New_Node($1, T_CNE, BOOLEAN, yylloc.last_line, NULL);
        $$ = temp;
    }
    ;

not_op:
    T_NOT { $$ = NULL; }
    ;

identifier:
    T_IDENTIFIER
    {
        Node *temp;

        temp = New_Node("identifier", T_IDENTIFIER, VOID, yylloc.last_line, NULL);
        temp->symbol = New_Symbol($1);
        $$ = temp;
    }
    ;

standard_type:
    T_INT
    {
        Node *temp;
        temp = New_Node("int", T_INT, INTEGER, yylloc.last_line, NULL);
        Set_Int(&temp->value, $1);
        $$ = temp;
    }
    | T_BOOL
    {
        Node *temp;
        temp = New_Node("bool", T_BOOL, BOOLEAN, yylloc.last_line, NULL);
        Set_Bool(&temp->value, $1);
        $$ = temp;
    }
    | T_CHAR
    {
        Node *temp;
        temp = New_Node("char", T_CHAR, CHAR, yylloc.last_line, NULL);
        Set_Char(&temp->value, $1);
        $$ = temp;
    }
    ;

%%

static void yyerror (const char *msg)
{
    fprintf(stderr, "Error in line number %d: %s\n", yyget_lineno(), msg);
}

int main (int argc, char **argv)
{
    Action *action;
	fptr = fopen("result.ill", "wb");

    if (argc > optind)
	{
        yyin = fopen(argv[optind], "r");

		if (yyin==NULL)
		{
			printf("The file doesn't exit.\n");
			exit(1);
			return 0;
		}
	}
    else
        yyin = stdin;

    yyparse();

    if (Check_Errors(global_node))
    {
        fprintf(stderr, "Too many errors to compile.\n");
        exit(1);
		return 0;
    }

    action = Code_Gen_New();

    if (action != NULL)
        Accept_Node(global_node, action);

    return 0;
}
