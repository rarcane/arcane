#define MAX_ANIMATION_FRAMES 10

// render_layer
// -128 -> 128
// -28 -> 28 is parallax free
#define LAYER_FRONT_UI -5
#define LAYER_FRONT_GROUND -4
#define LAYER_HUD -4
#define LAYER_GROUND -3
#define LAYER_ITEMS -1
#define LAYER_PLAYER 0
#define LAYER_GROUND_ROCKS 1 // Small rocks that go behind the player to give extra depth
#define LAYER_TREE 2
#define LAYER_SHRUB 3

// Background parallax
#define LAYER_BG1_HILL 28 + 40
#define LAYER_BG1_TREE 28 + 41
#define LAYER_BG1_SHRUBS 28 + 42
#define LAYER_BG1_SAPLING 28 + 43
#define LAYER_BG2_HILL 28 + 60
#define LAYER_BG2_TREE 28 + 61
#define LAYER_BG2_SHRUBS 28 + 62
#define LAYER_BG3_HILL 28 + 75
#define LAYER_BG3_TREES 28 + 76
#define LAYER_BG3_SHRUBS 28 + 77
#define LAYER_MID_MOUNTAINS 28 + 93
#define LAYER_FAR_MOUNTAINS 28 + 95

internal void UpdateAnimations();
internal void ArcPushTexture(Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint, f32 layer);
internal void RenderForegroundSprites();
internal void RenderBackgroundSprites();

internal void ArcPushLine(v4 colour, v2 p1, v2 p2, f32 layer);