#define MAX_ANIMATION_FRAMES 10

internal void UpdateAnimations();
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