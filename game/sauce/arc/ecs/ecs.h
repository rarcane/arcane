typedef struct Entity
{
	i32 entity_id;
	char name[20];
	EntityType type;
	EntityFlags flags;
	void *components[COMPONENT_MAX];
} Entity;

typedef struct EntitySet
{
	Entity entities[MAX_ENTITIES];
	i32 entity_count;
	i32 free_entity_id;
} EntitySet;

internal Entity *NewEntity(char *name, EntityType type);
// internal void AddComponent(Entity *entity, ComponentType component_type, void *component);
// internal void RemoveComponent(Entity *entity, ComponentType component_type);