#define MAX_POSITIONAL_ENTITIES (1024)
#define MAX_FLOATING_ENTITIES (128)
#define MAX_ENTITIES ((MAX_POSITIONAL_ENTITIES+MAX_FLOATING_ENTITIES))
#define MAX_OVERLAPPING_COLLIDERS (50)
#define MAX_COLLISION_PAIRS (2048)
#define MAX_STORAGE_SIZE (30)
#define MAX_PARTICLE_AMOUNT (300)
#define MAX_WORLD_CHUNKS (128)
#define CHUNK_SIZE (256)
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
static char *GetGeneralisedEntityTypeName(GeneralisedEntityType type);

typedef enum EditorState EditorState;
enum EditorState
{
EDITOR_STATE_none,
EDITOR_STATE_entity,
EDITOR_STATE_terrain,
EDITOR_STATE_collision,
EDITOR_STATE_chunk,
EDITOR_STATE_MAX,
};
static char *GetEditorStateName(EditorState type);

#define ENTITY_FLAGS_no_delete (1<<0)
typedef uint32 EntityFlags;

#define COLLIDER_FLAGS_ground (1<<0)
#define COLLIDER_FLAGS_player (1<<1)
#define COLLIDER_FLAGS_entity (1<<2)
#define COLLIDER_FLAGS_item (1<<3)
#define COLLIDER_FLAGS_trigger (1<<4)
typedef uint32 ColliderFlags;

#define ANIMATION_FLAGS_playing (1<<0)
#define ANIMATION_FLAGS_reversing (1<<1)
#define ANIMATION_FLAGS_repeat (1<<2)
typedef uint32 AnimationFlags;

#define PARTICLE_EMITTER_FLAGS_repeat (1<<0)
typedef uint32 ParticleEmitterFlags;

#define PIXEL_FLAGS_apply_gravity (1<<0)
typedef uint32 PixelFlags;

#define DEBUG_FLAGS_draw_world (1<<0)
#define DEBUG_FLAGS_draw_collision (1<<1)
#define DEBUG_FLAGS_draw_chunk_grid (1<<2)
#define DEBUG_FLAGS_debug_cell_view (1<<3)
#define DEBUG_FLAGS_manual_step (1<<4)
typedef uint32 DebugFlags;

#define ITEM_FLAGS_resource (1<<0)
#define ITEM_FLAGS_sword (1<<1)
#define ITEM_FLAGS_bouncy (1<<2)
typedef uint32 ItemFlags;

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

static char *GetStaticSpriteName(StaticSprite type);

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

static char *GetDynamicSpriteName(DynamicSprite type);

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
global ArcEntityAnimationStateData arc_entity_animation_state_data[ARC_ENTITY_ANIMATION_STATE_MAX] = {
    { DYNAMIC_SPRITE_player_idle, },
    { DYNAMIC_SPRITE_player_walking, },
    { DYNAMIC_SPRITE_player_sprinting, },
};

static char *GetArcEntityAnimationStateName(ArcEntityAnimationState type);

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
global ArcEntityTypeData arc_entity_type_data[ARC_ENTITY_TYPE_MAX] = {
    { {"Idle", "Walking", "Sprinting"}, {ARC_ENTITY_ANIMATION_STATE_player_idle, ARC_ENTITY_ANIMATION_STATE_player_walking, ARC_ENTITY_ANIMATION_STATE_player_sprinting}, },
};

static char *GetArcEntityTypeName(ArcEntityType type);

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
global ItemTypeData item_type_data[ITEM_TYPE_MAX] = {
    { "Flint Sword", STATIC_SPRITE_flint_sword_icon, STATIC_SPRITE_flint_sword_ground, 1, ITEM_FLAGS_sword, },
};

static char *GetItemTypeName(ItemType type);

typedef struct Entity Entity;

typedef struct PositionComponent
{
i32 parent_entity_id;
i32 component_id;
v2 position;
} PositionComponent;

typedef struct SpriteComponent
{
i32 parent_entity_id;
i32 component_id;
SpriteData sprite_data;
// @Editable 
b8 is_flipped;
b8 is_background_sprite;
} SpriteComponent;

typedef struct AnimationComponent
{
i32 parent_entity_id;
i32 component_id;
AnimationFlags flags;
i32 current_frame;
f32 interval_mult;
f32 frame_start_time;
} AnimationComponent;

typedef struct Line
{
v2 p1;
v2 p2;
} Line;

typedef union c2Shape
{
c2AABB aabb;
c2Capsule capsule;
c2Circle circle;
c2Poly poly;
Line line;
} c2Shape;

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
static char *Getc2ShapeTypeName(c2ShapeType type);

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

typedef struct PhysicsBodyComponent
{
i32 parent_entity_id;
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
i32 parent_entity_id;
i32 component_id;
f32 axis_x;
f32 move_speed;
f32 move_speed_mult;
} MovementComponent;

typedef struct ArcEntityComponent
{
i32 parent_entity_id;
i32 component_id;
ArcEntityType entity_type;
char *current_general_state;
ArcEntityAnimationState current_animation_state;
} ArcEntityComponent;

typedef struct ItemComponent
{
i32 parent_entity_id;
i32 component_id;
ItemType item_type;
i32 stack_size;
} ItemComponent;

typedef struct TriggerComponent
{
i32 parent_entity_id;
i32 component_id;
TriggerCallback enter_trigger_callback;
TriggerCallback exit_trigger_callback;
OverlappedColliderInfo previous_overlaps[MAX_OVERLAPPING_COLLIDERS];
i32 previous_overlaps_count;
b8 trigger_against;
} TriggerComponent;

typedef struct StorageComponent
{
i32 parent_entity_id;
i32 component_id;
i32 storage_size;
ItemComponent *items[MAX_STORAGE_SIZE];
} StorageComponent;

typedef struct ParallaxComponent
{
i32 parent_entity_id;
i32 component_id;
v2 parallax_amount;
v2 desired_position;
} ParallaxComponent;

typedef struct ParticleEmitterComponent
{
i32 parent_entity_id;
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

typedef struct Chunk Chunk;

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
PositionComponent position_components[MAX_ENTITIES];
i32 position_component_count;
i32 free_position_component_id;
SpriteComponent sprite_components[MAX_ENTITIES];
i32 sprite_component_count;
i32 free_sprite_component_id;
AnimationComponent animation_components[MAX_ENTITIES];
i32 animation_component_count;
i32 free_animation_component_id;
PhysicsBodyComponent physics_body_components[MAX_ENTITIES];
i32 physics_body_component_count;
i32 free_physics_body_component_id;
MovementComponent movement_components[MAX_ENTITIES];
i32 movement_component_count;
i32 free_movement_component_id;
ArcEntityComponent arc_entity_components[MAX_ENTITIES];
i32 arc_entity_component_count;
i32 free_arc_entity_component_id;
ItemComponent item_components[MAX_ENTITIES];
i32 item_component_count;
i32 free_item_component_id;
TriggerComponent trigger_components[MAX_ENTITIES];
i32 trigger_component_count;
i32 free_trigger_component_id;
StorageComponent storage_components[MAX_ENTITIES];
i32 storage_component_count;
i32 free_storage_component_id;
ParallaxComponent parallax_components[MAX_ENTITIES];
i32 parallax_component_count;
i32 free_parallax_component_id;
ParticleEmitterComponent particle_emitter_components[MAX_ENTITIES];
i32 particle_emitter_component_count;
i32 free_particle_emitter_component_id;
} ComponentSet;

// NOTE(randy): Gets a PositionComponent from a specified entity, it must have one.
internal PositionComponent *GetPositionComponentFromEntityID(i32 id);
// NOTE(randy): Gets a SpriteComponent from a specified entity, it must have one.
internal SpriteComponent *GetSpriteComponentFromEntityID(i32 id);
// NOTE(randy): Gets a AnimationComponent from a specified entity, it must have one.
internal AnimationComponent *GetAnimationComponentFromEntityID(i32 id);
// NOTE(randy): Gets a PhysicsBodyComponent from a specified entity, it must have one.
internal PhysicsBodyComponent *GetPhysicsBodyComponentFromEntityID(i32 id);
// NOTE(randy): Gets a MovementComponent from a specified entity, it must have one.
internal MovementComponent *GetMovementComponentFromEntityID(i32 id);
// NOTE(randy): Gets a ArcEntityComponent from a specified entity, it must have one.
internal ArcEntityComponent *GetArcEntityComponentFromEntityID(i32 id);
// NOTE(randy): Gets a ItemComponent from a specified entity, it must have one.
internal ItemComponent *GetItemComponentFromEntityID(i32 id);
// NOTE(randy): Gets a TriggerComponent from a specified entity, it must have one.
internal TriggerComponent *GetTriggerComponentFromEntityID(i32 id);
// NOTE(randy): Gets a StorageComponent from a specified entity, it must have one.
internal StorageComponent *GetStorageComponentFromEntityID(i32 id);
// NOTE(randy): Gets a ParallaxComponent from a specified entity, it must have one.
internal ParallaxComponent *GetParallaxComponentFromEntityID(i32 id);
// NOTE(randy): Gets a ParticleEmitterComponent from a specified entity, it must have one.
internal ParticleEmitterComponent *GetParticleEmitterComponentFromEntityID(i32 id);
internal void RemoveComponent(Entity *entity, ComponentType type);
#define MINIMUM_AIR_PRESSURE (1.0f)
#define LIQUID_RESOLUTION (0.2f)
#define MAX_LIQUID_MASS (1.0f)
typedef enum CellPropertiesType CellPropertiesType;
enum CellPropertiesType
{
CELL_PROPERTIES_TYPE_air,
CELL_PROPERTIES_TYPE_liquid,
CELL_PROPERTIES_TYPE_solid,
CELL_PROPERTIES_TYPE_MAX,
};
static char *GetCellPropertiesTypeName(CellPropertiesType type);

typedef struct DynamicAirProperties
{
f32 pressure;
} DynamicAirProperties;

typedef struct DynamicLiquidProperties
{
f32 mass;
} DynamicLiquidProperties;

typedef struct DynamicSolidProperties
{
f32 hardness;
} DynamicSolidProperties;

typedef union DynamicCellProperties
{
DynamicAirProperties air;
DynamicLiquidProperties liquid;
DynamicSolidProperties solid;
} DynamicCellProperties;

typedef struct StaticAirProperties
{
f32 resting_temp;
b8 test;
} StaticAirProperties;

typedef struct StaticLiquidProperties
{
f32 default_mass;
} StaticLiquidProperties;

typedef struct StaticSolidProperties
{
f32 default_mass;
f32 restitution;
i32 max_height;
i32 crust_depth;
} StaticSolidProperties;

typedef union StaticCellProperties
{
StaticAirProperties air;
StaticLiquidProperties liquid;
StaticSolidProperties solid;
} StaticCellProperties;

typedef struct CellMaterialTypeData
{
StaticCellProperties static_properties;
CellPropertiesType properties_type;
} CellMaterialTypeData;

typedef enum CellMaterialType CellMaterialType;
enum CellMaterialType
{
CELL_MATERIAL_TYPE_air,
CELL_MATERIAL_TYPE_water,
CELL_MATERIAL_TYPE_dirt,
CELL_MATERIAL_TYPE_sand,
CELL_MATERIAL_TYPE_MAX,
};
global CellMaterialTypeData cell_material_type_data[CELL_MATERIAL_TYPE_MAX] = {
    { .static_properties.air = { .resting_temp = 2.0f, .test = 1}, CELL_PROPERTIES_TYPE_air, },
    { .static_properties.liquid = { .default_mass = 1.5f} , CELL_PROPERTIES_TYPE_liquid, },
    { .static_properties.solid = { .default_mass = 1.0f, .max_height = 3, .crust_depth = 3 }, CELL_PROPERTIES_TYPE_solid, },
    { .static_properties.solid = { .default_mass = 0.5f, .max_height = 1, .crust_depth = 6 }, CELL_PROPERTIES_TYPE_solid, },
};

static char *GetCellMaterialTypeName(CellMaterialType type);

typedef struct Cell
{
CellMaterialType material_type;
DynamicCellProperties dynamic_properties;
} Cell;

typedef struct CellHelper
{
Cell *cell;
i32 dynamic_id;
i32 x_position;
i32 y_position;
Chunk *parent_chunk;
} CellHelper;

typedef struct Entity
{
i32 entity_id;
i32 component_ids[COMPONENT_MAX];
char name[20];
EntityFlags flags;
GeneralisedEntityType generalised_type;
} Entity;

#define CELL_CHUNKS_IN_CHUNK ((CHUNK_SIZE/CELL_CHUNK_SIZE))
#define CHUNK_AREA ((CHUNK_SIZE*CHUNK_SIZE))
typedef struct Chunk
{
b8 is_valid;
b8 remain_loaded;
i32 *entity_ids;
i32 entity_count;
i32 x_index;
i32 y_index;
Cell cells[CHUNK_SIZE][CHUNK_SIZE];
Ts2dTexture texture;
} Chunk;

typedef struct ChunkSave
{
SkeletonChunk skele_chunk;
Cell cells[CHUNK_SIZE][CHUNK_SIZE];
i32 entity_count;
i32 *entity_ids;
} ChunkSave;

typedef struct WorldSaveData
{
f32 elapsed_world_time;
i32 *test_ptr;
} WorldSaveData;

typedef struct EntityEditorData
{
Entity *selected_entity;
} EntityEditorData;

typedef struct TerrainEditorData
{
Cell *selected_cell;
v2 selection_start;
v2 selection_end;
} TerrainEditorData;

typedef struct CollisionEditorData
{
Entity *selected_ground_seg;
b8 is_seg_grabbed;
v2 grabbed_seg_pos;
} CollisionEditorData;

typedef struct ChunkEditorData
{
b8 is_chunk_selected;
SkeletonChunk selected_chunk;
} ChunkEditorData;

typedef struct RunData
{
Chunk active_chunks[MAX_WORLD_CHUNKS];
i32 active_chunk_count;
i32 save_job_index;
Entity entities_snapshot[MAX_ENTITIES];
i32 entity_count_snapshot;
ComponentSet entity_components_snapshot;
i32 positional_entity_ids_snapshot[MAX_POSITIONAL_ENTITIES];
i32 positional_entity_id_count_snapshot;
ChunkSave chunk_saves[MAX_WORLD_CHUNKS];
i32 chunk_save_count;
i32 load_job_index;
b8 not_first_time_temp;
EntitySave loaded_entities[MAX_ENTITIES];
i32 loaded_entity_count;
ComponentSet loaded_entity_components;
i32 loaded_positional_entity_ids[MAX_POSITIONAL_ENTITIES];
i32 loaded_positional_entity_id_count;
ChunkSave chunk_load_queue[MAX_WORLD_CHUNKS];
i32 chunk_load_queue_count;
ChunkSave *chunk_generate_queue[MAX_WORLD_CHUNKS];
i32 chunk_generate_queue_count;
Entity entities[MAX_ENTITIES];
i32 entity_count;
i32 free_entity_id;
i32 positional_entity_ids[MAX_POSITIONAL_ENTITIES];
i32 positional_entity_id_count;
i32 floating_entity_ids[MAX_FLOATING_ENTITIES];
i32 floating_entity_id_count;
ComponentSet entity_components;
char world_name[50];
char world_path[300];
char world_chunks_path[300];
WorldSaveData world;
Chunk *chunk_texture_update_queue[MAX_WORLD_CHUNKS];
i32 chunk_texture_update_queue_count;
b8 disable_chunk_loaded_based_off_view;
CellHelper dynamic_cells[MAX_DYNAMIC_CELLS];
i32 dynamic_cell_count;
i32 free_dynamic_cell_id;
CellHelper queued_dynamic_cells[MAX_DYNAMIC_CELLS];
i32 queued_dynamic_cell_count;
Entity *character_entity;
EditorState editor_state;
DebugFlags saved_debug_flags;
DebugFlags debug_flags;
EntityEditorData entity_editor;
TerrainEditorData terrain_editor;
CollisionEditorData collision_editor;
ChunkEditorData chunk_editor;
} RunData;

typedef struct ClientData
{
b32 bloom;
} ClientData;

WriteComponentToFile(FILE *file, i32 comp_id, ComponentType type);
ReadComponentFromFile(FILE *file, Entity *entity, ComponentType type);
SerialiseEntityComponentsFromIDList(FILE *file, Entity *entity_list, ComponentSet *component_set, i32 *ids, i32 id_count, ComponentType type);

SerialiseComponentsFromDataSet(FILE *file, Entity *entity_list, i32 entity_count, ComponentSet *component_set, i32 *ids, i32 id_count);

DeserialiseEntityComponentsFromIDList(FILE *file, i32 *ids, i32 id_count, ComponentType type);

DeserialiseComponentsToLoadData(FILE *file, ComponentSet *component_set, EntitySave *entity_list, i32 *ids, i32 id_count);

DeserialiseComponentsFromMap(i32 *entity_id_map, i32 entity_count);

internal void ResetComponentSet(ComponentSet *comp_set);
static void WritePositionComponentToFile(FILE *file, PositionComponent *data);

static void FillPositionComponentPointersInFile(FILE *file, PositionComponent *data);

static void ReadPositionComponentFromFile(FILE *file, PositionComponent *data);

static void FillPositionComponentPointersFromFile(FILE *file, PositionComponent *data);

static void WriteSpriteComponentToFile(FILE *file, SpriteComponent *data);

static void FillSpriteComponentPointersInFile(FILE *file, SpriteComponent *data);

static void ReadSpriteComponentFromFile(FILE *file, SpriteComponent *data);

static void FillSpriteComponentPointersFromFile(FILE *file, SpriteComponent *data);

static void WriteAnimationComponentToFile(FILE *file, AnimationComponent *data);

static void FillAnimationComponentPointersInFile(FILE *file, AnimationComponent *data);

static void ReadAnimationComponentFromFile(FILE *file, AnimationComponent *data);

static void FillAnimationComponentPointersFromFile(FILE *file, AnimationComponent *data);

static void WritePhysicsBodyComponentToFile(FILE *file, PhysicsBodyComponent *data);

static void FillPhysicsBodyComponentPointersInFile(FILE *file, PhysicsBodyComponent *data);

static void ReadPhysicsBodyComponentFromFile(FILE *file, PhysicsBodyComponent *data);

static void FillPhysicsBodyComponentPointersFromFile(FILE *file, PhysicsBodyComponent *data);

static void WriteMovementComponentToFile(FILE *file, MovementComponent *data);

static void FillMovementComponentPointersInFile(FILE *file, MovementComponent *data);

static void ReadMovementComponentFromFile(FILE *file, MovementComponent *data);

static void FillMovementComponentPointersFromFile(FILE *file, MovementComponent *data);

static void WriteArcEntityComponentToFile(FILE *file, ArcEntityComponent *data);

static void FillArcEntityComponentPointersInFile(FILE *file, ArcEntityComponent *data);

static void ReadArcEntityComponentFromFile(FILE *file, ArcEntityComponent *data);

static void FillArcEntityComponentPointersFromFile(FILE *file, ArcEntityComponent *data);

static void WriteItemComponentToFile(FILE *file, ItemComponent *data);

static void FillItemComponentPointersInFile(FILE *file, ItemComponent *data);

static void ReadItemComponentFromFile(FILE *file, ItemComponent *data);

static void FillItemComponentPointersFromFile(FILE *file, ItemComponent *data);

static void WriteTriggerComponentToFile(FILE *file, TriggerComponent *data);

static void FillTriggerComponentPointersInFile(FILE *file, TriggerComponent *data);

static void ReadTriggerComponentFromFile(FILE *file, TriggerComponent *data);

static void FillTriggerComponentPointersFromFile(FILE *file, TriggerComponent *data);

static void WriteStorageComponentToFile(FILE *file, StorageComponent *data);

static void FillStorageComponentPointersInFile(FILE *file, StorageComponent *data);

static void ReadStorageComponentFromFile(FILE *file, StorageComponent *data);

static void FillStorageComponentPointersFromFile(FILE *file, StorageComponent *data);

static void WriteParallaxComponentToFile(FILE *file, ParallaxComponent *data);

static void FillParallaxComponentPointersInFile(FILE *file, ParallaxComponent *data);

static void ReadParallaxComponentFromFile(FILE *file, ParallaxComponent *data);

static void FillParallaxComponentPointersFromFile(FILE *file, ParallaxComponent *data);

static void WriteParticleEmitterComponentToFile(FILE *file, ParticleEmitterComponent *data);

static void FillParticleEmitterComponentPointersInFile(FILE *file, ParticleEmitterComponent *data);

static void ReadParticleEmitterComponentFromFile(FILE *file, ParticleEmitterComponent *data);

static void FillParticleEmitterComponentPointersFromFile(FILE *file, ParticleEmitterComponent *data);

static void WriteComponentSetToFile(FILE *file, ComponentSet *data);

static void FillComponentSetPointersInFile(FILE *file, ComponentSet *data);

static void ReadComponentSetFromFile(FILE *file, ComponentSet *data);

static void FillComponentSetPointersFromFile(FILE *file, ComponentSet *data);

static void WriteEntityToFile(FILE *file, Entity *data);

static void FillEntityPointersInFile(FILE *file, Entity *data);

static void ReadEntityFromFile(FILE *file, Entity *data);

static void FillEntityPointersFromFile(FILE *file, Entity *data);

static void WriteWorldSaveDataToFile(FILE *file, WorldSaveData *data);

static void FillWorldSaveDataPointersInFile(FILE *file, WorldSaveData *data);

static void ReadWorldSaveDataFromFile(FILE *file, WorldSaveData *data);

static void FillWorldSaveDataPointersFromFile(FILE *file, WorldSaveData *data);

