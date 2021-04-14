internal void WorldUpdate()
{
	TickTimers();
    
	core->performance_timer_count = 0;
    
	START_PERF_TIMER("Update");
	
	{
		ElementalSkillTreeUpdate();
		
		PreMoveUpdatePlayer();
        
		UpdatePhysics();
        
		if (!core->run_data->editor_state)
			TransformInGameCamera();
        
		PostMoveUpdatePlayer();
	}
    
	UpdateParallax();
    
	InteractableUpdate();
	
	DrawWorld();
	
#ifdef DEVELOPER_TOOLS
	RenderColliders();
#endif
    
	DrawGameUI();
	
	END_PERF_TIMER;
}

internal void DrawWorld()
{
	Ts2dPushBackgroundBegin();
	{
		//Nighttime
		/*v4 top_sky_colour = v4(13.0f / 255.0f, 25.0f / 255.0f, 44.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(51.0f / 255.0f, 67.0f / 255.0f, 92.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(172.0f / 255.0f, 171.0f / 255.0f, 170.0f / 255.0f, 1.0f);*/
		
		//Sunrise
		/*v4 top_sky_colour = v4(176.0f / 255.0f, 208.0f / 255.0f, 223.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(232.0f / 255.0f, 216.0f / 255.0f, 217.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(172.0f / 255.0f, 102.0f / 255.0f, 96.0f / 255.0f, 1.0f);*/
		
		//Daytime (original)
		v4 top_sky_colour = v4(88.0f / 255.0f, 152.0f / 255.0f, 194.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(203.0f / 255.0f, 232.0f / 255.0f, 238.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(130.0f / 255.0f, 81.0f / 255.0f, 77.0f / 255.0f, 1.0f);
		
		//Sunset
		/*v4 top_sky_colour = v4(66.0f / 255.0f, 41.0f / 255.0f, 95.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(192.0f / 255.0f, 116.0f / 255.0f, 109.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(246.0f / 255.0f, 95.0f / 255.0f, 56.0f / 255.0f, 1.0f);*/
        
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
    
	// NOTE(randy): Pass visible ground vertices to renderer
	MemorySet(global_ts2d->ground_vertices, 0, sizeof(global_ts2d->ground_vertices));
	global_ts2d->ground_vertex_count = 0;
	v4 camera_region = GetCameraRegionRect();
	const float x_buffer = 50.0f;
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_ground_segment);)
	{
		if (global_ts2d->ground_vertex_count == MAX_GROUND_VERTICES)
			break;
		
		LineSegments *line_segs = &entity->physics.shape.line_segments;
		for (i32 i = 0; i < line_segs->count; i++)
		{
			v2 vert_pos = V2AddV2(line_segs->vertices[i], entity->position);
			if (vert_pos.x > camera_region.x - x_buffer && vert_pos.x < camera_region.x + camera_region.z + x_buffer)
			{
				global_ts2d->ground_vertices[global_ts2d->ground_vertex_count] = vert_pos;
				global_ts2d->ground_vertices[global_ts2d->ground_vertex_count].y *= -1.0f;
				global_ts2d->ground_vertex_count++;
				
				if (global_ts2d->ground_vertex_count == MAX_GROUND_VERTICES)
					break;
			}
		}
	}
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
	
	core->is_ingame = 1;
	
	Log("Created new world '%s' successfully.", world_name);
	
	// NOTE(randy): Temp
	Entity *entity = NewEntity();
	entity->position = v2(-128.0f, 0.0f);
	GroundSegmentEntityPresetCallback(entity);
	
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
	
	// NOTE(randy): Read in player & world data
	{
		char file_path[200] = "";
		sprintf(file_path, "%sfloating_data.arc", path);
		FILE *file = fopen(file_path, "rb");
		Assert(file);
		
		ReadWorldSaveDataFromFile(file, &core->run_data->world);
		ReadCharacterDataFromFile(file, &core->run_data->character_data);
		fclose(file);
		file = 0;
	}
	
	// NOTE(randy): Read in chunk data
	/*
		{
			char chunk_file_path[200] = "";
			sprintf(chunk_file_path, "%s\\chunks\\temp_chunk.arc", core->run_data->world_path);
			FILE *file = fopen(chunk_file_path, "rb");
			Assert(file);
			
			for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
			{
				Entity entity = {0};
				ReadEntityFromFile(file, &entity);
				
				if (EntityHasProperty(&entity, ENTITY_PROPERTY_is_allocated))
				{
					core->run_data->entities[i] = entity;
					
					if (EntityHasProperty(&entity, ENTITY_PROPERTY_is_character))
					{
						core->run_data->character_entity = &core->run_data->entities[i];
					}
				}
			}
			
			fclose(file);
		}
	 */
	
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

internal void SaveWorld()
{
	Assert(core->run_data->world_path[0]);
	
	// NOTE(randy): Save floating data
	char file_path[200] = "";
	sprintf(file_path, "%sfloating_data.arc", core->run_data->world_path);
	FILE *file = fopen(file_path, "wb");
	Assert(file);
	WriteWorldSaveDataToFile(file, &core->run_data->world);
	WriteCharacterDataToFile(file, &core->run_data->character_data);
	fclose(file);
	file = 0;
	
	// NOTE(randy): Save all entities to chunks
	// TODO(randy): Actually put them in chunks, not just one big chunk
	char chunk_file_path[200] = "";
	sprintf(chunk_file_path, "%s\\chunks\\temp_chunk.arc", core->run_data->world_path);
	file = fopen(chunk_file_path, "wb");
	Assert(file);
	
	// NOTE(randy): When chunk loading is a thing, need to separate out floating entities from positional ones
	for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
	{
		Entity *entity = &core->run_data->entities[i];
		WriteEntityToFile(file, entity);
	}
	
	fclose(file);
}

internal Chunk *GetUnallocatedChunk()
{
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		if (!(GetRunData()->chunks[i].flags & CHUNK_FLAGS_is_allocated))
		{
			return &GetRunData()->chunks[i];
		}
	}
	
	return 0;
}

internal void SaveChunkToFile(FILE *file, Chunk *chunk)
{
	i32 version = CHUNK_VERSION;
	WriteToFile(file, &version, sizeof(version));
	
	WriteToFile(file, &chunk->flags, sizeof(chunk->flags));
	WriteToFile(file, &chunk->pos, sizeof(chunk->pos));
	
	WriteToFile(file, &chunk->entity_count, sizeof(chunk->entity_count));
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		Entity *entity = chunk->entities[i];
		WriteEntityToFile(file, entity);
	}
	WriteToFile(file, chunk->terrain_verts, sizeof(chunk->terrain_verts));
}

internal void ReadChunkFromFile(FILE *file, Chunk *chunk)
{
	i32 version;
	ReadFromFile(file, &version, sizeof(version));
	Assert(version == CHUNK_VERSION);
	
	ReadFromFile(file, &chunk->flags, sizeof(chunk->flags));
	ReadFromFile(file, &chunk->pos, sizeof(chunk->pos));
	
	ReadFromFile(file, &chunk->entity_count, sizeof(chunk->entity_count));
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		Entity *entity = NewEntity();
		ReadEntityFromFile(file, entity);
	}
	
	ReadFromFile(file, chunk->terrain_verts, sizeof(chunk->terrain_verts));
}

internal Chunk *LoadWorldChunk(iv2 pos)
{
	if (GetChunkAtPos(pos))
	{
		return GetChunkAtPos(pos);
	}
	
	char chunk_path[100];
	sprintf(chunk_path, "%s%i.%i.chunk", GetRunData()->world_chunks_path, pos.x, pos.y);
	FILE *file = fopen(chunk_path, "rb");
	if (file)
	{
		// NOTE(randy): Read in world chunk from save
		Chunk *chunk = GetUnallocatedChunk();
		Assert(chunk);
		ReadChunkFromFile(file, chunk);
		fclose(file);
		
		return chunk;
	}
	else
	{
		// NOTE(randy): Chunk doesn't exist in save, so we try loading from map
		Chunk *chunk = LoadMapChunk(pos);
		if (chunk)
		{
			return chunk;
		}
		else
		{
			chunk = GetUnallocatedChunk();
			chunk->flags |= CHUNK_FLAGS_is_allocated;
			chunk->pos = pos;
		}
	}
}

internal Chunk *UnloadWorldChunk(iv2 pos)
{
	Chunk *chunk = GetChunkAtPos(pos);
	if (!chunk)
	{
		LogWarning("There is no chunk to unload at %i, %i", pos.x, pos.y);
		return 0;
	}
	
	char chunk_path[100];
	sprintf(chunk_path, "%s%i.%i.chunk", GetRunData()->world_chunks_path, pos.x, pos.y);
	FILE *file = fopen(chunk_path, "wb");
	Assert(file);
	SaveChunkToFile(file, chunk);
	fclose(file);
	
	DeleteChunk(chunk);
}

internal Chunk *GetChunkAtPos(iv2 pos)
{
	for (int i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &core->run_data->chunks[i];
		if ((chunk->flags & CHUNK_FLAGS_is_allocated) && pos.x == chunk->pos.x && pos.y == chunk->pos.y)
		{
			return chunk;
		}
	}
    
	return 0;
}

internal void DeleteChunk(Chunk *chunk)
{
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		DeleteEntity(chunk->entities[i]);
	}
    
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
