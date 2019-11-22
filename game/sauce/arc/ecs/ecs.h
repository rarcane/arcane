#define MAX_ENTITIES 1000
#define MAX_OVERLAPPING_COLLIDERS 50
#define MAX_ENTITY_REFERENCES 10
#define MAX_SUB_COLLIDERS 3

typedef enum ComponentType ComponentType;
typedef struct ComponentSet ComponentSet;
typedef struct Entity Entity;
typedef struct EntitySet EntitySet;

internal Entity *NewEntity(char *name);
internal void AddComponent(Entity *entity, ComponentType component_type, void *component);
internal void RemoveComponent(Entity *entity, ComponentType component_type);
internal void RequestEntity(Entity *entity, EntityReference *entity_reference);
internal void FreeEntityReference(EntityReference *reference);
internal void DeleteEntity(Entity *entity);

typedef struct PositionComponent
{
	i32 entity_id;
	i32 component_id;
	v2 position;
} PositionComponent;

typedef struct SpriteData
{
	SpriteType sprite_enum;
	v2 offset;
	f32 render_layer;
	v2 scale;
} SpriteData;
typedef struct SpriteComponent
{
	i32 entity_id;
	i32 component_id;
	SpriteData sprite_data;
	b8 is_flipped;
} SpriteComponent;
typedef struct SubSpriteComponent
{
	i32 entity_id;
	i32 component_id;
	SpriteData sub_sprites[MAX_SUB_SPRITES];
	i32 sub_sprite_count;
	b8 is_flipped;
} SubSpriteComponent;

#define ANIMATION_FLAG_playing (1 << 0)
#define ANIMATION_FLAG_reversing (1 << 1)
#define ANIMATION_FLAG_repeat (1 << 2)
typedef struct AnimationComponent
{
	i32 entity_id;
	i32 component_id;
	b8 flags;
	i32 current_frame;
	f32 interval_mult;
	f32 frame_start_time;
} AnimationComponent;

#define COLLIDER_FLAG_ground (1 << 0)
#define COLLIDER_FLAG_player (1 << 1)
#define COLLIDER_FLAG_entity (1 << 2)
#define COLLIDER_FLAG_item (1 << 3)
#define COLLIDER_FLAG_trigger (1 << 4)
typedef struct Shape
{
	v2 vertices[10];
	i32 vertex_count;
} Shape;
typedef struct ColliderComponent
{
	i32 entity_id;
	i32 component_id;
	Shape shape;
	b8 flags;
} ColliderComponent;

typedef struct VelocityComponent
{
	i32 entity_id;
	i32 component_id;
	v2 velocity;
	v2 ideal_velocity;
	v2 acceleration; // You'd have this be dynamic depending on the alpha of the ideal/current velocity to have a multi-phased acceleration function.
	v2 acceleration_mult;
	f32 previous_friction;
	b8 collide_against;
} VelocityComponent;

typedef struct PhysicsComponent
{
	i32 entity_id;
	i32 component_id;
	f32 friction_mult;
	f32 bounce_mult;
} PhysicsComponent;

typedef struct MovementComponent
{
	i32 entity_id;
	i32 component_id;
	f32 axis_x;
	f32 move_speed;
	f32 move_speed_mult;
} MovementComponent;

typedef struct ArcEntityComponent
{
	i32 entity_id;
	i32 component_id;
	ArcEntityType entity_type;
	char *current_general_state;
	AnimationStateType current_animation_state;
} ArcEntityComponent;

typedef struct ItemComponent
{
	i32 entity_id;
	i32 component_id;
	ItemType item_type;
	i32 stack_size;
} ItemComponent;

typedef struct OverlappedColliderInfo
{
	EntityReference overlapped_entity;
	// direction or some shit idk?
} OverlappedColliderInfo;

typedef struct TriggerComponent
{
	i32 entity_id;
	i32 component_id;
	void (*enter_trigger_callback)(OverlappedColliderInfo);
	void (*exit_trigger_callback)(OverlappedColliderInfo);
	OverlappedColliderInfo previous_overlaps[MAX_OVERLAPPING_COLLIDERS];
	i32 previous_overlaps_count;
	b8 trigger_against;
} TriggerComponent;

// Programming style: Would there ever be a situation when a component is just randomly deleted? Probs not.
// So holding onto pointers to components should be fine? The only way in which they get deleted are through specific
// interaction, therefore allowing us to nullify them ourself. I don't think an item will be magically plucked out of thin air?
#define MAX_STORAGE_COMPONENT_SIZE 30
typedef struct StorageComponent
{
	i32 entity_id;
	i32 component_id;
	i32 storage_size;
	ItemComponent *items[MAX_STORAGE_COMPONENT_SIZE];
} StorageComponent;

/* --- ECS Core --- */

enum ComponentType
{
#define Component(component_struct, component_name) COMPONENT_##component_name,
#include "components.inc"
#undef Component
	MAX_COMPONENTS,
};

struct ComponentSet
{
#define Component(component_struct, component_name)                        \
	component_struct##Component component_name##_components[MAX_ENTITIES]; \
	i32 component_name##_component_count;                                  \
	i32 component_name##_free_component_id;
#include "components.inc"
#undef Component
};

struct Entity
{
	i32 entity_id;
	char name[20];
	void *components[MAX_COMPONENTS];
	EntityReference *references[MAX_ENTITY_REFERENCES];
	i32 reference_count;
	i32 free_reference_index;
};

struct EntitySet
{
	Entity entities[MAX_ENTITIES];
	i32 entity_count;
	i32 free_entity_id;
};