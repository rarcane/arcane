#define DEFAULT_CAMERA_OFFSET_Y 45.0f
#define DEFAULT_CAMERA_ZOOM 3.3f
#define MIN_CAMERA_ZOOM 2.0f

// NOTE (tjr): Transforms the input via the camera
#define v2view(value) v2((value.x + (core->camera_position.x * -1.0f) + GetZeroWorldPosition().x) * core->camera_zoom, (value.y + (core->camera_position.y * -1.0f) + GetZeroWorldPosition().y) * core->camera_zoom)
// NOTE (tjr): Transforms the input via the camera zoom
#define v2zoom(value) v2(value.x * core->camera_zoom, value.y * core->camera_zoom)

#define v2screen(value) v2(value.x * core->render_w, value.y * core->render_h)
#define v2GUI(value) v2(value.x * core->render_h, value.y * core->render_h)

static inline v2 GetZeroWorldPosition()
{
	return v2(core->render_w / 2.0f / core->camera_zoom, core->render_h / 2.0f / core->camera_zoom);
}

internal void TransformEditorCamera();
internal void TransformInGameCamera();
internal void MoveCameraToLocation(v2 location, f32 zoom, f32 time);