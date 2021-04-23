internal void InitialiseWorldData()
{
	WorldData *world_data = GetWorldData();
	world_data->character_chunk = iv2(INT_MAX, INT_MAX);
}

internal void WorldUpdate()
{
	TickTimers();
    
	core->performance_timer_count = 0;
    
	START_PERF_TIMER("Update");
	
	{
		ElementalSkillTreeUpdate();
		
		PreMoveUpdatePlayer();
        
		UpdatePhysics();
        
		if (GetRunData()->is_ejected)
		{
			TransformEditorCamera();
			UpdateEjectedMode();
		}
		else
		{
			TransformInGameCamera();
		}
        
		PostMoveUpdatePlayer();
	}
    
	UpdateParallax();
    
	InteractableUpdate();
	
	UpdateWorldChunks();
	GenerateTerrainSegments();
	
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
	if (!(GetRunData()->debug_flags & DEBUG_FLAGS_disable_draw_terrain))
	{
		v4 camera_region = GetCameraRegionRect();
		const float x_buffer = 50.0f;
		for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_terrain_segment);)
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
		
		for (i32 step = 0; step < global_ts2d->ground_vertex_count - 1; step++)
			for (i32 i = 0; i < global_ts2d->ground_vertex_count - step - 1; i++)
		{
			if (global_ts2d->ground_vertices[i].x > global_ts2d->ground_vertices[i + 1].x)
			{
				v2 temp = global_ts2d->ground_vertices[i];
				global_ts2d->ground_vertices[i] = global_ts2d->ground_vertices[i + 1];
				global_ts2d->ground_vertices[i + 1] = temp;
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

internal b8 DoesWorldExist(char *world_name)
{
	char path[300] = "";
	sprintf(path, "%sworlds\\%s\\", core->res_path, world_name);
	return platform->DoesDirectoryExist(path);
}

internal b8 CreateWorld(char *world_name)
{
	Assert(world_name[0] && strlen(world_name) < sizeof(GetRunData()->world_name));
	
	// NOTE(randy): Initialise run & world data
	{
		strcpy(GetRunData()->world_name, world_name);
		
		char world_path[300] = "";
		sprintf(world_path, "%sworlds\\%s\\", core->res_path, world_name);
		strcpy(GetRunData()->world_path, world_path);
		
		char chunks_path[300] = "";
		sprintf(chunks_path, "%schunks\\", world_path);
		strcpy(GetRunData()->world_chunks_path, chunks_path);
		
		InitialiseRunData();
		InitialiseWorldData();
	}
	
	// NOTE(randy): Create world directories
	char path[300] = "";
	sprintf(path, "%sworlds\\", core->res_path);
	if (!platform->DoesDirectoryExist(path))
		platform->MakeDirectory(path);
	
	if (!platform->DoesDirectoryExist(GetRunData()->world_path))
		platform->MakeDirectory(GetRunData()->world_path);
	
	if (!platform->DoesDirectoryExist(GetRunData()->world_chunks_path))
		platform->MakeDirectory(GetRunData()->world_chunks_path);
	
	// NOTE(randy): Initialise the player
	{
		// TODO(randy): clean up
		Entity *character = NewEntity();
		core->run_data->character_entity = character;
		
		char nm[100] = "player";
		strncpy(character->debug_name, nm, sizeof(nm));
		
		EntitySetProperty(character, ENTITY_PROPERTY_sprite);
		EntitySetProperty(character, ENTITY_PROPERTY_flipbook);
		EntitySetProperty(character, ENTITY_PROPERTY_physical);
		EntitySetProperty(character, ENTITY_PROPERTY_positional);
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
		
		
		CharacterData *character_data = GetCharacterData();
		
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
	
	LoadWorldChunksInView();
	
	Log("Created new world '%s' successfully.", world_name);
	
	return 1;
}

internal void DeleteWorld(char *world_name)
{
	if (DoesWorldExist(world_name))
	{
		char path[300] = "";
		sprintf(path, "%sworlds\\%s\\", core->res_path, world_name);
		
		// TODO(randy): delete
	}
	else
	{
		LogWarning("World %s doesn't exist, can't delete", world_name);
	}
}

internal b8 LoadWorld(char *world_name)
{
	Assert(world_name[0] && strlen(world_name) < sizeof(core->run_data->world_name));
	
	char world_path[300] = "";
	sprintf(world_path, "%sworlds\\%s\\", core->res_path, world_name);
	if (!platform->DoesDirectoryExist(world_path))
		return 0;
	
	char chunks_path[300] = "";
	sprintf(chunks_path, "%s\\chunks\\", world_path);
	if (!platform->DoesDirectoryExist(chunks_path))
		return 0;
	
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	strcpy(core->run_data->world_name, world_name);
	strcpy(core->run_data->world_path, world_path);
	strcpy(core->run_data->world_chunks_path, chunks_path);
	InitialiseRunData();
	
	// NOTE(randy): Read in world data
	{
		char file_path[200] = "";
		sprintf(file_path, "%sworld_data.arc", world_path);
		FILE *file = fopen(file_path, "rb");
		Assert(file);
		ReadWorldDataFromFile(file, GetWorldData());
		fclose(file);
	}
	
	// NOTE(randy): Load in character entity chunk
	Chunk *chunk = LoadWorldChunk(GetWorldData()->character_chunk);
	Assert(chunk);
	
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
	Assert(GetRunData()->world_path[0]);
	
	// NOTE(randy): Save world data
	{
		char file_path[200] = "";
		sprintf(file_path, "%sworld_data.arc", GetRunData()->world_path);
		FILE *file = fopen(file_path, "wb");
		Assert(file);
		WriteWorldDataToFile(file, GetWorldData());
		fclose(file);
	}
	
	// NOTE(randy): Save all chunk data
	{
		Assert(GetWorldData()->character_chunk.x != INT_MAX);
		for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
		{
			Chunk *chunk = &GetRunData()->chunks[i];
			if (chunk->flags & CHUNK_FLAGS_is_allocated)
			{
				iv2 pos = chunk->pos;
				// NOTE(randy): Temp stress test
				UnloadWorldChunk(pos);
				LoadWorldChunk(pos);
			}
		}
	}
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

internal Chunk *AllocateNewChunk(iv2 pos)
{
	Chunk *chunk = GetUnallocatedChunk();
	Assert(chunk);
	chunk->flags |= CHUNK_FLAGS_is_allocated;
	chunk->pos = pos;
	for (i32 j = 0; j < MAX_TERRAIN_VERT_IN_CHUNK; j++)
	{
		chunk->terrain_verts[j].x = INFINITY;
		chunk->terrain_verts[j].y = INFINITY;
	}
	
	return chunk;
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
		chunk->entities[i] = entity;
	}
	
	ReadFromFile(file, chunk->terrain_verts, sizeof(chunk->terrain_verts));
}

internal Chunk *LoadWorldChunk(iv2 pos)
{
	Assert(pos.x != INT_MAX);
	
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
		
		// NOTE(randy): Check if this chunk had the character
		Assert(GetWorldData()->character_chunk.x != INT_MAX);
		if (GetWorldData()->character_chunk.x == pos.x &&
			GetWorldData()->character_chunk.y == pos.y)
		{
			for (i32 i = 0; i < chunk->entity_count; i++)
			{
				Entity *entity = chunk->entities[i];
				if (EntityHasProperty(entity, ENTITY_PROPERTY_is_character))
				{
					GetRunData()->character_entity = entity;
					break;
				}
			}
			Assert(GetRunData()->character_entity);
		}
		
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
			return AllocateNewChunk(pos);
		}
	}
}

internal void LoadWorldChunksInView()
{
	iv2 chunks[MAX_WORLD_CHUNKS] = {0};
	i32 count;
	GetChunkPositionsInRegion(chunks, &count, GetCameraRegionRect(), 1);
	
	for (i32 i = 0; i < count; i++)
	{
		iv2 pos = chunks[i];
		Chunk *chunk = LoadWorldChunk(pos);
		Assert(chunk);
	}
}

internal b8 UnloadWorldChunk(iv2 pos)
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
	
	// NOTE(randy): If this chunk was the character chunk, clear helper entity reference
	if (pos.x == GetWorldData()->character_chunk.x &&
		pos.y == GetWorldData()->character_chunk.y)
	{
		GetRunData()->character_entity = 0;
	}
	
	return 1;
}

internal void UnloadWorldChunksOutOfView()
{
	iv2 chunks[MAX_WORLD_CHUNKS] = {0};
	i32 count;
	GetChunkPositionsInRegion(chunks, &count, GetCameraRegionRect(), 1);
	
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &GetRunData()->chunks[i];
		if (chunk->flags & CHUNK_FLAGS_is_allocated)
		{
			b8 found = 0;
			for (i32 j = 0; j < count; j++)
			{
				iv2 pos = chunks[j];
				if (pos.x == chunk->pos.x && pos.y == chunk->pos.y)
				{
					found = 1;
					break;
				}
			}
			
			if (!found)
			{
				UnloadWorldChunk(chunk->pos);
			}
		}
	}
}

internal void DeleteChunk(Chunk *chunk)
{
	for (i32 i = 0; i < chunk->entity_count; i++)
	{
		DeleteEntity(chunk->entities[i]);
	}
    
	MemorySet(chunk, 0, sizeof(Chunk));
}

internal void UpdateWorldChunks()
{
	// NOTE(randy): Update positional entities
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &GetRunData()->chunks[i];
		chunk->entity_count = 0;
		MemorySet(chunk->entities, 0, sizeof(chunk->entities));
	}
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_positional);)
	{
		Chunk *chunk = GetChunkFromEntity(entity);
		if (chunk)
		{
			Assert(chunk->entity_count + 1 < 512);
			chunk->entities[chunk->entity_count++] = entity;
		}
		else
		{
			LogError("why is entity %s not in a loaded chunk??", entity->debug_name);
		}
	}
	
	UnloadWorldChunksOutOfView();
	LoadWorldChunksInView();
	
	// NOTE(randy): Figure out what chunk the character is in
	if (GetRunData()->character_entity)
	{
		Chunk* chunk = GetChunkFromEntity(GetRunData()->character_entity);
		if (chunk)
		{
			GetWorldData()->character_chunk = chunk->pos;
		}
		else
		{
			LogError("Character chunk isn't loaded?");
		}
	}
}

internal void GenerateTerrainSegments()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_terrain_segment);)
	{
		DeleteEntity(entity);
	}
	
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &GetRunData()->chunks[i];
		if (chunk->flags & CHUNK_FLAGS_is_allocated && isfinite(chunk->terrain_verts[0].x))
		{
			Entity *new_seg = NewEntity();
			TerrainSegmentEntityPresetCallback(new_seg);
			
			i32 vert_count = 0;
			for (i32 j = 0; j < MAX_TERRAIN_VERT_IN_CHUNK; j++)
			{
				if (isfinite(chunk->terrain_verts[j].x))
				{
					new_seg->physics.shape.line_segments.vertices[j] = v2(chunk->pos.x * CHUNK_SIZE + chunk->terrain_verts[j].x,
																		  chunk->pos.y * CHUNK_SIZE + chunk->terrain_verts[j].y);
					vert_count++;
				}
				else
				{
					break;
				}
			}
			new_seg->physics.shape.line_segments.count = vert_count;
			
			v2 last_vert = new_seg->physics.shape.line_segments.vertices[vert_count - 1];
			v2 closest_vert = {INFINITY, INFINITY};
			for (i32 x = 0; x < 2; x++)
				for (i32 y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0)
					continue;
				
				Chunk *neighbour_chunk = GetChunkAtPos(iv2(chunk->pos.x + x, chunk->pos.y + y));
				if (neighbour_chunk && neighbour_chunk->flags & CHUNK_FLAGS_is_allocated && isfinite(neighbour_chunk->terrain_verts[0].x))
				{
					v2 potential_vert = V2AddV2(neighbour_chunk->terrain_verts[0],
												v2((f32)neighbour_chunk->pos.x * CHUNK_SIZE,
												   (f32)neighbour_chunk->pos.y * CHUNK_SIZE));
					if (potential_vert.x > last_vert.x && potential_vert.x < closest_vert.x)
					{
						closest_vert = potential_vert;
					}
				}
			}
			
			if (isfinite(closest_vert.x) && vert_count > 0)
			{
				new_seg->physics.shape.line_segments.vertices[vert_count] = closest_vert;
				new_seg->physics.shape.line_segments.count = vert_count + 1;
			}
		}
	}
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
