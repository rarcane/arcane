internal void inventory_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
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
			
			if (player_data->grabbed_item.type)
			{
				if (icon_data->item->type)
				{
					if (player_data->grabbed_item.type == icon_data->item->type)
					{
						// NOTE(randy): Try combine the stacks
						if (player_data->grabbed_item.stack_size + icon_data->item->stack_size <=
							global_item_type_data[icon_data->item->type].max_stack_size)
						{
							// NOTE(randy): Combine stack
							icon_data->item->stack_size += player_data->grabbed_item.stack_size;
							
							// NOTE(randy): Delete held item
							MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
							player_data->grabbed_item_origin_slot = 0;
						}
						else
						{
							// NOTE(randy): Combine stack to max, but leave remainder.
							player_data->grabbed_item.stack_size =
								player_data->grabbed_item.stack_size +
								icon_data->item->stack_size -
								global_item_type_data[icon_data->item->type].max_stack_size;
							icon_data->item->stack_size = global_item_type_data[icon_data->item->type].max_stack_size;
						}
					}
					else
					{
						// NOTE(randy): Swap items
						Item temp = *icon_data->item;
						*icon_data->item = player_data->grabbed_item;
						player_data->grabbed_item = temp;
					}
				}
				else
				{
					// NOTE(randy): Put grabbed item in slot
					*icon_data->item = player_data->grabbed_item;
					MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
				}
			}
			else
			{
				if (icon_data->item->type)
				{
					// NOTE(randy): Pick up item.
					player_data->grabbed_item = *icon_data->item;
					player_data->grabbed_item_offset = mouse;
					MemorySet(icon_data->item, 0, sizeof(Item));
				}
			}
		}
		else if (platform->right_mouse_pressed)
		{
			platform->right_mouse_pressed = 0;
			
			if (icon_data->item->type && !player_data->grabbed_item.type)
			{
				player_data->grabbed_item = *icon_data->item;
				player_data->grabbed_item.stack_size /= 2;
				player_data->grabbed_item_offset = mouse;
				
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
					Assert(player_data->grabbed_item.type == ITEM_TYPE_none); // NOTE(randy): Shouldn't be an existing grabbed item if it's a new press.
					
					platform->left_mouse_pressed = 0;
					
					if (icon_data->item->type)
					{
						player_data->grabbed_item = *(icon_data->item);
						player_data->grabbed_item_origin_slot = icon_data->item;
						MemorySet(icon_data->item, 0, sizeof(Item));
						
						player_data->grabbed_item_offset = mouse;
					}
				}
				else if (!core->is_mid_right_click && core->left_mouse_released)
				{
					core->left_mouse_released = 0;
					
					if (player_data->grabbed_item.type)
					{
						if (icon_data->item->type)
						{
							if (player_data->grabbed_item.type == icon_data->item->type)
							{
								if (player_data->grabbed_item.stack_size + icon_data->item->stack_size <=
									item_type_data[icon_data->item->type].max_stack_size)
								{
									// NOTE(randy): Combine stack
									icon_data->item->stack_size += player_data->grabbed_item.stack_size;
									
									// NOTE(randy): Delete held item
									MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
									player_data->grabbed_item_origin_slot = 0;
								}
								else
								{
									// NOTE(randy): Combine stack to max, but leave remainder.
									player_data->grabbed_item.stack_size =
										player_data->grabbed_item.stack_size +
										icon_data->item->stack_size -
										item_type_data[icon_data->item->type].max_stack_size;
									
									*player_data->grabbed_item_origin_slot = player_data->grabbed_item;
									MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
									player_data->grabbed_item_origin_slot = 0;
									
									// NOTE(randy): Max the stack size of the released item.
									icon_data->item->stack_size = item_type_data[icon_data->item->type].max_stack_size;
								}
							}
							else
							{
								// NOTE(randy): Swap the two items.
								*player_data->grabbed_item_origin_slot = *icon_data->item;
								*icon_data->item = player_data->grabbed_item;
								
								MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
								player_data->grabbed_item_origin_slot = 0;
							}
						}
						else
						{
							// NOTE(randy): Empty slot, just add the item
							*icon_data->item = player_data->grabbed_item;
							MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
							player_data->grabbed_item_origin_slot = 0;
						}
					}
				}
				else if (!core->is_mid_left_click && platform->right_mouse_pressed)
				{
					Assert(player_data->grabbed_item.type == ITEM_TYPE_none); // NOTE(randy): Shouldn't be an existing grabbed item if it's a new press.
					
					platform->right_mouse_pressed = 0;
					
					if (icon_data->item->type && icon_data->item->stack_size > 1)
					{
						player_data->grabbed_item = *icon_data->item;
						player_data->grabbed_item->stack_size /= 2;
						player_data->grabbed_item_offset = mouse;
						player_data->grabbed_item_origin_slot = icon_data->item;
						
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
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
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
			
			if (player_data->grabbed_item.type)
			{
				if (icon_data->item->type)
				{
					if (player_data->grabbed_item.type == icon_data->item->type)
					{
						// NOTE(randy): Try combine the stacks
						if (player_data->grabbed_item.stack_size + icon_data->item->stack_size <=
							global_item_type_data[icon_data->item->type].max_stack_size)
						{
							// NOTE(randy): Combine stack
							icon_data->item->stack_size += player_data->grabbed_item.stack_size;
							
							// NOTE(randy): Delete held item
							MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
							player_data->grabbed_item_origin_slot = 0;
						}
						else
						{
							// NOTE(randy): Combine stack to max, but leave remainder.
							player_data->grabbed_item.stack_size =
								player_data->grabbed_item.stack_size +
								icon_data->item->stack_size -
								global_item_type_data[icon_data->item->type].max_stack_size;
							icon_data->item->stack_size = global_item_type_data[icon_data->item->type].max_stack_size;
						}
					}
					else
					{
						// NOTE(randy): Swap items
						Item temp = *icon_data->item;
						*icon_data->item = player_data->grabbed_item;
						player_data->grabbed_item = temp;
					}
				}
				else
				{
					// NOTE(randy): Put grabbed item in slot
					*icon_data->item = player_data->grabbed_item;
					MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
				}
			}
			else
			{
				if (icon_data->item->type)
				{
					// NOTE(randy): Pick up item.
					player_data->grabbed_item = *icon_data->item;
					player_data->grabbed_item_offset = mouse;
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
					Assert(player_data->grabbed_item.type == ITEM_TYPE_none);
					
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
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
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
	
	if (player_data->active_hotbar_slot == icon_data->slot)
	{
		f32 padding = 5.0f;
		Ts2dPushRect(v4(1.0f, 0.0f, 0.0f, 0.8f), v4(rect.x - padding / 2.0f, rect.y - padding / 2.0f, rect.z + padding, rect.w + padding));
	}
}

internal void grabbed_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
}

internal void grabbed_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	GrabbedIconCanvasData *texture_data = user_data;
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	Ts2dPushTexture(texture_data->static_sprite->texture_atlas, texture_data->static_sprite->source, rect);
	
	if (player_data->grabbed_item.stack_size > 1)
	{
		char txt[100];
		sprintf(txt, "%i", player_data->grabbed_item.stack_size);
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
	local_persist b8 is_backpack_open = 0;
	if (platform->key_pressed[KEY_b])
	{
		is_backpack_open = !is_backpack_open;
	}
	
	PlayerDataComponent *player_data = GetPlayerDataComponentFromEntityID(core->run_data->character_entity->entity_id);
	SpriteComponent *player_sprite_comp = GetSpriteComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	// NOTE(randy): Draw backpack UI.
	if (is_backpack_open)
	{
		TsUIBeginInputGroup();
		{
			TsUIPushSize(v2(60, 60));
			
			// NOTE(randy): Render backpack.
			{
				i32 column_length = 3;
				
				if (player_sprite_comp->is_flipped)
				{
					TsUIPushX(core->render_w / 2 - 50 - column_length * 60.0f);
				}
				else
				{
					TsUIPushX(core->render_w / 2 + 50);
				}
				TsUIPushY(core->render_h / 2 + 110 - (player_data->inventory_size / column_length * 30));
				
				for (int i = 0; i < player_data->inventory_size; i++)
				{
					TsUIPushX((i % column_length) * 60.0f);
					TsUIPushY((i / column_length) * 60.0f);
					
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->item = &player_data->inventory[i];
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
				if (player_sprite_comp->is_flipped)
					TsUIPushX(core->render_w / 2 + 50.0f);
				else
					TsUIPushX(core->render_w / 2 - 50.0f - player_data->hotbar_size * 60);
				TsUIPushY(core->render_h / 2 + 110.0f - 30.0f);
				
				for (int i = 0; i < player_data->hotbar_size; i++)
				{
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->item = &player_data->hotbar[i];
					icon_data->slot = i;
					TsUICanvas("Icon",
							   &hotbar_icon_canvas_update_callback, icon_data,
							   &hotbar_icon_canvas_render_callback, icon_data);
				}
				
				TsUIPopY();
				TsUIPopX();
			}
			
			TsUIPopSize();
		}
		TsUIEndInputGroup();
		
		// NOTE(randy): If there's a held item but none of the slots have picked up on a press then throw it onto the ground.
		if (player_data->grabbed_item.type != ITEM_TYPE_none &&
			platform->left_mouse_pressed)
		{
			NewGroundItemEntityAtPlayer(player_data->grabbed_item);
			MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
		}
		else if (player_data->grabbed_item.type != ITEM_TYPE_none &&
				 platform->right_mouse_pressed)
		{
			NewGroundItemEntityAtPlayer(player_data->grabbed_item);
			MemorySet(&player_data->grabbed_item, 0, sizeof(Item));
		}
		
		// NOTE(randy): Render grabbed item.
		if (player_data->grabbed_item.type)
		{
			TsUIPushPosition(V2SubtractV2(v2(platform->mouse_x,
											 platform->mouse_y),
										  player_data->grabbed_item_offset));
			TsUIPushSize(v2(60, 60));
			
			GrabbedIconCanvasData *grabbed_icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(GrabbedIconCanvasData));
			grabbed_icon_data->static_sprite = &global_static_sprite_data[global_item_type_data[player_data->grabbed_item.type].icon_sprite];
			TsUICanvas("Texture",
					   &grabbed_icon_canvas_update_callback, 0, &grabbed_icon_canvas_render_callback, grabbed_icon_data);
			
			TsUIPopSize();
			TsUIPopPosition();
		}
	}
	
#ifdef DEVELOPER_TOOLS
	switch (core->run_data->editor_state)
	{
		case EDITOR_STATE_entity:
		{
			if (core->run_data->entity_editor.selected_entity)
			{
				StaticSpriteData *x_arrow = &global_static_sprite_data[STATIC_SPRITE_x_axis_arrow_icon];
				StaticSpriteData *y_arrow = &global_static_sprite_data[STATIC_SPRITE_y_axis_arrow_icon];
				StaticSpriteData *middle = &global_static_sprite_data[STATIC_SPRITE_circle_icon];
				
				b8 entity_has_position = 0;
				v2 position = {0};
				
				if (core->run_data->entity_editor.selected_entity->component_ids[COMPONENT_parallax])
				{
					position = GetParallaxComponentFromEntityID(core->run_data->entity_editor.selected_entity->entity_id)->desired_position;
					entity_has_position = 1;
				}
				else if (core->run_data->entity_editor.selected_entity->component_ids[COMPONENT_position])
				{
					position = GetPositionComponentFromEntityID(core->run_data->entity_editor.selected_entity->entity_id)->position;
					entity_has_position = 1;
				}
				
				/* if (entity_has_position)
			{
			   Shape x_arrow_bounds = GetRectangleShape(v2(32.5f, 7.0f), v2(19.75f, 3.5f));
			   Shape y_arrow_bounds = GetRectangleShape(v2(7.0f, 32.5f), v2(0.0f, -3.5f));
			   Shape middle_bounds = GetRectangleShape(v2(7.0f, 7.0f), v2(0.0f, 3.5f));
	  
			   f32 x_arrow_tint = 1.0f;
			   f32 y_arrow_tint = 1.0f;
			   f32 middle_tint = 1.0f;
			   local_persist b8 is_holding_x_arrow = 0;
			   local_persist b8 is_holding_y_arrow = 0;
			   local_persist b8 is_holding_middle = 0;
			   local_persist v2 grab_offset = {0.0f, 0.0f};
	  
			   if (core->left_mouse_released)
			   {
				  is_holding_x_arrow = 0;
				  is_holding_y_arrow = 0;
				  is_holding_middle = 0;
				  x_arrow_tint = 1.0f;
				  y_arrow_tint = 1.0f;
				  middle_tint = 1.0f;
			   }
	  
			   if (is_holding_x_arrow)
			   {
				  x_arrow_tint = 1.5f;
	  
				  position.x = GetMousePositionInWorldSpace().x + grab_offset.x;
			   }
			   else if (is_holding_y_arrow)
			   {
				  y_arrow_tint = 1.5f;
	  
				  position.y = GetMousePositionInWorldSpace().y + grab_offset.y;
			   }
			   else if (is_holding_middle)
			   {
				  middle_tint = 1.5f;
	  
				  position = V2AddV2(GetMousePositionInWorldSpace(), grab_offset);
			   }
			   else
			   {
				  if (IsMouseOverlappingWorldShape(x_arrow_bounds, position))
				  {
					 x_arrow_tint = 1.25f;
	  
					 if (platform->left_mouse_pressed)
					 {
						is_holding_x_arrow = 1;
	  
						grab_offset = V2SubtractV2(position, GetMousePositionInWorldSpace());
					 }
				  }
				  else if (IsMouseOverlappingWorldShape(y_arrow_bounds, position))
				  {
					 y_arrow_tint = 1.25f;
	  
					 if (platform->left_mouse_pressed)
					 {
						is_holding_y_arrow = 1;
	  
						grab_offset = V2SubtractV2(position, GetMousePositionInWorldSpace());
					 }
				  }
				  else if (IsMouseOverlappingWorldShape(middle_bounds, position))
				  {
					 middle_tint = 1.25f;
	  
					 if (platform->left_mouse_pressed)
					 {
						is_holding_middle = 1;
	  
						grab_offset = V2SubtractV2(position, GetMousePositionInWorldSpace());
					 }
				  }
			   }
	  
			   if (parallax_comp)
			   {
				  parallax_comp->desired_position = position;
			   }
			   else if (position_comp)
			   {
				  position_comp->position = position;
			   }
	  
			   v2 x_arrow_pos = v2view(v2(position.x - 4.0f, position.y - 3.5f));
			   v2 x_arrow_size = v2zoom(v2(40.0f, 7.0f));
			   v2 y_arrow_pos = v2view(v2(position.x - 3.5f, position.y - 36.0f));
			   v2 y_arrow_size = v2zoom(v2(7.0f, 40.0f));
			   v2 middle_pos = v2view(v2(position.x - 3.5f, position.y - 3.5f));
			   v2 middle_size = v2zoom(v2(7.0f, 7.0f));
	  
			   Ts2dPushTintedTexture(core->renderer,
								x_arrow->texture_atlas,
								x_arrow->source,
								v4(x_arrow_pos.x, x_arrow_pos.y, x_arrow_size.x, x_arrow_size.y),
								v4(1.0f * x_arrow_tint, 1.0f * x_arrow_tint, 1.0f * x_arrow_tint, 1.0f));
	  
			   Ts2dPushTintedTexture(core->renderer,
								y_arrow->texture_atlas,
								y_arrow->source,
								v4(y_arrow_pos.x, y_arrow_pos.y, y_arrow_size.x, y_arrow_size.y),
								v4(1.0f * y_arrow_tint, 1.0f * y_arrow_tint, 1.0f * y_arrow_tint, 1.0f));
	  
			   Ts2dPushTintedTexture(core->renderer,
								middle->texture_atlas,
								middle->source,
								v4(middle_pos.x, middle_pos.y, middle_size.x, middle_size.y),
								v4(1.0f * middle_tint, 1.0f * middle_tint, 1.0f * middle_tint, 1.0f));
			} */
			}
			break;
		}
		
		case EDITOR_STATE_collision:
		{
			for (int i = 0; i < core->run_data->entity_count; i++)
			{
				Entity *seg_entity = &core->run_data->entities[i];
				if (seg_entity->entity_id && seg_entity->generalised_type == GENERALISED_ENTITY_TYPE_ground)
				{
					PhysicsBodyComponent *seg_body = GetPhysicsBodyComponentFromEntityID(seg_entity->entity_id);
					PositionComponent *seg_pos = GetPositionComponentFromEntityID(seg_entity->entity_id);
					
					StaticSpriteData *circle_sprite = &global_static_sprite_data[STATIC_SPRITE_circle_icon];
					f32 circle_size = 4.0f;
					
					v4 p1_tint = {0.9f, 0.9f, 0.9f, 1.0f};
					v2 p1 = V2AddV2(seg_pos->position, seg_body->shape.line.p1);
					v2 p2 = V2AddV2(seg_pos->position, seg_body->shape.line.p2);
					
					if (core->run_data->collision_editor.is_seg_grabbed)
					{
						if (EqualV2(core->run_data->collision_editor.grabbed_seg_pos, p1, 1.0f))
						{
							p1 = GetMousePositionInWorldSpace();
							seg_body->shape.line.p1 = V2SubtractV2(GetMousePositionInWorldSpace(), seg_pos->position);
						}
						else if (EqualV2(core->run_data->collision_editor.grabbed_seg_pos, p2, 1.0f))
						{
							p2 = GetMousePositionInWorldSpace();
							seg_body->shape.line.p2 = V2SubtractV2(GetMousePositionInWorldSpace(), seg_pos->position);
						}
					}
					
					c2Shape p1_box;
					p1_box.aabb.min = c2V(p1.x - circle_size / 2.0f, p1.y - circle_size / 2.0f);
					p1_box.aabb.max = c2V(p1.x + circle_size / 2.0f, p1.y + circle_size / 2.0f);
					if (IsMouseOverlappingShape(GetMousePositionInWorldSpace(), p1_box, C2_SHAPE_TYPE_aabb))
					{
						p1_tint = v4u(1.0f);
						
						if (platform->left_mouse_pressed)
						{
							if (platform->key_down[KEY_alt])
							{
								v2 mid_point = V2DivideF32(V2SubtractV2(p2, p1), 2.0f);
								seg_body->shape.line.p2 = V2AddV2(mid_point, seg_body->shape.line.p1);
								
								Entity *new_segment = NewEntity("Ground Seg", GENERALISED_ENTITY_TYPE_ground);
								AddPositionComponent(new_segment);
								AddPhysicsBodyComponent(new_segment);
								GetPhysicsBodyComponentFromEntityID(new_segment->entity_id)->shape_type = C2_SHAPE_TYPE_line;
								GetPhysicsBodyComponentFromEntityID(new_segment->entity_id)->mass_data = seg_body->mass_data;
								GetPhysicsBodyComponentFromEntityID(new_segment->entity_id)->material = seg_body->material;
								GetPositionComponentFromEntityID(new_segment->entity_id)->position = V2AddV2(V2AddV2(mid_point, seg_body->shape.line.p1), seg_pos->position);
								GetPhysicsBodyComponentFromEntityID(new_segment->entity_id)->shape.line.p2 = V2SubtractV2(p2, GetPositionComponentFromEntityID(new_segment->entity_id)->position);
							}
							else
							{
								core->run_data->collision_editor.grabbed_seg_pos = p1;
								core->run_data->collision_editor.is_seg_grabbed = 1;
								TsPlatformCaptureMouseButtons();
							}
						}
						else if (platform->key_pressed[KEY_delete])
						{
							DeleteEntity(seg_entity);
							for (int j = 0; j < core->run_data->entity_count; j++)
							{
								Entity *seg_entity_2 = &core->run_data->entities[j];
								if (seg_entity_2->entity_id && seg_entity_2->generalised_type == GENERALISED_ENTITY_TYPE_ground)
								{
									PhysicsBodyComponent *seg_body_2 = GetPhysicsBodyComponentFromEntityID(seg_entity_2->entity_id);
									PositionComponent *seg_pos_2 = GetPositionComponentFromEntityID(seg_entity_2->entity_id);
									
									v2 p2_2 = V2AddV2(seg_body_2->shape.line.p2, seg_pos_2->position);
									if (EqualV2(p2_2, p1, 1.0f))
									{
										seg_body_2->shape.line.p2 = V2SubtractV2(p2, seg_pos_2->position);
										break;
									}
								}
							}
							
							if (core->run_data->collision_editor.selected_ground_seg == seg_entity)
								core->run_data->collision_editor.selected_ground_seg = 0;
						}
					}
					
					v2 p1_render = v2view(V2SubtractF32(p1, circle_size / 2.0f));
					Ts2dPushTintedTexture(circle_sprite->texture_atlas, circle_sprite->source, v4(p1_render.x, p1_render.y, circle_size * core->camera_zoom, circle_size * core->camera_zoom), p1_tint);
				}
			}
			
			if (core->left_mouse_released)
			{
				core->run_data->collision_editor.grabbed_seg_pos = v2(0.0f, 0.0f);
				core->run_data->collision_editor.is_seg_grabbed = 0;
			}
			else if (platform->left_mouse_down)
			{
				core->run_data->collision_editor.grabbed_seg_pos = GetMousePositionInWorldSpace();
			}
			
			break;
		}
	}
#endif
	
	local_persist b8 is_pause_menu_open = 0;
	if (platform->key_pressed[KEY_esc])
	{
		is_pause_menu_open = !is_pause_menu_open;
		if (is_pause_menu_open)
			core->world_delta_mult = 0.0f;
		else
			core->world_delta_mult = 1.0f;
	}
	if (is_pause_menu_open)
	{
		Ts2dPushRectangleBlur(v4(0.0f, 0.0f, core->render_width, core->render_height), 0.5f);
		
		TsUIBeginInputGroup();
		TsUIPushCenteredColumn(v2(200, 50), 3);
		{
			TsUIMenuTitle("Paused");
			TsUIDivider();
			if (TsUIMenuButton("Resume"))
			{
				is_pause_menu_open = 0;
				core->world_delta_mult = 1.0f;
			}
			if (TsUIMenuButton("Main Menu"))
			{
				UnloadWorld();
				is_pause_menu_open = 0;
				core->world_delta_mult = 1.0f;
			}
		}
		TsUIPopColumn();
		TsUIEndInputGroup();
	}
}

internal void DrawEditorUI()
{
	local_persist b8 is_entity_window_open = 0;
	local_persist b8 is_performance_window_open = 0;
	local_persist b8 is_debug_window_open = 0;
	
	local_persist b8 pin_windows = 0;
	
	// NOTE(randy): Drop-down menus
	if (core->run_data->editor_state)
	{
		TsUIPushAutoRow(v2(0, 0), 30);
		{
			if (TsUIDropdown("World..."))
			{
				/* if (TsUIButton("Save"))
				{
					Assert(core->run_data->current_level[0], "No level currently loaded?");
				}

				if (TsUIButton("Reload"))
				{
					Assert(core->run_data->current_level[0], "No level currently loaded?");
				} */
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("Editor Mode..."))
			{
				if (TsUIToggler("Entity", core->run_data->editor_state == EDITOR_STATE_entity))
				{
					SwitchEditorState(EDITOR_STATE_entity);
				}
				
				if (TsUIToggler("Terrain", core->run_data->editor_state == EDITOR_STATE_terrain))
				{
					SwitchEditorState(EDITOR_STATE_terrain);
				}
				
				if (TsUIToggler("Collision", core->run_data->editor_state == EDITOR_STATE_collision))
				{
					SwitchEditorState(EDITOR_STATE_collision);
				}
				
				if (TsUIToggler("Chunk", core->run_data->editor_state == EDITOR_STATE_chunk))
				{
					SwitchEditorState(EDITOR_STATE_chunk);
				}
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("View..."))
			{
				if (TsUIToggler("Draw World", core->run_data->debug_flags & DEBUG_FLAGS_draw_world))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_world;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_world;
				
				if (TsUIToggler("Draw Colliders", core->run_data->debug_flags & DEBUG_FLAGS_draw_collision))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_collision;
				
				if (TsUIToggler("Draw Chunk Grid", core->run_data->debug_flags & DEBUG_FLAGS_draw_chunk_grid))
					core->run_data->debug_flags |= DEBUG_FLAGS_draw_chunk_grid;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_draw_chunk_grid;
				
				if (TsUIToggler("Debug Cell View", core->run_data->debug_flags & DEBUG_FLAGS_debug_cell_view))
					core->run_data->debug_flags |= DEBUG_FLAGS_debug_cell_view;
				else
					core->run_data->debug_flags &= ~DEBUG_FLAGS_debug_cell_view;
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("Windows..."))
			{
				if (TsUIToggler("Entity", is_entity_window_open))
					is_entity_window_open = 1;
				else
					is_entity_window_open = 0;
				
				if (TsUIToggler("Performance", is_performance_window_open))
					is_performance_window_open = 1;
				else
					is_performance_window_open = 0;
				
				if (TsUIToggler("Debug", is_debug_window_open))
					is_debug_window_open = 1;
				else
					is_debug_window_open = 0;
			}
			TsUIDropdownEnd();
			
			if (TsUIDropdown("Options..."))
			{
				if (TsUIToggler("Pin Windows", pin_windows))
					pin_windows = 1;
				else
					pin_windows = 0;
			}
			TsUIDropdownEnd();
		}
		TsUIPopRow();
	}
	
	// NOTE(randy): State-specific tools
	switch (core->run_data->editor_state)
	{
		case EDITOR_STATE_none:
		break;
		
		// NOTE(randy): $Entity Editor
		case EDITOR_STATE_entity:
		{
			{
				v2 window_size = {300.0f, 400.0f};
				TsUIWindowBegin("Entities", v4(0.0f, 10.0f, window_size.x, window_size.y), 0, 0);
				{
					TsUIPushColumn(v2(10, 10), v2(150, 30));
					TsUIPushWidth(270.0f);
					
					{
						char label[100];
						sprintf(label, "Entity Count: %i", core->run_data->entity_count);
						TsUILabel(label);
					}
					
					for (i32 i = 0; i < core->run_data->entity_count; i++)
					{
						char label[100];
						sprintf(label, "[%i] #%i %s", i, core->run_data->entities[i].entity_id, core->run_data->entities[i].name);
						TsUILabel(label);
					}
					
					TsUIPopWidth();
					TsUIPopColumn();
				}
				TsUIWindowEnd();
			}
			
			break;
		}
		
		// NOTE(randy): $Terrain Editor
		case EDITOR_STATE_terrain:
		{
			// NOTE(randy): Functionality list
			// Left click to use the selected brush
			// Right click press to select a single cell
			// Right click hold to select a region
			
			local_persist CellMaterialType selected_material = -1;
			local_persist i32 brush_size = 1;
			local_persist f32 pressure = 100.0f;
			
			// TODO: refactor this when the render queue is added.
			if (platform->right_mouse_pressed)
			{
				core->run_data->terrain_editor.selection_start = GetMousePositionInWorldSpace();
				core->run_data->terrain_editor.selection_start.x = floorf(core->run_data->terrain_editor.selection_start.x);
				core->run_data->terrain_editor.selection_start.y = floorf(core->run_data->terrain_editor.selection_start.y);
				core->run_data->terrain_editor.selection_end = core->run_data->terrain_editor.selection_start;
				
				//if (core->run_data->selected_cell)
				//QueueChunkForTextureUpdate(core->run_data->selected_cell->parent_chunk);
				core->run_data->terrain_editor.selected_cell = GetCellAtPosition((i32)core->run_data->terrain_editor.selection_start.x, (i32)core->run_data->terrain_editor.selection_start.y);
				//QueueChunkForTextureUpdate(core->run_data->terrain_editor.selected_cell->parent_chunk);
			}
			else if (core->right_mouse_released)
			{
				if (core->run_data->terrain_editor.selection_start.x != core->run_data->terrain_editor.selection_end.x && core->run_data->terrain_editor.selection_start.y != core->run_data->terrain_editor.selection_end.y)
				{
					//QueueChunkForTextureUpdate(core->run_data->terrain_editor.selected_cell->parent_chunk);
					core->run_data->terrain_editor.selected_cell = 0;
				}
			}
			
			if (platform->right_mouse_down)
			{
				core->run_data->terrain_editor.selection_end = GetMousePositionInWorldSpace();
				core->run_data->terrain_editor.selection_end.x = floorf(core->run_data->terrain_editor.selection_end.x);
				core->run_data->terrain_editor.selection_end.y = floorf(core->run_data->terrain_editor.selection_end.y);
				
				//if (core->run_data->terrain_editor.selected_cell)
				//QueueChunkForTextureUpdate(core->run_data->terrain_editor.selected_cell->parent_chunk);
				core->run_data->terrain_editor.selected_cell = GetCellAtPosition((i32)core->run_data->terrain_editor.selection_end.x, (i32)core->run_data->terrain_editor.selection_end.y);
				//QueueChunkForTextureUpdate(core->run_data->terrain_editor.selected_cell->parent_chunk);
			}
			
			v2 window_size = {300.0f, 400.0f};
			TsUIWindowBegin("Sculpting", v4(core->render_w - window_size.x - 10.0f, 10.0f, window_size.x, window_size.y), 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				TsUIPushWidth(270.0f);
				
				brush_size = TsUIIntSlider("Brush Size", brush_size, 1, 32);
				
				if (TsUICollapsable("Material Select"))
				{
					if (TsUIToggler("Erase", selected_material == -1))
						selected_material = -1;
					if (TsUIToggler("Air", selected_material == CELL_MATERIAL_TYPE_air))
						selected_material = CELL_MATERIAL_TYPE_air;
					if (TsUIToggler("Water", selected_material == CELL_MATERIAL_TYPE_water))
						selected_material = CELL_MATERIAL_TYPE_water;
					if (TsUIToggler("Dirt", selected_material == CELL_MATERIAL_TYPE_dirt))
						selected_material = CELL_MATERIAL_TYPE_dirt;
					/* if (TsUIToggler("Sand", selected_material == CELL_MATERIAL_TYPE_sand))
						selected_material = CELL_MATERIAL_TYPE_sand; */
					
					TsUICollapsableEnd();
				}
				
				if (selected_material == CELL_MATERIAL_TYPE_air)
				{
					pressure = TsUISlider("Pressure", pressure, 0.0f, 250.0f);
				}
				
				TsUIPopWidth();
				TsUIPopColumn();
			}
			TsUIWindowEnd();
			
			window_size = v2(300.0f, 400.0f);
			TsUIWindowBegin("Cell Info", v4(10.0f, core->render_h - window_size.y - 10.0f, window_size.x, window_size.y), 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				TsUIPushWidth(270.0f);
				
				// core->world_delta_mult = TsUISlider("Simulation Speed", core->world_delta_mult, 0.0f, 1.0f);
				
				{
					char lbl[20];
					sprintf(lbl, "Dynamic Cell Count: %i", core->run_data->dynamic_cell_count);
					TsUILabel(lbl);
				}
				
				/*
							if (TsUIButton("Delete All Dynamic"))
							{
								for (i32 i = 0; i < core->run_data->dynamic_cell_count; i++)
								{
									if (core->run_data->dynamic_cells[i])
										DeleteCell(core->run_data->dynamic_cells[i]);
								}
							}
				 */
				
				if (core->run_data->terrain_editor.selected_cell)
				{
					if (TsUICollapsable("Selected Cell"))
					{
						// TODO: Auto @Printable tagging in dd
						
						TsUILabel(GetCellMaterialTypeName(core->run_data->terrain_editor.selected_cell->material_type));
						
						/*
											{
												char lbl[20];
												sprintf(lbl, "Dynamic ID #%i", core->run_data->terrain_editor.selected_cell->dynamic_id);
												TsUILabel(lbl);
											}
						
											{
												char lbl[20];
												sprintf(lbl, "Posx %i", core->run_data->terrain_editor.selected_cell->x_position);
												TsUILabel(lbl);
											}
						
											{
												char lbl[20];
												sprintf(lbl, "Posy %i", core->run_data->terrain_editor.selected_cell->y_position);
												TsUILabel(lbl);
											}
						 */
						
						{
							char lbl[20];
							sprintf(lbl, "Air Pressure: %f", core->run_data->terrain_editor.selected_cell->dynamic_properties.air.pressure);
							TsUILabel(lbl);
						}
						
						{
							char lbl[20];
							sprintf(lbl, "Liquid Mass: %f", core->run_data->terrain_editor.selected_cell->dynamic_properties.liquid.mass);
							TsUILabel(lbl);
						}
						
						/* {
							char lbl[20];
							sprintf(lbl, "Liquid Mass adj: %f", core->run_data->terrain_editor.selected_cell->dynamic_properties.liquid.mass_adjustment);
							TsUILabel(lbl);
						} */
						
						TsUICollapsableEnd();
					}
					
					/*
									if (TsUIButton("Clear Selection"))
									{
										QueueChunkForTextureUpdate(core->run_data->terrain_editor.selected_cell->parent_chunk);
										core->run_data->terrain_editor.selected_cell = 0;
									}
					 */
				}
				else if (core->run_data->terrain_editor.selection_start.x != core->run_data->terrain_editor.selection_end.x && core->run_data->terrain_editor.selection_start.y != core->run_data->terrain_editor.selection_end.y)
				{
					{
						char start[50];
						sprintf(start, "Selection Start: %f, %f", core->run_data->terrain_editor.selection_start.x, core->run_data->terrain_editor.selection_start.y);
						
						char end[50];
						sprintf(end, "Selection End: %f, %f", core->run_data->terrain_editor.selection_end.x, core->run_data->terrain_editor.selection_end.y);
						
						TsUILabel(start);
						TsUILabel(end);
					}
					
					if (TsUIButton("Bake Selection"))
					{
						v2 selection_bounds = V2SubtractV2(core->run_data->terrain_editor.selection_end, core->run_data->terrain_editor.selection_start);
						
						for (int y = 1; y < fabsf(selection_bounds.height); y++)
						{
							for (int x = 1; x < fabsf(selection_bounds.width); x++)
							{
								Cell *cell = GetCellAtPosition((i32)core->run_data->terrain_editor.selection_start.x + x * GetSign(selection_bounds.width), (i32)core->run_data->terrain_editor.selection_start.y + y * GetSign(selection_bounds.height));
								/*
															if (cell->dynamic_id)
															{
																RemoveCellDynamism(cell);
															}
								 */
								
								if (ShouldCellHarden(cell))
								{
									cell->dynamic_properties.solid.hardness = 1.0f;
								}
								else if (global_cell_material_type_data[cell->material_type].properties_type == CELL_PROPERTIES_TYPE_solid)
								{
									cell->dynamic_properties.solid.hardness = 0.0f;
								}
							}
						}
						
						SkeletonChunk chunks[MAX_WORLD_CHUNKS];
						i32 chunk_count = 0;
						
						v2 absolute_start_pos;
						if (selection_bounds.x < 0.0f)
							absolute_start_pos.x = core->run_data->terrain_editor.selection_end.x;
						else
							absolute_start_pos.x = core->run_data->terrain_editor.selection_start.x;
						if (selection_bounds.y < 0.0f)
							absolute_start_pos.y = core->run_data->terrain_editor.selection_end.y;
						else
							absolute_start_pos.y = core->run_data->terrain_editor.selection_start.y;
						
						GetSkeletonChunksInRegion(chunks, &chunk_count, v4(absolute_start_pos.x, absolute_start_pos.y, fabsf(selection_bounds.width), fabsf(selection_bounds.height)), 0);
						for (i32 i = 0; i < chunk_count; i++)
						{
							Chunk *chunk = GetChunkAtIndex(chunks[i].x_index, chunks[i].y_index);
							if (chunk)
								QueueChunkForTextureUpdate(chunk);
							else
								LogWarning("Selected region has an unloaded chunk, is this intended?");
						}
						
						core->run_data->terrain_editor.selection_start = v2(0, 0);
						core->run_data->terrain_editor.selection_end = v2(0, 0);
					}
					
					if (TsUIButton("Clear Selection"))
					{
						core->run_data->terrain_editor.selection_start = v2(0, 0);
						core->run_data->terrain_editor.selection_end = v2(0, 0);
					}
				}
				else
				{
					TsUILabel("Make a selection with RMB");
				}
				
				if (core->world_delta_t == 0.0f && TsUIButton("Step Simulation"))
					core->run_data->debug_flags |= DEBUG_FLAGS_manual_step;
				
				TsUIPopWidth();
				TsUIPopColumn();
			}
			TsUIWindowEnd();
			
			if (!platform->mouse_position_captured && platform->left_mouse_down)
			{
				v2 mouse_pos = GetMousePositionInWorldSpace();
				
				Cell *cell = GetCellAtPosition((i32)roundf(mouse_pos.x),
											   (i32)roundf(mouse_pos.y));
				
				if (IsCellEmpty(cell))
				{
					if (selected_material != -1)
					{
						cell->material_type = selected_material;
						
						switch (global_cell_material_type_data[cell->material_type].properties_type)
						{
							case CELL_PROPERTIES_TYPE_air:
							{
								cell->dynamic_properties.air.pressure = pressure;
								break;
							}
							case CELL_PROPERTIES_TYPE_liquid:
							{
								cell->dynamic_properties.liquid.mass = MAX_LIQUID_MASS;
								break;
							}
							case CELL_PROPERTIES_TYPE_solid:
							{
								break;
							}
							default:
							Assert(0);
							break;
						}
						
						/*
											if (!cell->dynamic_id)
												MakeCellDynamic(cell);
						
											QueueChunkForTextureUpdate(cell->parent_chunk);
						 */
					}
				}
				else
				{
					if (selected_material == -1)
					{
						DeleteCell(cell);
					}
					else if (cell->material_type == CELL_MATERIAL_TYPE_air && selected_material == CELL_MATERIAL_TYPE_air)
					{
						cell->dynamic_properties.air.pressure = pressure;
					}
				}
				
				for (f32 brush_radius = 1; brush_radius <= brush_size - 1; brush_radius++)
				{
					for (f32 angle = 0; angle <= 360; angle += 0.05f)
					{
						f32 x = brush_radius * cosf(angle);
						f32 y = brush_radius * sinf(angle);
						
						cell = GetCellAtPosition((i32)roundf(mouse_pos.x) + (i32)roundf(x),
												 (i32)roundf(mouse_pos.y) + (i32)roundf(y));
						
						if (IsCellEmpty(cell))
						{
							if (selected_material != -1)
							{
								cell->material_type = selected_material;
								
								switch (global_cell_material_type_data[cell->material_type].properties_type)
								{
									case CELL_PROPERTIES_TYPE_air:
									{
										cell->dynamic_properties.air.pressure = pressure;
										break;
									}
									case CELL_PROPERTIES_TYPE_liquid:
									{
										cell->dynamic_properties.liquid.mass = MAX_LIQUID_MASS;
										break;
									}
									case CELL_PROPERTIES_TYPE_solid:
									{
										break;
									}
									default:
									Assert(0);
									break;
								}
								
								/*
															if (!cell->dynamic_id)
																MakeCellDynamic(cell);
								
															QueueChunkForTextureUpdate(cell->parent_chunk);
								 */
							}
						}
						else
						{
							if (selected_material == -1)
							{
								DeleteCell(cell);
							}
							else if (cell->material_type == CELL_MATERIAL_TYPE_air && selected_material == CELL_MATERIAL_TYPE_air)
							{
								cell->dynamic_properties.air.pressure = pressure;
							}
						}
					}
				}
			}
			
			break;
		}
		
		// NOTE(randy): $Collision Editor
		case EDITOR_STATE_collision:
		{
			v2 window_size = {300.0f, 400.0f};
			TsUIWindowBegin("Ground Segments", v4(core->render_w - window_size.x - 10.0f, 10.0f, window_size.x, window_size.y), 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				TsUIPushWidth(270.0f);
				
				// List segments
				for (int i = 0; i < core->run_data->entity_count; i++)
				{
					Entity *ground_seg = &core->run_data->entities[i];
					if (ground_seg->entity_id && ground_seg->generalised_type == GENERALISED_ENTITY_TYPE_ground)
					{
						char label[50];
						sprintf(label, "Segment #%i", ground_seg->entity_id);
						if (core->run_data->collision_editor.selected_ground_seg)
						{
							if (TsUIToggler(label, ground_seg->entity_id == core->run_data->collision_editor.selected_ground_seg->entity_id))
								core->run_data->collision_editor.selected_ground_seg = ground_seg;
							else if (core->run_data->collision_editor.selected_ground_seg->entity_id == ground_seg->entity_id)
								core->run_data->collision_editor.selected_ground_seg = 0;
						}
						else
						{
							if (TsUIToggler(label, 0))
								core->run_data->collision_editor.selected_ground_seg = ground_seg;
						}
						
						if (core->run_data->collision_editor.selected_ground_seg && core->run_data->collision_editor.selected_ground_seg->entity_id == ground_seg->entity_id)
						{
							{
								char label[50];
								sprintf(label, "Point 1: %f, %f", GetPhysicsBodyComponentFromEntityID(ground_seg->entity_id)->shape.line.p1.x, GetPhysicsBodyComponentFromEntityID(ground_seg->entity_id)->shape.line.p1.y);
								TsUILabel(label);
							}
							{
								char label[50];
								sprintf(label, "Point 2: %f, %f", GetPhysicsBodyComponentFromEntityID(ground_seg->entity_id)->shape.line.p2.x, GetPhysicsBodyComponentFromEntityID(ground_seg->entity_id)->shape.line.p2.y);
								TsUILabel(label);
							}
							
							// PrintEntityDataUI(core->run_data->collision_editor.selected_ground_seg->parent_generic_entity);
						}
					}
				}
				
				TsUIPopColumn();
				TsUIPopWidth();
			}
			TsUIWindowEnd();
		} break;
		
		// NOTE(randy): $Chunk editor
		case EDITOR_STATE_chunk :
		{
			v2 world_info_window_size = { 300.0f, 300.0f };
			v2 world_info_window_pos = {core->render_w - world_info_window_size.x - 10.0f, 10.0f};
			TsUIWindowBegin("World Info", v4(world_info_window_pos.x, world_info_window_pos.y, world_info_window_size.x, world_info_window_size.y), 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				TsUIPushWidth(270.0f);
				
				{
					char label[20];
					sprintf(label, "Active Chunks: %i", core->run_data->active_chunk_count);
					TsUILabel(label);
				}
				
				// NOTE(randy): Make a new chunk selection
				if (platform->left_mouse_pressed)
				{
					v2 click_pos = GetMousePositionInWorldSpace();
					core->run_data->chunk_editor.is_chunk_selected = 1;
					core->run_data->chunk_editor.selected_chunk.x_index = WorldSpaceToChunkIndex(click_pos.x);
					core->run_data->chunk_editor.selected_chunk.y_index = WorldSpaceToChunkIndex(click_pos.y);
				}
				
				TsUIPopColumn();
				TsUIPopWidth();
			}
			TsUIWindowEnd();
			
			v2 selected_chunk_window_size = { 300.0f, 400.0f };
			v2 selected_chunk_window_pos = {world_info_window_pos.x, world_info_window_pos.y + world_info_window_size.y + 10.0f};
			TsUIWindowBegin("Selected Chunk", v4(selected_chunk_window_pos.x, selected_chunk_window_pos.y, selected_chunk_window_size.x, selected_chunk_window_size.y), 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				TsUIPushWidth(270.0f);
				
				if (core->run_data->chunk_editor.is_chunk_selected)
				{
					Chunk *chunk = GetChunkAtIndex(core->run_data->chunk_editor.selected_chunk.x_index, core->run_data->chunk_editor.selected_chunk.y_index);
					if (chunk)
					{
						{
							char label[20];
							sprintf(label, "%i, %i", chunk->x_index, chunk->y_index);
							TsUITitle(label);
						}
						TsUIDivider();
						{
							char label[20];
							sprintf(label, "entity_count: %i", chunk->entity_count);
							TsUILabel(label);
						}
					}
					else
					{
						// NOTE(randy): The selected chunk isn't loaded in.
						{
							char label[20];
							sprintf(label, "%i, %i (unloaded)", core->run_data->chunk_editor.selected_chunk.x_index, core->run_data->chunk_editor.selected_chunk.y_index);
							TsUITitle(label);
						}
						TsUIDivider();
						
						// TODO(randy): Option to force load the chunk in?
					}
				}
				else
				{
					TsUILabel("No chunk selected.");
				}
				
				TsUIPopColumn();
				TsUIPopWidth();
			}
			TsUIWindowEnd();
		} break;
		
		default:
		Assert(0);
		break;
	}
	
	// NOTE(randy): Time dilation
	if (core->run_data->editor_state)
	{
		TsUIBeginInputGroup();
		TsUIPushColumn(v2(core->render_w / 2.0f - 125.0f, 40.0f), v2(250.0f, 30.0f));
		
		core->world_delta_mult = TsUISlider("World Time Dilation", core->world_delta_mult, 0.0f, 1.0f);
		
		TsUIPopColumn();
		TsUIEndInputGroup();
	}
	
	// NOTE(randy): Draw windows.
	if (pin_windows || core->run_data->editor_state)
	{
		if (is_entity_window_open)
		{
			// NOTE(randy): Entity info window.
			v4 entity_info_window_rect = {core->render_w - 360, 10, 350, 300};
			TsUIWindowBegin("Entity Info", entity_info_window_rect, 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(100, 30));
				
				if (!core->run_data->entity_editor.selected_entity)
				{
					TsUIPushAutoWidth();
					TsUILabel("No Entity selected.");
					TsUIPopWidth();
				}
				else
				{
					TsUIPushWidth(entity_info_window_rect.width - 50);
					{
						char *label = MakeCStringOnMemoryArena(core->frame_arena, "%s #%i",
															   core->run_data->entity_editor.selected_entity->name, core->run_data->entity_editor.selected_entity->entity_id);
						TsUITitle(label);
						
						//PrintEntityDataUI(core->run_data->entity_editor.selected_entity);
					}
					TsUIPopWidth();
					
					TsUIDivider();
					
					TsUIPushAutoWidth();
					if (!(core->run_data->entity_editor.selected_entity->flags & ENTITY_FLAGS_no_delete))
					{
						if (TsUIButton("Delete Entity"))
						{
							DeleteEntity(core->run_data->entity_editor.selected_entity);
							core->run_data->entity_editor.selected_entity = 0;
						}
					}
					TsUIPopWidth();
				}
				
				TsUIPopColumn();
			}
			TsUIWindowEnd();
			
			v4 entity_list_window_rect = {core->render_w - 360, 310, 350, 500};
			TsUIWindowBegin("Entity List", entity_list_window_rect, 0, 0);
			{
				TsUIPushColumn(v2(10, 10), v2(150, 30));
				
				local_persist b8 is_index_mode = 0;
				
				// NOTE(rjf): Sort Controls
				{
					TsUIPushWidth(90);
					TsUILabel("Sort by: ");
					TsUIPopWidth();
					
					TsUIPushAutoWidth();
					{
						TsUISameLine();
						if (TsUIToggler("Index", is_index_mode))
						{
							is_index_mode = 1;
						}
						
						TsUISameLine();
						if (TsUIToggler("Category", !is_index_mode))
						{
							is_index_mode = 0;
						}
					}
					TsUIPopWidth();
				}
				
				TsUIDivider();
				
				if (is_index_mode)
				{
					// NOTE(randy): Index view
					for (int i = 1; i < core->run_data->entity_count; i++)
					{
						TsUIPushWidth(30);
						{
							char label[100];
							sprintf(label, "%i", i);
							TsUILabel(label);
						}
						TsUIPopWidth();
						
						Entity *entity = &core->run_data->entities[i];
						if (entity->entity_id > 0)
						{
							TsUISameLine();
							TsUIPushWidth(entity_list_window_rect.width - 80);
							if (TsUIToggler(entity->name, (core->run_data->entity_editor.selected_entity ? core->run_data->entity_editor.selected_entity->entity_id == i : 0)))
							{
								core->run_data->entity_editor.selected_entity = entity;
							}
							else
							{
								if (core->run_data->entity_editor.selected_entity && core->run_data->entity_editor.selected_entity->entity_id == i)
								{
									core->run_data->entity_editor.selected_entity = 0;
								}
							}
							TsUIPopWidth();
						}
						else
						{
							TsUISameLine();
							TsUIPushSize(v2(100, 30));
							TsUILabel("- - - - -");
							TsUIPopSize();
						}
					}
				}
				else
				{
					TsUIPushWidth(entity_list_window_rect.width - 50);
					
					// NOTE(randy): Entity category (type) view
					for (int i = 0; i < GENERALISED_ENTITY_TYPE_MAX; i++)
					{
						if (TsUICollapsable(GetGeneralisedEntityTypeName(i)))
						{
							for (int j = 1; j < core->run_data->entity_count; j++) // TEMP: Need to sort these before-hand. Will eventually get too inefficient.
							{
								Entity *entity = &core->run_data->entities[j];
								if (entity->entity_id > 0 && entity->generalised_type == i)
								{
									if (TsUIToggler(entity->name, (core->run_data->entity_editor.selected_entity ? core->run_data->entity_editor.selected_entity->entity_id == j : 0)))
									{
										core->run_data->entity_editor.selected_entity = entity;
									}
									else
									{
										if (core->run_data->entity_editor.selected_entity &&
											core->run_data->entity_editor.selected_entity->entity_id == j)
										{
											core->run_data->entity_editor.selected_entity = 0;
										}
									}
								}
							}
							
							TsUICollapsableEnd();
						}
					}
					
					TsUIPopWidth();
				}
				
				TsUIPopColumn();
			}
			TsUIWindowEnd();
		}
		
		if (is_performance_window_open)
		{
			// NOTE(randy): Display performance data.
			TsUIWindowBegin("Performance", v4(10, 500, 300, 280), 0, 0);
			{
				TsUIPushColumn(v2(10, 0), v2(100, 50));
				
				f32 budget_total = 0.0f; // NOTE(randy): Not actual amount, need to calculate this more accurately. Need to create some sort of "Unnaccounted" measurement
				for (int i = 0; i < core->performance_timer_count; i++)
				{
					char label[100];
					
					f32 timer_budget_amount = (core->performance_timers[i].finish_time - core->performance_timers[i].start_time) / core->raw_delta_t * 100.0f;
					sprintf(label,
							"%s: %f",
							core->performance_timers[i].name,
							timer_budget_amount);
					
					budget_total += timer_budget_amount;
					
					TsUILabel(label);
				}
				
				char label[100];
				sprintf(label, "Frame Budget Usage: %f", budget_total);
				TsUILabel(label);
				
				TsUIPopColumn();
			}
			TsUIWindowEnd();
		}
		
		if (is_debug_window_open)
		{
			// NOTE(randy): Debug random stuff
			TsUIWindowBegin("Debug", v4(310, 500, 300, 200), 0, 0);
			{
				TsUIPushColumn(v2(10, 0), v2(200, 30));
				
				char label[100];
				sprintf(label, "Camera Pos: %f, %f", core->camera_position.x, core->camera_position.y);
				TsUILabel(label);
				
				TsUIPopColumn();
			}
			TsUIWindowEnd();
		}
	}
}

internal void SwitchEditorState(EditorState editor_state)
{
	if (core->run_data->editor_state != editor_state)
	{
		// NOTE(randy): Cache the no-state debug flags
		if (core->run_data->editor_state == EDITOR_STATE_none)
		{
			core->run_data->saved_debug_flags = core->run_data->debug_flags;
		}
		
		core->run_data->debug_flags = core->run_data->saved_debug_flags;
		
		// NOTE(randy): Set state specific debug flags
		switch (editor_state)
		{
			case EDITOR_STATE_entity :
			{
			} break;
			
			case EDITOR_STATE_terrain :
			{
				core->run_data->debug_flags |= DEBUG_FLAGS_debug_cell_view;
			} break;
			
			case EDITOR_STATE_collision :
			{
				core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
			} break;
			
			case EDITOR_STATE_chunk :
			{
				core->run_data->debug_flags |= DEBUG_FLAGS_draw_collision;
				core->run_data->debug_flags |= DEBUG_FLAGS_draw_chunk_grid; // TODO(randy): Need to separate this flag out from the collision draw
			} break;
		}
		
		
		core->run_data->disable_chunk_loaded_based_off_view = editor_state != EDITOR_STATE_none;
		
		core->run_data->editor_state = editor_state;
	}
}