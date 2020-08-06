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

// NOTE(randy): Generates a new world, returns 0 if the world already exists.
internal b8 CreateWorld(char *world_name);
// NOTE(randy): Saves the currently loaded world's basic level data.
internal void SaveLevelData();
// NOTE(randy): Saves the currently loaded world in its entirety
internal void SaveWorld();
// NOTE(randy): Loads the given world from disk.
// Returns 0 if that world doesn't exist.
internal b8 LoadWorld(char *world_name);
// NOTE(randy): Unloads the current world and returns to main menu.
internal void UnloadWorld();
// NOTE(randy): Save all of the queued up chunks to disk with a snapshot of data
int SaveQueuedChunks(void *job_data);
// NOTE(randy): Loads a queue of skele chunks either from disk or from generation
int LoadQueuedChunks(void *job_data);
// NOTE(randy): Queues the specified chunk for a save by taking a snapshot of data
internal b8 QueueChunkForSave(Chunk *chunk);
// NOTE(randy): Puts all positional entities into their appropriate chunks.
internal void FillChunkEntities();
// NOTE(randy): Loads in chunks that should be visible. Unloads chunks that are no longer active.
internal void UpdateChunks();

// NOTE(randy): Returns chunk at the specified chunk index.
// If there isn't a chunk loaded at that index, it returns null.
internal Chunk *GetChunkAtIndex(i32 x, i32 y);
// NOTE(randy): Converts the specified position (x OR y worldspace coord) into a chunk index
inline internal i32 WorldSpaceToChunkIndex(f32 world_space_coord)
{
	f32 div = world_space_coord / (f32)CHUNK_SIZE;
	i32 index = (i32)floorf(div);
	return index;
}
// NOTE(randy): Gets the desired terrain height at a specific x position via perlin noise
//internal f32 GetTerrainHeight(f32 x_pos);
// NOTE(randy): Returns NINE surrounding chunks relative to a world-pos.
// If a surrounding chunk is not loaded, it'll be NULL
internal void GetSurroundingChunks(Chunk **chunks, v2 position);

// NOTE(randy): Deletes the specified chunk and all of its entities
internal void DeleteChunk(Chunk *chunk);
// NOTE(randy): Loads a chunk in from disk at the specified index.
// Returns 0 if the chunk file cannot be found.
//internal Chunk *LoadChunkFromDisk(char *path, i32 x_index, i32 y_index);