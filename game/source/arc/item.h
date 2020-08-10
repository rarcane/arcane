/* Creates a new ground item at a specified location */
internal Entity *NewGroundItemEntity(v2 position, Item item_data);
/* Removes a specific item and amount from a given array of items, returns how much items actually end up getting removed */
internal i32 RemoveItemFromContainer(Item item, Item *container, i32 container_size);