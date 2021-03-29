EntityPresetCallback(GroundSegment)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	EntitySetProperty(entity, ENTITY_PROPERTY_ground_segment);
	EntitySetProperty(entity, ENTITY_PROPERTY_map_entity);
	char *name = "ground segment";
	strcpy(entity->debug_name, name);
	
	/*
		v4 camera_region = GetCameraRegionRect();
		entity->position = v2(-camera_region.x - camera_region.z / 2.0f,
							  -camera_region.y - camera_region.w / 2.0f);
	 */
	
	LineSegments lineseg = {0};
	for (i32 i = 0; i < 17; i++)
	{
		f32 x_pos = i * 16.0f + entity->position.x;
		
		i32 octaves = 8;
		f32 frequency = 0.2f;
		f32 amplitude = 20.0f;
		f32 max_noise_value = 0.0f;
		f32 noise_amount = 0.0f;
		for (int k = 0; k < octaves; k++)
		{
			noise_amount += GetPerlinNoise(x_pos / PERLIN_NOISE_LENGTH * frequency, 0.0f) * amplitude;
			max_noise_value += amplitude;
			frequency *= 2.0f;
			amplitude *= 0.5f;
		}
		f32 height_noise = noise_amount / max_noise_value;
		
		f32 height = height_noise * 100.0f;
		
		lineseg.vertices[i] = v2(i * 16.0f, height);
	}
	lineseg.count = 17;
	entity->physics.shape.line_segments = lineseg;
	entity->physics.shape_type = C2_SHAPE_TYPE_line_segments;
	
	entity->physics.material.static_friction = 0.7f;
	entity->physics.material.dynamic_friction = 0.7f;
	entity->physics.mass_data.mass = 0.0f;
	entity->physics.mass_data.inv_mass = 0.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_ground;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_character | PHYSICS_BODY_TYPE_FLAGS_item;
}