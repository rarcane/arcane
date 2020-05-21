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

global i32 component_nodes_count = 0;
global DataDeskNode *component_nodes[1024];
internal void GenerateComponentCode(void);

global i32 xmacro_count = 0;
global DataDeskNode *xmacro_nodes[128];
internal DataDeskNode *GetXMacroNode(const char *name);

internal void GeneratePrintUICodeForAST(FILE *file, DataDeskNode *root, char *access_string);

global i32 serialisable_struct_count = 0;
global DataDeskNode *serialisable_structs[128];
internal void GenerateSerialisationCode();

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
		if (DataDeskNodeHasTag(root, "GenerateComponentCode"))
		{
			GenerateComponentCode();
		}
		else if (DataDeskNodeHasTag(root, "ForwardDeclare"))
		{
			DataDeskNode *tag = DataDeskGetNodeTag(root, "ForwardDeclare");
			fprintf(h_file, "typedef struct %s %s;\n\n", DataDeskGetTagParameter(tag, 0)->name, DataDeskGetTagParameter(tag, 0)->name);
		}
		else
		{
			switch (root->type)
			{
				case DATA_DESK_NODE_TYPE_struct_declaration:
				{
					fprintf(h_file, "typedef struct %s\n", root->string);
					fprintf(h_file, "{\n");
					
					// DataDeskNode *original_member = root->struct_declaration.first_member;
					if (DataDeskNodeHasTag(root, "Component"))
					{
						fprintf(h_file, "i32 parent_entity_id;\n");
						/* DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "parent_entity_id");
						node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
						root->struct_declaration.first_member = node_1; */
						
						fprintf(h_file, "i32 component_id;\n");
						/* DataDeskNode *node_2 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "component_id");
						node_2->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
						node_1->next = node_2;
						node_2->next = original_member; */
						
						component_nodes[component_nodes_count++] = root;
					}
					
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
					else if (DataDeskNodeHasTag(root, "SerialisableStruct"))
					{
						serialisable_structs[serialisable_struct_count++] = root;
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
							
							fprintf(h_file, "global %s %s[%s_MAX] = {\n", macro_data_node->name, macro_data_node->name_lowercase_with_underscores, trimmed_name);
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
	
	fclose(global_catchall_header);
	fclose(global_catchall_implementation);
}

internal void GenerateComponentCode(void)
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (!h_file || !c_file)
		return;
	
	// NOTE(rjf): Generate component enums
	{
		fprintf(h_file, "typedef enum ComponentType\n");
		fprintf(h_file, "{\n");
		fprintf(h_file, "COMPONENT_INVALID,\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(h_file, "COMPONENT_%s,\n", trimmed_lowercase_name);
		}
		fprintf(h_file, "COMPONENT_MAX,\n");
		fprintf(h_file, "} ComponentType;\n\n");
	}
	
	// NOTE(randy): Generate component set structure.
	{
		DataDeskNode *struct_node = AllocNode(DATA_DESK_NODE_TYPE_struct_declaration, "ComponentSet");
		
		fprintf(h_file, "typedef struct ComponentSet\n");
		fprintf(h_file, "{\n");
		DataDeskNode *previous_node = 0;
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			char variable_1_name[100];
			sprintf(variable_1_name, "%ss", comp_node->name_lowercase_with_underscores);
			fprintf(h_file, "%s %s[MAX_ENTITIES];\n",
					comp_node->name,
					variable_1_name);
			DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_1_name);
			node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, comp_node->name);
			node_1->declaration.type->type_usage.first_array_size_expression = AllocNode(DATA_DESK_NODE_TYPE_identifier, "MAX_ENTITIES");
			if (!previous_node)
				struct_node->struct_declaration.first_member = node_1;
			else
				previous_node->next = node_1;
			
			char variable_2_name[100];
			sprintf(variable_2_name, "%s_count", comp_node->name_lowercase_with_underscores);
			fprintf(h_file, "i32 %s;\n", variable_2_name);
			DataDeskNode *node_2 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_2_name);
			node_2->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
			node_1->next = node_2;
			
			char variable_3_name[100];
			sprintf(variable_3_name, "free_%s_id", comp_node->name_lowercase_with_underscores);
			fprintf(h_file, "i32 %s;\n", variable_3_name);
			DataDeskNode *node_3 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_3_name);
			node_3->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
			node_2->next = node_3;
			
			previous_node = node_3;
		}
		fprintf(h_file, "} ComponentSet;\n\n");
		
		serialisable_structs[serialisable_struct_count++] = struct_node;
	}
	
	// NOTE(randy): Component add/remove functions, and helpers
	for (i32 i = 0; i < component_nodes_count; i++)
	{
		DataDeskNode *comp_node = component_nodes[i];
		
		i32 string_length = 0;
		for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
		{
		}
		char trimmed_lowercase_name[50];
		strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
		trimmed_lowercase_name[string_length - 10] = '\0';
		
		// NOTE(randy): Add Component function.
		{
			fprintf(c_file, "internal %s *Add%s(Entity *entity)\n", comp_node->name, comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    Assert(core->run_data->entity_components.free_%s_id > 0);\n", comp_node->name_lowercase_with_underscores, comp_node->name);
			fprintf(c_file, "    Assert(entity->component_ids[COMPONENT_%s] == 0);\n", trimmed_lowercase_name, comp_node->name);
			fprintf(c_file, "    i32 new_comp_id = core->run_data->entity_components.free_%s_id;\n\n", comp_node->name_lowercase_with_underscores);
			
			fprintf(c_file, "    %s *comp = &core->run_data->entity_components.%ss[new_comp_id - 1];\n", comp_node->name, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    *comp = GetDefault%s();\n", comp_node->name);
			fprintf(c_file, "    comp->parent_entity_id = entity->entity_id;\n");
			fprintf(c_file, "    comp->component_id = new_comp_id;\n");
			fprintf(c_file, "    entity->component_ids[COMPONENT_%s] = new_comp_id;\n\n", trimmed_lowercase_name);
			
			fprintf(c_file, "    if (core->run_data->entity_components.%s_count == core->run_data->entity_components.free_%s_id - 1)\n", comp_node->name_lowercase_with_underscores, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        core->run_data->entity_components.%s_count++;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->run_data->entity_components.free_%s_id++;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    }\n\n");
			
			fprintf(c_file, "    if (core->run_data->entity_components.%s_count < MAX_ENTITIES)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        if (core->run_data->entity_components.%s_count != core->run_data->entity_components.free_%s_id - 1)\n", comp_node->name_lowercase_with_underscores, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            b8 found = 0;\n");
			fprintf(c_file, "            for (i32 i = 0; i < core->run_data->entity_components.%s_count + 1; i++)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                if (!core->run_data->entity_components.%ss[i].component_id)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "                {\n");
			fprintf(c_file, "                    core->run_data->entity_components.free_%s_id = i + 1;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "                    found = 1;\n");
			fprintf(c_file, "                    break;\n");
			fprintf(c_file, "                }\n");
			fprintf(c_file, "            }\n");
			fprintf(c_file, "            Assert(found);\n");
			fprintf(c_file, "        }\n");
			fprintf(c_file, "    }\n");
			fprintf(c_file, "    else\n");
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        core->run_data->entity_components.free_%s_id = 0;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    }\n\n");
			
			fprintf(c_file, "    return comp;\n");
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Remove Component function.
		{
			fprintf(c_file, "internal void Remove%s(Entity *entity)\n", comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    Assert(entity->component_ids[COMPONENT_%s] != 0);\n", trimmed_lowercase_name);
			fprintf(c_file, "    %s *comp = &core->run_data->entity_components.%ss[entity->component_ids[COMPONENT_%s] - 1];\n\n", comp_node->name, comp_node->name_lowercase_with_underscores, trimmed_lowercase_name);
			
			fprintf(c_file, "    if (comp->component_id < core->run_data->entity_components.free_%s_id)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->run_data->entity_components.free_%s_id = comp->component_id;\n\n", comp_node->name_lowercase_with_underscores);
			
			fprintf(c_file, "    %s empty_comp = {0};\n", comp_node->name);
			fprintf(c_file, "    *comp = empty_comp;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    entity->component_ids[COMPONENT_%s] = 0;\n", trimmed_lowercase_name);
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Get component helper function.
		{
			fprintf(h_file, "// NOTE(randy): Gets a %s from a specified entity, it must have one.\n", comp_node->name);
			fprintf(h_file, "internal %s *Get%sFromEntityID(i32 id);\n", comp_node->name, comp_node->name);
			fprintf(c_file, "internal %s *Get%sFromEntityID(i32 id)\n", comp_node->name, comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    Entity *entity = GetEntityWithID(id);\n");
			fprintf(c_file, "    Assert(entity->component_ids[COMPONENT_%s]);\n", trimmed_lowercase_name);
			fprintf(c_file, "    %s *comp = &core->run_data->entity_components.%ss[entity->component_ids[COMPONENT_%s] - 1];\n", comp_node->name, comp_node->name_lowercase_with_underscores, trimmed_lowercase_name);
			fprintf(c_file, "    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_%s]);\n", trimmed_lowercase_name);
			fprintf(c_file, "    return comp;\n");
			fprintf(c_file, "}\n\n");
		}
	}
	
	// NOTE(randy): Generic component removal
	{
		fprintf(h_file, "internal void RemoveComponent(Entity *entity, ComponentType type);\n");
		fprintf(c_file, "internal void RemoveComponent(Entity *entity, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "    case COMPONENT_%s:\n", trimmed_lowercase_name);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        if (entity->component_ids[COMPONENT_%s])\n", trimmed_lowercase_name);
			fprintf(c_file, "            Remove%s(entity);\n", comp_node->name);
			fprintf(c_file, "        else\n");
			fprintf(c_file, "            Assert(0);\n", comp_node->name);
			fprintf(c_file, "        break;\n");
			fprintf(c_file, "    }\n");
		}
		fprintf(c_file, "    default:\n");
		fprintf(c_file, "        Assert(0);\n");
		fprintf(c_file, "        break;\n");
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n");
	}
	
	// NOTE(randy): Component initialisation
	{
		fprintf(c_file, "internal void InitialiseComponents()\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			fprintf(c_file, "    core->run_data->entity_components.free_%s_id = 1;\n", comp_node->name_lowercase_with_underscores);
		}
		fprintf(c_file, "}\n");
	}
	
	// NOTE(randy): Generate Component UI print function.
	/* {
		fprintf(c_file, "internal void PrintComponentDataUI(void *component_data, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		fprintf(c_file, "    case COMPONENT_INVALID :\n");
		fprintf(c_file, "    case COMPONENT_MAX :\n");
		fprintf(c_file, "        R_BREAK(\"Invalid component.\")\n");
		fprintf(c_file, "        break;\n\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *component = component_nodes[i];

			i32 string_length = 0;
			for (; component->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, component->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';

			fprintf(c_file, "    case COMPONENT_%s :\n", trimmed_lowercase_name);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        %s *component = (%s*)component_data;\n", component->name, component->name);
			GeneratePrintUICodeForAST(c_file, component, "component->");
			fprintf(c_file, "        break;\n");
			fprintf(c_file, "    }\n\n");
		}
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n\n");
	} */
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
				  node->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
				fprintf(file, "\nArray doesn't have proper type expressions?\n");
			
			if (node->declaration.type->type_usage.first_array_size_expression->next) // 2D Array
			{
				fprintf(file, "    for (i32 i = 0; i < %s; i++)\n", node->declaration.type->type_usage.first_array_size_expression->string);
				fprintf(file, "    {\n");
				fprintf(file, "        for (i32 j = 0; j < %s; j++)\n", node->declaration.type->type_usage.first_array_size_expression->next->string);
				fprintf(file, "        {\n");
				if (node->declaration.type->type_usage.pointer_count > 0) // 2D Array of pointers
				{
					if (node->declaration.type->type_usage.pointer_count > 1)
						fprintf(file, "No support for double or > pointers yet.\n");
					
					fprintf(file, "            if (%s%s[i])\n", access_string, node->name);
					fprintf(file, "            {\n");
					fprintf(file, "                i32 pos = ftell(file);\n");
					fprintf(file, "                Assert(pos != -1);\n");
					fprintf(file, "                Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);\n");
					fprintf(file, "                SerialisationPointer ptr = {&(%s%s[i][j]), pos};\n", access_string, node->name);
					fprintf(file, "                serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
					fprintf(file, "                i32 empty = INT_MAX;\n");
					fprintf(file, "                WriteToFile(file, &empty, sizeof(i32));\n");
					fprintf(file, "            }\n");
					fprintf(file, "            else\n");
					fprintf(file, "            {\n");
					fprintf(file, "                i32 null_ptr = 0;\n");
					fprintf(file, "                WriteToFile(file, &null_ptr, sizeof(i32));\n");
					fprintf(file, "            }\n\n");
				}
				else
				{
					if (is_complex) // 2D Array of complex types
					{
						fprintf(file, "            Write%sToFile(file, &(%s%s[i][j]));\n", node->declaration.type->name, access_string, node->name);
					}
					else // 2D Array of primative types
					{
						fprintf(file, "            WriteToFile(file, &%s%s[i][j], sizeof(%s));\n", access_string, node->name, node->declaration.type->name);
					}
				}
				fprintf(file, "        }\n");
				fprintf(file, "    }\n\n");
			}
			else // 1D Array
			{
				fprintf(file, "    for (i32 i = 0; i < %s; i++)\n", node->declaration.type->type_usage.first_array_size_expression->string);
				fprintf(file, "    {\n");
				
				if (node->declaration.type->type_usage.pointer_count > 0) // Array of pointers
				{
					if (node->declaration.type->type_usage.pointer_count > 1)
						fprintf(file, "No support for double or > pointers yet.\n");
					
					fprintf(file, "        if (%s%s[i])\n", access_string, node->name);
					fprintf(file, "        {\n");
					fprintf(file, "            i32 pos = ftell(file);\n");
					fprintf(file, "            Assert(pos != -1);\n");
					fprintf(file, "            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);\n");
					fprintf(file, "            SerialisationPointer ptr = {&(%s%s[i]), pos};\n", access_string, node->name);
					fprintf(file, "            serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
					fprintf(file, "            i32 empty = INT_MAX;\n");
					fprintf(file, "            WriteToFile(file, &empty, sizeof(i32));\n");
					fprintf(file, "        }\n");
					fprintf(file, "        else\n");
					fprintf(file, "        {\n");
					fprintf(file, "            i32 null_ptr = 0;\n");
					fprintf(file, "            WriteToFile(file, &null_ptr, sizeof(i32));\n");
					fprintf(file, "        }\n\n");
				}
				else
				{
					if (is_complex) // Array of complex types
					{
						fprintf(file, "        Write%sToFile(file, &(%s%s[i]));\n", node->declaration.type->name, access_string, node->name);
					}
					else // Array of primative types
					{
						fprintf(file, "        WriteToFile(file, &%s%s[i], sizeof(%s));\n", access_string, node->name, node->declaration.type->name);
					}
				}
				fprintf(file, "    }\n\n");
			}
		}
		else // Singular data type
		{
			if (node->declaration.type->type_usage.pointer_count > 0) // Pointer
			{
				if (node->declaration.type->type_usage.pointer_count > 1)
					fprintf(file, "No support for double or > pointers yet.\n");
				
				fprintf(file, "    if (%s%s)\n", access_string, node->name);
				fprintf(file, "    {\n");
				fprintf(file, "        i32 pos = ftell(file);\n");
				fprintf(file, "        Assert(pos != -1);\n");
				fprintf(file, "        Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);\n");
				fprintf(file, "        SerialisationPointer ptr = {&%s%s, pos};\n", access_string, node->name);
				fprintf(file, "        serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
				fprintf(file, "        i32 empty = INT_MAX;\n");
				fprintf(file, "        WriteToFile(file, &empty, sizeof(i32));\n");
				fprintf(file, "    }\n");
				fprintf(file, "    else\n");
				fprintf(file, "    {\n");
				fprintf(file, "        i32 null_ptr = 0;\n");
				fprintf(file, "        WriteToFile(file, &null_ptr, sizeof(i32));\n");
				fprintf(file, "    }\n");
			}
			else // Structure
			{
				if (is_complex)
					fprintf(file, "    Write%sToFile(file, &%s%s);\n\n", node->declaration.type->name, access_string, node->name);
				else
					fprintf(file, "    WriteToFile(file, &%s%s, sizeof(%s%s));\n\n", access_string, node->name, access_string, node->name);
			}
		}
	}
}

internal void GenerateSerialisationCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (!h_file || !c_file)
		return;
	
	{
		fprintf(h_file, "WriteComponentToFile(FILE *file, i32 comp_id, ComponentType type);\n");
		fprintf(c_file, "WriteComponentToFile(FILE *file, i32 comp_id, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "        case COMPONENT_%s:\n", trimmed_lowercase_name);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            Write%sToFile(file, &core->run_data->entity_components.%ss[comp_id - 1]);\n", comp_node->name, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        } break;\n");
		}
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n");
	}
	
	{
		fprintf(h_file, "ReadComponentFromFile(FILE *file, Entity *entity, ComponentType type);\n");
		fprintf(c_file, "ReadComponentFromFile(FILE *file, Entity *entity, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "        case COMPONENT_%s:\n", trimmed_lowercase_name);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            %s component;\n", comp_node->name);
			fprintf(c_file, "            Read%sFromFile(file, &component);\n", comp_node->name);
			
			fprintf(c_file, "            %s *new_comp = Add%s(entity);\n", comp_node->name, comp_node->name);
			fprintf(c_file, "            i32 new_comp_id = new_comp->component_id;\n");
			fprintf(c_file, "            *new_comp = component;\n");
			fprintf(c_file, "            new_comp->component_id = new_comp_id;\n");
			fprintf(c_file, "            new_comp->parent_entity_id = entity->entity_id;\n");
			fprintf(c_file, "        } break;\n");
		}
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n");
	}
	
	// NOTE(randy): Outdated
	{
		fprintf(h_file, "SerialiseEntityComponentsFromIDList(FILE *file, Entity *entity_list, ComponentSet *component_set, i32 *ids, i32 id_count, ComponentType type);\n\n");
		fprintf(c_file, "SerialiseEntityComponentsFromIDList(FILE *file, Entity *entity_list, ComponentSet *component_set, i32 *ids, i32 id_count, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "        case COMPONENT_%s:\n", trimmed_lowercase_name);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            typedef struct ComponentSave {\n");
			fprintf(c_file, "                i32 entity_offset;\n");
			fprintf(c_file, "                %s *comp_data;\n", comp_node->name);
			fprintf(c_file, "            } ComponentSave;\n");
			fprintf(c_file, "            ComponentSave comps[MAX_ENTITIES];\n");
			fprintf(c_file, "            i32 comp_count = 0;\n\n");
			
			fprintf(c_file, "            for (i32 i = 0; i < id_count; i++)\n");
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                if (entity_list[ids[i] - 1].component_ids[type])\n");
			fprintf(c_file, "                {\n");
			fprintf(c_file, "                    comps[comp_count].entity_offset = i;\n");
			fprintf(c_file, "                    comps[comp_count].comp_data = &component_set->%ss[entity_list[ids[i] - 1].component_ids[type] - 1];\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "                    comp_count++;\n");
			fprintf(c_file, "                }\n");
			fprintf(c_file, "            }\n\n");
			
			// Give components the ID of its Entity, that way when deserialising it can look up to an
			// already constructed entity in constant time.
			
			fprintf(c_file, "            WriteToFile(file, &comp_count, sizeof(comp_count));\n");
			fprintf(c_file, "            for (i32 i = 0; i < comp_count; i++)\n");
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));\n");
			fprintf(c_file, "                Write%sToFile(file, comps[i].comp_data);\n", comp_node->name);
			fprintf(c_file, "            }\n");
			fprintf(c_file, "        } break;\n");
		}
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n\n");
	}
	
	// NOTE(randy): Serialise all components that are attached to entities in the array of ids
	{
		fprintf(h_file, "SerialiseComponentsFromDataSet(FILE *file, Entity *entity_list, i32 entity_count, ComponentSet *component_set, i32 *ids, i32 id_count);\n\n");
		fprintf(c_file, "SerialiseComponentsFromDataSet(FILE *file, Entity *entity_list, i32 entity_count, ComponentSet *component_set, i32 *ids, i32 id_count)\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        ComponentSaveHelper comps[MAX_ENTITIES];\n");
			fprintf(c_file, "        i32 comp_count = 0;\n\n");
			
			fprintf(c_file, "        for (i32 i = 0; i < id_count; i++)\n");
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            Entity *entity = &entity_list[ids[i] - 1];\n");
			fprintf(c_file, "            Assert(ids[i] - 1 < entity_count);\n");
			
			fprintf(c_file, "            if (entity->component_ids[COMPONENT_%s])\n", trimmed_lowercase_name);
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                comps[comp_count].entity_offset = i;\n");
			fprintf(c_file, "                comps[comp_count].comp_data = &component_set->%ss[entity->component_ids[COMPONENT_%s] - 1];\n", comp_node->name_lowercase_with_underscores, trimmed_lowercase_name);
			fprintf(c_file, "                comp_count++;\n");
			fprintf(c_file, "            }\n");
			fprintf(c_file, "        }\n\n");
			
			fprintf(c_file, "        WriteToFile(file, &comp_count, sizeof(comp_count)); \n");
			fprintf(c_file, "        for (i32 i = 0; i < comp_count; i++)\n");
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));\n");
			fprintf(c_file, "            Write%sToFile(file, comps[i].comp_data);\n", comp_node->name);
			fprintf(c_file, "        }\n");
			fprintf(c_file, "    }\n\n");
		}
		
		fprintf(c_file, "}\n\n");
	}
	
	{
		fprintf(h_file, "DeserialiseEntityComponentsFromIDList(FILE *file, i32 *ids, i32 id_count, ComponentType type);\n\n");
		fprintf(c_file, "DeserialiseEntityComponentsFromIDList(FILE *file, i32 *ids, i32 id_count, ComponentType type)\n");
		fprintf(c_file, "{\n");
		fprintf(c_file, "    switch (type)\n");
		fprintf(c_file, "    {\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "        case COMPONENT_%s:\n", trimmed_lowercase_name);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            i32 comp_count = 0;\n");
			fprintf(c_file, "            ReadFromFile(file, &comp_count, sizeof(comp_count));\n");
			fprintf(c_file, "            for (i32 i = 0; i < comp_count; i++)\n");
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                i32 entity_offset = 0;\n");
			fprintf(c_file, "                ReadFromFile(file, &entity_offset, sizeof(i32));\n");
			
			fprintf(c_file, "                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];\n");
			fprintf(c_file, "                Assert(entity->entity_id)\n");
			
			fprintf(c_file, "                %s component = {0};\n", comp_node->name);
			fprintf(c_file, "                Read%sFromFile(file, &component);\n", comp_node->name);
			fprintf(c_file, "                %s *new_comp = Add%s(entity);\n", comp_node->name, comp_node->name);
			fprintf(c_file, "                i32 new_comp_id = new_comp->component_id;\n");
			fprintf(c_file, "                *new_comp = component;\n");
			fprintf(c_file, "                new_comp->component_id = new_comp_id;\n");
			fprintf(c_file, "                new_comp->parent_entity_id = entity->entity_id;\n");
			fprintf(c_file, "            }\n");
			fprintf(c_file, "        } break;\n");
		}
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n\n");
	}
	
	{
		fprintf(h_file, "DeserialiseComponentsToLoadData(FILE *file, ComponentSet *component_set, EntitySave *entity_list, i32 *ids, i32 id_count);\n\n");
		fprintf(c_file, "DeserialiseComponentsToLoadData(FILE *file, ComponentSet *component_set, EntitySave *entity_list, i32 *ids, i32 id_count)\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        i32 component_count;\n");
			fprintf(c_file, "        ReadFromFile(file, &component_count, sizeof(i32));\n");
			
			fprintf(c_file, "        for (i32 i = 0; i < component_count; i++)\n");
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            i32 entity_offset;\n");
			fprintf(c_file, "            ReadFromFile(file, &entity_offset, sizeof(i32));\n");
			
			fprintf(c_file, "            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];\n");
			fprintf(c_file, "            %s component_data;\n", comp_node->name);
			fprintf(c_file, "            Read%sFromFile(file, &component_data);\n", comp_node->name);
			fprintf(c_file, "            component_data.parent_entity_id = ids[entity_offset];\n");
			fprintf(c_file, "            component_set->%ss[component_set->%s_count++] = component_data;\n", comp_node->name_lowercase_with_underscores, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        }\n");
			fprintf(c_file, "    }\n");
		}
		fprintf(c_file, "}\n\n");
	}
	
	{
		fprintf(h_file, "DeserialiseComponentsFromMap(i32 *entity_id_map, i32 entity_count);\n\n");
		fprintf(c_file, "DeserialiseComponentsFromMap(i32 *entity_id_map, i32 entity_count)\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			
			i32 string_length = 0;
			for (; comp_node->name_lowercase_with_underscores[string_length]; ++string_length)
			{
			}
			char trimmed_lowercase_name[50];
			strcpy(trimmed_lowercase_name, comp_node->name_lowercase_with_underscores);
			trimmed_lowercase_name[string_length - 10] = '\0';
			
			fprintf(c_file, "    for (i32 i = 0; i < core->run_data->loaded_entity_components.%s_count; i++)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        %s *saved_comp = &core->run_data->loaded_entity_components.%ss[i];\n", comp_node->name, comp_node->name_lowercase_with_underscores);
			// NOTE(randy): The entity ID that is stored in the comp is just a temporary offset in the ID map
			fprintf(c_file, "        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];\n");
			fprintf(c_file, "        Assert(entity->entity_id);\n");
			fprintf(c_file, "        %s *new_comp = Add%s(entity);\n", comp_node->name, comp_node->name);
			fprintf(c_file, "        i32 new_comp_id = new_comp->component_id;\n");
			fprintf(c_file, "        *new_comp = *saved_comp;\n");
			fprintf(c_file, "        new_comp->component_id = new_comp_id;\n");
			fprintf(c_file, "        new_comp->parent_entity_id = entity->entity_id;\n");
			fprintf(c_file, "    }\n");
		}
		fprintf(c_file, "}\n\n");
	}
	
	for (i32 i = 0; i < serialisable_struct_count; i++)
	{
		DataDeskNode *root = serialisable_structs[i];
		
		// NOTE(randy): Write to file
		{
			fprintf(h_file, "static void Write%sToFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Write%sToFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				WriteMemberToFile(c_file, member, "data->");
			}
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Fill pointers in file
		{
			fprintf(h_file, "static void Fill%sPointersInFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Fill%sPointersInFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (!DataDeskNodeHasTag(member, "ComponentList") && !DataDeskNodeHasTag(member, "DoNotSerialise"))
				{
					if (member->type != DATA_DESK_NODE_TYPE_declaration || member->declaration.type->type != DATA_DESK_NODE_TYPE_type_usage)
						fprintf(c_file, "uhhhhh");
					
					b8 is_complex = 0;
					for (i32 j = 0; j < serialisable_struct_count; j++)
					{
						DataDeskNode *complex_struct = serialisable_structs[j];
						if (strcmp(complex_struct->name, member->declaration.type->name) == 0)
						{
							is_complex = 1;
							break;
						}
					}
					
					if (member->declaration.type->type_usage.first_array_size_expression) // NOTE(randy): Array type
					{
						if (!(member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
							  member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
							fprintf(c_file, "\nuhhhhh\n");
						
						if (member->declaration.type->type_usage.first_array_size_expression->next) // NOTE(randy): 2D Array
						{
							// 2d array
							fprintf(c_file, "// - 2D Arary %s %s\n", member->declaration.type->type_usage.first_array_size_expression->string, member->declaration.type->type_usage.first_array_size_expression->next->string);
							
							fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");
							fprintf(c_file, "        for (i32 j = 0; j < %s; j++)\n", member->declaration.type->type_usage.first_array_size_expression->next->string);
							fprintf(c_file, "        {\n");
							if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): 2D Array of ptrs
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");
								
								fprintf(c_file, "        // '%s' pointer array in %s\n", member->name, root->name);
								fprintf(c_file, "        fseek(file, sizeof(i32), SEEK_CUR);\n");
							}
							else // NOTE(randy): 2D Structure array
							{
								fprintf(c_file, "            // '%s' array in %s\n", member->name, root->name);
								fprintf(c_file, "            for (i32 k = 0; k < serialisation_pointer_count; k++)\n");
								fprintf(c_file, "            {\n");
								fprintf(c_file, "                SerialisationPointer *ptr = &serialisation_pointers[k];\n");
								fprintf(c_file, "                if (*ptr->pointer_address == &(data->%s[i][j]))\n", member->name);
								fprintf(c_file, "                {\n");
								fprintf(c_file, "                    i32 current_pos = ftell(file);\n");
								fprintf(c_file, "                    Assert(current_pos != -1);\n");
								fprintf(c_file, "                    fseek(file, ptr->offset, SEEK_SET);\n");
								fprintf(c_file, "                    WriteToFile(file, &current_pos, sizeof(i32));\n");
								fprintf(c_file, "                    fseek(file, current_pos, SEEK_SET);\n");
								fprintf(c_file, "                }\n");
								fprintf(c_file, "            }\n");
								if (is_complex)
									fprintf(c_file, "            Fill%sPointersInFile(file, &(data->%s[i][j]));\n", member->declaration.type->name, member->name);
								else
									fprintf(c_file, "            fseek(file, sizeof(%s), SEEK_CUR);\n", member->declaration.type->name);
							}
							fprintf(c_file, "        }\n");
							fprintf(c_file, "    }\n\n");
						}
						else // NOTE(randy): 1D Array
						{
							fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");
							
							if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Array of ptrs
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");
								
								fprintf(c_file, "    // '%s' pointer array in %s\n", member->name, root->name);
								fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n");
							}
							else // NOTE(randy): Structure array
							{
								fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
								fprintf(c_file, "        for (i32 j = 0; j < serialisation_pointer_count; j++)\n");
								fprintf(c_file, "        {\n");
								fprintf(c_file, "            SerialisationPointer *ptr = &serialisation_pointers[j];\n");
								fprintf(c_file, "            if (*ptr->pointer_address == &(data->%s[i]))\n", member->name);
								fprintf(c_file, "            {\n");
								fprintf(c_file, "                i32 current_pos = ftell(file);\n");
								fprintf(c_file, "                Assert(current_pos != -1);\n");
								fprintf(c_file, "                fseek(file, ptr->offset, SEEK_SET);\n");
								fprintf(c_file, "                WriteToFile(file, &current_pos, sizeof(i32));\n");
								fprintf(c_file, "                fseek(file, current_pos, SEEK_SET);\n");
								fprintf(c_file, "            }\n");
								fprintf(c_file, "        }\n");
								if (is_complex)
									fprintf(c_file, "        Fill%sPointersInFile(file, &(data->%s[i]));\n", member->declaration.type->name, member->name);
								else
									fprintf(c_file, "        fseek(file, sizeof(%s), SEEK_CUR);\n", member->declaration.type->name);
							}
							fprintf(c_file, "    }\n\n");
						}
					}
					else
					{
						if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Pointer
						{
							if (member->declaration.type->type_usage.pointer_count > 1)
								fprintf(c_file, "No support for double or > pointers yet.\n");
							
							fprintf(c_file, "    // '%s' pointer in %s\n", member->name, root->name);
							fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n\n");
						}
						else // NOTE(randy): Structure
						{
							fprintf(c_file, "    // '%s' in %s\n", member->name, root->name);
							fprintf(c_file, "    for (i32 i = 0; i < serialisation_pointer_count; i++)\n");
							fprintf(c_file, "    {\n");
							fprintf(c_file, "        SerialisationPointer *ptr = &serialisation_pointers[i];\n");
							fprintf(c_file, "        if (*ptr->pointer_address == &data->%s)\n", member->name);
							fprintf(c_file, "        {\n");
							fprintf(c_file, "            i32 current_pos = ftell(file);\n");
							fprintf(c_file, "            Assert(current_pos != -1);\n");
							fprintf(c_file, "            fseek(file, ptr->offset, SEEK_SET);\n");
							fprintf(c_file, "            WriteToFile(file, &current_pos, sizeof(i32));\n");
							fprintf(c_file, "            fseek(file, current_pos, SEEK_SET);\n");
							fprintf(c_file, "        }\n");
							fprintf(c_file, "    }\n");
							if (is_complex)
								fprintf(c_file, "    Fill%sPointersInFile(file, &data->%s);\n\n", member->declaration.type->name, member->name);
							else
								fprintf(c_file, "    fseek(file, sizeof(data->%s), SEEK_CUR);\n\n", member->name);
						}
					}
				}
			}
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Read from file
		{
			fprintf(h_file, "static void Read%sFromFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Read%sFromFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (!DataDeskNodeHasTag(member, "ComponentList") && !DataDeskNodeHasTag(member, "DoNotSerialise"))
				{
					if (member->type != DATA_DESK_NODE_TYPE_declaration || member->declaration.type->type != DATA_DESK_NODE_TYPE_type_usage)
						fprintf(c_file, "uhhhhh");
					
					b8 is_complex = 0;
					for (i32 j = 0; j < serialisable_struct_count; j++)
					{
						DataDeskNode *complex_struct = serialisable_structs[j];
						if (strcmp(complex_struct->name, member->declaration.type->name) == 0)
						{
							is_complex = 1;
							break;
						}
					}
					
					if (member->declaration.type->type_usage.first_array_size_expression) // NOTE(randy): Array type
					{
						if (!(member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
							  member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
							fprintf(c_file, "\nuhhhhh\n");
						
						if (member->declaration.type->type_usage.first_array_size_expression->next) // NOTE(randy): 2D Array
						{
							fprintf(c_file, "// - 2D Arary %s %s\n", member->declaration.type->type_usage.first_array_size_expression->string, member->declaration.type->type_usage.first_array_size_expression->next->string);
						}
						else // NOTE(randy): 1D Array
						{
							/* fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");

							if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Array of pointers
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");

								fprintf(c_file, "        // '%s' pointer array in %s\n", member->name, root->name);
								fprintf(c_file, "        if (data->%s[i])\n", member->name);
								fprintf(c_file, "        {\n");
								fprintf(c_file, "            i32 pos = ftell(file);\n");
								fprintf(c_file, "            Assert(pos != -1, \"Uh oh.\");\n");
								fprintf(c_file, "            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, \"Max pointers reached. Consider a better design?\");\n");
								fprintf(c_file, "            SerialisationPointer ptr = {&(data->%s[i]), pos};\n", member->name);
								fprintf(c_file, "            serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
								fprintf(c_file, "            i32 empty = INT_MAX;\n");
								fprintf(c_file, "            WriteToFile(file, &empty, sizeof(i32));\n");
								fprintf(c_file, "        }\n");
								fprintf(c_file, "        else\n");
								fprintf(c_file, "        {\n");
								fprintf(c_file, "            i32 null_ptr = 0;\n");
								fprintf(c_file, "            WriteToFile(file, &null_ptr, sizeof(i32));\n");
								fprintf(c_file, "        }\n\n");
							}
							else
							{
								if (is_complex) // NOTE(randy): Array of complex types
								{
									fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
									fprintf(c_file, "        Write%sToFile(file, &(data->%s[i]));\n", member->declaration.type->name, member->name);
								}
								else // NOTE(randy): Array of primative types
								{
									fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
									fprintf(c_file, "        WriteToFile(file, &data->%s[i], sizeof(%s));\n", member->name, member->declaration.type->name);
								}
							}
							fprintf(c_file, "    }\n\n"); */
						}
					}
					else // NOTE(randy): Singular data type
					{
						if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Pointer
						{
							if (member->declaration.type->type_usage.pointer_count > 1)
								fprintf(c_file, "No support for double or > pointers yet.\n");
							
							fprintf(c_file, "    // '%s' pointer in %s\n", member->name, root->name);
							fprintf(c_file, "    {\n");
							fprintf(c_file, "        i32 pointer_offset;\n");
							fprintf(c_file, "        ReadFromFile(file, &pointer_offset, sizeof(i32));\n");
							fprintf(c_file, "        if (pointer_offset)\n");
							fprintf(c_file, "        {\n");
							fprintf(c_file, "            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);\n");
							fprintf(c_file, "            SerialisationPointer ptr = {&data->%s, pointer_offset};\n", member->name);
							fprintf(c_file, "            serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
							fprintf(c_file, "        }\n");
							fprintf(c_file, "        else\n");
							fprintf(c_file, "            data->%s = 0;\n", member->name);
							fprintf(c_file, "    }\n");
						}
						else // NOTE(randy): Structure
						{
							fprintf(c_file, "    // '%s' in %s\n", member->name, root->name);
							if (is_complex)
								fprintf(c_file, "    Read%sFromFile(file, &data->%s);\n\n", member->declaration.type->name, member->name);
							else
								fprintf(c_file, "    ReadFromFile(file, &data->%s, sizeof(data->%s));\n\n", member->name, member->name);
						}
					}
				}
			}
			fprintf(c_file, "}\n\n");
		}
		
		// NOTE(randy): Fill pointers from file
		{
			fprintf(h_file, "static void Fill%sPointersFromFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Fill%sPointersFromFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (!DataDeskNodeHasTag(member, "ComponentList") && !DataDeskNodeHasTag(member, "DoNotSerialise"))
				{
					if (member->type != DATA_DESK_NODE_TYPE_declaration || member->declaration.type->type != DATA_DESK_NODE_TYPE_type_usage)
						fprintf(c_file, "uhhhhh");
					
					b8 is_complex = 0;
					for (i32 j = 0; j < serialisable_struct_count; j++)
					{
						DataDeskNode *complex_struct = serialisable_structs[j];
						if (strcmp(complex_struct->name, member->declaration.type->name) == 0)
						{
							is_complex = 1;
							break;
						}
					}
					
					if (member->declaration.type->type_usage.first_array_size_expression) // NOTE(randy): Array type
					{
						/* if (!(member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
							  member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
							fprintf(c_file, "\nuhhhhh\n");

						if (member->declaration.type->type_usage.first_array_size_expression->next) // NOTE(randy): 2D Array
						{
							// 2d array
							fprintf(c_file, "// - 2D Arary %s %s\n", member->declaration.type->type_usage.first_array_size_expression->string, member->declaration.type->type_usage.first_array_size_expression->next->string);
						}
						else // NOTE(randy): 1D Array
						{
							fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");

							if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Array of ptrs
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");

								fprintf(c_file, "    // '%s' pointer array in %s\n", member->name, root->name);
								fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n");
							}
							else // NOTE(randy): Structure array
							{
								fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
								fprintf(c_file, "        for (i32 j = 0; j < serialisation_pointer_count; j++)\n");
								fprintf(c_file, "        {\n");
								fprintf(c_file, "            SerialisationPointer *ptr = &serialisation_pointers[j];\n");
								fprintf(c_file, "            if (*ptr->pointer_address == &(data->%s[i]))\n", member->name);
								fprintf(c_file, "            {\n");
								fprintf(c_file, "                i32 current_pos = ftell(file);\n");
								fprintf(c_file, "                Assert(current_pos != -1, \"Uh oh.\");\n");
								fprintf(c_file, "                fseek(file, ptr->offset, SEEK_SET);\n");
								fprintf(c_file, "                WriteToFile(file, &current_pos, sizeof(i32));\n");
								fprintf(c_file, "                fseek(file, current_pos, SEEK_SET);\n");
								fprintf(c_file, "            }\n");
								fprintf(c_file, "        }\n");
								if (is_complex)
									fprintf(c_file, "        Fill%sPointersInFile(file, &(data->%s[i]));\n", member->declaration.type->name, member->name);
								else
									fprintf(c_file, "        fseek(file, sizeof(%s), SEEK_CUR);\n", member->declaration.type->name);
							}
							fprintf(c_file, "    }\n\n");
						} */
					}
					else
					{
						if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(randy): Pointer
						{
							if (member->declaration.type->type_usage.pointer_count > 1)
								fprintf(c_file, "No support for double or > pointers yet.\n");
							
							fprintf(c_file, "    // '%s' pointer in %s\n", member->name, root->name);
							fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n\n");
						}
						else // NOTE(randy): Structure
						{
							fprintf(c_file, "    // '%s' in %s\n", member->name, root->name);
							fprintf(c_file, "    for (i32 i = 0; i < serialisation_pointer_count; i++)\n");
							fprintf(c_file, "    {\n");
							fprintf(c_file, "        SerialisationPointer *ptr = &serialisation_pointers[i];\n");
							fprintf(c_file, "        i32 current_pos = ftell(file);\n");
							fprintf(c_file, "        Assert(current_pos != -1);\n");
							fprintf(c_file, "        if (ptr->offset == current_pos)\n"); // This is pre inefficient lmao
							fprintf(c_file, "        {\n");
							fprintf(c_file, "            *ptr->pointer_address = &data->%s;\n", member->name);
							fprintf(c_file, "        }\n");
							fprintf(c_file, "    }\n");
							if (is_complex)
								fprintf(c_file, "    Fill%sPointersInFile(file, &data->%s);\n\n", member->declaration.type->name, member->name);
							else
								fprintf(c_file, "    fseek(file, sizeof(data->%s), SEEK_CUR);\n\n", member->name);
						}
					}
				}
			}
			fprintf(c_file, "}\n\n");
		}
	}
}