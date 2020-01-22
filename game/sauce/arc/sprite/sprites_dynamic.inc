/*
	sprite_name, 
	sprite_atlas_path, 
	sprite_source, 
	sprite_offset, 
	sprite_frames, 
	sprite_frame_interval,
*/

// Player
DynamicSprite(
	player_idle_body, 
	"entity/player/player_animations", 
	v4(0.0f, 192.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	4, 
	0.15f
)
DynamicSprite(
	player_idle_arm, 
	"entity/player/player_animations", 
	v4(0.0f, 256.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	4, 
	0.15f
)

DynamicSprite(
	player_walking_body, 
	"entity/player/player_animations", 
	v4(0.0f, 320.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	8, 
	0.1f
)
DynamicSprite(
	player_walking_arm, 
	"entity/player/player_animations", 
	v4(0.0f, 384.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	8, 
	0.1f
)

DynamicSprite(
	player_sprinting_body, 
	"entity/player/player_animations", 
	v4(0.0f, 448.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	8, 
	0.1f
)
DynamicSprite(
	player_sprinting_arm, 
	"entity/player/player_animations", 
	v4(0.0f, 512.0f, 64.0f, 64.0f), 
	v2(0.0f, 0.0f), 
	8, 
	0.1f
)

// Birch Tree
DynamicSprite(
	birch_tree1, 
	"scenic/trees/birch_idle", 
	v4(0.0f, 0.0f, 120.0f, 160.0f), 
	v2(0.0f, 0.0f), 
	6, 
	0.1f
)
DynamicSprite(
	birch_tree2, 
	"scenic/trees/birch_idle", 
	v4(0.0f, 160.0f, 120.0f, 160.0f), 
	v2(0.0f, 0.0f), 
	6, 
	0.1f
)
DynamicSprite(
	birch_tree3, 
	"scenic/trees/birch_idle", 
	v4(0.0f, 320.0f, 120.0f, 160.0f), 
	v2(0.0f, 0.0f), 
	6, 
	0.1f
)
DynamicSprite(
	birch_tree4, 
	"scenic/trees/birch_idle", 
	v4(0.0f, 480.0f, 120.0f, 160.0f), 
	v2(0.0f, 0.0f), 
	6, 
	0.1f
)