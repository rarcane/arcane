internal void DrawElementalSkillTreeUI()
{
	if (!(core->run_data->character_state & CHARACTER_STATE_arcane_mode) ||
		!(core->run_data->character_state & CHARACTER_STATE_is_in_elemental_skill_tree))
		return;
	
	Entity *character = GetCharacterEntity();
	Entity *skilltree = core->run_data->engaged_station_entity;
	Assert(skilltree);
	
	local_persist v2 offset = {0};
	
	if (platform->left_mouse_down)
	{
		local_persist v2 last_mouse = {0};
		if (platform->left_mouse_pressed)
		{
			last_mouse = GetMousePositionInWorldSpace();
		}
		
		offset = V2AddV2(offset, V2SubtractV2(GetMousePositionInWorldSpace(), last_mouse));
		
		last_mouse = GetMousePositionInWorldSpace();
	}
	
	v2 centre_pos = V2AddV2(skilltree->position, v2(offset.x, -50.0f + offset.y));
	
	v2 canvas_pos = v2view(V2AddV2(skilltree->position, v2(-50.0f, -100.0f)));
	v2 canvas_size = v2zoom(v2(100.0f, 100.0f));
	
	ArcPushTextWithClip(Ts2dGetDefaultFont(),
						0,
						v4u(1.0f),
						v2view(centre_pos),
						0.1f * core->camera_zoom,
						"YEET",
						LAYER_HUD,
						v4(canvas_pos.x, canvas_pos.y, canvas_size.x, canvas_size.y));
	
	if (platform->key_pressed[KEY_esc])
	{
		TsPlatformCaptureKeyboard();
		SetArcaneMode(0);
		
		core->camera_zoom_mult = 1.0f;
		core->run_data->lock_camera = 0;
	}
}

internal void OnElementalSkillTreeInteract(Entity *entity)
{
	Entity *character = GetCharacterEntity();
	
	SetArcaneMode(1);
	core->run_data->character_state |= CHARACTER_STATE_is_in_elemental_skill_tree;
	
	core->run_data->engaged_station_entity = entity;
	
	MoveCameraToLocation(V2AddV2(entity->position, v2(0.0f, -50.0f)), 2.0f, 2.0f);
}