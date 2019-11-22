/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Data Desk

Author  : Ryan Fleury
Updated : 15 October 2019
License : MIT, at end of file.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
GenerateNullTerminatedStringsForAST(ParseContext *context, ASTNode *root)
{
    if(root)
    {
        if(root->string)
        {
            char *new_string = ParseContextAllocateMemory(context, root->string_length+1);
            MemoryCopy(new_string, root->string, root->string_length);
            new_string[root->string_length] = 0;
            root->string = new_string;
        }
        
        switch(root->type)
        {
            case DATA_DESK_AST_NODE_TYPE_binary_operator:
            {
                GenerateNullTerminatedStringsForAST(context, root->binary_operator.left);
                GenerateNullTerminatedStringsForAST(context, root->binary_operator.right);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_struct_declaration:
            {
                GenerateNullTerminatedStringsForAST(context, root->struct_declaration.first_member);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_enum_declaration:
            {
                GenerateNullTerminatedStringsForAST(context, root->enum_declaration.first_constant);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_flags_declaration:
            {
                GenerateNullTerminatedStringsForAST(context, root->flags_declaration.first_flag);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_declaration:
            {
                GenerateNullTerminatedStringsForAST(context, root->declaration.type);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_type_usage:
            {
                GenerateNullTerminatedStringsForAST(context, root->type_usage.first_array_size_expression);
                GenerateNullTerminatedStringsForAST(context, root->type_usage.struct_declaration);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_constant_definition:
            {
                GenerateNullTerminatedStringsForAST(context, root->constant_definition.expression);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_procedure_header:
            {
                GenerateNullTerminatedStringsForAST(context, root->procedure_header.first_parameter);
                GenerateNullTerminatedStringsForAST(context, root->procedure_header.return_type);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_tag:
            {
                GenerateNullTerminatedStringsForAST(context, root->tag.first_tag_parameter);
                break;
            }
            default: break;
        }
        
        if(root->first_tag)
        {
            GenerateNullTerminatedStringsForAST(context, root->first_tag);
        }
        
        if(root->next)
        {
            GenerateNullTerminatedStringsForAST(context, root->next);
        }
    }
}

static void
PrintASTFromRoot(ASTNode *root, int follow_next)
{
    if(root)
    {
        switch(root->type)
        {
            default:
            case DATA_DESK_AST_NODE_TYPE_identifier:
            case DATA_DESK_AST_NODE_TYPE_numeric_constant:
            case DATA_DESK_AST_NODE_TYPE_string_constant:
            case DATA_DESK_AST_NODE_TYPE_char_constant:
            {
                printf("%.*s", root->string_length, root->string);
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_binary_operator:
            {
                printf("(");
                PrintASTFromRoot(root->binary_operator.left, 0);
                printf(" %s ", GetBinaryOperatorStringFromType(root->binary_operator.type));
                PrintASTFromRoot(root->binary_operator.right, 0);
                printf(")");
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_struct_declaration:
            {
                printf("struct");
                
                if(root->string)
                {
                    printf(" %.*s", root->string_length, root->string);
                }
                
                printf("\n{\n");
                
                for(DataDeskASTNode *member = root->struct_declaration.first_member;
                    member;
                    member = member->next)
                {
                    PrintASTFromRoot(member, 0);
                    printf(";\n");
                }
                
                printf("}\n\n");
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_enum_declaration:
            {
                printf("enum");
                
                if(root->string)
                {
                    printf(" %.*s", root->string_length, root->string);
                }
                
                printf("\n{\n");
                
                for(DataDeskASTNode *member = root->enum_declaration.first_constant;
                    member;
                    member = member->next)
                {
                    PrintASTFromRoot(member, 0);
                    printf(",\n");
                }
                
                printf("}\n\n");
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_flags_declaration:
            {
                printf("flags");
                
                if(root->string)
                {
                    printf(" %.*s", root->string_length, root->string);
                }
                
                printf("\n{\n");
                
                for(DataDeskASTNode *member = root->flags_declaration.first_flag;
                    member;
                    member = member->next)
                {
                    PrintASTFromRoot(member, 0);
                    printf(",\n");
                }
                
                printf("}\n\n");
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_declaration:
            {
                printf("%.*s : ", root->string_length, root->string);
                PrintASTFromRoot(root->declaration.type, 0);
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_type_usage:
            {
                for(int i = 0; i < root->type_usage.pointer_count; ++i)
                {
                    printf("*");
                }
                
                if(root->type_usage.struct_declaration)
                {
                    PrintASTFromRoot(root->type_usage.struct_declaration, 0);
                }
                else
                {
                    printf("%.*s", root->string_length, root->string);
                }
                
                for(DataDeskASTNode *array = root->type_usage.first_array_size_expression;
                    array;
                    array = array->next)
                {
                    printf("[");
                    PrintASTFromRoot(array, 0);
                    printf("]");
                }
                
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_tag:
            {
                printf("%.*s\n", root->string_length, root->string);
                break;
            }
            
        }
        
        if(follow_next)
        {
            PrintASTFromRoot(root->next, follow_next);
        }
    }
}

static void 
PrintAST(ASTNode *root)
{
    PrintASTFromRoot(root, 1);
}

static void
TraverseASTAndCallCustomParseCallbacks(ParseContext *context, ASTNode *root, DataDeskCustom custom, char *filename)
{
    if(root)
    {
        
        switch(root->type)
        {
            
            case DATA_DESK_AST_NODE_TYPE_struct_declaration:
            {
                if(custom.StructCallback)
                {
                    DataDeskStruct struct_info = {0};
                    {
                        struct_info.name = root->string;
                        struct_info.name_lowercase_with_underscores =
                            ParseContextAllocateStringCopyLowercaseWithUnderscores(context, struct_info.name);
                        struct_info.name_uppercase_with_underscores =
                            ParseContextAllocateStringCopyUppercaseWithUnderscores(context, struct_info.name);
                        struct_info.name_lower_camel_case =
                            ParseContextAllocateStringCopyLowerCamelCase(context, struct_info.name);
                        struct_info.name_upper_camel_case =
                            ParseContextAllocateStringCopyUpperCamelCase(context, struct_info.name);
                        struct_info.name_with_spaces =
                            ParseContextAllocateStringCopyWithSpaces(context, struct_info.name);
                        struct_info.root = root;
                    }
                    custom.StructCallback(struct_info, filename);
                }
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_enum_declaration:
            {
                if(custom.EnumCallback)
                {
                    DataDeskEnum enum_info = {0};
                    {
                        enum_info.name = root->string;
                        enum_info.name_lowercase_with_underscores =
                            ParseContextAllocateStringCopyLowercaseWithUnderscores(context, enum_info.name);
                        enum_info.name_uppercase_with_underscores =
                            ParseContextAllocateStringCopyUppercaseWithUnderscores(context, enum_info.name);
                        enum_info.name_lower_camel_case =
                            ParseContextAllocateStringCopyLowerCamelCase(context, enum_info.name);
                        enum_info.name_upper_camel_case =
                            ParseContextAllocateStringCopyUpperCamelCase(context, enum_info.name);
                        enum_info.name_with_spaces =
                            ParseContextAllocateStringCopyWithSpaces(context, enum_info.name);
                        enum_info.root = root;
                    }
                    custom.EnumCallback(enum_info, filename);
                }
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_flags_declaration:
            {
                if(custom.FlagsCallback)
                {
                    DataDeskFlags flags_info = {0};
                    {
                        flags_info.name = root->string;
                        flags_info.name_lowercase_with_underscores =
                            ParseContextAllocateStringCopyLowercaseWithUnderscores(context, flags_info.name);
                        flags_info.name_uppercase_with_underscores =
                            ParseContextAllocateStringCopyUppercaseWithUnderscores(context, flags_info.name);
                        flags_info.name_lower_camel_case =
                            ParseContextAllocateStringCopyLowerCamelCase(context, flags_info.name);
                        flags_info.name_upper_camel_case =
                            ParseContextAllocateStringCopyUpperCamelCase(context, flags_info.name);
                        flags_info.name_with_spaces =
                            ParseContextAllocateStringCopyWithSpaces(context, flags_info.name);
                        flags_info.root = root;
                    }
                    custom.FlagsCallback(flags_info, filename);
                }
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_declaration:
            {
                if(custom.DeclarationCallback)
                {
                    DataDeskDeclaration decl_info = {0};
                    {
                        decl_info.name = root->string;
                        decl_info.name_lowercase_with_underscores =
                            ParseContextAllocateStringCopyLowercaseWithUnderscores(context, decl_info.name);
                        decl_info.name_uppercase_with_underscores =
                            ParseContextAllocateStringCopyUppercaseWithUnderscores(context, decl_info.name);
                        decl_info.name_lower_camel_case =
                            ParseContextAllocateStringCopyLowerCamelCase(context, decl_info.name);
                        decl_info.name_upper_camel_case =
                            ParseContextAllocateStringCopyUpperCamelCase(context, decl_info.name);
                        decl_info.name_with_spaces =
                            ParseContextAllocateStringCopyWithSpaces(context, decl_info.name);
                        decl_info.root = root;
                    }
                    custom.DeclarationCallback(decl_info, filename);
                }
                
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_constant_definition:
            {
                if(custom.ConstantCallback)
                {
                    DataDeskConstant const_info = {0};
                    {
                        const_info.name = root->string;
                        const_info.name_lowercase_with_underscores =
                            ParseContextAllocateStringCopyLowercaseWithUnderscores(context, const_info.name);
                        const_info.name_uppercase_with_underscores =
                            ParseContextAllocateStringCopyUppercaseWithUnderscores(context, const_info.name);
                        const_info.name_lower_camel_case =
                            ParseContextAllocateStringCopyLowerCamelCase(context, const_info.name);
                        const_info.name_upper_camel_case =
                            ParseContextAllocateStringCopyUpperCamelCase(context, const_info.name);
                        const_info.name_with_spaces =
                            ParseContextAllocateStringCopyWithSpaces(context, const_info.name);
                        const_info.root = root;
                    }
                    custom.ConstantCallback(const_info, filename);
                }
                break;
            }
            
            case DATA_DESK_AST_NODE_TYPE_procedure_header:
            {
                if(custom.ProcedureHeaderCallback)
                {
                    DataDeskProcedureHeader proc = {0};
                    {
                        proc.name = root->string;
                        proc.name_lowercase_with_underscores = ParseContextAllocateStringCopyLowercaseWithUnderscores(context, proc.name);
                        proc.name_uppercase_with_underscores = ParseContextAllocateStringCopyUppercaseWithUnderscores(context, proc.name);
                        proc.name_lower_camel_case = ParseContextAllocateStringCopyLowerCamelCase(context, proc.name);
                        proc.name_upper_camel_case = ParseContextAllocateStringCopyUpperCamelCase(context, proc.name);
                        proc.name_with_spaces = ParseContextAllocateStringCopyWithSpaces(context, proc.name);
                        proc.root = root;
                    }
                    custom.ProcedureHeaderCallback(proc, filename);
                }
                break;
            }
            
            default: break;
        }
        
        if(root->next)
        {
            TraverseASTAndCallCustomParseCallbacks(context, root->next, custom, filename);
        }
    }
}

static void
PatchASTSymbols(ParseContext *context, ASTNode *root)
{
    for(ASTNode *node = root; node; node = node->next)
    {
        switch(node->type)
        {
            case DATA_DESK_AST_NODE_TYPE_identifier:
            {
                node->identifier.declaration = ParseContextLookUpSymbol(context, node->string, node->string_length);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_binary_operator:
            {
                PatchASTSymbols(context, node->binary_operator.left);
                PatchASTSymbols(context, node->binary_operator.right);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_struct_declaration:
            {
                PatchASTSymbols(context, node->struct_declaration.first_member);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_union_declaration:
            {
                PatchASTSymbols(context, node->union_declaration.first_member);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_declaration:
            {
                PatchASTSymbols(context, node->declaration.type);
                PatchASTSymbols(context, node->declaration.initialization);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_type_usage:
            {
                if(!node->type_usage.struct_declaration && !node->type_usage.union_declaration)
                {
                    node->type_usage.type_definition = ParseContextLookUpSymbol(context, node->string, node->string_length);
                }
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_tag:
            {
                PatchASTSymbols(context, node->tag.first_tag_parameter);
                break;
            }
            case DATA_DESK_AST_NODE_TYPE_procedure_header:
            {
                PatchASTSymbols(context, node->procedure_header.return_type);
                PatchASTSymbols(context, node->procedure_header.first_parameter);
                break;
            }
            default: break;
        }
        
        if(node->first_tag)
        {
            PatchASTSymbols(context, node->first_tag);
        }
    }
}

/*
Copyright 2019 Ryan Fleury

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/