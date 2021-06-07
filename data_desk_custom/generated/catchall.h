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

typedef struct SpriteData
{
char texture_path[50];
v4 source;
v2 offset;
i32 frames;
f32 frame_interval;
// @Ignore 
Ts2dTexture *texture_atlas;
} SpriteData;

typedef enum Sprite Sprite;
enum Sprite
{
SPRITE_default,
SPRITE_ground_arctic,
SPRITE_ground_desert,
SPRITE_ground_plains,
SPRITE_player,
SPRITE_far_mountains,
SPRITE_mid_mountains,
SPRITE_cloud_v1,
SPRITE_cloud_v2,
SPRITE_cloud_v3,
SPRITE_cloud_v4,
SPRITE_cloud_v5,
SPRITE_cloud_v6,
SPRITE_ground_forest_flat,
SPRITE_ground_forest_10_degree,
SPRITE_ground_rocks_v1,
SPRITE_ground_rocks_v2,
SPRITE_pine_tree_v1,
SPRITE_bg1_hills_v1,
SPRITE_bg1_hills_v2,
SPRITE_bg1_hills_left_slant,
SPRITE_bg1_hills_right_slant,
SPRITE_bg1_shrub_v1,
SPRITE_bg1_shrub_v2,
SPRITE_bg1_shrub_v3,
SPRITE_bg1_sapling_v1,
SPRITE_bg1_sapling_v2,
SPRITE_bg1_sapling_v3,
SPRITE_bg1_pine_tree_v1,
SPRITE_bg1_pine_tree_v2,
SPRITE_bg2_hills_v1,
SPRITE_bg2_hills_v2,
SPRITE_bg2_shrub_v1,
SPRITE_bg2_shrub_v2,
SPRITE_bg2_shrub_v3,
SPRITE_bg2_pine_tree_v1,
SPRITE_bg2_pine_tree_v2,
SPRITE_bg3_hills_v1,
SPRITE_bg3_shrub_v1,
SPRITE_bg3_shrub_v2,
SPRITE_bg3_shrub_v3,
SPRITE_bg3_shrub_v4,
SPRITE_bg3_pine_tree_v1,
SPRITE_bg3_pine_tree_v2,
SPRITE_bg3_pine_tree_v3,
SPRITE_bg3_pine_tree_v4,
SPRITE_bg3_pine_tree_v5,
SPRITE_bg3_pine_tree_v6,
SPRITE_bg3_pine_tree_v7,
SPRITE_bg3_pine_tree_v8,
SPRITE_y_axis_arrow_icon,
SPRITE_x_axis_arrow_icon,
SPRITE_circle_icon,
SPRITE_side_arrow,
SPRITE_test_icon,
SPRITE_crafting_stump,
SPRITE_runic_enchanter,
SPRITE_wooden_wall,
SPRITE_flint_sword_icon,
SPRITE_flint_sword_ground,
SPRITE_flint_axe,
SPRITE_flint,
SPRITE_twig,
SPRITE_crafting_tool,
SPRITE_test_chestpiece,
SPRITE_yucca_fiber,
SPRITE_yucca,
SPRITE_default_dynamic,
SPRITE_player_idle,
SPRITE_player_walking,
SPRITE_player_sprinting,
SPRITE_birch_tree1,
SPRITE_birch_tree2,
SPRITE_birch_tree3,
SPRITE_birch_tree4,
SPRITE_MAX,
};
global SpriteData global_sprite_data[SPRITE_MAX] = {
    { "default_texture", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/biome_ground", {0.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, 0, 0.0f, },
    { "scenic/biome_ground", {100.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, 0, 0.0f, },
    { "scenic/biome_ground", {200.0f, 0.0f, 100.0f, 150.0f}, {0.0f, 147.0f}, 0, 0.0f, },
    { "entity/player/temp_player", {0.0f, 0.0f, 14.0f, 35.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/mountains", {0.0f, 0.0f, 600.0f, 150.0f}, {0.0f, 5.0f}, 0, 0.0f, },
    { "scenic/mountains", {0.0f, 150.0f, 600.0f, 100.0f}, {0.0f, 10.0f}, 0, 0.0f, },
    { "scenic/clouds", {0.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/clouds", {80.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/clouds", {160.0f, 0.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/clouds", {0.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/clouds", {80.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/clouds", {160.0f, 25.0f, 80.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/forest_ground", {0.0f, 40.0f, 100.0f, 90.0f }, {20.0f, 87.0f}, 0, 0.0f, },
    { "scenic/forest_ground", {100.0f, 25.0f, 100.0f, 105.0f}, {0.0f, 87.0f}, 0, 0.0f, },
    { "scenic/forest_ground", {0.0f, 0.0f, 100.0f, 4.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/forest_ground", {0.0f, 4.0f, 100.0f, 4.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/trees/pine_tree", {0.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg1_hills", {0.0f, 0.0f, 100.0f, 100.0f}, {0.0f, 80.0f}, 0, 0.0f, },
    { "scenic/background/bg1_hills", {100.0f, 0.0f, 100.0f, 100.0f}, {0.0f, 80.0f}, 0, 0.0f, },
    { "scenic/background/bg1_hills", {0.0f, 100.0f, 100.0f, 100.0f}, {0.0f, 60.0f}, 0, 0.0f, },
    { "scenic/background/bg1_hills", {100.0f, 100.0f, 100.0f, 100.0f}, {0.0f, 60.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {0.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {50.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {100.0f, 0.0f, 50.0f, 25.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {75.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {95.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {115.0f, 25.0f, 20.0f, 40.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg1_pine_tree", {0.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg1_pine_tree", {90.0f, 0.0f, 90.0f, 170.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg2_hills", {0.0f, 0.0f, 200.0f, 40.0f}, {0.0f, 21.0f}, 0, 0.0f, },
    { "scenic/background/bg2_hills", {0.0f, 40.0f, 200.0f, 40.0f}, {0.0f, 21.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {0.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {25.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {50.0f, 25.0f, 25.0f, 20.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg2_pine_tree", {0.0f, 0.0f, 40.0f, 110.0f}, {0.0f, -5.0f}, 0, 0.0f, },
    { "scenic/background/bg2_pine_tree", {40.0f, 0.0f, 40.0f, 110.0f}, {0.0f, -5.0f}, 0, 0.0f, },
    { "scenic/background/bg3_hills", {0.0f, 0.0f, 200.0f, 40.0f}, {0.0f, 17.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {0.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {15.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {30.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/vegetation", {45.0f, 45.0f, 15.0f, 10.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {0.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {20.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {40.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {60.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {80.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {100.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {120.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "scenic/background/bg3_pine_tree", {140.0f, 0.0f, 20.0f, 50.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "icon/axis_icons", {0.0f, 0.0f, 7.0f, 40.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "icon/axis_icons", {0.0f, 33.0f, 40.0f, 7.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "icon/axis_icons", {33.0f, 0.0f, 7.0f, 7.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "icon/side_arrow", {0.0f, 0.0f, 5.0f, 9.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "icon/test_icon", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "structures/crafting_stump", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "structures/runic_enchanter", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "structures/wooden_wall", {0.0f, 0.0f, 27.0f, 60.0f}, {-10.0f, 0.0f}, 0, 0.0f, },
    { "item/flint_sword", {0.0f, 0.0f, 16.0f, 16.0f}, {6.0f, 2.0f}, 0, 0.0f, },
    { "item/flint_sword_ground", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "item/flint_axe", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "item/flint", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "item/twig", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "item/crafting_tool", {0.0f, 0.0f, 16.0f, 16.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "item/test_chestpiece", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "resources/yucca_fiber", {0.0f, 0.0f, 24.0f, 24.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "resources/yucca", {0.0f, 0.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "invalid_texture", {0.0f, 0.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 0, 0.0f, },
    { "entity/player/player_animations", {0.0f, 192.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 4, 0.15f, },
    { "entity/player/player_animations", {0.0f, 256.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 8, 0.1f, },
    { "entity/player/player_animations", {0.0f, 320.0f, 64.0f, 64.0f}, {0.0f, 0.0f}, 8, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 0.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 160.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 320.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
    { "scenic/trees/birch_idle", {0.0f, 480.0f, 120.0f, 160.0f}, {0.0f, 0.0f}, 6, 0.1f, },
};

static char *GetSpriteName(Sprite type);

typedef struct SpriteRender
{
Sprite sprite;
v2 offset;
i8 render_layer;
v2 scale;
v4 tint;
} SpriteRender;

typedef struct ArcEntityAnimationStateData
{
Sprite dynamic_sprite;
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
    { SPRITE_player_idle, },
    { SPRITE_player_walking, },
    { SPRITE_player_sprinting, },
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
Sprite icon_sprite;
Sprite ground_sprite;
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
ITEM_TYPE_yucca_fiber,
ITEM_TYPE_MAX,
};
global ItemTypeData global_item_type_data[ITEM_TYPE_MAX] = {
    { "none", 0, 0, 0, 0, 0, },
    { "Flint Sword", SPRITE_flint_sword_icon, SPRITE_flint_sword_ground, 1, ITEM_CATEGORY_sword, ITEM_FLAGS_hotbarable | ITEM_FLAGS_primary_usable | ITEM_FLAGS_enchantable, },
    { "Flint Axe", SPRITE_flint_axe, SPRITE_flint_axe, 1, ITEM_CATEGORY_lumber_axe, ITEM_FLAGS_hotbarable | ITEM_FLAGS_primary_usable | ITEM_FLAGS_enchantable, },
    { "Flint", SPRITE_flint, SPRITE_flint, 8, 0, 0, },
    { "Twig", SPRITE_twig, SPRITE_twig, 8, 0, 0, },
    { "Crafting Tool", SPRITE_crafting_tool, SPRITE_crafting_tool, 1, 0, ITEM_FLAGS_hotbarable, },
    { "Test Chestpiece", SPRITE_test_chestpiece, SPRITE_test_chestpiece, 1, ITEM_CATEGORY_chestpiece, 0, },
    { "Yucca Fiber", SPRITE_yucca_fiber, SPRITE_yucca_fiber, 8, 0, 0, },
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
Sprite skill_image;
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
    { "Hand Flame", 3, { ELEMENTAL_SKILL_TYPE_hand_flame_2, ELEMENTAL_SKILL_TYPE_hand_flame_3 }, SPRITE_test_icon, },
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

#define MAX_LINE_SEGMENT_VERTICES (32)
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
    { {ITEM_TYPE_flint_axe, 1}, {{ITEM_TYPE_flint, 1}, {ITEM_TYPE_twig, 1}, {ITEM_TYPE_yucca_fiber, 1}}, },
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

#define MAX_ITEMS_IN_BLUEPRINT_RECIPE (8)
typedef struct StructureTypeData
{
char print_name[20];
StructureCategory category;
Sprite icon_sprite;
Sprite world_sprite;
Item recipe[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
StructureBuildCallback on_structure_build;
} StructureTypeData;

typedef enum StructureType StructureType;
enum StructureType
{
STRUCTURE_TYPE_none,
STRUCTURE_TYPE_wooden_wall,
STRUCTURE_TYPE_MAX,
};
global StructureTypeData global_structure_type_data[STRUCTURE_TYPE_MAX] = {
    { "none", 0, 0, 0, {0}, 0, },
    { "Wooden Wall", 0, SPRITE_wooden_wall, SPRITE_wooden_wall, {{ITEM_TYPE_twig, 4}, {ITEM_TYPE_yucca_fiber, 2}}, OnWoodenWallBuild, },
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

typedef enum TreeType TreeType;
enum TreeType
{
TREE_TYPE_oak,
TREE_TYPE_birch,
TREE_TYPE_pine,
TREE_TYPE_MAX,
};
static char *GetTreeTypeName(TreeType type);

typedef enum EntityProperty EntityProperty;
enum EntityProperty
{
ENTITY_PROPERTY_is_allocated,
ENTITY_PROPERTY_is_character,
ENTITY_PROPERTY__empty,
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
ENTITY_PROPERTY_tree,
ENTITY_PROPERTY_do_not_serialise,
ENTITY_PROPERTY_text_note,
ENTITY_PROPERTY_MAX,
};
static char *GetEntityPropertyName(EntityProperty type);

#define ENTITY_PROPERTY_SIZE (((ENTITY_PROPERTY_MAX/64)+1))
typedef struct Entity_Version1
{
u64 properties[ENTITY_PROPERTY_SIZE];
char debug_name[128];
v2 position;
char note[1024];
SpriteRender sprite_data;
b8 is_flipped;
b8 is_background_sprite;
v4 previous_parallax_rect;
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
f32 priority;
StructureType structure_type;
Item remaining_items_in_blueprint[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
f32 durability;
TreeType tree_type;
} Entity_Version1;

typedef struct Entity
{
u64 properties[ENTITY_PROPERTY_SIZE];
char debug_name[128];
v2 position;
// @MapFrom(note) 
char note[128];
SpriteRender sprite_data;
b8 is_flipped;
b8 is_background_sprite;
v4 previous_parallax_rect;
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
f32 priority;
StructureType structure_type;
Item remaining_items_in_blueprint[MAX_ITEMS_IN_BLUEPRINT_RECIPE];
f32 durability;
TreeType tree_type;
} Entity;

typedef enum EntityPresetCategory EntityPresetCategory;
enum EntityPresetCategory
{
ENTITY_PRESET_CATEGORY_none,
ENTITY_PRESET_CATEGORY_resource,
ENTITY_PRESET_CATEGORY_background1,
ENTITY_PRESET_CATEGORY_background2,
ENTITY_PRESET_CATEGORY_background3,
ENTITY_PRESET_CATEGORY_far_background,
ENTITY_PRESET_CATEGORY_misc,
ENTITY_PRESET_CATEGORY_MAX,
};
static char *GetEntityPresetCategoryName(EntityPresetCategory type);

typedef struct EntityPresetTypeData
{
char print_name[20];
EntityPresetCallback setup_callback;
EntityPresetCategory category;
} EntityPresetTypeData;

typedef enum EntityPresetType EntityPresetType;
enum EntityPresetType
{
ENTITY_PRESET_TYPE_none,
ENTITY_PRESET_TYPE_tree,
ENTITY_PRESET_TYPE_bg1_hill,
ENTITY_PRESET_TYPE_bg1_tree,
ENTITY_PRESET_TYPE_bg1_shrubs,
ENTITY_PRESET_TYPE_bg1_sapling,
ENTITY_PRESET_TYPE_bg2_hill,
ENTITY_PRESET_TYPE_bg2_tree,
ENTITY_PRESET_TYPE_bg2_shrubs,
ENTITY_PRESET_TYPE_bg3_hill,
ENTITY_PRESET_TYPE_bg3_trees,
ENTITY_PRESET_TYPE_bg3_shrubs,
ENTITY_PRESET_TYPE_mid_mountains,
ENTITY_PRESET_TYPE_far_mountains,
ENTITY_PRESET_TYPE_text_note,
ENTITY_PRESET_TYPE_item,
ENTITY_PRESET_TYPE_blueprint,
ENTITY_PRESET_TYPE_yucca,
ENTITY_PRESET_TYPE_MAX,
};
global EntityPresetTypeData global_entity_preset_type_data[ENTITY_PRESET_TYPE_MAX] = {
    { "none", 0, 0, },
    { "Tree", TreeEntityPresetCallback, ENTITY_PRESET_CATEGORY_resource, },
    { "BG1 Hill", BG1HillEntityPresetCallback, ENTITY_PRESET_CATEGORY_background1, },
    { "BG1 Tree", BG1TreeEntityPresetCallback, ENTITY_PRESET_CATEGORY_background1, },
    { "BG1 Shrubs", BG1ShrubsEntityPresetCallback, ENTITY_PRESET_CATEGORY_background1, },
    { "BG1 Sapling", BG1SaplingEntityPresetCallback, ENTITY_PRESET_CATEGORY_background1, },
    { "BG2 Hill", BG2HillEntityPresetCallback, ENTITY_PRESET_CATEGORY_background2, },
    { "BG2 Tree", BG2TreeEntityPresetCallback, ENTITY_PRESET_CATEGORY_background2, },
    { "BG2 Shrubs", BG2ShrubsEntityPresetCallback, ENTITY_PRESET_CATEGORY_background2, },
    { "BG3 Hill", BG3HillEntityPresetCallback, ENTITY_PRESET_CATEGORY_background3, },
    { "BG3 Trees", BG3TreesEntityPresetCallback, ENTITY_PRESET_CATEGORY_background3, },
    { "BG3 Shrubs", BG3ShrubsEntityPresetCallback, ENTITY_PRESET_CATEGORY_background3, },
    { "Mid Mountains", MidMountainsEntityPresetCallback, ENTITY_PRESET_CATEGORY_far_background, },
    { "Far Mountains", FarMountainsEntityPresetCallback, ENTITY_PRESET_CATEGORY_far_background, },
    { "Text Note", TextNoteEntityPresetCallback, ENTITY_PRESET_CATEGORY_misc, },
    { "Item", ItemEntityPresetCallback, ENTITY_PRESET_CATEGORY_misc, },
    { "Blueprint", BlueprintEntityPresetCallback, ENTITY_PRESET_CATEGORY_misc, },
    { "Yucca", YuccaEntityPresetCallback, ENTITY_PRESET_CATEGORY_misc, },
};

static char *GetEntityPresetTypeName(EntityPresetType type);

typedef enum RenderableType RenderableType;
enum RenderableType
{
RENDERABLE_TYPE_texture,
RENDERABLE_TYPE_text,
RENDERABLE_TYPE_rect,
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

typedef struct Rect
{
v4 colour;
v4 rect;
} Rect;

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
Rect rect;
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

#define ENTITY_TABLE_SIZE (2048)
#define TERRAIN_TABLE_SIZE (4096)
#define MAX_CAMERA_CUES (32)
typedef struct RunData
{
Timer timers[MAX_ACTIVE_TIMERS];
Entity entities[ENTITY_TABLE_SIZE];
v2 terrain[TERRAIN_TABLE_SIZE];
char world_name[50];
char world_path[300];
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
} RunData;

typedef struct ClientData
{
b32 bloom;
} ClientData;

#define DEBUG_FLAGS_draw_collision (1<<0)
#define DEBUG_FLAGS_disable_draw_terrain (1<<1)
typedef uint32 DebugFlags;

#define MAX_SELECTED_ENTITIES (128)
typedef struct EditorData
{
i32 selected_entities[MAX_SELECTED_ENTITIES];
DebugFlags debug_flags;
i32 grid_width;
i8 view_min;
i8 view_max;
v2 last_camera_pos;
f32 last_camera_zoom;
} EditorData;

static void WriteEntityToFile(FILE *file, Entity *data);

static void ReadEntityFromFile(FILE *file, Entity *data);

static void WriteCharacterDataToFile(FILE *file, CharacterData *data);

static void ReadCharacterDataFromFile(FILE *file, CharacterData *data);

static void WriteWorldDataToFile(FILE *file, WorldData *data);

static void ReadWorldDataFromFile(FILE *file, WorldData *data);

static void WriteEditorDataToFile(FILE *file, EditorData *data);

static void ReadEditorDataFromFile(FILE *file, EditorData *data);

static void WriteEntity_Version1ToFile(FILE *file, Entity_Version1 *data);

static void ReadEntity_Version1FromFile(FILE *file, Entity_Version1 *data);

static void MapEntity_Version1ToEntity(Entity_Version1 origin, Entity *dest);

static void WriteEntity_Version2ToFile(FILE *file, Entity *data);

static void ReadEntity_Version2FromFile(FILE *file, Entity *data);

static void WriteCharacterData_Version0ToFile(FILE *file, CharacterData *data);

static void ReadCharacterData_Version0FromFile(FILE *file, CharacterData *data);

static void WriteWorldData_Version0ToFile(FILE *file, WorldData *data);

static void ReadWorldData_Version0FromFile(FILE *file, WorldData *data);

static void WriteEditorData_Version0ToFile(FILE *file, EditorData *data);

static void ReadEditorData_Version0FromFile(FILE *file, EditorData *data);

