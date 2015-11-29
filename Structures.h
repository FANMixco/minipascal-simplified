#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/*
http://stackoverflow.com/questions/6004963/why-use-define-instead-of-a-variable
The #define is part of the preprocessor language for C and C++. When they're used in code, the compiler just replaces the #define statement with what ever you want. For example, if you're sick of writing for (int i=0; i<=10; i++) all the time, you can do the following:

#define fori10 for (int i=0; i<=10; i++)

// some code...

fori10 {
// do stuff to i
}

If you want something more generic, you can create preprocessor macros:

#define fori(x) for (int i=0; i<=x; i++)
// the x will be replaced by what ever is put into the parenthesis, such as
// 20 here
fori(20) {
// do more stuff to i
}
//why did I use exit(1) instead of exit(0)? Because I'm indicate that there is a bug in the user code indicates unsucessful termination. However, it's usage is non-portable.

More info previously checked:
http://www.cplusplus.com/doc/tutorial/preprocessor/

The C library function void free(void *ptr) deallocates the memory previously allocated by a call to calloc, malloc, or realloc.
http://www.tutorialspoint.com/c_standard_library/c_function_free.htm
*/

#define VOID(x) ((void *) &x)

FILE *fptr;

typedef enum Types {
    ERROR = -1,
    VOID,
    INTEGER,
    BOOLEAN,
    CHAR,
	REAL
} Type;

typedef enum {
    FALSE = 0,
    TRUE
} bool;

//Valid categories
typedef enum Categories {
	NONE_KIND = -1, //Without any category = -1
	PROGRAM, //The keyword for initialize any program = 0
	PROGRAM_DECL, //Program declarations = 1
	DECLARATIONS, //Variable collection = 2
	VAR_DECL, //Variable declaration = 3
	LIST_ID, //Collection of IDs = 4
	SUBPROGRAM_DECLARATIONS, //Subprogram collection = 5, it means the collection of Functions or Procedures
	PROCEDURE_DECL, //Procedure = 6
	FUNCTION_DECL, //Function = 7
	ARG_LIST, //Argument collection = 8
	ARG, //Just one Argument = 9
	OPTIONAL_STATEMENTS, //Optional statements = 10
	WRITE_INT, //Print an integer on screen = 11
	WRITE_CHAR, //Print a char on screen = 12
	WRITE_BOOL, //Print a bool on screen = 13
	WRITE_LN, //Print \n on screen = 14
	ASSIGNOP, //Assign a new value to the variable using the operator := because its pascal (it remembers me to Oracle) = 15
	IF, //Indicates that we're gonna use an If, = 16
	WHILE, //Indicates that we're gonna use a While = 17
	FOR, //Indicates that we're gonna use a For = 18
	REL_EXPR, //Indicates that we're evaluating a relational operation such as < (less than), <= (less or equal than), > (greater than), >= (greater or equal), = (equal) an example if a=b then begin a := b end else begin a := c end, = 19
	ADD_EXPR, //Indicates that we're gonna evaluate basic operations such as adding (a + b), subtracting (a - b), logical adding (a or b), = 20
	MUL_EXPR, //Indicates the expression with greater precedence such as multiplication (a * b), divion (a / b), logical multiplication (a and b), = 21
	NOT_ELEMENT, //Indicates the negation of an expression such as: not a, = 22
	CALL, //Indicates that we're gonna execute a function or a procedures example: gcd(5), = 23
	CALL_ARG_LIST, //Indicates the collection of arguments that we're gonna pass to the subprograms, 24
	CALL_ARG //Indicates that we're gonna send just one argument to subprograms, = 25
} Category;

typedef union {
        int integer;
        bool boolean;
        char character;
} Value;

typedef struct Symbols {
    char *name;
    Type type;
    Value value;
    int decl_line_num;

    // For procedures and functions
    int params;
    Type *param_types;

    bool is_global;
    int stack_index;
    struct Symbols *next;
} Symbol;

typedef struct Nodes {
    char *name;
    int category;
    Type type;
    Value value;
    Symbol *symbol;
    int line_num;
    int child_counter;
    struct Nodes* parent;
    struct Nodes* children;
    struct Nodes* sibling;
} Node;

//This is the core for translate to Any Language it can derive to others like Assembly, C++, LLVM, etc.
typedef struct Action_Table {
    void (*program) (struct Action_Table *, Node *);
    void (*program_decl) (struct Action_Table *, Node *);
    void (*declarations) (struct Action_Table *, Node *);
    void (*var_decl) (struct Action_Table *, Node *);
    void (*list_id) (struct Action_Table *, Node *);
    void (*identifier) (struct Action_Table *, Node *);
    void (*subprogram_list) (struct Action_Table *, Node *);
    void (*procedure) (struct Action_Table *, Node *);
    void (*function) (struct Action_Table *, Node *);
    void (*arg_list) (struct Action_Table *, Node *);
    void (*arg) (struct Action_Table *, Node *);
    void (*optional_statements) (struct Action_Table *, Node *);
    void (*write_int) (struct Action_Table *, Node *);
    void (*write_char) (struct Action_Table *, Node *);
    void (*write_bool) (struct Action_Table *, Node *);
    void (*write_ln) (struct Action_Table *, Node *);
    void (*assignop) (struct Action_Table *, Node *);
    void (*if_stmt) (struct Action_Table *, Node *);
    void (*while_stmt) (struct Action_Table *, Node *);
    void (*for_stmt) (struct Action_Table *, Node *);
    void (*rel_expr) (struct Action_Table *, Node *);
    void (*add_expr) (struct Action_Table *, Node *);
    void (*mul_expr) (struct Action_Table *, Node *);
    void (*not_element) (struct Action_Table *, Node *);
    void (*call) (struct Action_Table *, Node *);
    void (*call_arg_list) (struct Action_Table *, Node *);
    void (*call_arg) (struct Action_Table *, Node *);
    void (*standard_type) (struct Action_Table *, Node *);
    void (*add_op) (struct Action_Table *, Node *);
    void (*mul_op) (struct Action_Table *, Node *);
    void (*rel_op) (struct Action_Table *, Node *);
    void (*not_op) (struct Action_Table *, Node *);
} Action;

//This special void helps us to know which functions are we gonna to execute from the Action_Table based on the LLVM for example
typedef void (*ActToExec)(Action *, Node *);

//This are some kind of constructor for the Tree and it's nodes and the Symbol (I'm trying to imitate OOP!!!!)
Node *New_Node(const char *name, int category, int type, int line_num, Symbol *symbol);
Symbol *New_Symbol(char const * name);

//This function the type from the Variables
Type Get_Type_From_OpIden(const char *opiden);

//This is a generic function that is used to set the Variables Values
void Set_Value(Value *value, Type type, void *val);

//This function set an int
void Set_Int(Value *value, int val);
void Set_Bool(Value *value, bool val);
void Set_Char(Value *value, char val);
void Set_Real(Value *value, double val);
void Delete_Node(Node *);
int Get_Val_As_Int(Node *);
void Add_Child(Node *, Node *);
void Add_Sibling(Node *, Node *);
void Accept_Node(Node *, Action *);
void Accept_Children(Node *, Action *);
bool Check_Errors(Node *);
bool Is_Symbol_Subprogram(Symbol *symbol);

#endif // STRUCTURES_H
