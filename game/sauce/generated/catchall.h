typedef enum EntityType EntityType;
enum EntityType
{
ENTITY_TYPE_undefined,
ENTITY_TYPE_character,
ENTITY_TYPE_monster,
ENTITY_TYPE_animal,
ENTITY_TYPE_item,
ENTITY_TYPE_storage,
ENTITY_TYPE_resource,
ENTITY_TYPE_scenic,
ENTITY_TYPE_ground,
ENTITY_TYPE_MAX,
};
static char *GetEntityTypeTypeName(EntityType type);

#define ENTITY_FLAGS_no_delete (1<<0)
typedef unsigned int EntityFlags;

#define COLLIDER_FLAGS_ground (1<<0)
#define COLLIDER_FLAGS_player (1<<1)
#define COLLIDER_FLAGS_entity (1<<2)
#define COLLIDER_FLAGS_item (1<<3)
#define COLLIDER_FLAGS_trigger (1<<4)
typedef unsigned int ColliderFlags;

#define ANIMATION_FLAGS_playing (1<<0)
#define ANIMATION_FLAGS_reversing (1<<1)
#define ANIMATION_FLAGS_repeat (1<<2)
typedef unsigned int AnimationFlags;

#define MAX_ENTITIES 1000

#define MAX_OVERLAPPING_COLLIDERS 50

#define MAX_ENTITY_REFERENCES 10

#define MAX_SUB_COLLIDERS 3

#define MAX_STORAGE_SIZE 30

typedef struct PositionComponent
{
i32 entity_id;
i32 component_id;
v2 position;
} PositionComponent;

typedef struct SpriteComponent
{
i32 entity_id;
i32 component_id;
SpriteData sprite_data;
b8 is_flipped;
b8 is_background_sprite;
} SpriteComponent;

typedef struct SubSpriteComponent
{
i32 entity_id;
i32 component_id;
SpriteData sub_sprites[MAX_SUB_SPRITES];
i32 sub_sprite_count;
b8 is_flipped;
} SubSpriteComponent;

typedef struct AnimationComponent
{
i32 entity_id;
i32 component_id;
AnimationFlags flags;
i32 current_frame;
f32 interval_mult;
f32 frame_start_time;
} AnimationComponent;

typedef struct ColliderComponent
{
i32 entity_id;
i32 component_id;
Shape shape;
ColliderFlags flags;
} ColliderComponent;

typedef struct VelocityComponent
{
i32 entity_id;
i32 component_id;
v2 velocity;
v2 ideal_velocity;
v2 acceleration;
v2 acceleration_mult;
f32 previous_friction;
b8 collide_against;
} VelocityComponent;

typedef struct PhysicsComponent
{
i32 entity_id;
i32 component_id;
f32 friction_mult;
f32 bounce_mult;
} PhysicsComponent;

typedef struct MovementComponent
{
i32 entity_id;
i32 component_id;
f32 axis_x;
f32 move_speed;
f32 move_speed_mult;
} MovementComponent;

typedef struct ArcEntityComponent
{
i32 entity_id;
i32 component_id;
ArcEntityType entity_type;
char *current_general_state;
AnimationStateType current_animation_state;
} ArcEntityComponent;

typedef struct ItemComponent
{
i32 entity_id;
i32 component_id;
ItemType item_type;
i32 stack_size;
} ItemComponent;

typedef struct TriggerComponent
{
i32 entity_id;
i32 component_id;
TriggerCallback enter_trigger_callback;
TriggerCallback exit_trigger_callback;
OverlappedColliderInfo previous_overlaps[MAX_OVERLAPPING_COLLIDERS];
i32 previous_overlaps_count;
b8 trigger_against;
} TriggerComponent;

typedef struct StorageComponent
{
i32 entity_id;
i32 component_id;
i32 storage_size;
ItemComponent *items[MAX_STORAGE_SIZE];
} StorageComponent;

typedef struct ParallaxComponent
{
i32 entity_id;
i32 component_id;
v2 parallax_amount;
v2 desired_position;
} ParallaxComponent;

typedef enum ComponentType
{
COMPONENT_INVALID,
COMPONENT_position,
COMPONENT_sprite,
COMPONENT_sub_sprite,
COMPONENT_animation,
COMPONENT_collider,
COMPONENT_velocity,
COMPONENT_physics,
COMPONENT_movement,
COMPONENT_arc_entity,
COMPONENT_item,
COMPONENT_trigger,
COMPONENT_storage,
COMPONENT_parallax,
COMPONENT_MAX,
} ComponentType;

typedef struct ComponentSet
{
PositionComponent position_components[MAX_ENTITIES];
i32 position_component_count;
i32 position_free_component_id;
SpriteComponent sprite_components[MAX_ENTITIES];
i32 sprite_component_count;
i32 sprite_free_component_id;
SubSpriteComponent sub_sprite_components[MAX_ENTITIES];
i32 sub_sprite_component_count;
i32 sub_sprite_free_component_id;
AnimationComponent animation_components[MAX_ENTITIES];
i32 animation_component_count;
i32 animation_free_component_id;
ColliderComponent collider_components[MAX_ENTITIES];
i32 collider_component_count;
i32 collider_free_component_id;
VelocityComponent velocity_components[MAX_ENTITIES];
i32 velocity_component_count;
i32 velocity_free_component_id;
PhysicsComponent physics_components[MAX_ENTITIES];
i32 physics_component_count;
i32 physics_free_component_id;
MovementComponent movement_components[MAX_ENTITIES];
i32 movement_component_count;
i32 movement_free_component_id;
ArcEntityComponent arc_entity_components[MAX_ENTITIES];
i32 arc_entity_component_count;
i32 arc_entity_free_component_id;
ItemComponent item_components[MAX_ENTITIES];
i32 item_component_count;
i32 item_free_component_id;
TriggerComponent trigger_components[MAX_ENTITIES];
i32 trigger_component_count;
i32 trigger_free_component_id;
StorageComponent storage_components[MAX_ENTITIES];
i32 storage_component_count;
i32 storage_free_component_id;
ParallaxComponent parallax_components[MAX_ENTITIES];
i32 parallax_component_count;
i32 parallax_free_component_id;
} ComponentSet;

