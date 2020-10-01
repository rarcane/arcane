internal void PreMoveUpdatePlayer();
internal void PostMoveUpdatePlayer();
internal b8 CanPlayerMove();
internal void SetArcaneMode(i32 on);

inline internal Entity *GetCharacterEntity()
{
	return core->run_data->character_entity;
}

inline internal Item *GetItemInHand()
{
	Item *item = &core->run_data->character_data.hotbar[core->run_data->character_data.active_hotbar_slot];
	if (item->type)
		return item;
	else
		return 0;
}