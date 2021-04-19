internal void InitMapEditor()
{
	if (core->run_data->world_name[0])
	{
		UnloadWorld();
	}
	
	core->is_ingame = 1;
	GetRunData()->editor_state = EDITOR_STATE_general;
	InitialiseRunData();
	
	GetRunData()->debug_flags |= DEBUG_FLAGS_draw_chunk_grid;
	GetRunData()->debug_flags |= DEBUG_FLAGS_draw_collision;
}

internal void EditorUpdate()
{
	UpdateMapChunks();
	DrawEditorUI();
	TransformEditorCamera();
	GenerateTerrainSegments();
	DrawWorld();
	RenderColliders();
}

internal void DrawEditorUI()
{
	// NOTE(randy): Editor drawing / updating
	switch (core->run_data->editor_state)
	{
		case EDITOR_STATE_none:
		break;
		
		case EDITOR_STATE_general:
		{
			DrawGeneralEditor();
		} break;
		
		case EDITOR_STATE_terrain:
		{
			DrawTerrainEditor();
		} break;
		
		default:
		Assert(0);
		break;
	}
	
	// NOTE(randy): Time dilation
	if (core->run_data->editor_state)
	{
		TsUIBeginInputGroup();
		TsUIPushColumn(v2(core->render_w / 2.0f - 125.0f, 40.0f), v2(250.0f, 30.0f));
		
		core->world_delta_mult = TsUISlider("World Time Dilation", core->world_delta_mult, 0.0f, 1.0f);
		
		TsUIPopColumn();
		TsUIEndInputGroup();
	}
	
	
}

internal void SwitchEditorState(EditorState editor_state)
{
	if (GetRunData()->editor_state == EDITOR_STATE_none && editor_state != EDITOR_STATE_none)
	{
		InitMapEditor();
	}
	else if (GetRunData()->editor_state != EDITOR_STATE_none && editor_state == EDITOR_STATE_none)
	{
		// TODO(randy): Rebuild normal world
	}
	
	core->run_data->editor_state = editor_state;
}

internal void DrawGeneralEditor()
{
	TsUIPushAutoRow(v2(0, 0), 30);
	{
		if (TsUIDropdown("Map..."))
		{
			if (TsUIButton("Write Map Data"))
			{
				CommitActiveChunks();
			}
		}
		TsUIDropdownEnd();
	}
	TsUIPopRow();
	
	v2 window_size = {300.0f, 400.0f};
	TsUIWindowBegin("Info", v4(core->render_w - window_size.x, 0.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		char lbl[50];
		sprintf(lbl, "Camera: %f, %f", core->camera_position.x, core->camera_position.y);
		TsUILabel(lbl);
		
		TsUILabel("-------------");
		
		if (platform->left_mouse_pressed && platform->key_down[KEY_alt])
		{
			GetRunData()->selected_chunk = iv2(WorldSpaceToChunkIndex(GetMousePositionInWorldSpace().x),
											   WorldSpaceToChunkIndex(GetMousePositionInWorldSpace().y));
			
			platform->left_mouse_pressed = 0;
		}
		
		if (GetRunData()->selected_chunk.x != INT_MAX)
		{
			sprintf(lbl, "Chunk %i, %i",
					GetRunData()->selected_chunk.x,
					GetRunData()->selected_chunk.y);
			TsUILabel(lbl);
		}
		else
		{
			sprintf(lbl, "Alt left-click to select a chunk");
			TsUILabel(lbl);
		}
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	Entity *selected_entity = core->run_data->selected_entity;
	if (selected_entity)
	{
		// NOTE(randy): Center circle for moving entities
		{
			f32 circle_size = 3.0f;
			c2Shape middle_bounds = {0};
			middle_bounds.aabb.min = c2V(-circle_size / 2.0f, -circle_size / 2.0f);
			middle_bounds.aabb.max = c2V(circle_size / 2.0f, circle_size / 2.0f);
			AddPositionOffsetToShape(&middle_bounds, C2_SHAPE_TYPE_aabb, selected_entity->position);
			
			PushDebugShape(middle_bounds, C2_SHAPE_TYPE_aabb, v2(0.0f, 0.0f), v4u(1.0f));
			
			f32 middle_tint = 1.0f;
			local_persist b8 is_holding_middle = 0;
			local_persist v2 grab_offset = {0.0f, 0.0f};
			
			if (core->left_mouse_released)
			{
				is_holding_middle = 0;
			}
			
			if (is_holding_middle)
			{
				middle_tint = 0.5f;
				
				selected_entity->position = V2AddV2(GetMousePositionInWorldSpace(), grab_offset);
			}
			else if (IsV2OverlappingShape(GetMousePositionInWorldSpace(),
										  middle_bounds,
										  C2_SHAPE_TYPE_aabb))
			{
				middle_tint = 0.75f;
				
				if (platform->left_mouse_pressed)
				{
					is_holding_middle = 1;
					grab_offset = V2SubtractV2(selected_entity->position, GetMousePositionInWorldSpace());
					platform->left_mouse_pressed = 0;
				}
			}
			
			v2 middle_pos = v2view(v2(selected_entity->position.x - circle_size / 2.0f,
									  selected_entity->position.y - circle_size / 2.0f));
			v2 middle_size = v2zoom(v2(circle_size, circle_size));
			
			StaticSpriteData *middle = &global_static_sprite_data[STATIC_SPRITE_circle_icon];
			
			ArcPushTexture(middle->texture_atlas,
						   0,
						   middle->source,
						   v4(middle_pos.x, middle_pos.y, middle_size.x, middle_size.y),
						   v4u(middle_tint),
						   LAYER_HUD);
		}
	}
	
	//~Entity Library
	TsUIWindowBegin("Entity Library", v4(0.0f, 80.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		for (i32 i = 1; i < ENTITY_PRESET_TYPE_MAX; i++)
		{
			EntityPresetTypeData *entity_preset = &global_entity_preset_type_data[i];
			
			char label[100];
			sprintf(label, "%s", entity_preset->print_name);
			if (TsUIButton(label))
			{
				Entity *entity = NewEntity();
				entity_preset->setup_callback(entity);
			}
		}
		
		for (Entity *entity = 0; IncrementEntity(&entity);)
		{
			char label[100];
			sprintf(label, "%s", entity->debug_name);
			TsUILabel(label);
		}
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
}

internal void DrawTerrainEditor()
{
	local_persist b8 prime_terrain_drawer = 0;
	local_persist v2 last_point = {INFINITY, INFINITY};
	
	TsUIPushAutoRow(v2(0, 0), 30);
	{
		if (TsUIDropdown("Terrain..."))
		{
			if (TsUIButton("Draw Terrain"))
			{
				prime_terrain_drawer = 1;
				platform->left_mouse_pressed = 0;
			}
		}
		TsUIDropdownEnd();
	}
	TsUIPopRow();
	
	v2 window_size = {300.0f, 400.0f};
	TsUIWindowBegin("Info", v4(core->render_w - window_size.x, 0.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		if (platform->left_mouse_pressed && platform->key_down[KEY_alt])
		{
			iv2 new_sel_chunk = iv2(WorldSpaceToChunkIndex(GetMousePositionInWorldSpace().x),
									WorldSpaceToChunkIndex(GetMousePositionInWorldSpace().y));
			if (new_sel_chunk.x == GetRunData()->selected_chunk.x &&
				new_sel_chunk.y == GetRunData()->selected_chunk.y)
			{
				GetRunData()->selected_chunk = iv2(INT_MAX, INT_MAX);
			}
			else
			{
				GetRunData()->selected_chunk = new_sel_chunk;
			}
			
			platform->left_mouse_pressed = 0;
		}
		
		char lbl[50];
		if (GetRunData()->selected_chunk.x != INT_MAX)
		{
			Chunk *chunk = GetChunkAtPos(GetRunData()->selected_chunk);
			
			sprintf(lbl, "Chunk %i, %i",
					GetRunData()->selected_chunk.x,
					GetRunData()->selected_chunk.y);
			TsUILabel(lbl);
			
			if (chunk)
			{
				i32 vert_count = 0;
				for (i32 i = 0; i < MAX_TERRAIN_VERT_IN_CHUNK; i++)
				{
					v2 *vert = &chunk->terrain_verts[i];
					if (!isfinite(vert->x))
					{
						break;
					}
					vert_count++;
				}
				
				if (vert_count > 0)
				{
					sprintf(lbl, "Terrain Vertices: %i", vert_count);
					TsUILabel(lbl);
					if (TsUIButton("Clear Vertices"))
					{
						for (i32 i = 0; i < MAX_TERRAIN_VERT_IN_CHUNK; i++)
						{
							chunk->terrain_verts[i].x = INFINITY;
							chunk->terrain_verts[i].y = INFINITY;
						}
					}
				}
			}
		}
		else
		{
			sprintf(lbl, "Alt left-click to select a chunk");
			TsUILabel(lbl);
		}
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	local_persist b8 is_drawing_terrain = 0;
	if (prime_terrain_drawer && platform->left_mouse_pressed)
	{
		is_drawing_terrain = 1;
	}
	
	if (is_drawing_terrain)
	{
		if (core->left_mouse_released)
		{
			prime_terrain_drawer = 0;
			is_drawing_terrain = 0;
			last_point = v2(INFINITY, INFINITY);
			core->left_mouse_released = 0;
		}
		else if (platform->left_mouse_down)
		{
			v2 diff = V2SubtractV2(last_point, GetMousePositionInWorldSpace());
			f32 dist = PythagSolve(diff.x, diff.y);
			const f32 TERRAIN_LENGTH = 15.0f;
			if (!isfinite(last_point.x) || dist >= TERRAIN_LENGTH)
			{
				last_point = GetMousePositionInWorldSpace();
				iv2 lp_chunk_pos = iv2(WorldSpaceToChunkIndex(last_point.x),
									   WorldSpaceToChunkIndex(last_point.y));
				Chunk *chunk = GetChunkAtPos(lp_chunk_pos);
				if (!chunk && !LoadMapChunk(lp_chunk_pos))
				{
					chunk = AllocateNewChunk(lp_chunk_pos);
				}
				
				for (i32 i = 0; i < MAX_TERRAIN_VERT_IN_CHUNK; i++)
				{
					v2 *vert = &chunk->terrain_verts[i];
					if (!isfinite(vert->x))
					{
						*vert = V2SubtractV2(last_point, v2((f32)chunk->pos.x * CHUNK_SIZE,
															(f32)chunk->pos.y * CHUNK_SIZE));
						break;
					}
				}
			}
		}
	}
	
	f32 circle_size = 3.0f;
	c2Shape middle_bounds = {0};
	middle_bounds.aabb.min = c2V(-circle_size / 2.0f, -circle_size / 2.0f);
	middle_bounds.aabb.max = c2V(circle_size / 2.0f, circle_size / 2.0f);
	StaticSpriteData *circle = &global_static_sprite_data[STATIC_SPRITE_circle_icon];
	local_persist v2 *held_vert = 0;
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &GetRunData()->chunks[i];
		if (chunk->flags & CHUNK_FLAGS_is_allocated)
		{
			for (i32 j = 0; j < MAX_TERRAIN_VERT_IN_CHUNK; j++)
			{
				if (isfinite(chunk->terrain_verts[j].x))
				{
					v2 pos = V2AddV2(chunk->terrain_verts[j], v2((f32)chunk->pos.x * CHUNK_SIZE,
																 (f32)chunk->pos.y * CHUNK_SIZE));
					
					v4 tint = v4u(1.0);
					if (!is_drawing_terrain)
					{
						if (platform->left_mouse_down && held_vert == &chunk->terrain_verts[j])
						{
							tint = v4(0.5f, 0.5f, 0.5f, 1.0f);
							
							*held_vert = ClampV2(V2SubtractV2(GetMousePositionInWorldSpace(),
															  v2((f32)chunk->pos.x * CHUNK_SIZE,
																 (f32)chunk->pos.y * CHUNK_SIZE)),
												 v2(0.0f, 0.0f),
												 v2(CHUNK_SIZE, CHUNK_SIZE));
						}
						else
						{
							c2Shape vert_shape = middle_bounds;
							AddPositionOffsetToShape(&vert_shape, C2_SHAPE_TYPE_aabb, pos);
							if (IsV2OverlappingShape(GetMousePositionInWorldSpace(),
													 vert_shape,
													 C2_SHAPE_TYPE_aabb))
							{
								tint = v4(0.8f, 0.8f, 0.8f, 1.0f);
								
								if (platform->left_mouse_pressed)
								{
									held_vert = &chunk->terrain_verts[j];
									platform->left_mouse_pressed = 0;
								}
							}
						}
						
						if (core->left_mouse_released)
						{
							held_vert = 0;
							core->left_mouse_released = 0;
						}
					}
					
					v2 r_pos = v2view(v2(pos.x - circle_size / 2.0f,
										 pos.y - circle_size / 2.0f));
					v2 r_size = v2zoom(v2(circle_size, circle_size));
					
					ArcPushTexture(circle->texture_atlas,
								   0,
								   circle->source,
								   v4(r_pos.x, r_pos.y, r_size.x, r_size.y),
								   tint,
								   LAYER_HUD);
				}
			}
			
			// NOTE(randy): Sort all vertices in order of x pos
			for (i32 step = 0; step < MAX_TERRAIN_VERT_IN_CHUNK - 1; step++)
				for (i32 j = 0; j < MAX_TERRAIN_VERT_IN_CHUNK - step - 1; j++)
			{
				if (chunk->terrain_verts[j].x > chunk->terrain_verts[j + 1].x)
				{
					v2 temp = chunk->terrain_verts[j];
					chunk->terrain_verts[j] = chunk->terrain_verts[j + 1];
					chunk->terrain_verts[j + 1] = temp;
					
					if (held_vert == &chunk->terrain_verts[j])
					{
						held_vert = &chunk->terrain_verts[j + 1];
					}
					else if (held_vert == &chunk->terrain_verts[j + 1])
					{
						held_vert = &chunk->terrain_verts[j];
					}
				}
			}
		}
	}
}

internal void UpdateMapChunks()
{
	iv2 non_existent_chunks[MAX_WORLD_CHUNKS] = {0};
	i32 non_existent_chunk_count = 0;
	
	// NOTE(randy): Load/unload map chunks depending on view
	{
		iv2 chunks[MAX_WORLD_CHUNKS] = {0};
		i32 count;
		GetChunkPositionsInRegion(chunks, &count, GetCameraRegionRect(), 0);
		
		for (i32 i = 0; i < count; i++)
		{
			iv2 pos = chunks[i];
			if (!GetChunkAtPos(pos))
			{
				Chunk *chunk = LoadMapChunk(pos);
				if (!chunk)
				{
					non_existent_chunks[non_existent_chunk_count++] = pos;
				}
			}
		}
		
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
					UnloadMapChunk(chunk->pos);
				}
			}
		}
	}
	
	// NOTE(randy): Draw the chunk grid
	if (GetRunData()->debug_flags & DEBUG_FLAGS_draw_chunk_grid)
	{
		iv2 chunks[MAX_WORLD_CHUNKS] = {0};
		i32 count;
		GetChunkPositionsInRegion(chunks, &count, GetCameraRegionRect(), 0);
		
		for (i32 i = 0; i < count; i++)
		{
			iv2 chunk = chunks[i];
			
			v4 colour = v4u(0.5f);
			f32 layer = LAYER_FRONT_UI;
			if (GetRunData()->selected_chunk.x == chunk.x &&
				GetRunData()->selected_chunk.y == chunk.y)
			{
				colour = v4(1.0f, 0.0f, 0.0f, 1.0f);
				layer -= 2.0f;
			}
			else if (GetChunkAtPos(chunk))
			{
				colour = v4u(1.0f);
				layer -= 1.0f;
			}
			
			ArcPushLine(colour,
						v2view(v2((f32)CHUNK_SIZE * chunk.x,
								  (f32)CHUNK_SIZE * chunk.y)),
						v2view(v2((f32)CHUNK_SIZE * chunk.x,
								  (f32)CHUNK_SIZE * chunk.y + CHUNK_SIZE)),
						layer);
			ArcPushLine(colour,
						v2view(v2((f32)CHUNK_SIZE * chunk.x,
								  (f32)CHUNK_SIZE * chunk.y + CHUNK_SIZE)),
						v2view(v2((f32)CHUNK_SIZE * chunk.x + CHUNK_SIZE,
								  (f32)CHUNK_SIZE * chunk.y + CHUNK_SIZE)),
						layer);
			ArcPushLine(colour,
						v2view(v2((f32)CHUNK_SIZE * chunk.x + CHUNK_SIZE,
								  (f32)CHUNK_SIZE * chunk.y + CHUNK_SIZE)),
						v2view(v2((f32)CHUNK_SIZE * chunk.x + CHUNK_SIZE,
								  (f32)CHUNK_SIZE * chunk.y)),
						layer);
			ArcPushLine(colour,
						v2view(v2((f32)CHUNK_SIZE * chunk.x + CHUNK_SIZE,
								  (f32)CHUNK_SIZE * chunk.y)),
						v2view(v2((f32)CHUNK_SIZE * chunk.x,
								  (f32)CHUNK_SIZE * chunk.y)),
						layer);
		}
	}
	
	// NOTE(randy): Render load hints for all non-existent surrounding chunks
	{
		for (i32 i = 0; i < non_existent_chunk_count; i++)
		{
			iv2 pos = non_existent_chunks[i];
			
			v2 r_size = v2zoom(v2(CHUNK_SIZE / 5.0f, CHUNK_SIZE / 5.0f));
			v2 r_pos = V2SubtractV2(v2view(v2((f32)pos.x * CHUNK_SIZE + CHUNK_SIZE / 2.0f,
											  (f32)pos.y * CHUNK_SIZE + CHUNK_SIZE / 2.0f)),
									V2DivideF32(r_size, 2.0f));
			
			v4 colour = v4u(1.0f);
			if (IsPositionInBounds(GetMousePosition(),
								   v4(r_pos.x, r_pos.y, r_size.x, r_size.y)))
			{
				colour = v4(0.8f, 0.8f, 0.8f, 1.0f);
				
				if (platform->left_mouse_pressed)
				{
					platform->left_mouse_pressed = 0;
					AllocateNewChunk(pos);
				}
			}
			
			ArcPushFilledRect(colour, v4(r_pos.x, r_pos.y, r_size.x, r_size.y), 0.0f);
		}
	}
}

internal b8 DoesMapChunkExistOnDisk(iv2 pos)
{
	char chunk_path[100];
	sprintf(chunk_path, "%sinitial_map/chunks/%i.%i.arc", core->res_path, pos.x, pos.y);
	FILE *file = fopen(chunk_path, "rb");
	if (file)
	{
		fclose(file);
		return 1;
	}
	else
	{
		return 0;
	}
}

internal void CommitActiveChunks()
{
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		Chunk *chunk = &GetRunData()->chunks[i];
		if (chunk->flags & CHUNK_FLAGS_is_allocated)
		{
			iv2 pos = chunk->pos;
			// NOTE(randy): Stress test for now, there's a more efficent way of doin dis
			UnloadMapChunk(pos);
			LoadMapChunk(pos);
		}
	}
}

internal Chunk *LoadMapChunk(iv2 pos)
{
	if (GetChunkAtPos(pos))
	{
		LogWarning("Chunk %i, %i is already loaded", pos.x, pos.y);
		return GetChunkAtPos(pos);
	}
	
	char chunk_path[100];
	sprintf(chunk_path, "%sinitial_map/chunks/%i.%i.arc", core->res_path, pos.x, pos.y);
	FILE *file = fopen(chunk_path, "rb");
	if (file)
	{
		// NOTE(randy): Read in chunk
		Chunk *chunk = GetUnallocatedChunk();
		Assert(chunk);
		ReadChunkFromFile(file, chunk);
		fclose(file);
		
		return chunk;
	}
	else
	{
		return 0;
	}
}

internal void UnloadMapChunk(iv2 pos)
{
	Chunk *chunk = GetChunkAtPos(pos);
	if (!chunk)
	{
		LogWarning("There is no chunk to unload at %i, %i", pos.x, pos.y);
		return;
	}
	
	char path[100];
	sprintf(path, "%sinitial_map\\", core->res_path);
	if (!platform->DoesDirectoryExist(path))
		platform->MakeDirectory(path);
	
	sprintf(path, "%sinitial_map\\chunks\\", core->res_path);
	if (!platform->DoesDirectoryExist(path))
		platform->MakeDirectory(path);
	
	sprintf(path, "%sinitial_map\\chunks\\%i.%i.arc", core->res_path, pos.x, pos.y);
	FILE *file = fopen(path, "wb");
	Assert(file);
	
	SaveChunkToFile(file, chunk);
	fclose(file);
	
	// NOTE(randy): Commit chunk to the source map
	char dest[200];
	sprintf(dest, "%s..\\..\\res\\initial_map\\", core->res_path);
	if (!platform->DoesDirectoryExist(dest))
		platform->MakeDirectory(dest);
	
	sprintf(dest, "%s..\\..\\res\\initial_map\\chunks\\", core->res_path);
	if (!platform->DoesDirectoryExist(dest))
		platform->MakeDirectory(dest);
	
	sprintf(dest, "%s..\\..\\res\\initial_map\\chunks\\%i.%i.arc", core->res_path, pos.x, pos.y);
	platform->CopyFile(dest, path);
	
	DeleteChunk(chunk);
}