// NOTE(tjr): Smooths a linear 0.0 -> 1.0
internal f32 Fade(f32 alpha)
{
	return 6 * powf(alpha, 5) - 15 * powf(alpha, 4) + 10 * powf(alpha, 3);
}

internal v2 NormaliseV2(v2 vector)
{
	v2 normal_vector = V2DivideV2(vector, vector);

	if (isnan(normal_vector.x) || !isfinite(normal_vector.x))
		normal_vector.x = 0.0f;
	if (isnan(normal_vector.y) || !isfinite(normal_vector.y))
		normal_vector.y = 0.0f;

	return normal_vector;
}

internal i32 GetSign(f32 x)
{
	return (x > 0.0f) - (x < 0.0f);
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

internal c2Poly v2AddPoly(v2 a, c2Poly poly)
{
	for (int i = 0; i < poly.count; i++)
	{
		poly.verts[i].x += a.x;
		poly.verts[i].y += a.y;
	}

	return poly;
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