internal void ProcessCell(Cell *cell);
// NOTE(tjr): Adds the desired chunk to texture update queue. Does nothing if the chunk is already queued.
internal void QueueChunkForTextureUpdate(Chunk *chunk);

// NOTE(tjr): Returns a cell at the provided world position.
// Returns NULL if the position is in an unloaded chunk.
internal Cell *GetCellAtPosition(i32 x, i32 y);