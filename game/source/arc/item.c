internal Entity *NewGroundItemEntityAtPlayer(Item item_data)
{
	f32 flipped_multiplier = GetCharacterEntity()->is_flipped ? -1.0f : 1.0f;
	
	Entity *entity = NewEntity();
	ItemEntityPresetCallback(entity);
	
	entity->position = V2AddV2(GetCharacterEntity()->position,
							   v2(10.0f * flipped_multiplier, -20.0f));
	entity->item = item_data;
	entity->physics.velocity.x = 50.0f * flipped_multiplier;
	
	UpdateEntitySprite(entity);
	
	return entity;
}

internal i32 RemoveItemFromContainer(Item item, Item *container, i32 container_size)
{
	Assert(item.type);
	
	if (!item.stack_size)
		return 0;
	
	i32 removed_amount = 0;
	for (i32 i = 0; i < container_size; i++)
	{
		Item *container_item = &container[i];
		if (container_item->type == item.type)
		{
			if (container_item->stack_size == item.stack_size)
			{
				container_item->type = 0;
				container_item->stack_size = 0;
				return item.stack_size;
			}
			else if (container_item->stack_size > item.stack_size)
			{
				container_item->stack_size -= item.stack_size;
				return item.stack_size;
			}
			else
			{
				item.stack_size -= container_item->stack_size;
				removed_amount += container_item->stack_size;
				container_item->type = 0;
				container_item->stack_size = 0;
			}
		}
	}
	
	return removed_amount;
}