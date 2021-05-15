internal void ElementalSkillNodeRender(ElementalSkillType skill, i32 depth, i32 x_offset, v2 centre_pos, v4 clip)
{
	Assert(skill);
	ElementalSkillTypeData *skill_data = &global_elemental_skill_type_data[skill];
	
	// NOTE(randy): Render Skill
	v2 render_pos = v2view(v2(centre_pos.x + x_offset * 30.0f, centre_pos.y - depth * 30.0f));
	v2 render_size = v2zoom(v2(10.0f, 10.0f));
	
	v3 hsl_colour = RGBToHSV(v3(1.0f, 1.0f, 1.0f));
	b8 is_hovered = IsV2InRect(v2(platform->mouse_x, platform->mouse_y), v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
	
	if (is_hovered)
	{
		hsl_colour = v3(0.0f, 0.0f, 0.8f);
	}
	v3 rgb_colour = HSVToRGB(hsl_colour);
	
	if (is_hovered && platform->left_mouse_pressed)
	{
		platform->left_mouse_pressed = 0;
		
		// TODO(randy): Left click functionality
	}
	
	SpriteData *sprite = &global_sprite_data[skill_data->skill_image];
	
	ArcPushTextureWithClip(sprite->texture_atlas,
						   0,
						   sprite->source,
						   v4(render_pos.x, render_pos.y,
							  render_size.x, render_size.y),
						   v4(rgb_colour.x, rgb_colour.y, rgb_colour.z, 1.0f),
						   LAYER_HUD,
						   clip);
	
	// werewolfdev is a fucking CHAMPION that likes to get shit done.
	
	// NOTE(randy): Loop over children
	for (i32 i = 0; i < MAX_CHILD_SKILLS; i++)
	{
		if (skill_data->child_skills[i])
		{
			ElementalSkillNodeRender(skill_data->child_skills[i], depth + 1, x_offset + i, centre_pos, clip);
		}
	}
}

internal void ElementalSkillTreeUIDraw()
{
	if (!(core->run_data->character_state & CHARACTER_STATE_arcane_mode) ||
		!(core->run_data->character_state & CHARACTER_STATE_is_in_elemental_skill_tree))
		return;
	
	Entity *character = GetCharacterEntity();
	Entity *skilltree = core->run_data->engaged_station_entity;
	Assert(skilltree);
	
	local_persist v2 offset = {0};
	v2 skill_tree_size = { 90.0f, 90.0f };
	v2 skill_tree_pos = V2AddV2(skilltree->position,
								v2(skill_tree_size.x / -2.0f, -120.0f));
	
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
	
	v2 render_pos = v2view(skill_tree_pos);
	v2 render_size = v2zoom(skill_tree_size);
	
	ElementalSkillNodeRender(ELEMENTAL_SKILL_TYPE_hand_flame, 0, 0, centre_pos, v4(render_pos.x, render_pos.y,
																				   render_size.x, render_size.y));
	
	/*
		ArcPushTextWithClip(Ts2dGetDefaultFont(),
							0,
							v4u(1.0f),
							v2view(centre_pos),
							0.1f * core->camera_zoom,
							"YEET",
							LAYER_HUD,
							v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
	 */
	
	ArcPushFilledRect(v4(214.0f / 255.0f, 134.0f / 255.0f, 118.0f / 255.0f, 0.5f),
					  v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
					  LAYER_HUD + 0.01f);
	
	if (platform->key_pressed[KEY_esc])
	{
		TsPlatformCaptureKeyboard();
		SetArcaneMode(0);
		
		core->camera_zoom_mult = 1.0f;
		core->run_data->lock_camera = 0;
	}
}

internal b8 IsElementalSkillUnlocked(ElementalSkillType skill_type)
{
	return GetCharacterData()->unlocked_elemental_skills[skill_type];
}

internal void SetElementalSkillUnlocked(ElementalSkillType skill_type)
{
	GetCharacterData()->unlocked_elemental_skills[skill_type] = 1;
}

internal b8 IsElementalSkillPurchased(ElementalSkillType skill_type)
{
	return GetCharacterData()->purchased_elemental_skills[skill_type];
}

internal b8 PurchaseElementalSkill(ElementalSkillType skill_type)
{
	ElementalSkillTypeData *skill_data = &global_elemental_skill_type_data[skill_type];
	
	if (GetCharacterData()->elemental_skill_points - skill_data->cost >= 0)
	{
		GetCharacterData()->purchased_elemental_skills[skill_type] = 1;
		return 1;
	}
	
	return 0;
}

internal ElementalSkillTreeUpdate()
{
	// NOTE(randy): All elemental skills that require some sort of update and tick functionality will be put here in this big chungus motherfucker of a function
	
	if (1) // NOTE(randy): Some condition is met that would unlock a skill
	{
		SetElementalSkillUnlocked(ELEMENTAL_SKILL_TYPE_hand_flame);
		
		PurchaseElementalSkill(ELEMENTAL_SKILL_TYPE_hand_flame);
	}
	
	if (IsElementalSkillPurchased(ELEMENTAL_SKILL_TYPE_hand_flame) && platform->key_pressed[KEY_u])
	{
		GetCharacterEntity()->sprite_data.tint = v4u(0.5f);
	}
}