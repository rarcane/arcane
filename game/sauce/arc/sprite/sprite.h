#define MAX_ANIMATION_FRAMES 10

typedef enum SpriteType SpriteType;
typedef struct StaticSprite StaticSprite;
typedef struct SpriteSocket SpriteSocket;
typedef struct DynamicSprite DynamicSprite;

internal void InitialiseSpriteData();
internal b8 IsSpriteDynamic(SpriteType sprite_enum);
internal StaticSprite *GetStaticSprite(SpriteType static_sprite_enum);
internal DynamicSprite *GetDynamicSprite(SpriteType dynamic_sprite_enum);

typedef struct AnimationComponent AnimationComponent;
internal void UpdateAnimations(AnimationComponent animation_components[], i32 count);
typedef struct SpriteComponent SpriteComponent;
typedef struct SubSpriteComponent SubSpriteComponent;
internal void RenderForegroundSprites(SpriteComponent sprite_components[], SubSpriteComponent sub_sprite_components[], i32 sprite_count, i32 sub_sprite_count);

enum SpriteType
{
	INVALID_SPRITE,
#define StaticSprite(sprite_name, sprite_atlas_path, sprite_source, sprite_offset) STATIC_SPRITE_##sprite_name,
#include "sprites_static.inc"
#undef StaticSprite
	STATIC_SPRITE_MAX,

#define DynamicSprite(sprite_name, sprite_atlas_path, sprite_source, sprite_offset, sprite_frames, sprite_frame_interval) DYNAMIC_SPRITE_##sprite_name,
#include "sprites_dynamic.inc"
#undef DynamicSprite
	DYNAMIC_SPRITE_MAX,
};

struct StaticSprite
{
	Ts2dTexture *texture_atlas;
	v4 source;
	v2 offset;
};
global StaticSprite static_sprite_data[STATIC_SPRITE_MAX] = {0}; // TODO: Should this be declared on a memory arena?

struct SpriteSocket
{
	char name[20];
	v2 position;
};

struct DynamicSprite
{
	Ts2dTexture *texture_atlas;
	v4 source;
	v2 offset;
	i32 frames;
	f32 frame_interval;
	SpriteSocket sockets[MAX_ANIMATION_FRAMES];
	i32 socket_count;
};
global DynamicSprite dynamic_sprite_data[DYNAMIC_SPRITE_MAX - STATIC_SPRITE_MAX - 1] = {0};