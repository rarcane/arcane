#define MAX_SIMULTANEOUS_COLLISIONS 3
#define MAX_COLLIDER_VERTICES 4

#define TERMINAL_VELOCITY 1000
#define WORLD_GRAVITY 9800
#define WORLD_FRICTION 400

typedef struct CollisionInfo CollisionInfo;

internal void RenderColliders();

struct CollisionInfo
{
	b8 is_valid;
	Entity *instigator;
	Entity *victim;
	c2Manifold manifold;
};