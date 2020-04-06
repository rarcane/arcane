#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_desk.h"

static FILE *global_catchall_header;
static FILE *global_catchall_implementation;

static int component_count = 0;
static DataDeskNode *components[1024];
static void GenerateComponentCode(void);

static int unique_entity_count = 0;
static DataDeskNode *unique_entities[1024];
static void GenerateUniqueEntityArrays();
static void GenerateEntityCode();

static int xmacro_count = 0;
static DataDeskNode *xmacro_nodes[128];
static DataDeskNode *GetXMacroNode(const char *name);

static void GeneratePrintUICodeForAST(FILE *file, DataDeskNode *root, char *access_string);

DATA_DESK_FUNC void
DataDeskCustomInitCallback(void)
{
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
		if (DataDeskNodeHasTag(root, "Component"))
		{
			components[component_count++] = root;
		}
		else if (DataDeskNodeHasTag(root, "UniqueEntity"))
		{
			unique_entities[unique_entity_count++] = root;

			DataDeskNode *tag = DataDeskGetNodeTag(root, "UniqueEntity");
			fprintf(h_file, "#define MAX_%s_ENTITIES (%s)\n", root->name_uppercase_with_underscores, DataDeskGetTagParameter(tag, 0)->name);
			fprintf(h_file, "typedef struct %sEntity\n", root->name);
			fprintf(h_file, "{\n");

			fprintf(h_file, "Entity *parent_generic_entity;\n");
			if (atoi(DataDeskGetTagParameter(tag, 0)->name) > 1)
			{
				fprintf(h_file, "i32 unique_entity_id;\n");
			}

			for (DataDeskNode *member = root->struct_declaration.first_member; member; member = member->next)
			{
				DataDeskNode *component_list_tag = DataDeskGetNodeTag(member, "ComponentList");
				if (component_list_tag)
				{
					int tag_index = 0;
					DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
					while (tag_param)
					{
						fprintf(h_file, "    %sComponent *%s_comp;\n", tag_param->name, tag_param->name_lowercase_with_underscores);
						tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
					}
				}
				else
				{
					DataDeskFWriteGraphAsC(h_file, member, 0);
					fprintf(h_file, ";\n");
				}
			}

			fprintf(h_file, "} %sEntity;\n\n", root->name);
		}
		else if (DataDeskNodeHasTag(root, "GenerateComponentCode"))
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
			default:
			{
				DataDeskFWriteGraphAsC(h_file, root, 0);
				break;
			}

			case DATA_DESK_NODE_TYPE_struct_declaration:
			{
				fprintf(h_file, "typedef struct %s\n", root->string);
				fprintf(h_file, "{\n");
				for (DataDeskNode *member = root->struct_declaration.first_member;
					 member; member = member->next)
				{
					if (DataDeskNodeHasTag(member, "GenerateUniqueEntityArrays"))
					{
						GenerateUniqueEntityArrays();
					}

					_DataDeskFWriteGraphAsC(h_file, member, 0, 1);
					fprintf(h_file, ";\n");
				}
				fprintf(h_file, "} %s;\n\n", root->string);

				if (DataDeskNodeHasTag(root, "XMacro"))
				{
					xmacro_nodes[xmacro_count++] = root;
				}

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

					fprintf(h_file, "static char *Get%sTypeName(%s type);\n\n", root->name, root->name);
				}

				// NOTE(tjr): X-Macro initialisation
				if (DataDeskNodeHasTag(root, "XMacro"))
				{
					char name[50];
					sprintf(name, "%sData", root->name);
					DataDeskNode *macro_data_node = GetXMacroNode(name);
					if (macro_data_node)
					{
						int string_length = 0;
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
							int i = 0;
							for (DataDeskNode *member = macro_data_node->struct_declaration.first_member; member; member = member->next)
							{
								DataDeskNode *ignore_tag = DataDeskGetNodeTag(member, "Ignore");
								if (!ignore_tag)
								{
									DataDeskNode *tag_param = DataDeskGetTagParameter(data_tag, i);

									for (int i = 0; tag_param->name[i]; i++)
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

						/* fprintf(h_file, "static void Initialise%sData();\n", root->name);

						fprintf(c_file, "static void Initialise%sData()\n", root->name);
						fprintf(c_file, "{\n");
						for (DataDeskNode *field = root->enum_declaration.first_constant; field; field = field->next)
						{
							DataDeskNode *data_tag = DataDeskGetNodeTag(field, "Data");

							int i = 0;
							for (DataDeskNode *member = macro_data_node->struct_declaration.first_member; member; member = member->next)
							{
								fprintf(c_file, "%s_data[%s_%s].%s = %s;\n", root->name_lowercase_with_underscores, root->name_uppercase_with_underscores, field->name, member->name, DataDeskGetTagParameter(data_tag, i)->name);
								i++;
							}
						}
						fprintf(c_file, "}\n\n"); */
					}
				}

				// NOTE(tjr): Generate enum print function implementation.
				{
					fprintf(c_file, "static char *Get%sName(%s type)\n", root->name, root->name);
					fprintf(c_file, "{\n");
					fprintf(c_file, "switch(type)\n");
					fprintf(c_file, "{\n");
					for (DataDeskNode *field = root->enum_declaration.first_constant; field; field = field->next)
					{
						fprintf(c_file, "case %s_%s:\n", root->name_uppercase_with_underscores, field->string);
						fprintf(c_file, "return \"");

						// NOTE(tjr): Make enum name look pretty.
						int string_length = 0;
						for (; field->string[string_length]; ++string_length)
						{
						}

						for (int i = 0; i < string_length && field->string[i]; ++i)
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
				int count = 0;
				for (DataDeskNode *field = root->flags_declaration.first_flag; field; field = field->next)
				{
					fprintf(h_file, "#define %s_%s (1<<%i)\n", root->name_uppercase_with_underscores, field->string, count);
					count++;
				}
				fprintf(h_file, "typedef unsigned int %s;\n\n", root->name);

				break;
			}
			}
		}
	}
}

DATA_DESK_FUNC void
DataDeskCustomCleanUpCallback(void)
{
	fclose(global_catchall_header);
	fclose(global_catchall_implementation);
}

static void
GenerateComponentCode(void)
{
	// NOTE(tjr): Generate component struct declarations
	{
		FILE *file = global_catchall_header;
		for (int i = 0; i < component_count; ++i)
		{
			DataDeskNode *root = components[i];
			fprintf(file, "typedef struct %sComponent\n", root->name);
			fprintf(file, "{\n");
			fprintf(file, "Entity *parent_entity;\n");
			fprintf(file, "i32 component_id;\n");

			for (DataDeskNode *member = root->struct_declaration.first_member; member; member = member->next)
			{
				DataDeskFWriteGraphAsC(file, member, 0);
				fprintf(file, ";\n");
			}

			fprintf(file, "} %sComponent;\n\n", root->name);
		}
	}

	// NOTE(rjf): Generate component enums
	{
		FILE *file = global_catchall_header;

		fprintf(file, "typedef enum ComponentType\n");
		fprintf(file, "{\n");
		fprintf(file, "COMPONENT_INVALID,\n");
		for (int i = 0; i < component_count; i++)
		{
			fprintf(file, "COMPONENT_%s,\n", components[i]->name_lowercase_with_underscores);
		}
		fprintf(file, "COMPONENT_MAX,\n");
		fprintf(file, "} ComponentType;\n\n");
	}

	// NOTE(tjr): Generate component set structure.
	{
		FILE *file = global_catchall_header;

		fprintf(file, "typedef struct ComponentSet\n");
		fprintf(file, "{\n");
		for (int i = 0; i < component_count; i++)
		{
			fprintf(file, "%sComponent %s_components[MAX_ACTIVE_ENTITIES];\n", components[i]->name, components[i]->name_lowercase_with_underscores);
			fprintf(file, "i32 %s_component_count;\n", components[i]->name_lowercase_with_underscores);
			fprintf(file, "i32 %s_free_component_id;\n", components[i]->name_lowercase_with_underscores);
		}
		fprintf(file, "} ComponentSet;\n\n");
	}

	{
		FILE *file = global_catchall_implementation;
		if (file)
		{
			// NOTE(tjr): Generate Component UI print function.
			{
				fprintf(file, "internal void PrintComponentDataUI(void *component_data, ComponentType type)\n");
				fprintf(file, "{\n");
				fprintf(file, "    switch (type)\n");
				fprintf(file, "    {\n");
				fprintf(file, "    case COMPONENT_INVALID :\n");
				fprintf(file, "    case COMPONENT_MAX :\n");
				fprintf(file, "        R_BREAK(\"Invalid component.\")\n");
				fprintf(file, "        break;\n\n");
				for (int i = 0; i < component_count; i++)
				{
					fprintf(file, "    case COMPONENT_%s :\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    {\n");
					fprintf(file, "        %sComponent *component = (%sComponent*)component_data;\n", components[i]->name, components[i]->name);
					GeneratePrintUICodeForAST(file, components[i], "component->");
					fprintf(file, "        break;\n");
					fprintf(file, "    }\n\n");
				}
				fprintf(file, "    }\n");
				fprintf(file, "}\n\n");
			}

			for (int i = 0; i < component_count; i++)
			{
				// NOTE(tjr): Add Component function.
				{
					fprintf(file, "internal %sComponent *Add%sComponent(Entity *entity)\n", components[i]->name, components[i]->name);
					fprintf(file, "{\n");
					fprintf(file, "    i32 component_id;\n");
					fprintf(file, "    if (core->world_data->entity_components.%s_free_component_id == core->world_data->entity_components.%s_component_count)\n", components[i]->name_lowercase_with_underscores, components[i]->name_lowercase_with_underscores);
					fprintf(file, "    {\n");
					fprintf(file, "        component_id = core->world_data->entity_components.%s_component_count;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "        core->world_data->entity_components.%s_component_count++;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "        core->world_data->entity_components.%s_free_component_id = component_id + 1;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    }\n");
					fprintf(file, "    else\n");
					fprintf(file, "    {\n");
					fprintf(file, "        component_id = core->world_data->entity_components.%s_free_component_id;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    }\n\n");

					fprintf(file, "    core->world_data->entity_components.%s_components[component_id] = GetDefault%sComponent();\n", components[i]->name_lowercase_with_underscores, components[i]->name);
					fprintf(file, "    entity->components[COMPONENT_%s] = &core->world_data->entity_components.%s_components[component_id];\n", components[i]->name_lowercase_with_underscores, components[i]->name_lowercase_with_underscores);
					fprintf(file, "    core->world_data->entity_components.%s_components[component_id].parent_entity = entity;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    core->world_data->entity_components.%s_components[component_id].component_id = component_id;\n\n", components[i]->name_lowercase_with_underscores);

					fprintf(file, "    for (int i = 0; i < core->world_data->entity_components.%s_component_count + 1; i++)\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    {\n");
					fprintf(file, "        if (!core->world_data->entity_components.%s_components[i].parent_entity)\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "        {\n");
					fprintf(file, "            core->world_data->entity_components.%s_free_component_id = i;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "            break;\n");
					fprintf(file, "        }\n");
					fprintf(file, "    }\n\n");

					fprintf(file, "    return &core->world_data->entity_components.%s_components[component_id];\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "}\n\n");
				}

				// NOTE(tjr): Remove Component function.
				{
					fprintf(file, "internal void Remove%sComponent(Entity *entity)\n", components[i]->name);
					fprintf(file, "{\n");
					fprintf(file, "    %sComponent *component = entity->components[COMPONENT_%s];\n", components[i]->name, components[i]->name_lowercase_with_underscores);
					fprintf(file, "    R_DEV_ASSERT(component, \"Entity does not a %sComponent attached, so it can't remove it.\");\n\n", components[i]->name);

					fprintf(file, "    i32 deleted_component_id = component->component_id;\n");
					fprintf(file, "    %sComponent empty_comp = {0};\n", components[i]->name);
					fprintf(file, "    core->world_data->entity_components.%s_components[deleted_component_id] = empty_comp;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "    entity->components[COMPONENT_%s] = 0;\n\n", components[i]->name_lowercase_with_underscores);

					fprintf(file, "    if (deleted_component_id < core->world_data->entity_components.%s_free_component_id)\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "        core->world_data->entity_components.%s_free_component_id = deleted_component_id;\n", components[i]->name_lowercase_with_underscores);
					fprintf(file, "}\n\n");
				}
			}

			// NOTE(tjr): ECS delete entity.
			fprintf(file, "internal void DeleteEntity(Entity *entity)\n");
			fprintf(file, "{\n");
			for (int i = 0; i < component_count; i++)
			{
				fprintf(file, "    %sComponent *%s_component = entity->components[%i];\n", components[i]->name, components[i]->name_lowercase_with_underscores, i + 1);
				fprintf(file, "    if (%s_component)\n", components[i]->name_lowercase_with_underscores);
				fprintf(file, "        Remove%sComponent(entity);\n", components[i]->name);
			}

			fprintf(file, "\n    i32 deleted_entity_id = entity->entity_id;\n");
			fprintf(file, "    Entity empty_entity = {0};\n");
			fprintf(file, "    *entity = empty_entity;\n");
			fprintf(file, "    if (deleted_entity_id < core->world_data->free_entity_id)\n");
			fprintf(file, "        core->world_data->free_entity_id = deleted_entity_id;\n");
			fprintf(file, "}\n\n");
		}
	}
}

static void GenerateUniqueEntityArrays()
{
	FILE *file = global_catchall_header;
	fprintf(file, "\n");
	for (int i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		if (atoi(DataDeskGetTagParameter(entity_node_tag, 0)->name) > 1)
		{
			fprintf(file, "%sEntity %s_entities[MAX_%s_ENTITIES];\n",
					entity_node->name,
					entity_node->name_lowercase_with_underscores,
					entity_node->name_uppercase_with_underscores);
			fprintf(file, "i32 %s_entity_count;\n", entity_node->name_lowercase_with_underscores);
			fprintf(file, "i32 free_%s_entity_index;\n", entity_node->name_lowercase_with_underscores);
		}
		else
		{
			fprintf(file, "%sEntity %s_entity;\n", entity_node->name, entity_node->name_lowercase_with_underscores);
		}
	}
	fprintf(file, "\n");
}

static void GenerateEntityCode()
{
	FILE *h_file = global_catchall_header;
	FILE *c_file = global_catchall_implementation;

	// NOTE(tjr): Unique entity type enum.
	fprintf(h_file, "typedef enum EntityType\n");
	fprintf(h_file, "{\n");
	fprintf(h_file, "    ENTITY_TYPE_generic,\n");
	for (int i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		fprintf(h_file, "    ENTITY_TYPE_%s,\n", entity_node->name_lowercase_with_underscores);
	}
	fprintf(h_file, "    ENTITY_TYPE_MAX\n");
	fprintf(h_file, "} EntityType;\n\n");

	for (int i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		if (atoi(DataDeskGetTagParameter(entity_node_tag, 0)->name) > 1)
		{
			// NOTE(tjr): New unique entity
			fprintf(c_file, "static %sEntity *New%sEntity()\n", entity_node->name, entity_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    R_DEV_ASSERT(core->world_data->free_%s_entity_index + 1 < MAX_%s_ENTITIES, \"Maximum amount of %s entites reached\");\n\n", entity_node->name_lowercase_with_underscores, entity_node->name_uppercase_with_underscores, entity_node->name);

			fprintf(c_file, "    i32 new_unique_id = core->world_data->free_%s_entity_index;\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    if (core->world_data->free_%s_entity_index == core->world_data->%s_entity_count)\n", entity_node->name_lowercase_with_underscores, entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    {\n");
			fprintf(c_file, "        core->world_data->%s_entity_count++;\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->world_data->free_%s_entity_index++;\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    }\n");
			fprintf(c_file, "    core->world_data->%s_entities[new_unique_id].unique_entity_id = new_unique_id;\n\n", entity_node->name_lowercase_with_underscores);

			fprintf(c_file, "    Entity *generic_entity = NewEntity(\"%s\", ENTITY_TYPE_%s, GENERALISED_ENTITY_TYPE_%s);\n", entity_node->name, entity_node->name_lowercase_with_underscores, DataDeskGetTagParameter(entity_node_tag, 1)->name);
			fprintf(c_file, "    %sEntity *unique_entity = &core->world_data->%s_entities[new_unique_id];\n", entity_node->name, entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    generic_entity->unique_entity = unique_entity;\n");
			fprintf(c_file, "    unique_entity->parent_generic_entity = generic_entity;\n");
			fprintf(c_file, "    unique_entity->unique_entity_id = new_unique_id;\n\n");
			DataDeskNode *component_list_tag = DataDeskGetNodeTag(entity_node->struct_declaration.first_member, "ComponentList");
			int tag_index = 0;
			DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
			while (tag_param)
			{
				fprintf(c_file, "    unique_entity->%s_comp = Add%sComponent(generic_entity);\n", tag_param->name_lowercase_with_underscores, tag_param->name);
				tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
			}
			fprintf(c_file, "\n    return unique_entity;\n");
			fprintf(c_file, "}\n\n");

			// NOTE(tjr): Delete unique entity
			fprintf(c_file, "static void Delete%sEntity(%sEntity *entity)\n", entity_node->name, entity_node->name);
			fprintf(c_file, "{\n");
			fprintf(c_file, "    DeleteEntity(entity->parent_generic_entity);\n");
			fprintf(c_file, "    if (entity->unique_entity_id < core->world_data->free_%s_entity_index);\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "        core->world_data->free_%s_entity_index = entity->unique_entity_id;\n", entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    %sEntity empty_entity = {0};\n", entity_node->name);
			fprintf(c_file, "    *entity = empty_entity;\n");
			fprintf(c_file, "}\n\n");
		}
		else
		{
			// NOTE(tjr): New singular unique entity
			fprintf(c_file, "static %sEntity *Initialise%sEntity()\n", entity_node->name, entity_node->name);
			fprintf(c_file, "{\n");

			fprintf(c_file, "    Entity *generic_entity = NewEntity(\"%s\", ENTITY_TYPE_%s, GENERALISED_ENTITY_TYPE_%s);\n", entity_node->name, entity_node->name_lowercase_with_underscores, DataDeskGetTagParameter(entity_node_tag, 1)->name);
			fprintf(c_file, "    %sEntity *unique_entity = &core->world_data->%s_entity;\n", entity_node->name, entity_node->name_lowercase_with_underscores);
			fprintf(c_file, "    generic_entity->unique_entity = unique_entity;\n");
			fprintf(c_file, "    unique_entity->parent_generic_entity = generic_entity;\n\n");

			DataDeskNode *component_list_tag = DataDeskGetNodeTag(entity_node->struct_declaration.first_member, "ComponentList");
			int tag_index = 0;
			DataDeskNode *tag_param = DataDeskGetTagParameter(component_list_tag, tag_index);
			while (tag_param)
			{
				fprintf(c_file, "    unique_entity->%s_comp = Add%sComponent(generic_entity);\n", tag_param->name_lowercase_with_underscores, tag_param->name);
				tag_param = DataDeskGetTagParameter(component_list_tag, ++tag_index);
			}

			fprintf(c_file, "\n    return unique_entity;\n");
			fprintf(c_file, "}\n\n");
		}
	}

	// NOTE(tjr): Entity print function.
	fprintf(c_file, "static void PrintEntityDataUI(Entity *entity)\n");
	fprintf(c_file, "{\n");
	fprintf(c_file, "    switch(entity->type)\n");
	fprintf(c_file, "    {\n");
	for (int i = 0; i < unique_entity_count; i++)
	{
		DataDeskNode *entity_node = unique_entities[i];
		DataDeskNode *entity_node_tag = DataDeskGetNodeTag(entity_node, "UniqueEntity");

		fprintf(c_file, "    case ENTITY_TYPE_%s :\n", entity_node->name_lowercase_with_underscores);
		fprintf(c_file, "    {\n");
		fprintf(c_file, "        %sEntity *unique_entity = entity->unique_entity;\n", entity_node->name);

		for (DataDeskNode *member = entity_node->struct_declaration.first_member->next;
			 member; member = member->next)
		{
			GeneratePrintUICodeForAST(c_file, member, "unique_entity->");
		}

		fprintf(c_file, "        break;\n");
		fprintf(c_file, "    }\n");
	}
	fprintf(c_file, "    }\n\n");
	fprintf(c_file, "    for (int i = 1; i < COMPONENT_MAX; i++)\n");
	fprintf(c_file, "    {\n");
	fprintf(c_file, "        if (entity->components[i])\n");
	fprintf(c_file, "        {\n");
	fprintf(c_file, "            PrintComponentDataUI(entity->components[i], i);\n");
	fprintf(c_file, "        }\n");
	fprintf(c_file, "    }\n");
	fprintf(c_file, "}\n");
}

static DataDeskNode *GetXMacroNode(const char *name)
{
	for (int i = 0; i < xmacro_count; i++)
	{
		if (strcmp(xmacro_nodes[i]->name, name) == 0)
		{
			return xmacro_nodes[i];
		}
	}

	return 0;
}

static void GeneratePrintUICodeForAST(FILE *file, DataDeskNode *root, char *access_string)
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
			if (DataDeskDeclarationIsType(root, "int") ||
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
