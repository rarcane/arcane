// NOTE(tjr): Adds item into a storage component at the desired slot. Returns false if there is already an existing item there.
internal void AddItemToStorage(ItemComponent *item_comp, StorageComponent *storage_comp, i32 storage_slot)
{
	R_DEV_ASSERT(item_comp && storage_comp, "Invalid components.");
	R_DEV_ASSERT(item_comp->parent_entity && storage_comp->parent_entity, "Invalid components.");
	R_DEV_ASSERT(storage_slot < storage_comp->storage_size, "Out of range storage slot.");

	R_DEV_ASSERT(!storage_comp->items[storage_slot], "There's already something in this slot.");
	storage_comp->items[storage_slot] = item_comp;
}

internal void ConvertToGroundItem(ItemComponent *item_comp)
{
	Entity *item = item_comp->parent_entity;

	PositionComponent *player_pos = core->world_data->character_entity.position_comp;
	SpriteComponent *player_sprite = core->world_data->character_entity.sprite_comp;

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
	item_collider->flags = COLLIDER_FLAGS_item;*/
	R_TODO;
}

internal void StripItemGroundComponents(ItemComponent *item_comp)
{
	Entity *item = item_comp->parent_entity;
	RemovePositionComponent(item);
	RemoveSpriteComponent(item);
	/* RemoveVelocityComponent(item);
	RemovePhysicsComponent(item);
	RemoveColliderComponent(item);*/
}