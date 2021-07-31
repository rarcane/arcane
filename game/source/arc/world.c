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
	
	/*
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
		
		GenerateTerrainSegments();
		
	#ifdef DEVELOPER_TOOLS
		RenderColliders();
	#endif
		
		DrawGameUI();
		
		InteractableUpdate();
		
	 */
	DrawWorld();
	
	UpdateBlueprints();
	
	{
		m3 transform = M3InitD(1.0f);//M3Rotate(-90.f, v3(1, 0, 0));
		//transform = M3MultiplyM3(M3Rotate(platform->mouse_x, v3(0, 1, 0)), transform);
		transform = M3MultiplyM3(transform, M3InitD(1.5f));
		
		//transform = M3MultiplyM3(transform, M3RotateQuat(v4(0.77f, 0.0f, 0.77f, 0.0f)));
		
		Ts2dSkeleton skele = {0};
		TransformSkeletonFromTSM(&skele, &core->little_ron);
		
		if (platform->key_down[KEY_r])
			Ts2dPushModelWithSkeleton(&core->model, &skele, v2(0.0f, 0.0f), v2(1024, 1024), transform, 1.0f);
		else
			Ts2dPushModel(&core->model, v2(0, 0), v2(1024, 1024), transform, 1.f);
	}
	
	/*
		{
			m3 transform = M3Rotate(platform->current_time * 18.f, v3(0, 0, 1));
			transform = M3MultiplyM3(M3Rotate(-90.f, v3(1, 0, 0)), transform);
			Ts2dPushModel(&core->model_sphere, v2(900, 100), v2(512, 512), transform, 1.f);
		}
	 */
	
	/*
		{
			m3 transform = M3Rotate(platform->current_time * 18.f, v3(0, 1, 0));
			Ts2dPushModel(&core->model_link, v2(500, 300), v2(512, 512), transform, 1.f);
		}
	 */
	
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
		world_info.shadow_vector = v2(10.0f, -32.0f);
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
		
		//RenderArc();
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
	if (!(GetEditorData()->debug_flags & DEBUG_FLAGS_disable_draw_terrain))
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
	
	LoadMapData();
	
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
		character->sprite_data.sprite = SPRITE_player_idle;
		
		
		CharacterData *character_data = GetCharacterData();
		
		character_data->inventory_size = 9;
		character_data->hotbar_size = 2;
		character_data->freehand_spell_count = 2;
		
		Item twig = { .type = ITEM_TYPE_twig, .stack_size = 8 };
		character_data->inventory[2] = twig;
		
		/*
				Item flint_sword = { .type = ITEM_TYPE_flint_sword, .stack_size = 1 };
				character_data->inventory[0] = flint_sword;
				Item flint_axe = { .type = ITEM_TYPE_flint_axe, .stack_size = 1 };
				character_data->inventory[3] = flint_axe;
				
				Item flint = { .type = ITEM_TYPE_flint, .stack_size = 7 };
				character_data->inventory[1] = flint;
				
				
				
				Item tool = { .type = ITEM_TYPE_crafting_tool, .stack_size = 1 };
				character_data->hotbar[0] = tool;
				
				
				
				Item chest = { .type = ITEM_TYPE_test_chestpiece, .stack_size = 1 };
				character_data->equipment_slots[1] = chest;
				
				character_data->freehand_spell_slots[0].type = SPELL_TYPE_fireball;
				character_data->freehand_spell_slots[1].type = SPELL_TYPE_yeet;
		 */
	}
	
	core->is_ingame = 1;
	
	SaveWorld();
	
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
	
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	strcpy(core->run_data->world_name, world_name);
	strcpy(core->run_data->world_path, world_path);
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
	
	// NOTE(randy): Read in entities
	{
		char file_path[200] = "";
		sprintf(file_path, "%sentity_data.arc", world_path);
		FILE *file = fopen(file_path, "rb");
		Assert(file);
		
		for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
		{
			Entity *entity = NewEntity();
			ReadEntityFromFile(file, entity);
			
			if (EntityHasProperty(entity, ENTITY_PROPERTY_is_character))
				GetRunData()->character_entity = entity;
		}
		
		fclose(file);
	}
	
	// NOTE(randy): Read in terrain data
	{
		char file_path[200] = "";
		sprintf(file_path, "%sterrain_data.arc", world_path);
		FILE *file = fopen(file_path, "rb");
		Assert(file);
		
		ReadFromFile(file, GetRunData()->terrain, sizeof(GetRunData()->terrain));
		
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
	
	// NOTE(randy): Save entities
	{
		char file_path[200] = "";
		sprintf(file_path, "%sentity_data.arc", GetRunData()->world_path);
		FILE *file = fopen(file_path, "wb");
		Assert(file);
		
		for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
		{
			Entity *entity = &GetRunData()->entities[i];
			
			Entity empty_entity = {0};
			if (EntityHasProperty(entity, ENTITY_PROPERTY_do_not_serialise))
				WriteEntityToFile(file, &empty_entity);
			else
				WriteEntityToFile(file, entity);
		}
		
		fclose(file);
	}
	
	// NOTE(randy): Save terrain data
	{
		char file_path[200] = "";
		sprintf(file_path, "%sterrain_data.arc", GetRunData()->world_path);
		FILE *file = fopen(file_path, "wb");
		Assert(file);
		
		WriteToFile(file, GetRunData()->terrain, sizeof(GetRunData()->terrain));
		
		fclose(file);
	}
}

internal void GenerateTerrainSegments()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_terrain_segment);)
	{
		DeleteEntity(entity);
	}
	
	for (i32 i = 0; i < TERRAIN_TABLE_SIZE / MAX_LINE_SEGMENT_VERTICES; i++)
	{
		Assert(i * MAX_LINE_SEGMENT_VERTICES < TERRAIN_TABLE_SIZE);
		v2 *vert = &GetRunData()->terrain[i * MAX_LINE_SEGMENT_VERTICES - 1 * i];
		if (isfinite(vert->x))
		{
			Entity *new_seg = NewEntity();
			TerrainSegmentEntityPresetCallback(new_seg);
			
			memcpy(new_seg->physics.shape.line_segments.vertices,
				   vert,
				   sizeof(new_seg->physics.shape.line_segments.vertices));
			
			new_seg->physics.shape.line_segments.count = MAX_LINE_SEGMENT_VERTICES;
		}
		else
		{
			break;
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
