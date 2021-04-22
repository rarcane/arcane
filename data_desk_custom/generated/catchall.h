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
GENERALISED_ENTITY_TYPE_structure,
GENERALISED_ENTITY_TYPE_MAX,
};
static char *GetGeneralisedEntityTypeName(GeneralisedEntityType type);

typedef enum EditorState EditorState;
enum EditorState
{
EDITOR_STATE_none,
EDITOR_STATE_general,
EDITOR_STATE_terrain,
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

#define DEBUG_FLAGS_draw_collision (1<<0)
#define DEBUG_FLAGS_draw_chunk_grid (1<<1)
#define DEBUG_FLAGS_disable_draw_terrain (1<<2)
typedef uint32 DebugFlags;

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
STATIC_SPRITE_test_icon,
STATIC_SPRITE_crafting_stump,
STATIC_SPRITE_runic_enchanter,
STATIC_SPRITE_flint_sword_icon,
STATIC_SPRITE_flint_sword_ground,
STATIC_SPRITE_flint_axe,
STATIC_SPRITE_flint,
STATIC_SPRITE_twig,
STATIC_SPRITE_crafting_tool,
STATIC_SPRITE_test_chestpiece,
STATIC_SPRITE_shia,
STATIC_SPRITE_shia2,
STATIC_SPRITE_dummy,
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
    { "icon/test_icon", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, },
    { "structures/crafting_stump", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, },
    { "structures/runic_enchanter", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, },
    { "item/flint_sword", {0.0f, 0.0f, 16.0f, 16.0f}, {6.0f, 2.0f}, },
    { "item/flint_sword_ground", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, },
    { "item/flint_axe", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, },
    { "item/flint", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/twig", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/crafting_tool", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, },
    { "item/test_chestpiece", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, },
    { "item/shia", {0.0f, 0.0f, 800.0f, 1200.0f}, {0.0f, 0.0f}, },
    { "item/shia2", {0.0f, 0.0f, 590.0f, 631.0f}, {0.0f, 0.0f}, },
    { "entity/ron", {0.0f, 0.0f, 512.0f, 512.0f}, {0.0f, 0.0f}, },
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

typedef struct EnchantmentTypeData
{
char print_name[20];
} EnchantmentTypeData;

typedef enum EnchantmentType EnchantmentType;
enum EnchantmentType
{
ENCHANTMENT_TYPE_none,
ENCHANTMENT_TYPE_test,
ENCHANTMENT_TYPE_yeet,
ENCHANTMENT_TYPE_uhhhh,
ENCHANTMENT_TYPE_MAX,
};
global EnchantmentTypeData global_enchantment_type_data[ENCHANTMENT_TYPE_MAX] = {
    { "none", },
    { "enchantus prime", },
    { "yeeeet", },
    { "uuhhhhh", },
};

static char *GetEnchantmentTypeName(EnchantmentType type);

#define MAX_ENCHANTMENTS (8)
typedef struct Enchantment
{
EnchantmentType type;
} Enchantment;

typedef enum ItemCategory ItemCategory;
enum ItemCategory
{
ITEM_CATEGORY_none,
ITEM_CATEGORY_resource,
ITEM_CATEGORY_lumber_axe,
ITEM_CATEGORY_sword,
ITEM_CATEGORY_helmet,
ITEM_CATEGORY_chestpiece,
ITEM_CATEGORY_gauntlet,
ITEM_CATEGORY_leggings,
ITEM_CATEGORY_boots,
ITEM_CATEGORY_cloak,
ITEM_CATEGORY_MAX,
};
static char *GetItemCategoryName(ItemCategory type);

#define ITEM_FLAGS_bouncy (1<<0)
#define ITEM_FLAGS_hotbarable (1<<1)
#define ITEM_FLAGS_primary_usable (1<<2)
#define ITEM_FLAGS_secondary_usable (1<<3)
#define ITEM_FLAGS_tertiary_usable (1<<4)
#define ITEM_FLAGS_enchantable (1<<5)
typedef uint32 ItemFlags;

typedef struct ItemTypeData
{
char print_name[20];
StaticSprite icon_sprite;
StaticSprite ground_sprite;
i32 max_stack_size;
ItemCategory category;
ItemFlags flags;
} ItemTypeData;

typedef enum ItemType ItemType;
enum ItemType
{
ITEM_TYPE_none,
ITEM_TYPE_flint_sword,
ITEM_TYPE_flint_axe,
ITEM_TYPE_flint,
ITEM_TYPE_twig,
ITEM_TYPE_crafting_tool,
ITEM_TYPE_test_chestpiece,
ITEM_TYPE_MAX,
};
global ItemTypeData global_item_type_data[ITEM_TYPE_MAX] = {
    { "none", STATIC_SPRITE_INVALID, STATIC_SPRITE_INVALID, 0, 0, 0, },
    { "Flint Sword", STATIC_SPRITE_flint_sword_icon, STATIC_SPRITE_flint_sword_ground, 1, ITEM_CATEGORY_sword, ITEM_FLAGS_hotbarable | ITEM_FLAGS_primary_usable | ITEM_FLAGS_enchantable, },
    { "Flint Axe", STATIC_SPRITE_flint_axe, STATIC_SPRITE_flint_axe, 1, ITEM_CATEGORY_lumber_axe, ITEM_FLAGS_hotbarable | ITEM_FLAGS_primary_usable | ITEM_FLAGS_enchantable, },
    { "Flint", STATIC_SPRITE_flint, STATIC_SPRITE_flint, 8, 0, 0, },
    { "Twig", STATIC_SPRITE_twig, STATIC_SPRITE_twig, 8, 0, 0, },
    { "Crafting Tool", STATIC_SPRITE_crafting_tool, STATIC_SPRITE_crafting_tool, 1, 0, ITEM_FLAGS_hotbarable, },
    { "Test Chestpiece", STATIC_SPRITE_test_chestpiece, STATIC_SPRITE_test_chestpiece, 1, ITEM_CATEGORY_chestpiece, 0, },
};

static char *GetItemTypeName(ItemType type);

typedef struct Item
{
ItemType type;
i32 stack_size;
Enchantment enchantments[ENCHANTMENT_TYPE_MAX];
} Item;

#define MAX_CHILD_SKILLS (4)
typedef enum ElementalSkillType ElementalSkillType;

typedef struct ElementalSkillTypeData
{
char print_name[20];
i32 cost;
ElementalSkillType child_skills[MAX_CHILD_SKILLS];
StaticSprite skill_image;
} ElementalSkillTypeData;

typedef enum ElementalSkillType ElementalSkillType;
enum ElementalSkillType
{
ELEMENTAL_SKILL_TYPE_none,
ELEMENTAL_SKILL_TYPE_hand_flame,
ELEMENTAL_SKILL_TYPE_hand_flame_2,
ELEMENTAL_SKILL_TYPE_hand_flame_3,
ELEMENTAL_SKILL_TYPE_MAX,
};
global ElementalSkillTypeData global_elemental_skill_type_data[ELEMENTAL_SKILL_TYPE_MAX] = {
    { "none", 0, 0, 0, },
    { "Hand Flame", 3, { ELEMENTAL_SKILL_TYPE_hand_flame_2, ELEMENTAL_SKILL_TYPE_hand_flame_3 }, STATIC_SPRITE_test_icon, },
    { "Hand Flame 2", 4, 0, 0, },
    { "Hand Flame 3", 5, 0, 0, },
};

static char *GetElementalSkillTypeName(ElementalSkillType type);

typedef struct Entity Entity;

typedef struct LineCol
{
v2 p1;
v2 p2;
} LineCol;

#define MAX_LINE_SEGMENT_VERTICES (33)
typedef struct LineSegments
{
i32 count;
v2 vertices[MAX_LINE_SEGMENT_VERTICES];
} LineSegments;

typedef union c2Shape
{
c2AABB aabb;
c2Capsule capsule;
c2Circle circle;
c2Poly poly;
LineCol line;
LineSegments line_segments;
} c2Shape;

typedef enum c2ShapeType c2ShapeType;
enum c2ShapeType
{
C2_SHAPE_TYPE_aabb,
C2_SHAPE_TYPE_capsule,
C2_SHAPE_TYPE_circle,
C2_SHAPE_TYPE_poly,
C2_SHAPE_TYPE_line,
C2_SHAPE_TYPE_line_segments,
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
CRAFTING_RECIPE_TYPE_flint_axe,
CRAFTING_RECIPE_TYPE_flint_sword,
CRAFTING_RECIPE_TYPE_MAX,
};
global CraftingRecipeTypeData global_crafting_recipe_type_data[CRAFTING_RECIPE_TYPE_MAX] = {
    { {0}, {0}, },
    { {ITEM_TYPE_flint_axe, 1}, {{ITEM_TYPE_flint, 1}, {ITEM_TYPE_twig, 1}}, },
    { {ITEM_TYPE_flint_sword, 1}, {{ITEM_TYPE_flint, 3}, {ITEM_TYPE_twig, 2}}, },
};

static char *GetCraftingRecipeTypeName(CraftingRecipeType type);

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
StructureBuildCallback on_structure_build;
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
    { "none", STRUCTURE_CATEGORY_none, STATIC_SPRITE_INVALID, STATIC_SPRITE_INVALID, {0}, 0, },
    { "Shia", STRUCTURE_CATEGORY_shia, STATIC_SPRITE_shia, STATIC_SPRITE_shia, {0}, 0, },
    { "Crafting Stump", STRUCTURE_CATEGORY_crafting, STATIC_SPRITE_crafting_stump, STATIC_SPRITE_crafting_stump, { {ITEM_TYPE_twig, 5}, {ITEM_TYPE_flint, 2} }, 0, },
    { "Shia 2", STRUCTURE_CATEGORY_shia, STATIC_SPRITE_shia2, STATIC_SPRITE_shia2, {0}, 0, },
    { "Base", STRUCTURE_CATEGORY_base, STATIC_SPRITE_shia2, STATIC_SPRITE_shia2, {0}, 0, },
};

static char *GetStructureTypeName(StructureType type);

typedef enum MagicType MagicType;
enum MagicType
{
MAGIC_TYPE_conscientiousness,
MAGIC_TYPE_elemental,
MAGIC_TYPE_big_spook,
MAGIC_TYPE_MAX,
};
static char *GetMagicTypeName(MagicType type);

typedef enum ElementalAffinity ElementalAffinity;
enum ElementalAffinity
{
ELEMENTAL_AFFINITY_earth,
ELEMENTAL_AFFINITY_fire,
ELEMENTAL_AFFINITY_water,
ELEMENTAL_AFFINITY_air,
ELEMENTAL_AFFINITY_MAX,
};
static char *GetElementalAffinityName(ElementalAffinity type);

typedef struct SpellTypeData
{
char print_name[20];
MagicType magic_type;
f32 mana_cost;
SpellCastCallback cast_callback;
} SpellTypeData;

typedef enum SpellType SpellType;
enum SpellType
{
SPELL_TYPE_none,
SPELL_TYPE_fireball,
SPELL_TYPE_yeet,
SPELL_TYPE_MAX,
};
global SpellTypeData global_spell_type_data[SPELL_TYPE_MAX] = {
    { "none", MAGIC_TYPE_big_spook, 0.0f, 0, },
    { "yeetus maximus", MAGIC_TYPE_elemental, 10.0f, Fireball, },
    { "yeetus maximus", MAGIC_TYPE_elemental, 10.0f, Yeet, },
};

static char *GetSpellTypeName(SpellType type);

typedef struct Spell
{
SpellType type;
f32 last_used;
} Spell;

typedef struct Chunk Chunk;

// @GenerateComponentCode 
#define Dummy2 (_)
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

typedef enum EntityProperty EntityProperty;
enum EntityProperty
{
ENTITY_PROPERTY_is_allocated,
ENTITY_PROPERTY_is_character,
ENTITY_PROPERTY_map_entity,
ENTITY_PROPERTY_positional,
ENTITY_PROPERTY_interactable,
ENTITY_PROPERTY_interactable_left_click,
ENTITY_PROPERTY_lumber_axable,
ENTITY_PROPERTY_enemy,
ENTITY_PROPERTY_enchanter,
ENTITY_PROPERTY_elemental_skill_tree,
ENTITY_PROPERTY_item,
ENTITY_PROPERTY_sprite,
ENTITY_PROPERTY_flipbook,
ENTITY_PROPERTY_parallaxable,
ENTITY_PROPERTY_physical,
ENTITY_PROPERTY_blueprint,
ENTITY_PROPERTY_queryable,
ENTITY_PROPERTY_terrain_segment,
ENTITY_PROPERTY_MAX,
};
static char *GetEntityPropertyName(EntityProperty type);

#define ENTITY_PROPERTY_SIZE (((ENTITY_PROPERTY_MAX/64)+1))
typedef struct Entity
{
u64 properties[ENTITY_PROPERTY_SIZE];
i32 testint;
char debug_name[100];
v2 position;
SpriteData sprite_data;
b8 is_flipped;
b8 is_background_sprite;
AnimationFlags animation_flags;
i32 current_frame;
f32 interval_mult;
f32 frame_start_time;
PhysicsBodyData physics;
v2 smooth_velocity;
f32 axis_x;
f32 move_speed;
f32 move_speed_mult;
ArcEntityType entity_type;
// @DoNotSerialise 
char *current_general_state;
ArcEntityAnimationState current_animation_state;
Item item;
Enchantment enchantments[MAX_ENCHANTMENTS];
v2 parallax_amount;
v2 desired_position;
f32 priority;
StructureType structure_type;
Item remaining_items_in_blueprint[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
f32 durability;
} Entity;

typedef struct EntityPresetTypeData
{
char print_name[20];
EntityPresetCallback setup_callback;
} EntityPresetTypeData;

typedef enum EntityPresetType EntityPresetType;
enum EntityPresetType
{
ENTITY_PRESET_TYPE_none,
ENTITY_PRESET_TYPE_ground_segment,
ENTITY_PRESET_TYPE_MAX,
};
global EntityPresetTypeData global_entity_preset_type_data[ENTITY_PRESET_TYPE_MAX] = {
    { "none", 0, },
    { "Terrain Segment", TerrainSegmentEntityPresetCallback, },
};

static char *GetEntityPresetTypeName(EntityPresetType type);

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

#define CHUNK_FLAGS_is_allocated (1<<0)
typedef uint32 ChunkFlags;

#define MAX_TERRAIN_VERT_IN_CHUNK (32)
#define CHUNK_VERSION (0)
typedef struct Chunk
{
ChunkFlags flags;
Entity *entities[512];
i32 entity_count;
iv2 pos;
v2 terrain_verts[MAX_TERRAIN_VERT_IN_CHUNK];
} Chunk;

typedef enum RenderableType RenderableType;
enum RenderableType
{
RENDERABLE_TYPE_texture,
RENDERABLE_TYPE_text,
RENDERABLE_TYPE_filled_rect,
RENDERABLE_TYPE_line,
RENDERABLE_TYPE_MAX,
};
static char *GetRenderableTypeName(RenderableType type);

typedef struct Texture
{
Ts2dTexture *texture;
i32 flags;
v4 source;
v4 destination;
v4 tint;
} Texture;

typedef struct Text
{
Ts2dFont *font;
i32 flags;
v4 colour;
v2 pos;
f32 font_scale;
char text[512];
} Text;

typedef struct FilledRect
{
v4 colour;
v4 rect;
} FilledRect;

typedef struct Line
{
v4 colour;
v2 p1;
v2 p2;
} Line;

#define MAX_QUEUED_RENDERABLES (1024)
typedef union Renderable
{
Texture texture;
Text text;
FilledRect filled_rect;
Line line;
} Renderable;

typedef struct SortRenderable
{
Renderable data;
RenderableType type;
f32 layer;
v4 clip;
} SortRenderable;

#define CHARACTER_STATE_is_crafting (1<<0)
#define CHARACTER_STATE_is_backpack_open (1<<1)
#define CHARACTER_STATE_is_blueprinting (1<<2)
#define CHARACTER_STATE_is_enchanting (1<<3)
#define CHARACTER_STATE_is_in_elemental_skill_tree (1<<4)
#define CHARACTER_STATE_arcane_mode (1<<5)
#define CHARACTER_STATE_is_charging (1<<6)
#define CHARACTER_STATE_in_action (1<<7)
typedef uint32 CharacterState;

typedef enum CharacterCombatState CharacterCombatState;
enum CharacterCombatState
{
CHARACTER_COMBAT_STATE_none,
CHARACTER_COMBAT_STATE_light_charge,
CHARACTER_COMBAT_STATE_heavy_charge,
CHARACTER_COMBAT_STATE_light_attack,
CHARACTER_COMBAT_STATE_heavy_attack,
CHARACTER_COMBAT_STATE_MAX,
};
static char *GetCharacterCombatStateName(CharacterCombatState type);

#define MAX_ACTIVE_TIMERS (64)
typedef struct Timer
{
b8 is_allocated;
f32 start_time;
f32 duration;
TimerCompleteCallback complete_callback;
} Timer;

typedef struct CameraCue
{
v2 position;
f32 weight;
f32 zoom;
} CameraCue;

#define MAX_HOTBAR_SLOTS (9)
#define MAX_INVENTORY_SLOTS (9)
#define MAX_SPELL_SLOTS (8)
#define MAX_EQUIPMENT_SLOTS (6)
typedef struct CharacterData
{
Item inventory[MAX_INVENTORY_SLOTS];
i32 inventory_size;
Item hotbar[MAX_HOTBAR_SLOTS];
i32 hotbar_size;
i32 active_hotbar_slot;
Item grabbed_item;
v2 grabbed_item_offset;
// @DoNotSerialise 
Item *grabbed_item_origin_slot;
Spell freehand_spell_slots[MAX_SPELL_SLOTS];
i32 freehand_spell_count;
Item equipment_slots[MAX_EQUIPMENT_SLOTS];
b8 unlocked_elemental_skills[ELEMENTAL_SKILL_TYPE_MAX];
ElementalSkillType purchased_elemental_skills[ELEMENTAL_SKILL_TYPE_MAX];
i32 elemental_skill_points;
} CharacterData;

typedef struct WorldData
{
CharacterData character_data;
f32 elapsed_world_time;
iv2 character_chunk;
} WorldData;

#define MAX_POSITIONAL_ENTITIES (2048)
#define MAX_FLOATING_ENTITIES (2048)
#define ENTITY_TABLE_SIZE ((MAX_POSITIONAL_ENTITIES+MAX_FLOATING_ENTITIES))
#define MAX_CAMERA_CUES (32)
typedef struct RunData
{
Timer timers[MAX_ACTIVE_TIMERS];
Chunk chunks[MAX_WORLD_CHUNKS];
Entity entities[ENTITY_TABLE_SIZE];
char world_name[50];
char world_path[300];
char world_chunks_path[300];
WorldData world_data;
b8 is_paused;
Entity *character_entity;
CharacterState character_state;
CharacterCombatState character_combat_state;
f32 cooldown_timer;
Entity *current_e_interactable;
Entity *current_left_click_interactable;
Entity *engaged_station_entity;
Entity *current_blueprint_structure;
SortRenderable queued_renderables[MAX_QUEUED_RENDERABLES];
i32 queued_renderable_count;
CameraCue camera_cues[MAX_CAMERA_CUES];
i32 camera_cue_count;
b8 lock_camera;
v2 initial_camera_pos;
f32 inital_zoom;
v2 move_to_location;
f32 move_to_zoom;
f32 move_to_time;
f32 move_to_start_time;
b8 is_ejected;
EditorState editor_state;
DebugFlags debug_flags;
Entity *selected_entity;
iv2 selected_chunk;
} RunData;

typedef struct ClientData
{
b32 bloom;
} ClientData;

static void WriteEntityToFile(FILE *file, Entity *data);

static void ReadEntityFromFile(FILE *file, Entity *data);

static void WriteCharacterDataToFile(FILE *file, CharacterData *data);

static void ReadCharacterDataFromFile(FILE *file, CharacterData *data);

static void WriteWorldDataToFile(FILE *file, WorldData *data);

static void ReadWorldDataFromFile(FILE *file, WorldData *data);

static void WriteEntity_Version0ToFile(FILE *file, Entity *data);

static void ReadEntity_Version0FromFile(FILE *file, Entity *data);

static void WriteCharacterData_Version0ToFile(FILE *file, CharacterData *data);

static void ReadCharacterData_Version0FromFile(FILE *file, CharacterData *data);

static void WriteWorldData_Version0ToFile(FILE *file, WorldData *data);

static void ReadWorldData_Version0FromFile(FILE *file, WorldData *data);

