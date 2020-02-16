internal CellMaterial *NewCellMaterial(Cell *cell)
{
	ChunkData *chunk = cell->parent_cell_chunk->parent_chunk;
	R_DEV_ASSERT(!cell->material, "There's already a material in this cell.");
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

	AddCellChunkToUpdateQueue(cell->parent_cell_chunk);

	return cell_material;
}

internal void DeleteMaterialFromChunk(ChunkData *chunk, CellMaterial *material)
{
	for (int i = 0; i < chunk->cell_material_count; i++)
	{
		CellMaterial *supposed = &chunk->cell_materials[i];
		if (supposed == material)
		{
			CellMaterial empty = {0};
			chunk->cell_materials[i] = empty;

			if (i < chunk->free_cell_material_index)
				chunk->free_cell_material_index = i;

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
		CellMaterial *new_material = NewCellMaterial(new_cell);
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
	for (int i = 0; i < core->queued_cell_chunk_count; i++)
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
	material->is_material_dynamic = 1;
	material->parent_cell->parent_cell_chunk->parent_chunk->dynamic_cell_materials[material->parent_cell->parent_cell_chunk->parent_chunk->dynamic_cell_material_count++] = material;
}

internal void ProcessCellMaterial(CellMaterial *material)
{
	Cell *cell = material->parent_cell;

	b8 no_longer_dynamic = 0;

	R_DEV_ASSERT(!material->has_been_updated, "Cell contents have already been updated.");
	if (material->velocity.x != 0.0f || material->velocity.y != 0.0f)
	{
		v2 next_position = V2AddV2(material->position, V2MultiplyF32(material->velocity, core->world_delta_t));

		i32 x_cell_steps = (i32)floorf(next_position.x);
		i32 y_cell_steps = (i32)floorf(next_position.y);
		i32 x_is_larger = abs(x_cell_steps) >= abs(y_cell_steps);

		if (abs(x_cell_steps) > 0 || abs(y_cell_steps) > 0)
		{
			for (int j = 1; j < (x_is_larger ? abs(x_cell_steps) : abs(y_cell_steps)) + 1; j++)
			{
				// TODO: Crossing into different chunks
				Cell *next_cell = GetCellAtRelativePosition(cell, GetSign((f32)x_cell_steps), GetSign((f32)y_cell_steps));

				// Process the material in the next cell if it's got velocity
				if (next_cell->material &&
					(next_cell->material->velocity.x != 0.0f || next_cell->material->velocity.y != 0.0f) &&
					!next_cell->material->has_been_updated)
				{
					material->has_been_updated = 1;
					ProcessCellMaterial(next_cell->material);
					material->has_been_updated = 0;

					next_cell = GetCellAtRelativePosition(cell, GetSign((f32)x_cell_steps), GetSign((f32)y_cell_steps));
				}

				if (!next_cell->material)
				{
					// Next cell is empty, so just move the material into it
					material = MoveMaterialToCell(material, next_cell);
					cell = next_cell;

					if (next_position.x >= 0.0f)
						material->position.x = next_position.x - x_cell_steps;
					else
						material->position.x = fabsf(next_position.x - x_cell_steps);

					if (next_position.y >= 0.0f)
						material->position.y = next_position.y - y_cell_steps;
					else
						material->position.y = fabsf(next_position.y - y_cell_steps);
				}
				else
				{
					// Cell already has a material in it. Process interaction accordingly.
					CellMaterial *collided_material = next_cell->material;

					if (collided_material->mass == 0.0f)
					{
						b8 bake_material = 0;
						if (collided_material->max_height == -1)
						{
							bake_material = 1;
						}
						else
						{
							for (int i = 1; i < collided_material->max_height + 1; i++)
							{
								Cell *bottom_left_cell = GetCellAtRelativePosition(cell, -1, i);
								Cell *bottom_right_cell = GetCellAtRelativePosition(cell, 1, i);
								if (bottom_left_cell->material && bottom_right_cell->material)
								{
									bake_material = 1;
									break;
								}
							}
						}

						if (bake_material)
						{
							material->mass = 0;
							no_longer_dynamic = 1;

							material->position = collided_material->position;
							material->velocity.y = 0.0f;
						}
						else
						{
							Cell *bottom_left_cell = GetCellAtRelativePosition(cell, -1, 1);
							Cell *bottom_right_cell = GetCellAtRelativePosition(cell, 1, 1);

							i32 go_left = RandomI32(0, 1);
							if (go_left && bottom_left_cell->material)
								go_left = 0;
							if (!go_left && bottom_right_cell->material)
								go_left = 1;
							//R_DEV_ASSERT(!cell_to_move->material, "Somehow still a material in this cell.");

							Cell *cell_to_move = 0;
							if (go_left)
								cell_to_move = bottom_left_cell;
							else
								cell_to_move = bottom_right_cell;

							material = MoveMaterialToCell(material, cell_to_move);
							cell = cell_to_move;
						}
					}
					else
					{
						f32 collision_magnitude = SquareRoot((f32)x_cell_steps * (f32)x_cell_steps + (f32)y_cell_steps * (f32)y_cell_steps);
						v2 collision_normal = v2(x_cell_steps / collision_magnitude, y_cell_steps / collision_magnitude);

						v2 relative_velocity = V2SubtractV2(collided_material->velocity, material->velocity);
						f32 velocity_along_normal = relative_velocity.x * collision_normal.x + relative_velocity.y * collision_normal.y;

						f32 restitution = MinimumF32(material->restitution, collided_material->restitution);

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

						v2 impulse = V2MultiplyF32(collision_normal, impulse_scalar);

						v2 impulse_a = V2MultiplyF32(impulse, a_inv_mass);
						material->velocity.x -= impulse_a.x;
						if (fabsf(material->velocity.y - impulse_a.y) <= 1.0f)
							material->velocity.y = 0.0f;
						else
							material->velocity.y -= impulse_a.y;

						v2 impulse_b = V2MultiplyF32(impulse, b_inv_mass);
						collided_material->velocity = V2AddV2(collided_material->velocity, impulse_b);

						material->position = collided_material->position;
					}

					break;
				}
			}
		}
		else
		{
			// Hasn't moved across any cells
			material->position = next_position;
		}
	}
	else
	{
		/* R_BREAK("Why is this material being processed?"); */
	}

	material->has_been_updated = 1;
	if (!no_longer_dynamic)
		MakeMaterialDynamic(material);
}

internal void UpdateCellMaterials()
{
	for (int h = 0; h < core->world_data->active_chunk_count; h++)
	{
		ChunkData *chunk = &core->world_data->active_chunks[h];

		CellMaterial *dynamic_cell_materials[MAX_DYNAMIC_CELLS];
		MemoryCopy(dynamic_cell_materials, chunk->dynamic_cell_materials, sizeof(dynamic_cell_materials));
		i32 dynamic_cell_material_count = chunk->dynamic_cell_material_count;

		R_DEV_ASSERT(dynamic_cell_material_count < MAX_DYNAMIC_CELLS, "Too many dynamic cells");

		for (int i = 0; i < MAX_DYNAMIC_CELLS; i++)
			chunk->dynamic_cell_materials[i] = 0;
		chunk->dynamic_cell_material_count = 0;

		for (int i = 0; i < dynamic_cell_material_count; i++)
		{
			CellMaterial *material = dynamic_cell_materials[i];

			material->has_been_updated = 0;
			material->is_material_dynamic = 0;

			if (!(material->flags & CELL_FLAGS_no_gravity))
			{
				/* Cell *cell_below = GetCellAtRelativePosition(material->parent_cell, 0, 1);
			if (!cell_below->material)
			{ */
				material->velocity.y += 50.0f * core->world_delta_t;
				if (material->velocity.y > 980.0f)
					material->velocity.y = 980.0f;
				/* } */
			}
		}

		// Attempt to move in direction of velocity
		for (int i = 0; i < dynamic_cell_material_count; i++)
		{
			CellMaterial *material = dynamic_cell_materials[i];
			R_DEV_ASSERT(material->position.x <= 1.0f && material->position.x >= 0.0f && material->position.y <= 1.0f && material->position.y >= 0.0f,
						 "Position should not be outside of the material.");

			if (!material->has_been_updated)
				ProcessCellMaterial(material);
		}

		for (int i = 0; i < chunk->dynamic_cell_material_count; i++)
		{
			CellMaterial *material = chunk->dynamic_cell_materials[i];
			CellChunk *cell_chunk = material->parent_cell->parent_cell_chunk;
			//AddCellChunkToUpdateQueue(cell_chunk);
		}
	}
}

internal void RenderCells()
{
	for (int i = 0; i < core->queued_cell_chunk_count; i++)
	{
		CellChunk *cell_chunk = core->queued_cell_chunks_for_update[i];
		UpdateCellChunkTexture(cell_chunk);
		core->queued_cell_chunks_for_update[i] = 0;
	}
	core->queued_cell_chunk_count = 0;

	for (int h = 0; h < core->world_data->active_chunk_count; h++)
	{
		ChunkData *chunk = &core->world_data->active_chunks[h];

		for (int i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
		{
			for (int j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
			{
				CellChunk *cell_chunk = &chunk->cell_chunks[i][j];

				v2 render_pos = v2view(v2((f32)chunk->x_index * CHUNK_SIZE + (f32)cell_chunk->x_index * CELL_CHUNK_SIZE,
										  (f32)chunk->y_index * CHUNK_SIZE + (f32)cell_chunk->y_index * CELL_CHUNK_SIZE));
				v2 render_size = v2zoom(v2(CELL_CHUNK_SIZE, CELL_CHUNK_SIZE));

				Ts2dPushTexture(core->renderer,
								&cell_chunk->texture,
								v4(0.0f, 0.0f, CELL_CHUNK_SIZE, CELL_CHUNK_SIZE),
								v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
			}
		}
	}
}

internal void UpdateChunkTextures(ChunkData *chunk)
{
	for (int i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
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

	for (int y = 0; y < CELL_CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CELL_CHUNK_SIZE; x++)
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
									 CELL_CHUNK_SIZE,
									 CELL_CHUNK_SIZE,
									 pixel_data);
}