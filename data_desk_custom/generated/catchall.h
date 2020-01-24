// @Define 
#define MAX_ENTITIES_PER_CHUNK (128)
// @Define 
#define MAX_ACTIVE_ENTITIES (1024)
// @Define 
#define MAX_OVERLAPPING_COLLIDERS (50)
// @Define 
#define MAX_ENTITY_REFERENCES (10)
// @Define 
#define MAX_SUB_COLLIDERS (3)
// @Define 
#define MAX_STORAGE_SIZE (30)
// @Define 
#define MAX_PARTICLE_AMOUNT (300)
// @Define 
#define MAX_WORLD_CHUNKS (128)
// @Define 
#define CHUNK_SIZE (200)
typedef enum EntityType EntityType;
enum EntityType
{
ENTITY_TYPE_undefined,
ENTITY_TYPE_character,
ENTITY_TYPE_monster,
ENTITY_TYPE_animal,
ENTITY_TYPE_item,
ENTITY_TYPE_storage,
ENTITY_TYPE_resource,
ENTITY_TYPE_scenic,
ENTITY_TYPE_ground,
ENTITY_TYPE_MAX,
};
static char *GetEntityTypeTypeName(EntityType type);

#define ENTITY_FLAGS_no_delete (1<<0)
typedef unsigned int EntityFlags;

#define COLLIDER_FLAGS_ground (1<<0)
#define COLLIDER_FLAGS_player (1<<1)
#define COLLIDER_FLAGS_entity (1<<2)
#define COLLIDER_FLAGS_item (1<<3)
#define COLLIDER_FLAGS_trigger (1<<4)
typedef unsigned int ColliderFlags;

#define ANIMATION_FLAGS_playing (1<<0)
#define ANIMATION_FLAGS_reversing (1<<1)
#define ANIMATION_FLAGS_repeat (1<<2)
typedef unsigned int AnimationFlags;

#define PARTICLE_EMITTER_FLAGS_repeat (1<<0)
typedef unsigned int ParticleEmitterFlags;

typedef struct Entity Entity;

typedef struct ChunkData ChunkData;

typedef struct PositionComponent
{
Entity *parent_entity;
i32 component_id;
v2 position;
} PositionComponent;

typedef struct SpriteComponent
{
Entity *parent_entity;
i32 component_id;
SpriteData sprite_data;
// @Editable 
b8 is_flipped;
b8 is_background_sprite;
} SpriteComponent;

typedef struct SubSpriteComponent
{
Entity *parent_entity;
i32 component_id;
SpriteData sub_sprites[MAX_SUB_SPRITES];
i32 sub_sprite_count;
b8 is_flipped;
} SubSpriteComponent;

typedef struct AnimationComponent
{
Entity *parent_entity;
i32 component_id;
AnimationFlags flags;
i32 current_frame;
f32 interval_mult;
f32 frame_start_time;
} AnimationComponent;

typedef struct ColliderComponent
{
Entity *parent_entity;
i32 component_id;
Shape shape;
ColliderFlags flags;
} ColliderComponent;

typedef struct VelocityComponent
{
Entity *parent_entity;
i32 component_id;
v2 velocity;
v2 ideal_velocity;
v2 acceleration;
v2 acceleration_mult;
f32 previous_friction;
b8 collide_against;
} VelocityComponent;

typedef struct PhysicsComponent
{
Entity *parent_entity;
i32 component_id;
f32 friction_mult;
f32 bounce_mult;
} PhysicsComponent;

typedef struct MovementComponent
{
Entity *parent_entity;
i32 component_id;
f32 axis_x;
f32 move_speed;
f32 move_speed_mult;
} MovementComponent;

typedef struct ArcEntityComponent
{
Entity *parent_entity;
i32 component_id;
ArcEntityType entity_type;
char *current_general_state;
AnimationStateType current_animation_state;
} ArcEntityComponent;

typedef struct ItemComponent
{
Entity *parent_entity;
i32 component_id;
ItemType item_type;
i32 stack_size;
} ItemComponent;

typedef struct TriggerComponent
{
Entity *parent_entity;
i32 component_id;
TriggerCallback enter_trigger_callback;
TriggerCallback exit_trigger_callback;
OverlappedColliderInfo previous_overlaps[MAX_OVERLAPPING_COLLIDERS];
i32 previous_overlaps_count;
b8 trigger_against;
} TriggerComponent;

typedef struct StorageComponent
{
Entity *parent_entity;
i32 component_id;
i32 storage_size;
ItemComponent *items[MAX_STORAGE_SIZE];
} StorageComponent;

typedef struct ParallaxComponent
{
Entity *parent_entity;
i32 component_id;
v2 parallax_amount;
v2 desired_position;
} ParallaxComponent;

typedef struct ParticleEmitterComponent
{
Entity *parent_entity;
i32 component_id;
f32 life_time;
f32 start_time;
ParticleEmitterFlags flags;
Particle particles[MAX_PARTICLE_AMOUNT];
i32 particle_count;
i32 free_particle_index;
EmitterBeginCallback begin_callback;
EmitterFinishCallback finish_callback;
} ParticleEmitterComponent;

typedef enum ComponentType
{
COMPONENT_INVALID,
COMPONENT_position,
COMPONENT_sprite,
COMPONENT_sub_sprite,
COMPONENT_animation,
COMPONENT_collider,
COMPONENT_velocity,
COMPONENT_physics,
COMPONENT_movement,
COMPONENT_arc_entity,
COMPONENT_item,
COMPONENT_trigger,
COMPONENT_storage,
COMPONENT_parallax,
COMPONENT_particle_emitter,
COMPONENT_MAX,
} ComponentType;

typedef struct ComponentSet
{
PositionComponent position_components[MAX_ACTIVE_ENTITIES];
i32 position_component_count;
i32 position_free_component_id;
SpriteComponent sprite_components[MAX_ACTIVE_ENTITIES];
i32 sprite_component_count;
i32 sprite_free_component_id;
SubSpriteComponent sub_sprite_components[MAX_ACTIVE_ENTITIES];
i32 sub_sprite_component_count;
i32 sub_sprite_free_component_id;
AnimationComponent animation_components[MAX_ACTIVE_ENTITIES];
i32 animation_component_count;
i32 animation_free_component_id;
ColliderComponent collider_components[MAX_ACTIVE_ENTITIES];
i32 collider_component_count;
i32 collider_free_component_id;
VelocityComponent velocity_components[MAX_ACTIVE_ENTITIES];
i32 velocity_component_count;
i32 velocity_free_component_id;
PhysicsComponent physics_components[MAX_ACTIVE_ENTITIES];
i32 physics_component_count;
i32 physics_free_component_id;
MovementComponent movement_components[MAX_ACTIVE_ENTITIES];
i32 movement_component_count;
i32 movement_free_component_id;
ArcEntityComponent arc_entity_components[MAX_ACTIVE_ENTITIES];
i32 arc_entity_component_count;
i32 arc_entity_free_component_id;
ItemComponent item_components[MAX_ACTIVE_ENTITIES];
i32 item_component_count;
i32 item_free_component_id;
TriggerComponent trigger_components[MAX_ACTIVE_ENTITIES];
i32 trigger_component_count;
i32 trigger_free_component_id;
StorageComponent storage_components[MAX_ACTIVE_ENTITIES];
i32 storage_component_count;
i32 storage_free_component_id;
ParallaxComponent parallax_components[MAX_ACTIVE_ENTITIES];
i32 parallax_component_count;
i32 parallax_free_component_id;
ParticleEmitterComponent particle_emitter_components[MAX_ACTIVE_ENTITIES];
i32 particle_emitter_component_count;
i32 particle_emitter_free_component_id;
} ComponentSet;

// @GenerateComponentList 
typedef struct Entity Entity;
struct Entity
{
i32 entity_id;
char name[20];
EntityType type;
EntityFlags flags;
void *components[COMPONENT_MAX];
ChunkData *active_chunk;
};

typedef struct ChunkData ChunkData;
struct ChunkData
{
b8 is_valid;
i32 entity_ids[MAX_ENTITIES_PER_CHUNK];
i32 entity_count;
i32 x_index;
i32 y_index;
};

typedef struct WorldData WorldData;
struct WorldData
{
f32 elapsed_world_time;
ChunkData active_chunks[MAX_WORLD_CHUNKS];
i32 active_chunk_count;
ChunkData floating_chunk;
Entity entities[MAX_ACTIVE_ENTITIES];
i32 entity_count;
i32 free_entity_index;
ComponentSet entity_components;
};

