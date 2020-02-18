#define MAX_ENTITIES_PER_CHUNK (256)
#define MAX_ACTIVE_ENTITIES (1024)
#define MAX_OVERLAPPING_COLLIDERS (50)
#define MAX_ENTITY_REFERENCES (10)
#define MAX_SUB_COLLIDERS (3)
#define MAX_STORAGE_SIZE (30)
#define MAX_PARTICLE_AMOUNT (300)
#define MAX_WORLD_CHUNKS (128)
#define CHUNK_SIZE (256)
#define CELL_CHUNK_SIZE (128)
#define MAX_PIXEL_CLUSTER_LENGTH (64)
#define MAX_DYNAMIC_CELLS (256)
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

typedef union c2Shape c2Shape;
union c2Shape
{
c2AABB aabb;
c2Capsule capsule;
c2Circle circle;
c2Poly poly;
};

typedef enum c2ShapeType c2ShapeType;
enum c2ShapeType
{
C2_SHAPE_TYPE_aabb,
C2_SHAPE_TYPE_capsule,
C2_SHAPE_TYPE_circle,
C2_SHAPE_TYPE_poly,
C2_SHAPE_TYPE_MAX,
};
static char *Getc2ShapeTypeTypeName(c2ShapeType type);

typedef struct PhysicsMaterial
{
f32 density;
f32 restitution;
} PhysicsMaterial;

typedef struct MassData
{
f32 mass;
f32 inv_mass;
} MassData;

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

typedef struct PhysicsBodyComponent
{
Entity *parent_entity;
i32 component_id;
c2Shape shape;
c2ShapeType shape_type;
PhysicsMaterial material;
MassData mass_data;
v2 velocity;
v2 force;
f32 gravity_multiplier;
} PhysicsBodyComponent;

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
COMPONENT_physics_body,
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
PhysicsBodyComponent physics_body_components[MAX_ACTIVE_ENTITIES];
i32 physics_body_component_count;
i32 physics_body_free_component_id;
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
    PhysicsBodyComponent *physics_body_comp;
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
    PhysicsBodyComponent *physics_body_comp;
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

typedef struct Cell Cell;

typedef struct CellMaterial
{
i32 id;
Cell *parent_cell;
CellMaterialType material_type;
CellFlags flags;
f32 mass;
f32 restitution;
i32 max_height;
v2 position;
v2 velocity;
b8 is_material_dynamic;
b8 has_been_updated;
} CellMaterial;

typedef struct CellChunk CellChunk;

typedef struct Cell
{
CellChunk *parent_cell_chunk;
i32 x_index;
i32 y_index;
CellMaterial *material;
} Cell;

typedef struct CellChunk
{
ChunkData *parent_chunk;
i32 x_index;
i32 y_index;
Cell cells[CELL_CHUNK_SIZE][CELL_CHUNK_SIZE];
Ts2dTexture texture;
} CellChunk;

typedef struct ChunkData
{
b8 is_valid;
i32 entity_ids[MAX_ENTITIES_PER_CHUNK];
i32 entity_count;
i32 x_index;
i32 y_index;
CellChunk cell_chunks[(CHUNK_SIZE/CELL_CHUNK_SIZE)][(CHUNK_SIZE/CELL_CHUNK_SIZE)];
CellMaterial cell_materials[(CHUNK_SIZE*CHUNK_SIZE)];
i32 cell_material_count;
i32 free_cell_material_id;
CellMaterial *dynamic_cell_materials[MAX_DYNAMIC_CELLS];
i32 dynamic_cell_material_count;
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

