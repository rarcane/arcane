typedef struct Entity Entity;
typedef struct OverlappedColliderInfo
{
	Entity *overlapped_entity;
	// direction or some shit idk?
} OverlappedColliderInfo;

typedef void (*TriggerCallback)(OverlappedColliderInfo);

/* typedef struct Shape
{
	v2 vertices[10];
	i32 vertex_count;
} Shape; */

internal v2 GetMousePositionInWorldSpace();
/* internal b8 IsMouseOverlappingWorldShape(Shape shape, v2 shape_world_pos); */

typedef struct DebugLine DebugLine;
internal void PushDebugLine(v2 p1, v2 p2, v3 colour);
internal void PushDebugLineForDuration(v2 p1, v2 p2, v3 colour, f32 lifetime);
/* internal void PushDebugShape(Shape shape, v2 position, v3 colour);
internal void PushDebugShapeForDuration(Shape shape, v2 position, v3 colour, f32 lifetime); */
internal void DrawDebugLines();