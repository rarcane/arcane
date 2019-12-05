typedef void (*TriggerCallback)(OverlappedColliderInfo);

typedef struct Entity Entity;
typedef struct EntityReference
{
	Entity *entity;
	i32 reference_index;
} EntityReference;

typedef struct OverlappedColliderInfo
{
	EntityReference overlapped_entity;
	// direction or some shit idk?
} OverlappedColliderInfo;

typedef struct Shape
{
	v2 vertices[10];
	i32 vertex_count;
} Shape;

typedef struct DebugLine DebugLine;
internal void PushDebugLine(v2 p1, v2 p2, v3 colour, f32 lifetime);
internal void PushDebugShape(Shape shape, v2 position, v3 colour, f32 lifetime);
internal void DrawDebugLines();
