// NOTE (tjr): Transforms the input via the camera
#define v2view(value) v2((value.x + core->camera_position.x) * core->camera_zoom, (value.y + core->camera_position.y) * core->camera_zoom)
// NOTE (tjr): Transforms the input via the camera zoom
#define v2zoom(value) v2(value.x * core->camera_zoom, value.y * core->camera_zoom)

internal void TempInitGameWorld();
internal void DrawWorld();

typedef struct DebugLine DebugLine;
typedef struct Shape Shape;
internal void PushDebugLine(v2 p1, v2 p2, v3 colour, f32 lifetime);
internal void PushDebugShape(Shape shape, v2 position, v3 colour, f32 lifetime);
internal void DrawDebugLines();