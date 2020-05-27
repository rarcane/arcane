internal Entity *NewEntity(char name[], GeneralisedEntityType generalised_type)
{
	Assert(core->run_data->free_entity_id > 0);
    
	i32 new_id = core->run_data->free_entity_id;
    
	Entity *entity = &core->run_data->entities[new_id - 1];
	entity->entity_id = new_id;
	strcpy(entity->name, name);
	entity->generalised_type = generalised_type;
    
	if (core->run_data->entity_count == core->run_data->free_entity_id - 1)
	{
		core->run_data->entity_count++;
		core->run_data->free_entity_id++;
	}
    
	if (core->run_data->entity_count < MAX_ENTITIES)
	{
		if (core->run_data->entity_count != core->run_data->free_entity_id - 1)
		{
			b8 found = 0;
			for (i32 i = 0; i < core->run_data->entity_count + 1; i++)
			{
				if (core->run_data->entities[i].entity_id == 0)
				{
					core->run_data->free_entity_id = i + 1;
					found = 1;
					break;
				}
			}
			Assert(found);
		}
	}
	else
	{
		core->run_data->free_entity_id = 0;
	}
    
	return entity;
}

internal void DeleteEntity(Entity *entity)
{
	Assert(entity && entity->entity_id);
    
	for (i32 i = 1; i < COMPONENT_MAX; i++)
		if (entity->component_ids[i])
        RemoveComponent(entity, i);
    
	if (entity->entity_id < core->run_data->free_entity_id)
		core->run_data->free_entity_id = entity->entity_id;
    
	Entity empty = {0};
	*entity = empty;
}

internal Entity *GetEntityWithID(i32 id)
{
	Assert(id);
	Entity *entity = &core->run_data->entities[id - 1];
	Assert(entity->entity_id == id); // NOTE(randy): Mistmatched IDs
	return entity;
}