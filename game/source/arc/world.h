#define PARALLAX_FORMULA(base_amount) v2(base_amount, base_amount / 4.0f * 3.0f)

#define HILLS_1_PARALLAX PARALLAX_FORMULA(0.4f)
#define BG_TREE_1_PARALLAX PARALLAX_FORMULA(0.4f)
#define BG_SAPLING_1_PARALLAX PARALLAX_FORMULA(0.4f)
#define BG_SHRUB_1_PARALLAX PARALLAX_FORMULA(0.4f)

#define HILLS_2_PARALLAX PARALLAX_FORMULA(0.6f)
#define BG_TREE_2_PARALLAX PARALLAX_FORMULA(0.6f)
#define BG_SHRUB_2_PARALLAX PARALLAX_FORMULA(0.6f)

#define HILLS_3_PARALLAX PARALLAX_FORMULA(0.75f)
#define BG_TREE_3_PARALLAX PARALLAX_FORMULA(0.75f)
#define BG_SHRUB_3_PARALLAX PARALLAX_FORMULA(0.75f)

#define MID_MOUNTAINS_PARALLAX PARALLAX_FORMULA(0.93f)
#define FAR_MOUNTAINS_PARALLAX PARALLAX_FORMULA(0.95f)

internal void UpdateWorld();
internal void DrawWorld();
internal void UpdateParallax();

internal b8 CreateWorld(char *world_name);
internal void SaveWorld();
internal b8 LoadWorld(char *world_name);
internal void UnloadWorld();

internal void WriteInitialMapData();
internal void ReadInitialMapData();

internal Chunk *GetChunkAtPos(iv2 pos);

inline internal i32 WorldSpaceToChunkIndex(f32 world_space_coord)
{
	f32 div = world_space_coord / (f32)CHUNK_SIZE;
	i32 index = (i32)floorf(div);
	return index;
}

internal void DeleteChunk(Chunk *chunk);

internal void TickTimers();
internal Timer *NewTimer(f32 duration, TimerCompleteCallback complete_callback);