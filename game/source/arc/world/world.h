internal void CreateTestLevel();
internal void DrawWorld();
internal void PostUpdateWorldAnimations();

internal ChunkData *GetChunkAtPosition(v2 position);

internal f32 GetCurrentWorldTime()
{
	return core->world_data->elapsed_world_time;
}