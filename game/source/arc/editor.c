internal void InitMapEditor()
{
	if (core->run_data->world_name[0])
	{
		UnloadWorld();
	}
	
	core->is_ingame = 1;
	GetRunData()->editor_state = EDITOR_STATE_general;
	InitialiseRunData();
	
	LoadMapData();
	
	char file_path[128];
	sprintf(file_path, "%seditor_data.arc", core->res_path);
	FILE *file = fopen(file_path, "rb");
	if (file)
	{
		ReadEditorDataFromFile(file, core->editor_data);
		
		core->camera_position = GetEditorData()->last_camera_pos;
		core->camera_zoom = GetEditorData()->last_camera_zoom;
		
		fclose(file);
	}
	else
	{
		ClearSelectedEntities();
		GetEditorData()->debug_flags |= DEBUG_FLAGS_draw_collision;
		GetEditorData()->debug_flags |= DEBUG_FLAGS_disable_draw_terrain;
		GetEditorData()->grid_width = 1;
	}
}

internal void SaveEditorData()
{
	GetEditorData()->last_camera_pos = core->camera_position;
	GetEditorData()->last_camera_zoom = core->camera_zoom;
	
	char file_path[128];
	sprintf(file_path, "%seditor_data.arc", core->res_path);
	FILE *file = fopen(file_path, "wb");
	Assert(file);
	WriteEditorDataToFile(file, core->editor_data);
	fclose(file);
}

internal void ClearSelectedEntities()
{
	memset(GetEditorData()->selected_entities, -1, sizeof(GetEditorData()->selected_entities));
}

internal void EditorUpdate()
{
	DrawEditorUI();
	TransformEditorCamera();
	GenerateTerrainSegments();
	DrawWorld();
	RenderColliders();
	UpdateTextNoteEntities();
}

internal void SaveMapData()
{
	char file_path[300] = "";
	FILE *file = 0;
	
	sprintf(file_path, "%sinitial_map\\", core->res_path);
	if (!platform->DoesDirectoryExist(file_path))
		platform->MakeDirectory(file_path);
	
	sprintf(file_path, "%sinitial_map\\entity_data.arc", core->res_path);
	file = fopen(file_path, "wb");
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
	
	sprintf(file_path, "%sinitial_map\\terrain_data.arc", core->res_path);
	file = fopen(file_path, "wb");
	Assert(file);
	WriteToFile(file, GetRunData()->terrain, sizeof(GetRunData()->terrain));
	fclose(file);
	
	// NOTE(randy): Copy data to root res
	sprintf(file_path, "%sinitial_map\\", core->res_path);
	
	char root_path[200] = "";
	sprintf(root_path, "%s..\\..\\res\\initial_map\\", core->res_path);
	if (!platform->DoesDirectoryExist(root_path))
		platform->MakeDirectory(root_path);
	
	sprintf(root_path, "%s..\\..\\res\\initial_map\\", core->res_path);
	platform->CopyDirectoryRecursively(root_path, file_path);
}

internal b8 LoadMapData()
{
	char file_path[300] = "";
	FILE *file = 0;
	
	sprintf(file_path, "%sinitial_map\\", core->res_path);
	if (!platform->DoesDirectoryExist(file_path))
		return 0;
	
	sprintf(file_path, "%sinitial_map\\entity_data.arc", core->res_path);
	file = fopen(file_path, "rb");
	Assert(file);
	memset(GetRunData()->entities, 0, sizeof(GetRunData()->entities));
	for (i32 i = 0; i < ENTITY_TABLE_SIZE; i++)
	{
		Entity *entity = NewEntity();
		ReadEntityFromFile(file, entity);
	}
	fclose(file);
	
	sprintf(file_path, "%sinitial_map\\terrain_data.arc", core->res_path);
	file = fopen(file_path, "rb");
	Assert(file);
	ReadFromFile(file, GetRunData()->terrain, sizeof(GetRunData()->terrain));
	fclose(file);
	
	return 1;
}

internal void DrawEditorUI()
{
	if (platform->key_pressed[KEY_s] && platform->key_down[KEY_ctrl])
	{
		SaveMapData();
		TsPlatformCaptureKeyboard();
	}
	
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
		Assert(0);{
			
		}
		break;
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
		SaveMapData();
		
		SaveEditorData();
		memset(core->editor_data, 0, sizeof(EditorData));
		
		if (!LoadWorld("testing"))
			CreateWorld("testing");
	}
	
	core->run_data->editor_state = editor_state;
}

typedef struct EntityLibCanvasData
{
	EntityPresetType type;
} EntityLibCanvasData;

internal void EntityLibIconUpdateCallback(char *name, v4 rect, v2 mouse, void *user_data)
{
	EntityLibCanvasData *data = user_data;
	EntityPresetTypeData *entity_preset = &global_entity_preset_type_data[data->type];
	
	local_persist Entity *dragged_entity = 0;
	
	if (platform->left_mouse_pressed &&
		mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
	{
		Entity *entity = NewEntity();
		entity_preset->setup_callback(entity);
		dragged_entity = entity;
		
		ClearSelectedEntities();
		GetEditorData()->selected_entities[0] = (i32)(entity - GetRunData()->entities);
		
		platform->left_mouse_pressed = 0;
	}
	
	if (platform->left_mouse_down && dragged_entity)
	{
		dragged_entity->position = GetMousePositionInWorldSpace();
	}
	
	if (core->left_mouse_released)
	{
		dragged_entity = 0;
	}
}

internal void EntityLibIconRenderCallback(char *name, v4 rect, v2 mouse, void *user_data)
{
	EntityLibCanvasData *data = user_data;
	EntityPresetTypeData *entity_preset = &global_entity_preset_type_data[data->type];
	
	v4 tint = v4u(1.0f);
	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
	{
		tint = v4(0.7f, 0.7f, 0.7f, 1.0f);
		if (platform->left_mouse_down)
		{
			tint = v4(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}
	
	Ts2dPushFilledRect(v4u(0.5f), rect);
	Ts2dPushText(Ts2dGetDefaultFont(),
				 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
				 tint,
				 v2(rect.x + rect.z / 2,
					rect.y + rect.w / 2 + 3),
				 0.3f,
				 entity_preset->print_name);
}

internal void DrawGeneralEditor()
{
	TsUIPushAutoRow(v2(0, 0), 30);
	{
		if (TsUIDropdown("Map..."))
		{
			if (TsUIButton("Write Map Data"))
			{
				SaveMapData();
			}
		}
		TsUIDropdownEnd();
		
		if (TsUIDropdown("Options..."))
		{
			if (TsUIToggler("Draw Colliders", GetEditorData()->debug_flags & DEBUG_FLAGS_draw_collision))
				GetEditorData()->debug_flags |= DEBUG_FLAGS_draw_collision;
			else
				GetEditorData()->debug_flags &= ~DEBUG_FLAGS_draw_collision;
			
			if (TsUIToggler("Disable Terrain", GetEditorData()->debug_flags & DEBUG_FLAGS_disable_draw_terrain))
				GetEditorData()->debug_flags |= DEBUG_FLAGS_disable_draw_terrain;
			else
				GetEditorData()->debug_flags &= ~DEBUG_FLAGS_disable_draw_terrain;
			
			GetEditorData()->grid_width = TsUIIntSlider("Grid Width", GetEditorData()->grid_width, 0, 50);
			
		}
		TsUIDropdownEnd();
	}
	TsUIPopRow();
	
	//~Entity Library
	v2 window_size = {300.0f, 400.0f};
	TsUIWindowBegin("Entity Library", v4(0.0f, 80.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(0.0f, 0.0f), v2(window_size.x, 30.0f));
		
		for (i32 i = 1; i < ENTITY_PRESET_CATEGORY_MAX; i++)
		{
			if (TsUICollapsable(GetEntityPresetCategoryName(i)))
			{
				for (i32 j = 1; j < ENTITY_PRESET_TYPE_MAX; j++)
				{
					EntityPresetTypeData *entity_preset = &global_entity_preset_type_data[j];
					
					if (entity_preset->category == i)
					{
						char label[100];
						sprintf(label, "%s", entity_preset->print_name);
						
						EntityLibCanvasData *data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(EntityLibCanvasData));
						data->type = j;
						TsUICanvas("", EntityLibIconUpdateCallback, data, EntityLibIconRenderCallback, data);
					}
				}
				
				TsUICollapsableEnd();
			}
		}
		
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	//~Info panel
	TsUIWindowBegin("Info", v4(core->render_w - window_size.x, 0.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		char lbl[50];
		sprintf(lbl, "Camera: %f, %f", core->camera_position.x, core->camera_position.y);
		TsUILabel(lbl);
		
		sprintf(lbl, "mouse %f, %f", platform->mouse_x, platform->mouse_y);
		TsUILabel(lbl);
		
		TsUILabel("-------------");
		
		if (GetEditorData()->selected_entities[0] != -1)
		{
			if (GetEditorData()->selected_entities[1] == -1)
			{
				PrintEntityFields(&GetRunData()->entities[GetEditorData()->selected_entities[0]]);
			}
			else
			{
				// NOTE(randy): Multiple entities selected. List them all out
				for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
				{
					if (GetEditorData()->selected_entities[i] == -1)
						break;
					
					Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
					if (TsUICollapsable(selected_entity->debug_name))
					{
						PrintEntityFields(selected_entity);
						TsUICollapsableEnd();
					}
				}
				
				// NOTE(randy): Mass selection actions
				/*
								if (TsUIButton("Floor Selection"))
								{
									for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
									{
										if (GetEditorData()->selected_entities[i] == -1)
											break;
										
										Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
										
										selected_entity->position = v2((i32)(selected_entity->position.x),
																	   (i32)(selected_entity->position.y));
									}
								}
				 */
			}
		}
		else
		{
			TsUILabel("Select an entity...");
		}
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	if (platform->key_pressed[KEY_delete] && GetEditorData()->selected_entities[0] != -1)
	{
		for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
		{
			if (GetEditorData()->selected_entities[i] == -1)
				break;
			
			Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
			DeleteEntity(selected_entity);
		}
		
		TsPlatformCaptureKeyboard();
	}
	
	// NOTE(randy): Entity selection movement handle
	local_persist v2 last_pos = {INFINITY, INFINITY};
	v2 movement_amount = {INFINITY, INFINITY};
	if (GetEditorData()->selected_entities[0] != -1)
	{
		Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[0]];
		
		i32 count = 0;
		v2 pos_total = {0};
		for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
		{
			if (GetEditorData()->selected_entities[i] == -1)
				break;
			
			Entity *entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
			pos_total = V2AddV2(pos_total, GetEntityPosition(entity));
			count++;
		}
		
		v2 pos = V2DivideF32(pos_total, (f32)count);
		
		f32 circle_size = 3.0f;
		c2Shape middle_bounds = {0};
		middle_bounds.aabb.min = c2V(-circle_size / 2.0f, -circle_size / 2.0f);
		middle_bounds.aabb.max = c2V(circle_size / 2.0f, circle_size / 2.0f);
		AddPositionOffsetToShape(&middle_bounds, C2_SHAPE_TYPE_aabb, pos);
		
		PushDebugShape(middle_bounds, C2_SHAPE_TYPE_aabb, v2(0.0f, 0.0f), v4u(1.0f));
		
		f32 middle_tint = 1.0f;
		local_persist b8 is_holding_middle = 0;
		local_persist v2 grab_offset = {0.0f, 0.0f};
		
		if (core->left_mouse_released)
		{
			is_holding_middle = 0;
			last_pos = v2(INFINITY, INFINITY);
		}
		
		if (is_holding_middle)
		{
			middle_tint = 0.5f;
			
			v2 current_pos = V2AddV2(GetMousePositionInWorldSpace(), grab_offset);
			if (GetEditorData()->grid_width > 0)
			{
				current_pos= v2(floorf(current_pos.x / (f32)GetEditorData()->grid_width) * GetEditorData()->grid_width,
								floorf(current_pos.y / (f32)GetEditorData()->grid_width) * GetEditorData()->grid_width);
			}
			
			if (last_pos.x != INFINITY)
				movement_amount = V2SubtractV2(current_pos, last_pos);
			last_pos = current_pos;
		}
		else if (IsV2OverlappingShape(GetMousePositionInWorldSpace(),
									  middle_bounds,
									  C2_SHAPE_TYPE_aabb))
		{
			middle_tint = 0.75f;
			
			if (platform->left_mouse_pressed)
			{
				is_holding_middle = 1;
				grab_offset = V2SubtractV2(pos, GetMousePositionInWorldSpace());
				platform->left_mouse_pressed = 0;
				
				if (platform->key_down[KEY_alt])
				{
					for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
					{
						if (GetEditorData()->selected_entities[i] == -1)
							break;
						
						Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
						
						Entity *new_entity = GetUnallocatedEntity();
						memcpy(new_entity, selected_entity, sizeof(Entity));
						GetEditorData()->selected_entities[i] = (i32)(new_entity - GetRunData()->entities);
					}
				}
			}
		}
		
		v2 middle_pos = v2view(v2(pos.x - circle_size / 2.0f,
								  pos.y - circle_size / 2.0f));
		v2 middle_size = v2zoom(v2(circle_size, circle_size));
		
		SpriteData *middle = &global_sprite_data[SPRITE_circle_icon];
		
		ArcPushTexture(middle->texture_atlas,
					   0,
					   middle->source,
					   v4(middle_pos.x, middle_pos.y, middle_size.x, middle_size.y),
					   v4u(middle_tint),
					   LAYER_HUD);
	}
	
	// NOTE(randy): Move selected entities based on movement handle
	if (movement_amount.x != INFINITY)
		for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
	{
		if (GetEditorData()->selected_entities[i] == -1)
			break;
		
		Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
		
		if (EntityHasProperty(selected_entity, ENTITY_PROPERTY_parallaxable))
		{
			v2 par_amount = GetEntityParallaxAmount(selected_entity);
			v2 expected = V2AddV2(ParallaxPosition(selected_entity->position, par_amount), movement_amount);
			
			selected_entity->position = v2((-par_amount.x * core->camera_position.x + expected.x) /
										   (-par_amount.x + 1.0f),
										   (-par_amount.y * core->camera_position.y + expected.y) /
										   (-par_amount.y + 1.0f));
		}
		else
		{
			selected_entity->position = V2AddV2(selected_entity->position, movement_amount);
		}
	}
	
	// NOTE(randy): Entity selection logic
	if (platform->left_mouse_pressed)
	{
		if (!platform->key_down[KEY_shift])
			ClearSelectedEntities();
		
		for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_positional);)
		{
			if (IsPositionOverlappingEntity(entity, GetMousePositionInWorldSpace()) &&
				!EntityHasProperty(entity, ENTITY_PROPERTY_terrain_segment))
			{
				SelectNewEntity(entity);
				platform->left_mouse_pressed = 0;
				break;
			}
		}
	}
}

internal void DrawTerrainEditor()
{
	local_persist b8 prime_terrain_drawer = 0;
	
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
	
	// NOTE(randy): Vertex drawing
	local_persist b8 is_drawing_terrain = 0;
	local_persist v2 last_point = {INFINITY, INFINITY};
	
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
				for (i32 i = 0; i < TERRAIN_TABLE_SIZE; i++)
				{
					v2 *vert = &GetRunData()->terrain[i];
					if (!isfinite(vert->x))
					{
						*vert = GetMousePositionInWorldSpace();
						last_point = *vert;
						break;
					}
				}
			}
		}
	}
	
#define MAX_SEL_VERT 64
	local_persist v2 *selected_vertices[MAX_SEL_VERT];
	local_persist i32 selected_vert_count = 0;
	local_persist v2 initial_pos = {INFINITY, INFINITY};
	
	if (platform->right_mouse_pressed && !is_drawing_terrain)
	{
		initial_pos = GetMousePositionInWorldSpace();
	}
	
	if (platform->right_mouse_down && !is_drawing_terrain)
	{
		v2 current_pos = GetMousePositionInWorldSpace();
		v2 diff = V2SubtractV2(current_pos, initial_pos);
		diff.x = fabsf(diff.x);
		diff.y = fabsf(diff.y);
		
		v2 pos;
		pos.x = initial_pos.x < current_pos.x ? initial_pos.x : current_pos.x;
		pos.y = initial_pos.y < current_pos.y ? initial_pos.y : current_pos.y;
		pos = v2view(pos);
		
		v2 size = v2zoom(diff);
		
		ArcPushRect(v4(0.0f, 1.0f, 0.0f, 1.0f), v4(pos.x, pos.y, size.x, size.y), LAYER_FRONT_UI);
	}
	
	if (core->right_mouse_released)
	{
		v2 current_pos = GetMousePositionInWorldSpace();
		v2 diff = V2SubtractV2(current_pos, initial_pos);
		diff.x = fabsf(diff.x);
		diff.y = fabsf(diff.y);
		
		v2 pos;
		pos.x = initial_pos.x < current_pos.x ? initial_pos.x : current_pos.x;
		pos.y = initial_pos.y < current_pos.y ? initial_pos.y : current_pos.y;
		
		v4 rect = v4(pos.x, pos.y, diff.x, diff.y);
		
		selected_vert_count = 0;
		for (i32 i = 0; i < TERRAIN_TABLE_SIZE; i++)
		{
			v2 *vert = &GetRunData()->terrain[i];
			
			if (IsV2InRect(*vert, rect))
			{
				if (selected_vert_count + 1 >= MAX_SEL_VERT)
				{
					LogWarning("Selection too big");
					break;
				}
				selected_vertices[selected_vert_count++] = vert;
			}
		}
	}
	
	b8 bypass_inf = 0;
	if (platform->key_pressed[KEY_delete])
	{
		for (i32 i = 0; i < selected_vert_count; i++)
		{
			*selected_vertices[i] = v2(INFINITY, INFINITY);
		}
		selected_vert_count = 0;
		bypass_inf = 1;
	}
	
	// NOTE(randy): Vertex adjustment
	f32 circle_size = 3.0f;
	c2Shape middle_bounds = {0};
	middle_bounds.aabb.min = c2V(-circle_size / 2.0f, -circle_size / 2.0f);
	middle_bounds.aabb.max = c2V(circle_size / 2.0f, circle_size / 2.0f);
	SpriteData *circle = &global_sprite_data[SPRITE_circle_icon];
	local_persist v2 *held_vert = 0;
	
	for (i32 i = 0; i < TERRAIN_TABLE_SIZE; i++)
	{
		v2 *vert = &GetRunData()->terrain[i];
		if (isfinite(vert->x))
		{
			b8 is_group_selected_vert = 0;
			for (i32 j = 0; j < selected_vert_count; j++)
			{
				if (selected_vertices[j] == vert)
				{
					is_group_selected_vert = 1;
				}
			}
			
			v4 tint = v4u(1.0);
			if (!is_drawing_terrain)
			{
				if (platform->left_mouse_down && held_vert == vert)
				{
					tint = v4(0.5f, 0.5f, 0.5f, 1.0f);
					
					if (is_group_selected_vert)
					{
						v2 diff = V2SubtractV2(GetMousePositionInWorldSpace(), *held_vert);
						for (i32 j = 0; j < selected_vert_count; j++)
						{
							*selected_vertices[j] = V2AddV2(*selected_vertices[j],
															diff);
						}
					}
					
					*held_vert = GetMousePositionInWorldSpace();
				}
				else
				{
					c2Shape vert_shape = middle_bounds;
					AddPositionOffsetToShape(&vert_shape, C2_SHAPE_TYPE_aabb, *vert);
					if (IsV2OverlappingShape(GetMousePositionInWorldSpace(),
											 vert_shape,
											 C2_SHAPE_TYPE_aabb))
					{
						tint = v4(0.8f, 0.8f, 0.8f, 1.0f);
						
						if (platform->left_mouse_pressed)
						{
							held_vert = vert;
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
			
			if (is_group_selected_vert)
			{
				tint = V4MultiplyV4(tint, v4(0.5f, 1.0f, 0.5f, 1.0f));
			}
			
			v2 r_pos = v2view(v2(vert->x - circle_size / 2.0f,
								 vert->y - circle_size / 2.0f));
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
	for (i32 step = 0; step < TERRAIN_TABLE_SIZE - 1; step++)
	{
		if (!bypass_inf && !isfinite(GetRunData()->terrain[step].x))
			break;
		
		for (i32 j = 0; j < TERRAIN_TABLE_SIZE - step - 1; j++)
		{
			if (!bypass_inf && !isfinite(GetRunData()->terrain[j].x))
				break;
			
			if (GetRunData()->terrain[j].x > GetRunData()->terrain[j + 1].x)
			{
				v2 temp = GetRunData()->terrain[j];
				GetRunData()->terrain[j] = GetRunData()->terrain[j + 1];
				GetRunData()->terrain[j + 1] = temp;
				
				if (held_vert == &GetRunData()->terrain[j])
				{
					held_vert = &GetRunData()->terrain[j + 1];
				}
				else if (held_vert == &GetRunData()->terrain[j + 1])
				{
					held_vert = &GetRunData()->terrain[j];
				}
			}
		}
	}
	
	v2 window_size = {300.0f, 400.0f};
	TsUIWindowBegin("Info", v4(core->render_w - window_size.x, 0.0f, window_size.x, window_size.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		char lbl[100];
		sprintf(lbl, "vert count: %i", selected_vert_count);
		TsUILabel(lbl);
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
}

internal void SetEjectedMode(b8 value)
{
	GetRunData()->is_ejected = value;
}

internal void UpdateEjectedMode()
{
	DrawGeneralEditor();
}

internal void UpdateTextNoteEntities()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_text_note);)
	{
		if (strcmp(entity->note, "") == 0)
		{
			Ts2dPushText(Ts2dGetDefaultFont(),
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1.0f, 1.0f, 1.0f, 1.0f),
						 v2view(entity->position),
						 core->camera_zoom / 10.0f,
						 "Empty Note");
		}
		else
		{
			Ts2dPushText(Ts2dGetDefaultFont(),
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1.0f, 1.0f, 1.0f, 1.0f),
						 v2view(entity->position),
						 core->camera_zoom / 10.0f,
						 entity->note);
		}
	}
}