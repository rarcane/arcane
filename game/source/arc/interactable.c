internal void InteractableUpdate()
{
	if (!CanPlayerInteract())
	{
		core->run_data->current_interactable = 0;
		return;
	}
	
	Entity *character = core->run_data->character_entity;
	PhysicsBodyData *player_physics = &core->run_data->character_entity->physics;
	
	Entity *highest_priority_entity = 0;
	
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_interactable);)
	{
		InteractableData *inter = &entity->interactable;
		
		c2Shape inter_shape = inter->bounds;
		AddPositionOffsetToShape(&inter_shape,
								 inter->bounds_type,
								 entity->position);
		
		c2Shape character_shape = character->interactable.bounds;
		AddPositionOffsetToShape(&character_shape,
								 character->interactable.bounds_type,
								 character->position);
		
		c2Manifold manifold = {0};
		GenerateCollisionManifold(inter_shape, inter->bounds_type,
								  character_shape, character->interactable.bounds_type,
								  &manifold);
		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			// NOTE(randy): Enter testing
			if (!inter->is_overlapping_player)
			{
				inter->is_overlapping_player = 1;
				if (inter->enter_interactable_callback)
				{
					inter->enter_interactable_callback(character);
				}
			}
			
			// NOTE(randy): Player is within interaction bounds.
			if (!highest_priority_entity ||
				inter->priority > highest_priority_entity->interactable.priority)
			{
				highest_priority_entity = entity;
			}
		}
		else
		{
			// NOTE(randy): Exit testing
			if (inter->is_overlapping_player)
			{
				inter->is_overlapping_player = 0;
				if (inter->exit_interactable_callback)
				{
					inter->exit_interactable_callback(character);
				}
			}
		}
	}
	
	// NOTE(randy): Interaction dispatch
	core->run_data->current_interactable = highest_priority_entity;
	if (highest_priority_entity && platform->key_pressed[KEY_e])
	{
		platform->key_pressed[KEY_e] = 0;
		
		if (highest_priority_entity->interactable.interact_callback)
		{
			highest_priority_entity->interactable.interact_callback(highest_priority_entity);
		}
	}
}

internal void OnCraftingTableInteract(Entity *entity)
{
	Assert(!core->run_data->engaged_station_entity &&
		   entity->station_type == STATION_TYPE_crafting);
	
	core->run_data->engaged_station_entity = entity;
	core->run_data->character_state |= CHARACTER_STATE_is_crafting;;
}

internal b8 IsRecipeCraftable(CraftingRecipeType recipe, Item *item_pool, i32 item_count)
{
	Assert(recipe);
	CraftingRecipeTypeData *recipe_data = &global_crafting_recipe_type_data[recipe];
	for (i32 i = 0; i < MAX_ITEMS_IN_CRAFTING_RECIPE; i++)
	{
		if (recipe_data->input[i].type)
		{
			b8 item_found = 0;
			i32 remaining_count = recipe_data->input[i].stack_size;
			for (i32 j = 0; j < item_count; j++)
			{
				Item *item = &item_pool[j];
				if (!item->type)
					continue;
				
				if (item->type == recipe_data->input[i].type &&
					item->stack_size >= remaining_count)
				{
					item_found = 1;
					break;
				}
				else
				{
					remaining_count -= item->stack_size;
				}
			}
			
			if (item_found)
				continue;
			else
				return 0;
		}
		else
		{
			return 1;
		}
	}
	
	return 1;
}

internal void StationUpdate()
{
	Entity *character = core->run_data->character_entity;
	
	if (core->run_data->engaged_station_entity)
	{
		Entity *engaged_station = core->run_data->engaged_station_entity;
		Assert(!CanPlayerInteract());
		
		switch (core->run_data->engaged_station_entity->station_type)
		{
			case STATION_TYPE_crafting :
			{
				CraftingStation *crafting_station_data = &core->run_data->engaged_station_entity->station_data.crafting;
				
				CraftingRecipeType craftable_recipes[CRAFTING_RECIPE_TYPE_MAX];
				i32 craftable_recipe_count = 0;
				
				i32 existing_recipe_index = -1;
				for (i32 i = 1;
					 i < CRAFTING_RECIPE_TYPE_MAX;
					 i++)
				{
					if (IsRecipeCraftable(i, character->inventory, character->inventory_size))
					{
						craftable_recipes[craftable_recipe_count] = i;
						if (crafting_station_data->current_recipe == i)
						{
							Assert(existing_recipe_index == -1); // NOTE(randy): Some sort of double up?
							existing_recipe_index = craftable_recipe_count;
						}
						craftable_recipe_count++;
					}
				}
				
				if (craftable_recipe_count == 0)
				{
					break;
				}
				
				if (crafting_station_data->current_recipe == CRAFTING_RECIPE_TYPE_none ||
					existing_recipe_index == -1)
				{
					// NOTE(randy): No crafting recipe has been selected prior, so just select the highest craftable one.
					crafting_station_data->current_recipe = craftable_recipes[craftable_recipe_count - 1];
					existing_recipe_index = craftable_recipe_count - 1;
				}
				
				Assert(existing_recipe_index >= 0);
				
				// NOTE(randy): Left or right recipe checks
				if (existing_recipe_index - 1 >= 0)
				{
					if (platform->key_pressed[KEY_a] )
					{
						crafting_station_data->current_recipe = craftable_recipes[existing_recipe_index - 1];
					}
					
					v4 tint = v4u(1.0f);
					if (platform->key_down[KEY_a])
					{
						tint = v4u(0.75f);
					}
					
					// NOTE(randy): Render left arrow
					StaticSpriteData *arrow_sprite = &global_static_sprite_data[STATIC_SPRITE_side_arrow];
					v2 render_pos = v2view(v2(engaged_station->position.x - 20.0f,
											  engaged_station->position.y - 40.0f));
					v2 render_size = v2zoom(v2(arrow_sprite->source.width,
											   arrow_sprite->source.height));
					
					ArcPushTexture(arrow_sprite->texture_atlas,
								   0,
								   arrow_sprite->source,
								   v4(render_pos.x,
									  render_pos.y,
									  render_size.x,
									  render_size.y),
								   tint,
								   LAYER_HUD);
				}
				if (existing_recipe_index + 1 < craftable_recipe_count)
				{
					if (platform->key_pressed[KEY_d])
					{
						crafting_station_data->current_recipe = craftable_recipes[existing_recipe_index + 1];
					}
					
					v4 tint = v4u(1.0f);
					if (platform->key_down[KEY_d])
					{
						tint = v4u(0.75f);
					}
					
					// NOTE(randy): Render left arrow
					StaticSpriteData *arrow_sprite = &global_static_sprite_data[STATIC_SPRITE_side_arrow];
					v2 render_pos = v2view(v2(core->run_data->engaged_station_entity->position.x + 20.0f,
											  core->run_data->engaged_station_entity->position.y - 40.0f));
					v2 render_size = v2zoom(v2(arrow_sprite->source.width,
											   arrow_sprite->source.height));
					ArcPushTexture(arrow_sprite->texture_atlas,
								   0,
								   arrow_sprite->source,
								   v4(render_pos.x,
									  render_pos.y,
									  -render_size.x,
									  render_size.y),
								   tint,
								   LAYER_HUD);
				}
				
				// NOTE(randy): Render the craftable sprite.
				Assert(crafting_station_data->current_recipe);
				CraftingRecipeTypeData *recipe = &global_crafting_recipe_type_data[crafting_station_data->current_recipe];
				
				StaticSpriteData *recipe_output_sprite = &global_static_sprite_data[global_item_type_data[recipe->output.type].icon_sprite];
				
				v2 render_pos = v2view(v2(engaged_station->position.x - 7.0f,
										  engaged_station->position.y - 43.0f));
				v2 render_size = v2zoom(v2(recipe_output_sprite->source.width,
										   recipe_output_sprite->source.height));
				
				ArcPushTexture(recipe_output_sprite->texture_atlas,
							   0,
							   recipe_output_sprite->source,
							   v4(render_pos.x,
								  render_pos.y,
								  render_size.x,
								  render_size.y),
							   v4(0.75f, 0.75f, 1.0f, 0.8f),
							   LAYER_HUD);
				
				// NOTE(randy): Craft the recipe
				if (platform->key_pressed[KEY_e])
				{
					for (i32 i = 0; i < MAX_ITEMS_IN_CRAFTING_RECIPE; i++)
					{
						Item *recipe_item = &recipe->input[i];
						if (!recipe_item->type)
							break;
						
						RemoveItemFromContainer(*recipe_item,
												character->inventory,
												character->inventory_size);
					}
					
					core->run_data->engaged_station_entity = 0;
					core->run_data->character_state &= ~CHARACTER_STATE_is_crafting;
					
					Entity *entity = NewGroundItemEntity(v2(engaged_station->position.x,
															engaged_station->position.y - 43.0f),
														 recipe->output);
				}
			} break;
			
			default :
			Assert(0);
			break;
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			core->run_data->character_state &= ~CHARACTER_STATE_is_crafting;
			core->run_data->engaged_station_entity = 0;
			
			platform->key_pressed[KEY_esc] = 0;
			platform->key_down[KEY_esc] = 0;
		}
	}
}

internal void OnCraftingStumpBuild(Entity *entity)
{
	EntityUnsetProperty(entity, ENTITY_PROPERTY_blueprint);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	entity->sprite_data.tint = v4u(1.0f);
	entity->interactable.interact_callback = OnCraftingTableInteract;
	entity->interactable.enter_interactable_callback = 0;
	entity->interactable.exit_interactable_callback = 0;
	entity->station_type = STATION_TYPE_crafting;
	c2AABB aabb = {
		.min = c2V(-15.0f, -25.0f),
		.max = c2V(15.0f, 0.0f),
	};
	entity->physics.shape.aabb = aabb;
	entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
	entity->physics.material.restitution = 0.1f;
	entity->physics.material.static_friction = 0.1f;
	entity->physics.material.dynamic_friction = 0.1f;
	entity->physics.gravity_multiplier = 0.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_station;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_item;
}

internal void OnBlueprintInteract(Entity *entity)
{
	Entity *character = GetCharacterEntity();
	
	b8 blueprint_unfinished = 0;
	for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
	{
		if (!entity->remaining_items_in_blueprint[i].type)
			continue;
		
		i32 removed_amount = RemoveItemFromContainer(entity->remaining_items_in_blueprint[i],
													 character->inventory,
													 character->inventory_size);
		entity->remaining_items_in_blueprint[i].stack_size -= removed_amount;
		
		if (entity->remaining_items_in_blueprint[i].stack_size)
			blueprint_unfinished = 1;
	}
	
	if (!blueprint_unfinished)
	{
		StructureTypeData *structure_data = &global_structure_type_data[entity->structure_type];
		if (structure_data->on_structure_build)
			structure_data->on_structure_build(entity);
	}
}

internal void OnBlueprintEnter(Entity *entity)
{
	Log("blueprint entered");
}

internal void OnBlueprintExit(Entity *entity)
{
	Log("blueprint exited");
}

internal void BlueprintUpdate()
{
	Entity *character = core->run_data->character_entity;
	
	if (character->hotbar[character->active_hotbar_slot].type == ITEM_TYPE_crafting_tool &&
		platform->left_mouse_pressed &&
		CanPlayerInteract())
	{
		core->run_data->character_state |= CHARACTER_STATE_is_blueprinting;
		platform->left_mouse_pressed = 0;
	}
	
	// NOTE(randy): Blueprint UI & placement
	if (!!(core->run_data->character_state & CHARACTER_STATE_is_blueprinting))
	{
		local_persist i32 selected_category = (STRUCTURE_CATEGORY_MAX - 1) / 2 + 1;
		
		local_persist StructureTypeData *selected_structure = 0;
		local_persist b8 is_in_category = 0;
		b8 in_new_cat = 0;
		
		if (platform->right_mouse_pressed)
		{
			core->run_data->character_state &= ~CHARACTER_STATE_is_blueprinting;
			return;
		}
		
		if (platform->key_pressed[KEY_w] && !is_in_category)
		{
			is_in_category = 1;
			in_new_cat = 1;
		}
		
		if (platform->key_pressed[KEY_s] && is_in_category)
		{
			is_in_category = 0;
		}
		
		if (!is_in_category)
		{
			i32 category_count = STRUCTURE_CATEGORY_MAX - 1;
			f32 start_pos = category_count * 20.0f / -2.0f;
			
			if (platform->key_pressed[KEY_a])
			{
				selected_category = selected_category > 1 ?
					selected_category - 1 : STRUCTURE_CATEGORY_MAX - 1;
			}
			
			if (platform->key_pressed[KEY_d])
			{
				selected_category = selected_category < STRUCTURE_CATEGORY_MAX - 1 ?
					selected_category + 1 : 1;
			}
			
			for (i32 i = 1; i < STRUCTURE_CATEGORY_MAX;  i++)
			{
				v2 render_pos = v2view(v2(character->position.x + start_pos + (i - 1) * 20.0f, character->position.y - 60.0f));
				v2 render_size = v2zoom(v2(20.0f, 20.0f));
				
				StaticSprite texture = STATIC_SPRITE_INVALID;
				switch(i)
				{
					case STRUCTURE_CATEGORY_shia:
					{
						texture = STATIC_SPRITE_shia;
					} break;
					
					case STRUCTURE_CATEGORY_crafting:
					{
						texture = STATIC_SPRITE_crafting_tool;
					} break;
					
					case STRUCTURE_CATEGORY_base:
					{
						texture = STATIC_SPRITE_flint_sword_icon;
					} break;
				}
				
				StaticSpriteData *category_sprite = &global_static_sprite_data[texture];
				ArcPushTexture(category_sprite->texture_atlas,
							   0,
							   category_sprite->source,
							   v4(render_pos.x, render_pos.y,
								  render_size.x, render_size.y),
							   selected_category == i ? v4(1.0f, 0.0f, 0.0f, 1.0f) : v4u(1.0f),
							   LAYER_HUD);
			}
		}
		else
		{
#define MAX_STRUCTURES_IN_CATEGORY 40
			typedef struct structure_pair
			{
				StructureType type;
				StructureTypeData *data;
			} structure_pair;
			
			i32 structure_count = 0;
			structure_pair structures[MAX_STRUCTURES_IN_CATEGORY];
			
			for (i32 i = 1; i < STRUCTURE_TYPE_MAX; i++)
			{
				StructureTypeData *structure = &global_structure_type_data[i];
				if (structure->category == selected_category)
				{
					structures[structure_count].data = structure;
					structures[structure_count++].type = i;
				}
			}
			
			if (in_new_cat && structure_count)
			{
				selected_structure = structures[structure_count / 2].data;
			}
			
			i32 selected_structure_index = 0;
			for (i32 i = 0; i < structure_count; i++)
			{
				if (structures[i].data == selected_structure)
				{
					selected_structure = structures[i].data;
					selected_structure_index = i;
					break;
				}
			}
			
			if (platform->key_pressed[KEY_a])
			{
				i32 new_index = selected_structure_index > 0 ?
					selected_structure_index - 1 : structure_count - 1;
				selected_structure = structures[new_index].data;
			}
			
			if (platform->key_pressed[KEY_d])
			{
				i32 new_index = selected_structure_index < structure_count - 1 ?
					selected_structure_index + 1 : 0;
				selected_structure = structures[new_index].data;
			}
			
			f32 start_pos = structure_count * 20.0f / -2.0f;
			
			for (i32 i = 0; i < structure_count; i++)
			{
				v2 render_pos = v2view(v2(character->position.x + start_pos + i * 20.0f, character->position.y - 60.0f));
				v2 render_size = v2zoom(v2(20.0f, 20.0f));
				
				StaticSpriteData *sprite = &global_static_sprite_data[structures[i].data->icon_sprite];
				ArcPushTexture(sprite->texture_atlas,
							   0,
							   sprite->source,
							   v4(render_pos.x, render_pos.y,
								  render_size.x, render_size.y),
							   selected_structure == structures[i].data ? v4(1.0f, 0.0f, 0.0f, 1.0f) : v4u(1.0f),
							   LAYER_HUD);
			}
			
			// NOTE(randy): Render blueprint at mouse location
			StaticSpriteData *sprite = &global_static_sprite_data[selected_structure->world_sprite];
			v2 render_size = v2zoom(v2(sprite->source.z, sprite->source.w));
			v2 render_pos = v2view(V2AddV2(GetMousePositionInWorldSpace(),
										   v2(sprite->source.z / -2.0f, -sprite->source.w)));
			ArcPushTexture(sprite->texture_atlas,
						   0,
						   sprite->source,
						   v4(render_pos.x, render_pos.y,
							  render_size.x, render_size.y),
						   v4(0.5f, 0.5f, 1.0f, 0.4f),
						   LAYER_HUD);
			
			if (platform->left_mouse_pressed)
			{
				Entity *new_structure = NewEntity();
				EntitySetProperty(new_structure, ENTITY_PROPERTY_sprite);
				EntitySetProperty(new_structure, ENTITY_PROPERTY_interactable);
				EntitySetProperty(new_structure, ENTITY_PROPERTY_blueprint);
				
				new_structure->position = GetMousePositionInWorldSpace();
				
				new_structure->sprite_data.static_sprite = selected_structure->world_sprite;
				new_structure->sprite_data.tint = v4(0.5f, 0.5f, 1.0f, 0.4f);
				
				new_structure->structure_type = structures[selected_structure_index].type;
				StructureTypeData *structure_data = &global_structure_type_data[new_structure->structure_type];
				
				new_structure->interactable.bounds.aabb.min = c2V(-30.0f, -30.0f);
				new_structure->interactable.bounds.aabb.max = c2V(30.0f, 30.0f);
				new_structure->interactable.bounds_type = C2_SHAPE_TYPE_aabb;
				new_structure->interactable.priority = 5.0f;
				new_structure->interactable.interact_callback = OnBlueprintInteract;
				new_structure->interactable.enter_interactable_callback = OnBlueprintEnter;
				new_structure->interactable.exit_interactable_callback = OnBlueprintExit;
				
				MemoryCopy(new_structure->remaining_items_in_blueprint,
						   structure_data->recipe,
						   sizeof(structure_data->recipe));
			}
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			core->run_data->character_state &= ~CHARACTER_STATE_is_blueprinting;
			
			platform->key_pressed[KEY_esc] = 0;
			platform->key_down[KEY_esc] = 0;
		}
	}
	
	// NOTE(randy): $Remaining Items UI
	Entity *current_blueprint = core->run_data->current_interactable;
	if (current_blueprint &&
		current_blueprint->structure_type)
	{
		StructureTypeData *structure_data = &global_structure_type_data[current_blueprint->structure_type];
		
		i32 remaining_count = 0;
		for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
		{
			if (current_blueprint->remaining_items_in_blueprint[i].stack_size &&
				current_blueprint->remaining_items_in_blueprint[i].type)
				remaining_count++;
		}
		
		f32 start_pos = (remaining_count - 1) * 20.0f / -2.0f;
		
		i32 index = 0;
		for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
		{
			Item *item = &current_blueprint->remaining_items_in_blueprint[i];
			ItemTypeData *item_data = &global_item_type_data[item->type];
			
			if (item->stack_size > 0)
			{
				StaticSpriteData *sprite = &global_static_sprite_data[item_data->icon_sprite];
				
				v2 render_pos = v2view(V2AddV2(v2(current_blueprint->position.x + start_pos + index * 20.0f,
												  current_blueprint->position.y - 40.0f),
											   v2(sprite->source.z / -2.0f,
												  -sprite->source.w)));
				v2 render_size = v2zoom(v2(sprite->source.z, sprite->source.w));
				
				ArcPushTexture(sprite->texture_atlas,
							   0,
							   sprite->source,
							   v4(render_pos.x, render_pos.y,
								  render_size.x, render_size.y),
							   v4u(1.0f),
							   LAYER_HUD);
				
				char remaining_count[5];
				sprintf(remaining_count, "%i", item->stack_size);
				
				Ts2dPushText(Ts2dGetDefaultFont(),
							 0,
							 v4u(1.0f),
							 render_pos,
							 0.3f,
							 remaining_count);
				
				index++;
			}
		}
	}
}

internal b8 CanPlayerInteract()
{
	return !(core->run_data->character_state & CHARACTER_STATE_is_crafting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_backpack_open) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_blueprinting) &&
		!core->run_data->is_paused;
}