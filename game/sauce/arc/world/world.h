internal void TempInitGameWorld();
internal void DrawWorld();
internal void PostUpdateWorldAnimations();

internal f32 GetCurrentWorldTime()
{
	return core->world_data->elapsed_world_time;
}