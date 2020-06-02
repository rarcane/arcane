internal void GroundItemInteractCallback(Entity *entity)
{
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	i32 free_inventory_slot = -1;
	for (i32 i = 0; i < player_data->inventory_size; i++)
	{
		Item *item = &player_data->inventory[i];
		if (item->type == ITEM_TYPE_none)
		{
			free_inventory_slot = i;
			break;
		}
	}
	
	if (free_inventory_slot != -1)
	{
		ItemComponent *item_comp = GetItemComponentFromEntityID(entity->entity_id);
		player_data->inventory[free_inventory_slot] = item_comp->item;
		DeleteEntity(entity);
	}
}

internal Entity *NewGroundItemEntity(v2 position, Item item_data)
{
	Entity *entity = NewEntity("item", GENERALISED_ENTITY_TYPE_undefined);
	
	PositionComponent *pos_comp = AddPositionComponent(entity);
	pos_comp->position = position;
	
	ItemComponent *item_comp = AddItemComponent(entity);
	item_comp->item = item_data;
	
	ItemTypeData *item_type = &global_item_type_data[item_data.type];
	
	SpriteComponent *sprite_comp = AddSpriteComponent(entity);
	sprite_comp->sprite_data.static_sprite = item_type->ground_sprite;
	
	PhysicsBodyComponent *phys_comp = AddPhysicsBodyComponent(entity);
	c2Circle circle = {
		.p = c2V(0.0f, 0.0f),
		.r = 5.0f,
	};
	phys_comp->shape.circle = circle;
	phys_comp->shape_type = C2_SHAPE_TYPE_circle;
	phys_comp->mass_data.mass = 10.0f;
	phys_comp->mass_data.inv_mass = 1.0f / 10.0f;
	phys_comp->material.restitution = 0.1f;
	phys_comp->material.static_friction = 0.1f;
	phys_comp->material.dynamic_friction = 0.1f;
	phys_comp->gravity_multiplier = 1.0f;
	phys_comp->type |= PHYSICS_BODY_TYPE_FLAGS_item;
	phys_comp->collide_against |= PHYSICS_BODY_TYPE_FLAGS_ground;
	
	InteractableComponent *inter_comp = AddInteractableComponent(entity);
	inter_comp->bounds.aabb.min = c2V(-20.0f, -20.0f);
	inter_comp->bounds.aabb.max = c2V(20.0f, 20.0f);
	inter_comp->bounds_type = C2_SHAPE_TYPE_aabb;
	inter_comp->priority = 2.0f;
	inter_comp->interact_callback = GroundItemInteractCallback;
	
	return entity;
}

internal Entity *NewGroundItemEntityAtPlayer(Item item_data)
{
	PositionComponent *pos_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	SpriteComponent *char_sprite_comp = GetSpriteComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	f32 flipped_multiplier = char_sprite_comp->is_flipped ? -1.0f : 1.0f;
	
	Entity *entity = NewGroundItemEntity(V2AddV2(pos_comp->position,
												 v2(10.0f * flipped_multiplier, -20.0f)),
										 item_data);
	
	PhysicsBodyComponent *body_comp = GetPhysicsBodyComponentFromEntityID(entity->entity_id);
	body_comp->velocity.x = 50.0f * flipped_multiplier;
	
	return entity;
}