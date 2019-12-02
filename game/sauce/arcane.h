internal void TempInitGameWorld();
internal void DrawWorld();

typedef struct DebugLine DebugLine;
typedef struct Shape Shape;
internal void PushDebugLine(v2 p1, v2 p2, v3 colour, f32 lifetime);
internal void PushDebugShape(Shape shape, v2 position, v3 colour, f32 lifetime);
internal void DrawDebugLines();