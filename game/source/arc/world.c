internal void WorldUpdate()
{
	TickTimers();
	
	// NOTE(randy): Testing
	{
		/*
				if (platform->key_down[KEY_p])
				{
					CameraCue camera = {0};
					camera.position = v2(0.0f, -200.0f);
					camera.weight = 1.0f;
					
					core->run_data->camera_cues[core->run_data->camera_cue_count++] = camera;
				}
				
				if (platform->key_down[KEY_o])
				{
					CameraCue camera = {0};
					camera.position = v2(0.0f, 200.0f);
					camera.weight = 1.0f;
					
					core->run_data->camera_cues[core->run_data->camera_cue_count++] = camera;
				}
		 */
	}
	
#ifdef DEVELOPER_TOOLS
	DrawEditorUI();
	if (core->run_data->editor_state)
		TransformEditorCamera();
#endif
    
	core->performance_timer_count = 0;
    
	START_PERF_TIMER("Update");
	
	if ((core->world_delta_t == 0.0f ? (core->run_data->debug_flags & DEBUG_FLAGS_manual_step) : 1))
	{
		UpdateCells();
		
		PreMoveUpdatePlayer();
        
		UpdatePhysics();
        
		if (!core->run_data->editor_state)
			TransformInGameCamera();
        
		PostMoveUpdatePlayer();
        
		core->run_data->debug_flags &= ~DEBUG_FLAGS_manual_step;
	}
    
	UpdateParallax();
    
	UpdateChunks();
    
	InteractableUpdate();
	
	DrawWorld();
	RenderCells();
	
#ifdef DEVELOPER_TOOLS
	RenderColliders();
#endif
    
	DrawGameUI();
#ifdef DEVELOPER_TOOLS
	DrawDebugLines();
#endif
    
	END_PERF_TIMER;
}

internal void DrawWorld()
{
#ifdef DEVELOPER_TOOLS
	if (!(core->run_data->debug_flags & DEBUG_FLAGS_draw_world))
		return;
#endif
    
	
	Ts2dPushBackgroundBegin();
	{
		v4 top_sky_colour = v4(88.0f / 255.0f, 152.0f / 255.0f, 194.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(203.0f / 255.0f, 232.0f / 255.0f, 238.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(130.0f / 255.0f, 81.0f / 255.0f, 77.0f / 255.0f, 1.0f);
        
		Ts2dPushFilledVertexColoredRect(top_sky_colour,
										middle_sky_colour,
										top_sky_colour,
										middle_sky_colour,
										v4(0.0f, 0.0f, core->render_width, core->render_height / 2.0f));
        
		Ts2dPushFilledVertexColoredRect(middle_sky_colour,
										bottom_sky_colour,
										middle_sky_colour,
										bottom_sky_colour,
										v4(0.0f, core->render_height / 2.0f, core->render_width, core->render_height / 2.0f));
        
		//RenderBackgroundSprites();
	}
	Ts2dPushBackgroundEnd();
    
	Ts2dWorldInfo world_info = {0};
	{
		world_info.flags |= TS2D_BLUR_SHADOWS;
		world_info.flags |= TS2D_BLUR_BACKGROUNDS;
		if (core->client_data->bloom)
			world_info.flags |= TS2D_BLOOM;
		// world_info.shadow_opacity = 1.0f;
		world_info.shadow_vector = v2(32.0f, 64.0f);
		world_info.camera_pos = core->camera_position;
		world_info.camera_zoom = core->camera_zoom;
		world_info.brightness = 1.0f;
		world_info.grayscale = 0.f;
	}
    
	Ts2dPushWorldBegin(&world_info);
    
	// NOTE(randy): Sprite rendering.
	{
		UpdateAnimations();
		// PostUpdateWorldAnimations();
		//RenderForegroundSprites();
		
		RenderSprites();
	}
    
	/*
		v2 pos = v2view(v2(-350.0f, 35.0f));
		v2 size = v2zoom(v2(700.0f, 200.0f));
		Ts2dPushReflectiveRect(v4(pos.x, pos.y, size.x, size.y),
							   v4(1.0f, 1.0f, 1.0f, 1.0f),
							   0.0f, 0.75f);
	 */
    
	Ts2dPushWorldEnd();
    
	// NOTE(randy): Draw velocity projection.
	/* if (core->draw_velocity)
    {
       for (int j = 0; j < core->run_data->entity_components.velocity_component_count; j++)
       {
          VelocityComponent *velocity_comp = &core->run_data->entity_components.velocity_components[j];
          PositionComponent *PositionComponent = velocity_comp->parent_entity->components[COMPONENT_position];
 
          Ts2dPushLine(
             core->renderer,
             v4(1.0f, 0.0f, 0.0f, 1.0f),
             v2view(PositionComponent->position),
             v2view(V2AddV2(PositionComponent->position, V2MultiplyV2(velocity_comp->velocity, v2(0.5f, 0.5f)))));
       }
    } */
}

internal void UpdateParallax()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_parallaxable);)
	{
		// TODO: Need to find a way to centralise the desired_position of the parallax, whilst still maintaining spatial consistency across sprites
		entity->position.x = entity->desired_position.x + core->camera_position.x * entity->parallax_amount.x;
		entity->position.y = entity->desired_position.y + (core->camera_position.y + DEFAULT_CAMERA_OFFSET_Y) * entity->parallax_amount.y;
		
		// position_comp->position.x = parallax_comp->desired_position.x - (parallax_comp->desired_position.x - player_pos->position.x + core->camera_offset.x) * parallax_comp->parallax_amount.x;
		// position_comp->position.y = parallax_comp->desired_position.y - (parallax_comp->desired_position.y - player_pos->position.y + core->camera_offset.y) * parallax_comp->parallax_amount.y;
	}
}

internal void GenerateTestPlatform()
{
	i32 width = 2000;
	i32 depth = 150;
	f32 height_variation = 20;
	f32 edge_percent = 0.4f;
	
	i32 ground_interval = 16;
	Entity *previous_ground_entity = 0;
	
	i32 height_map[200];
	
	for (i32 i = 0; i < width; i++)
	{
		i32 x_pos = i - width / 2;
		
		f32 height_noise;
		{
			i32 octaves = 8;
			f32 frequency = 1.0f;
			f32 amplitude = 5.0f;
			f32 max_noise_value = 0.0f;
			f32 noise_amount = 0.0f;
			for (int k = 0; k < octaves; k++)
			{
				noise_amount += GetPerlinNoise((f32)i / (f32)PERLIN_NOISE_LENGTH * frequency, 0.0f) * amplitude;
				max_noise_value += amplitude;
				frequency *= 2.0f;
				amplitude *= 0.5f;
			}
			
			height_noise = noise_amount / max_noise_value;
		}
		
		i32 height = (i32)(height_noise * height_variation);
		
		if (i % 10)
		{
			height_map[i / 10] = height;
		}
		
		f32 depth_noise;
		{
			i32 octaves = 8;
			f32 frequency = 0.5f;
			f32 amplitude = 50.0f;
			f32 max_noise_value = 0.0f;
			f32 noise_amount = 0.0f;
			for (int k = 0; k < octaves; k++)
			{
				noise_amount += GetPerlinNoise((f32)i / (f32)PERLIN_NOISE_LENGTH * frequency, 0.0f) * amplitude;
				max_noise_value += amplitude;
				frequency *= 2.0f;
				amplitude *= 0.5f;
			}
			
			depth_noise = noise_amount / max_noise_value;
		}
		
		f32 edge_length = (f32)width * edge_percent;
		i32 depth_with_edges;
		f32 alpha;
		if (i < edge_length)
		{
			alpha = (f32)i / edge_length;
		}
		else if (i > width - edge_length)
		{
			alpha = ((f32)width - (f32)i) / edge_length;
		}
		
		depth_with_edges = (i32)LerpF32(alpha, 2.0f, (f32)depth - height);
		depth_with_edges += (i32)(depth_noise * LerpF32(alpha, 10.0f, 200.0f));
		
		for (i32 y = 0; y < depth_with_edges; y++)
		{
			i32 y_pos = height + y;
			
			Chunk *chunk = GetChunkAtIndex(WorldSpaceToChunkIndex((f32)x_pos), WorldSpaceToChunkIndex((f32)y_pos));
			if (!chunk)
			{
				chunk = &core->run_data->active_chunks[core->run_data->active_chunk_count++];
				
				chunk->is_valid = 1;
				chunk->remain_loaded = 1;
				chunk->x_index = WorldSpaceToChunkIndex((f32)x_pos);
				chunk->y_index = WorldSpaceToChunkIndex((f32)y_pos);
				chunk->entity_count = 0;
				chunk->entity_ids = 0;
			}
			
			Cell *cell = GetCellAtPosition(x_pos, y_pos);
			cell->material_type = CELL_MATERIAL_TYPE_dirt;
			
			QueueChunkForTextureUpdate(chunk);
		}
		
		// NOTE(randy): $Generate ground
		if (i % ground_interval == 0)
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_physical);
			entity->position = v2((f32)x_pos, (f32)height);
			
			Line line = {0};
			if (previous_ground_entity)
			{
				previous_ground_entity->physics.shape.line.p2.x = (f32)ground_interval;
				previous_ground_entity->physics.shape.line.p2.y = (f32)height - previous_ground_entity->position.y;
			}
			
			
			entity->physics.shape.line = line;
			entity->physics.shape_type = C2_SHAPE_TYPE_line;
			entity->physics.material.static_friction = 0.7f;
			entity->physics.material.dynamic_friction = 0.7f;
			entity->physics.mass_data.mass = 0.0f;
			entity->physics.mass_data.inv_mass = 0.0f;
			entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_ground;
			entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_character | PHYSICS_BODY_TYPE_FLAGS_item;
			
			previous_ground_entity = entity;
		}
	}
	
	// NOTE(randy): $Ron
	{
		Entity *entity = NewEntity();
		EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable_left_click);
		EntitySetProperty(entity, ENTITY_PROPERTY_queryable);
		EntitySetProperty(entity, ENTITY_PROPERTY_enemy);
		
		entity->position = v2(-120.0f, -5.0f);
		entity->sprite_data.static_sprite = STATIC_SPRITE_dummy;
		entity->sprite_data.render_layer = 0.5f;
		entity->sprite_data.scale = v2(0.1f, 0.1f);
		
		entity->priority = 2.0f;
		
		entity->durability = 30.0f;
		
		entity->physics.shape.aabb.min = c2V(-10.0f, 0.0f);
		entity->physics.shape.aabb.max = c2V(10.0f, 40.0f);
		entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
	}
	
	// NOTE(randy): runic enchanter
	{
		Entity *entity = NewEntity();
		EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable_e);
		EntitySetProperty(entity, ENTITY_PROPERTY_queryable);
		
		entity->position = v2(-70.0f, -5.0f);
		entity->sprite_data.static_sprite = STATIC_SPRITE_runic_enchanter;
		entity->sprite_data.render_layer = 0.5f;
		
		entity->priority = 2.0f;
		entity->interact_callback = OnEnchanterInteract;
		
		entity->physics.shape.aabb.min = c2V(-10.0f, 0.0f);
		entity->physics.shape.aabb.max = c2V(10.0f, 20.0f);
		entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
	}
	
	// NOTE(randy): elemental skill tree
	{
		Entity *entity = NewEntity();
		EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
		EntitySetProperty(entity, ENTITY_PROPERTY_interactable_e);
		EntitySetProperty(entity, ENTITY_PROPERTY_queryable);
		
		entity->position = v2(-20.0f, -5.0f);
		entity->sprite_data.static_sprite = STATIC_SPRITE_runic_enchanter;
		entity->sprite_data.tint = v4(1.0f, 0.0f, 0.0f, 1.0f);
		entity->sprite_data.render_layer = 0.5f;
		
		entity->priority = 2.0f;
		entity->interact_callback = OnElementalSkillTreeInteract;
		
		entity->physics.shape.aabb.min = c2V(-10.0f, 0.0f);
		entity->physics.shape.aabb.max = c2V(10.0f, 20.0f);
		entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
	}
	
	// NOTE(randy): $Generate background stuff
	{
		// NOTE(randy): $Trees
		for (i32 i = 10; i < 200; i += 10)
		{
			if (RandomI32(0, 3))
			{
				i32 height = height_map[i];
				i32 x_pos = i * 10 - width / 2 + 10;
				
				Entity *entity = NewEntity();
				EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
				EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
				EntitySetProperty(entity, ENTITY_PROPERTY_interactable_left_click);
				EntitySetProperty(entity, ENTITY_PROPERTY_queryable);
				EntitySetProperty(entity, ENTITY_PROPERTY_lumber_axable);
				
				entity->position = v2((f32)x_pos, (f32)height + 1);
				entity->sprite_data.static_sprite = STATIC_SPRITE_pine_tree_v1;
				entity->sprite_data.render_layer = LAYER_TREE;
				
				entity->priority = 2.0f;
				
				entity->durability = 30.0f;
				
				entity->physics.shape.aabb.min = c2V(-3.0f, 0.0f);
				entity->physics.shape.aabb.max = c2V(3.0f, 50.0f);
				entity->physics.shape_type = C2_SHAPE_TYPE_aabb;
			}
		}
		
		// NOTE(randy): $Hill layer 1
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-458.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-272.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-93.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(93.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(272.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(458.0f, 0.0f);
			entity->parallax_amount = HILLS_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			entity->sprite_data.render_layer = LAYER_HILL_1;
		}
		
		// NOTE(randy): $bg trees 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(80, 150))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_TREE_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg1_pine_tree_v1 + RandomI32(0, 1);
			entity->sprite_data.render_layer = LAYER_BG_TREE_1;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_SHRUB_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg1_shrub_v1 + RandomI32(0, 2);
			entity->sprite_data.render_layer = LAYER_BG_SHRUBS_1;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg saplings 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(30, 90))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_SAPLING_1_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg1_sapling_v1 + RandomI32(0, 2);
			entity->sprite_data.render_layer = LAYER_BG_SAPLINGS_1;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $Hill layer 2
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-500, 0.0f);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-300.0f, 0.0f);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-100, 0.0f);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(100, 0.0f);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(300, 0);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(500, 0.0f);
			entity->parallax_amount = HILLS_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILL_2;
		}
		
		// NOTE(randy): $bg trees 2
		for (i32 x = width / -2 + 450; x < width / 2 - 450; x += RandomI32(80, 150))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_TREE_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_pine_tree_v1 + RandomI32(0, 1);
			entity->sprite_data.render_layer = LAYER_BG_TREES_2;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 2
		for (i32 x = width / -2 + 450; x < width / 2 - 450; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_SHRUB_2_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg2_shrub_v1 + RandomI32(0, 2);
			entity->sprite_data.render_layer = LAYER_BG_SHRUBS_2;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $Hill layer 3
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-500.0f, 0.0f);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-300.0f, 0.0f);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-100, 0.0f);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(100, 0.0f);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(300, 0);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-500.0f, 0.0f);
			entity->parallax_amount = HILLS_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			entity->sprite_data.render_layer = LAYER_HILLS_3;
		}
		
		// NOTE(randy): $bg trees 3
		for (i32 x = width / -2 + 500; x < width / 2 - 650; x += RandomI32(30, 50))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_TREE_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_pine_tree_v1 + RandomI32(0, 7);
			entity->sprite_data.render_layer = LAYER_BG_TREES_3;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 3
		for (i32 x = width / -2 + 500; x < width / 2 - 650; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x, 0.0f);
			entity->parallax_amount = BG_SHRUB_3_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_bg3_shrub_v1 + RandomI32(0, 3);
			entity->sprite_data.render_layer = LAYER_BG_SHRUBS_3;
			entity->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $mid mountains
		for (i32 x = -1; x < 2; x++)
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2((f32)x * 500, 0.0f);
			entity->parallax_amount = MID_MOUNTAINS_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_mid_mountains;
			entity->sprite_data.render_layer = LAYER_MID_MOUNTAINS;
		}
		
		// NOTE(randy): $far mountains
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(-300, 0.0f);
			entity->parallax_amount = FAR_MOUNTAINS_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_far_mountains;
			entity->sprite_data.render_layer = LAYER_FAR_MOUNTAINS;
		}
		{
			Entity *entity = NewEntity();
			EntitySetProperty(entity, ENTITY_PROPERTY_force_floating);
			EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
			EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
			
			entity->position = v2(0, 0);
			
			entity->desired_position = v2(300, 0.0f);
			entity->parallax_amount = FAR_MOUNTAINS_PARALLAX;
			
			entity->sprite_data.static_sprite = STATIC_SPRITE_far_mountains;
			entity->sprite_data.render_layer = LAYER_FAR_MOUNTAINS;
		}
	}
}

internal b8 CreateWorld(char *world_name)
{
	Assert(world_name[0] && strlen(world_name) < sizeof(core->run_data->world_name));
	
	// NOTE(randy): Create world directories & initialise world data
	{
		char path[300] = "";
		sprintf(path, "%sworlds\\", core->res_path);
		
		if (!platform->DoesDirectoryExist(path))
			platform->MakeDirectory(path);
		
		strcat(path, world_name);
		strcat(path, "\\");
		if (platform->DoesDirectoryExist(path))
			return 0;
		platform->MakeDirectory(path);
		
		char chunks_path[300] = "";
		sprintf(chunks_path, "%schunks\\", path);
		platform->MakeDirectory(chunks_path);
		
		strcpy(core->run_data->world_name, world_name);
		strcpy(core->run_data->world_path, path);
		strcpy(core->run_data->world_chunks_path, chunks_path);
		InitialiseRunData();
	}
	
	GenerateTestPlatform();
	
	// NOTE(randy): Initialise the player
	{
		// TODO(randy): clean up
		Entity *character = NewEntity();
		core->run_data->character_entity = character;
		
		char nm[100] = "player";
		strncpy(character->debug_name, nm, sizeof(nm));
		
		EntitySetProperty(character, ENTITY_PROPERTY_no_delete);
		EntitySetProperty(character, ENTITY_PROPERTY_sprite);
		EntitySetProperty(character, ENTITY_PROPERTY_flipbook);
		EntitySetProperty(character, ENTITY_PROPERTY_physical);
		EntitySetProperty(character, ENTITY_PROPERTY_is_character);
		character->position = v2(0.0f, -100.0f);
		
		c2Capsule capsule = {
			.a = c2V(0.0f, -10.0f),
			.b = c2V(0.0f, -50.0f),
			.r = 10.0f,
		};
		
		character->physics.shape.capsule = capsule;
		character->physics.shape_type = C2_SHAPE_TYPE_capsule;
		
		character->physics.mass_data.mass = 60.0f;
		character->physics.mass_data.inv_mass = 1.0f / character->physics.mass_data.mass;
		character->physics.material.restitution = 0.4f;
		character->physics.material.static_friction = 0.9f;
		character->physics.material.dynamic_friction = 0.9f;
		character->physics.gravity_multiplier = 1.0f;
		character->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_ground;
		character->physics.type |= PHYSICS_BODY_TYPE_FLAGS_character;
		
		character->move_speed = 100.0f;
		character->entity_type = ARC_ENTITY_TYPE_player;
		character->current_animation_state = ARC_ENTITY_ANIMATION_STATE_player_idle;
		character->sprite_data.dynamic_sprite = DYNAMIC_SPRITE_player_idle;
		
		
		CharacterData *character_data = &core->run_data->character_data;
		
		Item flint_sword = { .type = ITEM_TYPE_flint_sword, .stack_size = 1 };
		character_data->inventory[0] = flint_sword;
		Item flint_axe = { .type = ITEM_TYPE_flint_axe, .stack_size = 1 };
		character_data->inventory[3] = flint_axe;
		
		Item flint = { .type = ITEM_TYPE_flint, .stack_size = 7 };
		character_data->inventory[1] = flint;
		
		Item twig = { .type = ITEM_TYPE_twig, .stack_size = 14 };
		character_data->inventory[2] = twig;
		
		Item tool = { .type = ITEM_TYPE_crafting_tool, .stack_size = 1 };
		character_data->hotbar[0] = tool;
		
		character_data->inventory_size = 9;
		character_data->hotbar_size = 2;
		
		Item chest = { .type = ITEM_TYPE_test_chestpiece, .stack_size = 1 };
		character_data->equipment_slots[1] = chest;
		
		character_data->freehand_spell_count = 2;
		character_data->freehand_spell_slots[0].type = SPELL_TYPE_fireball;
		character_data->freehand_spell_slots[1].type = SPELL_TYPE_yeet;
	}
	
	// FillChunkEntities();
	
	// NOTE(randy): Initial save.
	SaveWorld();
	//Assert(core->run_data->save_job_index != -1);
	//while(!platform->WaitForJob(core->run_data->save_job_index, TS_WAIT_FOREVER));
	
	core->is_ingame = 1;
	
	Log("Created new world '%s' successfully.", world_name);
	return 1;
}

internal b8 LoadWorld(char *world_name)
{
	Assert(world_name[0] && strlen(world_name) < sizeof(core->run_data->world_name));
	
	char path[300] = "";
	sprintf(path, "%sworlds\\%s\\", core->res_path, world_name);
	
	if (!platform->DoesDirectoryExist(path))
		return 0;
	
	{
		FreeRunData();
		MemorySet(core->run_data, 0, sizeof(RunData));
		
		strcpy(core->run_data->world_name, world_name);
		strcpy(core->run_data->world_path, path);
		InitialiseRunData();
	}
	
	// Read in basic data from the world_data.save
	{
		char file_path[200] = "";
		sprintf(file_path, "%slevel_data.save", path);
		FILE *file = fopen(file_path, "rb");
		Assert(file);
		
		ReadWorldSaveDataFromFile(file, &core->run_data->world);
		
		ReadCharacterDataFromFile(file, &core->run_data->character_data);
		
		for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
		{
			Entity *entity = NewEntity();
			ReadEntityFromFile(file, entity);
			
			if (EntityHasProperty(entity, ENTITY_PROPERTY_is_character))
				core->run_data->character_entity = entity;
		}
		
		fclose(file);
	}
	
	core->is_ingame = 1;
	
	Log("Successfully loaded world \'%s\'", world_name);
	
	return 1;
}

internal void UnloadWorld()
{
	Assert(core->is_ingame && core->run_data->world_name[0]);
	
	SaveWorld();
	
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	core->is_ingame = 0;
}

internal int SaveQueuedChunks(void *job_data)
{
	/*
		if (core->run_data->save_job_index == -1)
			return 1; // NOTE(randy): Temporary fix, this ideally shouldn't be called
		//Assert(core->run_data->save_job_index != -1);
		f32 start_time = platform->GetTime();
		
		// NOTE(randy): Could probably move all the snapshot data out of the RunData struct to make this reentrant but ehhhhhhhh cbf
		
		for (i32 i = 0; i < core->run_data->chunk_save_count; i++)
		{
			ChunkSave *chunk_save = &core->run_data->chunk_saves[i];
			
			char chunk_file_path[300];
			sprintf(chunk_file_path, "%s%i.%i.chunk", core->run_data->world_chunks_path, chunk_save->skele_chunk.x_index, chunk_save->skele_chunk.y_index);
			FILE *file = fopen(chunk_file_path, "wb");
			Assert(file);
			
			// NOTE(randy): Write cells to file.
			WriteToFile(file, &chunk_save->cells, sizeof(chunk_save->cells));
			
			// NOTE(randy): Write entities into the chunk file. Layout is as follows.
			// entity count, all entities, component1 count, all component1s, component2 count, all component2s...
			WriteToFile(file, &chunk_save->entity_count, sizeof(chunk_save->entity_count));
			for (i32 j = 0; j < chunk_save->entity_count; j++)
			{
				Entity *entity = &core->run_data->entities_snapshot[chunk_save->entity_ids[j] - 1];
				
				// NOTE(randy): Fill out an EntitySave structure for serialisation
				EntitySave entity_save = {.flags = entity->flags, .type = entity->generalised_type};
				MemoryCopy(entity_save.name, entity->name, sizeof(entity_save.name));
				
				if (chunk_save->skele_chunk.x_index == 3 && chunk_save->skele_chunk.y_index == -1)
				{
					entity_save.flags = (uint32)j + 1;
				}
				
				WriteToFile(file, &entity_save, sizeof(EntitySave));
			}
			SerialiseComponentsFromDataSet(file, core->run_data->entities_snapshot, core->run_data->entity_count_snapshot, &core->run_data->entity_components_snapshot, chunk_save->entity_ids, chunk_save->entity_count);
			
			fclose(file);
		}
		
		Log("Save job completed in %fms", (platform->GetTime() - start_time) * 1000.0f);
	 */
	
	return 1;
}

internal int LoadQueuedChunks(void *job_data)
{
	/*
		if (core->run_data->load_job_index == -1)
			return 1;
		//Assert(core->run_data->load_job_index != -1);
		
		f32 start_time = platform->GetTime();
		
		Assert(core->run_data->active_chunk_count + core->run_data->chunk_load_queue_count < MAX_WORLD_CHUNKS);
		
		for (i32 i = 0; i < core->run_data->chunk_load_queue_count; i++)
		{
			ChunkSave *chunk_save = &core->run_data->chunk_load_queue[i];
			
			Assert(!GetChunkAtIndex(chunk_save->skele_chunk.x_index, chunk_save->skele_chunk.y_index)); // NOTE(randy): Chunk is already loaded in.
			
			char chunk_file_path[100];
			sprintf(chunk_file_path, "%s%i.%i.chunk", core->run_data->world_chunks_path, chunk_save->skele_chunk.x_index, chunk_save->skele_chunk.y_index);
			FILE *file = fopen(chunk_file_path, "rb");
			if (file)
			{
				ReadFromFile(file, &chunk_save->cells, sizeof(chunk_save->cells));
				
				// NOTE(randy): Read in entities and their components
				{
					ReadFromFile(file, &chunk_save->entity_count, sizeof(i32));
					
					chunk_save->entity_ids = &core->run_data->loaded_positional_entity_ids[core->run_data->loaded_positional_entity_id_count];
					core->run_data->loaded_positional_entity_id_count += chunk_save->entity_count;
					
					for (i32 j = 0; j < chunk_save->entity_count; j++)
					{
						EntitySave entity_save;
						ReadFromFile(file, &entity_save, sizeof(EntitySave));
						core->run_data->loaded_entities[core->run_data->loaded_entity_count++] = entity_save;
						i32 loaded_entity_id = core->run_data->loaded_entity_count;
						
						chunk_save->entity_ids[j] = loaded_entity_id;
					}
					
					DeserialiseComponentsToLoadData(file, &core->run_data->loaded_entity_components,
													core->run_data->loaded_entities,
													chunk_save->entity_ids, chunk_save->entity_count);
				}
				
				fclose(file);
			}
			else
			{
				--
					// NOTE(randy): Chunk doesn't exist yet. Generate it.
					//chunk = GenerateNewChunk(chunks[i].x_index, chunks[i].y_index);
					core->run_data->chunk_generate_queue[core->run_data->chunk_generate_queue_count++] = chunk_save;
					
					for (int y = 0; y < CHUNK_SIZE; y++)
					{
						for (int x = 0; x < CHUNK_SIZE; x++)
						{
							Cell *cell = &chunk_save->cells[y][x];
							i32 x_pos = chunk_save->skele_chunk.x_index * CHUNK_SIZE + x;
							i32 y_pos = chunk_save->skele_chunk.y_index * CHUNK_SIZE + y;
							
							if (y_pos > GetTerrainHeight((f32)x_pos) && y_pos < 500.0f)
							{
								cell->material_type = CELL_MATERIAL_TYPE_dirt;
							}
						}
					}
		 --
			}
		}
		
		Log("Load job completed in %fms", (platform->GetTime() - start_time) * 1000.0f);
	 */
	
	return 1;
}

internal b8 QueueChunkForSave(Chunk *chunk)
{
	/*
		Assert(chunk && chunk->is_valid);
		
		// NOTE(randy): Ensure chunk is not already queued up
		for (i32 i = 0; i < core->run_data->chunk_save_count; i++)
		{
			ChunkSave *chunk_save = &core->run_data->chunk_saves[i];
			if (chunk_save->skele_chunk.x_index == chunk->x_index && chunk_save->skele_chunk.y_index == chunk->y_index)
			{
				return 0;
			}
		}
		
		ChunkSave *chunk_save = &core->run_data->chunk_saves[core->run_data->chunk_save_count++];
		chunk_save->skele_chunk.x_index = chunk->x_index;
		chunk_save->skele_chunk.y_index = chunk->y_index;
		MemoryCopy(&chunk_save->cells, &chunk->cells, sizeof(chunk->cells));
		chunk_save->entity_count = chunk->entity_count;
		
		if (chunk->entity_count)
		{
			i64 offset = chunk->entity_ids - core->run_data->positional_entity_ids;
			chunk_save->entity_ids = core->run_data->positional_entity_ids_snapshot + offset;
		}
		else
		{
			chunk_save->entity_ids = 0;
		}
	 */
	
	return 1;
}

internal void SaveWorld()
{
	Assert(core->run_data->world_path[0]);
	
	// NOTE(randy): Save global level data
	char file_path[200] = "";
	sprintf(file_path, "%slevel_data.save", core->run_data->world_path);
	FILE *file = fopen(file_path, "wb");
	Assert(file);
	
	WriteWorldSaveDataToFile(file, &core->run_data->world);
	
	WriteCharacterDataToFile(file, &core->run_data->character_data);
	
	for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
	{
		Entity *entity = &core->run_data->entities[i];
		WriteEntityToFile(file, entity);
	}
	
	fclose(file);
}


internal void UpdateChunks()
{
	/*
		FillChunkEntities();
		
		// NOTE(randy): Unload any chunks that haven't been marked as 'remain_loaded' by now
		Chunk *chunks_to_unload[MAX_WORLD_CHUNKS];
		i32 chunks_to_unload_count = 0;
		for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
		{
			Chunk *chunk = &core->run_data->active_chunks[i];
			if (chunk->is_valid)
			{
				if (!chunk->remain_loaded)
				{
					Assert(chunks_to_unload_count + 1 < MAX_WORLD_CHUNKS);
					chunks_to_unload[chunks_to_unload_count++] = chunk;
				}
				else
				{
					chunk->remain_loaded = 0;
				}
			}
		}
		
		if (chunks_to_unload_count > 0 && core->run_data->save_job_index == -1 && core->run_data->load_job_index == -1)
		{
			// TODO(randy): Optimise this. Most of it's coming from each chunks' cell copy in QueueChunkForSave. Might need to try streamline the cells themselves, get it a lot smaller.
			//BLOCK_TIMER("initial unload copy",
			// NOTE(randy): Snapshot the current world data
			MemoryCopy(&core->run_data->entities_snapshot, &core->run_data->entities, sizeof(core->run_data->entities));
			MemoryCopy(&core->run_data->entity_count_snapshot, &core->run_data->entity_count, sizeof(core->run_data->entity_count));
			MemoryCopy(&core->run_data->entity_components_snapshot, &core->run_data->entity_components, sizeof(core->run_data->entity_components));
			MemoryCopy(&core->run_data->positional_entity_ids_snapshot, &core->run_data->positional_entity_ids,
					   sizeof(core->run_data->positional_entity_ids));
			MemoryCopy(&core->run_data->positional_entity_id_count_snapshot, &core->run_data->positional_entity_id_count, sizeof(core->run_data->positional_entity_id_count));
			
			// NOTE(randy): Queue up all of the chunks that need to get unloaded.
			for (i32 i = 0; i < chunks_to_unload_count; i++)
			{
				Chunk *chunk = chunks_to_unload[i];
				Assert(QueueChunkForSave(chunk));
				
				DeleteChunk(chunk);
			}
			
			core->run_data->save_job_index = platform->QueueJob(0, SaveQueuedChunks, 0);
			//);
		}
		
		if (!core->run_data->disable_chunk_loaded_based_off_view)
		{
			//BLOCK_TIMER_IF("initial load copy",
			//timer_length > 1,
			SkeletonChunk view_chunks[MAX_WORLD_CHUNKS];
			i32 view_chunk_count = 0;
			GetSkeletonChunksInRegion(view_chunks, &view_chunk_count, GetCameraRegionRect(), 1);
			
			// NOTE(randy): Load in any visible chunks
			if(core->run_data->load_job_index == -1 && core->run_data->save_job_index == -1)
			{
				for (i32 i = 0; i < view_chunk_count; i++)
				{
					Chunk *chunk = GetChunkAtIndex(view_chunks[i].x_index, view_chunks[i].y_index);
					if (!chunk)
					{
						ChunkSave new_chunk = {.skele_chunk = view_chunks[i]};
						core->run_data->chunk_load_queue[core->run_data->chunk_load_queue_count++] = new_chunk;
						
						--
						chunk = LoadChunkFromDisk(core->run_data->world_chunks_path, chunks[i].x_index, chunks[i].y_index);
						if (!chunk)
						{
							chunk = GenerateNewChunk(chunks[i].x_index, chunks[i].y_index);
						}
		 --
					}
				}
				
				if (core->run_data->chunk_load_queue_count > 0)
				{
					core->run_data->load_job_index = platform->QueueJob(0, LoadQueuedChunks, 0);
					
					if (!core->run_data->not_first_time_temp) // lmaoooo
					{
						while (!platform->WaitForJob(core->run_data->load_job_index, TS_WAIT_FOREVER));
						{
							core->run_data->not_first_time_temp = 1;
						}
					}
				}
			}
			
			// NOTE(randy): Keep the visible chunks loaded
			for (i32 i = 0; i < view_chunk_count; i++)
			{
				Chunk *chunk = GetChunkAtIndex(view_chunks[i].x_index, view_chunks[i].y_index);
				if (chunk && chunk->is_valid)
				{
					chunk->remain_loaded = 1;
				}
			}
			//);
		}
		else
		{
			// TODO(randy): Cache the player's previous view region or something nd just keep using that or smth. Temporarily force load all chuks for now.
			for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
			{
				Chunk *chunk = &core->run_data->active_chunks[i];
				if (chunk->is_valid)
				{
					chunk->remain_loaded = 1;
				}
			}
		}
	 */
}

internal Chunk *GetChunkAtIndex(i32 x, i32 y)
{
	for (int i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
			if (x == chunk->x_index &&
				y == chunk->y_index)
			return chunk;
	}
    
	// Chunk isn't loaded.
	return 0;
}

/*
internal f32 GetTerrainHeight(f32 x_pos)
{
	f32 world_height = 300.0f;
	x_pos /= PERLIN_NOISE_LENGTH; // Makes the scale a bit more friendly
    
	i32 octaves = 8;
	f32 frequency = 0.1f;
	f32 amplitude = 1.0f;
	
	f32 max_noise_value = 0.0f;
	f32 noise_amount = 0.0f;
	for (int k = 0; k < octaves; k++)
	{
		noise_amount += GetPerlinNoise(x_pos * frequency, 0.0f) * amplitude;
		max_noise_value += amplitude;
		frequency *= 2.0f;
		amplitude *= 0.5f;
	}
    
	f32 noise = noise_amount / max_noise_value;
	
	return world_height * noise;
}
 */

internal void GetSurroundingChunks(Chunk **chunks, v2 position)
{
	Assert(0); // What's this func for again?
    
	chunks[0] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[1] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x),
								WorldSpaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[2] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[3] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y));
	chunks[4] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x),
								WorldSpaceToChunkIndex(position.y));
	chunks[5] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y));
	chunks[6] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[7] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x),
								WorldSpaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[8] = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldSpaceToChunkIndex(position.y + CHUNK_SIZE));
}

internal void DeleteChunk(Chunk *chunk)
{
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		Entity *entity = &core->run_data->entities[chunk->entity_ids[i] - 1];
		DeleteEntity(entity);
	}
    
	Ts2dTextureCleanUp(&chunk->texture);
	MemorySet(chunk, 0, sizeof(Chunk));
}

internal void TickTimers()
{
	for (i32 i = 0; i < MAX_ACTIVE_TIMERS; i++)
	{
		Timer *timer = &core->run_data->timers[i];
		if (timer->is_allocated &&
			platform->GetTime() >= timer->start_time + timer->duration)
		{
			Assert(timer->complete_callback);
			timer->complete_callback();
			MemorySet(timer, 0, sizeof(Timer));
		}
	}
}

internal Timer *NewTimer(f32 duration, TimerCompleteCallback complete_callback)
{
	for (i32 i = 0; i < MAX_ACTIVE_TIMERS; i++)
	{
		Timer *timer = &core->run_data->timers[i];
		if (!timer->is_allocated)
		{
			timer->is_allocated = 1;
			timer->start_time = platform->GetTime();
			timer->duration = duration;
			timer->complete_callback = complete_callback;
			return timer;
		}
	}
	
	Assert(0);
	return 0;
}