internal Entity *NewEntity();
internal void DeleteEntity(Entity *entity);

inline internal Entity *GetCharacterEntity()
{
	return core->run_data->character_entity;
}

internal b32 EntityHasProperty(Entity *entity, EntityProperty property);
internal void EntitySetProperty(Entity *entity, EntityProperty property);
internal void EntityUnsetProperty(Entity *entity, EntityProperty property);
internal b32 IncrementEntity(Entity **entity_ptr);
internal b32 IncrementEntityWithProperty(Entity **entity_ptr, EntityProperty property);