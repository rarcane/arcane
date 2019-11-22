#define MAX_SUB_SPRITES 5

typedef enum AnimationStateType AnimationStateType;
typedef struct ArcEntityAnimationState ArcEntityAnimationState;
typedef enum ArcEntityType ArcEntityType;
typedef struct ArcEntity ArcEntity;

enum AnimationStateType
{
#define Entity(entity_name, entity_general_states, entity_animation_states)
#define EntityAnimationState(entity_animation_state_name, entity_animation_state_dynamic_sprites, entity_animation_state_sprite_count) ANIMATION_STATE_##entity_animation_state_name,
#include "entities.inc"
#undef EntityAnimationState
#undef Entity
	ANIMATION_STATE_MAX
};

struct ArcEntityAnimationState
{
	SpriteType dynamic_sprites[MAX_SUB_SPRITES];
	i32 dynamic_sprite_count;
};
global ArcEntityAnimationState arc_entity_animation_state_data[ANIMATION_STATE_MAX] = {
#define Entity(entity_name, entity_general_states, entity_animation_states)
#define EntityAnimationState(entity_animation_state_name, entity_animation_state_dynamic_sprites, entity_animation_state_sprite_count) {entity_animation_state_dynamic_sprites, entity_animation_state_sprite_count},
#include "entities.inc"
#undef EntityAnimationState
#undef Entity
};

enum ArcEntityType
{
#define EntityAnimationState(entity_animation_state_name, entity_animation_state_dynamic_sprites, entity_animation_state_sprite_count)
#define Entity(entity_name, entity_general_states, entity_animation_states) ARC_ENTITY_##entity_name,
#include "entities.inc"
#undef Entity
#undef EntityAnimationState
	ARC_ENTITY_MAX,
};

struct ArcEntity
{
	const char *general_states[10];
	AnimationStateType animation_states[20];
};
global ArcEntity arc_entity_data[ARC_ENTITY_MAX] = {
#define EntityAnimationState(entity_animation_state_name, entity_animation_state_dynamic_sprites, entity_animation_state_sprite_count)
#define Entity(entity_name, entity_general_states, entity_animation_states) {entity_general_states, entity_animation_states},
#include "entities.inc"
#undef Entity
#undef EntityAnimationState
};