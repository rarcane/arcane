internal void RenderColliders()
{
	/* 	for (int i = 0; i < core->world_data->entity_components.collider_component_count; i++)
	{
		ColliderComponent *collider_comp = &core->world_data->entity_components.collider_components[i];
		if (collider_comp->parent_entity)
		{
			PositionComponent *position_comp = collider_comp->parent_entity->components[COMPONENT_position];
			R_DEV_ASSERT(position_comp, "Position component not found?");

			v2 world_pos = position_comp->position;
			v2 view_pos = v2view(world_pos);

			for (int j = 0; j < collider_comp->shape.vertex_count; j++)
			{
				int secondPoint = (j == collider_comp->shape.vertex_count - 1 ? 0 : j + 1);
				Ts2dPushLine(
					core->renderer,
					v4(1.0f, 1.0f, 1.0f, 1.0f),
					V2AddV2(view_pos, v2(collider_comp->shape.vertices[j].x * core->camera_zoom, collider_comp->shape.vertices[j].y * core->camera_zoom)),
					V2AddV2(view_pos, v2(collider_comp->shape.vertices[secondPoint].x * core->camera_zoom, collider_comp->shape.vertices[secondPoint].y * core->camera_zoom)));
			}
		}
	} */
}

internal void UpdatePhysics()
{
	for (int i = 0; i < core->world_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->world_data->entity_components.physics_body_components[i];
		Entity *entity = body_comp->parent_entity;
		if (entity)
		{
			PositionComponent *pos_comp = entity->components[COMPONENT_position];
			R_DEV_ASSERT(pos_comp, "Physics entity doesn't have a position component.");

			// Apply gravity
			body_comp->force.y = WORLD_GRAVITY * body_comp->gravity_multiplier;

			// Integrate next position
			v2 acceleration = V2MultiplyF32(body_comp->force, body_comp->mass_data.inv_mass);
			body_comp->velocity = V2AddV2(body_comp->velocity, V2MultiplyF32(acceleration, core->world_delta_t));
			pos_comp->position = V2AddV2(pos_comp->position, V2MultiplyF32(body_comp->velocity, core->world_delta_t));

			body_comp->force = v2(0.0f, 0.0f);
		}
	}

	// Generate pairs that need to be collision tested
	// Loop through each pair
	// Test for collision and resolve any collision via impulse
}