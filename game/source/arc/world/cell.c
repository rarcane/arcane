internal void UpdateCells()
{
	/*
		// Cull inactive cells.
		for (i32 i = 0; i < core->run_data->dynamic_cell_count; i++)
		{
			Cell *cell = core->run_data->dynamic_cells[i];
			if (cell)
			{
				if (cell->material_type == CELL_MATERIAL_TYPE_air && cell->dynamic_properties.air.pressure < MINIMUM_AIR_PRESSURE)
				{
					DeleteCell(cell);
				}
			}
		}
	
		// Cell processing update.
		for (i32 i = 0; i < core->run_data->dynamic_cell_count; i++)
		{
			Cell *cell = core->run_data->dynamic_cells[i];
			if (cell)
			{
				Assert(core->run_data->dynamic_cells[cell->dynamic_id - 1] == cell,
							 "The cell's dynamic ID is mismatched with the array.");
				ProcessCell(cell);
			}
		}
	
		// Add in new dynamic cells.
		for (i32 i = 0; i < core->run_data->queued_dynamic_cell_count; i++)
		{
			if (core->run_data->queued_dynamic_cells[i])
			{
				MakeCellDynamic(core->run_data->queued_dynamic_cells[i]);
				core->run_data->queued_dynamic_cells[i] = 0;
			}
		}
		core->run_data->queued_dynamic_cell_count = 0;
	 */
	
	// Post cell process update.
	/* for (i32 i = 0; i < core->run_data->dynamic_cell_count; i++)
	{
		Cell *cell = core->run_data->dynamic_cells[i];
		if (cell)
		{
			CellMaterialTypeData *cell_type_data = &cell_material_type_data[cell->material_type];
			switch (cell_type_data->properties_type)
			{
			case CELL_PROPERTIES_TYPE_air:
				break;
			case CELL_PROPERTIES_TYPE_liquid:
			{
				cell->dynamic_properties.liquid.mass += cell->dynamic_properties.liquid.mass_adjustment;
				cell->dynamic_properties.liquid.mass_adjustment = 0.0f;
			}
			break;
			case CELL_PROPERTIES_TYPE_solid:
				break;
			default:
				R_TODO;
				break;
			}
		}
	} */
}

internal void ProcessCell(Cell *cell)
{
	/*
		QueueChunkForTextureUpdate(cell->parent_chunk);
		
		CellMaterialTypeData *cell_type_data = &cell_material_type_data[cell->material_type];
		switch (cell_type_data->properties_type)
		{
			case CELL_PROPERTIES_TYPE_air:
			ProcessAirCell(cell);
			break;
			case CELL_PROPERTIES_TYPE_liquid:
			ProcessLiquidCell(cell);
			break;
			case CELL_PROPERTIES_TYPE_solid:
			ProcessSolidCell(cell);
			break;
			default:
			R_TODO;
			break;
		}
	 */
}

internal void ProcessAirCell(Cell *cell)
{
	/*
		CellMaterialTypeData *cell_type_data = &cell_material_type_data[cell->material_type];
		StaticAirProperties *static_air_properties = &cell_type_data->static_properties.air;
		DynamicAirProperties *dynamic_air_properties = &cell->dynamic_properties.air;
		
		Cell *air_cells[5];
		i32 air_cell_count = 0;
		air_cells[air_cell_count++] = cell;
		
		Cell *neighbour_cell_l = GetCellAtPosition(cell->x_position - 1, cell->y_position);
		if (neighbour_cell_l->material_type == CELL_MATERIAL_TYPE_air)
			air_cells[air_cell_count++] = neighbour_cell_l;
		Cell *neighbour_cell_r = GetCellAtPosition(cell->x_position + 1, cell->y_position);
		if (neighbour_cell_r->material_type == CELL_MATERIAL_TYPE_air)
			air_cells[air_cell_count++] = neighbour_cell_r;
		Cell *neighbour_cell_t = GetCellAtPosition(cell->x_position, cell->y_position - 1);
		if (neighbour_cell_t->material_type == CELL_MATERIAL_TYPE_air)
			air_cells[air_cell_count++] = neighbour_cell_t;
		Cell *neighbour_cell_b = GetCellAtPosition(cell->x_position, cell->y_position + 1);
		if (neighbour_cell_b->material_type == CELL_MATERIAL_TYPE_air)
			air_cells[air_cell_count++] = neighbour_cell_b;
		
		f32 total_pressure = 0;
		for (i32 i = 0; i < air_cell_count; i++)
		{
			total_pressure += air_cells[i]->dynamic_properties.air.pressure;
		}
		
		if (total_pressure < MINIMUM_AIR_PRESSURE * air_cell_count)
		{
			for (i32 i = 0; i < air_cell_count; i++)
			{
				if (!IsCellEmpty(air_cells[i]))
					DeleteCell(air_cells[i]);
			}
		}
		else
		{
			for (i32 i = 0; i < air_cell_count; i++)
			{
				air_cells[i]->dynamic_properties.air.pressure = total_pressure / air_cell_count;
				QueueCellForDynamism(air_cells[i]);
			}
		}*/
}


internal void ProcessLiquidCell(Cell *cell)
{
	/*
		CellMaterialTypeData *cell_type_data = &cell_material_type_data[cell->material_type];
		StaticLiquidProperties *static_liquid_properties = &cell_type_data->static_properties.liquid;
		DynamicLiquidProperties *dynamic_liquid_properties = &cell->dynamic_properties.liquid;
		
		// TODO: Use fluid dynamics to model this. Just use a basic falling sand sim for now though.
		Cell *cell_below = GetCellAtPosition(cell->x_position, cell->y_position + 1);
		CellMaterialTypeData *cell_below_type_data = &cell_material_type_data[cell_below->material_type];
		if (cell_below_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
		{
			SwapCells(cell, cell_below);
			return;
		}
		
		i32 rand = RandomI32(0, 1);
		if (rand)
		{
			Cell *cell_below_left = GetCellAtPosition(cell->x_position - 1, cell->y_position + 1);
			CellMaterialTypeData *cell_below_left_type_data = &cell_material_type_data[cell_below_left->material_type];
			if (cell_below_left_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_below_left);
				return;
			}
			
			Cell *cell_below_right = GetCellAtPosition(cell->x_position + 1, cell->y_position + 1);
			CellMaterialTypeData *cell_below_right_type_data = &cell_material_type_data[cell_below_right->material_type];
			if (cell_below_right_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_below_right);
				return;
			}
			
			Cell *cell_left = GetCellAtPosition(cell->x_position - 1, cell->y_position);
			CellMaterialTypeData *cell_left_type_data = &cell_material_type_data[cell_left->material_type];
			if (cell_left_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_left);
				return;
			}
			
			Cell *cell_right = GetCellAtPosition(cell->x_position + 1, cell->y_position);
			CellMaterialTypeData *cell_right_type_data = &cell_material_type_data[cell_right->material_type];
			if (cell_right_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_right);
				return;
			}
		}
		else
		{
			Cell *cell_below_right = GetCellAtPosition(cell->x_position + 1, cell->y_position + 1);
			CellMaterialTypeData *cell_below_right_type_data = &cell_material_type_data[cell_below_right->material_type];
			if (cell_below_right_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_below_right);
				return;
			}
			
			Cell *cell_below_left = GetCellAtPosition(cell->x_position - 1, cell->y_position + 1);
			CellMaterialTypeData *cell_below_left_type_data = &cell_material_type_data[cell_below_left->material_type];
			if (cell_below_left_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_below_left);
				return;
			}
			
			Cell *cell_right = GetCellAtPosition(cell->x_position + 1, cell->y_position);
			CellMaterialTypeData *cell_right_type_data = &cell_material_type_data[cell_right->material_type];
			if (cell_right_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_right);
				return;
			}
			
			Cell *cell_left = GetCellAtPosition(cell->x_position - 1, cell->y_position);
			CellMaterialTypeData *cell_left_type_data = &cell_material_type_data[cell_left->material_type];
			if (cell_left_type_data->properties_type == CELL_PROPERTIES_TYPE_air)
			{
				SwapCells(cell, cell_left);
				return;
			}
		}
	 */
}

internal void ProcessSolidCell(Cell *cell)
{
	/*
		CellMaterialTypeData *cell_type_data = &cell_material_type_data[cell->material_type];
		StaticSolidProperties *static_solid_properties = &cell_type_data->static_properties.solid;
		DynamicSolidProperties *dynamic_solid_properties = &cell->dynamic_properties.solid;
		
		// TODO: Change this to a velocity-based move system. Keep it simple for now though.
		Cell *cell_below = GetCellAtPosition(cell->x_position, cell->y_position + 1);
		CellMaterialTypeData *cell_below_type_data = &cell_material_type_data[cell_below->material_type];
		switch (cell_below_type_data->properties_type)
		{
			case CELL_PROPERTIES_TYPE_air:
			{
				// Move solid down
				// TODO: displace air pressure
				SwapCells(cell, cell_below);
				
				break;
			}
			case CELL_PROPERTIES_TYPE_liquid:
			{
				break;
			}
			case CELL_PROPERTIES_TYPE_solid:
			{
				break;
			}
			default:
			R_TODO;
			break;
		}
	 */
}

internal void QueueChunkForTextureUpdate(Chunk *chunk)
{
	for (i32 i = 0; i < core->run_data->chunk_texture_update_queue_count; i++)
	{
		Chunk *existing_chunk = core->run_data->chunk_texture_update_queue[i];
		if (existing_chunk == chunk)
		{
			return;
		}
	}
	
	core->run_data->chunk_texture_update_queue[core->run_data->chunk_texture_update_queue_count++] = chunk;
}

internal void RenderCells()
{
	// NOTE(randy): Might be an issue here, if a chunk is queued but gets unloaded.
	
	// Update the textures of queued chunks
	for (i32 i = 0; i < core->run_data->chunk_texture_update_queue_count; i++)
	{
		Chunk *chunk = core->run_data->chunk_texture_update_queue[i];
		unsigned char pixel_data[CHUNK_SIZE * CHUNK_SIZE * 4];
		unsigned char *pixel_buffer = &pixel_data[0];
		
		for (i32 y = 0; y < CHUNK_SIZE; y++)
		{
			for (i32 x = 0; x < CHUNK_SIZE; x++)
			{
				Cell *cell = &chunk->cells[y][x];
				v4 colour = v4u(0.0f);
				switch (cell->material_type)
				{
					case CELL_MATERIAL_TYPE_air:
					{
						DynamicAirProperties *dyn_air_properties = &cell->dynamic_properties.air;
						
						f32 pressure = dyn_air_properties->pressure;
						pressure = ClampF32(pressure, 0.0f, 50.0f);
						
						if (pressure > 0.0f)
						{
							if (core->run_data->editor_flags & EDITOR_FLAGS_debug_cell_view)
								colour = v4(pressure / 50.0f, 0.0f, 0.0f, 1.0f);
							else
								colour = V4MultiplyF32(v4u(1.0f), pressure / 10.0f);
						}
						
						break;
					}
					case CELL_MATERIAL_TYPE_dirt:
					{
						v3 dirt_colour = {14.0f / 360.0f, 0.36f, 0.31f};
						
						if (cell->dynamic_properties.solid.hardness > 0.0f)
						{
							v3 col = HSVToRGB(v3(dirt_colour.x, dirt_colour.y - 0.1f, dirt_colour.z - 0.05f));
							colour = v4(col.r, col.g, col.b, 1.0f);
						}
						else
						{
							v3 col = HSVToRGB(v3(dirt_colour.x, dirt_colour.y, dirt_colour.z));
							colour = v4(col.r, col.g, col.b, 1.0f);
						}
						
						break;
					}
					case CELL_MATERIAL_TYPE_sand:
					{
						v3 sand_colour = {55.0f / 360.0f, 0.43f, 1.0f};
						
						if (cell->dynamic_properties.solid.hardness > 0.0f)
						{
							v3 col = HSVToRGB(v3(sand_colour.x, sand_colour.y - 0.1f, sand_colour.z - 0.05f));
							colour = v4(col.r, col.g, col.b, 1.0f);
						}
						else
						{
							v3 col = HSVToRGB(v3(sand_colour.x, sand_colour.y, sand_colour.z));
							colour = v4(col.r, col.g, col.b, 1.0f);
						}
						
						break;
					}
					case CELL_MATERIAL_TYPE_water:
					{
						if (core->run_data->editor_flags & EDITOR_FLAGS_debug_cell_view)
						{
							v3 water_colour = {218.0f / 360.0f, 0.58f, 0.91f};
							f32 mass = cell->dynamic_properties.liquid.mass;
							
							v3 col = HSVToRGB(v3(water_colour.x, water_colour.y, water_colour.z));
							colour = v4(LerpF32(1.0f - mass, col.r, 1.0f), LerpF32(1.0f - mass, col.g, 1.0f), LerpF32(1.0f - mass, col.b, 1.0f), 1.0f);
						}
						else
						{
							v3 water_colour = {218.0f / 360.0f, 0.58f, 0.91f};
							f32 mass = cell->dynamic_properties.liquid.mass;
							
							v3 col = HSVToRGB(v3(water_colour.x, water_colour.y, water_colour.z));
							colour = v4(col.r, col.g, col.b, 1.0f);
							colour = V4MultiplyF32(colour, 0.7f * mass);
						}
						break;
					}
					default:
					colour = v4u(1.0f);
					break;
				}
				
				/*
								if (core->run_data->selected_cell &&
									core->run_data->selected_cell->parent_chunk == chunk &&
									CellPositionToIndex(core->run_data->selected_cell->x_position) == x &&
									CellPositionToIndex(core->run_data->selected_cell->y_position) == y)
								{
									colour = v4(0.5f, 0.0f, 0.0f, 0.5f);
								}
								else if (cell->dynamic_id && !(cell->material_type == CELL_MATERIAL_TYPE_air || cell->material_type == CELL_MATERIAL_TYPE_water))
								{
									colour = V4MultiplyV4(colour, v4u(0.75f));
								}
				 */
				
				*pixel_buffer = (unsigned char)(ClampF32(colour.r, 0.0f, 1.0f) * 255.0f);
				pixel_buffer++;
				*pixel_buffer = (unsigned char)(ClampF32(colour.g, 0.0f, 1.0f) * 255.0f);
				pixel_buffer++;
				*pixel_buffer = (unsigned char)(ClampF32(colour.b, 0.0f, 1.0f) * 255.0f);
				pixel_buffer++;
				*pixel_buffer = (unsigned char)(ClampF32(colour.a, 0.0f, 1.0f) * 255.0f);
				pixel_buffer++;
			}
		}
		
		chunk->texture = Ts2dTextureInit(TS2D_TEXTURE_FORMAT_R8G8B8A8,
										 CHUNK_SIZE,
										 CHUNK_SIZE,
										 pixel_data);
	}
	
	// Push existing chunk textures
	for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
		{
			v2 render_pos = v2view(v2((f32)chunk->x_index * CHUNK_SIZE,
									  (f32)chunk->y_index * CHUNK_SIZE));
			v2 render_size = v2zoom(v2(CHUNK_SIZE, CHUNK_SIZE));
			
			Ts2dPushTexture(&chunk->texture,
							v4(0.0f, 0.0f, CHUNK_SIZE, CHUNK_SIZE),
							v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
		}
	}
	
	core->run_data->chunk_texture_update_queue_count = 0;
	
#ifdef DEVELOPER_TOOLS
	if (core->run_data->editor_state == EDITOR_STATE_terrain)
	{
		v2 cell_selection = V2SubtractV2(core->run_data->selection_end, core->run_data->selection_start);
		if (cell_selection.x != 0.0f && cell_selection.y != 0.0f)
		{
			v2 pos = v2view(core->run_data->selection_start);
			v2 size = v2zoom(cell_selection);
			Ts2dPushFilledRect(v4(0.5f, 0.0f, 0.0f, 0.5f), v4(pos.x, pos.y, size.x, size.y));
		}
	}
#endif
}

internal i32 CellPositionToIndex(i32 pos)
{
	i32 index;
	if (pos >= 0)
		index = pos % CHUNK_SIZE;
	else
	{
		if (abs(pos) % CHUNK_SIZE == 0)
			index = 0;
		else
			index = CHUNK_SIZE - abs(pos) % CHUNK_SIZE;
	}
	
	return index;
}

internal Cell *GetCellAtPosition(i32 x, i32 y)
{
	Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex((f32)x), WorldspaceToChunkIndex((f32)y));
	if (!chunk)
		return 0;
	
	return &chunk->cells[CellPositionToIndex(y)][CellPositionToIndex(x)];
}

internal b8 IsCellEmpty(Cell *cell)
{
	return (cell->material_type == CELL_MATERIAL_TYPE_air && cell->dynamic_properties.air.pressure == 0.0f);
}

internal void SwapCells(Cell *cell_1, Cell *cell_2)
{
	/*
		Cell cell_1_copy = *cell_1;
		
		// Need to update the cells with new addresses if they're dynamic.
		if (cell_1->dynamic_id)
			core->run_data->dynamic_cells[cell_1->dynamic_id - 1] = cell_2;
		if (cell_2->dynamic_id)
			core->run_data->dynamic_cells[cell_2->dynamic_id - 1] = cell_1;
		
		cell_1->dynamic_id = cell_2->dynamic_id;
		cell_1->material_type = cell_2->material_type;
		cell_1->dynamic_properties = cell_2->dynamic_properties;
		
		cell_2->dynamic_id = cell_1_copy.dynamic_id;
		cell_2->material_type = cell_1_copy.material_type;
		cell_2->dynamic_properties = cell_1_copy.dynamic_properties;
		
		QueueChunkForTextureUpdate(cell_1->parent_chunk);
		QueueChunkForTextureUpdate(cell_2->parent_chunk);
	 */
}

internal void QueueCellForDynamism(Cell *cell)
{
	/*
		Assert(cell, "Invalid cell.");
		
		if (cell->dynamic_id)
			return;
		
		Assert(core->run_data->queued_dynamic_cell_count + 1 < MAX_DYNAMIC_CELLS, "Max reached.");
		
		for (i32 i = 0; i < core->run_data->queued_dynamic_cell_count; i++)
		{
			if (core->run_data->queued_dynamic_cells[i] == cell)
				return;
		}
		
		core->run_data->queued_dynamic_cells[core->run_data->queued_dynamic_cell_count++] = cell;
	 */
}

internal void MakeCellDynamic(Cell *cell)
{
	/*
		Assert(!cell->dynamic_id, "Already dynamic.");
		Assert(core->run_data->free_dynamic_cell_id != 0, "Max dynamic cells reached.");
		
		i32 new_id = core->run_data->free_dynamic_cell_id;
		
		core->run_data->dynamic_cells[new_id - 1] = cell;
		cell->dynamic_id = new_id;
		
		if (core->run_data->dynamic_cell_count == core->run_data->free_dynamic_cell_id - 1)
		{
			core->run_data->dynamic_cell_count++;
			core->run_data->free_dynamic_cell_id++;
		}
		
		if (core->run_data->dynamic_cell_count < MAX_DYNAMIC_CELLS)
		{
			// If the free ID isn't at the current count position, find out where it should be
			if (core->run_data->dynamic_cell_count != core->run_data->free_dynamic_cell_id - 1)
			{
				b8 found = 0;
				for (i32 i = 0; i < core->run_data->dynamic_cell_count + 1; i++)
				{
					if (!core->run_data->dynamic_cells[i])
					{
						core->run_data->free_dynamic_cell_id = i + 1;
						found = 1;
						break;
					}
				}
				Assert(found, "Couldn't find a free ID?");
			}
		}
		else
		{
			// Count has gone past the max, it's now full.
			core->run_data->free_dynamic_cell_id = 0;
		}
	 */
}

internal void RemoveCellDynamism(Cell *cell)
{
	/*
		Assert(cell->dynamic_id, "Cell is already not dynamic.");
		Assert(core->run_data->dynamic_cells[cell->dynamic_id - 1] == cell, "Mismatched cell with ID??");
		
		// Remove from the dynamic cell from the array.
		core->run_data->dynamic_cells[cell->dynamic_id - 1] = 0;
		if (cell->dynamic_id < core->run_data->free_dynamic_cell_id)
			core->run_data->free_dynamic_cell_id = cell->dynamic_id;
		
		cell->dynamic_id = 0;
		
		// Shrink array size
		for (i32 i = 0; i < core->run_data->dynamic_cell_count; i++)
		{
			i32 inv = core->run_data->dynamic_cell_count - 1 - i;
			if (core->run_data->dynamic_cells[inv])
			{
				core->run_data->dynamic_cell_count = inv + 1;
				return;
			}
		}
		core->run_data->dynamic_cell_count = 0;
	 */
}

internal void DeleteCell(Cell *cell)
{
	/*
		Assert(!IsCellEmpty(cell), "Cell is already empty.");
		
		if (cell->dynamic_id)
			RemoveCellDynamism(cell);
		
		// Remove from dynamic queue if it's in it.
		for (i32 i = 0; i < core->run_data->queued_dynamic_cell_count; i++)
		{
			if (core->run_data->queued_dynamic_cells[i] == cell)
			{
				core->run_data->queued_dynamic_cells[i] = 0;
				break;
			}
		}
		
		// Clear cell to room temp
		cell->material_type = CELL_MATERIAL_TYPE_air;
		DynamicCellProperties empty = {0};
		cell->dynamic_properties = empty;
		
		QueueChunkForTextureUpdate(cell->parent_chunk);
	 */
}

internal b8 ShouldCellHarden(Cell *cell)
{
	/*
		CellMaterialTypeData *material_type_data = &cell_material_type_data[cell->material_type];
		if (material_type_data->properties_type == CELL_PROPERTIES_TYPE_solid)
		{
			i32 distance_from_air_above = 0;
			for (i32 i = 1; i <= material_type_data->static_properties.solid.crust_depth; i++)
			{
				Cell *neighbour_cell = GetCellAtPosition(cell->x_position, cell->y_position - i);
				if (neighbour_cell->material_type != cell->material_type)
				{
					distance_from_air_above = i;
					break;
				}
			}
			
			if (distance_from_air_above)
			{
				b8 is_support_below = 0;
				for (i32 i = 1; i <= material_type_data->static_properties.solid.crust_depth + 1 - distance_from_air_above; i++)
				{
					Cell *neighbour_cell = GetCellAtPosition(cell->x_position, cell->y_position + i);
					if (neighbour_cell->material_type == cell->material_type)
					{
						is_support_below = 1;
						break;
					}
				}
				
				return !is_support_below;
			}
			else
			{
				return 1;
			}
		}
		else
			return 0;
	 */
	
	return 1;
}

/* f32 GetStableFlowState(f32 total_mass)
{
	if (total_mass <= MAX_LIQUID_MASS)
		return MAX_LIQUID_MASS;
	else if (total_mass < 2 * MAX_LIQUID_MASS + LIQUID_RESOLUTION)
		return (MAX_LIQUID_MASS * MAX_LIQUID_MASS + total_mass * LIQUID_RESOLUTION) / (MAX_LIQUID_MASS + LIQUID_RESOLUTION);
	else
		return (total_mass + LIQUID_RESOLUTION) / 2.0f;
} */