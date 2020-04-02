#define MAX_ANIMATION_FRAMES 10

internal void UpdateAnimations();
internal void RenderForegroundSprites();
internal void RenderBackgroundSprites();

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
	STATIC_SPRITE_middle_axis_icon,
	STATIC_SPRITE_MAX,
};

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

typedef struct SpriteData
{
	StaticSprite static_sprite;
	DynamicSprite dynamic_sprite;
	v2 offset;
	f32 render_layer;
	v2 scale;
	v4 tint;
} SpriteData;