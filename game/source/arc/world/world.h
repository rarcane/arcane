typedef struct CellSave
{
	CellMaterialType type;
	DynamicCellProperties dynamic_properties;
	b8 is_dynamic;
} CellSave;

internal void InitialiseWorldData();

internal void UpdateWorld();
internal void DrawWorld();
internal void UpdateParallax();

// NOTE(randy): Loads in chunks that should be visible.
// Unloads chunks that are no longer active.
internal void UpdateChunks();

// NOTE(randy): Converts the specified position (x OR y worldspace coord) into a chunk index
inline internal i32 WorldspaceToChunkIndex(f32 world_space_coord)
{
	f32 div = world_space_coord / (f32)CHUNK_SIZE;
	i32 index = (i32)floorf(div);
	return index;
}

// NOTE(randy): Returns chunk at the specified chunk index.
// If there isn't a chunk loaded at that index, it returns null.
internal Chunk *GetChunkAtIndex(i32 x, i32 y);

// NOTE(randy): Returns NINE surrounding chunks relative to a world-pos.
// If a surrounding chunk is not loaded, it'll be NULL
internal void GetSurroundingChunks(Chunk **chunks, v2 position);

// NOTE(randy): Creates a brand new chunk at the specified indices.
internal Chunk *CreateNewChunk(i32 x_index, i32 y_index);
// NOTE(randy): Saves the specified chunk's data to disk
internal void SaveChunkToDisk(char *path, Chunk *chunk);
// NOTE(randy): Loads a chunk in from disk at the specified index.
// Returns 0 if the chunk file cannot be found.
internal Chunk *LoadChunkFromDisk(char *path, i32 x_index, i32 y_index);
// NOTE(randy): Unloads the specified chunk to disk.
internal void UnloadChunk(Chunk *chunk);