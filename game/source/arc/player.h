internal void PreMoveUpdatePlayer();
internal void PostMoveUpdatePlayer();
internal b8 CanPlayerMove();

inline internal Entity *GetCharacterEntity()
{
	return core->run_data->character_entity;
}

inline internal Item *GetItemInHand()
{
	return &core->run_data->character_entity->hotbar[core->run_data->character_entity->active_hotbar_slot];
}