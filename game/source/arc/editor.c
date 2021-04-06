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
					WriteInitialMapData();
					
					char dest[200] = "";
					sprintf(dest, "%s..\\..\\res\\initial_map", core->res_path);
					char source[200] = "";
					sprintf(source, "%s/initial_map", core->res_path);
					platform->CopyDirectoryRecursively(dest, source);
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
				if (TsUIToggler("Draw World", core->run_data->debug_flags & DEBUG_FLAGS_draw_world))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_world;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_world;
				
				if (TsUIToggler("Draw Colliders", core->run_data->debug_flags & DEBUG_FLAGS_draw_collision))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_collision;
				
				if (TsUIToggler("Draw Chunk Grid", core->run_data->debug_flags & DEBUG_FLAGS_draw_chunk_grid))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_chunk_grid;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_chunk_grid;
				
				if (TsUIToggler("Debug Cell View", core->run_data->debug_flags & DEBUG_FLAGS_debug_cell_view))
					core->run_data->debug_flags |= DEBUG_FLAGS_debug_cell_view;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_debug_cell_view;
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
		
		//~Chunk editor
		case EDITOR_STATE_chunk :
		{
			UpdateChunkEditor();
			
			/*
						v2 world_info_window_size = { 300.0f, 300.0f };
						v2 world_info_window_pos = {core->render_w - world_info_window_size.x - 10.0f, 10.0f};
						TsUIWindowBegin("World Info", v4(world_info_window_pos.x, world_info_window_pos.y, world_info_window_size.x, world_info_window_size.y), 0, 0);
						{
							TsUIPushColumn(v2(10, 10), v2(150, 30));
							TsUIPushWidth(270.0f);
							
							{
								char label[20];
								sprintf(label, "Active Chunks: %i", core->run_data->active_chunk_count);
								TsUILabel(label);
							}
							
							// NOTE(randy): Make a new chunk selection
							if (platform->left_mouse_pressed)
							{
								v2 click_pos = GetMousePositionInWorldSpace();
								core->run_data->chunk_editor.is_chunk_selected = 1;
								core->run_data->chunk_editor.selected_chunk.x_index = WorldSpaceToChunkIndex(click_pos.x);
								core->run_data->chunk_editor.selected_chunk.y_index = WorldSpaceToChunkIndex(click_pos.y);
							}
							
							TsUIPopColumn();
							TsUIPopWidth();
						}
						TsUIWindowEnd();
						
						v2 selected_chunk_window_size = { 300.0f, 400.0f };
						v2 selected_chunk_window_pos = {world_info_window_pos.x, world_info_window_pos.y + world_info_window_size.y + 10.0f};
						TsUIWindowBegin("Selected Chunk", v4(selected_chunk_window_pos.x, selected_chunk_window_pos.y, selected_chunk_window_size.x, selected_chunk_window_size.y), 0, 0);
						{
							TsUIPushColumn(v2(10, 10), v2(150, 30));
							TsUIPushWidth(270.0f);
							
							if (core->run_data->chunk_editor.is_chunk_selected)
							{
								Chunk *chunk = GetChunkAtIndex(core->run_data->chunk_editor.selected_chunk.x_index, core->run_data->chunk_editor.selected_chunk.y_index);
								if (chunk)
								{
									{
										char label[20];
										sprintf(label, "%i, %i", chunk->x_index, chunk->y_index);
										TsUITitle(label);
									}
									TsUIDivider();
									{
										char label[20];
										sprintf(label, "entity_count: %i", chunk->entity_count);
										TsUILabel(label);
									}
								}
								else
								{
									// NOTE(randy): The selected chunk isn't loaded in.
									{
										char label[20];
										sprintf(label, "%i, %i (unloaded)", core->run_data->chunk_editor.selected_chunk.x_index, core->run_data->chunk_editor.selected_chunk.y_index);
										TsUITitle(label);
									}
									TsUIDivider();
									
									// TODO(randy): Option to force load the chunk in?
								}
							}
							else
							{
								TsUILabel("No chunk selected.");
							}
							
							TsUIPopColumn();
							TsUIPopWidth();
						}
						TsUIWindowEnd();
			 */
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
		// NOTE(randy): Cache the no-state debug flags
		if (core->run_data->editor_state == EDITOR_STATE_none)
		{
			core->run_data->saved_debug_flags = core->run_data->debug_flags;
		}
		
		core->run_data->debug_flags = core->run_data->saved_debug_flags;
		
		// NOTE(randy): Set state specific debug flags
		switch (editor_state)
		{
			case EDITOR_STATE_map :
			{
				
			} break;
			
			case EDITOR_STATE_chunk :
			{
				core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
				core->run_data->debug_flags |= DEBUG_FLAGS_draw_chunk_grid; // TODO(randy): Need to separate this flag out from the collision draw
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
	TsUIWindowBegin("Entity Library", v4(0.0f, 10.0f, window_size.x, window_size.y), 0, 0);
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

internal void UpdateChunkEditor()
{
	v2 window_size_a = {300.0f, 400.0f};
	TsUIWindowBegin("Chunk List", v4(0.0f, 40.0f, window_size_a.x, window_size_a.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
		{
			Chunk *chunk = &GetRunData()->active_chunks[i];
			
			char lbl[100];
			sprintf(lbl, "%i, %i", chunk->pos.x, chunk->pos.y);
			TsUILabel(lbl);
		}
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	v2 window_size_b = {300.0f, 400.0f};
	TsUIWindowBegin("Selected Chunk", v4(0.0f, window_size_a.y + 50.0f, window_size_b.x, window_size_b.y), 0, 0);
	{
		TsUIPushColumn(v2(10, 10), v2(150, 30));
		TsUIPushWidth(270.0f);
		
		
		
		TsUIPopWidth();
		TsUIPopColumn();
	}
	TsUIWindowEnd();
}