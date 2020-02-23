#define Wrapped(...) {__VA_ARGS__}

// --- entity_name, entity_general_states, entity_animation_states --- //

EntityAnimationState(player_idle, Wrapped(DYNAMIC_SPRITE_player_idle_body, DYNAMIC_SPRITE_player_idle_arm), 2)
EntityAnimationState(player_walking, Wrapped(DYNAMIC_SPRITE_player_walking_body, DYNAMIC_SPRITE_player_walking_arm), 2)
EntityAnimationState(player_sprinting, Wrapped(DYNAMIC_SPRITE_player_sprinting_body, DYNAMIC_SPRITE_player_sprinting_arm), 2)
Entity(
	player, 
	Wrapped("Idle", "Walking", "Sprinting"), 
	Wrapped(ANIMATION_STATE_player_idle, ANIMATION_STATE_player_walking, ANIMATION_STATE_player_sprinting)
)

#undef Wrapped