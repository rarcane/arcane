#define MAX_ANIMATION_FRAMES 10

#define LAYER_FRONT_UI -5.0f
#define LAYER_FRONT_GROUND -1.0f
#define LAYER_FOREGROUND_GEORGE 0.0f
#define LAYER_HUD 0.5f
#define LAYER_GROUND_ROCKS 1.0f
#define LAYER_TREE 2.0f
#define LAYER_SHRUB 3.0f
#define LAYER_HILL_1 4.0f
#define LAYER_BG_TREE_1 5.0f
#define LAYER_BG_SHRUBS_1 6.0f
#define LAYER_BG_SAPLINGS_1 7.0f
#define LAYER_HILL_2 8.0f
#define LAYER_BG_TREES_2 9.0f
#define LAYER_BG_SHRUBS_2 10.0f
#define LAYER_HILLS_3 11.0f
#define LAYER_BG_TREES_3 12.0f
#define LAYER_BG_SHRUBS_3 13.0f
#define LAYER_MID_MOUNTAINS 14.0f
#define LAYER_FAR_MOUNTAINS 15.0f

internal void UpdateAnimations();
internal void ArcPushTexture(Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint, f32 layer);
internal void RenderForegroundSprites();
internal void RenderBackgroundSprites();

typedef enum StaticSprite StaticSprite;
typedef enum DynamicSprite DynamicSprite;

typedef struct SpriteData
{
	StaticSprite static_sprite;
	DynamicSprite dynamic_sprite;
	v2 offset;
	f32 render_layer;
	v2 scale;
	v4 tint;
} SpriteData;