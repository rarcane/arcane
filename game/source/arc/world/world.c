internal void InitialiseWorldData()
{
	core->run_data->save_job_index = -1;
	core->run_data->free_entity_id = 1;
	InitialiseComponents();
}

internal void CreateTestLevel()
{
	Assert(!core->run_data->world_name[0]); // NOTE(randy): A level is already loaded in.
    
	InitialiseRunData();
	InitialiseWorldData();
	core->is_ingame = 1;
    
	core->run_data->world.test_ptr = &core->run_data->free_entity_id;
    
	{
		// scuffed lmao
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
        
		GetMovementComponentFromEntityID(character->entity_id)->move_speed = 100.0f;
		GetArcEntityComponentFromEntityID(character->entity_id)->entity_type = ARC_ENTITY_TYPE_player;
		GetArcEntityComponentFromEntityID(character->entity_id)->current_animation_state = ARC_ENTITY_ANIMATION_STATE_player_idle;
		SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(character->entity_id);
		sprite_comp->sprite_data.dynamic_sprite = DYNAMIC_SPRITE_player_idle;
	}
    
	/* {
		core->sword = NewEntity("Sword", GENERALISED_ENTITY_TYPE_item);
		core->sword->flags |= ENTITY_FLAGS_no_delete;
		ItemComponent *sword_item = AddItemComponent(core->sword);
		sword_item->item_type = ITEM_TYPE_flint_sword;
		sword_item->stack_size = 1;

		core->backpack = NewEntity("Backpack", GENERALISED_ENTITY_TYPE_storage);
		core->backpack->flags |= ENTITY_FLAGS_no_delete;
		StorageComponent *backpack_storage = AddStorageComponent(core->backpack);
		backpack_storage->storage_size = 9;

		core->hotbar = NewEntity("Hotbar", GENERALISED_ENTITY_TYPE_storage); // Hotbar should technically be attached to the player entity
		core->hotbar->flags |= ENTITY_FLAGS_no_delete;
		StorageComponent *hotbar_storage = AddStorageComponent(core->hotbar);
		hotbar_storage->storage_size = 2;

		AddItemToStorage(sword_item, backpack_storage, 2);
	} */
    
	CreateWorld("testing");
}

internal void WorldUpdate()
{
	if (core->run_data->save_job_index != -1)
	{
		if (platform->WaitForJob(core->run_data->save_job_index, 0))
		{
			core->run_data->chunk_save_count = 0;
			core->run_data->save_job_index = -1;
			Log("Queued chunk save complete");
		}
	}
	
#ifdef DEVELOPER_TOOLS
	DrawEditorUI();
	if (core->run_data->editor_state)
		TransformEditorCamera();
#endif
    
	core->performance_timer_count = 0;
    
	START_PERF_TIMER("Update");
    
	// NOTE(randy): Perform if the game is not paused.
	if ((core->world_delta_t == 0.0f ? (core->run_data->editor_flags & EDITOR_FLAGS_manual_step) : 1))
	{
		UpdateCells();
		
		PreMoveUpdatePlayer();
        
		UpdatePhysics();
        
		if (!core->run_data->editor_state)
			TransformInGameCamera();
        
		PostMoveUpdatePlayer();
        
		core->run_data->editor_flags &= ~EDITOR_FLAGS_manual_step;
	}
    
	UpdateParallax();
    
	UpdateChunks();
    
	DrawWorld();
	RenderCells();
    
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
	if (!(core->run_data->editor_flags & EDITOR_FLAGS_draw_world))
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
        
		RenderBackgroundSprites();
	}
	Ts2dPushBackgroundEnd();
    
	Ts2dWorldInfo world_info = {0};
	{
		world_info.flags |= TS2D_BLUR_SHADOWS;
		world_info.flags |= TS2D_BLUR_BACKGROUNDS;
		if (core->client_data->bloom)
			world_info.flags |= TS2D_BLOOM;
		//world_info.shadow_opacity = core->shadow_opacity;
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
		RenderForegroundSprites();
	}
    
	/* v2 pos = v2view(v2(-350.0f, 35.0f));
                v2 size = v2zoom(v2(700.0f, 200.0f));
                Ts2dPushReflectiveRect(
                core->renderer,
                v4(pos.x, pos.y, size.x, size.y),
                v4(1.0f, 1.0f, 1.0f, 1.0f),
                0.0f, 0.75f); */
    
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

internal void UpdateChunks()
{
	// Reset counts
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
    
	// NOTE(randy):  Decide where each entity belongs
	{
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
				if (entity->component_ids[COMPONENT_position])
				{
					position = GetPositionComponentFromEntityID(entity->entity_id)->position;
					if (entity->component_ids[COMPONENT_parallax])
					{
						position = GetParallaxComponentFromEntityID(entity->entity_id)->desired_position;
					}
					Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex(position.x), WorldspaceToChunkIndex(position.y));
                    
					chunk->entity_count++;
					positional_entities[positional_entity_count++] = entity;
				}
				else
				{
					core->run_data->floating_entity_ids[core->run_data->floating_entity_id_count++] = entity->entity_id;
				}
			}
		}
        
		// NOTE(randy): Set each chunk's local array
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
        
		// NOTE(randy): Put all positional entities into their desired chunk arrays
		for (i32 i = 0; i < positional_entity_count; i++)
		{
			Entity *entity = positional_entities[i];
            
			v2 position = GetPositionComponentFromEntityID(entity->entity_id)->position;
			if (entity->component_ids[COMPONENT_parallax])
			{
				position = GetParallaxComponentFromEntityID(entity->entity_id)->desired_position;
			}
			Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex(position.x), WorldspaceToChunkIndex(position.y));
            
			chunk->entity_ids[chunk->entity_count++] = entity->entity_id;
		}
	}
    
	// NOTE(randy): Unload any chunks that haven't been marked as 'remain_loaded' by now
#define MAX_CHUNK_UNLOAD_COUNT 8
	Chunk *chunks_to_unload[MAX_CHUNK_UNLOAD_COUNT];
	i32 chunks_to_unload_count = 0;
	for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
		{
			if (!chunk->remain_loaded)
			{
				Assert(chunks_to_unload_count + 1 < MAX_CHUNK_UNLOAD_COUNT);
				chunks_to_unload[chunks_to_unload_count++] = chunk;
			}
			else
			{
				chunk->remain_loaded = 0;
			}
		}
	}
	
	if (chunks_to_unload_count > 0 && core->run_data->save_job_index == -1)
	{
		//BLOCK_TIMER("Unload copy",
		// NOTE(randy): Snapshot the current world data
		MemoryCopy(&core->run_data->entities_snapshot, &core->run_data->entities, sizeof(core->run_data->entities));
		MemoryCopy(&core->run_data->entity_count_snapshot, &core->run_data->entity_count, sizeof(core->run_data->entity_count));
		MemoryCopy(&core->run_data->entity_components_snapshot, &core->run_data->entity_components, sizeof(core->run_data->entity_components));
		MemoryCopy(&core->run_data->positional_entity_ids_snapshot, &core->run_data->positional_entity_ids,
				   sizeof(core->run_data->positional_entity_ids));
		MemoryCopy(&core->run_data->positional_entity_id_count_snapshot, &core->run_data->positional_entity_id_count, sizeof(core->run_data->positional_entity_id_count));
		
		for (i32 i = 0; i < chunks_to_unload_count; i++)
		{
			Chunk *chunk = chunks_to_unload[i];
			Assert(QueueChunkForSave(chunk));
			
			DeleteChunk(chunk);
		}
		
		core->run_data->save_job_index = platform->QueueJob(0, SaveQueuedChunks, 0);
		//);
	}
	
	// NOTE(randy): Load in chunks that should be visible to the player + a buffer
	if (!core->run_data->disable_chunk_loaded_based_off_view)
	{
		SkeletonChunk chunks[MAX_WORLD_CHUNKS];
		i32 chunk_count;
		GetSkeletonChunksInRegion(chunks, &chunk_count, GetCameraRegionRect(), 1);
		for (i32 i = 0; i < chunk_count; i++)
		{
			Chunk *chunk = GetChunkAtIndex(chunks[i].x_index, chunks[i].y_index);
			if (!chunk)
			{
				chunk = LoadChunkFromDisk(core->run_data->world_chunks_path, chunks[i].x_index, chunks[i].y_index);
				if (!chunk)
				{
					chunk = GenerateNewChunk(chunks[i].x_index, chunks[i].y_index);
				}
			}
            
			chunk->remain_loaded = 1;
		}
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
    
	/* 	f32 last_height = 0.0f;
	for (int x = 0; x < CHUNK_SIZE * 4; x++)
	{
		i32 octaves = 4;
		f32 frequency = 0.5f;
		f32 amplitude = 1.0f;
		f32 max_value = 0.0f;

		f32 noise_amount = 0.0f;
		for (int k = 0; k < octaves; k++)
		{
			noise_amount += GetPerlinNoise(((f32)x / (f32)CHUNK_SIZE) * frequency, 0.0f) * amplitude;
			max_value += amplitude;
			frequency *= 2.0f;
			amplitude *= 0.5f;
		}

		f32 noise = noise_amount / max_value;

		i32 terrain_height = (i32)floorf(200.0f + 50.0f * noise);
		for (int y = -1; y > -terrain_height - 1; y--)
		{
			i32 x_pos = x - CHUNK_SIZE * 2;
			i32 y_pos = y + 200;

			Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex((f32)x_pos), WorldspaceToChunkIndex((f32)y_pos));
			if (!chunk)
				chunk = GenerateNewChunk(WorldspaceToChunkIndex((f32)x_pos), WorldspaceToChunkIndex((f32)y_pos));

			Cell *cell = GetCellAtPosition(x_pos, y_pos);
			cell->material_type = CELL_MATERIAL_TYPE_dirt;

			QueueChunkForTextureUpdate(chunk);
		}

		f32 width = ((f32)CHUNK_SIZE * 4.0f) / 32.0f;
	} */
}

internal void GetSurroundingChunks(Chunk **chunks, v2 position)
{
	Assert(0); // What's this func for again?
    
	chunks[0] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[1] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[2] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[3] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y));
	chunks[4] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y));
	chunks[5] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y));
	chunks[6] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[7] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[8] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
}

internal Chunk *GenerateNewChunk(i32 x_index, i32 y_index)
{
	Assert(core->run_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS);
	Assert(!GetChunkAtIndex(x_index, y_index));
    
	// TODO: This will shit the bed eventually. Might need to give chunks an ID/free system
	i32 chunk_index = -1;
	for (i32 i = 0; i < core->run_data->active_chunk_count + 1; i++)
	{
		if (!core->run_data->active_chunks[i].is_valid)
		{
			chunk_index = i;
			break;
		}
	}
	if (chunk_index == core->run_data->active_chunk_count)
		core->run_data->active_chunk_count++;
	Assert(chunk_index != -1);
    
	Chunk *chunk = &core->run_data->active_chunks[chunk_index];
	chunk->is_valid = 1;
	chunk->remain_loaded = 1;
	chunk->x_index = x_index;
	chunk->y_index = y_index;
    
	/* if (x == 0)
		last_height = (f32)terrain_height;
	else if (((x + 1) % (i32)width) == 0)
	{
		Entity *ground = NewEntity("Ground Seg", GENERALISED_ENTITY_TYPE_ground);
		AddPositionComponent(ground);
		AddPhysicsBodyComponent(ground);
		GetPositionComponentFromEntityID(ground->entity_id)->position = v2(floorf(x / width) * width - CHUNK_SIZE * 2, 0.0f);

		Line line = {0};
		line.p1.y = 200.0f - (f32)last_height;
		line.p2.x = width;
		line.p2.y = 200.0f - (f32)terrain_height;

		PhysicsBodyComponent *phys_body_comp = GetPhysicsBodyComponentFromEntityID(ground->entity_id);
		phys_body_comp->shape.line = line;
		phys_body_comp->shape_type = C2_SHAPE_TYPE_line;
		phys_body_comp->material.static_friction = 0.2f;
		phys_body_comp->material.dynamic_friction = 0.2f;
		phys_body_comp->mass_data.mass = 0.0f;
		phys_body_comp->mass_data.inv_mass = 0.0f;

		last_height = (f32)terrain_height;
	} */
    
	if (y_index == 0)
	{
		i32 ground_segments_per_chunk = 16;
		f32 segment_width = (f32)CHUNK_SIZE / (f32)ground_segments_per_chunk;
		for (i32 i = 0; i < ground_segments_per_chunk; i++)
		{
			Entity *entity = NewEntity("floor", GENERALISED_ENTITY_TYPE_ground);
			PositionComponent *pos_comp = AddPositionComponent(entity);
			f32 x_pos = (f32)x_index * (f32)CHUNK_SIZE + segment_width * i;
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
    
	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			Cell *cell = &chunk->cells[y][x];
			i32 x_pos = x_index * CHUNK_SIZE + x;
			i32 y_pos = y_index * CHUNK_SIZE + y;
			//cell->x_position = x_index * CHUNK_SIZE + x;
			//cell->y_position = y_index * CHUNK_SIZE + y;
			//cell->parent_chunk = chunk;
            
			if (y_pos > GetTerrainHeight((f32)x_pos) && y_pos < 500.0f)
			{
				cell->material_type = CELL_MATERIAL_TYPE_dirt;
			}
		}
	}
    
	QueueChunkForTextureUpdate(chunk);
    
	return chunk;
}

internal void DeleteChunk(Chunk *chunk)
{
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		DeleteEntity(&core->run_data->entities[chunk->entity_ids[i] - 1]);
	}
    
	Ts2dTextureCleanUp(&chunk->texture);
	MemorySet(chunk, 0, sizeof(Chunk));
}

internal Chunk *LoadChunkFromDisk(char *path, i32 x_index, i32 y_index)
{
	Assert(core->run_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS);
	Assert(!GetChunkAtIndex(x_index, y_index));
    
	char chunk_file_path[100];
	sprintf(chunk_file_path, "%s%i.%i.chunk", path, x_index, y_index);
	FILE *file = fopen(chunk_file_path, "r");
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

internal void UnloadChunk(Chunk *chunk)
{
	Assert(0);
	Assert(chunk && chunk->is_valid);
    
	//BLOCK_TIMER("save",
	//SaveChunkToDisk(core->run_data->world_chunks_path, chunk);
	//);
	
	for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
	{
		if (chunk == &core->run_data->active_chunks[i])
		{
			//BLOCK_TIMER("delete",
			DeleteChunk(chunk);
			//);
			return;
		}
	}
}