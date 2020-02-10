#define MAX_ENTITIES_PER_CHUNK (256)
#define MAX_ACTIVE_ENTITIES (1024)
#define MAX_OVERLAPPING_COLLIDERS (50)
#define MAX_ENTITY_REFERENCES (10)
#define MAX_SUB_COLLIDERS (3)
#define MAX_STORAGE_SIZE (30)
#define MAX_PARTICLE_AMOUNT (300)
#define MAX_WORLD_CHUNKS (128)
#define CHUNK_SIZE (256)
#define MAX_PIXEL_CLUSTER_LENGTH (64)
typedef enum GeneralisedEntityType GeneralisedEntityType;
enum GeneralisedEntityType
{
GENERALISED_ENTITY_TYPE_undefined,
GENERALISED_ENTITY_TYPE_character,
GENERALISED_ENTITY_TYPE_monster,
GENERALISED_ENTITY_TYPE_animal,
GENERALISED_ENTITY_TYPE_item,
GENERALISED_ENTITY_TYPE_storage,
GENERALISED_ENTITY_TYPE_resource,
GENERALISED_ENTITY_TYPE_scenic,
GENERALISED_ENTITY_TYPE_ground,
GENERALISED_ENTITY_TYPE_pixel_object,
GENERALISED_ENTITY_TYPE_MAX,
};
static char *GetGeneralisedEntityTypeTypeName(GeneralisedEntityType type);

typedef enum CellMaterialType CellMaterialType;
enum CellMaterialType
{
CELL_MATERIAL_TYPE_air,
CELL_MATERIAL_TYPE_dirt,
CELL_MATERIAL_TYPE_sand,
CELL_MATERIAL_TYPE_water,
CELL_MATERIAL_TYPE_MAX,
};
static char *GetCellMaterialTypeTypeName(CellMaterialType type);

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

#define PIXEL_FLAGS_apply_gravity (1<<0)
typedef unsigned int PixelFlags;

#define CELL_FLAGS_no_gravity (1<<0)
typedef unsigned int CellFlags;

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

#define MAX_CHARACTER_ENTITIES (1)
typedef struct CharacterEntity
{
Entity *parent_generic_entity;
    PositionComponent *position_comp;
    SubSpriteComponent *sub_sprite_comp;
    AnimationComponent *animation_comp;
    ColliderComponent *collider_comp;
    PhysicsComponent *physics_comp;
    VelocityComponent *velocity_comp;
    MovementComponent *movement_comp;
    ArcEntityComponent *arc_entity_comp;
} CharacterEntity;

#define MAX_CLOUD_ENTITIES (50)
typedef struct CloudEntity
{
Entity *parent_generic_entity;
i32 unique_entity_id;
    PositionComponent *position_comp;
    SpriteComponent *sprite_comp;
    ParallaxComponent *parallax_comp;
} CloudEntity;

#define MAX_GROUND_ENTITIES (1024)
typedef struct GroundEntity
{
Entity *parent_generic_entity;
i32 unique_entity_id;
    PositionComponent *position_comp;
    ColliderComponent *collider_comp;
    PhysicsComponent *physics_comp;
} GroundEntity;

typedef enum EntityType
{
    ENTITY_TYPE_generic,
    ENTITY_TYPE_character,
    ENTITY_TYPE_cloud,
    ENTITY_TYPE_ground,
    ENTITY_TYPE_MAX
} EntityType;

typedef struct Entity
{
i32 entity_id;
char name[20];
GeneralisedEntityType generalised_type;
void *unique_entity;
EntityType type;
EntityFlags flags;
void *components[COMPONENT_MAX];
ChunkData *active_chunk;
} Entity;

typedef struct Cell
{
CellMaterialType material;
CellFlags flags;
f32 mass;
} Cell;

typedef struct ChunkData
{
b8 is_valid;
i32 entity_ids[MAX_ENTITIES_PER_CHUNK];
i32 entity_count;
i32 x_index;
i32 y_index;
Cell cells[CHUNK_SIZE][CHUNK_SIZE];
Ts2dTexture texture;
} ChunkData;

typedef struct WorldData
{
f32 elapsed_world_time;
ChunkData active_chunks[MAX_WORLD_CHUNKS];
i32 active_chunk_count;
ChunkData floating_chunk;

CharacterEntity character_entity;
CloudEntity cloud_entities[MAX_CLOUD_ENTITIES];
i32 cloud_entity_count;
i32 free_cloud_entity_index;
GroundEntity ground_entities[MAX_GROUND_ENTITIES];
i32 ground_entity_count;
i32 free_ground_entity_index;

// @GenerateUniqueEntityArrays 
Entity entities[MAX_ACTIVE_ENTITIES];
i32 entity_count;
i32 free_entity_index;
ComponentSet entity_components;
} WorldData;

