static char *GetEntityTypeName(EntityType type)
{
switch(type)
{
case ENTITY_undefined:
return "Undefined";
break;
case ENTITY_character:
return "Character";
break;
case ENTITY_monster:
return "Monster";
break;
case ENTITY_animal:
return "Animal";
break;
case ENTITY_item:
return "Item";
break;
case ENTITY_storage:
return "Storage";
break;
case ENTITY_resource:
return "Resource";
break;
case ENTITY_scenic:
return "Scenic";
break;
case ENTITY_ground:
return "Ground";
break;
default:
return "INVALID";
break;
}
}

void PrintFoo(Foo *object)
{

printf("{ ");
printf("%i, ", object->integer_value);
printf("%s, ", object->boolean_value ? "true" : "false");
printf("%f, ", object->float_value);
printf("}");

}

