internal void WorldUpdate()
{
	if (core->run_data->save_job_index != -1)
	{
		if (platform->WaitForJob(core->run_data->save_job_index, 0))
		{
			core->run_data->chunk_save_count = 0;
			core->run_data->save_job_index = -1;
		}
	}
	
	if (core->run_data->load_job_index != -1)
	{
		if (platform->WaitForJob(core->run_data->load_job_index, 0))
		{
			//BLOCK_TIMER("End of load copy",
			// NOTE(randy): Create new chunks from the load queue
			for (i32 i = 0; i < core->run_data->chunk_load_queue_count; i++)
			{
				Assert(core->run_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS);
				
				ChunkSave *chunk_save = &core->run_data->chunk_load_queue[i];
				Chunk *chunk = &core->run_data->active_chunks[core->run_data->active_chunk_count++];
				
				chunk->is_valid = 1;
				chunk->remain_loaded = 1;
				chunk->x_index = chunk_save->skele_chunk.x_index;
				chunk->y_index = chunk_save->skele_chunk.y_index;
				chunk->entity_count = 0;
				chunk->entity_ids = 0;
				
				// NOTE(randy): Cell copy.
				MemoryCopy(&chunk->cells, &chunk_save->cells, sizeof(chunk->cells));
				
				QueueChunkForTextureUpdate(chunk);
			}
			
			// NOTE(randy): Create new entities and new components based on all the load data
			{
				i32 entity_id_map[MAX_ENTITIES];
				for (i32 i = 0; i < core->run_data->loaded_entity_count; i++)
				{
					EntitySave *entity_save = &core->run_data->loaded_entities[i];
					Entity *entity = NewEntity(entity_save->name, entity_save->type);
					entity->flags = entity_save->flags;
					
					entity_id_map[i] = entity->entity_id;
				}
				
				DeserialiseComponentsFromMap(entity_id_map, core->run_data->loaded_entity_count);
			}
			
			// NOTE(randy): Chunk generation is on hold until we actually being creating the world map.
			/*
						// NOTE(randy): Generate all the new chunks' entities
						for (i32 i = 0; i < core->run_data->chunk_generate_queue_count; i++)
						{
							ChunkSave *chunk_save = core->run_data->chunk_generate_queue[i];
							Chunk *chunk = GetChunkAtIndex(chunk_save->skele_chunk.x_index, chunk_save->skele_chunk.y_index);
							
							if (chunk->y_index == 0)
							{
								i32 ground_segments_per_chunk = 16;
								f32 segment_width = (f32)CHUNK_SIZE / (f32)ground_segments_per_chunk;
								for (i32 j = 0; j < ground_segments_per_chunk; j++)
								{
									Entity *entity = NewEntity("floor", GENERALISED_ENTITY_TYPE_ground);
									PositionComponent *pos_comp = AddPositionComponent(entity);
									f32 x_pos = (f32)chunk->x_index * (f32)CHUNK_SIZE + segment_width * j;
									f32 y_pos = GetTerrainHeight(x_pos);
									pos_comp->position = v2(x_pos, y_pos);
									
									Line line = {0};
									line.p2.x = segment_width;
									line.p2.y = GetTerrainHeight(x_pos + segment_width) - y_pos;
									
									PhysicsBodyComponent *body_comp = AddPhysicsBodyComponent(entity);
									body_comp->shape.line = line;
									body_comp->shape_type = C2_SHAPE_TYPE_line;
									body_comp->material.static_friction = 0.2f;
									body_comp->material.dynamic_friction = 0.2f;
									body_comp->mass_data.mass = 0.0f;
									body_comp->mass_data.inv_mass = 0.0f;
								}
							}
						}
			 */
			
			// NOTE(randy): Reset load data
			core->run_data->loaded_entity_count = 0;
			ResetComponentSet(&core->run_data->loaded_entity_components);
			core->run_data->loaded_positional_entity_id_count = 0;
			core->run_data->chunk_load_queue_count = 0;
			core->run_data->chunk_generate_queue_count = 0;
			
			core->run_data->load_job_index = -1;
			//);
		}
	}
	
	// NOTE(randy): TEMP
	/*
		PlayerDataComponent *player_dat = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	 */
	
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
	
	StationUpdate(); // TODO(randy): Ordered renderer. This is out of place.
    
	BlueprintUpdate();
	
#ifdef DEVELOPER_TOOLS
	RenderColliders();
#endif
    
	UpdateParticleEmitters();
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
		//world_info.shadow_opacity = 1.0f;
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
	for (int j = 0; j < core->run_data->entity_components.parallax_component_count; j++)
	{
		ParallaxComponent *parallax_comp = &core->run_data->entity_components.parallax_components[j];
		if (parallax_comp->component_id)
		{
			PositionComponent *position_comp = GetPositionComponentFromEntityID(parallax_comp->parent_entity_id);
			Assert(position_comp); // NOTE(randy): Parallax must be attached with a position to update.
            
			// TODO: Need to find a way to centralise the desired_position of the parallax, whilst still maintaining spatial consistency across sprites
			position_comp->position.x = parallax_comp->desired_position.x + -core->camera_position.x * parallax_comp->parallax_amount.x;
			position_comp->position.y = parallax_comp->desired_position.y + (-core->camera_position.y + DEFAULT_CAMERA_OFFSET_Y) * parallax_comp->parallax_amount.y;
            
			// position_comp->position.x = parallax_comp->desired_position.x - (parallax_comp->desired_position.x - player_pos->position.x + core->camera_offset.x) * parallax_comp->parallax_amount.x;
			// position_comp->position.y = parallax_comp->desired_position.y - (parallax_comp->desired_position.y - player_pos->position.y + core->camera_offset.y) * parallax_comp->parallax_amount.y;
		}
	}
}

internal void GenerateTestPlatform()
{
	i32 width = 2000;
	i32 depth = 150;
	f32 height_variation = 20;
	f32 edge_percent = 0.4f;
	
	i32 ground_interval = 16;
	PositionComponent *previous_pos = 0;
	PhysicsBodyComponent *previous_body = 0;
	
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
			Entity *entity = NewEntity("floor", GENERALISED_ENTITY_TYPE_ground);
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2((f32)x_pos, (f32)height);
			
			Line line = {0};
			if (previous_body)
			{
				previous_body->shape.line.p2.x = (f32)ground_interval;
				previous_body->shape.line.p2.y = (f32)height - previous_pos->position.y;
			}
			
			
			PhysicsBodyComponent *body_comp = AddPhysicsBodyComponent(entity);
			body_comp->shape.line = line;
			body_comp->shape_type = C2_SHAPE_TYPE_line;
			body_comp->material.static_friction = 0.2f;
			body_comp->material.dynamic_friction = 0.2f;
			body_comp->mass_data.mass = 0.0f;
			body_comp->mass_data.inv_mass = 0.0f;
			body_comp->type |= PHYSICS_BODY_TYPE_FLAGS_ground;
			body_comp->collide_against |= PHYSICS_BODY_TYPE_FLAGS_character | PHYSICS_BODY_TYPE_FLAGS_item;
			
			previous_body = body_comp;
			previous_pos = pos_comp;
		}
	}
	
	// NOTE(randy): $Spawn structures
	{
		Entity *entity = NewEntity("crafting stump", GENERALISED_ENTITY_TYPE_undefined);
		
		PositionComponent *pos_comp = AddPositionComponent(entity);
		pos_comp->position = v2(-100, 0);
		
		SpriteComponent *sprite_comp = AddSpriteComponent(entity);
		sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_crafting_stump;
		
		InteractableComponent *inter_comp = AddInteractableComponent(entity);
		inter_comp->bounds.aabb.min = c2V(-30.0f, -30.0f);
		inter_comp->bounds.aabb.max = c2V(30.0f, 30.0f);
		inter_comp->bounds_type = C2_SHAPE_TYPE_aabb;
		inter_comp->priority = 1.0f;
		inter_comp->interact_callback = OnCraftingTableInteract;
		
		StationComponent *station_comp = AddStationComponent(entity);
		station_comp->type = STATION_TYPE_crafting;
		
		PhysicsBodyComponent *phys_comp = AddPhysicsBodyComponent(entity);
		c2AABB aabb = {
			.min = c2V(-15.0f, -25.0f),
			.max = c2V(15.0f, 0.0f),
		};
		phys_comp->shape.aabb = aabb;
		phys_comp->shape_type = C2_SHAPE_TYPE_aabb;
		phys_comp->material.restitution = 0.1f;
		phys_comp->material.static_friction = 0.1f;
		phys_comp->material.dynamic_friction = 0.1f;
		phys_comp->gravity_multiplier = 0.0f;
		phys_comp->type |= PHYSICS_BODY_TYPE_FLAGS_station;
		phys_comp->collide_against |= PHYSICS_BODY_TYPE_FLAGS_item;
	}
	
	// NOTE(randy): $Generate background stuff
	{
		for (i32 i = 10; i < 200; i += 10)
		{
			if (RandomI32(0, 3))
			{
				i32 height = height_map[i];
				i32 x_pos = i * 10 - width / 2 + 10;
				
				Entity *entity = NewEntity("Tree", GENERALISED_ENTITY_TYPE_resource);
				
				PositionComponent *pos_comp = AddPositionComponent(entity);
				pos_comp->position = v2((f32)x_pos, (f32)height + 1);
				
				SpriteComponent *sprite_comp = AddSpriteComponent(entity);
				sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_pine_tree_v1;
				sprite_comp->sprite_data.render_layer = 0.5f;
			}
		}
		
		// NOTE(randy): $Hill layer 1
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-458.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-272.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-93.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(93.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(272.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v2;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		{
			Entity *entity = NewEntity("Hills", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(458.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_hills_1_v1;
			sprite_comp->sprite_data.render_layer = 1.0f;
		}
		
		// NOTE(randy): $bg trees 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(80, 150))
		{
			Entity *entity = NewEntity("Bg1 tree", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_TREE_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg1_pine_tree_v1 + RandomI32(0, 1);
			sprite_comp->sprite_data.render_layer = 2.0f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity("vegetation", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_SHRUB_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg1_shrub_v1 + RandomI32(0, 2);
			sprite_comp->sprite_data.render_layer = 1.5f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg saplings 1
		for (i32 x = width / -2 + 500; x < width / 2 - 500; x += RandomI32(30, 90))
		{
			Entity *entity = NewEntity("vegetation", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_SAPLING_1_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg1_sapling_v1 + RandomI32(0, 2);
			sprite_comp->sprite_data.render_layer = 1.75f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $Hill layer 2
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-500, 0.0f);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-300.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-100, 0.0f);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(100, 0.0f);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(300, 0);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		{
			Entity *entity = NewEntity("Hills 2", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(500, 0.0f);
			parallax_comp->parallax_amount = HILLS_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_hills_v1;
			sprite_comp->sprite_data.render_layer = 3.0f;
		}
		
		// NOTE(randy): $bg trees 2
		for (i32 x = width / -2 + 450; x < width / 2 - 450; x += RandomI32(80, 150))
		{
			Entity *entity = NewEntity("Bg2 tree", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_TREE_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_pine_tree_v1 + RandomI32(0, 1);
			sprite_comp->sprite_data.render_layer = 4.0f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 2
		for (i32 x = width / -2 + 450; x < width / 2 - 450; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity("vegetation", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_SHRUB_2_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg2_shrub_v1 + RandomI32(0, 2);
			sprite_comp->sprite_data.render_layer = 3.5f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $Hill layer 3
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-500.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-300.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-100, 0.0f);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(100, 0.0f);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(300, 0);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		{
			Entity *entity = NewEntity("Hills 3", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-500.0f, 0.0f);
			parallax_comp->parallax_amount = HILLS_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_hills_v1;
			sprite_comp->sprite_data.render_layer = 5.0f;
		}
		
		// NOTE(randy): $bg trees 3
		for (i32 x = width / -2 + 500; x < width / 2 - 650; x += RandomI32(30, 50))
		{
			Entity *entity = NewEntity("Bg2 tree", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_TREE_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_pine_tree_v1 + RandomI32(0, 7);
			sprite_comp->sprite_data.render_layer = 6.0f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $bg shrubs 3
		for (i32 x = width / -2 + 500; x < width / 2 - 650; x += RandomI32(40, 120))
		{
			Entity *entity = NewEntity("vegetation", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x, 0.0f);
			parallax_comp->parallax_amount = BG_SHRUB_3_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_bg3_shrub_v1 + RandomI32(0, 3);
			sprite_comp->sprite_data.render_layer = 5.5f;
			sprite_comp->is_flipped = RandomI32(0, 1);
		}
		
		// NOTE(randy): $mid mountains
		for (i32 x = -1; x < 2; x++)
		{
			Entity *entity = NewEntity("mid mountain", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2((f32)x * 500, 0.0f);
			parallax_comp->parallax_amount = MID_MOUNTAINS_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_mid_mountains;
			sprite_comp->sprite_data.render_layer = 7.0f;
		}
		
		// NOTE(randy): $far mountains
		{
			Entity *entity = NewEntity("far mountain", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(-300, 0.0f);
			parallax_comp->parallax_amount = FAR_MOUNTAINS_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_far_mountains;
			sprite_comp->sprite_data.render_layer = 8.0f;
		}
		{
			Entity *entity = NewEntity("far mountain", GENERALISED_ENTITY_TYPE_scenic);
			entity->flags |= ENTITY_FLAGS_force_floating;
			
			PositionComponent *pos_comp = AddPositionComponent(entity);
			pos_comp->position = v2(0, 0);
			
			ParallaxComponent *parallax_comp = AddParallaxComponent(entity);
			parallax_comp->desired_position = v2(300, 0.0f);
			parallax_comp->parallax_amount = FAR_MOUNTAINS_PARALLAX;
			
			SpriteComponent *sprite_comp = AddSpriteComponent(entity);
			sprite_comp->sprite_data.static_sprite = STATIC_SPRITE_far_mountains;
			sprite_comp->sprite_data.render_layer = 8.0f;
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
		Entity *character = NewEntity("Player", GENERALISED_ENTITY_TYPE_character);
		core->run_data->character_entity = character;
		AddPositionComponent(character);
		AddSpriteComponent(character);
		AddArcEntityComponent(character);
		AddMovementComponent(character);
		AddPhysicsBodyComponent(character);
		AddAnimationComponent(character);
		
		character->flags |= ENTITY_FLAGS_no_delete;
		GetPositionComponentFromEntityID(character->entity_id)->position = v2(0.0f, -100.0f);
		
		c2Capsule capsule = {
			.a = c2V(0.0f, -10.0f),
			.b = c2V(0.0f, -50.0f),
			.r = 10.0f,
		};
		
		PhysicsBodyComponent *phys_body_comp = GetPhysicsBodyComponentFromEntityID(character->entity_id);
		phys_body_comp->shape.capsule = capsule;
		phys_body_comp->shape_type = C2_SHAPE_TYPE_capsule;
		
		phys_body_comp->mass_data.mass = 60.0f;
		phys_body_comp->mass_data.inv_mass = 1.0f / phys_body_comp->mass_data.mass;
		phys_body_comp->material.restitution = 0.4f;
		phys_body_comp->material.static_friction = 0.5f;
		phys_body_comp->material.dynamic_friction = 0.5f;
		phys_body_comp->gravity_multiplier = 1.0f;
		phys_body_comp->collide_against |= PHYSICS_BODY_TYPE_FLAGS_ground;
		phys_body_comp->type |= PHYSICS_BODY_TYPE_FLAGS_character;
		
		GetMovementComponentFromEntityID(character->entity_id)->move_speed = 100.0f;
		GetArcEntityComponentFromEntityID(character->entity_id)->entity_type = ARC_ENTITY_TYPE_player;
		GetArcEntityComponentFromEntityID(character->entity_id)->current_animation_state = ARC_ENTITY_ANIMATION_STATE_player_idle;
		SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(character->entity_id);
		sprite_comp->sprite_data.dynamic_sprite = DYNAMIC_SPRITE_player_idle;
		
		PlayerDataComponent *player_comp = AddPlayerDataComponent(character);
		
		Item flint_sword = { .type = ITEM_TYPE_flint_sword, .stack_size = 1 };
		player_comp->inventory[0] = flint_sword;
		
		Item flint = { .type = ITEM_TYPE_flint, .stack_size = 7 };
		player_comp->inventory[1] = flint;
		
		Item twig = { .type = ITEM_TYPE_twig, .stack_size = 14 };
		player_comp->inventory[2] = twig;
		
		Item tool = { .type = ITEM_TYPE_crafting_tool, .stack_size = 1 };
		player_comp->hotbar[0] = tool;
		
		player_comp->inventory_size = 9;
		player_comp->hotbar_size = 2;
	}
	
	FillChunkEntities();
	
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
		
		// TODO: refactor this
		InitialiseRunData();
		// NOTE(randy): Might want to seperate out some run data that is permanent
		// and will not change between saves
	}
    
	// Read in basic data from the world_data.save
	{
		char file_path[200] = "";
		sprintf(file_path, "%slevel_data.save", path);
		FILE *save = fopen(file_path, "rb");
		Assert(save);
		
		// NOTE(randy): Read the player entity in
		{
			EntitySave entity_save;
			ReadFromFile(save, &entity_save, sizeof(EntitySave));
			
			Entity *entity = NewEntity(entity_save.name, entity_save.type);
			entity->flags = entity_save.flags;
			
			i32 component_ids[COMPONENT_MAX];
			ReadFromFile(save, &component_ids, sizeof(component_ids));
			for (i32 i = 1; i < COMPONENT_MAX; i++)
			{
				if (component_ids[i])
					ReadComponentFromFile(save, entity, i);
			}
            
			core->run_data->character_entity = entity;
		}
		
		// NOTE(randy): Read in basic world data
		ReadWorldSaveDataFromFile(save, &core->run_data->world);
		
		// NOTE(randy): Read in floating entities
		ReadFromFile(save, &core->run_data->floating_entity_id_count, sizeof(i32));
		for (i32 i = 0; i < core->run_data->floating_entity_id_count; i++)
		{
			EntitySave entity_save;
			ReadFromFile(save, &entity_save, sizeof(EntitySave));
			
			Entity *entity = NewEntity(entity_save.name, entity_save.type);
			entity->flags = entity_save.flags;
			
			core->run_data->floating_entity_ids[i] = entity->entity_id;
		}
        
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			DeserialiseEntityComponentsFromIDList(save, core->run_data->floating_entity_ids, core->run_data->floating_entity_id_count, i);
		}
        
		fclose(save);
	}
    
	// Load some surrounding chunks in based off of the player's position.
	{
		char file_path[300] = "";
		sprintf(file_path, "%schunks\\", path);
		Assert(platform->DoesDirectoryExist(file_path));
		strcpy(core->run_data->world_chunks_path, file_path);
		
		TransformInGameCamera();
		
		SkeletonChunk chunks[MAX_WORLD_CHUNKS];
		i32 chunk_count;
		GetSkeletonChunksInRegion(chunks, &chunk_count, GetCameraRegionRect(), 1);
		
		for (i32 i = 0; i < chunk_count; i++)
		{
			Chunk *chunk = LoadChunkFromDisk(file_path, chunks[i].x_index, chunks[i].y_index);
			
			if (!chunk)
				LogError("A surrounding chunk in the player's region doesn't exist, is this intended?");
		}
	}
    
	core->is_ingame = 1;
	
	Log("Successfully loaded world \'%s\'", world_name);
	return 1;
}

internal void UnloadWorld()
{
	Assert(core->is_ingame && core->run_data->world_name[0]);
	
	SaveWorld();
	Assert(core->run_data->save_job_index != -1);
	platform->WaitForJob(core->run_data->save_job_index, TS_WAIT_FOREVER);
	
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	core->is_ingame = 0;
}

internal int SaveQueuedChunks(void *job_data)
{
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
	
	return 1;
}

internal int LoadQueuedChunks(void *job_data)
{
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
			/*
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
	 */
		}
	}
	
	Log("Load job completed in %fms", (platform->GetTime() - start_time) * 1000.0f);
	
	return 1;
}

internal b8 QueueChunkForSave(Chunk *chunk)
{
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
	
	return 1;
}

internal void SaveLevelData()
{
	Assert(core->run_data->world_path[0]);
	
	char file_path[200] = "";
	sprintf(file_path, "%slevel_data.save", core->run_data->world_path);
	FILE *file = fopen(file_path, "wb");
	Assert(file);
	
	// NOTE(randy): Save the player entity
	{
		EntitySave entity_save = {.flags = core->run_data->character_entity->flags, .type = core->run_data->character_entity->generalised_type};
		MemoryCopy(entity_save.name, core->run_data->character_entity->name, sizeof(entity_save.name));
		
		WriteToFile(file, &entity_save, sizeof(EntitySave));
		
		WriteToFile(file, &core->run_data->character_entity->component_ids, sizeof(core->run_data->character_entity->component_ids));
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			if (core->run_data->character_entity->component_ids[i])
			{
				WriteComponentToFile(file, core->run_data->character_entity->component_ids[i], i);
			}
		}
	}
	
	// NOTE(randy): Save world data struct
	WriteWorldSaveDataToFile(file, &core->run_data->world);
	
	// NOTE(randy): Save all of the entities that don't belong to chunks.
	{
		WriteToFile(file, &core->run_data->floating_entity_id_count, sizeof(i32));
		for (i32 i = 0; i < core->run_data->floating_entity_id_count; i++)
		{
			Entity *entity = &core->run_data->entities[core->run_data->floating_entity_ids[i] - 1];
			
			EntitySave entity_save = {.flags = entity->flags, .type = entity->generalised_type};
			MemoryCopy(entity_save.name, entity->name, sizeof(entity_save.name));
			
			WriteToFile(file, &entity_save, sizeof(EntitySave));
		}
        
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			SerialiseEntityComponentsFromIDList(file, core->run_data->entities, &core->run_data->entity_components, core->run_data->floating_entity_ids, core->run_data->floating_entity_id_count, i);
		}
	}
    
	fclose(file);
	Log("Level data saved to %s", file_path);
}

internal void SaveWorld()
{
	if (core->run_data->chunk_save_count > 0 || core->run_data->save_job_index != -1)
	{
		LogWarning("A save is already in progress.");
		return;
	}
	
	SaveLevelData();
	
	// NOTE(randy): Capture a snapshot of the world's data
	{
		MemoryCopy(&core->run_data->entities_snapshot, &core->run_data->entities, sizeof(core->run_data->entities));
		MemoryCopy(&core->run_data->entity_count_snapshot, &core->run_data->entity_count, sizeof(core->run_data->entity_count));
		MemoryCopy(&core->run_data->entity_components_snapshot, &core->run_data->entity_components, sizeof(core->run_data->entity_components));
		MemoryCopy(&core->run_data->positional_entity_ids_snapshot, &core->run_data->positional_entity_ids,
				   sizeof(core->run_data->positional_entity_ids));
		MemoryCopy(&core->run_data->positional_entity_id_count_snapshot, &core->run_data->positional_entity_id_count, sizeof(core->run_data->positional_entity_id_count));
		
		for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
		{
			Chunk *chunk = &core->run_data->active_chunks[i];
			
			if (chunk->is_valid)
			{
				Assert(QueueChunkForSave(chunk));
			}
		}
	}
	
	core->run_data->save_job_index = platform->QueueJob(0, SaveQueuedChunks, 0);
	
	// TODO(randy): Try doing a malloc and free for the snapshop data to see if it's more performant and we won't need to store scratch in RunData
}

internal void FillChunkEntities()
{
	// NOTE(randy): Reset counts
	core->run_data->floating_entity_id_count = 0;
	core->run_data->positional_entity_id_count = 0;
	MemorySet(core->run_data->positional_entity_ids, 0, sizeof(core->run_data->positional_entity_ids));
	MemorySet(core->run_data->floating_entity_ids, 0, sizeof(core->run_data->floating_entity_ids));
	for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		chunk->entity_count = 0;
		chunk->entity_ids = 0;
	}
	
	// NOTE(randy): Find all positional entities currently within the world.
	Entity *positional_entities[MAX_POSITIONAL_ENTITIES];
	i32 positional_entity_count = 0;
	for (i32 i = 0; i < core->run_data->entity_count; i++)
	{
		Entity *entity = &core->run_data->entities[i];
		if (entity->entity_id)
		{
			if (entity == core->run_data->character_entity)
				continue;
			
			v2 position = {0.0f, 0.0f};
			if (entity->component_ids[COMPONENT_position] && ((entity->flags & ENTITY_FLAGS_force_floating) != ENTITY_FLAGS_force_floating))
			{
				position = GetPositionComponentFromEntityID(entity->entity_id)->position;
				if (entity->component_ids[COMPONENT_parallax])
				{
					position = GetParallaxComponentFromEntityID(entity->entity_id)->desired_position;
				}
				Chunk *chunk = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x),
											   WorldSpaceToChunkIndex(position.y));
				if (!chunk)
				{
					LogError("Item found outside of chunk.");
					continue;
				}
				
				chunk->entity_count++;
				positional_entities[positional_entity_count++] = entity;
			}
			else
			{
				Assert(core->run_data->floating_entity_id_count + 1 < MAX_FLOATING_ENTITIES);
				core->run_data->floating_entity_ids[core->run_data->floating_entity_id_count++] = entity->entity_id;
			}
		}
	}
	
	// NOTE(randy): Allocate each chunk's local array
	i32 id_count = 0;
	for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid && chunk->entity_count)
		{
			chunk->entity_ids = &core->run_data->positional_entity_ids[id_count];
			id_count += chunk->entity_count;
			core->run_data->positional_entity_id_count += chunk->entity_count;
			chunk->entity_count = 0;
		}
	}
	
	// NOTE(randy): Put all positional entities into their desired chunk arrays.
	for (i32 i = 0; i < positional_entity_count; i++)
	{
		Entity *entity = positional_entities[i];
		
		v2 position = GetPositionComponentFromEntityID(entity->entity_id)->position;
		if (entity->component_ids[COMPONENT_parallax])
		{
			position = GetParallaxComponentFromEntityID(entity->entity_id)->desired_position;
		}
		Chunk *chunk = GetChunkAtIndex(WorldSpaceToChunkIndex(position.x),
									   WorldSpaceToChunkIndex(position.y));
		Assert(chunk);
		
		chunk->entity_ids[chunk->entity_count++] = entity->entity_id;
	}
}

internal void UpdateChunks()
{
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
					
					/*
					chunk = LoadChunkFromDisk(core->run_data->world_chunks_path, chunks[i].x_index, chunks[i].y_index);
					if (!chunk)
					{
						chunk = GenerateNewChunk(chunks[i].x_index, chunks[i].y_index);
					}
	 */
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

// TODO(randy): Remove
internal Chunk *LoadChunkFromDisk(char *path, i32 x_index, i32 y_index)
{
	Assert(core->run_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS);
	Assert(!GetChunkAtIndex(x_index, y_index));
	
	char chunk_file_path[100];
	sprintf(chunk_file_path, "%s%i.%i.chunk", path, x_index, y_index);
	FILE *file = fopen(chunk_file_path, "rb");
	if (file)
	{
		Chunk *chunk = &core->run_data->active_chunks[core->run_data->active_chunk_count++];
		chunk->is_valid = 1;
		chunk->remain_loaded = 1;
		chunk->x_index = x_index;
		chunk->y_index = y_index;
		
		// Read in cells
		{
			ReadFromFile(file, &chunk->cells, sizeof(Cell) * CHUNK_SIZE * CHUNK_SIZE);
			QueueChunkForTextureUpdate(chunk);
		}
		
        
		// Read in entities
		{
			ReadFromFile(file, &chunk->entity_count, sizeof(i32));
			
			// Allocate a segment in the id array for it.
			chunk->entity_ids = &core->run_data->positional_entity_ids[core->run_data->positional_entity_id_count];
			core->run_data->positional_entity_id_count += chunk->entity_count;
			
			for (i32 i = 0; i < chunk->entity_count; i++)
			{
				EntitySave entity_save;
				ReadFromFile(file, &entity_save, sizeof(EntitySave));
				
				Entity *entity = NewEntity(entity_save.name, entity_save.type);
				entity->flags = entity_save.flags;
				
				chunk->entity_ids[i] = entity->entity_id;
			}
            
			for (i32 i = 1; i < COMPONENT_MAX; i++)
			{
				DeserialiseEntityComponentsFromIDList(file, chunk->entity_ids, chunk->entity_count, i);
			}
		}
        
		fclose(file);
		Log("Chunk %i.%i successfully loaded from disk", chunk->x_index, chunk->y_index);
		return chunk;
	}
	else
	{
		return 0;
	}
}