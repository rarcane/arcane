internal void DrawElementalSkillTreeUI()
{
	if (!(core->run_data->character_state & CHARACTER_STATE_arcane_mode) ||
		!(core->run_data->character_state & CHARACTER_STATE_is_in_elemental_skill_tree))
		return;
	
	Entity *character = GetCharacterEntity();
	Entity *skilltree = core->run_data->engaged_station_entity;
	Assert(skilltree);
	
	ArcPushText(Ts2dGetDefaultFont(),
				0,
				v4u(1.0f),
				v2(0.0f, 0.0f),
				1.0f,
				"YEET",
				LAYER_HUD);
}

internal void OnElementalSkillTreeInteract(Entity *entity)
{
	Entity *character = GetCharacterEntity();
	
	SetArcaneMode(1);
	core->run_data->character_state |= CHARACTER_STATE_is_in_elemental_skill_tree;
	
	core->run_data->engaged_station_entity = entity;
}