static void PositionCamera()
{
	v2 zero_world_position = {core->render_w / 2.0f / core->camera_zoom, core->render_h / 2.0f / core->camera_zoom};

	if (core->player)
	{
		PositionComponent *position_comp = core->player->components[COMPONENT_position];
		core->camera_offset = V2AddV2(v2(0.0f, 45.0f), V2MultiplyF32(position_comp->position, -1.0f));
	}

	core->camera_position = V2AddV2(zero_world_position, core->camera_offset);
}