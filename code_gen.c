#include "Code_Gen.h"

static int stack_size = -1;
static void Write_Load(Node *, Action *);
static int Get_Type_Size(Type);
static int Process_Exp(Node *, Action *);

Action * Code_Gen_New()
{
	/*
	Allocate memory block
	Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
	The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
	If size is zero, the return value depends on the particular library implementation (it may or may not be a null pointer), but the returned pointer shall not be dereferenced.*/
	Action *action = (Action *)malloc(sizeof(Action));

	action->program = &Code_Gen_Program;
	action->program_decl = &Code_Gen_Program_Decl;
	action->declarations = &Code_Gen_Declarations;
	action->var_decl = &Code_Gen_Var_Decl;
	action->list_id = &Code_Gen_List_Id;
	action->subprogram_list = &Code_Gen_Subprogram_List;
	action->procedure = &Code_Gen_Subprogram;
	action->function = &Code_Gen_Subprogram;
	action->arg_list = &Code_Gen_Arg_List;
	action->arg = &Code_Gen_Arg;
	action->optional_statements = &Code_Gen_Optional_Statements;
	action->write_int = &Code_Gen_Write_Int;
	action->write_char = &Code_Gen_Write_Char;
	action->write_bool = &Code_Gen_Write_Bool;
	action->write_ln = &Code_Gen_Write_Ln;
	action->assignop = &Code_Gen_Assignop;
	action->if_stmt = &Code_Gen_If;
	action->while_stmt = &Code_Gen_While;
	action->for_stmt = &Code_Gen_For;
	action->rel_expr = &Code_Gen_Binary_Expr;
	action->add_expr = &Code_Gen_Binary_Expr;
	action->mul_expr = &Code_Gen_Binary_Expr;
	action->not_element = &Code_Gen_Not_Element;
	action->call = &Code_Gen_Call;
	action->call_arg_list = &Code_Gen_Arg_List;
	action->call_arg = &Code_Gen_Call_Arg;
	action->identifier = &Code_Gen_Identifier;
	action->standard_type = &Code_Gen_Standard_Type;
	action->add_op = &Code_Gen_Binary_Op;
	action->mul_op = &Code_Gen_Binary_Op;
	action->rel_op = &Code_Gen_Binary_Op;
	action->not_op = &Code_Gen_Not_Op;

	return action;
}

void Code_Gen_Program(Action *action, Node *node)
{
	Node *child;

	printf("; Generated with My Pascal Compiler\n");
	fprintf(fptr, "%s", "; Generated with My Pascal Compiler\n");
	for (child = node->children; child != NULL && child->category != OPTIONAL_STATEMENTS; child = child->sibling)
		Accept_Node(child, action);

	if (child != NULL)
	{
		printf("; Definition of main function\n");
		printf("define i32 @main () {\nentry:\n");
		Accept_Node(child, action);
		printf(EXTRA_SPACE"ret i32 0\n}\n\n");
	}
}

void Code_Gen_Program_Decl(Action *action, Node *node)
{
	printf("; program ");
	fprintf(fptr, "%s", "; program ");
	
	Accept_Node(node->children, action);

	printf("\n\n");
	fprintf(fptr, "%s", "\n\n");
	
	printf("; Declaration of Global Constants: \n");
	fprintf(fptr, "%s", "; Declaration of String Global Constants: \n");
	
	printf("@bool_str = global [2 x i8*] [ "
		"i8* getelementptr ([6 x i8]* @.false_str, i32 0, i32 0), "
		"i8* getelementptr ([5 x i8]* @.true_str, i32 0, i32 0) ]\n");
	fprintf(fptr, "@bool_str = global [2 x i8*] [ "
		"i8* getelementptr ([6 x i8]* @.false_str, i32 0, i32 0), "
		"i8* getelementptr ([5 x i8]* @.true_str, i32 0, i32 0) ]\n");

	printf("@.false_str = internal constant [6 x i8] c\"false\\00\"\n");
	fprintf(fptr, "%s", "@.false_str = internal constant[6 x i8] c\"false\\00\"\n");
	
	printf("@.true_str = internal constant [5 x i8] c\"true\\00\"\n");
	fprintf(fptr, "%s", "@.true_str = internal constant [5 x i8] c\"true\\00\"\n");

	printf("@.int_fmt = internal constant [3 x i8] c\"%%d\\00\"\n");
	fprintf(fptr, "%s", "@.int_fmt = internal constant [3 x i8] c\"%%d\\00\"\n");

	printf("@.bool_fmt = internal constant [3 x i8] c\"%%s\\00\"\n");
	fprintf(fptr, "%s", "@.bool_fmt = internal constant [3 x i8] c\"%%s\\00\"\n");

	printf("\n; External declaration of functions\n");
	fprintf(fptr, "%s", "\n; External declaration of functions\n");

	printf("declare i32 @puts(i8 *)\n");
	fprintf(fptr, "%s", "declare i32 @puts(i8 *)\n");
	
	printf("declare i32 @putchar(i32)\n");
	fprintf(fptr, "%s", "declare i32 @putchar(i32)\n");
	
	printf("declare i32 @printf(i8*, ...)\n\n");
	fprintf(fptr, "%s", "declare i32 @printf(i8*, ...)\n\n");
}

void Code_Gen_Declarations(Action *action, Node *node)
{
	Accept_Children(node->children, action);
	if (node->parent->category == PROGRAM)
	{
		printf("\n");
		fprintf(fptr, "%s", "\n");
	}
}

void Code_Gen_List_Id(Action *action, Node *node)
{
	Node *child;

	for (child = node->children; child != NULL; child = child->sibling)
	{
		Accept_Node(child, action);
		if (child->sibling != NULL)
			printf(", ");
	}
}

void Code_Gen_Subprogram_List(Action *action, Node *node)
{
	Accept_Children(node->children, action);
}

void Code_Gen_Subprogram(Action *action, Node *node)
{
	Node *child;

	printf("define ");
	fprintf(fptr, "%s", "define ");

	Write_Type(node->type);
	printf(" ");

	child = node->children; // identifier
	Accept_Node(child, action);

	printf(" ( ");
	fprintf(fptr, "%s", " ( ");

	child = child->sibling;
	if (child->category == ARG_LIST)
	{
		Accept_Node(child, action);
		child = child->sibling;
	}

	printf(" ) {\n");
	fprintf(fptr, " ) {\n");

	printf("entry:\n");
	fprintf(fptr, "%s", "entry:\n");

	if (child->category == DECLARATIONS)
	{
		Accept_Node(child, action);
		child = child->sibling;
	}

	Accept_Node(child, action);

	printf(EXTRA_SPACE"ret ");
	fprintf(fptr, "%s", EXTRA_SPACE"ret ");

	Write_Type(node->type);
	if (node->category == FUNCTION_DECL)
	{
		printf(" ");
		fprintf(fptr, "%s", " ");

		Write_Value(node->children, node->children->symbol->stack_index);
	}

	printf("\n}\n\n");
	fprintf(fptr, "%s", "\n}\n\n");
}

void Code_Gen_Arg_List(Action *action, Node *node)
{
	Node *child;

	for (child = node->children; child != NULL; child = child->sibling)
	{
		Accept_Node(child, action);
		if (child->sibling != NULL)
		{
			printf(", ");
			fprintf(fptr, "%s", ", ");
		}
	}
}

void Code_Gen_Optional_Statements(Action *action, Node *node)
{
	Node *child;

	stack_size = -1;

	for (child = node->children; child != NULL; child = child->sibling)
	{
		Accept_Node(child, action);
		fprintf(fptr, "%s", "\n");
	}
}

void Code_Gen_Binary_Expr(Action *action, Node *node)
{
	int lindex = -1;
	int rindex = -1;
	Node *lnode = node->children;
	Node *op = lnode->sibling;
	Node *rnode = op->sibling;

	int __process_binexpr_node(Node *node)
	{
		if (node->category == T_IDENTIFIER)
		{
			if (node->symbol->is_global)
			{
				if (Is_Symbol_Subprogram(node->symbol))
					return node->symbol->stack_index;

				Write_Load(node, action);
				return stack_size;
			}

		}
		else if (!IS_DATATYPE(node->category)) {
			Accept_Node(node, action);
			return stack_size;

		}
		return -1;
	}

	void __print_operand(Node *node, int index)
	{
		if (index > -1)
		{
			printf("%%%d", index);
			fprintf(fptr, "%%%d", index);
		}
		else if (node->symbol != NULL && Is_Symbol_Subprogram(node->symbol))
		{
			printf("0");
			fprintf(fptr, "%s", "0");
		}
		else
			Accept_Node(node, action);
	}

	/* Simple Structures */
	if (IS_DATATYPE(lnode->category) && IS_DATATYPE(rnode->category))
	{
		Accept_Node(op, action);
		printf(" ");
		fprintf(fptr, "%s", " ");
		Write_Type(lnode->type);
		printf(" ");
		fprintf(fptr, "%s", " ");
		Accept_Node(lnode, action);
		printf(", ");
		fprintf(fptr, "%s", ", ");
		Accept_Node(rnode, action);
		printf("\n");
		fprintf(fptr, "%s", "\n");
	}
	else
	{
		/* Complex structures */
		lindex = __process_binexpr_node(lnode);
		rindex = __process_binexpr_node(rnode);

		Accept_Node(op, action);
		printf(" ");
		fprintf(fptr, "%s", " ");
		Write_Type(lnode->type);
		printf(" ");
		fprintf(fptr, "%s", " ");

		__print_operand(lnode, lindex);
		printf(", ");
		fprintf(fptr, "%s", ", ");
		__print_operand(rnode, rindex);

		printf("\n");
		fprintf(fptr, "%s", "\n");
	}

	stack_size++;
}

void Code_Gen_Call_Arg_List(Action *action, Node *node)
{
	Accept_Node(node->children, action);
}

void Code_Gen_Call_Arg(Action *action, Node *node)
{
	Accept_Node(node->children, action);
}

void Code_Gen_Identifier(Action *action, Node *node)
{
	Symbol *sym = node->symbol;

	if (sym->is_global || node->parent->category == CALL)
	{
		printf("@%s", sym->name);
		fprintf(fptr, "@%s", sym->name);
	}
	else if (!sym->is_global && node->parent->category == ARG && sym->stack_index == -1)
	{
		printf("%%%s", sym->name);
		fprintf(fptr, "%%%s", sym->name);
	}
	else if (sym->stack_index > -1)
	{
		printf("%d", sym->stack_index);
		fprintf(fptr, "%s", sym->stack_index);
	}
	else
	{
		printf("0");
		fprintf(fptr, "%s", "0");
	}
}

void Code_Gen_Standard_Type(Action *action, Node *node)
{
	printf("%d", node->value.integer);
	fprintf(fptr, "%d", node->value.integer);
}

void Code_Gen_Var_Decl(Action *action, struct Nodes *node)
{
	Node *child;

	child = node->children;

	for (child = child->children; child != NULL; child = child->sibling)
	{
		if (node->parent->parent->category == PROGRAM)
		{
			Accept_Node(child, action);
			printf(" = global i%d 0\n", Get_Type_Size(child->type));
			fprintf(fptr, " = global i%d 0\n", Get_Type_Size(child->type));
		}
		else
			child->symbol->stack_index = -1;
	}
}

void Code_Gen_Arg(Action *action, Node *node)
{
	Write_Type(node->type);
	printf(" ");
	fprintf(fptr, "%s", " ");
	Accept_Node(node->children, action);
}

void Code_Gen_Write_Int(Action *action, Node *node)
{
	int index = -1;
	Node *child = node->children;

	index = Process_Exp(child, action);

	printf(EXTRA_SPACE"call i32 (i8* noalias , ...)* bitcast (i32 (i8*, ...)* \n");
	printf(EXTRA_SPACE EXTRA_SPACE"@printf to i32 (i8* noalias, ...)*)\n");
	printf(EXTRA_SPACE EXTRA_SPACE"( i8* getelementptr ");
	printf("([3 x i8]* @.int_fmt, i32 0, i32 0) noalias ,\n");
	printf(EXTRA_SPACE EXTRA_SPACE"i32 ");
	fprintf(fptr, "%s", EXTRA_SPACE"call i32 (i8* noalias , ...)* bitcast (i32 (i8*, ...)* \n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"@printf to i32 (i8* noalias, ...)*)\n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"( i8* getelementptr ");
	printf("([3 x i8]* @.int_fmt, i32 0, i32 0) noalias ,\n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"i32 ");
	Write_Value(child, index);

	printf(" )\n");
	fprintf(fptr, "%s", " )\n");
	stack_size++;
}

void Code_Gen_Write_Char(Action *action, Node *node)
{
	int index = -1;
	Node *child = node->children;

	index = Process_Exp(child, action);

	printf(EXTRA_SPACE"call i32 @putchar ( i32 ");
	fprintf(fptr, "%s", EXTRA_SPACE"call i32 @putchar ( i32 ");
	Write_Value(child, index);
	printf(" )\n");
	fprintf(fptr, "%s", " )\n");
	stack_size++;
}

void Code_Gen_Write_Bool(Action *action, Node *node)
{
	int index = -1;
	Node *child = node->children;

	index = Process_Exp(child, action);

	if (index == -1)
	{
		printf(EXTRA_SPACE"load i8** getelementptr ([2 x i8*]* @bool_str, i32 0, i32 %d ), align 4\n", Get_Val_As_Int(child));
		fprintf(fptr, EXTRA_SPACE"load i8** getelementptr ([2 x i8*]* @bool_str, i32 0, i32 %d ), align 4\n", Get_Val_As_Int(child));
	}
	else
	{
		printf(EXTRA_SPACE"getelementptr [2 x i8*]* @bool_str, i32 0, i32 %%%d\n", index);
		fprintf(fptr, EXTRA_SPACE"getelementptr [2 x i8*]* @bool_str, i32 0, i32 %%%d\n", index);
		stack_size++;
		printf(EXTRA_SPACE"load i8** %%%d, align 4\n", stack_size);
		fprintf(fptr, EXTRA_SPACE"load i8** %%%d, align 4\n", stack_size);
	}
	stack_size++;

	printf(EXTRA_SPACE"call i32 (i8* noalias , ...)* bitcast (i32 (i8*, ...)* \n");
	printf(EXTRA_SPACE EXTRA_SPACE"@printf to i32 (i8* noalias , ...)*)\n");
	printf(EXTRA_SPACE EXTRA_SPACE"( i8* getelementptr ");
	printf("([3 x i8]* @.bool_fmt, i32 0, i32 0) noalias , \n");
	printf(EXTRA_SPACE EXTRA_SPACE"i8* %%%d )\n", stack_size);

	fprintf(fptr, "%s", EXTRA_SPACE"call i32 (i8* noalias , ...)* bitcast (i32 (i8*, ...)* \n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"@printf to i32 (i8* noalias , ...)*)\n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"( i8* getelementptr ");
	fprintf(fptr, "%s", "([3 x i8]* @.bool_fmt, i32 0, i32 0) noalias , \n");
	fprintf(fptr, "%s", EXTRA_SPACE EXTRA_SPACE"i8* %%%d )\n", stack_size);
	stack_size++;
}

void Code_Gen_Write_Ln(Action *action, Node *node)
{
	printf(EXTRA_SPACE"call i32 @putchar( i32 10 )\n");
	fprintf(fptr, "%s", EXTRA_SPACE"call i32 @putchar( i32 10 )\n");
	stack_size++;
}

void Code_Gen_Assignop(Action *action, Node *node)
{
	int rindex = -1;
	Node *lnode = node->children;
	Node *rnode = lnode->sibling;

	printf("; [assignop][%s] %s(%d/%d) = %d\n", rnode->name, lnode->symbol->name, lnode->symbol->stack_index, stack_size, Get_Val_As_Int(rnode));
	fprintf(fptr, "; [assignop][%s] %s(%d/%d) = %d\n", rnode->name, lnode->symbol->name, lnode->symbol->stack_index, stack_size, Get_Val_As_Int(rnode));

	/* rnode */
	rindex = Process_Exp(rnode, action);

	/* lnode */
	if (lnode->symbol->is_global && !Is_Symbol_Subprogram(lnode->symbol)) {
		printf(EXTRA_SPACE"store ");
		fprintf(fptr, "%s", EXTRA_SPACE"store ");
		Write_Type(lnode->type);
		printf(" ");
		fprintf(fptr, "%s", " ");
		Write_Value(rnode, rindex);
		printf(", ");
		fprintf(fptr, "%s", ", ");
		Write_Type(lnode->type);
		printf("* ");
		fprintf(fptr, "%s", "* ");
		Accept_Node(lnode, action);
		printf(", align 4\n");
		fprintf(fptr, "%s", ", align 4\n");
	}
	else if (rindex == -1)
	{
		printf(EXTRA_SPACE"add ");
		fprintf(fptr, "%s", EXTRA_SPACE"add ");
		Write_Type(lnode->type);
		printf(" ");
		fprintf(fptr, "%s", " ");
		Write_Value(rnode, rindex);
		printf(", 0\n");
		fprintf(fptr, "%s", ", 0\n");
		stack_size++;
		lnode->symbol->stack_index = stack_size;

	}
	else
		lnode->symbol->stack_index = rindex;

	printf("; [assignop][%s] %s(%d/%d) = %d\n", rnode->name, lnode->symbol->name, lnode->symbol->stack_index, stack_size, Get_Val_As_Int(rnode));
	fprintf(fptr, "; [assignop][%s] %s(%d/%d) = %d\n", rnode->name, lnode->symbol->name, lnode->symbol->stack_index, stack_size, Get_Val_As_Int(rnode));
}

void Code_Gen_If(Action *action, Node *node)
{
	int index = -1;
	Node *expr = node->children;
	Node *cmd1 = expr->sibling;
	Node *cmd2 = cmd1->sibling;

	printf("; if evaluation, line %d\n", node->line_num);
	fprintf(fptr, "%s", " ");

	index = Process_Exp(expr, action);

	printf(EXTRA_SPACE"br i1 ");
	fprintf(fptr, "%s", EXTRA_SPACE"br i1 ");
	Write_Value(expr, index);
	printf(", label %%cond_true_%x, label ", node);
	fprintf(fptr, ", label %%cond_true_%x, label ", node);

	if (cmd2 == NULL)
	{
		printf("%%cond_next_%x\n", node);
		fprintf(fptr, "%%cond_next_%x\n", node);
	}
	else
	{
		printf("%%cond_false_%x\n", node);
		fprintf(fptr, "%%cond_false_%x\n", node);
	}

	printf("\ncond_true_%x:\n", node);
	fprintf(fptr, "\ncond_true_%x:\n", node);
	Accept_Node(cmd1, action);
	printf(EXTRA_SPACE"br label %%cond_next_%x\n", node);
	fprintf(fptr, EXTRA_SPACE"br label %%cond_next_%x\n", node);

	if (cmd2 != NULL)
	{
		printf("\ncond_false_%x:\n", node);
		fprintf(fptr, "\ncond_false_%x:\n", node);
		Accept_Node(cmd2, action);
		printf(EXTRA_SPACE"br label %%cond_next_%x\n", node);
		fprintf(fptr, EXTRA_SPACE"br label %%cond_next_%x\n", node);
	}

	printf("\ncond_next_%x:\n", node);
	fprintf(fptr, "\ncond_next_%x:\n", node);
}

void Code_Gen_While(Action *action, Node *node)
{
	Node *child;

	printf("while (");
	fprintf(fptr, "while (");

	child = node->children; // expression
	Accept_Node(child, action);

	printf(") {\n");
	fprintf(fptr, "%s", ") {\n");

	child = child->sibling; // statements
	Accept_Node(child, action);

	printf("}\n");
	fprintf(fptr, "%s", "}\n");
}

void Code_Gen_For(Action *action, Node *node)
{
	int index = -1;
	Node *asgn = node->children;
	Node *expr = asgn->sibling;
	Node *statement = expr->sibling;

	printf("; for evaluation, line %d\n", node->line_num);
	fprintf(fptr, "; for evaluation, line %d\n", node->line_num);

	Accept_Node(asgn, action);
	index = Process_Exp(expr, action);
	printf(EXTRA_SPACE"br i1 ");
	fprintf(fptr, "%s", EXTRA_SPACE"br i1 ");

	Write_Value(expr, index);
	printf(", label %%bb_%x.preheader, label %%bb_%x\n", node, node);
	fprintf(fptr, ", label %%bb_%x.preheader, label %%bb_%x\n", node, node);

	printf("\nbb_%x.preheader:\n", node);
	fprintf(fptr, "\nbb_%x.preheader:\n", node);

	printf("\ncond_true_%x:\n", node);
	fprintf(fptr, "\ncond_true_%x:\n", node);

	Accept_Node(statement, action);

	printf(EXTRA_SPACE"br label %%cond_next_%x\n", node);
	fprintf(fptr, EXTRA_SPACE"br label %%cond_next_%x\n", node);

	printf("\ncond_next_%x:\n", node);
	fprintf(fptr, "\ncond_next_%x:\n", node);
}

void Code_Gen_Not_Element(Action *action, Node *node)
{
	Accept_Children(node->children, action);
}

void Code_Gen_Call(Action *action, Node *node)
{
	Node *child = node->children;

	printf(EXTRA_SPACE"call ");
	fprintf(fptr, "%s", EXTRA_SPACE"call ");

	Write_Type(child->symbol->type);
	printf(" ");
	fprintf(fptr, "%s", " ");

	Accept_Node(child, action);
	printf("( ");
	fprintf(fptr, "%s", "( ");

	child = child->sibling;
	if (child != NULL)
	{
		for (child = child->children; child != NULL; child = child->sibling)
		{
			Write_Type(child->type);
			printf(" ");
			fprintf(fptr, "%s", " ");
			Accept_Node(child, action);
			if (child->sibling != NULL)
			{
				printf(", ");
				fprintf(fptr, "%s", ", ");
			}
		}
	}

	printf(" )\n");
	fprintf(fptr, "%s", " )\n");
	stack_size++;
}

void Code_Gen_simplenode(Action *action, Node *node)
{
	Accept_Children(node->children, action);
}

void Code_Gen_Binary_Op(Action *action, Node *node)
{
	switch (node->category)
	{
	case T_OR:
		printf(EXTRA_SPACE"or");
		fprintf(fptr, "%s", EXTRA_SPACE"or");
		break;
	case T_AND:
		printf(EXTRA_SPACE"and");
		fprintf(fptr, "%s", EXTRA_SPACE"and");
		break;
	case T_CEQ:
		printf(EXTRA_SPACE"icmp eq");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp eq");
		break;
	case T_CNE:
		printf(EXTRA_SPACE"icmp ne");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp ne");
		break;
	case T_CLT:
		printf(EXTRA_SPACE"icmp slt");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp slt");
		break;
	case T_CGT:
		printf(EXTRA_SPACE"icmp sgt");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp sgt");
		break;
	case T_CLE:
		printf(EXTRA_SPACE"icmp sle");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp sle");
		break;
	case T_CGE:
		printf(EXTRA_SPACE"icmp sge");
		fprintf(fptr, "%s", EXTRA_SPACE"icmp sge");
		break;
	case T_PLUS:
		printf(EXTRA_SPACE"add");
		fprintf(fptr, "%s", EXTRA_SPACE"add");
		break;
	case T_MINUS:
		printf(EXTRA_SPACE"sub");
		fprintf(fptr, "%s", EXTRA_SPACE"sub");
		break;
	case T_MUL:
		printf(EXTRA_SPACE"mul");
		fprintf(fptr, "%s", EXTRA_SPACE"mul");
		break;
	case T_DIV:
		printf(EXTRA_SPACE"udiv");
		fprintf(fptr, "%s", EXTRA_SPACE"udiv");
		break;
	}
}

void Code_Gen_Not_Op(Action *action, Node *node)
{
	printf("%s!", node->name);
	fprintf(fptr, "%s!", node->name);
}

static void Write_Load(Node *node, Action *action)
{
	printf(EXTRA_SPACE"load ");
	fprintf(fptr, "%s", EXTRA_SPACE"load ");
	Write_Type(node->type);
	printf("* ");
	fprintf(fptr, "%s", "* ");
	Accept_Node(node, action);
	printf(", align 4\n");
	fprintf(fptr, "%s", ", align 4\n");
	stack_size++;
}

static int Get_Type_Size(Type type)
{
	switch (type)
	{
		case INTEGER:
			return 32;
		case CHAR:
			return 8;
		case BOOLEAN:
			return 1;
		default:
			return 0;
	}
}

static int Process_Exp(Node *rnode, Action *action)
{
	if (!IS_DATATYPE(rnode->category)) {
		if (rnode->category != T_IDENTIFIER)
		{
			Accept_Node(rnode, action);
			return stack_size;
		}
		else if (rnode->symbol->is_global)
		{
			Write_Load(rnode, action);
			return stack_size;
		}
		else
			return rnode->symbol->stack_index;
	}
	return -1;
}

void Write_Value(Node *n, int i)
{
	if (n->category == T_IDENTIFIER && n->parent->category == ARG)
	{
		printf("%s", n->symbol->name);
		fprintf(fptr, "%s", n->symbol->name);
	}
	else
	{
		if (i == -1)
		{
			printf("%d", Get_Val_As_Int(n));
			fprintf(fptr, "%d", Get_Val_As_Int(n));
		}
		else
		{
			printf("%d", i);
			fprintf(fptr, "%d", i);
		}
	}
}

void Write_Type(Type t)
{
	if (t == VOID)
	{
		printf("void");
		fprintf(fptr, "%s", "void");
	}
	else
	{
		printf("i%d", Get_Type_Size(t));
		fprintf(fptr, "%d", "i%d", Get_Type_Size(t));
	}
}