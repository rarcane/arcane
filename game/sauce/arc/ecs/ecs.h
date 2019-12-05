/* --- ECS Core --- */

#define ENTITY_FLAG_no_delete (1 << 0)
typedef struct Entity
{
	i32 entity_id;
	char name[20];
	EntityType type;
	b16 flags;
	void *components[COMPONENT_MAX];
	EntityReference *references[MAX_ENTITY_REFERENCES];
	i32 reference_count;
	i32 free_reference_index;
} Entity;

typedef struct EntitySet
{
	Entity entities[MAX_ENTITIES];
	i32 entity_count;
	i32 free_entity_id;
} EntitySet;

internal Entity *NewEntity(char *name, EntityType type);
internal void AddComponent(Entity *entity, ComponentType component_type, void *component);
internal void RemoveComponent(Entity *entity, ComponentType component_type);
internal void RequestEntity(Entity *entity, EntityReference *entity_reference);
internal void FreeEntityReference(EntityReference *reference);
internal void DeleteEntity(Entity *entity);