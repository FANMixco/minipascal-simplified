#ifndef LLVM_CODEGEN_VISITOR_H
#define LLVM_CODEGEN_VISITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Structures.h"

#define IS_DATATYPE(x) (x == T_BOOL || x == T_CHAR || x == T_INT)
#define EXTRA_SPACE "    "

//Kind of constructor to set LLVM actions
Action *Code_Gen_New();

//Actions to transform the code to LLVM
void Code_Gen_Program (Action *, Node *);
void Code_Gen_Program_Decl (Action *, Node *);
void Code_Gen_Subprogram_List (Action *, Node *);
void Code_Gen_Subprogram (Action *, Node *);
void Code_Gen_Declarations (Action *, Node *);
void Code_Gen_Var_Decl (Action *, Node *);
void Code_Gen_List_Id (Action *, Node *);
void Code_Gen_Arg_List (Action *, Node *);
void Code_Gen_Arg (Action *, Node *);
void Code_Gen_Optional_Statements(Action *, Node *);
void Code_Gen_Write_Int (Action *, Node *);
void Code_Gen_Write_Char (Action *, Node *);
void Code_Gen_Write_Bool (Action *, Node *);
void Code_Gen_Write_Ln (Action *, Node *);
void Code_Gen_Assignop (Action *, Node *);
void Code_Gen_If (Action *, Node *);
void Code_Gen_While (Action *, Node *);
void Code_Gen_For (Action *, Node *);
void Code_Gen_Binary_Expr (Action *, Node *);
void Code_Gen_Not_Element (Action *, Node *);
void Code_Gen_Call (Action *, Node *);
void Code_Gen_Call_Arg_List (Action *, Node *);
void Code_Gen_Call_Arg (Action *, Node *);
void Code_Gen_Identifier (Action *, Node *);
void Code_Gen_Standard_Type (Action *, Node *);
void Code_Gen_Binary_Op (Action *, Node *);
void Code_Gen_Not_Op (Action *, Node *);
void Write_Value (Node *, int);
void Write_Type (Type);

#endif // LLVM_CODEGEN_ACTIONS_H
