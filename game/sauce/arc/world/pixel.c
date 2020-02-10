internal void UpdateCells()
{
	ChunkData *chunk = GetChunkAtPosition(v2(0, 0));

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			// Probs need a quadtree for dis lmao
		}
	}
}

internal void RenderCells()
{
	ChunkData *chunk = GetChunkAtPosition(v2(0, 0));

	v2 render_pos = v2view(v2(CHUNK_SIZE / -2.0f, CHUNK_SIZE / -1.5f));
	v2 render_size = v2zoom(v2(CHUNK_SIZE, CHUNK_SIZE));
	Ts2dPushTexture(core->renderer,
					&chunk->texture,
					v4(0.0f, 0.0f, CHUNK_SIZE, CHUNK_SIZE),
					v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
}