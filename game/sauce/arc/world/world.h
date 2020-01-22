internal void TempInitGameWorld();
internal void DrawWorld();
internal void PostUpdateWorldAnimations();

internal i32 GetChunksInView(ChunkData *chunks[512]);
internal ChunkData *GetChunkAtPosition(v2 position);

internal f32 GetCurrentWorldTime()
{
	return core->world_data->elapsed_world_time;
}