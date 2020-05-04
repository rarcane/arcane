// Smooths a linear 0.0 -> 1.0
internal f32 Fade(f32 alpha)
{
	return 6 * powf(alpha, 5) - 15 * powf(alpha, 4) + 10 * powf(alpha, 3);
}

internal f32 LerpF32(f32 alpha, f32 a, f32 b)
{
	return a + (b - a) * alpha;
}

internal void v2Normalise(v2 *v)
{
	f32 magnitude = PythagSolve(v->x, v->y);

	if (magnitude > 0)
	{
		v->x = v->x / magnitude;
		v->y = v->y / magnitude;
	}
}

internal void v2Realise(v2 *v)
{
	if (isnan(v->x) || !isfinite(v->x))
		v->x = 0.0f;
	if (isnan(v->y) || !isfinite(v->y))
		v->y = 0.0f;
}

internal f32 PythagSolve(f32 a, f32 b)
{
	return SquareRoot(a * a + b * b);
}

internal f32 Square(f32 a)
{
	return a * a;
}

internal i32 GetSign(f32 x)
{
	return (x > 0.0f) - (x < 0.0f);
}

// Floors to the 1st decimal place.
internal f32 RoundFirst(f32 x)
{
	x *= 10.0f;
	x = roundf(x);
	x /= 10.0f;
	return x;
}

internal b8 EqualF32(f32 a, f32 b, f32 leeway)
{
	return (fabsf(a - b) <= leeway);
}

internal b8 EqualV2(v2 a, v2 b, f32 leeway)
{
	return (EqualF32(a.x, b.x, leeway) && EqualF32(a.y, b.y, leeway));
}

internal void ShufflePerlinNoise()
{
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			core->random_field[j][i].x = (RandomI32(0, 1) ? -1.0f : 1.0f);
			core->random_field[i][j].y = (RandomI32(0, 1) ? -1.0f : 1.0f);
		}
	}
}

internal f32 GetPerlinNoise(f32 x_pos, f32 y_pos)
{
	i32 x_min = (i32)floorf(x_pos);
	i32 x_max = (i32)floorf(x_pos) + 1;

	i32 y_min = (i32)floorf(y_pos);
	i32 y_max = (i32)floorf(y_pos) + 1;

	R_DEV_ASSERT(x_min < 256 && y_min < 256, "Noise out of bonds."); // TODO: Wrap around

	v2 g0 = core->random_field[y_min][x_min];
	v2 g1 = core->random_field[y_min][x_min + 1];
	v2 g2 = core->random_field[y_min + 1][x_min];
	v2 g3 = core->random_field[y_min + 1][x_min + 1];

	v2 d0 = {x_pos - x_min, y_pos - y_min};
	v2 d1 = {x_pos - x_max, y_pos - y_min};
	v2 d2 = {x_pos - x_min, y_pos - y_max};
	v2 d3 = {x_pos - x_max, y_pos - y_max};

	f32 p0 = g0.x * d0.x + g0.y * d0.y;
	f32 p1 = g1.x * d1.x + g1.y * d1.y;
	f32 p2 = g2.x * d2.x + g2.y * d2.y;
	f32 p3 = g3.x * d3.x + g3.y * d3.y;

	f32 x_fade = Fade(x_pos - x_min);
	f32 y_fade = Fade(y_pos - y_min);

	f32 lerp1 = p0 + x_fade * (p1 - p0);
	f32 lerp2 = p2 + x_fade * (p3 - p2);
	f32 lerp3 = lerp1 + y_fade * (lerp2 - lerp1);

	return lerp3;
}

internal c2AABB v2AddAABB(v2 a, c2AABB aabb)
{
	c2AABB new_aabb = {.min = c2V(a.x + aabb.min.x, a.y + aabb.min.y),
					   .max = c2V(a.x + aabb.max.x, a.y + aabb.max.y)};
	return new_aabb;
}

internal void CapsuleToWorldSpace(c2Capsule *capsule, v2 world_space)
{
	capsule->a.x += world_space.x;
	capsule->a.y += world_space.y;

	capsule->b.x += world_space.x;
	capsule->b.y += world_space.y;
}

internal void LineToWorldSpace(c2Ray *line, v2 world_space)
{
	line->p.x += world_space.x;
	line->p.y += world_space.y;
}

internal c2Poly v2AddPoly(v2 a, c2Poly poly)
{
	for (int i = 0; i < poly.count; i++)
	{
		poly.verts[i].x += a.x;
		poly.verts[i].y += a.y;
	}

	return poly;
}

#ifdef DEVELOPER_TOOLS
internal void PushDebugLine(v2 p1, v2 p2, v3 colour)
{
	DebugLine debug_line = {
		1,
		p1,
		p2,
		colour,
		0,
		0.0f,
		0.0f,
	};

	if (core->debug_line_count == core->free_debug_line_index)
	{
		core->debug_lines[core->debug_line_count++] = debug_line;
		core->free_debug_line_index++;
	}
	else
	{
		core->debug_lines[core->free_debug_line_index] = debug_line;

		b8 found_free_index = 0;
		for (int i = 0; i < core->debug_line_count + 1; i++)
		{
			if (!core->debug_lines[i].is_valid)
			{
				core->free_debug_line_index = i;
				found_free_index = 1;
				break;
			}
		}

		R_DEV_ASSERT(found_free_index, "Couldn't find a spare index.");
	}
}

internal void PushDebugLineForDuration(v2 p1, v2 p2, v3 colour, f32 lifetime)
{
	DebugLine debug_line = {
		1,
		p1,
		p2,
		colour,
		1,
		lifetime,
		core->run_data->world.elapsed_world_time,
	};

	if (core->debug_line_count == core->free_debug_line_index)
	{
		core->debug_lines[core->debug_line_count++] = debug_line;
		core->free_debug_line_index++;
	}
	else
	{
		core->debug_lines[core->free_debug_line_index] = debug_line;

		b8 found_free_index = 0;
		for (int i = 0; i < core->debug_line_count + 1; i++)
		{
			if (!core->debug_lines[i].is_valid)
			{
				core->free_debug_line_index = i;
				found_free_index = 1;
				break;
			}
		}

		R_DEV_ASSERT(found_free_index, "Couldn't find a spare index.");
	}
}

internal void DrawDebugLines()
{
	for (int i = 0; i < core->debug_line_count; i++)
	{
		DebugLine *debug_line = &core->debug_lines[i];

		if (debug_line->is_valid && debug_line->has_duration && debug_line->start_time + debug_line->lifetime <= core->run_data->world.elapsed_world_time)
		{
			DebugLine empty_debug_line = {0};
			core->debug_lines[i] = empty_debug_line;

			if (i < core->free_debug_line_index)
				core->free_debug_line_index = i;
		}

		if (debug_line->is_valid)
		{
			f32 alpha;
			if (debug_line->has_duration)
				alpha = ((debug_line->start_time + debug_line->lifetime) - core->run_data->world.elapsed_world_time) / debug_line->lifetime;
			else
				alpha = 1.0f;

			Ts2dPushLine(v4(debug_line->colour.r, debug_line->colour.g, debug_line->colour.b, alpha),
						 v2view(debug_line->p1),
						 v2view(debug_line->p2));

			if (!debug_line->has_duration)
			{
				DebugLine empty_debug_line = {0};
				core->debug_lines[i] = empty_debug_line;

				if (i < core->free_debug_line_index)
					core->free_debug_line_index = i;
			}
		}
	}
}

internal void PushDebugShape(c2Shape shape, c2ShapeType type, v2 position, v3 colour)
{
	switch (type)
	{
	case C2_SHAPE_TYPE_aabb:
	{
		v2 p0 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.min.y));
		v2 p1 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.min.y));
		v2 p2 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.max.y));
		v2 p3 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.max.y));

		PushDebugLine(p0, p1, colour);
		PushDebugLine(p1, p2, colour);
		PushDebugLine(p2, p3, colour);
		PushDebugLine(p3, p0, colour);
	}
	break;

	case C2_SHAPE_TYPE_poly:
	{
		for (int i = 0; i < shape.poly.count; i++)
		{
			int secondPoint = (i == shape.poly.count - 1 ? 0 : i + 1);

			v2 p1 = V2AddV2(position, v2(shape.poly.verts[i].x, shape.poly.verts[i].y));
			v2 p2 = V2AddV2(position, v2(shape.poly.verts[secondPoint].x, shape.poly.verts[secondPoint].y));

			PushDebugLine(p1,
						  p2,
						  colour);
		}
	}
	break;

	case C2_SHAPE_TYPE_line:
	{
		PushDebugLine(V2AddV2(position, shape.line.p1),
					  V2AddV2(position, shape.line.p2),
					  colour);
	}
	break;

	default:
		//R_TODO;
		break;
	}
}

/* internal void PushDebugShapeForDuration(Shape shape, v2 position, v3 colour, f32 lifetime)
{
	for (int i = 0; i < shape.vertex_count; i++)
	{
		int secondPoint = (i == shape.vertex_count - 1 ? 0 : i + 1);

		v2 p1 = V2AddV2(position, v2(shape.vertices[i].x, shape.vertices[i].y));
		v2 p2 = V2AddV2(position, v2(shape.vertices[secondPoint].x, shape.vertices[secondPoint].y));

		PushDebugLineForDuration(p1,
								 p2,
								 colour,
								 lifetime);
	}
} */
#endif

internal v2 GetMousePositionInWorldSpace()
{
	return v2(platform->mouse_x / core->camera_zoom - core->camera_position.x - GetZeroWorldPosition().x, platform->mouse_y / core->camera_zoom - core->camera_position.y - GetZeroWorldPosition().y);
}

internal v4 GetCameraRegionRect()
{
	v2 top_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 top_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};

	// TODO(randy): simplify

	return v4(top_left.x, top_left.y, top_right.x - top_left.x, bottom_left.y - top_left.y);
}

internal void GetSkeletonChunksInRegion(SkeletonChunk *chunks, i32 *chunk_count, v4 rect, i32 buffer)
{
	*chunk_count = 0;

	i32 width = WorldspaceToChunkIndex(rect.x + rect.width) - WorldspaceToChunkIndex(rect.x);
	i32 height = WorldspaceToChunkIndex(rect.y + rect.height) - WorldspaceToChunkIndex(rect.y);

	for (int y = -buffer; y <= height + buffer; y++)
	{
		for (int x = -buffer; x <= width + buffer; x++)
		{
			R_DEV_ASSERT(*chunk_count + 1 < MAX_WORLD_CHUNKS, "Chunks in region exceed the max.");

			SkeletonChunk chunk = {WorldspaceToChunkIndex(rect.x) + x, WorldspaceToChunkIndex(rect.y) + y};
			chunks[(*chunk_count)++] = chunk;
		}
	}
}

// TODO(randy): windows-only, put in #ifdefs or some shit

internal b8 MakeDirectory(char *directory_path)
{
	return CreateDirectoryA(directory_path, 0);
}

internal b8 DoesDirectoryExist(char *directory_path)
{
	DWORD dwAttrib = GetFileAttributes(directory_path);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// ~~~

internal void WriteToFile(FILE *file, void *data, size_t size_bytes)
{
	fwrite(data, size_bytes, 1, file);
}

internal void ReadFromFile(FILE *file, void *data, size_t size_bytes)
{
	fread(data, size_bytes, 1, file);
}

// Saves current data to a specified level.
internal void SaveLevel(char *level_name)
{
	R_DEV_ASSERT(level_name[0], "Invalid name.");

	serialisation_pointer_count = 0;

	char path[200] = "";
	sprintf(path, "%sworlds\\", core->run_data->res_path);

	if (!DoesDirectoryExist(path))
		MakeDirectory(path);

	strcat(path, level_name);
	strcat(path, "\\");
	if (!DoesDirectoryExist(path))
		MakeDirectory(path);

	// Write basic data to world_data.save
	{
		char file_path[200] = "";
		sprintf(file_path, "%sworld_data.save", path);
		FILE *save = fopen(file_path, "w");
		R_DEV_ASSERT(save, "Couldn't open file.");

		// Save the player entity
		{
			EntitySave entity_save = {.flags = core->run_data->character_entity->flags, .type = core->run_data->character_entity->generalised_type};
			MemoryCopy(entity_save.name, core->run_data->character_entity->name, sizeof(entity_save.name));

			WriteToFile(save, &entity_save, sizeof(EntitySave));

			WriteToFile(save, &core->run_data->character_entity->component_ids, sizeof(core->run_data->character_entity->component_ids));
			for (i32 i = 1; i < COMPONENT_MAX; i++)
			{
				if (core->run_data->character_entity->component_ids[i])
				{
					WriteComponentToFile(save, core->run_data->character_entity->component_ids[i], i);
				}
			}
		}

		// Save world data struct
		WriteWorldSaveDataToFile(save, &core->run_data->world);

		// Save all of the entities that don't belong to chunks.
		WriteToFile(save, &core->run_data->floating_entity_id_count, sizeof(i32));
		for (i32 i = 0; i < core->run_data->floating_entity_id_count; i++)
		{
			Entity *entity = &core->run_data->entities[core->run_data->floating_entity_ids[i] - 1];

			EntitySave entity_save = {.flags = entity->flags, .type = entity->generalised_type};
			MemoryCopy(entity_save.name, entity->name, sizeof(entity_save.name));

			WriteToFile(save, &entity_save, sizeof(EntitySave));
		}

		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			SerialiseEntityComponentsFromIDList(save, core->run_data->floating_entity_ids, core->run_data->floating_entity_id_count, i);
		}

		fclose(save);
	}

	// Save currently loaded chunks to the chunk folder
	{
		char file_path[200] = "";
		sprintf(file_path, "%schunks\\", path);

		if (!DoesDirectoryExist(file_path))
			MakeDirectory(file_path);

		for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
		{
			SaveChunkToDisk(file_path, &core->run_data->active_chunks[i]);
		}
	}

	Log("Level saved to %s.save", level_name);

	/* WriteWorldDataToFile(save, core->run_data);
	save = fopen(path, "r+");
	fseek(save, 0, SEEK_SET);
	FillWorldDataPointersInFile(save, core->run_data);
	fclose(save); */
}

// Loads a given level. Returns 0 if the level doesn't exist.
internal b8 LoadLevel(char *level_name)
{
	R_DEV_ASSERT(level_name[0], "Invalid level name.");

	char path[200] = "";
	sprintf(path, "%sworlds\\%s\\", core->run_data->res_path, level_name);

	if (!DoesDirectoryExist(path))
		return 0;

	serialisation_pointer_count = 0;
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	InitialiseRunData();
	InitialiseWorldData();
	// NOTE(randy): Might want to seperate some run data that is permanent
	// and will not change between saves

	// Read in basic data from the world_data.save
	{
		char file_path[200] = "";
		sprintf(file_path, "%sworld_data.save", path);
		FILE *save = fopen(file_path, "r");
		R_DEV_ASSERT(save, "Failed to read world_data.save from directory %s", path);
		// TODO(randy): Figure out a proper way of doing these asserts.
		// Start using the in-built telescope assert and really think through crashing, error messages, warnings, etc.

		// Read the player entity in
		{
			EntitySave entity_save;
			ReadFromFile(save, &entity_save, sizeof(EntitySave));

			Entity *entity = NewEntity(entity_save.name, entity_save.type);
			entity->flags = entity_save.flags;

			i32 component_ids[COMPONENT_MAX];
			ReadFromFile(save, &component_ids, sizeof(component_ids));
			for (i32 i = 1; i < COMPONENT_MAX; i++)
			{
				if (component_ids[i])
					ReadComponentFromFile(save, entity, i);
			}

			core->run_data->character_entity = entity;
		}

		// Read in basic world data
		ReadWorldSaveDataFromFile(save, &core->run_data->world);

		// Read in floating entities
		ReadFromFile(save, &core->run_data->floating_entity_id_count, sizeof(i32));
		for (i32 i = 0; i < core->run_data->floating_entity_id_count; i++)
		{
			EntitySave entity_save;
			ReadFromFile(save, &entity_save, sizeof(EntitySave));

			Entity *entity = NewEntity(entity_save.name, entity_save.type);
			entity->flags = entity_save.flags;

			core->run_data->floating_entity_ids[i] = entity->entity_id;
		}

		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			DeserialiseEntityComponentsFromIDList(save, core->run_data->floating_entity_ids, core->run_data->floating_entity_id_count, i);
		}

		fclose(save);
	}

	// Load some surrounding chunks in based off of the player's position.
	{
		char file_path[200] = "";
		sprintf(file_path, "%schunks\\", path);
		R_DEV_ASSERT(DoesDirectoryExist(file_path), "Chunk directory does not exist.");

		TransformInGameCamera();

		SkeletonChunk chunks[MAX_WORLD_CHUNKS];
		i32 chunk_count;
		GetSkeletonChunksInRegion(chunks, &chunk_count, GetCameraRegionRect(), 1);

		for (i32 i = 0; i < chunk_count; i++)
		{
			Chunk *chunk = LoadChunkFromDisk(file_path, chunks[i].x_index, chunks[i].y_index);

			// NOTE(randy): Maybe Dev Asserts get switched into simple error logs in release builds?
			// R_DEV_ASSERT(chunk, "A surrounding chunk in the player's region doesn't exist, is this intended?");
			if (!chunk)
				LogError("A surrounding chunk in the player's region doesn't exist, is this intended?");
		}
	}

	strcpy(core->run_data->current_level, level_name);
	Log("Successfully loaded level data in from %s", level_name);
	return 1;

	/* save = fopen(path, "r");
	R_DEV_ASSERT(save, "Couldn't open file.");
	FillWorldDataPointersFromFile(save, core->run_data);
	fclose(save); */
}

// Attempts to move level into the root res folder. Only works if being run from arc/game/build
internal void CommitLevel(char *level_name)
{
}