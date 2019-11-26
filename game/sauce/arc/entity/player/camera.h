#define CAMERA_OFFSET_Y 45.0f

static inline v2 GetZeroWorldPosition()
{
	return v2(core->render_w / 2.0f / core->camera_zoom, core->render_h / 2.0f / core->camera_zoom);
}