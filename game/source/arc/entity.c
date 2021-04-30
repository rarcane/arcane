internal Entity *NewEntity()
{
	Entity *entity = 0;
	for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
	{
		if (!EntityHasProperty(&core->run_data->entities[i], ENTITY_PROPERTY_is_allocated))
		{
			entity = &core->run_data->entities[i];
			entity->testint = i;
			break;
		}
	}
	Assert(entity);
	
	EntitySetProperty(entity, ENTITY_PROPERTY_is_allocated);
	entity->sprite_data.tint = v4u(1.0f);
	entity->sprite_data.scale = v2(1.0f, 1.0f);
	entity->animation_flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat;
	entity->frame_start_time = GetWorldData()->elapsed_world_time;
	entity->interval_mult = 1.0f;
	
	entity->move_speed_mult = 1.0f;
	
	return entity;
}

internal void DeleteEntity(Entity *entity)
{
	if (GetRunData()->selected_entity == entity)
		GetRunData()->selected_entity = 0;
	
	MemorySet(entity, 0, sizeof(Entity));
}

internal b32
EntityHasProperty(Entity *entity, EntityProperty property)
{
	return !!(entity->properties[property/64] & ((u64)1 << (property%64)));
}

internal void
EntitySetProperty(Entity *entity, EntityProperty property)
{
	entity->properties[property/64] |= ((u64)1 << (property%64));
}

internal void
EntityUnsetProperty(Entity *entity, EntityProperty property)
{
	entity->properties[property/64] &= ~((u64)1 << (property%64));
}

internal b32
IncrementEntity(Entity **entity_ptr)
{
	b32 result = 0;
	Entity *entity = *entity_ptr;
	Entity *first_entity = &core->run_data->entities[0];
	
	if (!entity)
	{
		entity = first_entity;
		
		if(EntityHasProperty(entity, ENTITY_PROPERTY_is_allocated))
		{
			*entity_ptr = entity;
			result = !!entity;
			return result;
		}
	}
	
	Entity *next_entity = 0;
	for(u64 index = (entity - first_entity) + 1;
		index < ENTITY_TABLE_SIZE;
		index += 1)
	{
		Entity *candidate = first_entity + index;
		if(EntityHasProperty(candidate, ENTITY_PROPERTY_is_allocated))
		{
			next_entity = candidate;
			break;
		}
	}
	entity = next_entity;
	
	*entity_ptr = entity;
	result = !!entity;
	return result;
}

internal b32
IncrementEntityWithProperty(Entity **entity_ptr, EntityProperty property)
{
	b32 result = 0;
	Entity *entity = *entity_ptr;
	Entity *first_entity = &core->run_data->entities[0];
	
	if (!entity)
	{
		entity = first_entity;
		
		if(EntityHasProperty(entity, ENTITY_PROPERTY_is_allocated) &&
		   EntityHasProperty(entity, property))
		{
			*entity_ptr = entity;
			result = !!entity;
			return result;
		}
	}
	
	Entity *next_entity = 0;
	for(u64 index = (entity - first_entity) + 1;
		index < ENTITY_TABLE_SIZE;
		index += 1)
	{
		Entity *candidate = first_entity + index;
		if(EntityHasProperty(candidate, ENTITY_PROPERTY_is_allocated) &&
		   EntityHasProperty(candidate, property))
		{
			next_entity = candidate;
			break;
		}
	}
	entity = next_entity;
	
	*entity_ptr = entity;
	result = !!entity;
	return result;
}

internal void UpdateEntitySprite(Entity *entity)
{
	if (EntityHasProperty(entity, ENTITY_PROPERTY_tree))
	{
		switch (entity->tree_type)
		{
			case TREE_TYPE_oak :
			{
				entity->sprite_data.sprite = SPRITE_birch_tree1;
			} break;
			case TREE_TYPE_birch :
			{
				entity->sprite_data.sprite = SPRITE_birch_tree2;
			} break;
			case TREE_TYPE_pine :
			{
				entity->sprite_data.sprite = SPRITE_pine_tree_v1;
			} break;
		}
	}
}

internal void PrintEntityFields(Entity *entity)
{
	Assert(entity && EntityHasProperty(entity, ENTITY_PROPERTY_is_allocated));
	
	TsUITitle(entity->debug_name); // TODO(randy): Make this editable
	
	if (TsUICollapsable("Properties"))
	{
		for (i32 i = 1; i < ENTITY_PROPERTY_MAX; i++)
		{
			if (TsUIToggler(GetEntityPropertyName(i), EntityHasProperty(entity, i)))
			{
				EntitySetProperty(entity, i);
			}
			else
			{
				EntityUnsetProperty(entity, i);
			}
		}
		
		TsUICollapsableEnd();
	}
	
	char lbl[100];
	if (EntityHasProperty(entity, ENTITY_PROPERTY_positional))
	{
		sprintf(lbl, "Position: %f, %f", entity->position.x, entity->position.y);
		TsUILabel(lbl);
	}
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_sprite))
	{
		if (TsUICollapsable("Sprite"))
		{
			sprintf(lbl, "Render Layer: %s", GetRenderLayerName(entity->sprite_data.render_layer));
			TsUILabel(lbl);
			
			if (TsUICollapsable("Sprite Type"))
			{
				for (i32 i = 0; i < SPRITE_MAX; i++)
				{
					if (TsUIToggler(GetSpriteName(i), entity->sprite_data.sprite == i))
					{
						entity->sprite_data.sprite = i;
					}
				}
				
				TsUICollapsableEnd();
			}
			
			TsUICollapsableEnd();
		}
	}
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_tree))
	{
		if (TsUICollapsable("Tree Type"))
		{
			for (i32 i = 0; i < TREE_TYPE_MAX; i++)
			{
				if (TsUIToggler(GetTreeTypeName(i), entity->tree_type == i))
				{
					entity->tree_type = i;
					UpdateEntitySprite(entity);
				}
			}
			
			TsUICollapsableEnd();
		}
	}
	
	TsUIDivider();
	
	if (TsUIButton("Delete"))
	{
		DeleteEntity(entity);
	}
}