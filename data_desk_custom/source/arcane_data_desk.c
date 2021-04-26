#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_desk.h"
#include "../../game/source/telescope/telescope.h"

global FILE *global_catchall_header;
global FILE *global_catchall_implementation;

global char storage[Megabytes(64)];
global MemoryArena parse_arena;

global DataDeskNode global_data_desk_nodes[512];
global i32 global_data_desk_node_count = 0;
internal DataDeskNode *AllocNode(DataDeskNodeType node_type, char *string)
{
	DataDeskNode *node = &global_data_desk_nodes[global_data_desk_node_count++];
	memset(node, 0, sizeof(DataDeskNode));
	node->type = node_type;
	node->string = MemoryArenaAllocateCStringCopy(&parse_arena, string);
	return node;
}

/*
global i32 component_nodes_count = 0;
global DataDeskNode *component_nodes[1024];
internal void GenerateComponentCode(void);
 */

global i32 xmacro_count = 0;
global DataDeskNode *xmacro_nodes[128];
internal DataDeskNode *GetXMacroNode(const char *name);

internal void GeneratePrintUICodeForAST(FILE *file, DataDeskNode *root, char *access_string);

global i32 serialisable_struct_count = 0;
global DataDeskNode *serialisable_structs[128];
internal void GenerateSerialisationCode();

global i32 version_struct_count = 0;
global DataDeskNode *version_structs[128];
internal void GenerateVersionCode();

DATA_DESK_FUNC void
DataDeskCustomInitCallback(void)
{
	parse_arena = MemoryArenaInit(storage, sizeof(storage));
	
	global_catchall_header = fopen("catchall.h", "w");
	global_catchall_implementation = fopen("catchall.c", "w");
}

DATA_DESK_FUNC void
DataDeskCustomParseCallback(DataDeskNode *root, char *filename)
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (h_file && c_file)
	{
		if (DataDeskNodeHasTag(root, "ForwardDeclare"))
		{
			DataDeskNode *tag = DataDeskGetNodeTag(root, "ForwardDeclare");
			fprintf(h_file, "typedef struct %s %s;\n\n", DataDeskGetTagParameter(tag, 0)->name, DataDeskGetTagParameter(tag, 0)->name);
		}
		else if (DataDeskNodeHasTag(root, "ForwardDeclareEnum"))
		{
			DataDeskNode *tag = DataDeskGetNodeTag(root, "ForwardDeclareEnum");
			fprintf(h_file, "typedef enum %s %s;\n\n", DataDeskGetTagParameter(tag, 0)->name, DataDeskGetTagParameter(tag, 0)->name);
		}
		else
		{
			switch (root->type)
			{
				case DATA_DESK_NODE_TYPE_struct_declaration:
				{
					fprintf(h_file, "typedef struct %s\n", root->string);
					fprintf(h_file, "{\n");
					
					for (DataDeskNode *member = root->struct_declaration.first_member;
						 member; member = member->next)
					{
						DataDeskFWriteGraphAsC(h_file, member, 0);
						fprintf(h_file, ";\n");
					}
					fprintf(h_file, "} %s;\n\n", root->string);
					
					if (DataDeskNodeHasTag(root, "XMacro"))
					{
						xmacro_nodes[xmacro_count++] = root;
					}
					
					if (DataDeskNodeHasTag(root, "SerialisableStruct"))
					{
						serialisable_structs[serialisable_struct_count++] = root;
					}
					
					if (DataDeskNodeHasTag(root, "Version"))
					{
						version_structs[version_struct_count++] = root;
					}
					
					break;
				}
				
				case DATA_DESK_NODE_TYPE_union_declaration:
				{
					fprintf(h_file, "typedef union %s\n", root->string);
					fprintf(h_file, "{\n");
					for (DataDeskNode *member = root->union_declaration.first_member;
						 member; member = member->next)
					{
						DataDeskFWriteGraphAsC(h_file, member, 0);
						fprintf(h_file, ";\n");
					}
					fprintf(h_file, "} %s;\n\n", root->string);
					
					break;
				}
				
				case DATA_DESK_NODE_TYPE_enum_declaration:
				{
					// MOTE(tjr): Generate styled enum.
					{
						fprintf(h_file, "typedef enum %s %s;\n", root->name, root->name);
						//DataDeskFWriteEnumAsC(global_catchall_header, enum_info);
						fprintf(h_file, "enum %s\n", root->name);
						fprintf(h_file, "{\n");
						for (DataDeskNode *field = root->enum_declaration.first_constant; field; field = field->next)
						{
							fprintf(h_file, "%s_%s,\n", root->name_uppercase_with_underscores, field->string);
						}
						fprintf(h_file, "%s_MAX,\n", root->name_uppercase_with_underscores);
						fprintf(h_file, "};\n");
					}
					
					// NOTE(randy): X-Macro initialisation
					if (DataDeskNodeHasTag(root, "XMacro"))
					{
						char name[50];
						sprintf(name, "%sData", root->name);
						DataDeskNode *macro_data_node = GetXMacroNode(name);
						if (macro_data_node)
						{
							i32 string_length = 0;
							for (; macro_data_node->name_uppercase_with_underscores[string_length]; ++string_length)
							{
							}
							
							char trimmed_name[50];
							strncpy(trimmed_name, macro_data_node->name_uppercase_with_underscores, string_length - 5);
							trimmed_name[string_length - 5] = '\0';
							
							fprintf(h_file, "global %s global_%s[%s_MAX] = {\n", macro_data_node->name, macro_data_node->name_lowercase_with_underscores, trimmed_name);
							for (DataDeskNode *enum_field = root->enum_declaration.first_constant; enum_field; enum_field = enum_field->next)
							{
								DataDeskNode *data_tag = DataDeskGetNodeTag(enum_field, "Data");
								
								fprintf(h_file, "    { ");
								i32 i = 0;
								for (DataDeskNode *member = macro_data_node->struct_declaration.first_member; member; member = member->next)
								{
									DataDeskNode *ignore_tag = DataDeskGetNodeTag(member, "Ignore");
									if (!ignore_tag)
									{
										DataDeskNode *tag_param = DataDeskGetTagParameter(data_tag, i);
										
										for (i32 i = 0; tag_param->name[i]; i++)
										{
											if (tag_param->name[i] == '\'')
												fprintf(h_file, "\"");
											else if (tag_param->name[i] != '"')
												fprintf(h_file, "%c", tag_param->name[i]);
										}
										fprintf(h_file, ", ");
										
										i++;
									}
								}
								fprintf(h_file, "},\n");
							}
							fprintf(h_file, "};\n\n");
						}
					}
					
					// NOTE(randy): Generate enum print function implementation.
					{
						fprintf(h_file, "static char *Get%sName(%s type);\n\n", root->name, root->name);
						fprintf(c_file, "static char *Get%sName(%s type)\n", root->name, root->name);
						fprintf(c_file, "{\n");
						fprintf(c_file, "switch(type)\n");
						fprintf(c_file, "{\n");
						for (DataDeskNode *field = root->enum_declaration.first_constant; field; field = field->next)
						{
							fprintf(c_file, "case %s_%s:\n", root->name_uppercase_with_underscores, field->string);
							fprintf(c_file, "return \"");
							
							// NOTE(randy): Make enum name look pretty.
							i32 string_length = 0;
							for (; field->string[string_length]; ++string_length)
							{
							}
							
							for (i32 i = 0; i < string_length && field->string[i]; ++i)
							{
								if (field->string[i] != '_')
								{
									if (i == 0)
									{
										fprintf(c_file, "%c", DataDeskCharToUpper(field->string[i]));
									}
									else
									{
										fprintf(c_file, "%c", field->string[i]);
										if (field->string[i + 1] == '_')
										{
											fprintf(c_file, " ");
											fprintf(c_file, "%c", DataDeskCharToUpper(field->string[i + 2]));
											i = i + 2;
										}
									}
								}
							}
							
							fprintf(c_file, "\";\n");
							fprintf(c_file, "break;\n");
						}
						fprintf(c_file, "default:\n");
						fprintf(c_file, "return \"INVALID\";\n");
						fprintf(c_file, "break;\n");
						fprintf(c_file, "}\n");
						fprintf(c_file, "}\n\n");
					}
					
					break;
				}
				
				case DATA_DESK_NODE_TYPE_flags_declaration:
				{
					i32 count = 0;
					for (DataDeskNode *field = root->flags_declaration.first_flag; field; field = field->next)
					{
						fprintf(h_file, "#define %s_%s (1<<%i)\n", root->name_uppercase_with_underscores, field->string, count);
						count++;
					}
					fprintf(h_file, "typedef uint32 %s;\n\n", root->name);
					
					break;
				}
				
				default:
				DataDeskFWriteGraphAsC(h_file, root, 0);
				break;
			}
		}
	}
}

DATA_DESK_FUNC void
DataDeskCustomCleanUpCallback(void)
{
	GenerateSerialisationCode();
	GenerateVersionCode();
	
	fclose(global_catchall_header);
	fclose(global_catchall_implementation);
}

internal DataDeskNode *GetXMacroNode(const char *name)
{
	for (i32 i = 0; i < xmacro_count; i++)
	{
		if (strcmp(xmacro_nodes[i]->name, name) == 0)
		{
			return xmacro_nodes[i];
		}
	}
	
	return 0;
}

// todo: rework
internal void GeneratePrintUICodeForAST(FILE *file, DataDeskNode *root, char *access_string)
{
	if (!DataDeskNodeHasTag(root, "NoPrint"))
	{
		switch (root->type)
		{
			case DATA_DESK_NODE_TYPE_struct_declaration:
			{
				fprintf(file, "        char title[100];\n");
				fprintf(file, "        sprintf(title, \"%s #%%i\", component->component_id);\n", root->string);
				fprintf(file, "        if (TsUICollapsable(title))");
				fprintf(file, "        {\n");
				for (DataDeskNode *field = root->struct_declaration.first_member; field; field = field->next)
				{
					GeneratePrintUICodeForAST(file, field, access_string);
				}
				fprintf(file, "\n            TsUICollapsableEnd();\n");
				fprintf(file, "        }\n");
				break;
			}
			
			case DATA_DESK_NODE_TYPE_declaration:
			{
				if (DataDeskDeclarationIsType(root, "i32") ||
					DataDeskDeclarationIsType(root, "i32") || DataDeskDeclarationIsType(root, "u32") ||
					DataDeskDeclarationIsType(root, "i16") || DataDeskDeclarationIsType(root, "u16") ||
					DataDeskDeclarationIsType(root, "i8") || DataDeskDeclarationIsType(root, "u8"))
				{
					DataDeskNode *editable_tag = DataDeskGetNodeTag(root, "Editable");
					if (editable_tag)
					{
						DataDeskNode *param1 = DataDeskGetTagParameter(editable_tag, 0);
						DataDeskNode *param2 = DataDeskGetTagParameter(editable_tag, 1);
						
						fprintf(file, "            ");
						fprintf(file, "%s%s = TsUIIntSlider(\"%s\", %s%s, %s, %s);\n",
								access_string, root->string,
								root->string,
								access_string, root->string,
								param1->string,
								param2->string);
					}
					else
					{
						fprintf(file, "            ");
						fprintf(file, "TsUIPushAutoWidth();\n");
						fprintf(file, "            ");
						fprintf(file, "{ char label[100]; ");
						fprintf(file, "sprintf(label, \"%s: %%i\", %s%s); ", root->string, access_string, root->string);
						fprintf(file, "TsUILabel(label); }\n");
						fprintf(file, "            ");
						fprintf(file, "TsUIPopWidth();\n");
					}
				}
				else if (DataDeskDeclarationIsType(root, "float") || DataDeskDeclarationIsType(root, "f32") ||
						 DataDeskDeclarationIsType(root, "f64"))
				{
					DataDeskNode *editable_tag = DataDeskGetNodeTag(root, "Editable");
					if (editable_tag)
					{
						DataDeskNode *param1 = DataDeskGetTagParameter(editable_tag, 0);
						DataDeskNode *param2 = DataDeskGetTagParameter(editable_tag, 1);
						
						fprintf(file, "            ");
						fprintf(file, "%s%s = TsUISlider(\"%s\", %s%s, %s, %s);\n",
								access_string, root->string,
								root->string,
								access_string, root->string,
								param1->string,
								param2->string);
					}
					else
					{
						fprintf(file, "            ");
						fprintf(file, "TsUIPushAutoWidth();\n");
						fprintf(file, "            ");
						fprintf(file, "{ char label[100]; ");
						fprintf(file, "sprintf(label, \"%s: %%f\", %s%s); ", root->string, access_string, root->string);
						fprintf(file, "TsUILabel(label); }\n");
						fprintf(file, "            ");
						fprintf(file, "TsUIPopWidth();\n");
					}
				}
				else if (DataDeskDeclarationIsType(root, "b32") || DataDeskDeclarationIsType(root, "b16") ||
						 DataDeskDeclarationIsType(root, "b8"))
				{
					DataDeskNode *editable_tag = DataDeskGetNodeTag(root, "Editable");
					if (editable_tag)
					{
						fprintf(file, "            ");
						fprintf(file, "%s%s = TsUIToggler(\"%s\", %s%s);\n",
								access_string, root->string,
								root->string,
								access_string, root->string);
					}
					else
					{
						fprintf(file, "            ");
						fprintf(file, "TsUIPushAutoWidth();\n");
						fprintf(file, "            ");
						fprintf(file, "{ char label[100]; ");
						fprintf(file, "sprintf(label, %s%s ? \"%s: true\" : \"%s: false\"); ", access_string, root->string, root->string, root->string);
						fprintf(file, "TsUILabel(label); }\n");
						fprintf(file, "            ");
						fprintf(file, "TsUIPopWidth();\n");
					}
				}
				else if (DataDeskDeclarationIsType(root, "v2"))
				{
					/* DataDeskNode *editable_tag = DataDeskGetNodeTag(root, "Editable");
						if (editable_tag)
						{
						   DataDeskNode *param1 = DataDeskGetTagParameter(editable_tag, 0);
						   DataDeskNode *param2 = DataDeskGetTagParameter(editable_tag, 1);
			
						   fprintf(file, "            ");
						   fprintf(file, "%s%s = TsUISlider(\"%s\", %s%s, %s, %s);\n",
								 access_string, root->string,
								 root->string,
								 access_string, root->string,
								 param1->string,
								 param2->string);
						} */
					
					fprintf(file, "            ");
					fprintf(file, "TsUIPushAutoWidth();\n");
					fprintf(file, "            ");
					fprintf(file, "{ char label[100]; ");
					fprintf(file, "sprintf(label, \"%s: %%f, %%f\", %s%s.x, %s%s.y); ", root->string, access_string, root->string, access_string, root->string);
					fprintf(file, "TsUILabel(label); }\n");
					fprintf(file, "            ");
					fprintf(file, "TsUIPopWidth();\n");
				}
				else if (DataDeskDeclarationIsType(root, "*char"))
				{
					fprintf(file, "            ");
					fprintf(file, "TsUIPushAutoWidth();\n");
					fprintf(file, "            ");
					fprintf(file, "{ char label[100]; ");
					fprintf(file, "sprintf(label, \"%s: %%s\", %s%s); ", root->string, access_string, root->string);
					fprintf(file, "TsUILabel(label); }\n");
					fprintf(file, "            ");
					fprintf(file, "TsUIPopWidth();\n");
				}
				else
				{
					fprintf(file, "            ");
					fprintf(file, "// TODO: Don't know how to generate UI print for variable '%s'\n", root->string);
				}
				
				break;
			}
			
			default:
			break;
		}
	}
}

internal void WriteMemberToFile(FILE *file, DataDeskNode *node, char *access_string)
{
	if (!DataDeskNodeHasTag(node, "DoNotSerialise"))
	{
		if (node->type != DATA_DESK_NODE_TYPE_declaration || node->declaration.type->type != DATA_DESK_NODE_TYPE_type_usage)
			fprintf(file, "Node is not a member declaration.");
		
		b8 is_complex = 0;
		for (i32 j = 0; j < serialisable_struct_count; j++)
		{
			DataDeskNode *complex_struct = serialisable_structs[j];
			if (strcmp(complex_struct->name, node->declaration.type->name) == 0)
			{
				is_complex = 1;
				break;
			}
		}
		
		if (node->declaration.type->type_usage.first_array_size_expression) // Array type
		{
			if (!(node->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
				  node->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant ||
				  node->declaration.type->type_usage.first_array_size_expression->next))
				fprintf(file, "\nArray doesn't have proper type expressions?\n");
			
			fprintf(file, "    for (i32 i = 0; i < %s; i++)\n", node->declaration.type->type_usage.first_array_size_expression->string);
			fprintf(file, "    {\n");
			
			if (is_complex) // NOTE(randy)/ Array of complex types
			{
				fprintf(file, "        Write%sToFile(file, &(%s%s[i]));\n", node->declaration.type->name, access_string, node->name);
			}
			else // NOTE(randy) Array of primative types
			{
				fprintf(file, "        WriteToFile(file, &%s%s[i], sizeof(%s));\n", access_string, node->name, node->declaration.type->name);
			}
			fprintf(file, "    }\n\n");
		}
		else // NOTE(randy) Singular data type
		{
			if (is_complex)
			{
				fprintf(file, "    Write%sToFile(file, &%s%s);\n\n", node->declaration.type->name, access_string, node->name);
			}
			else
			{
				fprintf(file, "    WriteToFile(file, &%s%s, sizeof(%s%s));\n\n", access_string, node->name, access_string, node->name);
			}
		}
	}
}

internal void ReadMemberFromFile(FILE *file, DataDeskNode *node, char *access_string)
{
	if (!DataDeskNodeHasTag(node, "DoNotSerialise"))
	{
		if (node->type != DATA_DESK_NODE_TYPE_declaration || node->declaration.type->type != DATA_DESK_NODE_TYPE_type_usage)
			fprintf(file, "Node is not a member declaration.");
		
		b8 is_complex = 0;
		for (i32 j = 0; j < serialisable_struct_count; j++)
		{
			DataDeskNode *complex_struct = serialisable_structs[j];
			if (strcmp(complex_struct->name, node->declaration.type->name) == 0)
			{
				is_complex = 1;
				break;
			}
		}
		
		if (node->declaration.type->type_usage.first_array_size_expression) // Array type
		{
			if (!(node->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
				  node->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant ||
				  node->declaration.type->type_usage.first_array_size_expression->next))
				fprintf(file, "\nArray doesn't have proper type expressions?\n");
			
			
			fprintf(file, "    for (i32 i = 0; i < %s; i++)\n", node->declaration.type->type_usage.first_array_size_expression->string);
			fprintf(file, "    {\n");
			
			if (is_complex) // Array of complex types
			{
				fprintf(file, "        Read%sFromFile(file, &(%s%s[i]));\n", node->declaration.type->name, access_string, node->name);
			}
			else // Array of primative types
			{
				fprintf(file, "        ReadFromFile(file, &%s%s[i], sizeof(%s));\n", access_string, node->name, node->declaration.type->name);
			}
			fprintf(file, "    }\n\n");
		}
		else // Singular data type
		{
			if (is_complex)
			{
				fprintf(file, "    Read%sFromFile(file, &%s%s);\n\n", node->declaration.type->name, access_string, node->name);
			}
			else
			{
				
				fprintf(file, "    ReadFromFile(file, &%s%s, sizeof(%s%s));\n\n", access_string, node->name, access_string, node->name);
			}
		}
	}
}

internal DataDeskNode *FindStructVersion(i32 version, char *struct_name)
{
	for (i32 i = 0; i < version_struct_count; i++)
	{
		DataDeskNode *candidate_node = version_structs[i];
		DataDeskNode *candidate_version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(candidate_node, "Version"), 0);
		i32 candidate_version_number = DataDeskInterpretNumericExpressionAsInteger(candidate_version_param);
		
		char candidate_trimmed_name[200] = "";
		if (DataDeskGetNodeTag(candidate_node, "SerialisableStruct"))
		{
			sprintf(candidate_trimmed_name, "%s", candidate_node->name);
		}
		else
		{
			char *candidate_found = strstr(candidate_node->name, "_Version");
			if (candidate_found)
			{
				strncpy(candidate_trimmed_name, candidate_node->name, candidate_found - candidate_node->name);
			}
		}
		
		if (strcmp(struct_name, candidate_trimmed_name) == 0 && version == candidate_version_number)
		{
			return candidate_node;
		}
	}
	
	return 0;
}

internal i32 FindStartStructVersion(char *struct_name)
{
	i32 lowest = -1;
	for (i32 i = 0; i < version_struct_count; i++)
	{
		DataDeskNode *candidate_node = version_structs[i];
		DataDeskNode *candidate_version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(candidate_node, "Version"), 0);
		i32 candidate_version_number = DataDeskInterpretNumericExpressionAsInteger(candidate_version_param);
		
		char candidate_trimmed_name[200] = "";
		if (DataDeskGetNodeTag(candidate_node, "SerialisableStruct"))
		{
			sprintf(candidate_trimmed_name, "%s", candidate_node->name);
		}
		else
		{
			char *candidate_found = strstr(candidate_node->name, "_Version");
			if (candidate_found)
			{
				strncpy(candidate_trimmed_name, candidate_node->name, candidate_found - candidate_node->name);
			}
		}
		
		if (strcmp(struct_name, candidate_trimmed_name) == 0 && (lowest == -1 || candidate_version_number < lowest))
		{
			lowest = candidate_version_number;
		}
	}
	
	return lowest;
}

internal void GenerateSerialisationCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (!h_file || !c_file)
		return;
	
	for (i32 i = 0; i < serialisable_struct_count; i++)
	{
		DataDeskNode *root = serialisable_structs[i];
		DataDeskNode *version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(root, "SerialisableStruct"), 0);
		i32 max_version = DataDeskInterpretNumericExpressionAsInteger(version_param);
		
		// NOTE(randy): Write to file
		{
			fprintf(h_file, "static void Write%sToFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Write%sToFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    i32 version = %i;\n", max_version);
			fprintf(c_file, "    WriteToFile(file, &version, sizeof(i32));\n");
			fprintf(c_file, "    Write%s_Version%iToFile(file, data);\n", root->name, max_version);
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Read from file
		{
			fprintf(h_file, "static void Read%sFromFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Read%sFromFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    i32 actual_version = -1;\n");
			fprintf(c_file, "    ReadFromFile(file, &actual_version, sizeof(i32));\n");
			
			fprintf(c_file, "    if (actual_version == %i)\n", max_version);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        Read%s_Version%iFromFile(file, data);\n", root->name, max_version);
			fprintf(c_file, "        return;\n");
			fprintf(c_file, "    }\n\n");
			
			fprintf(c_file, "    switch (actual_version)\n");
			fprintf(c_file, "    {\n");
			i32 start_version = FindStartStructVersion(root->name);
			for (start_version; start_version < max_version; start_version++)
			{
				char *struct_name = root->name_lowercase_with_underscores;
				DataDeskNode *start_version_node = FindStructVersion(start_version, root->name);
				
				fprintf(c_file, "    case %i:\n", start_version);
				fprintf(c_file, "    {\n");
				
				fprintf(c_file, "        %s %s%i = {0};\n", start_version_node->name, struct_name, start_version);
				fprintf(c_file, "        Read%sFromFile(file, &%s%i);\n\n", start_version_node->name, struct_name, start_version);
				
				for (i32 version = start_version; version < max_version; version++)
				{
					DataDeskNode *version_node = FindStructVersion(version, root->name);
					DataDeskNode *next_version_node = FindStructVersion(version + 1, root->name);
					
					fprintf(c_file, "        %s %s%i = {0};\n", next_version_node->name, struct_name, version + 1);
					fprintf(c_file, "        Map%sTo%s(%s%i, &%s%i);\n\n", version_node->name, next_version_node->name, struct_name, version, struct_name, version + 1);
				}
				
				
				fprintf(c_file, "        memcpy(data, &%s%i, sizeof(*data));\n", struct_name, max_version);
				
				fprintf(c_file, "    } break;\n");
			}
			fprintf(c_file, "    }\n");
			fprintf(c_file, "}\n\n");
		}
	}
}

internal DataDeskNode *GetNextVersion(DataDeskNode *node)
{
	DataDeskNode *version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(node, "Version"), 0);
	i32 version_number = DataDeskInterpretNumericExpressionAsInteger(version_param);
	
	char trimmed_name[200] = "";
	char *found = strstr(node->name, "_Version");
	if (found)
	{
		strncpy(trimmed_name, node->name, found - node->name);
	}
	
	for (i32 i = 0; i < version_struct_count; i++)
	{
		DataDeskNode *candidate_root = version_structs[i];
		if (candidate_root == node)
			continue;
		DataDeskNode *candidate_version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(candidate_root, "Version"), 0);
		i32 candidate_version_number = DataDeskInterpretNumericExpressionAsInteger(candidate_version_param);
		
		char candidate_trimmed_name[200] = "";
		if (DataDeskGetNodeTag(candidate_root, "SerialisableStruct"))
		{
			sprintf(candidate_trimmed_name, "%s", candidate_root->name);
		}
		else
		{
			char *candidate_found = strstr(candidate_root->name, "_Version");
			if (candidate_found)
			{
				strncpy(candidate_trimmed_name, candidate_root->name, candidate_found - candidate_root->name);
			}
		}
		
		if (strcmp(trimmed_name, candidate_trimmed_name) == 0 && version_number == candidate_version_number - 1)
		{
			return candidate_root;
		}
	}
	
	return 0;
}

internal DataDeskNode *FindMemberInStruct(DataDeskNode *node, char *member_name)
{
	for (DataDeskNode *member = node->struct_declaration.first_member;
		 member; member = member->next)
	{
		if (strcmp(member->name, member_name) == 0)
		{
			return member;
		}
	}
	
	return 0;
}

internal void GenerateVersionCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (!h_file || !c_file)
		return;
	
	for (i32 i = 0; i < version_struct_count; i++)
	{
		DataDeskNode *root = version_structs[i];
		DataDeskNode *version_param = DataDeskGetTagParameter(DataDeskGetNodeTag(root, "Version"), 0);
		i32 version_number = DataDeskInterpretNumericExpressionAsInteger(version_param);
		
		char struct_name[100] = "";
		strcpy(struct_name, root->name);
		
		if (DataDeskGetNodeTag(root, "SerialisableStruct"))
		{
			sprintf(struct_name, "%s_Version%i", struct_name, version_number);
		}
		
		// NOTE(randy): Write to file
		fprintf(h_file, "static void Write%sToFile(FILE *file, %s *data);\n\n", struct_name, root->name);
		fprintf(c_file, "static void Write%sToFile(FILE *file, %s *data)\n", struct_name, root->name);
		fprintf(c_file, "{\n");
		for (DataDeskNode *member = root->struct_declaration.first_member;
			 member; member = member->next)
		{
			WriteMemberToFile(c_file, member, "data->");
		}
		fprintf(c_file, "}\n\n");
		
		// NOTE(randy): Read from file
		{
			fprintf(h_file, "static void Read%sFromFile(FILE *file, %s *data);\n\n", struct_name, root->name);
			fprintf(c_file, "static void Read%sFromFile(FILE *file, %s *data)\n", struct_name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				ReadMemberFromFile(c_file, member, "data->");
			}
			fprintf(c_file, "}\n\n");
		}
		
		DataDeskNode *next_version = GetNextVersion(root);
		if (next_version)
		{
			// generate function for version->next_version
			fprintf(h_file, "static void Map%sTo%s(%s origin, %s *dest);\n\n", struct_name, next_version->name, root->name, next_version->name);
			fprintf(c_file, "static void Map%sTo%s(%s origin, %s *dest)\n", struct_name, next_version->name, root->name, next_version->name);
			fprintf(c_file, "{\n");
			
			for (DataDeskNode *member = next_version->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (DataDeskGetNodeTag(member, "DoNotSerialise"))
					continue;
				
				DataDeskNode *new_value = DataDeskGetTagParameter(DataDeskGetNodeTag(member, "NewValue"), 0);
				DataDeskNode *map_from = DataDeskGetTagParameter(DataDeskGetNodeTag(member, "MapFrom"), 0);
				
				if (map_from)
				{
					if (new_value)
					{
						// TODO(randy)
					}
					else
					{
						fprintf(c_file, "MemoryCopy(&dest->%s, &origin.%s, sizeof(origin.%s));\n", member->name, map_from->name, map_from->name);
					}
				}
				else
				{
					if (new_value)
					{
						// NOTE(randy): New member
						fprintf(c_file, "    dest->%s = %s;\n", member->name, new_value->name);
					}
					else
					{
						// NOTE(randy): Just assume this member hasn't changed
						if (member->declaration.type->type_usage.first_array_size_expression)
						{
							fprintf(c_file, "    MemoryCopy(dest->%s, origin.%s, sizeof(origin.%s));\n", member->name, member->name, member->name);
						}
						else
						{
							fprintf(c_file, "    MemoryCopy(&dest->%s, &origin.%s, sizeof(origin.%s));\n", member->name, member->name, member->name);
						}
					}
				}
			}
			
			fprintf(c_file, "}\n\n");
		}
	}
}