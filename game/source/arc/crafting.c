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

internal void DrawCraftingUI()
{
	if (!(core->run_data->character_state & CHARACTER_STATE_arcane_mode) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_crafting))
		return;
	
	// NOTE(randy): Get items in range
	Entity *overlapping_entities[MAX_OVERLAPPING_COLLIDERS];
	c2Shape shape = {
		.aabb = { .min = c2V(-50.0f, -50.0f), .max = c2V(50.0f, 50.0f) },
	};
	shape.aabb = v2AddAABB(GetCharacterEntity()->position, shape.aabb);
	i32 overlap_count = GetOverlappingBodiesWithShape(overlapping_entities, shape, C2_SHAPE_TYPE_aabb);
	
	Item item_pool[32] = {0};
	Entity *entity_item_pool[32] = {0};
	i32 item_pool_count = 0;
	
	v2 item_position = {0};
	
	for (i32 i = 0; i < overlap_count; i++)
	{
		if (EntityHasProperty(overlapping_entities[i], ENTITY_PROPERTY_item))
		{
			if (item_pool_count == 0)
				item_position = overlapping_entities[i]->position;
			
			entity_item_pool[item_pool_count] = overlapping_entities[i];
			item_pool[item_pool_count++] = overlapping_entities[i]->item;
		}
	}
	
	// NOTE(randy): Get craftable recipes from those items
	CraftingRecipeType craftable_recipes[CRAFTING_RECIPE_TYPE_MAX] = {0};
	i32 craftable_recipe_count = 0;
	local_persist i32 selected_recipe_index = -1;
	for (i32 i = 1;
		 i < CRAFTING_RECIPE_TYPE_MAX;
		 i++)
	{
		if (IsRecipeCraftable(i, item_pool, item_pool_count))
		{
			craftable_recipes[craftable_recipe_count++] = i;
		}
	}
	
	if (craftable_recipe_count == 0)
	{
		selected_recipe_index = -1;
		return;
	}
	
	if (!(core->run_data->character_state & CHARACTER_STATE_is_crafting) &&
		platform->key_pressed[KEY_e])
	{
		core->run_data->character_state |= CHARACTER_STATE_is_crafting;
		
		MoveCameraToLocation(V2AddV2(item_position, v2(0.0f, -50.0f)), 2.0f, 2.0f);
		
		TsPlatformCaptureKeyboard();
	}
	
	// NOTE(randy): No crafting recipe has been selected prior, so just select the highest craftable one.
	if (selected_recipe_index == -1)
	{
		selected_recipe_index = craftable_recipe_count - 1;
	}
	
	if (core->run_data->character_state & CHARACTER_STATE_is_crafting)
	{
		// NOTE(randy): Navigate LR through recipes
		if (selected_recipe_index - 1 >= 0)
		{
			if (platform->key_pressed[KEY_a])
			{
				selected_recipe_index = selected_recipe_index - 1;
				TsPlatformCaptureKeyboard();
			}
			
			SpriteData *arrow_sprite = &global_sprite_data[SPRITE_side_arrow];
			v2 render_pos = v2view(v2(item_position.x - 20.0f,
									  item_position.y - 40.0f));
			v2 render_size = v2zoom(v2(arrow_sprite->source.width,
									   arrow_sprite->source.height));
			ArcPushTexture(arrow_sprite->texture_atlas,
						   0,
						   arrow_sprite->source,
						   v4(render_pos.x,
							  render_pos.y,
							  render_size.x,
							  render_size.y),
						   v4u(1.0f),
						   LAYER_HUD);
		}
		
		
		if (selected_recipe_index + 1 < craftable_recipe_count)
		{
			if (platform->key_pressed[KEY_d])
			{
				selected_recipe_index = selected_recipe_index + 1;
				TsPlatformCaptureKeyboard();
			}
			
			SpriteData *arrow_sprite = &global_sprite_data[SPRITE_side_arrow];
			v2 render_pos = v2view(v2(item_position.x + 20.0f,
									  item_position.y - 40.0f));
			v2 render_size = v2zoom(v2(arrow_sprite->source.width,
									   arrow_sprite->source.height));
			ArcPushTexture(arrow_sprite->texture_atlas,
						   0,
						   arrow_sprite->source,
						   v4(render_pos.x,
							  render_pos.y,
							  -render_size.x,
							  render_size.y),
						   v4u(1.0f),
						   LAYER_HUD);
		}
		
		// NOTE(randy): Render the craftable sprite
		CraftingRecipeTypeData *recipe = &global_crafting_recipe_type_data[craftable_recipes[selected_recipe_index]];
		
		SpriteData *recipe_output_sprite = &global_sprite_data[global_item_type_data[recipe->output.type].icon_sprite];
		v2 render_pos = v2view(v2(item_position.x - 7.0f,
								  item_position.y - 43.0f));
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
										item_pool,
										item_pool_count);
			}
			
			for (i32 i = 0; i < 32; i++)
			{
				Entity *entity = entity_item_pool[i];
				if (!entity)
					break;
				
				entity->item = item_pool[i];
				
				if (entity->item.stack_size <= 0)
					DeleteEntity(entity);
			}
			
			Entity *entity = NewEntity();
			ItemEntityPresetCallback(entity);
			entity->position = v2(item_position.x,
								  item_position.y - 43.0f);
			entity->item = recipe->output;
			entity->priority = 10.0f;
			UpdateEntitySprite(entity);
			
			SetArcaneMode(0);
			TsPlatformCaptureKeyboard();
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			SetArcaneMode(0);
			TsPlatformCaptureKeyboard();
		}
	}
	else
	{
		// NOTE(randy): Display crafting prompt
		v2 render_pos = v2view(V2AddV2(item_position, v2(-2.0f, -30.0f)));
		ArcPushText(Ts2dGetDefaultFont(),
					0,
					v4u(1.0f),
					render_pos,
					0.5f,
					"E",
					LAYER_HUD);
	}
}