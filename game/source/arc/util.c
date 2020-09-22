f32 sigmoid(f32 x)
{
	f32 exp_value = (f32)exp(-x);
	return  1 / (1 + exp_value);
}

f32 EaseOut(f32 x)
{
	return 1 - (1 - x) * (1 - x);
}

f32 Clamp(f32 min, f32 max, f32 value)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
	
	return value;
}

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
	f32 x = a * a + b * b;
	//Based on Quake's fast inverse square root
	u32 i = *(u32*)&x;//Making bits an int-type, so that we can bit hack
	i = 0x1fc00000 + (i >> 1);//Little sus, but it works
	f32 y = *(f32*)&i;//Back to float
	
	y = (y * y + x) / (2 * y);//Runs a couple iterations of the Newton-Raphson method
	y = (y * y + x) / (2 * y);//Keep adding more to increase accuracy
	return y;
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

internal void AddPositionOffsetToShape(c2Shape *shape, c2ShapeType shape_type, v2 position)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb :
		{
			shape->aabb.min.x += position.x;
			shape->aabb.min.y += position.y;
			shape->aabb.max.x += position.x;
			shape->aabb.max.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_capsule :
		{
			shape->capsule.a.x += position.x;
			shape->capsule.a.y += position.y;
			shape->capsule.b.x += position.x;
			shape->capsule.b.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_poly :
		{
			// TODO(randy): Test this out to ensure it's working.
			for (int i = 0; i < shape->poly.count; i++)
			{
				shape->poly.verts[i].x += position.x;
				shape->poly.verts[i].y += position.y;
			}
			c2MakePoly(&shape->poly);
		} break;
		
		case C2_SHAPE_TYPE_line :
		{
			shape->line.p1 = V2AddV2(shape->line.p1, position);
			shape->line.p2 = V2AddV2(shape->line.p2, position);
		} break;
		
		case C2_SHAPE_TYPE_circle :
		{
			shape->circle.p.x += position.x;
			shape->circle.p.y += position.y;
		} break;
		
		default :
		Assert(0);
		break;
	}
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
        case C2_SHAPE_TYPE_aabb :
        {
            v2 p0 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.min.y));
            v2 p1 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.min.y));
            v2 p2 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.max.y));
            v2 p3 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.max.y));
            
            PushDebugLine(p0, p1, colour);
            PushDebugLine(p1, p2, colour);
            PushDebugLine(p2, p3, colour);
            PushDebugLine(p3, p0, colour);
        } break;
        
        case C2_SHAPE_TYPE_poly :
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
        } break;
        
        case C2_SHAPE_TYPE_line :
        {
            PushDebugLine(V2AddV2(position, shape.line.p1),
                          V2AddV2(position, shape.line.p2),
                          colour);
        } break;
		
		case C2_SHAPE_TYPE_circle :
		{
			position = V2AddV2(position, v2(shape.circle.p.x, shape.circle.p.y));
			
			v2 p0 = V2AddV2(position, v2(shape.circle.r, shape.circle.r));
            v2 p1 = V2AddV2(position, v2(shape.circle.r, -shape.circle.r));
            v2 p2 = V2AddV2(position, v2(-shape.circle.r, -shape.circle.r));
            v2 p3 = V2AddV2(position, v2(-shape.circle.r, shape.circle.r));
            
            PushDebugLine(p0, p1, colour);
            PushDebugLine(p1, p2, colour);
            PushDebugLine(p2, p3, colour);
            PushDebugLine(p3, p0, colour);
		} break;
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
    
	i32 width = WorldSpaceToChunkIndex(rect.x + rect.width) - WorldSpaceToChunkIndex(rect.x);
	i32 height = WorldSpaceToChunkIndex(rect.y + rect.height) - WorldSpaceToChunkIndex(rect.y);
    
	for (int y = -buffer; y <= height + buffer; y++)
	{
		for (int x = -buffer; x <= width + buffer; x++)
		{
			Assert(*chunk_count + 1 < MAX_WORLD_CHUNKS);
            
			SkeletonChunk chunk = {WorldSpaceToChunkIndex(rect.x) + x, WorldSpaceToChunkIndex(rect.y) + y};
			chunks[(*chunk_count)++] = chunk;
		}
	}
}

internal Entity *GetClosestEntityWithProperty(EntityProperty property, f32 max_distance_from_player)
{
	// NOTE(randy): This will eventually prioritise player direction and whatnot to fine tune selection
	Entity *character = GetCharacterEntity();
	
	Entity *closest_entity = 0;
	f32 closest_mag = 0.0f;
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, property);)
	{
		if (closest_entity)
		{
			f32 entity_mag = fabsf(PythagSolve(character->position.x - entity->position.x,
											   character->position.y - entity->position.y));
			if (entity_mag < closest_mag)
			{
				closest_entity = entity;
				closest_mag = entity_mag;
			}
		}
		else
		{
			closest_entity = entity;
			closest_mag = fabsf(PythagSolve(character->position.x - entity->position.x,
											character->position.y - entity->position.y));
		}
	}
	
	return (closest_mag <= max_distance_from_player ? closest_entity : 0);
}

internal void WriteToFile(FILE *file, void *data, size_t size_bytes)
{
	if (fwrite(data, size_bytes, 1, file) != 1)
	{
		Assert(ferror(file) == 0); // NOTE(randy): Ensure this error is caught.
		Assert(0);
	}
}

internal void ReadFromFile(FILE *file, void *data, size_t size_bytes)
{
	if (fread(data, size_bytes, 1, file) != 1)
	{
		Assert(feof(file) == 0);
		Assert(ferror(file) == 0);
	}
}