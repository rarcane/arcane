#define MAX_SIMULTANEOUS_COLLISIONS 3
#define MAX_COLLIDER_VERTICES 4

#define TERMINAL_VELOCITY 1000
#define WORLD_GRAVITY 400
#define WORLD_FRICTION 400

typedef struct CollisionInfo
{
	b8 is_valid;
	Entity *instigator;
	Entity *victim;
	c2Manifold manifold;
} CollisionInfo;

typedef struct CollisionPair
{
	Entity *a;
	Entity *b;
} CollisionPair;

internal void UpdatePhysics();
internal void RenderColliders();
internal void GenerateCollisionPairs(CollisionPair pairs[], i32 *count);
// NOTE(randy): Get a collision manifold between two different shapes. Each shape must already be in their relative space
internal void GenerateCollisionManifold(c2Shape a_shape, c2ShapeType a_shape_type,
										c2Shape b_shape, c2ShapeType b_shape_type,
										c2Manifold *manifold);
// NOTE(randy): Applies the provided v2 to the shape
internal void AddPositionOffsetToShape(c2Shape *shape, c2ShapeType shape_type, v2 position);