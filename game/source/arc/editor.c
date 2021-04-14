internal void InitMapEditor()
{
	if (core->run_data->world_name[0])
	{
		UnloadWorld();
	}
	
	core->is_ingame = 1;
	GetRunData()->editor_state = EDITOR_STATE_map;
	InitialiseRunData();
	
	GetRunData()->debug_flags |= DEBUG_FLAGS_draw_chunk_grid;
}

internal void EditorUpdate()
{
	DrawEditorUI();
	TransformEditorCamera();
	DrawWorld();
	RenderColliders();
}

internal void DrawEditorUI()
{
	local_persist b8 is_entity_window_open = 0;
	local_persist b8 is_performance_window_open = 0;
	local_persist b8 is_debug_window_open = 0;
	
	local_persist b8 pin_windows = 0;
	
	// NOTE(randy): Drop-down menus
	if (core->run_data->editor_state)
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
			
			if (TsUIDropdown("Editor Mode..."))
			{
				for (i32 i = 1; i < EDITOR_STATE_MAX; i++)
				{
					if (TsUIToggler(GetEditorStateName(i),
									core->run_data->editor_state == i))
					{
						SwitchEditorState(i);
					}
				}
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("View..."))
			{
				if (TsUIToggler("Draw Colliders", core->run_data->debug_flags & DEBUG_FLAGS_draw_collision))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_collision;
				
				if (TsUIToggler("Draw Chunk Grid", core->run_data->debug_flags & DEBUG_FLAGS_draw_chunk_grid))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_chunk_grid;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_chunk_grid;
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("Windows..."))
			{
				if (TsUIToggler("Entity", is_entity_window_open))
					is_entity_window_open = 1;
				else
					is_entity_window_open = 0;
				
				if (TsUIToggler("Performance", is_performance_window_open))
					is_performance_window_open = 1;
				else
					is_performance_window_open = 0;
				
				if (TsUIToggler("Debug", is_debug_window_open))
					is_debug_window_open = 1;
				else
					is_debug_window_open = 0;
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("Options..."))
			{
				if (TsUIToggler("Pin Windows", pin_windows))
					pin_windows = 1;
				else
					pin_windows = 0;
			}
			TsUIDropdownEnd();
		}
		TsUIPopRow();
	}
	
	//~Editors
	switch (core->run_data->editor_state)
	{
		case EDITOR_STATE_none:
		break;
		
		case EDITOR_STATE_map:
		{
			UpdateMapEditor();
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
	if (core->run_data->editor_state != editor_state)
	{
		switch (editor_state)
		{
			case EDITOR_STATE_none :
			{
				return;
			} break;
			
			case EDITOR_STATE_map :
			{
				InitMapEditor();
			} break;
		}
		
		core->run_data->editor_state = editor_state;
	}
}

internal void UpdateMapEditor()
{
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
			
			PushDebugShape(middle_bounds, C2_SHAPE_TYPE_aabb, v2(0.0f, 0.0f), v3(1.0f, 1.0f, 1.0f));
			
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
		
		// NOTE(randy): Vertex circles
		if (selected_entity->physics.shape_type == C2_SHAPE_TYPE_line_segments)
		{
			f32 circle_size = 3.0f;
			c2Shape middle_bounds = {0};
			middle_bounds.aabb.min = c2V(-circle_size / 2.0f, -circle_size / 2.0f);
			middle_bounds.aabb.max = c2V(circle_size / 2.0f, circle_size / 2.0f);
			
			local_persist v2 *selected_seg = 0;
			
			for (i32 i = 0; i < selected_entity->physics.shape.line_segments.count; i++)
			{
				LineSegments *segs = &selected_entity->physics.shape.line_segments;
				
				c2Shape point_shape = middle_bounds;
				v2 p1_pos = V2AddV2(selected_entity->position,
									segs->vertices[i]);
				AddPositionOffsetToShape(&point_shape, C2_SHAPE_TYPE_aabb, p1_pos);
				
				f32 p1_tint = 1.0f;
				local_persist v2 grab_offset = {0.0f, 0.0f};
				
				if (IsV2OverlappingShape(GetMousePositionInWorldSpace(),
										 point_shape,
										 C2_SHAPE_TYPE_aabb))
				{
					p1_tint = 0.75f;
					
					if (platform->left_mouse_pressed)
					{
						grab_offset = V2SubtractV2(p1_pos, GetMousePositionInWorldSpace());
						selected_seg = &segs->vertices[i];
						platform->left_mouse_pressed = 0;
					}
				}
				
				if (&segs->vertices[i] == selected_seg)
				{
					p1_tint = 0.5f;
					
					b8 has_snapped = 0;
					/*
										for (Entity *candidate = 0; IncrementEntityWithProperty(&candidate, ENTITY_PROPERTY_ground_segment);)
										{
											if (candidate == selected_entity)
												continue;
											
											f32 snap_amount = 2.0f;
											
											v2 pos = GetMousePositionInWorldSpace();
											v2 candidate_pos_p1 = V2AddV2(candidate->physics.shape.line.p1,
																		  candidate->position);
											v2 candidate_pos_p2 = V2AddV2(candidate->physics.shape.line.p2,
																		  candidate->position);
											
											if (pos.x >= candidate_pos_p1.x - snap_amount &&
												pos.x <= candidate_pos_p1.x + snap_amount &&
												pos.y >= candidate_pos_p1.y - snap_amount &&
												pos.y <= candidate_pos_p1.y + snap_amount)
											{
												has_snapped = 1;
												selected_entity->physics.shape.line.p1 = V2SubtractV2(candidate_pos_p1, selected_entity->position);
												break;
											}
											else if (pos.x >= candidate_pos_p2.x - snap_amount &&
													 pos.x <= candidate_pos_p2.x + snap_amount &&
													 pos.y >= candidate_pos_p2.y - snap_amount &&
													 pos.y <= candidate_pos_p2.y + snap_amount)
											{
												has_snapped = 1;
												selected_entity->physics.shape.line.p1 = V2SubtractV2(candidate_pos_p2, selected_entity->position);
												break;
											}
											
										}
					 */
					
					if (!has_snapped)
					{
						segs->vertices[i] = V2AddV2(V2SubtractV2(GetMousePositionInWorldSpace(), selected_entity->position), grab_offset);
					}
				}
				
				if (core->left_mouse_released)
				{
					selected_seg = 0;
				}
				
				v2 p1_pos_view = v2view(v2(p1_pos.x - circle_size / 2.0f,
										   p1_pos.y - circle_size / 2.0f));
				v2 middle_size = v2zoom(v2(circle_size, circle_size));
				
				StaticSpriteData *circle = &global_static_sprite_data[STATIC_SPRITE_circle_icon];
				
				ArcPushTexture(circle->texture_atlas,
							   0,
							   circle->source,
							   v4(p1_pos_view.x, p1_pos_view.y, middle_size.x, middle_size.y),
							   v4u(p1_tint),
							   LAYER_HUD);
			}
		}
	}
	
	v2 window_size = {300.0f, 400.0f};
	TsUIWindowBegin("Entity Library", v4(core->render_w - window_size.x, 0.0f, window_size.x, window_size.y), 0, 0);
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
				layer -= 1.0f;
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
					
					Chunk *chunk = GetUnallocatedChunk();
					chunk->flags |= CHUNK_FLAGS_is_allocated;
					chunk->pos = pos;
				}
			}
			
			ArcPushFilledRect(colour, v4(r_pos.x, r_pos.y, r_size.x, r_size.y), 0.0f);
		}
	}
	
	
	/*
		{
			iv2 chunks[MAX_WORLD_CHUNKS] = {0};
			i32 count = 0;
			GetChunkPositionsInRegion(chunks, &count, GetCameraRegionRect(), 0);
			for (i32 i = 0; i < count; i++)
			{
				iv2 pos = chunks[i];
				
				
			}
		}
	 */
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