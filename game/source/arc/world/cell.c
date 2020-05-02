internal void UpdateCells()
{
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
			R_DEV_ASSERT(core->run_data->dynamic_cells[cell->dynamic_id - 1] == cell,
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
}

internal void ProcessAirCell(Cell *cell)
{
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
	}
}

internal void ProcessLiquidCell(Cell *cell)
{
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
}

internal void ProcessSolidCell(Cell *cell)
{
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
						colour = v4(pressure / 50.0f, 0.0f, 0.0f, 1.0f);
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

		v2 render_pos = v2view(v2((f32)chunk->x_index * CHUNK_SIZE,
								  (f32)chunk->y_index * CHUNK_SIZE));
		v2 render_size = v2zoom(v2(CHUNK_SIZE, CHUNK_SIZE));

		Ts2dPushTexture(&chunk->texture,
						v4(0.0f, 0.0f, CHUNK_SIZE, CHUNK_SIZE),
						v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
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
}

internal void QueueCellForDynamism(Cell *cell)
{
	R_DEV_ASSERT(cell, "Invalid cell.");

	if (cell->dynamic_id)
		return;

	R_DEV_ASSERT(core->run_data->queued_dynamic_cell_count + 1 < MAX_DYNAMIC_CELLS, "Max reached.");

	for (i32 i = 0; i < core->run_data->queued_dynamic_cell_count; i++)
	{
		if (core->run_data->queued_dynamic_cells[i] == cell)
			return;
	}

	core->run_data->queued_dynamic_cells[core->run_data->queued_dynamic_cell_count++] = cell;
}

internal void MakeCellDynamic(Cell *cell)
{
	R_DEV_ASSERT(!cell->dynamic_id, "Already dynamic.");
	R_DEV_ASSERT(core->run_data->free_dynamic_cell_id != 0, "Max dynamic cells reached.");

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
			R_DEV_ASSERT(found, "Couldn't find a free ID?");
		}
	}
	else
	{
		// Count has gone past the max, it's now full.
		core->run_data->free_dynamic_cell_id = 0;
	}
}

internal void RemoveCellDynamism(Cell *cell)
{
	R_DEV_ASSERT(cell->dynamic_id, "Cell is already not dynamic.");
	R_DEV_ASSERT(core->run_data->dynamic_cells[cell->dynamic_id - 1] == cell, "Mismatched cell with ID??");

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
}

internal void DeleteCell(Cell *cell)
{
	R_DEV_ASSERT(!IsCellEmpty(cell), "Cell is already empty.");

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
}

internal b8 ShouldCellHarden(Cell *cell)
{
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
}

/* f32 GetDownwardFlow(f32 origin_mass, f32 below_mass)
{
	if (below_mass >= MAX_LIQUID_MASS)
	{
		
	}
} */

/* f32 GetStableFlowState(f32 total_mass)
{
	if (total_mass <= MAX_LIQUID_MASS)
		return MAX_LIQUID_MASS;
	else if (total_mass < 2 * MAX_LIQUID_MASS + LIQUID_RESOLUTION)
		return (MAX_LIQUID_MASS * MAX_LIQUID_MASS + total_mass * LIQUID_RESOLUTION) / (MAX_LIQUID_MASS + LIQUID_RESOLUTION);
	else
		return (total_mass + LIQUID_RESOLUTION) / 2.0f;
} */

/*
internal CellMaterial *NewCellMaterial(Cell *cell, CellMaterialType material_type)
{
	ChunkData *chunk = cell->parent_cell_chunk->parent_chunk;
	R_DEV_ASSERT(!cell->material, "There's already a material in this cell.");
	R_DEV_ASSERT(chunk->free_cell_material_id != 0, "No free cell materials in chunk.");

	i32 new_id = chunk->free_cell_material_id;
	if (chunk->cell_material_count == chunk->free_cell_material_id - 1)
	{
		chunk->cell_material_count++;
		chunk->free_cell_material_id++;
	}

	CellMaterial *cell_material = &chunk->cell_materials[new_id - 1];
	cell_material->id = new_id;
	cell_material->parent_cell = cell;
	cell_material->material_type = material_type;
	cell_material->mass = cell_material_type_data[material_type].default_mass;
	cell_material->properties_type = cell_material_type_data[material_type].properties_type;

	if (cell_material->properties_type == CELL_PROPERTIES_TYPE_solid)
	{
		cell_material->properties.solid.position = v2(0.5f, 0.5f);
	}
	else if (cell_material->properties_type == CELL_PROPERTIES_TYPE_fluid)
	{
		// defaults
	}

	cell->material = cell_material;
	AddCellChunkToUpdateQueue(cell->parent_cell_chunk);

	if (chunk->cell_material_count != chunk->free_cell_material_id - 1)
	{
		R_DEV_ASSERT(chunk->cell_material_count + 1 < CHUNK_SIZE * CHUNK_SIZE, "What would happen here.");

		// Free index would have been used. Find the next one.
		b8 found = 0;
		for (i32 i = 0; i < chunk->cell_material_count + 1; i++)
		{
			if (chunk->cell_materials[i].id == 0)
			{
				chunk->free_cell_material_id = i + 1;
				found = 1;
				break;
			}
		}
		R_DEV_ASSERT(found, "Couldn't find a free index?");
	}

	return cell_material;
}

internal void DeleteMaterialFromChunk(ChunkData *chunk, CellMaterial *material)
{
	for (i32 i = 0; i < chunk->cell_material_count; i++)
	{
		CellMaterial *existing = &chunk->cell_materials[i];
		if (existing == material)
		{
			AddCellChunkToUpdateQueue(material->parent_cell->parent_cell_chunk);

			material->parent_cell->material = 0;

			CellMaterial empty = {0};
			chunk->cell_materials[i] = empty;

			if (i + 1 < chunk->free_cell_material_id)
				chunk->free_cell_material_id = i + 1;

			break;
		}
	}
}

internal CellMaterial *MoveMaterialToCell(CellMaterial *material, Cell *new_cell)
{
	Cell *original_cell = material->parent_cell;
	original_cell->material = 0;

	AddCellChunkToUpdateQueue(original_cell->parent_cell_chunk);

	if (original_cell->parent_cell_chunk->parent_chunk != new_cell->parent_cell_chunk->parent_chunk)
	{
		// Move across chunk
		CellMaterial material_data = *material;
		DeleteMaterialFromChunk(original_cell->parent_cell_chunk->parent_chunk, material);
		CellMaterial *new_material = NewCellMaterial(new_cell, material_data.material_type);
		i32 new_id = new_material->id;
		*new_material = material_data;

		new_material->parent_cell = new_cell;
		new_material->id = new_id;
		new_cell->material = new_material;

		return new_material;
	}
	else
	{
		material->parent_cell = new_cell;
		new_cell->material = material;
		return material;
	}
}

internal CellChunk *GetCellChunkAtPosition(i32 x, i32 y)
{
	ChunkData *chunk = GetChunkAtPosition(v2((f32)x, (f32)y));

	i32 cell_x;
	if (x >= 0)
		cell_x = x % CHUNK_SIZE;
	else
	{
		if (abs(x) % CHUNK_SIZE == 0)
			cell_x = 0;
		else
			cell_x = CHUNK_SIZE - abs(x) % CHUNK_SIZE;
	}

	i32 cell_y;
	if (y >= 0)
		cell_y = y % CHUNK_SIZE;
	else
	{
		if (abs(y) % CHUNK_SIZE == 0)
			cell_y = 0;
		else
			cell_y = CHUNK_SIZE - abs(y) % CHUNK_SIZE;
	}

	i32 cell_chunk_x = (i32)floorf((f32)cell_x / (f32)CELL_CHUNK_SIZE);
	i32 cell_chunk_y = (i32)floorf((f32)cell_y / (f32)CELL_CHUNK_SIZE);

	return &chunk->cell_chunks[cell_chunk_y][cell_chunk_x];
}

internal Cell *GetCellAtPosition(i32 x, i32 y)
{
	CellChunk *cell_chunk = GetCellChunkAtPosition(x, y);

	i32 cell_x;
	if (x >= 0)
		cell_x = x % CELL_CHUNK_SIZE;
	else
	{
		if (abs(x) % CELL_CHUNK_SIZE == 0)
			cell_x = 0;
		else
			cell_x = CELL_CHUNK_SIZE - abs(x) % CELL_CHUNK_SIZE;
	}

	i32 cell_y;
	if (y >= 0)
		cell_y = y % CELL_CHUNK_SIZE;
	else
	{
		if (abs(y) % CELL_CHUNK_SIZE == 0)
			cell_y = 0;
		else
			cell_y = CELL_CHUNK_SIZE - abs(y) % CELL_CHUNK_SIZE;
	}

	return &cell_chunk->cells[cell_y][cell_x];
}

internal Cell *GetCellAtRelativePosition(Cell *relative_cell, i32 x, i32 y)
{
	return GetCellAtPosition(relative_cell->parent_cell_chunk->parent_chunk->x_index * CHUNK_SIZE + relative_cell->parent_cell_chunk->x_index * CELL_CHUNK_SIZE + relative_cell->x_index + x,
							 relative_cell->parent_cell_chunk->parent_chunk->y_index * CHUNK_SIZE + relative_cell->parent_cell_chunk->y_index * CELL_CHUNK_SIZE + relative_cell->y_index + y);
}

internal void AddCellChunkToUpdateQueue(CellChunk *cell_chunk)
{
	for (i32 i = 0; i < core->queued_cell_chunk_count; i++)
	{
		CellChunk *existing = core->queued_cell_chunks_for_update[i];
		if (existing == cell_chunk)
			return;
	}

	R_DEV_ASSERT(core->queued_cell_chunk_count + 1 < 512, "Too many cell chunks queued for update.");
	core->queued_cell_chunks_for_update[core->queued_cell_chunk_count++] = cell_chunk;
}

internal void MakeMaterialDynamic(CellMaterial *material)
{
	R_DEV_ASSERT(!material->is_material_dynamic, "Material is already dynamic.");
	R_DEV_ASSERT(material->parent_cell->parent_cell_chunk->parent_chunk->dynamic_cell_material_count + 1 < MAX_DYNAMIC_CELLS, "Too many dynamic cells.");
	material->is_material_dynamic = 1;
	material->parent_cell->parent_cell_chunk->parent_chunk->dynamic_cell_materials[material->parent_cell->parent_cell_chunk->parent_chunk->dynamic_cell_material_count++] = material;
}

internal b8 ShouldMaterialHarden(CellMaterial *material)
{
	Cell *cell = material->parent_cell;
	CellMaterialTypeData *material_type_data = &cell_material_type_data[material->material_type];

	i32 distance_from_air_above = 0;
	for (i32 i = 1; i <= material_type_data->crust_depth; i++)
	{
		Cell *neighbour_cell = GetCellAtRelativePosition(cell, 0, -i);
		if (!neighbour_cell->material)
		{
			distance_from_air_above = i;
			break;
		}
	}

	if (distance_from_air_above)
	{
		b8 is_support_below = 0;
		for (i32 i = 1; i <= material_type_data->crust_depth + 1 - distance_from_air_above; i++)
		{
			Cell *neighbour_cell = GetCellAtRelativePosition(cell, 0, i);
			if (neighbour_cell->material)
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

f32 GetStableFlowState(f32 total_mass)
{
	if (total_mass <= 1.0f)
		return 1.0f;
	else if (total_mass < 2 * 1.0f + FLUID_COMPRESSION)
		return (1.0f + total_mass * FLUID_COMPRESSION) / (1.0f + FLUID_COMPRESSION);
	else
		return (total_mass + FLUID_COMPRESSION) / 2.0f;
}

internal void ProcessCellMaterial(CellMaterial *material)
{
	Cell *cell = material->parent_cell;

	b8 no_longer_dynamic = 0;

	R_DEV_ASSERT(!material->has_been_updated, "Cell contents have already been updated.");
	R_DEV_ASSERT(material->id != 0, "Invalid material.");

	switch (material->properties_type)
	{
	case CELL_PROPERTIES_TYPE_solid:
	{
		SolidMaterial *solid_material_properties = &material->properties.solid;

		if (solid_material_properties->velocity.x != 0.0f || solid_material_properties->velocity.y != 0.0f)
		{
			v2 next_position = V2AddV2(solid_material_properties->position, V2MultiplyF32(solid_material_properties->velocity, core->world_delta_t));

			i32 x_cell_steps = (i32)floorf(next_position.x);
			i32 y_cell_steps = (i32)floorf(next_position.y);
			i32 x_is_larger = abs(x_cell_steps) >= abs(y_cell_steps);

			f32 integration_magnitude = SquareRoot((f32)x_cell_steps * (f32)x_cell_steps + (f32)y_cell_steps * (f32)y_cell_steps);
			v2 integration_normal = v2(x_cell_steps / integration_magnitude, y_cell_steps / integration_magnitude);

			if (abs(x_cell_steps) > 0 || abs(y_cell_steps) > 0)
			{
				b8 break_loop = 0;
				for (i32 j = 1; j < (x_is_larger ? abs(x_cell_steps) : abs(y_cell_steps)) + 1; j++)
				{
					if (break_loop)
						break;

					// TODO: Crossing into different chunks
					Cell *next_cell = GetCellAtRelativePosition(cell, GetSign((f32)x_cell_steps), GetSign((f32)y_cell_steps));

					// Process the material in the next cell if it's got velocity
					if (next_cell->material &&
						(next_cell->material->properties.solid.velocity.x != 0.0f || next_cell->material->properties.solid.velocity.y != 0.0f) &&
						!next_cell->material->has_been_updated)
					{
						material->has_been_updated = 1;
						ProcessCellMaterial(next_cell->material);
						material->has_been_updated = 0;

						next_cell = GetCellAtRelativePosition(cell, GetSign((f32)x_cell_steps), GetSign((f32)y_cell_steps));
					}

					CellMaterial *collided_material = next_cell->material;
					if (!collided_material)
					{
						// Next cell is empty, so just move the material into it
						material = MoveMaterialToCell(material, next_cell);
						cell = next_cell;

						if (next_position.x >= 0.0f)
							solid_material_properties->position.x = next_position.x - x_cell_steps;
						else
							solid_material_properties->position.x = fabsf(next_position.x - x_cell_steps);

						if (next_position.y >= 0.0f)
							solid_material_properties->position.y = next_position.y - y_cell_steps;
						else
							solid_material_properties->position.y = fabsf(next_position.y - y_cell_steps);
					}
					else
					{
						CellMaterialTypeData *material_data = &cell_material_type_data[material->material_type];
						CellMaterialTypeData *collided_material_data = &cell_material_type_data[collided_material->material_type];

						switch (collided_material->properties_type)
						{
						case CELL_PROPERTIES_TYPE_solid: // Solid on solid
						{
							SolidMaterial *collided_solid_material_properties = &collided_material->properties.solid;

							i32 distance = 1;
							b8 is_at_ground = 0;
							while (1)
							{
								Cell *cell_below = GetCellAtRelativePosition(material->parent_cell, 0, distance);
								if (cell_below->material)
								{
									if (cell_below->material->mass == 0.0f)
									{
										is_at_ground = 1;
										break;
									}
								}
								else
								{
									break;
								}

								distance++;
							}

							if (material_data->max_height == 1 && integration_normal.y > 0.0f)
							{
								Cell *below = GetCellAtRelativePosition(cell, 0, 1);
								R_DEV_ASSERT(below->material, "uhh");

								Cell *below_l = GetCellAtRelativePosition(cell, -1, 1);
								Cell *below_r = GetCellAtRelativePosition(cell, 1, 1);

								if (!below_l->material)
								{
									material = MoveMaterialToCell(material, below_l);
									cell = below_l;
									break_loop = 1;
								}
								else if (!below_r->material)
								{
									material = MoveMaterialToCell(material, below_r);
									cell = below_r;
									break_loop = 1;
								}
							}

							// Exchange immpulses
							v2 relative_velocity = V2SubtractV2(collided_solid_material_properties->velocity, solid_material_properties->velocity);
							f32 velocity_along_normal = relative_velocity.x * integration_normal.x + relative_velocity.y * integration_normal.y;

							f32 restitution = MinimumF32(material_data->restitution, collided_material_data->restitution);

							f32 a_inv_mass;
							if (material->mass == 0)
								a_inv_mass = 0;
							else
								a_inv_mass = 1.0f / material->mass;

							f32 b_inv_mass;
							if (collided_material->mass == 0)
								b_inv_mass = 0;
							else
								b_inv_mass = 1.0f / collided_material->mass;

							f32 impulse_scalar = -(1 + restitution) * velocity_along_normal;
							impulse_scalar = impulse_scalar / (a_inv_mass + b_inv_mass);

							v2 impulse = V2MultiplyF32(integration_normal, impulse_scalar);

							v2 impulse_a = V2MultiplyF32(impulse, a_inv_mass);
							solid_material_properties->velocity.x -= impulse_a.x;
							if (fabsf(solid_material_properties->velocity.y - impulse_a.y) <= 1.0f)
								solid_material_properties->velocity.y = 0.0f;
							else
								solid_material_properties->velocity.y -= impulse_a.y;

							v2 impulse_b = V2MultiplyF32(impulse, b_inv_mass);
							collided_solid_material_properties->velocity = V2AddV2(collided_solid_material_properties->velocity, impulse_b);

							solid_material_properties->position = collided_solid_material_properties->position;

							break;
						}
						default:
							R_TODO;
							break;
						}
					}
				}
			}
			else
			{
				// Hasn't moved across any cells
				solid_material_properties->position = next_position;
			}
		}

		if (solid_material_properties->velocity.x != 0.0f || solid_material_properties->velocity.y != 0.0f)
			material->idle_start = 0;

		// Check how long this material has been in idle for.
		if (material->idle_start > 0)
		{
			if ((GetCurrentWorldTime() - material->idle_start) > 2.0f)
				no_longer_dynamic = 1;
		}
		else
			material->idle_start = GetCurrentWorldTime();

		material->has_been_updated = 1;
		if (!no_longer_dynamic)
			MakeMaterialDynamic(material);

		break;
	}

	case CELL_PROPERTIES_TYPE_fluid:
	{
		if (material->material_type == CELL_MATERIAL_TYPE_air)
		{
			FluidMatieral *material_properties = &material->properties.fluid;

			f32 remaining_air_pressure = material_properties->pressure;

			i32 rand_start = RandomI32(0, 3);
			for (i32 i = rand_start; i < rand_start + 4; i++)
			{
				Cell *neighbour_cell = 0;
				if (i % 4 == 0)
				{
					neighbour_cell = GetCellAtRelativePosition(cell, -1, 0);
				}
				else if (i % 4 == 1)
				{
					neighbour_cell = GetCellAtRelativePosition(cell, 0, -1);
				}
				else if (i % 4 == 2)
				{
					neighbour_cell = GetCellAtRelativePosition(cell, 1, 0);
				}
				else
				{
					neighbour_cell = GetCellAtRelativePosition(cell, 0, 1);
				}

				if (neighbour_cell->material)
				{
					if (neighbour_cell->material->properties_type == CELL_PROPERTIES_TYPE_fluid)
					{
						FluidMatieral *neighbour_properties = &neighbour_cell->material->properties.fluid;

						f32 pressure_diff = remaining_air_pressure - neighbour_properties->pressure;

						if (pressure_diff > 0)
						{
							f32 flow = pressure_diff * 0.5f;
							neighbour_properties->pressure += flow;
							material_properties->pressure -= flow;
							remaining_air_pressure -= flow;
						}
					}
				}
				else
				{
					if (remaining_air_pressure >= 1.0f)
					{
						CellMaterial *neighbor_material = NewCellMaterial(neighbour_cell, CELL_MATERIAL_TYPE_air);
						FluidMatieral *neighbour_properties = &neighbor_material->properties.fluid;
						MakeMaterialDynamic(neighbor_material);

						f32 pressure_diff = remaining_air_pressure - neighbour_properties->pressure;

						if (pressure_diff > 0)
						{
							f32 flow = pressure_diff * 0.5f;
							neighbour_properties->pressure += flow;
							material_properties->pressure -= flow;
							remaining_air_pressure -= flow;
						}
					}
				}
			}
		}
		else
		{
			FluidMatieral *material_properties = &material->properties.fluid;

			f32 remaining_mass = material_properties->mass;
			f32 flow = 0.0f;

			Cell *cell_below = GetCellAtRelativePosition(cell, 0, 1);
			CellMaterial *cell_below_material = cell_below->material;
			if (!cell_below_material)
			{
				cell_below_material = NewCellMaterial(cell_below, CELL_MATERIAL_TYPE_water);
				MakeMaterialDynamic(cell_below_material);
			}
			if (cell_below_material->material_type == CELL_MATERIAL_TYPE_water)
			{
				flow = GetStableFlowState(remaining_mass + cell_below_material->properties.fluid.mass) - cell_below_material->properties.fluid.mass;
				flow *= 0.5f;
				flow = ClampF32(flow, 0.0f, remaining_mass);

				material_properties->new_mass -= flow;
				cell_below_material->properties.fluid.new_mass += flow;
				remaining_mass -= flow;
			}

			if (remaining_mass > 0.0f)
			{
				Cell *cell_left = GetCellAtRelativePosition(cell, -1, 0);
				CellMaterial *cell_left_material = cell_left->material;
				if (!cell_left_material)
				{
					cell_left_material = NewCellMaterial(cell_left, CELL_MATERIAL_TYPE_water);
					MakeMaterialDynamic(cell_left_material);
				}
				if (cell_left_material && cell_left_material->material_type == CELL_MATERIAL_TYPE_water)
				{
					flow = (material_properties->mass - cell_left_material->properties.fluid.mass) / 4.0f;
					flow *= 0.5;
					flow = ClampF32(flow, 0.0f, remaining_mass);

					material_properties->new_mass -= flow;
					cell_left_material->properties.fluid.new_mass += flow;
					remaining_mass -= flow;
				}
			}

			if (remaining_mass > 0.0f)
			{
				Cell *cell_right = GetCellAtRelativePosition(cell, 1, 0);
				CellMaterial *cell_right_material = cell_right->material;
				if (!cell_right_material)
				{
					cell_right_material = NewCellMaterial(cell_right, CELL_MATERIAL_TYPE_water);
					MakeMaterialDynamic(cell_right_material);
				}
				if (cell_right_material && cell_right_material->material_type == CELL_MATERIAL_TYPE_water)
				{
					flow = (material_properties->mass - cell_right_material->properties.fluid.mass) / 4.0f;
					flow *= 0.5;
					flow = ClampF32(flow, 0.0f, remaining_mass);

					material_properties->new_mass -= flow;
					cell_right_material->properties.fluid.new_mass += flow;
					remaining_mass -= flow;
				}
			}

			if (remaining_mass > 0.0f)
			{
				Cell *cell_above = GetCellAtRelativePosition(cell, 0, -1);
				CellMaterial *cell_above_material = cell_above->material;
				if (!cell_above_material)
				{
					cell_above_material = NewCellMaterial(cell_above, CELL_MATERIAL_TYPE_water);
					MakeMaterialDynamic(cell_above_material);
				}
				if (cell_above_material && cell_above_material->material_type == CELL_MATERIAL_TYPE_water)
				{
					flow = remaining_mass - GetStableFlowState(remaining_mass + cell_above_material->properties.fluid.mass);
					flow *= 0.5f;
					flow = ClampF32(flow, 0.0f, remaining_mass);

					material_properties->new_mass -= flow;
					cell_above_material->properties.fluid.new_mass += flow;
					remaining_mass -= flow;
				}
			}
		}

		material->has_been_updated = 1;
		if (!no_longer_dynamic)
			MakeMaterialDynamic(material);
		AddCellChunkToUpdateQueue(cell->parent_cell_chunk);

		break;
	}
	default:
		R_TODO;
		break;
	}
}

internal void UpdateCellMaterials()
{
	for (i32 h = 0; h < core->run_data->active_chunk_count; h++)
	{
		ChunkData *chunk = &core->run_data->active_chunks[h];

		CellMaterial *dynamic_cell_materials[MAX_DYNAMIC_CELLS];
		MemoryCopy(dynamic_cell_materials, chunk->dynamic_cell_materials, sizeof(dynamic_cell_materials));
		i32 dynamic_cell_material_count = chunk->dynamic_cell_material_count;

		R_DEV_ASSERT(dynamic_cell_material_count < MAX_DYNAMIC_CELLS, "Too many dynamic cells");

		for (i32 i = 0; i < MAX_DYNAMIC_CELLS; i++)
			chunk->dynamic_cell_materials[i] = 0;
		chunk->dynamic_cell_material_count = 0;

		for (i32 i = 0; i < dynamic_cell_material_count; i++)
		{
			CellMaterial *material = dynamic_cell_materials[i];

			material->has_been_updated = 0;
			material->is_material_dynamic = 0;

			if (material->properties_type == CELL_PROPERTIES_TYPE_solid)
			{
				if (!(material->flags & CELL_FLAGS_no_gravity) && material->properties.solid.hardness == 0.0f)
				{
					i32 distance = 1;
					b8 apply_gravity = 1;
					while (1)
					{
						Cell *cell_below = GetCellAtRelativePosition(material->parent_cell, 0, distance);
						if (cell_below->material)
						{
							if (cell_below->material->mass == 0.0f)
							{
								apply_gravity = 0;
								break;
							}
						}
						else
						{
							break;
						}

						distance++;
					}

					if (apply_gravity)
					{
						material->properties.solid.velocity.y += 50.0f * core->world_delta_t;
						if (material->properties.solid.velocity.y > 980.0f)
							material->properties.solid.velocity.y = 980.0f;
					}
				}
			}
		}

		// Process cell
		for (i32 i = 0; i < dynamic_cell_material_count; i++)
		{
			CellMaterial *material = dynamic_cell_materials[i];
			if (material->id && !material->has_been_updated)
				ProcessCellMaterial(material);
		}

		// New mass values
		for (i32 i = 0; i < chunk->dynamic_cell_material_count; i++)
		{
			CellMaterial *material = chunk->dynamic_cell_materials[i];
			if (material->material_type == CELL_MATERIAL_TYPE_water)
			{
				material->properties.fluid.mass += material->properties.fluid.new_mass;
				material->properties.fluid.new_mass = 0.0f;
			}
		}
	}
}

internal void RenderCells()
{
#ifdef DEVELOPER_TOOLS
	if (!(core->run_data->editor_flags & EDITOR_FLAGS_draw_world))
		return;
#endif

	for (i32 i = 0; i < core->queued_cell_chunk_count; i++)
	{
		CellChunk *cell_chunk = core->queued_cell_chunks_for_update[i];
		UpdateCellChunkTexture(cell_chunk);
		core->queued_cell_chunks_for_update[i] = 0;
	}
	core->queued_cell_chunk_count = 0;

	for (i32 h = 0; h < core->run_data->active_chunk_count; h++)
	{
		ChunkData *chunk = &core->run_data->active_chunks[h];

		for (i32 i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
		{
			for (i32 j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
			{
				CellChunk *cell_chunk = &chunk->cell_chunks[i][j];

				v2 render_pos = v2view(v2((f32)chunk->x_index * CHUNK_SIZE + (f32)cell_chunk->x_index * CELL_CHUNK_SIZE,
										  (f32)chunk->y_index * CHUNK_SIZE + (f32)cell_chunk->y_index * CELL_CHUNK_SIZE));
				v2 render_size = v2zoom(v2(CELL_CHUNK_SIZE, CELL_CHUNK_SIZE));

				Ts2dPushTexture(&cell_chunk->texture,
								v4(0.0f, 0.0f, CELL_CHUNK_SIZE, CELL_CHUNK_SIZE),
								v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
			}
		}
	}
}

internal void UpdateChunkTextures(ChunkData *chunk)
{
	for (i32 i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
	{
		for (i32 j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
		{
			CellChunk *cell_chunk = &chunk->cell_chunks[i][j];
			UpdateCellChunkTexture(cell_chunk);
		}
	}
}

internal void UpdateCellChunkTexture(CellChunk *chunk)
{
	unsigned char pixel_data[CHUNK_SIZE * CHUNK_SIZE * 4];
	unsigned char *pixel_buffer = &pixel_data[0];

	for (i32 y = 0; y < CELL_CHUNK_SIZE; y++)
	{
		for (i32 x = 0; x < CELL_CHUNK_SIZE; x++)
		{
			Cell *cell = &chunk->cells[y][x];
			v4 colour;
			if (cell->material)
			{
				switch (cell->material->material_type)
				{
				case CELL_MATERIAL_TYPE_air:
				{
					f32 pressure = cell->material->properties.fluid.pressure;
					if (pressure > 10.0f)
						pressure = 10.0f;
					else if (pressure < 0.0f)
						pressure = 0.0f;

					colour = v4(pressure / 10.0f, 0.0f, 0.0f, 1.0f);
					break;
				}
				case CELL_MATERIAL_TYPE_dirt:
				{
					v3 dirt_colour = {14.0f / 360.0f, 0.36f, 0.31f};

					if (cell->material->properties.solid.hardness > 0.0f)
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

					if (cell->material->properties.solid.hardness > 0.0f)
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
					v3 water_colour = {218.0f / 360.0f, 0.58f, 0.91f};
					f32 mass = cell->material->properties.fluid.mass;

					v3 col = HSVToRGB(v3(water_colour.x, water_colour.y, water_colour.z));
					colour = v4(col.r, col.g, col.b, mass);
					colour = V4MultiplyF32(colour, 0.7f);

					break;
				}
				default:
					colour = v4u(1.0f);
					break;
				}
			}
			else
				colour = v4u(0.0f);

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
									 CELL_CHUNK_SIZE,
									 CELL_CHUNK_SIZE,
									 pixel_data);
} */