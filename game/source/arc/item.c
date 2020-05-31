internal Entity *NewGroundItemEntity(v2 position, Item item_data)
{
	Entity *entity = NewEntity("item", GENERALISED_ENTITY_TYPE_undefined);
	
	PositionComponent *pos_comp = AddPositionComponent(entity);
	pos_comp->position = position;
	
	ItemComponent *item_comp = AddItemComponent(entity);
	item_comp->item = item_data;
	
	ItemTypeData *item_type = &item_type_data[item_data.type];
	
	SpriteComponent *sprite_comp = AddSpriteComponent(entity);
	sprite_comp->sprite_data.static_sprite = item_type->ground_sprite;
	
	PhysicsBodyComponent *phys_comp = AddPhysicsBodyComponent(entity);
	c2AABB aabb = {
		.min = c2V(-10.0f, 10.0f),
		.max = c2V(10.0f, 10.0f),
	};
	phys_comp->shape.aabb = aabb;
	phys_comp->shape_type = C2_SHAPE_TYPE_aabb;
	phys_comp->mass_data.mass = 30.0f;
	phys_comp->mass_data.inv_mass = 1.0f / 30.0f;
	phys_comp->material.restitution = 1.0f;
	phys_comp->material.static_friction = 0.5f;
	phys_comp->material.dynamic_friction = 0.5f;
	phys_comp->gravity_multiplier = 1.0f;
	
	return entity;
}

internal Entity *NewGroundItemEntityAtPlayer(Item item_data)
{
	PositionComponent *pos_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	return NewGroundItemEntity(pos_comp->position, item_data);
}





/*
internal void AddItemToStorage(ItemComponent *item_comp, StorageComponent *storage_comp, i32 storage_slot)
{
	Assert(item_comp->component_id && storage_comp->component_id);
	Assert(storage_slot < storage_comp->storage_size);
	
	Assert(!storage_comp->items[storage_slot]);
	storage_comp->items[storage_slot] = item_comp;
}
 */

/*
internal void ConvertToGroundItem(ItemComponent *item_comp)
{
		Entity *item = GetEntityWithID(item_comp->parent_entity_id);
		
		PositionComponent *player_pos = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
		SpriteComponent *player_sprite = GetSpriteComponentFromEntityID(core->run_data->character_entity->entity_id);
		
		StaticSpriteData *ground_sprite = &static_sprite_data[item_type_data[item_comp->item_type].ground_sprite];
		
		PositionComponent *item_pos = AddPositionComponent(item);
		item_pos->position = v2(player_pos->position.x + 10 * (player_sprite->is_flipped ? -1 : 1), player_pos->position.y - 20);
		
		SpriteComponent *item_sprite = AddSpriteComponent(item);
		item_sprite->sprite_data.static_sprite = item_type_data[item_comp->item_type].ground_sprite;
		item_sprite->sprite_data.render_layer = 1.0f;
	
	/* VelocityComponent *item_velocity = AddVelocityComponent(item);
	item_velocity->velocity = v2(player_sub_sprite->is_flipped ? -50.0f : 50.0f, 0.0f);
	item_velocity->collide_against = COLLIDER_FLAGS_ground;

	PhysicsComponent *item_physics = AddPhysicsComponent(item);
	item_physics->restitution = (item_data[item_comp->item_type].flags & ITEM_FLAG_bouncy) ? 0.5f : 0.0f;

	ColliderComponent *item_collider = AddColliderComponent(item);
	//item_collider->shape = GetRectangleShape(ground_sprite->source.zw,  v2(0.0f, 0.0f));
	item_collider->flags = COLLIDER_FLAGS_item;
	Assert(0);
}*/

/*
internal void StripItemGroundComponents(ItemComponent *item_comp)
{
	Entity *item = GetEntityWithID(item_comp->component_id);
	RemovePositionComponent(item);
	RemoveSpriteComponent(item);
	/* RemoveVelocityComponent(item);
	RemovePhysicsComponent(item);
	RemoveColliderComponent(item);
}
*/