/* Creates a new ground item at a specified location */
internal Entity *NewGroundItemEntity(v2 position, Item item_data);
/* Removes a specific item with stack from a given array of items */
internal void RemoveItemFromContianer(Item item, Item *container, i32 container_size);