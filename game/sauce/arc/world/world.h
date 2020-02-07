internal void TempInitGameWorld();
internal void DrawWorld();
internal void PostUpdateWorldAnimations();

static f32 GenerateNoise(f32 x_pos, f32 y_pos, f32 frequency);

internal f32 GetCurrentWorldTime()
{
	return core->world_data->elapsed_world_time;
}