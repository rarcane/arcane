internal void GroundItemInteractCallback(Entity *entity)
{
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = &core->run_data->character_data;
	
	i32 free_inventory_slot = -1;
	for (i32 i = 0; i < character_data->inventory_size; i++)
	{
		Item *item = &character_data->inventory[i];
		if (item->type == ITEM_TYPE_none)
		{
			free_inventory_slot = i;
			break;
		}
	}
	
	if (free_inventory_slot != -1)
	{
		character_data->inventory[free_inventory_slot] = entity->item;
		DeleteEntity(entity);
	}
}

internal Entity *NewGroundItemEntity(v2 position, Item item_data)
{
	Entity *entity = NewEntity();
	EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
	EntitySetProperty(entity, ENTITY_PROPERTY_interactable_e);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	
	entity->position = position;
	entity->item = item_data;
	
	ItemTypeData *item_type = &global_item_type_data[item_data.type];
	
	entity->sprite_data.static_sprite = item_type->ground_sprite;
	
	c2Circle circle = {
		.p = c2V(0.0f, -10.0f),
		.r = 10.0f,
	};
	entity->physics.shape.circle = circle;
	entity->physics.shape_type = C2_SHAPE_TYPE_circle;
	entity->physics.mass_data.mass = 10.0f;
	entity->physics.mass_data.inv_mass = 1.0f / 10.0f;
	entity->physics.material.restitution = 0.1f;
	entity->physics.material.static_friction = 0.1f;
	entity->physics.material.dynamic_friction = 0.1f;
	entity->physics.gravity_multiplier = 1.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_item;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_ground | PHYSICS_BODY_TYPE_FLAGS_station;
	
	entity->priority = 2.0f;
	entity->interact_callback = GroundItemInteractCallback;
	
	return entity;
}

internal Entity *NewGroundItemEntityAtPlayer(Item item_data)
{
	f32 flipped_multiplier = GetCharacterEntity()->is_flipped ? -1.0f : 1.0f;
	
	Entity *entity = NewGroundItemEntity(V2AddV2(GetCharacterEntity()->position,
												 v2(10.0f * flipped_multiplier, -20.0f)),
										 item_data);
	EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	
	entity->physics.velocity.x = 50.0f * flipped_multiplier;
	
	return entity;
}

internal i32 RemoveItemFromContainer(Item item, Item *container, i32 container_size)
{
	Assert(item.type);
	
	if (!item.stack_size)
		return 0;
	
	i32 removed_amount = 0;
	for (i32 i = 0; i < container_size; i++)
	{
		Item *container_item = &container[i];
		if (container_item->type == item.type)
		{
			if (container_item->stack_size == item.stack_size)
			{
				container_item->type = 0;
				container_item->stack_size = 0;
				return item.stack_size;
			}
			else if (container_item->stack_size > item.stack_size)
			{
				container_item->stack_size -= item.stack_size;
				return item.stack_size;
			}
			else
			{
				item.stack_size -= container_item->stack_size;
				removed_amount += container_item->stack_size;
				container_item->type = 0;
				container_item->stack_size = 0;
			}
		}
	}
	
	return removed_amount;
}