internal void UpdateWorld();
internal void DrawWorld();
internal void UpdateParallax();

// NOTE(randy): Loads in chunks that should be visible.
// Unloads chunks that are no longer active.
internal void UpdateChunks();

// NOTE(randy): Converts the specified position (x OR y worldspace coord) into a chunk index
internal i32 WorldspaceToChunkIndex(f32 world_space_coordinate);

// NOTE(randy): Returns chunk at the specified chunk index.
// If there isn't a chunk loaded at that index, it returns null.
internal Chunk *GetChunkAtIndex(i32 x, i32 y);

// NOTE(randy): Gets the chunks that are within the provided rectangular region.
// Chunks that haven't been loaded in yet are marked as NULL.
internal void GetChunksInRegion(Chunk **chunks, i32 *chunk_count, v4 rect);

// NOTE(randy): Returns NINE surrounding chunks relative to a world-pos.
// If a surrounding chunk is not loaded, it'll be NULL
internal void GetSurroundingChunks(Chunk **chunks, v2 position);

// NOTE(randy): Saves the specified chunk's data to disk
internal void SaveChunkToDisk(char *path, Chunk *chunk);
// NOTE(randy): Loads a chunk in from disk at the specified index.
// Before calling, must ensure chunk isn't already loaded.
internal Chunk *LoadChunkFromDisk(i32 x, i32 y);
internal void UnloadChunk(Chunk *chunk_to_unload);