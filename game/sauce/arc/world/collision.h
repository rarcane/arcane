#define MAX_SIMULTANEOUS_COLLISIONS 3
#define MAX_COLLIDER_VERTICES 4

#define TERMINAL_VELOCITY 1000
#define WORLD_GRAVITY 500
#define WORLD_FRICTION 400

typedef struct CollisionInfo CollisionInfo;

internal void GetRectVerticies(v2 vertices[], v2 size, v2 offset);
internal Shape GetRectangleShape(v2 size, v2 offset);
internal Shape GetSlopeShape(v2 size);
internal CollisionInfo DoEntitiesCollide(Entity *instigator, Entity *supposed, v2 extrapolated_instigator_pos);
internal void RenderColliders(ColliderComponent collider_components[], i32 entity_count);

struct CollisionInfo
{
	b8 is_valid;
	Entity *instigator;
	Entity *victim;
	v2 instigatorMTV;
	v2 collision_axis;
	// orientation stuff, etc..
};