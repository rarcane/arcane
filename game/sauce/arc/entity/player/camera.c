static void PositionCamera()
{
	if (core->player)
	{
		PositionComponent *position_comp = core->player->components[COMPONENT_position];
		core->camera_position = V2AddV2(GetZeroWorldPosition(), V2AddV2(v2(core->camera_offset.x, core->camera_offset.y + CAMERA_OFFSET_Y), V2MultiplyF32(position_comp->position, -1.0f)));
	}
}