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

internal int GetSign(f32 x)
{
	return (x > 0) - (x < 0);
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