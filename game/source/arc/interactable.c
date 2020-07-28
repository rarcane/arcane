internal void InteractableUpdate()
{
	if (core->run_data->disable_interaction)
	{
		core->run_data->current_interactable = 0;
		return;
	}
	
	PositionComponent *player_pos_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	PhysicsBodyComponent *player_body_comp = GetPhysicsBodyComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	InteractableComponent *highest_priority_interactable = 0;
	for (i32 i = 0; i < core->run_data->entity_components.interactable_component_count; i++)
	{
		InteractableComponent *inter_comp = &core->run_data->entity_components.interactable_components[i];
		if (inter_comp->parent_entity_id)
		{
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(inter_comp->parent_entity_id);
			Assert(pos_comp);
			
			c2Shape inter_shape = inter_comp->bounds;
			AddPositionOffsetToShape(&inter_shape,
									 inter_comp->bounds_type,
									 pos_comp->position);
			
			c2Shape player_shape = player_body_comp->shape;
			AddPositionOffsetToShape(&player_shape,
									 player_body_comp->shape_type,
									 player_pos_comp->position);
			
			c2Manifold manifold = {0};
			GenerateCollisionManifold(inter_shape, inter_comp->bounds_type,
									  player_shape, player_body_comp->shape_type,
									  &manifold);
			if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
			{
				// NOTE(randy): Player is within interaction bounds.
				if (!highest_priority_interactable ||
					inter_comp->priority > highest_priority_interactable->priority)
				{
					highest_priority_interactable = inter_comp;
				}
			}
		}
	}
	
	core->run_data->current_interactable = highest_priority_interactable;
	if (highest_priority_interactable && platform->key_pressed[KEY_e])
	{
		platform->key_pressed[KEY_e] = 0;
		
		if (highest_priority_interactable->interact_callback)
		{
			highest_priority_interactable->interact_callback(GetEntityWithID(highest_priority_interactable->parent_entity_id));
		}
	}
}

internal void OnCraftingTableInteract(Entity *entity)
{
	Assert(!core->run_data->engaged_station);
	
	StationComponent *station_comp = GetStationComponentFromEntityID(entity->entity_id);
	Assert(station_comp && station_comp->type == STATION_TYPE_crafting);
	
	core->run_data->engaged_station = station_comp;
	core->run_data->engaged_station_type = STATION_TYPE_crafting;
	core->run_data->disable_interaction = 1;
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
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	if (core->run_data->engaged_station)
	{
		PositionComponent *station_pos_comp = GetPositionComponentFromEntityID(core->run_data->engaged_station->parent_entity_id);
		core->run_data->disable_player_input = 1;
		
		switch (core->run_data->engaged_station_type)
		{
			case STATION_TYPE_crafting :
			{
				CraftingStation *crafting_station_data = &core->run_data->engaged_station->data.crafting;
				
				CraftingRecipeType craftable_recipes[CRAFTING_RECIPE_TYPE_MAX];
				i32 craftable_recipe_count = 0;
				
				i32 existing_recipe_index = -1;
				for (i32 i = 1;
					 i < CRAFTING_RECIPE_TYPE_MAX;
					 i++)
				{
					if (IsRecipeCraftable(i, player_data->inventory, player_data->inventory_size))
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
					v2 render_pos = v2view(v2(station_pos_comp->position.x - 20.0f,
											  station_pos_comp->position.y - 40.0f));
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
					v2 render_pos = v2view(v2(station_pos_comp->position.x + 20.0f,
											  station_pos_comp->position.y - 40.0f));
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
				
				v2 render_pos = v2view(v2(station_pos_comp->position.x - 7.0f,
										  station_pos_comp->position.y - 43.0f));
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
						
						RemoveItemFromContianer(*recipe_item,
												player_data->inventory,
												player_data->inventory_size);
					}
					
					core->run_data->engaged_station = 0;
					core->run_data->disable_player_input = 0;
					core->run_data->disable_interaction = 0;
					
					Entity *entity = NewGroundItemEntity(v2(station_pos_comp->position.x,
															station_pos_comp->position.y - 43.0f),
														 recipe->output);
					InteractableComponent *inter_comp = GetInteractableComponentFromEntityID(entity->entity_id);
				}
			} break;
			
			default :
			Assert(0);
			break;
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			core->run_data->engaged_station = 0;
			core->run_data->disable_player_input = 0;
			core->run_data->disable_interaction = 0;
		}
		
		TsPlatformCaptureKeyboard();
	}
}

internal void BlueprintUpdate()
{
	PlayerDataComponent *player_dat = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	PositionComponent *pos_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	if (player_dat->hotbar[player_dat->active_hotbar_slot].type == ITEM_TYPE_crafting_tool &&
		platform->left_mouse_pressed)
	{
		core->run_data->disable_player_input = 1;
		core->run_data->is_blueprinting = 1;
	}
	
	// NOTE(randy): comment go brr
	if (core->run_data->is_blueprinting)
	{
		i32 structure_count = STRUCTURE_CATEGORY_MAX - 1;
		
		f32 start_pos = structure_count * 20.0f / -2.0f;
		
		local_persist i32 selected_category = (STRUCTURE_CATEGORY_MAX - 1) / 2 + 1;
		
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
		
		// NOTE(randy): this is a comment.
		for (i32 i = 1; i < STRUCTURE_CATEGORY_MAX;  i++)
		{
			v2 render_pos = v2view(v2(pos_comp->position.x + start_pos + (i - 1) * 20.0f, pos_comp->position.y - 60.0f));
			v2 render_size = v2zoom(v2(20.0f, 20.0f));
			
			StaticSprite texture = STATIC_SPRITE_INVALID;
			switch(i)
			{
				case STRUCTURE_CATEGORY_shia:
				{
					texture = STATIC_SPRITE_shia;2
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
			
			// NOTE(randy): i have successfully documented this function with adequate comments.
			StaticSpriteData *category_sprite = &global_static_sprite_data[texture];
			ArcPushTexture(category_sprite->texture_atlas,
						   0,
						   category_sprite->source,
						   v4(render_pos.x, render_pos.y,
							  render_size.x, render_size.y),
						   selected_category == i ? v4(1.0f, 0.0f, 0.0f, 1.0f) : v4u(1.0f),
						   LAYER_FOREGROUND_GEORGE);
		}
	}
}