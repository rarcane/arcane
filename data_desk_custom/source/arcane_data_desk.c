#include <stdio.h>
#include <string.h>
#include "data_desk.h"

typedef struct Component
{
	char *name;
	char *name_lowercase_with_underscores;
	char *name_uppercase_with_underscores;
	char *name_upper_camel_case;
	char *name_lower_camel_case;
	DataDeskASTNode *root;
} Component;

static int component_count = 0;
static Component components[500];

static FILE *global_catchall_header;
static FILE *global_catchall_implementation;

static void GeneratePrintCodeForAST(FILE *file, DataDeskASTNode *root, char *access_string);

DATA_DESK_FUNC void
DataDeskCustomInitCallback(void)
{
	global_catchall_header = fopen("catchall.h", "w");
	global_catchall_implementation = fopen("catchall.c", "w");
}

DATA_DESK_FUNC void
DataDeskCustomFileCallback(DataDeskASTNode *root, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomConstantCallback(DataDeskConstant constant_info, char *filename)
{
	if (DataDeskNodeHasTag(constant_info.root, "Define"))
	{
		FILE *file = global_catchall_header;
		if (file)
		{
			fprintf(file, "#define %s %s\n\n", constant_info.name, constant_info.root->constant_definition.expression->string);
		}
	}
}

DATA_DESK_FUNC void
DataDeskCustomStructCallback(DataDeskStruct struct_info, char *filename)
{
	if (DataDeskNodeHasTag(struct_info.root, "Component"))
	{
		// NOTE(tjr): Generate component struct declaration
		{
			if (component_count < sizeof(components) / sizeof(components[0]))
			{
				Component component = {
					struct_info.name,
					struct_info.name_lowercase_with_underscores,
					struct_info.name_uppercase_with_underscores,
					struct_info.name_upper_camel_case,
					struct_info.name_lower_camel_case,
					struct_info.root,
				};
				components[component_count++] = component;
			}

			FILE *file = global_catchall_header;
			if (file)
			{
				fprintf(file, "typedef struct %sComponent\n", struct_info.name);
				fprintf(file, "{\n");
				fprintf(file, "i32 entity_id;\n");
				fprintf(file, "i32 component_id;\n");
				for (DataDeskASTNode *member = struct_info.root->struct_declaration.first_member;
					 member;
					 member = member->next)
				{
					_DataDeskFWriteASTFromRootAsC(file, member, 0, 1);
					fprintf(file, ";\n");
					// fprintf(file, "%s %s;\n", member->declaration.type->string, member->string);
				}
				fprintf(file, "} %sComponent;\n\n", struct_info.name);
			}
		}
	}
	else
	{
		DataDeskFWriteStructAsC(global_catchall_header, struct_info);
	}
}

DATA_DESK_FUNC void
DataDeskCustomEnumCallback(DataDeskEnum enum_info, char *filename)
{
	// MOTE(tjr): Generate styled enum.
	{
		FILE *file = global_catchall_header;
		if (file)
		{
			fprintf(file, "typedef enum %s %s;\n", enum_info.name, enum_info.name);
			//DataDeskFWriteEnumAsC(global_catchall_header, enum_info);
			fprintf(file, "enum %s\n", enum_info.name);
			fprintf(file, "{\n");
			for (DataDeskASTNode *field = enum_info.root->enum_declaration.first_constant; field; field = field->next)
			{
				fprintf(file, "%s_%s,\n", enum_info.name_uppercase_with_underscores, field->string);
			}
			fprintf(file, "%s_MAX,\n", enum_info.name_uppercase_with_underscores);
			fprintf(file, "};\n");

			fprintf(file, "static char *Get%sTypeName(%s type);\n\n", enum_info.name, enum_info.name);
		}
	}

	// NOTE(tjr): Generate enum print function implementation.
	{
		FILE *file = global_catchall_implementation;
		if (file)
		{
			fprintf(file, "static char *Get%sName(%s type)\n", enum_info.name, enum_info.name);
			fprintf(file, "{\n");
			fprintf(file, "switch(type)\n");
			fprintf(file, "{\n");
			for (DataDeskASTNode *field = enum_info.root->enum_declaration.first_constant; field; field = field->next)
			{
				fprintf(file, "case %s_%s:\n", enum_info.name_uppercase_with_underscores, field->string);
				fprintf(file, "return \"");

				// NOTE(tjr): Make enum name look pretty.
				int string_length = 0;
				for (; field->string[string_length]; ++string_length)
					;
				for (int i = 0; i < string_length && field->string[i]; ++i)
				{
					if (field->string[i] != '_')
					{
						if (i == 0)
						{
							fprintf(file, "%c", DataDeskCharToUpper(field->string[i]));
						}
						else
						{
							fprintf(file, "%c", field->string[i]);
							if (field->string[i + 1] == '_')
							{
								fprintf(file, " ");
								fprintf(file, "%c", DataDeskCharToUpper(field->string[i + 2]));
								i = i + 2;
							}
						}
					}
				}

				fprintf(file, "\";\n");
				fprintf(file, "break;\n");
			}
			fprintf(file, "default:\n");
			fprintf(file, "return \"INVALID\";\n");
			fprintf(file, "break;\n");
			fprintf(file, "}\n");
			fprintf(file, "}\n\n");
		}
	}
}

DATA_DESK_FUNC void
DataDeskCustomFlagsCallback(DataDeskFlags flags_info, char *filename)
{
	FILE *file = global_catchall_header;
	if (file)
	{
		int count = 0;
		for (DataDeskASTNode *field = flags_info.root->enum_declaration.first_constant; field; field = field->next)
		{
			fprintf(file, "#define %s_%s (1<<%i)\n", flags_info.name_uppercase_with_underscores, field->string, count);
			count++;
		}
		fprintf(file, "typedef unsigned int %s;\n\n", flags_info.name);
	}
}

DATA_DESK_FUNC void
DataDeskCustomDeclarationCallback(DataDeskDeclaration declaration_info, char *filename) {}

DATA_DESK_FUNC void
DataDeskCustomCleanUpCallback(void)
{
	{
		FILE *file = global_catchall_header;
		if (file)
		{
			// NOTE(tjr): Generate component enums.
			fprintf(file, "typedef enum ComponentType\n");
			fprintf(file, "{\n");
			fprintf(file, "COMPONENT_INVALID,\n");
			for (int i = 0; i < component_count; i++)
			{
				Component *component = &components[i];
				fprintf(file, "COMPONENT_%s,\n", component->name_lowercase_with_underscores);
			}
			fprintf(file, "COMPONENT_MAX,\n");
			fprintf(file, "} ComponentType;\n\n");

			// NOTE(tjr): Generate component set.
			fprintf(file, "typedef struct ComponentSet\n");
			fprintf(file, "{\n");
			for (int i = 0; i < component_count; i++)
			{
				Component *component = &components[i];

				fprintf(file, "%sComponent %s_components[MAX_ENTITIES];\n", component->name, component->name_lowercase_with_underscores);
				fprintf(file, "i32 %s_component_count;\n", component->name_lowercase_with_underscores);
				fprintf(file, "i32 %s_free_component_id;\n", component->name_lowercase_with_underscores);
			}
			fprintf(file, "} ComponentSet;\n\n");
		}
	}

	{
		FILE *file = global_catchall_implementation;
		if (file)
		{
			for (int i = 0; i < component_count; i++)
			{
				Component *component = &components[i];

				// NOTE(tjr): Add Component function.
				{
					fprintf(file, "internal void Add%sComponent(Entity *entity, void *component_data)\n", component->name);
					fprintf(file, "{\n");
					fprintf(file, "    i32 component_id;\n");
					fprintf(file, "    if (core->component_set->%s_free_component_id == core->component_set->%s_component_count)\n", component->name_lowercase_with_underscores, component->name_lowercase_with_underscores);
					fprintf(file, "    {\n");
					fprintf(file, "        component_id = core->component_set->%s_component_count;\n", component->name_lowercase_with_underscores);
					fprintf(file, "        core->component_set->%s_component_count++;\n", component->name_lowercase_with_underscores);
					fprintf(file, "        core->component_set->%s_free_component_id = component_id + 1;\n", component->name_lowercase_with_underscores);
					fprintf(file, "    }\n");
					fprintf(file, "    else\n");
					fprintf(file, "    {\n");
					fprintf(file, "        component_id = core->component_set->%s_free_component_id;\n", component->name_lowercase_with_underscores);
					fprintf(file, "        for (int i = 1; i < core->component_set->%s_component_count + 1; i++)\n", component->name_lowercase_with_underscores);
					fprintf(file, "        {\n");
					fprintf(file, "            if (core->component_set->%s_components[i].component_id == 0)\n", component->name_lowercase_with_underscores);
					fprintf(file, "            {\n");
					fprintf(file, "                core->component_set->%s_free_component_id = i;\n", component->name_lowercase_with_underscores);
					fprintf(file, "                break;\n");
					fprintf(file, "            }\n");
					fprintf(file, "        }\n");
					fprintf(file, "    }\n\n");

					fprintf(file, "    core->component_set->%s_components[component_id] = *((%sComponent*)component_data);\n", component->name_lowercase_with_underscores, component->name);
					fprintf(file, "    entity->components[COMPONENT_%s] = &core->component_set->%s_components[component_id];\n", component->name_lowercase_with_underscores, component->name_lowercase_with_underscores);
					fprintf(file, "    core->component_set->%s_components[component_id].entity_id = entity->entity_id;\n", component->name_lowercase_with_underscores);
					fprintf(file, "    core->component_set->%s_components[component_id].component_id = component_id;\n", component->name_lowercase_with_underscores);
					fprintf(file, "}\n\n");
				}

				// NOTE(tjr): Remove Component function.
				{
					fprintf(file, "internal void Remove%sComponent(Entity *entity)\n", component->name);
					fprintf(file, "{\n");
					fprintf(file, "    %sComponent *component = entity->components[COMPONENT_%s];\n", component->name, component->name_lowercase_with_underscores);
					fprintf(file, "    R_DEV_ASSERT(component, \"Entity does not a %sComponent attached, so it can't remove it.\");\n\n", component->name);

					fprintf(file, "    i32 deleted_component_id = component->component_id;\n");
					fprintf(file, "    %sComponent empty_comp = {0};\n", component->name);
					fprintf(file, "    core->component_set->%s_components[deleted_component_id] = empty_comp;\n", component->name_lowercase_with_underscores);
					fprintf(file, "    entity->components[COMPONENT_%s] = 0;\n\n", component->name_lowercase_with_underscores);

					fprintf(file, "    if (deleted_component_id < core->component_set->%s_free_component_id)\n", component->name_lowercase_with_underscores);
					fprintf(file, "        core->component_set->%s_free_component_id = deleted_component_id;\n", component->name_lowercase_with_underscores);
					fprintf(file, "}\n\n");
				}
			}

			// NOTE(tjr): ECS delete entity.
			fprintf(file, "internal void DeleteEntity(Entity *entity)\n");
			fprintf(file, "{\n");
			for (int i = 0; i < component_count; i++)
			{
				Component *component = &components[i];

				fprintf(file, "%sComponent *%s_component = entity->components[%i];\n", component->name, component->name_lowercase_with_underscores, i + 1);
				fprintf(file, "if (%s_component)\n", component->name_lowercase_with_underscores);
				fprintf(file, "Remove%sComponent(entity);\n", component->name);
			}
			fprintf(file, "\ni32 deleted_entity_id = entity->entity_id;\n");
			fprintf(file, "Entity empty_entity = {0};\n");
			fprintf(file, "*entity = empty_entity;\n");
			fprintf(file, "if (deleted_entity_id < core->entity_set->free_entity_id)\n");
			fprintf(file, "core->entity_set->free_entity_id = deleted_entity_id;\n");
			fprintf(file, "}\n");
		}
	}
}

/* static void
GeneratePrintCodeForAST(FILE *file, DataDeskASTNode *root, char *access_string)
{
	if (!DataDeskNodeHasTag(root, "DoNotPrint"))
	{
		switch (root->type)
		{
		case DATA_DESK_AST_NODE_TYPE_struct_declaration:
		{
			fprintf(file, "printf(\"{ \");\n");
			for (DataDeskASTNode *field = root->struct_declaration.first_member; field; field = field->next)
			{
				GeneratePrintCodeForAST(file, field, access_string);
			}
			fprintf(file, "printf(\"}\");\n");
			break;
		}

		case DATA_DESK_AST_NODE_TYPE_declaration:
		{
			if (DataDeskDeclarationIsType(root, "int") ||
				DataDeskDeclarationIsType(root, "i32") || DataDeskDeclarationIsType(root, "u32") ||
				DataDeskDeclarationIsType(root, "i16") || DataDeskDeclarationIsType(root, "u16") ||
				DataDeskDeclarationIsType(root, "i8") || DataDeskDeclarationIsType(root, "u8"))
			{
				fprintf(file, "printf(\"%%i, \", %s%s);\n", access_string, root->string);
			}
			else if (DataDeskDeclarationIsType(root, "float") || DataDeskDeclarationIsType(root, "f32") ||
					 DataDeskDeclarationIsType(root, "f64"))
			{
				fprintf(file, "printf(\"%%f, \", %s%s);\n", access_string, root->string);
			}
			else if (DataDeskDeclarationIsType(root, "b32") || DataDeskDeclarationIsType(root, "b16") ||
					 DataDeskDeclarationIsType(root, "b8"))
			{
				fprintf(file, "printf(\"%%s, \", %s%s ? \"true\" : \"false\");\n", access_string, root->string);
			}
			else if (DataDeskDeclarationIsType(root, "*char"))
			{
				fprintf(file, "printf(\"%%s, \", %s%s);\n", access_string, root->string);
			}
			break;
		}

		default:
			break;
		}
	}
} */