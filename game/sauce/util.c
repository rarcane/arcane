internal v2 NormaliseV2(v2 vector)
{
	v2 normal_vector = V2DivideV2(vector, vector);

	if (isnan(normal_vector.x) || !isfinite(normal_vector.x))
		normal_vector.x = 0.0f;
	if (isnan(normal_vector.y) || !isfinite(normal_vector.y))
		normal_vector.y = 0.0f;

	return normal_vector;
}