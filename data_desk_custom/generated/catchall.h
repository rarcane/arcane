#define MAX_POSITIONAL_ENTITIES (1024)
#define MAX_FLOATING_ENTITIES (1024)
#define MAX_ENTITIES ((MAX_POSITIONAL_ENTITIES+MAX_FLOATING_ENTITIES))
#define MAX_OVERLAPPING_COLLIDERS (50)
#define MAX_COLLISION_PAIRS (2048)
#define MAX_STORAGE_SIZE (30)
#define MAX_PARTICLE_AMOUNT (300)
#define MAX_WORLD_CHUNKS (128)
#define CHUNK_SIZE (256)
#define MAX_DYNAMIC_CELLS (16384)
#define MAX_QUEUED_TEXTURES (1024)
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
GENERALISED_ENTITY_TYPE_structure,
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
STATIC_SPRITE_hills_1_v2,
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
STATIC_SPRITE_side_arrow,
STATIC_SPRITE_crafting_stump,
STATIC_SPRITE_crafting_tool,
STATIC_SPRITE_flint_sword_icon,
STATIC_SPRITE_flint_sword_ground,
STATIC_SPRITE_flint,
STATIC_SPRITE_twig,
STATIC_SPRITE_shia,
STATIC_SPRITE_shia2,
STATIC_SPRITE_MAX,
};
global StaticSpriteData global_static_sprite_data[STATIC_SPRITE_MAX] = {
    { "invalid_texture", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, },
    { "scenic/biome_ground", {0.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "scenic/biome_ground", {100.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "scenic/biome_ground", {200.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, },
    { "entity/player/temp_player", {0.0f, 0.0f, 14.0f, 35.0f}, {0.0f, 0.0f}, },
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
    { "scenic/background/bg1_hills", {0.0f, 35.0f, 200.0f, 35.0f}, {0.0f, 20.0f}, },
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
    { "scenic/background/vegetation", {0.0f, 25.0f, 25.0f, 20.0f}, {0.0f, -13.0f}, },
    { "scenic/background/vegetation", {25.0f, 25.0f, 25.0f, 20.0f}, {0.0f, -13.0f}, },
    { "scenic/background/vegetation", {50.0f, 25.0f, 25.0f, 20.0f}, {0.0f, -13.0f}, },
    { "scenic/background/bg2_pine_tree", {0.0f, 0.0f, 40.0f, 110.0f}, {0.0f, -5.0f}, },
    { "scenic/background/bg2_pine_tree", {40.0f, 0.0f, 40.0f, 110.0f}, {0.0f, -5.0f}, },
    { "scenic/background/bg3_hills", {0.0f, 0.0f, 200.0f, 40.0f}, {0.0f, 17.0f}, },
    { "scenic/background/vegetation", {0.0f, 45.0f, 15.0f, 10.0f}, {0.0f, -20.0f}, },
    { "scenic/background/vegetation", {15.0f, 45.0f, 15.0f, 10.0f}, {0.0f, -20.0f}, },
    { "scenic/background/vegetation", {30.0f, 45.0f, 15.0f, 10.0f}, {0.0f, -20.0f}, },
    { "scenic/background/vegetation", {45.0f, 45.0f, 15.0f, 10.0f}, {0.0f, -20.0f}, },
    { "scenic/background/bg3_pine_tree", {0.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {20.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {40.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {60.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {80.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {100.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {120.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "scenic/background/bg3_pine_tree", {140.0f, 0.0f, 20.0f, 50.0f}, {0.0f, -18.0f}, },
    { "icon/axis_icons", {0.0f, 0.0f, 7.0f, 40.0f}, {0.0f, 0.0f}, },
    { "icon/axis_icons", {0.0f, 33.0f, 40.0f, 7.0f}, {0.0f, 0.0f}, },
    { "icon/axis_icons", {33.0f, 0.0f, 7.0f, 7.0f}, {0.0f, 0.0f}, },
    { "icon/side_arrow", {0.0f, 0.0f, 5.0f, 9.0f}, {0.0f, 0.0f}, },
    { "structures/crafting_stump", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, },
    { "structures/crafting_tool", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/flint_sword", {0.0f, 0.0f, 16.0f, 16.0f}, {6.0f, 2.0f}, },
    { "item/flint_sword_ground", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, },
    { "item/flint", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/twig", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/shia", {0.0f, 0.0f, 800.0f, 1200.0f}, {0.0f, 0.0f}, },
    { "item/shia2", {0.0f, 0.0f, 590.0f, 631.0f}, {0.0f, 0.0f}, },
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
global DynamicSpriteData global_dynamic_sprite_data[DYNAMIC_SPRITE_MAX] = {
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
global ArcEntityAnimationStateData global_arc_entity_animation_state_data[ARC_ENTITY_ANIMATION_STATE_MAX] = {
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
global ArcEntityTypeData global_arc_entity_type_data[ARC_ENTITY_TYPE_MAX] = {
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
ITEM_TYPE_none,
ITEM_TYPE_flint_sword,
ITEM_TYPE_flint,
ITEM_TYPE_twig,
ITEM_TYPE_crafting_tool,
ITEM_TYPE_MAX,
};
global ItemTypeData global_item_type_data[ITEM_TYPE_MAX] = {
    { "none", STATIC_SPRITE_INVALID, STATIC_SPRITE_INVALID, 0, 0, },
    { "Flint Sword", STATIC_SPRITE_flint_sword_icon, STATIC_SPRITE_flint_sword_ground, 1, ITEM_FLAGS_sword, },
    { "Flint", STATIC_SPRITE_flint, STATIC_SPRITE_flint, 8, 0, },
    { "Twig", STATIC_SPRITE_twig, STATIC_SPRITE_twig, 8, 0, },
    { "Crafting Tool", STATIC_SPRITE_crafting_tool, STATIC_SPRITE_crafting_tool, 1, 0, },
};

static char *GetItemTypeName(ItemType type);

typedef struct Entity Entity;

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

#define PHYSICS_BODY_TYPE_FLAGS_ground (1<<0)
#define PHYSICS_BODY_TYPE_FLAGS_item (1<<1)
#define PHYSICS_BODY_TYPE_FLAGS_character (1<<2)
#define PHYSICS_BODY_TYPE_FLAGS_station (1<<3)
typedef uint32 PhysicsBodyTypeFlags;

typedef struct Item
{
ItemType type;
i32 stack_size;
} Item;

#define MAX_ITEMS_IN_CRAFTING_RECIPE (10)
typedef struct CraftingRecipeTypeData
{
Item output;
Item input[MAX_ITEMS_IN_CRAFTING_RECIPE];
} CraftingRecipeTypeData;

typedef enum CraftingRecipeType CraftingRecipeType;
enum CraftingRecipeType
{
CRAFTING_RECIPE_TYPE_none,
CRAFTING_RECIPE_TYPE_flint_sword,
CRAFTING_RECIPE_TYPE_flint,
CRAFTING_RECIPE_TYPE_twig,
CRAFTING_RECIPE_TYPE_MAX,
};
global CraftingRecipeTypeData global_crafting_recipe_type_data[CRAFTING_RECIPE_TYPE_MAX] = {
    { {0}, {0}, },
    { {ITEM_TYPE_flint_sword, 1}, {{ITEM_TYPE_flint, 3}, {ITEM_TYPE_twig, 2}}, },
    { {ITEM_TYPE_flint, 2}, {{ITEM_TYPE_flint, 1}}, },
    { {ITEM_TYPE_twig, 2}, {{ITEM_TYPE_twig, 1}}, },
};

static char *GetCraftingRecipeTypeName(CraftingRecipeType type);

typedef struct CraftingStation
{
CraftingRecipeType current_recipe;
} CraftingStation;

typedef struct SmeltingStation
{
i32 temp;
} SmeltingStation;

typedef union StationData
{
CraftingStation crafting;
SmeltingStation smelting;
} StationData;

typedef enum StationType StationType;
enum StationType
{
STATION_TYPE_crafting,
STATION_TYPE_smelting,
STATION_TYPE_MAX,
};
static char *GetStationTypeName(StationType type);

typedef enum StructureCategory StructureCategory;
enum StructureCategory
{
STRUCTURE_CATEGORY_none,
STRUCTURE_CATEGORY_shia,
STRUCTURE_CATEGORY_crafting,
STRUCTURE_CATEGORY_base,
STRUCTURE_CATEGORY_MAX,
};
static char *GetStructureCategoryName(StructureCategory type);

#define MAX_ITEMS_IN_BLUEPRINT_RECIPE (10)
typedef struct StructureTypeData
{
char print_name[20];
StructureCategory category;
StaticSprite icon_sprite;
StaticSprite world_sprite;
Item recipe[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
} StructureTypeData;

typedef enum StructureType StructureType;
enum StructureType
{
STRUCTURE_TYPE_none,
STRUCTURE_TYPE_shia,
STRUCTURE_TYPE_crafting_stump,
STRUCTURE_TYPE_shia2,
STRUCTURE_TYPE_base,
STRUCTURE_TYPE_MAX,
};
global StructureTypeData global_structure_type_data[STRUCTURE_TYPE_MAX] = {
    { "none", STRUCTURE_CATEGORY_none, STATIC_SPRITE_INVALID, STATIC_SPRITE_INVALID, {0}, },
    { "Shia", STRUCTURE_CATEGORY_shia, STATIC_SPRITE_shia, STATIC_SPRITE_shia, {0}, },
    { "Crafting Stump", STRUCTURE_CATEGORY_crafting, STATIC_SPRITE_crafting_stump, STATIC_SPRITE_crafting_stump, {0}, },
    { "Shia 2", STRUCTURE_CATEGORY_shia, STATIC_SPRITE_shia2, STATIC_SPRITE_shia2, {0}, },
    { "Base", STRUCTURE_CATEGORY_base, STATIC_SPRITE_shia2, STATIC_SPRITE_shia2, {0}, },
};

static char *GetStructureTypeName(StructureType type);

typedef struct Chunk Chunk;

// @GenerateComponentCode 
#define Dummy2 (_)
#define MAX_HOTBAR_SLOTS (9)
#define MAX_INVENTORY_SLOTS (9)
typedef struct PhysicsBodyData
{
c2Shape shape;
c2ShapeType shape_type;
PhysicsMaterial material;
MassData mass_data;
v2 velocity;
v2 force;
f32 gravity_multiplier;
PhysicsBodyTypeFlags type;
PhysicsBodyTypeFlags collide_against;
} PhysicsBodyData;

typedef struct InteractableData
{
c2Shape bounds;
c2ShapeType bounds_type;
f32 priority;
InteractCallback interact_callback;
EnterInteractableCallback enter_interactable_callback;
ExitInteractableCallback exit_interactable_callback;
b8 is_overlapping_player;
} InteractableData;

#define ENTITY_FLAGS_no_delete (1<<0)
#define ENTITY_FLAGS_force_floating (1<<1)
#define ENTITY_FLAGS_interactable (1<<2)
#define ENTITY_FLAGS_animation (1<<3)
#define ENTITY_FLAGS_sprite (1<<4)
#define ENTITY_FLAGS_parallax (1<<5)
#define ENTITY_FLAGS_physics (1<<6)
typedef uint32 EntityFlags;

typedef struct Entity
{
EntityFlags flags;
v2 position;
SpriteData sprite_data;
b8 is_flipped;
b8 is_background_sprite;
AnimationFlags animation_flags;
i32 current_frame;
f32 interval_mult;
f32 frame_start_time;
PhysicsBodyData physics;
f32 axis_x;
f32 move_speed;
f32 move_speed_mult;
ArcEntityType entity_type;
char *current_general_state;
ArcEntityAnimationState current_animation_state;
Item item;
v2 parallax_amount;
v2 desired_position;
Item inventory[MAX_INVENTORY_SLOTS];
i32 inventory_size;
Item hotbar[MAX_HOTBAR_SLOTS];
i32 hotbar_size;
i32 active_hotbar_slot;
Item grabbed_item;
v2 grabbed_item_offset;
Item *grabbed_item_origin_slot;
InteractableData interactable;
StructureType structure_type;
Item items_contributed[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
StationData station_data;
StationType station_type;
} Entity;

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
global CellMaterialTypeData global_cell_material_type_data[CELL_MATERIAL_TYPE_MAX] = {
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

typedef struct QueuedTexture
{
Ts2dTexture *texture;
i32 flags;
v4 source;
v4 destination;
v4 tint;
f32 layer;
} QueuedTexture;

typedef struct RunData
{
Chunk active_chunks[MAX_WORLD_CHUNKS];
i32 active_chunk_count;
Entity entities[MAX_ENTITIES];
i32 entity_count;
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
b8 disable_player_input;
b8 disable_interaction;
InteractableData *current_interactable;
Entity *engaged_station_entity;
b8 is_blueprinting;
i32 queued_texture_count;
QueuedTexture queued_textures[MAX_QUEUED_TEXTURES];
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

static void WriteWorldSaveDataToFile(FILE *file, WorldSaveData *data);

static void ReadWorldSaveDataFromFile(FILE *file, WorldSaveData *data);

