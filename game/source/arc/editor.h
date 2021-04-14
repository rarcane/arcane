internal void InitMapEditor();
internal void UpdateMapEditor();
internal b8 DoesMapChunkExistOnDisk(iv2 pos);
internal Chunk *LoadMapChunk(iv2 pos);
internal void UnloadMapChunk(iv2 pos);
internal void CommitActiveChunks();