// NOTE(tjr): Loads in chunks that should be visible.
// Unloads chunks that are no longer active.
internal void UpdateChunks();

// NOTE(tjr): Converts the specified position (x OR y worldspace coord) into a chunk index
internal i32 WorldspaceToChunkIndex(f32 world_space_coordinate);

// NOTE(tjr): Returns chunk at the specified chunk index.
// If there isn't a chunk loaded at that index, it returns null.
internal Chunk *GetChunkAtIndex(i32 x, i32 y);

// NOTE(tjr): Returns NINE surrounding chunks relative to a world-pos.
// If a surrounding chunk is not loaded, it'll be NULL
internal void GetSurroundingChunks(Chunk **chunks, v2 position);

// NOTE(tjr): Loads a chunk in from disk at the specified index.
// Before calling, must ensure chunk isn't already loaded.
internal Chunk *LoadChunkAtIndex(i32 x, i32 y);
internal void UnloadChunk(Chunk *chunk_to_unload);