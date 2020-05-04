internal void PreMoveUpdatePlayer()
{
	R_DEV_ASSERT(core->run_data->character_entity->entity_id, "Character hasn't been created yet.");

	PositionComponent *position_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	PhysicsBodyComponent *body_comp = GetPhysicsBodyComponentFromEntityID(core->run_data->character_entity->entity_id);
	MovementComponent *movement_comp = GetMovementComponentFromEntityID(core->run_data->character_entity->entity_id);
	ArcEntityComponent *arc_entity_comp = GetArcEntityComponentFromEntityID(core->run_data->character_entity->entity_id);
	SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(core->run_data->character_entity->entity_id);
	AnimationComponent *animation_comp = GetAnimationComponentFromEntityID(core->run_data->character_entity->entity_id);

	b8 is_sprinting = 0;
	if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_down[KEY_a])
	{
		if (platform->key_down[KEY_d])
			movement_comp->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				movement_comp->axis_x = -1.5;
				is_sprinting = 1;
			}
			else
			{
				movement_comp->axis_x = -0.5f;
			}
		}
	}
	else if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_down[KEY_d])
	{
		if (platform->key_down[KEY_a])
			movement_comp->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				movement_comp->axis_x = 1.5;
				is_sprinting = 1;
			}
			else
			{
				movement_comp->axis_x = 0.5f;
			}
		}
	}
	else
	{
		movement_comp->axis_x = 0.0f;
	}

	if (fabsf(body_comp->velocity.x) < fabsf(movement_comp->move_speed * movement_comp->axis_x))
		body_comp->force.x = movement_comp->axis_x * (1200 / body_comp->mass_data.inv_mass);

	if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_pressed[KEY_space])
	{
		body_comp->force.y = -10000.0f / body_comp->mass_data.inv_mass;
	}

	// NOTE(randy): Update animation state
	{
		if (movement_comp->axis_x < 0.0f)
		{
			sprite_comp->is_flipped = 1;
		}
		else if (movement_comp->axis_x > 0.0f)
		{
			sprite_comp->is_flipped = 0;
		}

		if (movement_comp->axis_x == 0)
		{
			SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_idle, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
		}
		else
		{
			if (is_sprinting)
			{
				SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_sprinting, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
			else
			{
				SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_walking, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
		}
	}

	// Acceleration calculations?
}

internal void PostMoveUpdatePlayer()
{
	// NOTE(randy): Check for an interaction with the world.
	/* if (platform->key_pressed[KEY_e])
	{
		ColliderComponent *overlapping_colliders[MAX_OVERLAPPING_COLLIDERS];

		PositionComponent *player_pos = core->run_data->character_entity.position_comp;
		SubSpriteComponent *player_sprite = core->run_data->character_entity.sub_sprite_comp;

		i32 overlap_count = GetOverlappingCollidersAtPosition(overlapping_colliders,
															  GetRectangleShape(v2(20.0f, 30.0f), v2(0.0f, 0.0f)),
															  v2(player_pos->position.x + 20 * (player_sprite->is_flipped ? -1.0f : 1.0f), player_pos->position.y),
															  COLLIDER_FLAGS_item);

		Entity *closest_item = 0;
		ItemComponent *closest_item_comp = 0;
		for (int i = 0; i < overlap_count; i++)
		{
			// TODO: find the closest one

			closest_item = overlapping_colliders[0]->parent_entity;
			closest_item_comp = closest_item->components[COMPONENT_item];
			R_DEV_ASSERT(closest_item_comp, "Collider entity does not have an item component attached?");
		}

		if (closest_item)
		{
			i32 free_hotbar_slot = -1;
			StorageComponent *hotbar_storage_comp = core->hotbar->components[COMPONENT_storage];
			if (item_data[closest_item_comp->item_type].flags & ITEM_FLAGS_HOTBARABLE)
			{
				for (int i = 0; i < hotbar_storage_comp->storage_size; i++)
				{
					if (!hotbar_storage_comp->items[i])
					{
						free_hotbar_slot = i;
						break;
					}
				}
			}

			if (free_hotbar_slot != -1)
			{
				StripItemGroundComponents(closest_item_comp);
				AddItemToStorage(closest_item_comp, hotbar_storage_comp, free_hotbar_slot);
			}
			else // If it didn't find a slot in the hotbar, try the backpack.
			{
				if (core->backpack)
				{
					StorageComponent *storage_comp = core->backpack->components[COMPONENT_storage];
					i32 first_free_inv_slot = -1;
					for (int i = 0; i < storage_comp->storage_size; i++)
					{
						if (storage_comp->items[i] && storage_comp->items[i]->item_type == closest_item_comp->item_type)
						{
							if (storage_comp->items[i]->stack_size + closest_item_comp->stack_size <=
								item_data[closest_item_comp->item_type].max_stack_size)
							{
								storage_comp->items[i]->stack_size += closest_item_comp->stack_size;
								DeleteEntity(closest_item);
								closest_item = 0;
								break;
							}
							else
							{
								i32 partial_amount = item_data[closest_item_comp->item_type].max_stack_size - storage_comp->items[i]->stack_size;
								storage_comp->items[i]->stack_size += partial_amount;
								closest_item_comp->stack_size -= partial_amount;
							}
						}
						else if (first_free_inv_slot == -1)
						{
							first_free_inv_slot = i;
						}
					}

					if (closest_item && first_free_inv_slot != -1) // If it still exists and there's a free inv slot.
					{
						StripItemGroundComponents(closest_item_comp);
						AddItemToStorage(closest_item_comp, storage_comp, first_free_inv_slot);
					}
				}
			}
		}
	} */

	/* // NOTE(randy): Check for a hotbar slot update.
	{
		StorageComponent *hotbar_storage = GetStorageComponentFromEntityID(core->hotbar->entity_id);
		for (int i = 0; i < hotbar_storage->storage_size; i++)
		{
			if (platform->key_pressed[KEY_1 + i])
			{
				if (core->active_hotbar_slot != i + 1)
				{
					if (hotbar_storage->items[i])
					{
						if (core->held_item)
						{
							RemoveHeldItem();
						}

						ItemComponent *new_held_item_comp = hotbar_storage->items[i];
						Entity *new_held_item = GetEntityWithID(hotbar_storage->items[i]->parent_entity_id);

						AddPositionComponent(new_held_item);
						SpriteComponent *new_held_item_sprite = AddSpriteComponent(new_held_item);
						new_held_item_sprite->sprite_data.static_sprite = item_type_data[new_held_item_comp->item_type].icon_sprite;
						new_held_item_sprite->sprite_data.render_layer = -0.025f;

						core->held_item = new_held_item;
						core->active_hotbar_slot = i + 1;
					}
					else
					{
						if (core->held_item)
						{
							RemoveHeldItem();
						}
					}
				}
				else
				{
					RemoveHeldItem(); // Should definitely be a held item equipped.
				}
			}
		}
	}

	// NOTE(randy): Update held item to player hand position.
	if (core->held_item)
	{
	} */
}
/* 
internal void RemoveHeldItem()
{
	R_DEV_ASSERT(core->held_item, "No held iem active.");

	RemovePositionComponent(core->held_item);
	RemoveSpriteComponent(core->held_item);
	core->held_item = 0;
	core->active_hotbar_slot = 0;
} */