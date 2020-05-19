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
	for (int i = 0; i < PERLIN_NOISE_LENGTH; i++)
	{
		for (int j = 0; j < PERLIN_NOISE_LENGTH; j++)
		{
			core->random_field[j][i].x = (RandomI32(0, 1) ? -1.0f : 1.0f);
			core->random_field[i][j].y = (RandomI32(0, 1) ? -1.0f : 1.0f);
		}
	}
}

internal f32 GetPerlinNoise(f32 x_pos, f32 y_pos)
{
	if (x_pos >= 0)
		x_pos = fmodf(x_pos, PERLIN_NOISE_LENGTH);
	else
	{
		if (fmodf(fabsf(x_pos), PERLIN_NOISE_LENGTH) == 0)
			x_pos = 0;
		else
			x_pos = PERLIN_NOISE_LENGTH - fmodf(fabsf(x_pos), PERLIN_NOISE_LENGTH);
	}
    
	if (y_pos >= 0)
		y_pos = fmodf(y_pos, PERLIN_NOISE_LENGTH);
	else
	{
		if (fmodf(fabsf(y_pos), PERLIN_NOISE_LENGTH) == 0)
			y_pos = 0;
		else
			y_pos = PERLIN_NOISE_LENGTH - fmodf(fabsf(y_pos), PERLIN_NOISE_LENGTH);
	}
    
	i32 x_min = (i32)floorf(x_pos);
	i32 x_max = (i32)floorf(x_pos) + 1;
    
	i32 y_min = (i32)floorf(y_pos);
	i32 y_max = (i32)floorf(y_pos) + 1;
    
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
        
		Assert(found_free_index);
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
        
		Assert(found_free_index);
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
			Assert(*chunk_count + 1 < MAX_WORLD_CHUNKS);
            
			SkeletonChunk chunk = {WorldspaceToChunkIndex(rect.x) + x, WorldspaceToChunkIndex(rect.y) + y};
			chunks[(*chunk_count)++] = chunk;
		}
	}
}

internal void WriteToFile(FILE *file, void *data, size_t size_bytes)
{
	fwrite(data, size_bytes, 1, file);
}

internal void ReadFromFile(FILE *file, void *data, size_t size_bytes)
{
	fread(data, size_bytes, 1, file);
}

internal b8 CreateWorld(char *world_name)
{
	Assert(world_name[0]);
    
	char path[300] = "";
	sprintf(path, "%sworlds\\", core->res_path);
    
	// Make the worlds directory if it doesn't already exist.
	if (!platform->DoesDirectoryExist(path))
		platform->MakeDirectory(path);
    
	// Attempt to create the world folder
	strcat(path, world_name);
	strcat(path, "\\");
	if (platform->DoesDirectoryExist(path))
		return 0;
	platform->MakeDirectory(path);
    
	// Create chunks folder
	char chunks_path[300] = "";
	sprintf(chunks_path, "%schunks\\", path);
	platform->MakeDirectory(chunks_path);
    
	strcpy(core->run_data->world_name, world_name);
	strcpy(core->run_data->world_path, path);
	strcpy(core->run_data->world_chunks_path, chunks_path);
    
	SaveLevelData();
	Log("Created new world '%s' successfully.", world_name);
	return 1;
}

internal void SaveLevelData()
{
	Assert(core->run_data->world_path[0]);
    
	char file_path[200] = "";
	sprintf(file_path, "%slevel_data.save", core->run_data->world_path);
	FILE *file = fopen(file_path, "w");
	Assert(file);
    
	// NOTE(randy): Save the player entity
	{
		EntitySave entity_save = {.flags = core->run_data->character_entity->flags, .type = core->run_data->character_entity->generalised_type};
		MemoryCopy(entity_save.name, core->run_data->character_entity->name, sizeof(entity_save.name));
        
		WriteToFile(file, &entity_save, sizeof(EntitySave));
        
		WriteToFile(file, &core->run_data->character_entity->component_ids, sizeof(core->run_data->character_entity->component_ids));
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			if (core->run_data->character_entity->component_ids[i])
			{
				WriteComponentToFile(file, core->run_data->character_entity->component_ids[i], i);
			}
		}
	}
    
	// NOTE(randy): Save world data struct
	WriteWorldSaveDataToFile(file, &core->run_data->world);
    
	// NOTE(randy): Save all of the entities that don't belong to chunks.
	{
		WriteToFile(file, &core->run_data->floating_entity_id_count, sizeof(i32));
		for (i32 i = 0; i < core->run_data->floating_entity_id_count; i++)
		{
			Entity *entity = &core->run_data->entities[core->run_data->floating_entity_ids[i] - 1];
            
			EntitySave entity_save = {.flags = entity->flags, .type = entity->generalised_type};
			MemoryCopy(entity_save.name, entity->name, sizeof(entity_save.name));
            
			WriteToFile(file, &entity_save, sizeof(EntitySave));
		}
        
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			SerialiseEntityComponentsFromIDList(file, core->run_data->entities, &core->run_data->entity_components, core->run_data->floating_entity_ids, core->run_data->floating_entity_id_count, i);
		}
	}
    
	fclose(file);
	Log("Level data saved to %s", file_path);
}

int SaveQueuedChunks(void *job_data)
{
	f32 start_time = platform->GetTime();
	Assert(core->run_data->save_job_index != -1);
	
	// NOTE(randy): Could probably move all the snapshot data out of the RunData struct to make this reentrant but ehhhhhhhh cbf
	
	for (i32 i = 0; i < core->run_data->chunk_save_count; i++)
	{
		ChunkSave *chunk_save = &core->run_data->chunk_saves[i];
		
		char chunk_file_path[300];
		sprintf(chunk_file_path, "%s%i.%i.chunk", core->run_data->world_chunks_path, chunk_save->skele_chunk.x_index, chunk_save->skele_chunk.y_index);
		FILE *file = fopen(chunk_file_path, "w");
		Assert(file);
		
		// NOTE(randy): Write cells to file.
		WriteToFile(file, &chunk_save->cells, sizeof(chunk_save->cells));
		
		// NOTE(randy): Write entities into the chunk file. Layout is as follows.
		// entity count, all entities, component1 count, all component1s, component2 count, all component2s...
		WriteToFile(file, &chunk_save->entity_count, sizeof(chunk_save->entity_count));
		for (i32 i = 0; i < chunk_save->entity_count; i++)
		{
			Entity *entity = &core->run_data->entities_snapshot[chunk_save->entity_ids[i] - 1];
			
			// NOTE(randy): Fill out an EntitySave structure for serialisation
			EntitySave entity_save = {.flags = entity->flags, .type = entity->generalised_type};
			MemoryCopy(entity_save.name, entity->name, sizeof(entity_save.name));
			WriteToFile(file, &entity_save, sizeof(EntitySave));
		}
		for (i32 i = 1; i < COMPONENT_MAX; i++)
		{
			SerialiseEntityComponentsFromIDList(file, core->run_data->entities_snapshot, &core->run_data->entity_components_snapshot, chunk_save->entity_ids, chunk_save->entity_count, i);
		}
		
		fclose(file);
	}
	
	Log("Save job completed in %fms", (platform->GetTime() - start_time) * 1000.0f);
	
	return 0;
}

internal b8 QueueChunkForSave(Chunk *chunk)
{
	Assert(chunk && chunk->is_valid);
	
	// NOTE(randy): Ensure chunk is not already queued up
	for (i32 i = 0; i < core->run_data->chunk_save_count; i++)
	{
		ChunkSave *chunk_save = &core->run_data->chunk_saves[i];
		if (chunk_save->skele_chunk.x_index == chunk->x_index && chunk_save->skele_chunk.y_index == chunk->y_index)
		{
			return 0;
		}
	}
	
	ChunkSave *chunk_save = &core->run_data->chunk_saves[core->run_data->chunk_save_count++];
	chunk_save->skele_chunk.x_index = chunk->x_index;
	chunk_save->skele_chunk.y_index = chunk->y_index;
	MemoryCopy(&chunk_save->cells, &chunk->cells, sizeof(chunk->cells));
	chunk_save->entity_count = chunk->entity_count;
	
	if (chunk->entity_count)
	{
		i64 offset = chunk->entity_ids - core->run_data->positional_entity_ids;
		chunk_save->entity_ids = core->run_data->positional_entity_ids_snapshot + offset;
	}
	else
	{
		chunk_save->entity_ids = 0;
	}
	
	return 1;
}

internal void SaveWorld()
{
	//BLOCK_TIMER("World save copy",
	if (core->run_data->chunk_save_count > 0)
	{
		LogWarning("A save is already in progress.");
		return;
	}
	
	SaveLevelData();
	
	// NOTE(randy): Capture a snapshot of the world's data
	{
		MemoryCopy(&core->run_data->entities_snapshot, &core->run_data->entities, sizeof(core->run_data->entities));
		MemoryCopy(&core->run_data->entity_count_snapshot, &core->run_data->entity_count, sizeof(core->run_data->entity_count));
		MemoryCopy(&core->run_data->entity_components_snapshot, &core->run_data->entity_components, sizeof(core->run_data->entity_components));
		MemoryCopy(&core->run_data->positional_entity_ids_snapshot, &core->run_data->positional_entity_ids,
				   sizeof(core->run_data->positional_entity_ids));
		MemoryCopy(&core->run_data->positional_entity_id_count_snapshot, &core->run_data->positional_entity_id_count, sizeof(core->run_data->positional_entity_id_count));
		
		for (i32 i = 0; i < core->run_data->active_chunk_count; i++)
		{
			Chunk *chunk = &core->run_data->active_chunks[i];
			
			if (chunk->is_valid)
			{
				Assert(QueueChunkForSave(chunk));
			}
		}
	}
	
	
	core->run_data->save_job_index = platform->QueueJob(0, SaveQueuedChunks, 0);
	
	// TODO(randy): Try doing a malloc and free for the snapshop data to see if it's more performant and we won't need to store scratch in RunData
	
	//);
}

internal b8 LoadWorld(char *world_name)
{
	// TODO(randy): bulletproof world name sizing
	Assert(world_name[0] && strlen(world_name) < sizeof(core->run_data->world_name));
    
	char path[300] = "";
	sprintf(path, "%sworlds\\%s\\", core->res_path, world_name);
    
	if (!platform->DoesDirectoryExist(path))
		return 0;
    
	{
		FreeRunData();
		MemorySet(core->run_data, 0, sizeof(RunData));
		strcpy(core->run_data->world_name, world_name);
		strcpy(core->run_data->world_path, path);
        
		// TODO: refactor this
		InitialiseRunData();
		InitialiseWorldData();
		// NOTE(randy): Might want to seperate out some run data that is permanent
		// and will not change between saves
	}
    
	// Read in basic data from the world_data.save
	{
		char file_path[200] = "";
		sprintf(file_path, "%slevel_data.save", path);
		FILE *save = fopen(file_path, "r");
		Assert(save);
        
		// NOTE(randy): Read the player entity in
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
        
		// NOTE(randy): Read in basic world data
		ReadWorldSaveDataFromFile(save, &core->run_data->world);
        
		// NOTE(randy): Read in floating entities
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
		char file_path[300] = "";
		sprintf(file_path, "%schunks\\", path);
		Assert(platform->DoesDirectoryExist(file_path));
		strcpy(core->run_data->world_chunks_path, file_path);
        
		TransformInGameCamera();
        
		SkeletonChunk chunks[MAX_WORLD_CHUNKS];
		i32 chunk_count;
		GetSkeletonChunksInRegion(chunks, &chunk_count, GetCameraRegionRect(), 1);
        
		for (i32 i = 0; i < chunk_count; i++)
		{
			Chunk *chunk = LoadChunkFromDisk(file_path, chunks[i].x_index, chunks[i].y_index);
            
			if (!chunk)
				LogError("A surrounding chunk in the player's region doesn't exist, is this intended?");
		}
	}
    
	core->is_ingame = 1;
    
	Log("Successfully loaded world \'%s\'", world_name);
	return 1;
}

internal void UnloadWorld()
{
	Assert(core->is_ingame && core->run_data->world_name[0]);
    
	SaveWorld();
	Assert(core->run_data->save_job_index != -1);
	platform->WaitForJob(core->run_data->save_job_index, TS_WAIT_FOREVER);
    
	FreeRunData();
	MemorySet(core->run_data, 0, sizeof(RunData));
	core->is_ingame = 0;
}