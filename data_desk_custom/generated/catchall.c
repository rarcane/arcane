static char *GetEntityTypeName(EntityType type)
{
switch(type)
{
case ENTITY_TYPE_undefined:
return "Undefined";
break;
case ENTITY_TYPE_character:
return "Character";
break;
case ENTITY_TYPE_monster:
return "Monster";
break;
case ENTITY_TYPE_animal:
return "Animal";
break;
case ENTITY_TYPE_item:
return "Item";
break;
case ENTITY_TYPE_storage:
return "Storage";
break;
case ENTITY_TYPE_resource:
return "Resource";
break;
case ENTITY_TYPE_scenic:
return "Scenic";
break;
case ENTITY_TYPE_ground:
return "Ground";
break;
default:
return "INVALID";
break;
}
}

