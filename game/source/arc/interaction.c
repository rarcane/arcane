internal void InteractableUpdate()
{
	core->run_data->current_e_interactable = 0;
	core->run_data->current_left_click_interactable = 0;
	
	if (!CanPlayerInteract())
	{
		return;
	}
	
	Entity *character = core->run_data->character_entity;
	PhysicsBodyData *player_physics = &core->run_data->character_entity->physics;
	
	// NOTE(randy): E interaction
	{
		// TODO(randy): Change shape with player dir
		c2Shape e_interaction_shape = {
			.aabb = { .min = c2V(-20.0f, -20.0f), .max = c2V(20.0f, 20.0f) },
		};
		AddPositionOffsetToShape(&e_interaction_shape, C2_SHAPE_TYPE_aabb, character->position);
		
		Entity *overlapping_entities[MAX_OVERLAPPING_COLLIDERS] = {0};
		i32 overlapping_count = GetOverlappingBodiesWithShape(overlapping_entities,
															  e_interaction_shape,
															  C2_SHAPE_TYPE_aabb);
		for (i32 i = 0; i < overlapping_count; i++)
		{
			Entity *entity = overlapping_entities[i];
			
			if (EntityHasProperty(entity, ENTITY_PROPERTY_interactable_e))
			{
				if (core->run_data->current_e_interactable &&
					entity->priority > core->run_data->current_e_interactable->priority ||
					!core->run_data->current_e_interactable)
				{
					core->run_data->current_e_interactable = entity;
				}
			}
		}
		
		if (core->run_data->current_e_interactable && platform->key_pressed[KEY_e])
		{\
			platform->key_pressed[KEY_e] = 0;
			
			if (core->run_data->current_e_interactable->interact_callback)
			{
				core->run_data->current_e_interactable->interact_callback(core->run_data->current_e_interactable);
			}
		}
	}
	
	// NOTE(randy): Primary interaction
	if (platform->left_mouse_pressed &&
		GetItemInHand() &&
		global_item_type_data[GetItemInHand()->type].flags & ITEM_FLAGS_primary_usable)
	{
		ItemTypeData *item_type_data = &global_item_type_data[GetItemInHand()->type];
		ItemCategory held_item_cat = item_type_data->category;
		
		switch (held_item_cat)
		{
			case ITEM_CATEGORY_lumber_axe :
			{
				Entity *entity = HitQueryEntityFromHeldItem();
				if (entity)
				{
					entity->durability -= 30.0f;
					if (entity->durability <= 0.0f)
					{
						Item item = {
							.type = ITEM_TYPE_twig,
							.stack_size = 2
						};
						NewGroundItemEntity(V2AddV2(entity->position, v2(-5.0f, -30.0f)), item);
						NewGroundItemEntity(V2AddV2(entity->position, v2(5.0f, -60.0f)), item);
						DeleteEntity(entity);
						
					}
				}
			} break;
			
			case ITEM_CATEGORY_sword :
			{
				// disable hotbar switching
				
				if (!core->run_data->character_combat_state)
				{
					core->run_data->character_combat_state = CHARACTER_COMBAT_STATE_light_charge;
					core->run_data->cooldown_timer = 0.3f; // charge animation time
					
					character->sprite_data.tint = v4(1.0f, 0.0f, 0.0f, 1.0f);
				}
			} break;
		}
	}
	
	// NOTE(randy): Right click interaction
	if (platform->right_mouse_pressed &&
		GetItemInHand() &&
		global_item_type_data[GetItemInHand()->type].flags & ITEM_FLAGS_secondary_usable)
	{
		
	}
	
	// NOTE(randy): Attack charging
	// TODO(randy): Continued charge with holding the button
	if ((core->run_data->character_combat_state == CHARACTER_COMBAT_STATE_light_charge ||
		 core->run_data->character_combat_state == CHARACTER_COMBAT_STATE_heavy_charge) &&
		core->run_data->cooldown_timer > 0.0f)
	{
		core->run_data->cooldown_timer -= core->world_delta_t;
		if (core->run_data->cooldown_timer <= 0.0f)
		{
			// NOTE(randy): All charged up baybeee
			switch (core->run_data->character_combat_state)
			{
				case CHARACTER_COMBAT_STATE_light_charge :
				{
					core->run_data->character_combat_state = CHARACTER_COMBAT_STATE_light_attack;
					core->run_data->cooldown_timer = 0.5f;
					
					character->sprite_data.tint = v4(0.0f, 1.0f, 0.0f, 1.0f);
				} break;
				
				case CHARACTER_COMBAT_STATE_heavy_charge :
				{
					
				} break;
				
				default :
				Assert(0);
				break;
			}
		}
	}
	
	// NOTE(randy): Attacking
	if ((core->run_data->character_combat_state == CHARACTER_COMBAT_STATE_light_attack ||
		 core->run_data->character_combat_state == CHARACTER_COMBAT_STATE_heavy_attack) &&
		core->run_data->cooldown_timer > 0.0f)
	{
		local_persist f32 cooldown_half = -1.0f;
		local_persist b8 has_hit_queried = 0;
		if (cooldown_half == -1.0f)
		{
			cooldown_half = core->run_data->cooldown_timer / 2.0f;
		}
		if (core->run_data->cooldown_timer <= cooldown_half &&
			!has_hit_queried)
		{
			has_hit_queried = 1;
			Entity *entity = HitQueryEntityFromHeldItem();
			if (entity)
			{
				// doink hp from it
				Log("Entity doinked");
				entity->sprite_data.tint = v4(RandomF32(0.0f, 1.0f), RandomF32(0.0f, 1.0f), RandomF32(0.0f, 1.0f), 1.0f);
			}
		}
		
		core->run_data->cooldown_timer -= core->world_delta_t;
		if (core->run_data->cooldown_timer <= 0.0f)
		{
			// attack is finished
			Log("attack finished");
			
			character->sprite_data.tint = v4u(1.0f);
			
			core->run_data->cooldown_timer = 0.0f;
			core->run_data->character_combat_state = CHARACTER_COMBAT_STATE_none;
			cooldown_half = -1.0f;
			has_hit_queried = 0;
		}
	}
	
	
}

/*
internal void TreeInteractCallback(Entity *entity)
{
	ItemTypeData *item_type_data = &global_item_type_data[GetItemInHand()->type];
	if (item_type_data->flags & ITEM_FLAGS_lumber_axe)
	{
		entity->durability -= 10.0f;
		if (entity->durability <= 0.0f)
		{
			Item twigs = { ITEM_TYPE_twig, 6 };
			NewGroundItemEntity(V2AddV2(entity->position, v2(0.0f, -30.0f)), twigs);
			
			DeleteEntity(entity);
			core->run_data->current_left_click_interactable = 0;
		}
	}
}
 */

internal Entity *HitQueryEntityFromHeldItem()
{
	Entity *character = GetCharacterEntity();
	ItemTypeData *item_type_data = &global_item_type_data[GetItemInHand()->type];
	ItemCategory held_item_cat = item_type_data->category;
	
	// NOTE(randy): Determine shape of the interaction
	c2Shape interaction_shape = {0};
	switch (held_item_cat)
	{
		case ITEM_CATEGORY_lumber_axe :
		{
			interaction_shape.aabb.min = c2V(-20.0f, -20.0f);
			interaction_shape.aabb.max = c2V(20.0f, 20.0f);
			AddPositionOffsetToShape(&interaction_shape, C2_SHAPE_TYPE_aabb, character->position);
		} break;
		
		case ITEM_CATEGORY_sword :
		{
			interaction_shape.aabb.min = c2V(-20.0f, -20.0f);
			interaction_shape.aabb.max = c2V(30.0f, 20.0f);
			AddPositionOffsetToShape(&interaction_shape, C2_SHAPE_TYPE_aabb, character->position);
		} break;
		
		default :
		Assert(0);
		break;
	}
	
	// NOTE(randy): Find highest priority entity
	Entity *overlapping_entities[MAX_OVERLAPPING_COLLIDERS] = {0};
	i32 overlapping_count = GetOverlappingBodiesWithShape(overlapping_entities,
														  interaction_shape,
														  C2_SHAPE_TYPE_aabb);
	Entity *priority_entity = 0;
	f32 priority_mag = 0.0f;
	for (i32 i = 0; i < overlapping_count; i++)
	{
		Entity *entity = overlapping_entities[i];
		
		if (!EntityHasProperty(entity, ENTITY_PROPERTY_interactable_left_click))
		{
			continue;
		}
		
		switch (held_item_cat)
		{
			case ITEM_CATEGORY_sword :
			{
				if (!EntityHasProperty(entity, ENTITY_PROPERTY_enemy))
				{
					continue;
				}
			} break;
			case ITEM_CATEGORY_lumber_axe :
			{
				if (!EntityHasProperty(entity, ENTITY_PROPERTY_lumber_axable))
				{
					continue;
				}
			} break;
			
			default :
			{
				continue;
			} break;
		}
		
		if (priority_entity)
		{
			v2 diff = V2SubtractV2(entity->position, character->position);
			f32 entity_mag = PythagSolve(diff.x, diff.y);
			if (entity_mag <  priority_mag)
			{
				priority_entity = entity;
				priority_mag = entity_mag;
			}
		}
		else
		{
			priority_entity = entity;
		}
	}
	
	return priority_entity;
}

internal i32 HitQueryEntitiesFromHeldItem(Entity **entities)
{
	// TODO(randy):
	Assert(0);
}

internal b8 CanPlayerInteract()
{
	return !(core->run_data->character_state & CHARACTER_STATE_is_crafting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_backpack_open) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_blueprinting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_in_elemental_skill_tree) &&
		!core->run_data->is_paused;
}