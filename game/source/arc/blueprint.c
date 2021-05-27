internal void OnWoodenWallBuild(Entity *entity)
{
	EntityUnsetProperty(entity, ENTITY_PROPERTY_blueprint);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	entity->sprite_data.tint = v4u(1.0f);
	c2AABB aabb = {
		.min = c2V(-5.0f, -60.0f),
		.max = c2V(5.0f, 0.0f),
	};
	entity->physics.shape.aabb = aabb;
	entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
	entity->physics.material.restitution = 0.1f;
	entity->physics.material.static_friction = 0.1f;
	entity->physics.material.dynamic_friction = 0.1f;
	entity->physics.gravity_multiplier = 0.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_ground;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_item | PHYSICS_BODY_TYPE_FLAGS_character;
}

internal void UpdateBlueprints()
{
	Entity *character = core->run_data->character_entity;
	
	// NOTE(randy): Blueprint UI & placement
	if (core->run_data->character_state & CHARACTER_STATE_arcane_mode)
	{
		StructureType structures[16] = {0};
		i32 structure_count = 0;
		for (i32 i = 1; i < STRUCTURE_TYPE_MAX; i++)
		{
			StructureTypeData *structure_data = &global_structure_type_data[i];
			if (structure_data->category)
				structures[structure_count++] = i;
		}
		
		for (i32 i = 0; i < structure_count; i++)
		{
			StructureTypeData *structure_data = &global_structure_type_data[structures[i]];
			
			const f32 spacing = 0.1f;
			f32 inital = 0.5f - (structure_count / 2.0f * spacing);
			
			v2 render_pos = v2screen(v2(inital + i * spacing, 0.9f));
			v2 render_size = v2(100.f, 100.f);
			
			// TODO(randy): Need to sort the ground shader properly, right now we just render this without a layer to go over the top of it
			SpriteData *sprite = &global_sprite_data[structure_data->icon_sprite];
			Ts2dPushTexture(sprite->texture_atlas,
							sprite->source,
							v4(render_pos.x, render_pos.y,
							   render_size.x, render_size.y));
		}
		
		/*
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
		 */
	}
	
	//~Remaining Items UI
	Entity *current_blueprint = GetClosestEntityWithProperty(ENTITY_PROPERTY_blueprint, 50.0f);
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
				
				ArcPushText(Ts2dGetDefaultFont(),
							0,
							v4u(1.0f),
							render_pos,
							0.3f,
							remaining_count,
							LAYER_HUD);
				
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
