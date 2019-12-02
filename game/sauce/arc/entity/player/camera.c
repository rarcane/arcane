static void TransformEditorCamera()
{
	f32 transform_mult = platform->key_down[KEY_shift] ? 2.0f : 1.0f;

	if (platform->mouse_scroll_y != 0.0f)
	{
		core->camera_zoom += platform->mouse_scroll_y / 120.0f / 4 * transform_mult;
		if (core->camera_zoom <= 0.1f)
			core->camera_zoom = 0.1f;
		else if (core->camera_zoom > 10.0f)
			core->camera_zoom = 10.0f;
	}

	if (platform->key_down[KEY_a])
	{
		core->camera_position.x += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_d])
	{
		core->camera_position.x -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	if (platform->key_down[KEY_w])
	{
		core->camera_position.y += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_s])
	{
		core->camera_position.y -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
}

static void TransformInGameCamera()
{
	if (!core->is_in_editor && core->player)
	{
		PositionComponent *position_comp = core->player->components[COMPONENT_position];
		core->camera_zoom = DEFAULT_CAMERA_ZOOM;
		core->camera_position = V2AddV2(v2(core->camera_offset.x, core->camera_offset.y + DEFAULT_CAMERA_OFFSET_Y), V2MultiplyF32(position_comp->position, -1.0f));
	}
}