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

global i32 unique_entity_count = 0;
global DataDeskNode *unique_entities[1024];
internal void GenerateEntityCode();

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
		else if (DataDeskNodeHasTag(root, "GenerateEntityCode"))
		{
			GenerateEntityCode();
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
				DataDeskNode *unique_entity_tag = DataDeskGetNodeTag(root, "UniqueEntity");
				if (unique_entity_tag)
				{
					fprintf(h_file, "#define MAX_%s_COUNT (%s)\n", root->name_uppercase_with_underscores, DataDeskGetTagParameter(unique_entity_tag, 0)->name);
					unique_entities[unique_entity_count++] = root;
				}

				fprintf(h_file, "typedef struct %s\n", root->string);
				fprintf(h_file, "{\n");

				DataDeskNode *previous_node = 0;
				DataDeskNode *original_member = root->struct_declaration.first_member;
				if (DataDeskNodeHasTag(root, "Component"))
				{
					fprintf(h_file, "i32 parent_entity_id;\n");
					DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "parent_entity_id");
					node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
					root->struct_declaration.first_member = node_1;

					fprintf(h_file, "i32 component_id;\n");
					DataDeskNode *node_2 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "component_id");
					node_2->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
					node_1->next = node_2;
					previous_node = node_2;

					component_nodes[component_nodes_count++] = root;
				}
				else if (unique_entity_tag)
				{
					DataDeskNode *comp_list_node = root->struct_declaration.first_member;
					original_member = comp_list_node->next;

					fprintf(h_file, "i32 entity_id;\n");
					DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "entity_id");
					node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
					root->struct_declaration.first_member->next = node_1;

					previous_node = node_1;
					if (atoi(DataDeskGetTagParameter(unique_entity_tag, 0)->name) > 1)
					{
						fprintf(h_file, "i32 unique_entity_id;\n");
						DataDeskNode *node_2 = AllocNode(DATA_DESK_NODE_TYPE_declaration, "unique_entity_id");
						node_2->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
						node_1->next = node_2;
						previous_node = node_2;
					}

					/* DataDeskNode *component_list_tag = DataDeskGetNodeTag(comp_list_node, "ComponentList");
					if (component_list_tag)
					{
						i32 tag_index = 0;
						DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
						while (tag_param)
						{
							char variable_1_type[100];
							sprintf(variable_1_type, "%sComponent", tag_param->name);
							char variable_1_name[100];
							sprintf(variable_1_name, "%s_comp", tag_param->name_lowercase_with_underscores);
							fprintf(h_file, "%s *%s;\n",
									variable_1_type,
									variable_1_name);
							DataDeskNode *comp_node = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_1_name);
							comp_node->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, variable_1_type);
							comp_node->declaration.type->type_usage.pointer_count = 1;
							previous_node->next = comp_node;

							previous_node = comp_node;
							tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
						}
					}
					else
						fprintf(h_file, "uhhhhhh"); */
				}

				for (DataDeskNode *member = original_member;
					 member; member = member->next)
				{
					if (DataDeskNodeHasTag(member, "GenerateUniqueEntityArrays"))
					{
						fprintf(h_file, "\n");
						for (i32 i = 0; i < unique_entity_count; i++)
						{
							DataDeskNode *entity_node = unique_entities[i];
							DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

							if (atoi(DataDeskGetTagParameter(entity_node_tag, 0)->name) > 1)
							{
								char variable_1_type[100];
								sprintf(variable_1_type, "%s", entity_node->name);
								char variable_1_name[100];
								sprintf(variable_1_name, "%s_list", entity_node->name_lowercase_with_underscores);
								char variable_1_array_size[100];
								sprintf(variable_1_array_size, "MAX_%s_COUNT", entity_node->name_uppercase_with_underscores);
								fprintf(h_file, "%s %s[%s];\n",
										variable_1_type,
										variable_1_name,
										variable_1_array_size);
								DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_1_name);
								node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, variable_1_type);
								node_1->declaration.type->type_usage.first_array_size_expression = AllocNode(DATA_DESK_NODE_TYPE_identifier, variable_1_array_size);
								if (previous_node)
									previous_node->next = node_1;

								char variable_2_name[100];
								sprintf(variable_2_name, "%s_count", entity_node->name_lowercase_with_underscores);
								fprintf(h_file, "i32 %s;\n", variable_2_name);
								DataDeskNode *node_2 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_2_name);
								node_2->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
								node_1->next = node_2;

								char variable_3_name[100];
								sprintf(variable_3_name, "free_%s_id", entity_node->name_lowercase_with_underscores);
								fprintf(h_file, "i32 %s;\n", variable_3_name);
								DataDeskNode *node_3 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_3_name);
								node_3->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, "i32");
								node_2->next = node_3;

								previous_node = node_3;
							}
							else
							{
								fprintf(h_file, "%s %s;\n", entity_node->name, entity_node->name_lowercase_with_underscores);
								DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, entity_node->name_lowercase_with_underscores);
								node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, entity_node->name);
								if (previous_node)
									previous_node->next = node_1;

								previous_node = node_1;
							}
						}
						fprintf(h_file, "\n");
					}

					if (previous_node)
						previous_node->next = member;

					DataDeskFWriteGraphAsC(h_file, member, 0);
					fprintf(h_file, ";\n");

					previous_node = member;
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

				// NOTE(tjr): X-Macro initialisation
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

				// NOTE(tjr): Generate enum print function implementation.
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

						// NOTE(tjr): Make enum name look pretty.
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

	// NOTE(tjr): Generate component set structure.
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
			fprintf(h_file, "%s %s[MAX_ACTIVE_ENTITIES];\n",
					comp_node->name,
					variable_1_name);
			DataDeskNode *node_1 = AllocNode(DATA_DESK_NODE_TYPE_declaration, variable_1_name);
			node_1->declaration.type = AllocNode(DATA_DESK_NODE_TYPE_type_usage, comp_node->name);
			node_1->declaration.type->type_usage.first_array_size_expression = AllocNode(DATA_DESK_NODE_TYPE_identifier, "MAX_ACTIVE_ENTITIES");
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

	// NOTE(tjr): Component add/remove functions, and helpers
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

		// NOTE(tjr): Add Component function.
		{
			fprintf(c_file, "internal %s *Add%s(Entity *entity)\n", comp_node->name, comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    R_DEV_ASSERT(core->world_data->entity_components.free_%s_id > 0, \"Max %s reached.\");\n", comp_node->name_lowercase_with_underscores, comp_node->name);
			fprintf(c_file, "    R_DEV_ASSERT(entity->component_ids[COMPONENT_%s] == 0, \"Entity already has a %s\");\n", trimmed_lowercase_name, comp_node->name);
			fprintf(c_file, "    i32 new_comp_id = core->world_data->entity_components.free_%s_id;\n\n", comp_node->name_lowercase_with_underscores);

			fprintf(c_file, "    %s *comp = &core->world_data->entity_components.%ss[new_comp_id - 1];\n", comp_node->name, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    *comp = GetDefault%s();\n", comp_node->name);
			fprintf(c_file, "    comp->parent_entity_id = entity->entity_id;\n");
			fprintf(c_file, "    comp->component_id = new_comp_id;\n");
			fprintf(c_file, "    entity->component_ids[COMPONENT_%s] = new_comp_id;\n\n", trimmed_lowercase_name);

			fprintf(c_file, "    if (core->world_data->entity_components.%s_count == core->world_data->entity_components.free_%s_id - 1)\n", comp_node->name_lowercase_with_underscores, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        core->world_data->entity_components.%s_count++;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->world_data->entity_components.free_%s_id++;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    }\n\n");

			fprintf(c_file, "    if (core->world_data->entity_components.%s_count < MAX_ACTIVE_ENTITIES)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        if (core->world_data->entity_components.%s_count != core->world_data->entity_components.free_%s_id - 1)\n", comp_node->name_lowercase_with_underscores, comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        {\n");
			fprintf(c_file, "            b8 found = 0;\n");
			fprintf(c_file, "            for (i32 i = 0; i < core->world_data->entity_components.%s_count; i++)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "            {\n");
			fprintf(c_file, "                if (core->world_data->entity_components.%ss[i].component_id)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "                {\n");
			fprintf(c_file, "                    core->world_data->entity_components.free_%s_id = i + 1;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "                    found = 1;\n");
			fprintf(c_file, "                    break;\n");
			fprintf(c_file, "                }\n");
			fprintf(c_file, "            }\n");
			fprintf(c_file, "        }\n");
			fprintf(c_file, "    }\n");
			fprintf(c_file, "    else\n");
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        core->world_data->entity_components.free_%s_id = 0;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    }\n\n");

			fprintf(c_file, "    return comp;\n");
			fprintf(c_file, "}\n\n");
		}

		// NOTE(tjr): Remove Component function.
		{
			fprintf(c_file, "internal void Remove%s(Entity *entity)\n", comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    R_DEV_ASSERT(entity->component_ids[COMPONENT_%s] != 0, \"Entity does not have a %s\");\n", trimmed_lowercase_name, comp_node->name);
			fprintf(c_file, "    %s *comp = &core->world_data->entity_components.%ss[entity->component_ids[COMPONENT_%s] - 1];\n\n", comp_node->name, comp_node->name_lowercase_with_underscores, trimmed_lowercase_name);

			fprintf(c_file, "    if (comp->component_id < core->world_data->entity_components.free_%s_id)\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->world_data->entity_components.free_%s_id = comp->component_id;\n\n", comp_node->name_lowercase_with_underscores);

			fprintf(c_file, "    %s empty_comp = {0};\n", comp_node->name);
			fprintf(c_file, "    *comp = empty_comp;\n", comp_node->name_lowercase_with_underscores);
			fprintf(c_file, "    entity->component_ids[COMPONENT_%s] = 0;\n", trimmed_lowercase_name);
			fprintf(c_file, "}\n\n");
		}

		// NOTE(tjr): Get component helper function.
		{
			fprintf(h_file, "// NOTE(tjr): Gets a %s from a specified entity, it must have one.\n", comp_node->name);
			fprintf(h_file, "internal %s *Get%sFromEntityID(i32 id);\n", comp_node->name, comp_node->name);
			fprintf(c_file, "internal %s *Get%sFromEntityID(i32 id)\n", comp_node->name, comp_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    Entity *entity = GetEntityWithID(id);\n");
			fprintf(c_file, "    R_DEV_ASSERT(entity->component_ids[COMPONENT_%s], \"Entity doesn't have a %s\");\n", trimmed_lowercase_name, comp_node->name);
			fprintf(c_file, "    %s *comp = &core->world_data->entity_components.%ss[entity->component_ids[COMPONENT_%s] - 1];\n", comp_node->name, comp_node->name_lowercase_with_underscores, trimmed_lowercase_name);
			fprintf(c_file, "    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_%s], \"IDs are mismatched.\");\n", trimmed_lowercase_name);
			fprintf(c_file, "    return comp;\n");
			fprintf(c_file, "}\n\n");
		}
	}

	// NOTE(tjr): Generic component removal
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
			fprintf(c_file, "            R_BREAK(\"Entity doesn't have a %s to remove.\");\n", comp_node->name);
			fprintf(c_file, "        break;\n");
			fprintf(c_file, "    }\n");
		}
		fprintf(c_file, "    default:\n");
		fprintf(c_file, "        R_BREAK(\"Invalid component type.\");\n");
		fprintf(c_file, "        break;\n");
		fprintf(c_file, "    }\n");
		fprintf(c_file, "}\n");
	}

	// NOTE(tjr): Component initialisation
	{
		fprintf(c_file, "internal void InitialiseComponents()\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < component_nodes_count; i++)
		{
			DataDeskNode *comp_node = component_nodes[i];
			fprintf(c_file, "    core->world_data->entity_components.free_%s_id = 1;\n", comp_node->name_lowercase_with_underscores);
		}
		fprintf(c_file, "}\n");
	}

	// NOTE(tjr): Generate Component UI print function.
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

internal void GenerateEntityCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;

	// NOTE(tjr): Unique entity type enum.
	fprintf(h_file, "typedef enum EntityType\n");
	fprintf(h_file, "{\n");
	fprintf(h_file, "    ENTITY_TYPE_generic,\n");
	for (i32 i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		fprintf(h_file, "    ENTITY_TYPE_%s,\n", entity_node->name_lowercase_with_underscores);
	}
	fprintf(h_file, "    ENTITY_TYPE_MAX\n");
	fprintf(h_file, "} EntityType;\n\n");

	// NOTE(tjr): Entity initialisation
	{
		fprintf(c_file, "internal void InitialiseUniqueEntities()\n");
		fprintf(c_file, "{\n");
		for (i32 i = 0; i < unique_entity_count; i++)
		{
			DataDeskNode *entity_node = unique_entities[i];
			DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

			if (atoi(DataDeskGetTagParameter(entity_node_tag, 0)->name) > 1)
				fprintf(c_file, "    core->world_data->free_%s_id = 1;\n", entity_node->name_lowercase_with_underscores);
		}
		fprintf(c_file, "}\n");
	}

	for (i32 i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		if (atoi(DataDeskGetTagParameter(entity_node_tag, 0)->name) > 1) // NOTE(tjr): Unique entity, mulitple
		{
			// NOTE(tjr): New unique entity
			{
				fprintf(c_file, "static %s *New%s()\n", entity_node->name, entity_node->name);
				fprintf(c_file, "{\n");
				fprintf(c_file, "    R_DEV_ASSERT(core->world_data->free_%s_id > 0, \"Maximum amount of %s entites reached\");\n\n", entity_node->name_lowercase_with_underscores, entity_node->name);

				fprintf(c_file, "    i32 new_unique_id = core->world_data->free_%s_id;\n\n", entity_node->name_lowercase_with_underscores);

				fprintf(c_file, "    Entity *generic_entity = NewEntity(\"%s\", GENERALISED_ENTITY_TYPE_%s);\n", entity_node->name, DataDeskGetTagParameter(entity_node_tag, 1)->name);
				fprintf(c_file, "    generic_entity->unique_entity_id = new_unique_id;\n");
				fprintf(c_file, "    generic_entity->type = ENTITY_TYPE_%s;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    %s *unique_entity = &core->world_data->%s_list[new_unique_id - 1];\n", entity_node->name, entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    unique_entity->entity_id = generic_entity->entity_id;\n");
				fprintf(c_file, "    unique_entity->unique_entity_id = new_unique_id;\n\n");

				DataDeskNode *component_list_tag = DataDeskGetNodeTag(entity_node->struct_declaration.first_member, "ComponentList");
				if (!component_list_tag)
					fprintf(c_file, "uh ohh\n");
				i32 tag_index = 0;
				DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
				while (tag_param)
				{
					fprintf(c_file, "    Add%sComponent(generic_entity);\n", tag_param->name);
					tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
				}

				fprintf(c_file, "\n    if (core->world_data->%s_count == core->world_data->free_%s_id - 1)\n", entity_node->name_lowercase_with_underscores, entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    {\n");
				fprintf(c_file, "        core->world_data->%s_count++;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "        core->world_data->free_%s_id++;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    }\n\n");

				fprintf(c_file, "    if (core->world_data->%s_count < MAX_%s_COUNT)\n", entity_node->name_lowercase_with_underscores, entity_node->name_uppercase_with_underscores);
				fprintf(c_file, "    {\n");
				fprintf(c_file, "        if (core->world_data->%s_count != core->world_data->free_%s_id - 1)\n", entity_node->name_lowercase_with_underscores, entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "        {\n");
				fprintf(c_file, "            b8 found = 0;\n");
				fprintf(c_file, "            for (i32 i = 0; i < core->world_data->%s_count++; i++)\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "            {\n");
				fprintf(c_file, "                core->world_data->free_%s_id = i + 1;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "                found = 1;\n");
				fprintf(c_file, "                break;\n");
				fprintf(c_file, "            }\n");
				fprintf(c_file, "            R_DEV_ASSERT(found, \"Couldn't find a free index?\");\n");
				fprintf(c_file, "        }\n");
				fprintf(c_file, "    }\n");
				fprintf(c_file, "    else\n");
				fprintf(c_file, "    {\n");
				fprintf(c_file, "        core->world_data->free_%s_id = 0;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    }\n\n");

				fprintf(c_file, "    return unique_entity;\n");
				fprintf(c_file, "}\n\n");
			}

			// NOTE(tjr): Delete unique entity
			{
				fprintf(c_file, "static void Delete%s(%s *entity)\n", entity_node->name, entity_node->name);
				fprintf(c_file, "{\n");
				fprintf(c_file, "    R_DEV_ASSERT(entity && entity->entity_id && entity->unique_entity_id, \"Invalid entity\");\n");
				fprintf(c_file, "    Entity *generic_entity = GetEntityWithID(entity->entity_id);\n");
				fprintf(c_file, "    R_DEV_ASSERT(generic_entity->unique_entity_id == entity->unique_entity_id, \"Mismatched id.\");\n");
				fprintf(c_file, "    generic_entity->unique_entity_id = 0;\n");
				fprintf(c_file, "    DeleteEntity(generic_entity);\n\n");

				fprintf(c_file, "    if (entity->unique_entity_id < core->world_data->free_%s_id);\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "        core->world_data->free_%s_id = entity->unique_entity_id;\n", entity_node->name_lowercase_with_underscores);
				fprintf(c_file, "    %s empty_entity = {0};\n", entity_node->name);
				fprintf(c_file, "    *entity = empty_entity;\n");
				fprintf(c_file, "}\n\n");
			}
		}
		else // NOTE(tjr): Unique entity, singular
		{
			// NOTE(tjr): New singular unique entity
			fprintf(c_file, "static %s *Initialise%s()\n", entity_node->name, entity_node->name);
			fprintf(c_file, "{\n");

			fprintf(c_file, "    Entity *generic_entity = NewEntity(\"%s\", GENERALISED_ENTITY_TYPE_%s);\n", entity_node->name, DataDeskGetTagParameter(entity_node_tag, 1)->name);
			fprintf(c_file, "    %s *unique_entity = &core->world_data->%s;\n", entity_node->name, entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    generic_entity->type = ENTITY_TYPE_%s;\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    generic_entity->unique_entity_id = 1;\n");
			fprintf(c_file, "    unique_entity->entity_id = generic_entity->entity_id;\n\n");

			DataDeskNode *component_list_tag = DataDeskGetNodeTag(entity_node->struct_declaration.first_member, "ComponentList");
			i32 tag_index = 0;
			DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
			while (tag_param)
			{
				fprintf(c_file, "    Add%sComponent(generic_entity);\n", tag_param->name);
				tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
			}

			fprintf(c_file, "\n    return unique_entity;\n");
			fprintf(c_file, "}\n\n");

			// Don't really need to delete singular entities
		}
	}
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

internal void GenerateSerialisationCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;
	if (!h_file || !c_file)
		return;

	for (i32 i = 0; i < serialisable_struct_count; i++)
	{
		DataDeskNode *root = serialisable_structs[i];

		// NOTE(tjr): Write to file
		{
			fprintf(h_file, "static void Write%sToFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Write%sToFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (!DataDeskNodeHasTag(member, "ComponentList"))
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

					if (member->declaration.type->type_usage.first_array_size_expression) // NOTE(tjr): Array type
					{
						if (!(member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
							  member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
							fprintf(c_file, "\nuhhhhh\n");

						if (member->declaration.type->type_usage.first_array_size_expression->next) // NOTE(tjr): 2D Array
						{
							fprintf(c_file, "// - 2D Arary %s %s\n", member->declaration.type->type_usage.first_array_size_expression->string, member->declaration.type->type_usage.first_array_size_expression->next->string);
						}
						else // NOTE(tjr): 1D Array
						{
							fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");

							if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(tjr): Array of pointers
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");

								fprintf(c_file, "        // '%s' pointer array in %s\n", member->name, root->name);
								fprintf(c_file, "        i32 pos = ftell(file);\n");
								fprintf(c_file, "        R_DEV_ASSERT(pos != -1, \"Uh oh.\");\n");
								fprintf(c_file, "        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, \"Max pointers reached. Consider a better design?\");\n");
								fprintf(c_file, "        SerialisationPointer ptr = {&(data->%s[i]), pos};\n", member->name);
								fprintf(c_file, "        serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
								fprintf(c_file, "        i32 empty = INT_MAX;\n");
								fprintf(c_file, "        WriteToFile(file, &empty, sizeof(i32));\n");
							}
							else
							{
								if (is_complex) // NOTE(tjr): Array of complex types
								{
									fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
									fprintf(c_file, "        Write%sToFile(file, &(data->%s[i]));\n", member->declaration.type->name, member->name);
								}
								else // NOTE(tjr): Array of primative types
								{
									fprintf(c_file, "        // '%s' array in %s\n", member->name, root->name);
									fprintf(c_file, "        WriteToFile(file, &data->%s[i], sizeof(%s));\n", member->name, member->declaration.type->name);
								}
							}
							fprintf(c_file, "    }\n\n");
						}
					}
					else // NOTE(tjr): Singular data type
					{
						if (member->declaration.type->type_usage.pointer_count > 0) // NOTE(tjr): Pointer
						{
							if (member->declaration.type->type_usage.pointer_count > 1)
								fprintf(c_file, "No support for double or > pointers yet.\n");

							fprintf(c_file, "    // '%s' pointer in %s\n", member->name, root->name);
							fprintf(c_file, "    {\n");
							fprintf(c_file, "        i32 pos = ftell(file);\n");
							fprintf(c_file, "        R_DEV_ASSERT(pos != -1, \"Uh oh.\");\n");
							fprintf(c_file, "        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, \"Max pointers reached. Consider a better design?\");\n");
							fprintf(c_file, "        SerialisationPointer ptr = {&data->%s, pos};\n", member->name);
							fprintf(c_file, "        serialisation_pointers[serialisation_pointer_count++] = ptr;\n");
							fprintf(c_file, "        i32 empty = INT_MAX;\n");
							fprintf(c_file, "        WriteToFile(file, &empty, sizeof(i32));\n");
							fprintf(c_file, "    }\n\n");
						}
						else
						{
							if (is_complex) // NOTE(tjr): Singular complex type
							{
								// A complex structure should already have its own serialisation function
								fprintf(c_file, "    // '%s' in %s\n", member->name, root->name);
								fprintf(c_file, "    Write%sToFile(file, &data->%s);\n\n", member->declaration.type->name, member->name);
							}
							else // NOTE(tjr): Singular primative type
							{
								// Just directly read/write the data-structure, since it just consists of primatives.
								fprintf(c_file, "    // '%s' in %s\n", member->name, root->name);
								fprintf(c_file, "    WriteToFile(file, &data->%s, sizeof(data->%s));\n\n", member->name, member->name);
							}
						}
					}
				}
			}
			fprintf(c_file, "}\n\n");
		}

		// NOTE(tjr): Fill pointers in file
		{
			fprintf(h_file, "static void Fill%sPointersInFile(FILE *file, %s *data);\n\n", root->name, root->name);
			fprintf(c_file, "static void Fill%sPointersInFile(FILE *file, %s *data)\n", root->name, root->name);
			fprintf(c_file, "{\n");
			for (DataDeskNode *member = root->struct_declaration.first_member;
				 member; member = member->next)
			{
				if (!DataDeskNodeHasTag(member, "ComponentList"))
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

					/* if (member->declaration.type->type_usage.first_array_size_expression)
					{
						if (!(member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_identifier ||
							  member->declaration.type->type_usage.first_array_size_expression->type == DATA_DESK_NODE_TYPE_numeric_constant))
							fprintf(c_file, "\nuhhhhh\n");

						if (member->declaration.type->type_usage.first_array_size_expression->next)
						{
							// 2d array
							fprintf(c_file, "// - 2D Arary %s %s\n", member->declaration.type->type_usage.first_array_size_expression->string, member->declaration.type->type_usage.first_array_size_expression->next->string);
						}
						else
						{
							fprintf(c_file, "// - Arary %s\n", member->declaration.type->type_usage.first_array_size_expression->string);

							fprintf(c_file, "    for (i32 i = 0; i < %s; i++)\n", member->declaration.type->type_usage.first_array_size_expression->string);
							fprintf(c_file, "    {\n");

							if (member->declaration.type->type_usage.pointer_count > 0)
							{
								if (member->declaration.type->type_usage.pointer_count > 1)
									fprintf(c_file, "No support for double or > pointers yet.\n");

								fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n");
							}
							else
							{
								if (is_complex)
								{
									fprintf(c_file, "        for (i32 j = 0; j < serialisation_pointer_count; j++)\n");
									fprintf(c_file, "        {\n");
									fprintf(c_file, "            SerialisationPointer *ptr = &serialisation_pointers[j];\n");
									fprintf(c_file, "            if (*ptr->pointer_address == &(data->%s[i]))\n", member->name);
									fprintf(c_file, "            {\n");
									/* fprintf(c_file, "                i32 current_pos = ftell(file);\n");
									fprintf(c_file, "        R_DEV_ASSERT(current_pos != -1, \"Uh oh.\");\n"); ..
									fprintf(c_file, "                fseek(file, ptr->offset, SEEK_SET);\n");
									fprintf(c_file, "                WriteToFile(file, &current_pos, sizeof(i32));\n");
									fprintf(c_file, "                fseek(file, current_pos, SEEK_SET);\n");
									fprintf(c_file, "            }\n");
									fprintf(c_file, "        }\n");
									fprintf(c_file, "        Fill%sPointersInFile(file, &(data->%s[i]));\n", member->declaration.type->name, member->name);
								}
								else
								{
									fprintf(c_file, "        for (i32 j = 0; j < serialisation_pointer_count; j++)\n");
									fprintf(c_file, "        {\n");
									fprintf(c_file, "            SerialisationPointer *ptr = &serialisation_pointers[j];\n");
									fprintf(c_file, "            if (*ptr->pointer_address == &data->%s[i])\n", member->name);
									fprintf(c_file, "            {\n");
									/* fprintf(c_file, "                i32 current_pos = ftell(file);\n");
									fprintf(c_file, "        R_DEV_ASSERT(current_pos != -1, \"Uh oh.\");\n"); ..
									fprintf(c_file, "                fseek(file, ptr->offset, SEEK_SET);\n");
									fprintf(c_file, "                WriteToFile(file, &current_pos, sizeof(i32));\n");
									fprintf(c_file, "                fseek(file, current_pos, SEEK_SET);\n");
									fprintf(c_file, "            }\n");
									fprintf(c_file, "        }\n");
									fprintf(c_file, "        fseek(file, sizeof(%s), SEEK_CUR);\n", member->declaration.type->name);
								}
							}
							fprintf(c_file, "    }\n\n");
						}
					}
					else
					{
						if (member->declaration.type->type_usage.pointer_count > 0)
						{
							if (member->declaration.type->type_usage.pointer_count > 1)
								fprintf(c_file, "No support for double or > pointers yet.\n");

							fprintf(c_file, "    fseek(file, sizeof(i32), SEEK_CUR);\n\n");
						}
						else
						{
							if (is_complex)
							{
								fprintf(c_file, "    for (i32 i = 0; i < serialisation_pointer_count; i++)\n");
								fprintf(c_file, "    {\n");
								fprintf(c_file, "        SerialisationPointer *ptr = &serialisation_pointers[i];\n");
								fprintf(c_file, "        if (*ptr->pointer_address == &data->%s)\n", member->name);
								fprintf(c_file, "        {\n");
								/* fprintf(c_file, "            i32 current_pos = ftell(file);\n");
								fprintf(c_file, "        R_DEV_ASSERT(current_pos != -1, \"Uh oh.\");\n"); ..
								fprintf(c_file, "            fseek(file, ptr->offset, SEEK_SET);\n");
								fprintf(c_file, "            WriteToFile(file, &current_pos, sizeof(i32));\n");
								fprintf(c_file, "            fseek(file, current_pos, SEEK_SET);\n");
								fprintf(c_file, "        }\n");
								fprintf(c_file, "    }\n");
								fprintf(c_file, "    Fill%sPointersInFile(file, &data->%s);\n\n", member->declaration.type->name, member->name);
							}
							else
							{
								fprintf(c_file, "    for (i32 i = 0; i < serialisation_pointer_count; i++)\n");
								fprintf(c_file, "    {\n");
								fprintf(c_file, "        SerialisationPointer *ptr = &serialisation_pointers[i];\n");
								fprintf(c_file, "        if (*ptr->pointer_address == &data->%s)\n", member->name);
								fprintf(c_file, "        {\n");
								/* fprintf(c_file, "            i32 current_pos = ftell(file);\n");
								fprintf(c_file, "        R_DEV_ASSERT(current_pos != -1, \"Uh oh.\");\n"); ..
								fprintf(c_file, "            fseek(file, ptr->offset, SEEK_SET);\n");
								fprintf(c_file, "            WriteToFile(file, &current_pos, sizeof(i32));\n");
								fprintf(c_file, "            fseek(file, current_pos, SEEK_SET);\n");
								fprintf(c_file, "        }\n");
								fprintf(c_file, "    }\n");
								fprintf(c_file, "    fseek(file, sizeof(data->%s), SEEK_CUR);\n\n", member->name);
							}
						}
					} */
				}
			}
			fprintf(c_file, "}\n\n");
		}
	}
}