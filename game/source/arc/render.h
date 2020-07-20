#define MAX_ANIMATION_FRAMES 10

#define LAYER_FRONT_GROUND -1
#define LAYER_FOREGROUND_GEORGE 0
#define LAYER_HUD 0.5
#define LAYER_GROUND_ROCKS 1
#define LAYER_TREE 2
#define LAYER_SHRUB 3

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