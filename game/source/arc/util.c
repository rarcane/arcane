// NOTE(tjr): Smooths a linear 0.0 -> 1.0
internal f32 Fade(f32 alpha)
{
	return 6 * powf(alpha, 5) - 15 * powf(alpha, 4) + 10 * powf(alpha, 3);
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
		core->world_data->elapsed_world_time,
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

		if (debug_line->is_valid && debug_line->has_duration && debug_line->start_time + debug_line->lifetime <= core->world_data->elapsed_world_time)
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
				alpha = ((debug_line->start_time + debug_line->lifetime) - core->world_data->elapsed_world_time) / debug_line->lifetime;
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

internal void WriteToFile(FILE *file, void *data, size_t size_bytes)
{
	fwrite(data, size_bytes, 1, file);
}

internal void ReadFromFile(FILE *file, void *data, size_t size_bytes)
{
	fread(data, size_bytes, 1, file);
}

// NOTE(tjr): Saves current data to a specified level.
internal void SaveLevel(char *level_name)
{
	R_DEV_ASSERT(level_name[0], "Invalid name.");

	char path[200] = "";
	sprintf(path, "%s%s.save", core->client_data->res_path, level_name);
	FILE *save = fopen(path, "w");
	R_DEV_ASSERT(save, "Couldn't open file.");

	WriteWorldDataToFile(save, core->world_data);
	fclose(save);

	save = fopen(path, "r+");
	fseek(save, 0, SEEK_SET);
	FillWorldDataPointersInFile(save, core->world_data);
	fclose(save);
}

// NOTE(tjr): Loads a given level. Returns 0 if the level doesn't exist.
internal b8 LoadLevel(char *level_name)
{
	char path[200] = "";
	sprintf(path, "%s%s.save", core->client_data->res_path, level_name);
	FILE *save = fopen(path, "r");
	if (!save)
		return 0;

	strcpy(core->client_data->current_level, level_name);

	// ...

	fclose(save);
	return 1;
}

// NOTE(tjr): Attempts to move level into the root res folder. Only works if being run from arc/game/build
internal void CommitLevel(char *level_name)
{
}