#include <stdio.h>
#include "data_desk.h"

static FILE *global_catchall_header;
static FILE *global_catchall_implementation;

static void GeneratePrintCodeForAST(FILE *file, DataDeskASTNode *root, char *access_string);

DATA_DESK_FUNC void
DataDeskCustomInitCallback(void)
{
	global_catchall_header          = fopen("catchall.h", "w");
	global_catchall_implementation  = fopen("catchall.c", "w");
}

DATA_DESK_FUNC void
DataDeskCustomFileCallback(DataDeskASTNode *root, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomConstantCallback(DataDeskConstant constant_info, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomStructCallback(DataDeskStruct struct_info, char *filename)
{
	DataDeskFWriteStructAsC(global_catchall_header, struct_info);

	if(DataDeskNodeHasTag(struct_info.root, "Printable"))
	{
		// NOTE(rjf): Generate print function header.
		{
			FILE *file = global_catchall_header;	
			fprintf(file, "void Print%s(%s *);\n", struct_info.name, struct_info.name);
		}

		// NOTE(rjf): Generate print function implementation.
		{
			FILE *file = global_catchall_implementation;
			fprintf(file, "void Print%s(%s *object)\n", struct_info.name, struct_info.name);
			fprintf(file, "{\n\n");
			GeneratePrintCodeForAST(file, struct_info.root, "object->");
			fprintf(file, "\n}\n\n");
		}
	}
}

DATA_DESK_FUNC void
DataDeskCustomEnumCallback(DataDeskEnum enum_info, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomFlagsCallback(DataDeskFlags flags_info, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomDeclarationCallback(DataDeskDeclaration declaration_info, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomCleanUpCallback(void) {}

static void
GeneratePrintCodeForAST(FILE *file, DataDeskASTNode *root, char *access_string)
{
	if(!DataDeskNodeHasTag(root, "DoNotPrint"))
	{
		switch(root->type)
		{
			case DATA_DESK_AST_NODE_TYPE_struct_declaration:
			{
				fprintf(file, "printf(\"{ \");\n");
				for(DataDeskASTNode *field = root->struct_declaration.first_member; field; field = field->next)
				{
					GeneratePrintCodeForAST(file, field, access_string);
				}
				fprintf(file, "printf(\"}\");\n");
				break;
			}

			case DATA_DESK_AST_NODE_TYPE_declaration:
			{
				if(DataDeskDeclarationIsType(root, "int") ||
				   DataDeskDeclarationIsType(root, "i32") || DataDeskDeclarationIsType(root, "u32") ||
				   DataDeskDeclarationIsType(root, "i16") || DataDeskDeclarationIsType(root, "u16") ||
				   DataDeskDeclarationIsType(root, "i8")  || DataDeskDeclarationIsType(root,  "u8"))
				{
					fprintf(file, "printf(\"%%i, \", %s%s);\n", access_string, root->string);
				}
				else if(DataDeskDeclarationIsType(root, "float") || DataDeskDeclarationIsType(root, "f32") ||
					    DataDeskDeclarationIsType(root, "f64"))
				{
					fprintf(file, "printf(\"%%f, \", %s%s);\n", access_string, root->string);
				}
				else if(DataDeskDeclarationIsType(root, "b32") || DataDeskDeclarationIsType(root, "b16") ||
				   		DataDeskDeclarationIsType(root, "b8"))
				{
					fprintf(file, "printf(\"%%s, \", %s%s ? \"true\" : \"false\");\n", access_string, root->string);
				}
				else if(DataDeskDeclarationIsType(root, "*char"))
				{
					fprintf(file, "printf(\"%%s, \", %s%s);\n", access_string, root->string);
				}
				break;
			}

			default: break;
		}
	}
}