internal void inventory_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
		icon_data->is_hovered = 1;
	else
		icon_data->is_hovered = 0;
	
	if (icon_data->is_hovered)
	{
		if (platform->key_pressed[KEY_q])
		{
			if (icon_data->item)
			{
				NewGroundItemEntityAtPlayer(*icon_data->item);
				MemorySet(icon_data->item, 0, sizeof(Item));
			}
		}
		else if (platform->left_mouse_pressed)
		{
			platform->left_mouse_pressed = 0;
			
			if (character_data->grabbed_item.type)
			{
				if (icon_data->item->type)
				{
					if (character_data->grabbed_item.type == icon_data->item->type)
					{
						// NOTE(randy): Try combine the stacks
						if (character_data->grabbed_item.stack_size + icon_data->item->stack_size <=
							global_item_type_data[icon_data->item->type].max_stack_size)
						{
							// NOTE(randy): Combine stack
							icon_data->item->stack_size += character_data->grabbed_item.stack_size;
							
							// NOTE(randy): Delete held item
							MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
							character_data->grabbed_item_origin_slot = 0;
						}
						else
						{
							// NOTE(randy): Combine stack to max, but leave remainder.
							character_data->grabbed_item.stack_size =
								character_data->grabbed_item.stack_size +
								icon_data->item->stack_size -
								global_item_type_data[icon_data->item->type].max_stack_size;
							icon_data->item->stack_size = global_item_type_data[icon_data->item->type].max_stack_size;
						}
					}
					else
					{
						// NOTE(randy): Swap items
						Item temp = *icon_data->item;
						*icon_data->item = character_data->grabbed_item;
						character_data->grabbed_item = temp;
					}
				}
				else
				{
					// NOTE(randy): Put grabbed item in slot
					*icon_data->item = character_data->grabbed_item;
					MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
				}
			}
			else
			{
				if (icon_data->item->type)
				{
					// NOTE(randy): Pick up item.
					character_data->grabbed_item = *icon_data->item;
					character_data->grabbed_item_offset = mouse;
					MemorySet(icon_data->item, 0, sizeof(Item));
				}
			}
		}
		else if (platform->right_mouse_pressed)
		{
			platform->right_mouse_pressed = 0;
			
			if (icon_data->item->type && !character_data->grabbed_item.type)
			{
				character_data->grabbed_item = *icon_data->item;
				character_data->grabbed_item.stack_size /= 2;
				character_data->grabbed_item_offset = mouse;
				
				if (IsOdd(icon_data->item->stack_size))
				{
					icon_data->item->stack_size /= 2;
					icon_data->item->stack_size += 1;
				}
				else
				{
					icon_data->item->stack_size /= 2;
				}
			}
		}
		
		/*
				else if (!core->is_mid_right_click && platform->left_mouse_pressed)
				{
					Assert(character->grabbed_item.type == ITEM_TYPE_none); // NOTE(randy): Shouldn't be an existing grabbed item if it's a new press.
					
					platform->left_mouse_pressed = 0;
					
					if (icon_data->item->type)
					{
						character->grabbed_item = *(icon_data->item);
						character->grabbed_item_origin_slot = icon_data->item;
						MemorySet(icon_data->item, 0, sizeof(Item));
						
						character->grabbed_item_offset = mouse;
					}
				}
				else if (!core->is_mid_right_click && core->left_mouse_released)
				{
					core->left_mouse_released = 0;
					
					if (character->grabbed_item.type)
					{
						if (icon_data->item->type)
						{
							if (character->grabbed_item.type == icon_data->item->type)
							{
								if (character->grabbed_item.stack_size + icon_data->item->stack_size <=
									item_type_data[icon_data->item->type].max_stack_size)
								{
									// NOTE(randy): Combine stack
									icon_data->item->stack_size += character->grabbed_item.stack_size;
									
									// NOTE(randy): Delete held item
									MemorySet(&character->grabbed_item, 0, sizeof(Item));
									character->grabbed_item_origin_slot = 0;
								}
								else
								{
									// NOTE(randy): Combine stack to max, but leave remainder.
									character->grabbed_item.stack_size =
										character->grabbed_item.stack_size +
										icon_data->item->stack_size -
										item_type_data[icon_data->item->type].max_stack_size;
									
									*character->grabbed_item_origin_slot = character->grabbed_item;
									MemorySet(&character->grabbed_item, 0, sizeof(Item));
									character->grabbed_item_origin_slot = 0;
									
									// NOTE(randy): Max the stack size of the released item.
									icon_data->item->stack_size = item_type_data[icon_data->item->type].max_stack_size;
								}
							}
							else
							{
								// NOTE(randy): Swap the two items.
								*character->grabbed_item_origin_slot = *icon_data->item;
								*icon_data->item = character->grabbed_item;
								
								MemorySet(&character->grabbed_item, 0, sizeof(Item));
								character->grabbed_item_origin_slot = 0;
							}
						}
						else
						{
							// NOTE(randy): Empty slot, just add the item
							*icon_data->item = character->grabbed_item;
							MemorySet(&character->grabbed_item, 0, sizeof(Item));
							character->grabbed_item_origin_slot = 0;
						}
					}
				}
				else if (!core->is_mid_left_click && platform->right_mouse_pressed)
				{
					Assert(character->grabbed_item.type == ITEM_TYPE_none); // NOTE(randy): Shouldn't be an existing grabbed item if it's a new press.
					
					platform->right_mouse_pressed = 0;
					
					if (icon_data->item->type && icon_data->item->stack_size > 1)
					{
						character->grabbed_item = *icon_data->item;
						character->grabbed_item->stack_size /= 2;
						character->grabbed_item_offset = mouse;
						character->grabbed_item_origin_slot = icon_data->item;
						
						if (IsOdd(icon_data->item->stack_size))
						{
							icon_data->item->stack_size /= 2;
							icon_data->item->stack_size += 1;
						}
						else
						{
							icon_data->item->stack_size /= 2;
						}
					}
				}
				else if (!core->is_mid_left_click && core->right_mouse_released)
				{
					core->right_mouse_released = 0;
					
								if (core->grabbed_inv_item_comp)
								{
									if (icon_data->item_comp)
									{
										ConvertToGroundItem(core->grabbed_inv_item_comp);
										core->grabbed_inv_item_comp = 0;
									}
									else
									{
										AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
										core->grabbed_inv_item_comp = 0;
									}
								}
				}
		 */
	}
}

internal void inventory_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	
	Ts2dPushRect(v4(1.0f, 1.0f, 1.0f, 0.8f), rect);
	
	if (icon_data->item->type)
	{
		f32 padding = 15;
		StaticSpriteData *sprite = &global_static_sprite_data[global_item_type_data[icon_data->item->type].icon_sprite];
		Ts2dPushTexture(sprite->texture_atlas, sprite->source, v4(rect.x + padding / 2, rect.y + padding / 2, rect.z - padding, rect.w - padding));
		
		if (icon_data->item->stack_size > 1)
		{
			char txt[100];
			sprintf(txt, "%i", icon_data->item->stack_size);
			Ts2dPushText(Ts2dGetDefaultFont(),
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1.0f, 1.0f, 1.0f, 1.0f),
						 v2(rect.x + rect.z / 2,
							rect.y + rect.w - 4),
						 0.3f,
						 txt);
		}
	}
	
	if (icon_data->is_hovered)
	{
		Ts2dPushFilledRect(v4(0.0f, 0.0f, 0.0f, 0.2f), rect);
	}
}

internal void hotbar_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
	{
		icon_data->is_hovered = 1;
	}
	else
	{
		icon_data->is_hovered = 0;
	}
	
	if (icon_data->is_hovered)
	{
		if (platform->key_pressed[KEY_q])
		{
			if (icon_data->item)
			{
				NewGroundItemEntityAtPlayer(*icon_data->item);
				MemorySet(icon_data->item, 0, sizeof(Item));
			}
		}
		else if (platform->left_mouse_pressed)
		{
			platform->left_mouse_pressed = 0;
			
			if (character_data->grabbed_item.type)
			{
				if (icon_data->item->type)
				{
					if (character_data->grabbed_item.type == icon_data->item->type)
					{
						// NOTE(randy): Try combine the stacks
						if (character_data->grabbed_item.stack_size + icon_data->item->stack_size <=
							global_item_type_data[icon_data->item->type].max_stack_size)
						{
							// NOTE(randy): Combine stack
							icon_data->item->stack_size += character_data->grabbed_item.stack_size;
							
							// NOTE(randy): Delete held item
							MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
							character_data->grabbed_item_origin_slot = 0;
						}
						else
						{
							// NOTE(randy): Combine stack to max, but leave remainder.
							character_data->grabbed_item.stack_size =
								character_data->grabbed_item.stack_size +
								icon_data->item->stack_size -
								global_item_type_data[icon_data->item->type].max_stack_size;
							icon_data->item->stack_size = global_item_type_data[icon_data->item->type].max_stack_size;
						}
					}
					else
					{
						// NOTE(randy): Swap items
						Item temp = *icon_data->item;
						*icon_data->item = character_data->grabbed_item;
						character_data->grabbed_item = temp;
					}
				}
				else
				{
					// NOTE(randy): Put grabbed item in slot
					*icon_data->item = character_data->grabbed_item;
					MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
				}
			}
			else
			{
				if (icon_data->item->type)
				{
					// NOTE(randy): Pick up item.
					character_data->grabbed_item = *icon_data->item;
					character_data->grabbed_item_offset = mouse;
					MemorySet(icon_data->item, 0, sizeof(Item));
				}
			}
		}
		
		/*
				if (platform->key_pressed[KEY_q])
				{
					if (icon_data->item_comp)
					{
						if (core->held_item)
						{
							ItemComponent *held_item_comp = GetItemComponentFromEntityID(core->held_item->entity_id);
							if (held_item_comp == icon_data->item_comp)
								RemoveHeldItem();
						}
						
						ConvertToGroundItem(icon_data->item_comp);
						icon_data->storage_comp->items[icon_data->slot] = 0;
					}
				}
				else if (platform->left_mouse_pressed)
				{
					Assert(character_data->grabbed_item.type == ITEM_TYPE_none);
					
					platform->left_mouse_pressed = 0;
					
					if (icon_data->item_comp)
					{
						core->grabbed_inv_item_comp = icon_data->item_comp;
						core->grabbed_inv_item_origin_slot = icon_data->slot;
						core->grabbed_inv_item_origin_storage_comp = icon_data->storage_comp;
						icon_data->storage_comp->items[icon_data->slot] = 0;
						
						core->grabbed_inventory_item_offset = mouse;
						
						if (icon_data->slot + 1 == core->active_hotbar_slot)
							RemoveHeldItem();
					}
				}
				else if (core->left_mouse_released)
				{
					core->left_mouse_released = 0;
					
					if (core->grabbed_inv_item_comp)
					{
						if (item_type_data[core->grabbed_inv_item_comp->item_type].flags & ITEM_FLAGS_HOTBARABLE)
						{
							if (icon_data->storage_comp->items[icon_data->slot])
							{
								AddItemToStorage(icon_data->item_comp, core->grabbed_inv_item_origin_storage_comp, core->grabbed_inv_item_origin_slot);
								icon_data->storage_comp->items[icon_data->slot] = 0;
								AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
								core->grabbed_inv_item_comp = 0;
							}
							else
							{
								AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
								core->grabbed_inv_item_comp = 0;
							}
						}
						else
						{
							AddItemToStorage(core->grabbed_inv_item_comp, core->grabbed_inv_item_origin_storage_comp, core->grabbed_inv_item_origin_slot);
							core->grabbed_inv_item_comp = 0;
						}
					}
				}
		 */
	}
}

internal void hotbar_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	Ts2dPushRect(v4(1.0f, 1.0f, 1.0f, 0.8f), rect);
	
	if (icon_data->item->type)
	{
		f32 padding = 15;
		StaticSpriteData *sprite = &global_static_sprite_data[global_item_type_data[icon_data->item->type].icon_sprite];
		Ts2dPushTexture(sprite->texture_atlas, sprite->source, v4(rect.x + padding / 2, rect.y + padding / 2, rect.z - padding, rect.w - padding));
		
		// Assert(icon_data->item->stack_size == 1); // NOTE(randy): Don't yet support hotbar items with stacks. Is this intentional?
	}
	
	char txt[100];
	sprintf(txt, "%i", icon_data->slot + 1);
	Ts2dPushText(Ts2dGetDefaultFont(),
				 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
				 v4(1.0f, 1.0f, 1.0f, 1.0f),
				 v2(rect.x + rect.z / 2,
					rect.y - 10.0f),
				 0.35f,
				 txt);
	
	if (icon_data->is_hovered)
	{
		Ts2dPushFilledRect(v4(0.0f, 0.0f, 0.0f, 0.2f), rect);
	}
	
	if (character_data->active_hotbar_slot == icon_data->slot)
	{
		f32 padding = 5.0f;
		Ts2dPushRect(v4(1.0f, 0.0f, 0.0f, 0.8f), v4(rect.x - padding / 2.0f, rect.y - padding / 2.0f, rect.z + padding, rect.w + padding));
	}
}

internal void equipment_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
	{
		icon_data->is_hovered = 1;
	}
	else
	{
		icon_data->is_hovered = 0;
	}
	
	if (icon_data->is_hovered)
	{
		if (platform->key_pressed[KEY_q])
		{
			if (icon_data->item)
			{
				NewGroundItemEntityAtPlayer(*icon_data->item);
				MemorySet(icon_data->item, 0, sizeof(Item));
			}
		}
		else if (platform->left_mouse_pressed)
		{
			platform->left_mouse_pressed = 0;
			
			if (character_data->grabbed_item.type)
			{
				if (icon_data->item->type)
				{
					if (character_data->grabbed_item.type == icon_data->item->type)
					{
						// NOTE(randy): Try combine the stacks
						if (character_data->grabbed_item.stack_size + icon_data->item->stack_size <=
							global_item_type_data[icon_data->item->type].max_stack_size)
						{
							// NOTE(randy): Combine stack
							icon_data->item->stack_size += character_data->grabbed_item.stack_size;
							
							// NOTE(randy): Delete held item
							MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
							character_data->grabbed_item_origin_slot = 0;
						}
						else
						{
							// NOTE(randy): Combine stack to max, but leave remainder.
							character_data->grabbed_item.stack_size =
								character_data->grabbed_item.stack_size +
								icon_data->item->stack_size -
								global_item_type_data[icon_data->item->type].max_stack_size;
							icon_data->item->stack_size = global_item_type_data[icon_data->item->type].max_stack_size;
						}
					}
					else
					{
						// NOTE(randy): Swap items
						Item temp = *icon_data->item;
						*icon_data->item = character_data->grabbed_item;
						character_data->grabbed_item = temp;
					}
				}
				else
				{
					// NOTE(randy): Put grabbed item in slot
					*icon_data->item = character_data->grabbed_item;
					MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
				}
			}
			else
			{
				if (icon_data->item->type)
				{
					// NOTE(randy): Pick up item.
					character_data->grabbed_item = *icon_data->item;
					character_data->grabbed_item_offset = mouse;
					MemorySet(icon_data->item, 0, sizeof(Item));
				}
			}
		}
	}
}

internal void equipment_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	Entity *character = GetCharacterEntity();
	
	Ts2dPushRect(v4(1.0f, 1.0f, 1.0f, 0.8f), rect);
	
	if (icon_data->item->type)
	{
		f32 padding = 15;
		StaticSpriteData *sprite = &global_static_sprite_data[global_item_type_data[icon_data->item->type].icon_sprite];
		Ts2dPushTexture(sprite->texture_atlas, sprite->source, v4(rect.x + padding / 2, rect.y + padding / 2, rect.z - padding, rect.w - padding));
	}
	
	/*
		char txt[100];
		sprintf(txt, "%i", icon_data->slot + 1);
		Ts2dPushText(Ts2dGetDefaultFont(),
					 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
					 v4(1.0f, 1.0f, 1.0f, 1.0f),
					 v2(rect.x + rect.z / 2,
						rect.y - 10.0f),
					 0.35f,
					 txt);
	 */
	
	if (icon_data->is_hovered)
	{
		Ts2dPushFilledRect(v4(0.0f, 0.0f, 0.0f, 0.2f), rect);
	}
}

internal void grabbed_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
}

internal void grabbed_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	GrabbedIconCanvasData *texture_data = user_data;
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	Ts2dPushTexture(texture_data->static_sprite->texture_atlas, texture_data->static_sprite->source, rect);
	
	if (character_data->grabbed_item.stack_size > 1)
	{
		char txt[100];
		sprintf(txt, "%i", character_data->grabbed_item.stack_size);
		Ts2dPushText(Ts2dGetDefaultFont(),
					 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
					 v4(1.0f, 1.0f, 1.0f, 1.0f),
					 v2(rect.x + rect.z / 2,
						rect.y + rect.w),
					 0.35f,
					 txt);
	}
}

internal void DrawGameUI()
{
	if (platform->key_pressed[KEY_b])
	{
		core->run_data->character_state ^= CHARACTER_STATE_is_backpack_open;
	}
	
	Entity *character = GetCharacterEntity();
	CharacterData *character_data = GetCharacterData();
	
	// NOTE(randy): Draw backpack UI.
	if (!!(core->run_data->character_state & CHARACTER_STATE_is_backpack_open))
	{
		TsUIBeginInputGroup();
		{
			TsUIPushSize(v2(60, 60));
			
			// NOTE(randy): Render backpack.
			{
				i32 column_length = 3;
				
				if (character->is_flipped)
				{
					TsUIPushX(core->render_w / 2 - 50 - column_length * 60.0f);
				}
				else
				{
					TsUIPushX(core->render_w / 2 + 50);
				}
				TsUIPushY(core->render_h / 2 + 110 - (character_data->inventory_size / column_length * 30));
				
				for (int i = 0; i < character_data->inventory_size; i++)
				{
					TsUIPushX((i % column_length) * 60.0f);
					TsUIPushY((i / column_length) * 60.0f);
					
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->item = &character_data->inventory[i];
					icon_data->slot = i;
					TsUICanvas("Icon",
							   &inventory_icon_canvas_update_callback, icon_data,
							   &inventory_icon_canvas_render_callback, icon_data);
					
					/*
										icon_data->storage_comp = storage_comp;
										icon_data->item_comp = storage_comp->items[i];
										icon_data->slot = i;
					 */
					
					TsUIPopY();
					TsUIPopX();
				}
				
				TsUIPopY();
				TsUIPopX();
			}
			
			// NOTE(randy): Render hotbar.
			{
				if (character->is_flipped)
					TsUIPushX(core->render_w / 2 + 50.0f);
				else
					TsUIPushX(core->render_w / 2 - 50.0f - character_data->hotbar_size * 60);
				TsUIPushY(core->render_h / 2 + 110.0f - 30.0f);
				
				for (int i = 0; i < character_data->hotbar_size; i++)
				{
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->item = &character_data->hotbar[i];
					icon_data->slot = i;
					TsUICanvas("Icon",
							   &hotbar_icon_canvas_update_callback, icon_data,
							   &hotbar_icon_canvas_render_callback, icon_data);
					
				}
				
				TsUIPopY();
				TsUIPopX();
			}
			
			// NOTE(randy): Render equipment
			{
				TsUIPushColumn(v2(0.0f, 0.0f), v2(60, 60));
				
				if (character->is_flipped)
					TsUIPushX(core->render_w / 2 + 250.0f);
				else
					TsUIPushX(core->render_w / 2 - 250.0f);
				TsUIPushY(core->render_h / 2  - 30.0f);
				
				for (i32 i = 0; i < MAX_EQUIPMENT_SLOTS; i++)
				{
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->item = &character_data->equipment_slots[i];
					icon_data->slot = i;
					TsUICanvas("Icon",
							   &equipment_icon_canvas_update_callback, icon_data,
							   &equipment_icon_canvas_render_callback, icon_data);
				}
				
				TsUIPopY();
				TsUIPopX();
				
				TsUIPopColumn();
			}
			
			TsUIPopSize();
		}
		TsUIEndInputGroup();
		
		// NOTE(randy): If there's a held item but none of the slots have picked up on a press then throw it onto the ground.
		if (character_data->grabbed_item.type != ITEM_TYPE_none &&
			platform->left_mouse_pressed)
		{
			NewGroundItemEntityAtPlayer(character_data->grabbed_item);
			MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
		}
		else if (character_data->grabbed_item.type != ITEM_TYPE_none &&
				 platform->right_mouse_pressed)
		{
			NewGroundItemEntityAtPlayer(character_data->grabbed_item);
			MemorySet(&character_data->grabbed_item, 0, sizeof(Item));
		}
		
		// NOTE(randy): Render grabbed item.
		if (character_data->grabbed_item.type)
		{
			TsUIPushPosition(V2SubtractV2(v2(platform->mouse_x,
											 platform->mouse_y),
										  character_data->grabbed_item_offset));
			TsUIPushSize(v2(60, 60));
			
			GrabbedIconCanvasData *grabbed_icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(GrabbedIconCanvasData));
			grabbed_icon_data->static_sprite = &global_static_sprite_data[global_item_type_data[character_data->grabbed_item.type].icon_sprite];
			TsUICanvas("Texture",
					   &grabbed_icon_canvas_update_callback, 0, &grabbed_icon_canvas_render_callback, grabbed_icon_data);
			
			TsUIPopSize();
			TsUIPopPosition();
		}
		
		if (platform->key_pressed[KEY_esc])
		{
			core->run_data->character_state &= ~CHARACTER_STATE_is_backpack_open;
			
			platform->key_pressed[KEY_esc] = 0;
			platform->key_down[KEY_esc] = 0;
		}
	}
	
	if (platform->key_pressed[KEY_esc])
	{
		core->run_data->is_paused = !core->run_data->is_paused;
		if (core->run_data->is_paused)
			core->world_delta_mult = 0.0f;
		else
			core->world_delta_mult = 1.0f;
	}
	if (core->run_data->is_paused)
	{
		Ts2dPushRectangleBlur(v4(0.0f, 0.0f, core->render_width, core->render_height), 0.5f);
		
		TsUIBeginInputGroup();
		TsUIPushCenteredColumn(v2(200, 50), 3);
		{
			TsUIMenuTitle("Paused");
			TsUIDivider();
			if (TsUIMenuButton("Resume"))
			{
				core->run_data->is_paused = 0;
				core->world_delta_mult = 1.0f;
			}
			if (TsUIMenuButton("Main Menu"))
			{
				UnloadWorld();
				core->run_data->is_paused = 0;
				core->world_delta_mult = 1.0f;
			}
		}
		TsUIPopColumn();
		TsUIEndInputGroup();
	}
	
	DrawCraftingUI();
	DrawEnchantingUI();
	DrawBlueprintUI();
	ElementalSkillTreeUIDraw();
}