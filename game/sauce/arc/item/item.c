internal void InitialiseItemData(){
#define Item(item_name,                                                   \
			 item_print_name,                                             \
			 item_icon_sprite,                                            \
			 item_ground_sprite,                                          \
			 item_max_stack_size,                                         \
			 item_flags)                                                  \
	{                                                                     \
		strcpy(item_data[ITEM_##item_name].print_name, item_print_name);  \
		item_data[ITEM_##item_name].icon_sprite = item_icon_sprite;       \
		item_data[ITEM_##item_name].ground_sprite = item_ground_sprite;   \
		item_data[ITEM_##item_name].max_stack_size = item_max_stack_size; \
		item_data[ITEM_##item_name].flags = item_flags;                   \
	}
#include "items.inc"
#undef Item
}

// NOTE(tjr): Adds item into a storage component at the desired slot. Returns false if there is already an existing item there.
internal void AddItemToStorage(ItemComponent *item_comp, StorageComponent *storage_comp, i32 storage_slot)
{
	R_DEV_ASSERT(item_comp && storage_comp, "Invalid components.");
	R_DEV_ASSERT(item_comp->entity_id > 0 && storage_comp->entity_id > 0, "Invalid components.");
	R_DEV_ASSERT(storage_slot < storage_comp->storage_size, "Out of range storage slot.");

	R_DEV_ASSERT(!storage_comp->items[storage_slot], "There's already something in this slot.");
	storage_comp->items[storage_slot] = item_comp;
}

internal void ConvertToGroundItem(ItemComponent *item_comp)
{
	Entity *item = &core->entity_set->entities[item_comp->entity_id];

	PositionComponent *player_pos = core->player->components[COMPONENT_position];
	SubSpriteComponent *player_sub_sprite = core->player->components[COMPONENT_sub_sprite];

	StaticSprite *ground_sprite = GetStaticSprite(item_data[item_comp->item_type].ground_sprite);

	AttachPosition(item, v2(player_pos->position.x + 10 * (player_sub_sprite->is_flipped ? -1 : 1), player_pos->position.y - 20));
	AttachSprite(item, item_data[item_comp->item_type].ground_sprite, 1.0f);
	AttachVelocity(item, v2(player_sub_sprite->is_flipped ? -50.0f : 50.0f, 0.0f), v2(0, 0), COLLIDER_FLAGS_ground);
	AttachPhysics(item, 1.0f, (item_data[item_comp->item_type].flags & ITEM_FLAG_bouncy) ? 0.5f : 0.0f);
	AttachCollider(item, GetRectangleShape(ground_sprite->source.zw, v2(0.0f, 0.0f)), COLLIDER_FLAGS_item);
}

internal void StripItemGroundComponents(ItemComponent *item_comp)
{
	Entity *item = &core->entity_set->entities[item_comp->entity_id];
	RemoveComponent(item, COMPONENT_position);
	RemoveComponent(item, COMPONENT_sprite);
	RemoveComponent(item, COMPONENT_velocity);
	RemoveComponent(item, COMPONENT_physics);
	RemoveComponent(item, COMPONENT_collider);
}