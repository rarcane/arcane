internal CellMaterial *NewCellMaterial(Cell *cell)
{
	ChunkData *chunk = cell->parent_chunk;
	R_DEV_ASSERT(chunk->free_cell_material_index != -1, "No free cell materials in chunk.");

	i32 new_id = chunk->free_cell_material_index;
	if (chunk->cell_material_count == chunk->free_cell_material_index)
	{
		chunk->cell_material_count++;
		chunk->free_cell_material_index++;
	}
	CellMaterial *cell_material = &chunk->cell_materials[new_id];
	cell_material->id = new_id;
	cell_material->parent_cell = cell;
	cell_material->position = v2(0.5f, 0.5f);

	cell->material = cell_material;

	return cell_material;
}

internal Cell *GetCellAtPosition(i32 x, i32 y)
{
	ChunkData *chunk = GetChunkAtPosition(v2((f32)x, (f32)y));
	i32 cell_x = abs(x % CHUNK_SIZE);
	i32 cell_y = CHUNK_SIZE - abs(y % CHUNK_SIZE);

	return &chunk->cells[cell_y][cell_x];
}

internal void ProcessCellMaterial(CellMaterial *material)
{
	Cell *cell = material->parent_cell;
	ChunkData *cell_chunk = material->parent_cell->parent_chunk;

	R_DEV_ASSERT(!material->has_been_updated, "Cell contents have already been updated.");
	if (material->velocity.x != 0.0f || material->velocity.y != 0.0f)
	{
		v2 next_position = V2AddV2(material->position, V2MultiplyF32(material->velocity, core->world_delta_t));

		i32 x_cell_steps = (i32)floorf(next_position.x);
		i32 y_cell_steps = (i32)floorf(next_position.y);
		i32 x_is_larger = x_cell_steps >= y_cell_steps;

		if (abs(x_cell_steps) > 0 || abs(y_cell_steps) > 0)
		{
			b8 has_collided = 0;

			for (int j = 1; j < (x_is_larger ? abs(x_cell_steps) : abs(y_cell_steps)) + 1; j++)
			{
				f32 step_alpha = (f32)j / (x_is_larger ? (f32)abs(x_cell_steps) : (f32)abs(y_cell_steps));

				i32 x_cell_movement = (i32)round(x_cell_steps * step_alpha);
				i32 y_cell_movement = (i32)round(y_cell_steps * step_alpha);

				Cell *next_cell = GetCellAtPosition(cell_chunk->x_index * CHUNK_SIZE + cell->x_index + x_cell_movement,
													cell_chunk->y_index * CHUNK_SIZE + cell->y_index + y_cell_movement);

				if (next_cell->material &&
					(next_cell->material->velocity.x != 0.0f || next_cell->material->velocity.y != 0.0f) &&
					!next_cell->material->has_been_updated)
				{
					ProcessCellMaterial(next_cell->material);

					next_cell = GetCellAtPosition(cell_chunk->x_index * CHUNK_SIZE + cell->x_index + x_cell_movement,
												  cell_chunk->y_index * CHUNK_SIZE + cell->y_index + y_cell_movement);
				}

				if (!next_cell->material)
				{
					cell->material = 0;

					cell = next_cell;

					cell->material = material;
					material->parent_cell = cell;

					material->position = v2(next_position.x - x_cell_steps, next_position.y - y_cell_steps);
				}
				else
				{
					has_collided = 1;
					break;
				}
			}

			if (has_collided)
			{
				material->position = v2(0.5f, 0.5f);
				material->velocity = v2(0.0f, 0.0f);
			}
			else
			{
				/* material->has_been_updated = 1;
				cell_chunk->dynamic_cell_materials[cell_chunk->dynamic_cell_material_count++] = material; */
			}
		}
		else
		{
			// Hasn't moved across any cells
			material->position = next_position;

			/* material->has_been_updated = 1;
			cell_chunk->dynamic_cell_materials[cell_chunk->dynamic_cell_material_count++] = material; */
		}
	}
	else
		R_BREAK("Why is this cell dynamic?");

	// TEMP
	material->has_been_updated = 1;
	cell_chunk->dynamic_cell_materials[cell_chunk->dynamic_cell_material_count++] = material;
}

internal void UpdateCellMaterials()
{
	ChunkData *chunk = GetChunkAtPosition(v2(0, -1));

	CellMaterial *dynamic_cell_materials[MAX_DYNAMIC_CELLS];
	MemoryCopy(dynamic_cell_materials, chunk->dynamic_cell_materials, sizeof(dynamic_cell_materials));
	i32 dynamic_cell_material_count = chunk->dynamic_cell_material_count;

	R_DEV_ASSERT(dynamic_cell_material_count < MAX_DYNAMIC_CELLS, "Too many dynamic cells");

	for (int i = 0; i < MAX_DYNAMIC_CELLS; i++)
		chunk->dynamic_cell_materials[i] = 0;
	chunk->dynamic_cell_material_count = 0;

	// Apply gravity
	for (int i = 0; i < dynamic_cell_material_count; i++)
	{
		CellMaterial *cell = dynamic_cell_materials[i];

		cell->has_been_updated = 0;

		cell->velocity.y += 50.0f * core->world_delta_t;
		if (cell->velocity.y > 980.0f)
			cell->velocity.y = 980.0f;
	}

	// Attempt to move in direction of velocity
	for (int i = 0; i < dynamic_cell_material_count; i++)
	{
		CellMaterial *cell = dynamic_cell_materials[i];
		R_DEV_ASSERT(cell->position.x <= 1.0f && cell->position.x >= 0.0f && cell->position.y <= 1.0f && cell->position.y >= 0.0f,
					 "Position should not be outside of the cell.");

		if (!cell->has_been_updated)
			ProcessCellMaterial(cell);
	}

	UpdateChunkTexture(chunk);
}

internal void RenderCells()
{
	ChunkData *chunk = GetChunkAtPosition(v2(0, -1));

	v2 render_pos = v2view(v2((f32)chunk->x_index * CHUNK_SIZE, (f32)chunk->y_index * CHUNK_SIZE));
	v2 render_size = v2zoom(v2(CHUNK_SIZE, CHUNK_SIZE));
	Ts2dPushTexture(core->renderer,
					&chunk->texture,
					v4(0.0f, 0.0f, CHUNK_SIZE, CHUNK_SIZE),
					v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
}

internal void UpdateChunkTexture(ChunkData *chunk)
{
	unsigned char pixel_data[CHUNK_SIZE * CHUNK_SIZE * 4];
	unsigned char *pixel_buffer = &pixel_data[0];

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			Cell *cell = &chunk->cells[y][x];
			v4 colour;
			if (cell->material)
			{
				switch (cell->material->material_type)
				{
				case CELL_MATERIAL_TYPE_air:
					colour = v4u(0.0f);
					break;
				case CELL_MATERIAL_TYPE_dirt:
					colour = v4(80.0f / 255.0f, 58.0f / 255.0f, 51.0f / 255.0f, 1.0f);
					break;
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

	chunk->texture = Ts2dTextureInit(core->renderer,
									 TS2D_TEXTURE_FORMAT_R8G8B8A8,
									 CHUNK_SIZE,
									 CHUNK_SIZE,
									 pixel_data);
}