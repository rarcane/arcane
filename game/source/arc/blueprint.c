/*
internal void OnCraftingStumpBuild(Entity *entity)
{
	EntityUnsetProperty(entity, ENTITY_PROPERTY_blueprint);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	entity->sprite_data.tint = v4u(1.0f);
	entity->interact_callback = OnCraftingTableInteract;
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
 */

internal void DrawBlueprintUI()
{
	Entity *character = core->run_data->character_entity;
	
	if (GetCharacterData()->hotbar[GetCharacterData()->active_hotbar_slot].type == ITEM_TYPE_crafting_tool &&
		platform->left_mouse_pressed &&
		CanPlayerInteract())
	{
		core->run_data->character_state |= CHARACTER_STATE_is_blueprinting;
		SetArcaneMode(1);
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
			SetArcaneMode(0);
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
				
				Sprite texture = 0;
				switch(i)
				{
					case STRUCTURE_CATEGORY_shia:
					{
						texture = SPRITE_shia;
					} break;
					
					case STRUCTURE_CATEGORY_crafting:
					{
						texture = SPRITE_crafting_tool;
					} break;
					
					case STRUCTURE_CATEGORY_base:
					{
						texture = SPRITE_flint_sword_icon;
					} break;
				}
				
				SpriteData *category_sprite = &global_sprite_data[texture];
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
				
				SpriteData *sprite = &global_sprite_data[structures[i].data->icon_sprite];
				ArcPushTexture(sprite->texture_atlas,
							   0,
							   sprite->source,
							   v4(render_pos.x, render_pos.y,
								  render_size.x, render_size.y),
							   selected_structure == structures[i].data ? v4(1.0f, 0.0f, 0.0f, 1.0f) : v4u(1.0f),
							   LAYER_HUD);
			}
			
			// NOTE(randy): Render blueprint at mouse location
			SpriteData *sprite = &global_sprite_data[selected_structure->world_sprite];
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
				
				new_structure->sprite_data.sprite = selected_structure->world_sprite;
				new_structure->sprite_data.tint = v4(0.5f, 0.5f, 1.0f, 0.4f);
				
				new_structure->structure_type = structures[selected_structure_index].type;
				StructureTypeData *structure_data = &global_structure_type_data[new_structure->structure_type];
				
				MemoryCopy(new_structure->remaining_items_in_blueprint,
						   structure_data->recipe,
						   sizeof(structure_data->recipe));
			}
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			core->run_data->character_state &= ~CHARACTER_STATE_is_blueprinting;
			SetArcaneMode(0);
			
			platform->key_pressed[KEY_esc] = 0;
			platform->key_down[KEY_esc] = 0;
		}
	}
	
	// NOTE(randy): $Remaining Items UI
	Entity *current_blueprint = GetClosestEntityWithProperty(ENTITY_PROPERTY_blueprint, 50.0f);
	if ((core->run_data->character_state & CHARACTER_STATE_arcane_mode) &&
		current_blueprint &&
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
		
		// NOTE(randy): Render remaining items
		i32 index = 0;
		for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
		{
			Item *item = &current_blueprint->remaining_items_in_blueprint[i];
			ItemTypeData *item_data = &global_item_type_data[item->type];
			
			if (item->stack_size > 0)
			{
				SpriteData *sprite = &global_sprite_data[item_data->icon_sprite];
				
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
		
		// NOTE(randy): Contributing items
		if (platform->key_pressed[KEY_e])
		{
			b8 blueprint_unfinished = 0;
			for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
			{
				if (!current_blueprint->remaining_items_in_blueprint[i].type)
					continue;
				
				i32 removed_amount = RemoveItemFromContainer(current_blueprint->remaining_items_in_blueprint[i],
															 GetCharacterData()->inventory,
															 GetCharacterData()->inventory_size);
				current_blueprint->remaining_items_in_blueprint[i].stack_size -= removed_amount;
				
				if (current_blueprint->remaining_items_in_blueprint[i].stack_size)
					blueprint_unfinished = 1;
			}
			
			if (!blueprint_unfinished)
			{
				StructureTypeData *structure_data = &global_structure_type_data[current_blueprint->structure_type];
				if (structure_data->on_structure_build)
					structure_data->on_structure_build(current_blueprint);
			}
			
			TsPlatformCaptureKeyboard();
		}
	}
}
