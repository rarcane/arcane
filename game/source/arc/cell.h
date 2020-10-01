internal void ProcessCell(Cell *cell);
internal void ProcessAirCell(Cell *cell);
internal void ProcessLiquidCell(Cell *cell);
internal void ProcessSolidCell(Cell *cell);
// NOTE(randy): Adds the desired chunk to texture update queue. Does nothing if the chunk is already queued.
internal void QueueChunkForTextureUpdate(Chunk *chunk);

// NOTE(randy): Converts a cell's position into a local index within the chunk array
internal i32 CellPositionToIndex(i32 pos);
// NOTE(randy): Returns a cell at the provided world position.
// Returns NULL if the position is in an unloaded chunk.
internal Cell *GetCellAtPosition(i32 x, i32 y);

// NOTE(randy): Returns true if the provided cell is just air at room temperature (implicitly empty)
internal b8 IsCellEmpty(Cell *cell);

// NOTE(randy): Swaps the data between two given cells.
internal void SwapCells(Cell *cell_1, Cell *cell_2);

// NOTE(randy): Marks cell to be dynamic next frame.
// If it's already been queued or if the cell is already dynamic, nothing will happen.
internal void QueueCellForDynamism(Cell *cell);
// NOTE(randy): Adds the desired cell to the dynamic array for processing and provides it with an indexed ID.
// The dynamic ID indexes directly into 'dynamic_cells' at (ID - 1)
internal void MakeCellDynamic(Cell *cell);

// NOTE(randy): Removes a given cell from the dynamic array.
internal void RemoveCellDynamism(Cell *cell);

// NOTE(randy): Deletes the desired cell and safely removes it from the dynamic array if it's dynamic.
// Cell must not already be empty.
internal void DeleteCell(Cell *cell);

// NOTE(randy): Determines whether or not the desired cell should be hardened based off of a few factors.
// It can only harden if it's a Solid cell, all other types will return 0 by default.
// If it is a solid cell, it will harden based on it's static crust_depth property according to the cells around it.
internal b8 ShouldCellHarden(Cell *cell);