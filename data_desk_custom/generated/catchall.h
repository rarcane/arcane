#define MAX_ENTITIES_PER_CHUNK (256)
#define MAX_ACTIVE_ENTITIES (1024)
#define MAX_OVERLAPPING_COLLIDERS (50)
#define MAX_COLLISION_PAIRS (2048)
#define MAX_ENTITY_REFERENCES (10)
#define MAX_SUB_COLLIDERS (3)
#define MAX_STORAGE_SIZE (30)
#define MAX_PARTICLE_AMOUNT (300)
#define MAX_WORLD_CHUNKS (128)
#define CHUNK_SIZE (256)
#define CELL_CHUNK_SIZE (128)
#define MAX_PIXEL_CLUSTER_LENGTH (64)
#define MAX_DYNAMIC_CELLS (16384)
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

typedef enum EditorState EditorState;
enum EditorState
{
EDITOR_STATE_none,
EDITOR_STATE_entity,
EDITOR_STATE_terrain,
EDITOR_STATE_collision,
EDITOR_STATE_MAX,
};
static char *GetEditorStateTypeName(EditorState type);

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

#define EDITOR_FLAGS_draw_world (1<<0)
#define EDITOR_FLAGS_draw_collision (1<<1)
typedef unsigned int EditorFlags;

#define ITEM_FLAGS_resource (1<<0)
#define ITEM_FLAGS_sword (1<<1)
#define ITEM_FLAGS_bouncy (1<<2)
typedef unsigned int ItemFlags;

#define ITEM_FLAGS_HOTBARABLE (ITEM_FLAGS_sword)
typedef struct StaticSpriteData
{
char texture_path[50];
v4 source;
v2 offset;
// @Ignore 
Ts2dTexture *texture_atlas;
} StaticSpriteData;

typedef enum StaticSprite StaticSprite;
enum StaticSprite
{
STATIC_SPRITE_INVALID,
STATIC_SPRITE_ground_arctic,
STATIC_SPRITE_ground_desert,
STATIC_SPRITE_ground_plains,
STATIC_SPRITE_player,
STATIC_SPRITE_flint_sword_icon,
STATIC_SPRITE_flint_sword_ground,
STATIC_SPRITE_far_mountains,
STATIC_SPRITE_mid_mountains,
STATIC_SPRITE_cloud_v1,
STATIC_SPRITE_cloud_v2,
STATIC_SPRITE_cloud_v3,
STATIC_SPRITE_cloud_v4,
STATIC_SPRITE_cloud_v5,
STATIC_SPRITE_cloud_v6,
STATIC_SPRITE_ground_forest_flat,
STATIC_SPRITE_ground_forest_10_degree,
STATIC_SPRITE_ground_rocks_v1,
STATIC_SPRITE_ground_rocks_v2,
STATIC_SPRITE_pine_tree_v1,
STATIC_SPRITE_hills_1_v1,
STATIC_SPRITE_bg1_shrub_v1,
STATIC_SPRITE_bg1_shrub_v2,
STATIC_SPRITE_bg1_shrub_v3,
STATIC_SPRITE_bg1_sapling_v1,
STATIC_SPRITE_bg1_sapling_v2,
STATIC_SPRITE_bg1_sapling_v3,
STATIC_SPRITE_bg1_pine_tree_v1,
STATIC_SPRITE_bg1_pine_tree_v2,
STATIC_SPRITE_bg2_hills_v1,
STATIC_SPRITE_bg2_hills_v2,
STATIC_SPRITE_bg2_shrub_v1,
STATIC_SPRITE_bg2_shrub_v2,
STATIC_SPRITE_bg2_shrub_v3,
STATIC_SPRITE_bg2_pine_tree_v1,
STATIC_SPRITE_bg2_pine_tree_v2,
STATIC_SPRITE_bg3_hills_v1,
STATIC_SPRITE_bg3_shrub_v1,
STATIC_SPRITE_bg3_shrub_v2,
STATIC_SPRITE_bg3_shrub_v3,
STATIC_SPRITE_bg3_shrub_v4,
STATIC_SPRITE_bg3_pine_tree_v1,
STATIC_SPRITE_bg3_pine_tree_v2,
STATIC_SPRITE_bg3_pine_tree_v3,
STATIC_SPRITE_bg3_pine_tree_v4,
STATIC_SPRITE_bg3_pine_tree_v5,
STATIC_SPRITE_bg3_pine_tree_v6,
STATIC_SPRITE_bg3_pine_tree_v7,
STATIC_SPRITE_bg3_pine_tree_v8,
STATIC_SPRITE_y_axis_arrow_icon,
STATIC_SPRITE_x_axis_arrow_icon,
STATIC_SPRITE_circle_icon,
STATIC_SPRITE_MAX,
};
static char *GetStaticSpriteTypeName(StaticSprite type);

global StaticSpriteData static_sprite_data[STATIC_SPRITE_MAX] = {
    { "invalid_texture", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, },
    { "scenic/biome_ground", {0.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "scenic/biome_ground", {100.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "scenic/biome_ground", {200.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "entity/player/temp_player", {0.0f, 0.0f, 14.0f, 35.0f}, {0.0f, 0.0f}, },
    { "item/flint_sword", {0.0f, 0.0f, 16.0f, 16.0f}, {6.0f, 2.0f}, },
    { "item/flint_sword_ground", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, },
    { "scenic/mountains", {0.0f, 0.0f, 600.0f, 160.0f}, {0.0f, 5.0f}, },
    { "scenic/mountains", {0.0f, 160.0f, 600.0f, 90.0f}, {0.0f, 10.0f}, },
    { "scenic/clouds", {0.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/clouds", {80.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/clouds", {160.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/clouds", {0.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/clouds", {80.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/clouds", {160.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/forest_ground", {0.0f, 40.0f, 100.0f, 90.0f }, {20.0f, 87.0f}, },
    { "scenic/forest_ground", {100.0f, 25.0f, 100.0f, 105.0f}, {0.0f, 87.0f}, },
    { "scenic/forest_ground", {0.0f, 0.0f, 100.0f, 4.0f}, {0.0f, 0.0f}, },
    { "scenic/forest_ground", {0.0f, 4.0f, 100.0f, 4.0f}, {0.0f, 0.0f}, },
    { "scenic/trees/pine_tree", {0.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg1_hills", {0.0f, 0.0f, 200.0f, 35.0f}, {0.0f, 20.0f}, },
    { "scenic/background/vegetation", {0.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {50.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {100.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {75.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {95.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {115.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg1_pine_tree", {0.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg1_pine_tree", {90.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg2_hills", {0.0f, 0.0f, 200.0f, 40.0f}, {0.0f, 21.0f}, },
    { "scenic/background/bg2_hills", {0.0f, 40.0f, 200.0f, 40.0f}, {0.0f, 21.0f}, },
    { "scenic/background/vegetation", {0.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {25.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {50.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg2_pine_tree", {0.0f, 0.0f, 40.0f, 110.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg2_pine_tree", {40.0f, 0.0f, 40.0f, 110.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_hills", {0.0f, 0.0f, 200.0f, 40.0f}, {0.0f, 17.0f}, },
    { "scenic/background/vegetation", {0.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {15.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {30.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, },
    { "scenic/background/vegetation", {45.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {0.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {20.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {40.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {60.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {80.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {100.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {120.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "scenic/background/bg3_pine_tree", {140.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, },
    { "icon/axis_icons", {0.0f, 0.0f, 7.0f, 40.0f}, {0.0f, 0.0f}, },
    { "icon/axis_icons", {0.0f, 33.0f, 40.0f, 7.0f}, {0.0f, 0.0f}, },
    { "icon/axis_icons", {33.0f, 0.0f, 7.0f, 7.0f}, {0.0f, 0.0f}, },
};

typedef struct DynamicSpriteData
{
char texture_path[50];
v4 source;
v2 offset;
i32 frames;
f32 frame_interval;
// @Ignore 
Ts2dTexture *texture_atlas;
} DynamicSpriteData;

typedef enum DynamicSprite DynamicSprite;
enum DynamicSprite
{
DYNAMIC_SPRITE_INVALID,
DYNAMIC_SPRITE_player_idle,
DYNAMIC_SPRITE_player_walking,
DYNAMIC_SPRITE_player_sprinting,
DYNAMIC_SPRITE_birch_tree1,
DYNAMIC_SPRITE_birch_tree2,
DYNAMIC_SPRITE_birch_tree3,
DYNAMIC_SPRITE_birch_tree4,
DYNAMIC_SPRITE_MAX,
};
static char *GetDynamicSpriteTypeName(DynamicSprite type);

global DynamicSpriteData dynamic_sprite_data[DYNAMIC_SPRITE_MAX] = {
    { "invalid_texture", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "entity/player/player_animations", {0.0f, 192.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 4, 0.15f, },
    { "entity/player/player_animations", {0.0f, 256.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 8, 0.1f, },
    { "entity/player/player_animations", {0.0f, 320.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 8, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 0.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 160.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 320.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 480.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
};

#define MAX_SUB_SPRITES (5)
typedef struct ArcEntityAnimationStateData
{
DynamicSprite dynamic_sprite;
} ArcEntityAnimationStateData;

typedef enum ArcEntityAnimationState ArcEntityAnimationState;
enum ArcEntityAnimationState
{
ARC_ENTITY_ANIMATION_STATE_player_idle,
ARC_ENTITY_ANIMATION_STATE_player_walking,
ARC_ENTITY_ANIMATION_STATE_player_sprinting,
ARC_ENTITY_ANIMATION_STATE_MAX,
};
static char *GetArcEntityAnimationStateTypeName(ArcEntityAnimationState type);

global ArcEntityAnimationStateData arc_entity_animation_state_data[ARC_ENTITY_ANIMATION_STATE_MAX] = {
    { DYNAMIC_SPRITE_player_idle, },
    { DYNAMIC_SPRITE_player_walking, },
    { DYNAMIC_SPRITE_player_sprinting, },
};

typedef struct ArcEntityTypeData
{
char *general_states[10];
ArcEntityAnimationState animation_states[20];
} ArcEntityTypeData;

typedef enum ArcEntityType ArcEntityType;
enum ArcEntityType
{
ARC_ENTITY_TYPE_player,
ARC_ENTITY_TYPE_MAX,
};
static char *GetArcEntityTypeTypeName(ArcEntityType type);

global ArcEntityTypeData arc_entity_type_data[ARC_ENTITY_TYPE_MAX] = {
    { {"Idle", "Walking", "Sprinting"}, {ARC_ENTITY_ANIMATION_STATE_player_idle, ARC_ENTITY_ANIMATION_STATE_player_walking, ARC_ENTITY_ANIMATION_STATE_player_sprinting}, },
};

typedef struct ItemTypeData
{
char print_name[20];
StaticSprite icon_sprite;
StaticSprite ground_sprite;
i32 max_stack_size;
ItemFlags flags;
} ItemTypeData;

typedef enum ItemType ItemType;
enum ItemType
{
ITEM_TYPE_flint_sword,
ITEM_TYPE_MAX,
};
static char *GetItemTypeTypeName(ItemType type);

global ItemTypeData item_type_data[ITEM_TYPE_MAX] = {
    { "Flint Sword", STATIC_SPRITE_flint_sword_icon, STATIC_SPRITE_flint_sword_ground, 1, ITEM_FLAGS_sword, },
};

#define FLUID_COMPRESSION (0.01f)
typedef struct SolidMaterial
{
v2 position;
v2 velocity;
f32 hardness;
} SolidMaterial;

typedef struct FluidMatieral
{
f32 pressure;
f32 mass;
f32 new_mass;
} FluidMatieral;

typedef union CellProperties CellProperties;
union CellProperties
{
SolidMaterial solid;
FluidMatieral fluid;
};

typedef enum CellPropertiesType CellPropertiesType;
enum CellPropertiesType
{
CELL_PROPERTIES_TYPE_solid,
CELL_PROPERTIES_TYPE_fluid,
CELL_PROPERTIES_TYPE_MAX,
};
static char *GetCellPropertiesTypeTypeName(CellPropertiesType type);

typedef struct CellMaterialTypeData
{
f32 default_mass;
f32 restitution;
i32 max_height;
i32 crust_depth;
CellPropertiesType properties_type;
} CellMaterialTypeData;

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

global CellMaterialTypeData cell_material_type_data[CELL_MATERIAL_TYPE_MAX] = {
    { 0.0f, 0.0f, 0, 0, CELL_PROPERTIES_TYPE_fluid, },
    { 5.0f, 0.0f, 3, 3, CELL_PROPERTIES_TYPE_solid, },
    { 3.0f, 0.0f, 1, 5, CELL_PROPERTIES_TYPE_solid, },
    { 10.0f, 1.50f, 0, 0, CELL_PROPERTIES_TYPE_fluid, },
};

typedef struct Entity Entity;

typedef struct Line
{
v2 p1;
v2 p2;
} Line;

typedef union c2Shape c2Shape;
union c2Shape
{
c2AABB aabb;
c2Capsule capsule;
c2Circle circle;
c2Poly poly;
Line line;
};

typedef enum c2ShapeType c2ShapeType;
enum c2ShapeType
{
C2_SHAPE_TYPE_aabb,
C2_SHAPE_TYPE_capsule,
C2_SHAPE_TYPE_circle,
C2_SHAPE_TYPE_poly,
C2_SHAPE_TYPE_line,
C2_SHAPE_TYPE_MAX,
};
static char *Getc2ShapeTypeTypeName(c2ShapeType type);

typedef struct PhysicsMaterial
{
f32 density;
f32 restitution;
f32 static_friction;
f32 dynamic_friction;
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
ArcEntityAnimationState current_animation_state;
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
    SpriteComponent *sprite_comp;
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

#define MAX_GROUND_SEGMENT_ENTITIES (1024)
typedef struct GroundSegmentEntity
{
Entity *parent_generic_entity;
i32 unique_entity_id;
    PositionComponent *position_comp;
    PhysicsBodyComponent *physics_body_comp;
} GroundSegmentEntity;

typedef enum EntityType
{
    ENTITY_TYPE_generic,
    ENTITY_TYPE_character,
    ENTITY_TYPE_cloud,
    ENTITY_TYPE_ground_segment,
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
CellPropertiesType properties_type;
CellProperties properties;
b8 is_material_dynamic;
b8 has_been_updated;
f32 idle_start;
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
GroundSegmentEntity ground_segment_entities[MAX_GROUND_SEGMENT_ENTITIES];
i32 ground_segment_entity_count;
i32 free_ground_segment_entity_index;

// @GenerateUniqueEntityArrays 
Entity entities[MAX_ACTIVE_ENTITIES];
i32 entity_count;
i32 free_entity_id;
ComponentSet entity_components;
} WorldData;

typedef struct ClientData
{
b32 bloom;
EditorState editor_state;
EditorFlags editor_flags;
Entity *selected_entity;
GroundSegmentEntity *selected_ground_seg;
} ClientData;

