#include "Structures.h"

Node * New_Node(const char* name, int category, int type, int line_num, Symbol *symbol)
{
	Node *node;

	node = (Node *)malloc(sizeof(Node));

	if (name != NULL)
		node->name = strdup(name);
	else
		node->name = NULL;

	node->category = category;
	node->type = type;
	node->line_num = line_num;
	node->child_counter = 0;
	node->symbol = symbol;
	node->parent = NULL;
	node->children = NULL;
	node->sibling = NULL;

	return node;
}

Symbol * New_Symbol(char const * name)
{
	Symbol * symbol = (Symbol *)malloc(sizeof(Symbol));
	symbol->type = VOID;
	Set_Value(&symbol->value, symbol->type, NULL);
	symbol->params = -1;
	symbol->param_types = NULL;
	symbol->decl_line_num = 0;
	symbol->is_global = FALSE;
	symbol->stack_index = -1;

	symbol->next = NULL;

	if (name != NULL)
		symbol->name = strdup(name);
	else
		symbol->name = NULL;

	return symbol;
}

Type Get_Type_From_OpIden(const char *opiden)
{
    if (!strcasecmp (opiden, "Integer"))
        return INTEGER;
    else if (!strcasecmp (opiden, "Boolean"))
        return BOOLEAN;
    else if (!strcasecmp (opiden, "Char"))
        return CHAR;
    else
        return VOID;
}

void Set_Value(Value *value, Type type, void *val)
{
    if (value == NULL)
	{
        fprintf(stderr, "Trying to set a NULL value\n");
        exit(1);
    }

    if (type == VOID || val == NULL)
        value->integer = 0;
    else if (type == INTEGER)
       value->integer = *((int *) val);
    else if (type == BOOLEAN)
       value->boolean = *((bool *) val);
    else if (type == CHAR)
       value->character = *((char *) val);
    else
	{
       fprintf(stderr, "Set_Value: unknow type\n");
       exit(1);
    }
}

void Set_Int(Value *value, int val)
{
    Set_Value(value, INTEGER, VOID(val));
}

void Set_Bool(Value *value, bool val)
{
    Set_Value(value, BOOLEAN, VOID(val));
}

void Set_Char(Value *value, char val)
{
    Set_Value(value, CHAR, VOID(val));
}

void Set_Real(Value *value, double val)
{
	Set_Value(value, REAL, VOID(val));
}

void Delete_Node(Node *self)
{
    if (self != NULL)
	{
        Delete_Node(self->children);
        Delete_Node(self->sibling);
        free(self);
    }
}

int Get_Val_As_Int(Node *self)
{
    if (self->category == T_IDENTIFIER)
        return self->symbol->value.integer;

    return self->value.integer;
}

void Add_Child(Node *self, Node *child)
{
    Node *temp;

    if (child == NULL)
        return;

    if (self->children == NULL)
	{
        child->parent = self;
        self->children = child;
    }
	else
        Add_Sibling(self->children, child);
    
    for (temp = child; temp != NULL; temp = temp->sibling)
        temp->parent = self;
}

void Add_Sibling(Node *self, Node *sibling)
{
    Node *temp;

    if (sibling == NULL)
        return;

    if (self->sibling == NULL)
        self->sibling = sibling;
    else
	{
        for (temp = self->sibling; temp->sibling != NULL; temp = temp->sibling)
            ;
        temp->sibling = sibling;
    }
}

void Accept_Node(Node *self, Action *action)
{
    ActToExec acttoexec;

    if (self == NULL)
        return;

    self->child_counter = 1;

    switch (self->category)
	{
        case PROGRAM:
            acttoexec = action->program;
            break;
        case PROGRAM_DECL:
            acttoexec = action->program_decl;
            break;
        case DECLARATIONS:
            acttoexec = action->declarations;
            break;
        case VAR_DECL:
            acttoexec = action->var_decl;
            break;
        case LIST_ID:
            acttoexec = action->list_id;
            break;
        case SUBPROGRAM_DECLARATIONS:
            acttoexec = action->subprogram_list;
            break;
        case PROCEDURE_DECL:
            acttoexec = action->procedure;
            break;
        case FUNCTION_DECL:
            acttoexec = action->function;
            break;
        case ARG_LIST:
            acttoexec = action->arg_list;
            break;
        case ARG:
            acttoexec = action->arg;
            break;
        case OPTIONAL_STATEMENTS:
            acttoexec = action->optional_statements;
            break;
        case WRITE_INT:
            acttoexec = action->write_int;
            break;
        case WRITE_CHAR:
            acttoexec = action->write_char;
            break;
        case WRITE_BOOL:
            acttoexec = action->write_bool;
            break;
        case WRITE_LN:
            acttoexec = action->write_ln;
            break;
        case ASSIGNOP:
            acttoexec = action->assignop;
            break;
        case IF:
            acttoexec = action->if_stmt;
            break;
        case WHILE:
            acttoexec = action->while_stmt;
            break;
        case FOR:
            acttoexec = action->for_stmt;
            break;
        case REL_EXPR:
            acttoexec = action->rel_expr;
            break;
        case ADD_EXPR:
            acttoexec = action->add_expr;
            break;
        case MUL_EXPR:
            acttoexec = action->mul_expr;
            break;
        case NOT_ELEMENT:
            acttoexec = action->not_element;
            break;
        case CALL:
            acttoexec = action->call;
            break;
        case CALL_ARG_LIST:
            acttoexec = action->call_arg_list;
            break;
        case CALL_ARG:
            acttoexec = action->call_arg;
            break;
        case T_IDENTIFIER:
            acttoexec = action->identifier;
            break;
        case T_INT:
        case T_BOOL:
        case T_CHAR:
            acttoexec = action->standard_type;
            break;
        case T_PLUS:
        case T_MINUS:
        case T_OR:
            acttoexec = action->add_op;
            break;
        case T_MUL:
        case T_DIV:
        case T_AND:
            acttoexec = action->mul_op;
            break;
        case T_CEQ:
        case T_CNE:
        case T_CLT:
        case T_CGT:
        case T_CLE:
        case T_CGE:
            acttoexec = action->rel_op;
            break;
        case T_NOT:
            acttoexec = action->not_op;
            break;
        default:
            acttoexec = NULL;
			//fclose(fptr);
    }

    if (acttoexec != NULL)
        acttoexec(action, self);
}

void Accept_Children(Node *self, Action *action)
{
    Node *temp;
    for (temp = self; temp != NULL; temp = temp->sibling)
        Accept_Node(temp, action);
}

bool Is_Symbol_Subprogram(Symbol *symbol)
{
    return (symbol->params > -1);
}

bool Check_Errors(Node *self)
{
	Node *child;

	if (self == NULL || self->type == ERROR)
		return TRUE;

	for (child = self->children; child != NULL; child = child->sibling)
	{
		if (Check_Errors(child) == TRUE)
			return TRUE;
	}

	return FALSE;
}