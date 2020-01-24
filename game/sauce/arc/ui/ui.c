internal void inventory_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;

	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
		icon_data->is_hovered = 1;
	else
		icon_data->is_hovered = 0;

	if (icon_data->is_hovered)
	{
		if (platform->key_pressed[KEY_q])
		{
			if (icon_data->item_comp)
			{
				ConvertToGroundItem(icon_data->item_comp);
				icon_data->storage_comp->items[icon_data->slot] = 0;
			}
		}
		else if (!core->is_mid_right_click && platform->left_mouse_pressed)
		{
			R_DEV_ASSERT(!core->grabbed_inv_item_comp, "Shouldn't be an existing grabbed item if it's a new press.");

			platform->left_mouse_pressed = 0;

			if (icon_data->item_comp)
			{
				core->grabbed_inv_item_comp = icon_data->item_comp;
				core->grabbed_inv_item_origin_slot = icon_data->slot;
				core->grabbed_inv_item_origin_storage_comp = icon_data->storage_comp;
				icon_data->storage_comp->items[icon_data->slot] = 0;

				core->grabbed_inventory_item_offset = mouse;
			}
		}
		else if (!core->is_mid_right_click && core->left_mouse_released)
		{
			core->left_mouse_released = 0;

			if (core->grabbed_inv_item_comp)
			{
				if (icon_data->storage_comp->items[icon_data->slot])
				{
					StorageComponent *hotbar_storage_comp = core->hotbar->components[COMPONENT_storage];
					if (core->grabbed_inv_item_origin_storage_comp == hotbar_storage_comp) // If this incoming swap is from the hotbar.
					{
						if (item_data[icon_data->item_comp->item_type].flags & ITEM_FLAGS_HOTBARABLE) // Swap is allowed.
						{
							AddItemToStorage(icon_data->item_comp, core->grabbed_inv_item_origin_storage_comp, core->grabbed_inv_item_origin_slot);
							icon_data->storage_comp->items[icon_data->slot] = 0;
							AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
							core->grabbed_inv_item_comp = 0;
						}
						else // Fail swap. Leave item in hotbar.
						{
							AddItemToStorage(core->grabbed_inv_item_comp, hotbar_storage_comp, core->grabbed_inv_item_origin_slot);
							core->grabbed_inv_item_comp = 0;
						}
					}
					else
					{
						if (core->grabbed_inv_item_comp->item_type == icon_data->item_comp->item_type)
						{
							if (core->grabbed_inv_item_comp->stack_size + icon_data->item_comp->stack_size <=
								item_data[icon_data->item_comp->item_type].max_stack_size)
							{
								// Combine stack
								icon_data->item_comp->stack_size += core->grabbed_inv_item_comp->stack_size;

								// Delete held item
								Entity *grabbed_item = core->grabbed_inv_item_comp->parent_entity;
								DeleteEntity(grabbed_item);
								core->grabbed_inv_item_comp = 0;
							}
							else // Combine with leftovers.
							{
								// Calculate leftover item stack size.
								core->grabbed_inv_item_comp->stack_size = core->grabbed_inv_item_comp->stack_size +
																		  icon_data->item_comp->stack_size -
																		  item_data[icon_data->item_comp->item_type].max_stack_size;
								// Move leftover item back where it came from.
								AddItemToStorage(core->grabbed_inv_item_comp, core->grabbed_inv_item_origin_storage_comp, core->grabbed_inv_item_origin_slot);
								core->grabbed_inv_item_comp = 0;
								// Max the stack size of the released item.
								icon_data->item_comp->stack_size = item_data[icon_data->item_comp->item_type].max_stack_size;
							}
						}
						else // Swap
						{
							AddItemToStorage(icon_data->item_comp, core->grabbed_inv_item_origin_storage_comp, core->grabbed_inv_item_origin_slot);
							icon_data->storage_comp->items[icon_data->slot] = 0;
							AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
							core->grabbed_inv_item_comp = 0;
						}
					}
				}
				else
				{
					AddItemToStorage(core->grabbed_inv_item_comp, icon_data->storage_comp, icon_data->slot);
					core->grabbed_inv_item_comp = 0;
				}
			}
		}
		else if (!core->is_mid_left_click && platform->right_mouse_pressed)
		{
			R_DEV_ASSERT(!core->grabbed_inv_item_comp, "Shouldn't be an existing grabbed item if it's a new press.");

			platform->right_mouse_pressed = 0;

			if (icon_data->item_comp && icon_data->item_comp->stack_size > 1)
			{
				Entity *new_item = NewEntity("Item", ENTITY_TYPE_item);
				AttachItem(new_item, icon_data->item_comp->item_type, icon_data->item_comp->stack_size / 2);
				core->grabbed_inv_item_comp = new_item->components[COMPONENT_item];
				core->grabbed_inv_item_origin_slot = -1;
				core->grabbed_inv_item_origin_storage_comp = 0;
				core->grabbed_inventory_item_offset = mouse;

				if (IsOdd(icon_data->item_comp->stack_size))
					icon_data->item_comp->stack_size = icon_data->item_comp->stack_size / 2 + 1;
				else
					icon_data->item_comp->stack_size = icon_data->item_comp->stack_size / 2;
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
	}
}

internal void inventory_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;

	Ts2dPushRect(core->renderer, v4(1.0f, 1.0f, 1.0f, 0.8f), rect);

	if (icon_data->item_comp)
	{
		f32 padding = 15;
		StaticSprite *sprite = GetStaticSprite(item_data[icon_data->item_comp->item_type].icon_sprite);
		Ts2dPushTexture(core->renderer, sprite->texture_atlas, sprite->source, v4(rect.x + padding / 2, rect.y + padding / 2, rect.z - padding, rect.w - padding));

		if (icon_data->item_comp->stack_size > 1)
		{
			char txt[100];
			sprintf(txt, "%i", icon_data->item_comp->stack_size);
			Ts2dPushText(core->renderer,
						 Ts2dGetDefaultFont(core->renderer),
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
		Ts2dPushFilledRect(core->renderer, v4(0.0f, 0.0f, 0.0f, 0.2f), rect);
	}
}

internal void hotbar_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;

	if (mouse.x >= 0.0f && mouse.x < rect.z && mouse.y >= 0.0f && mouse.y < rect.w)
		icon_data->is_hovered = 1;
	else
		icon_data->is_hovered = 0;

	if (icon_data->is_hovered)
	{
		if (platform->key_pressed[KEY_q])
		{
			if (icon_data->item_comp)
			{
				if (core->held_item)
				{
					ItemComponent *held_item_comp = core->held_item->components[COMPONENT_item];
					if (held_item_comp == icon_data->item_comp)
						RemoveHeldItem();
				}

				ConvertToGroundItem(icon_data->item_comp);
				icon_data->storage_comp->items[icon_data->slot] = 0;
			}
		}
		else if (platform->left_mouse_pressed)
		{
			R_DEV_ASSERT(!core->grabbed_inv_item_comp, "Shouldn't be an existing grabbed item if it's a new press.");

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
				if (item_data[core->grabbed_inv_item_comp->item_type].flags & ITEM_FLAGS_HOTBARABLE)
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
	}
}

internal void hotbar_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	InventoryIconCanvasData *icon_data = user_data;

	Ts2dPushRect(core->renderer, v4(1.0f, 1.0f, 1.0f, 0.8f), rect);

	if (icon_data->item_comp)
	{
		f32 padding = 15;
		StaticSprite *sprite = GetStaticSprite(item_data[icon_data->item_comp->item_type].icon_sprite);
		Ts2dPushTexture(core->renderer, sprite->texture_atlas, sprite->source, v4(rect.x + padding / 2, rect.y + padding / 2, rect.z - padding, rect.w - padding));

		R_DEV_ASSERT(icon_data->item_comp->stack_size == 1, "Don't support hotbar items with stacks. Is this intentional?");
	}

	char txt[100];
	sprintf(txt, "%i", icon_data->slot + 1);
	Ts2dPushText(core->renderer,
				 Ts2dGetDefaultFont(core->renderer),
				 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
				 v4(1.0f, 1.0f, 1.0f, 1.0f),
				 v2(rect.x + rect.z / 2,
					rect.y - 10.0f),
				 0.35f,
				 txt);

	if (icon_data->is_hovered)
	{
		Ts2dPushFilledRect(core->renderer, v4(0.0f, 0.0f, 0.0f, 0.2f), rect);
	}

	if (core->active_hotbar_slot == icon_data->slot + 1)
	{
		f32 padding = 5.0f;
		Ts2dPushRect(core->renderer, v4(1.0f, 0.0f, 0.0f, 0.8f), v4(rect.x - padding / 2.0f, rect.y - padding / 2.0f, rect.z + padding, rect.w + padding));
	}
}

internal void grabbed_icon_canvas_update_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
}

internal void grabbed_icon_canvas_render_callback(char *name, v4 rect, v2 mouse, void *user_data)
{
	GrabbedIconCanvasData *texture_data = user_data;

	Ts2dPushTexture(core->renderer, texture_data->static_sprite->texture_atlas, texture_data->static_sprite->source, rect);

	if (core->grabbed_inv_item_comp->stack_size > 1)
	{
		char txt[100];
		sprintf(txt, "%i", core->grabbed_inv_item_comp->stack_size);
		Ts2dPushText(core->renderer,
					 Ts2dGetDefaultFont(core->renderer),
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

	// NOTE(tjr): Draw backpack UI.
	if (is_backpack_open)
	{
		TsUIBeginInputGroup(core->ui);
		{
			SubSpriteComponent *player_sub_sprite = core->player->components[COMPONENT_sub_sprite];

			TsUIPushSize(core->ui, v2(60, 60));

			// NOTE(tjr): Render backpack.
			{
				StorageComponent *storage_comp = core->backpack->components[COMPONENT_storage];
				i32 column_length = 3;

				if (player_sub_sprite->is_flipped)
					TsUIPushX(core->ui, core->render_w / 2 - 50 - column_length * 60.0f);
				else
					TsUIPushX(core->ui, core->render_w / 2 + 50);
				TsUIPushY(core->ui, core->render_h / 2 + 110 - (storage_comp->storage_size / column_length * 30));

				for (int i = 0; i < storage_comp->storage_size; i++)
				{
					TsUIPushX(core->ui, (i % column_length) * 60.0f);
					TsUIPushY(core->ui, (i / column_length) * 60.0f);

					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->storage_comp = storage_comp;
					icon_data->item_comp = storage_comp->items[i];
					icon_data->slot = i;
					TsUICanvas(core->ui, "Icon", &inventory_icon_canvas_update_callback, icon_data, &inventory_icon_canvas_render_callback, icon_data);

					TsUIPopY(core->ui);
					TsUIPopX(core->ui);
				}

				TsUIPopY(core->ui);
				TsUIPopX(core->ui);
			}

			// NOTE(tjr): Render hotbar.
			{
				StorageComponent *hotbar_storage_comp = core->hotbar->components[COMPONENT_storage];

				if (player_sub_sprite->is_flipped)
					TsUIPushX(core->ui, core->render_w / 2 + 50.0f);
				else
					TsUIPushX(core->ui, core->render_w / 2 - 50.0f - hotbar_storage_comp->storage_size * 60);
				TsUIPushY(core->ui, core->render_h / 2 + 110.0f - 30.0f);

				for (int i = 0; i < hotbar_storage_comp->storage_size; i++)
				{
					InventoryIconCanvasData *icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(InventoryIconCanvasData));
					icon_data->storage_comp = hotbar_storage_comp;
					icon_data->item_comp = hotbar_storage_comp->items[i];
					icon_data->slot = i;
					TsUICanvas(core->ui, "Icon", &hotbar_icon_canvas_update_callback, icon_data, &hotbar_icon_canvas_render_callback, icon_data);
				}

				TsUIPopY(core->ui);
				TsUIPopX(core->ui);
			}

			TsUIPopSize(core->ui);
		}
		TsUIEndInputGroup(core->ui);

		// NOTE(tjr): If there's a held item but none of the slots have picked up on it's release. Throw it onto the ground.
		if (core->grabbed_inv_item_comp && !core->is_mid_right_click && core->left_mouse_released)
		{
			core->left_mouse_released = 0;
			ConvertToGroundItem(core->grabbed_inv_item_comp);
			core->grabbed_inv_item_comp = 0;
		}
		else if (core->grabbed_inv_item_comp && !core->is_mid_left_click && core->right_mouse_released)
		{
			core->right_mouse_released = 0;
			ConvertToGroundItem(core->grabbed_inv_item_comp);
			core->grabbed_inv_item_comp = 0;
		}

		// NOTE(tjr): Render grabbed item.
		if (core->grabbed_inv_item_comp)
		{
			TsUIPushPosition(core->ui, V2SubtractV2(v2(platform->mouse_x, platform->mouse_y), core->grabbed_inventory_item_offset));
			TsUIPushSize(core->ui, v2(60, 60));

			GrabbedIconCanvasData *grabbed_icon_data = MemoryArenaAllocateAndZero(core->frame_arena, sizeof(GrabbedIconCanvasData));
			grabbed_icon_data->static_sprite = GetStaticSprite(item_data[core->grabbed_inv_item_comp->item_type].icon_sprite);
			TsUICanvas(core->ui, "Texture", &grabbed_icon_canvas_update_callback, 0, &grabbed_icon_canvas_render_callback, grabbed_icon_data);

			TsUIPopSize(core->ui);
			TsUIPopPosition(core->ui);
		}
	}

	// NOTE(tjr): Move arrows for editor.
	if (core->is_in_editor && core->selected_entity)
	{
		StaticSprite *x_arrow = GetStaticSprite(STATIC_SPRITE_x_axis_arrow_icon);
		StaticSprite *y_arrow = GetStaticSprite(STATIC_SPRITE_y_axis_arrow_icon);
		StaticSprite *middle = GetStaticSprite(STATIC_SPRITE_middle_axis_icon);

		b8 entity_has_position = 0;
		v2 position = {0};
		PositionComponent *position_comp = core->selected_entity->components[COMPONENT_position];
		ParallaxComponent *parallax_comp = core->selected_entity->components[COMPONENT_parallax];
		if (parallax_comp)
		{
			position = parallax_comp->desired_position;
			entity_has_position = 1;
		}
		else if (position_comp)
		{
			position = position_comp->position;
			entity_has_position = 1;
		}

		if (entity_has_position)
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
		}
	}
}

internal void DrawEditorUI()
{
	local_persist b8 is_entity_window_open = 1;
	local_persist b8 is_performance_window_open = 0;
	local_persist b8 is_debug_window_open = 0;

	local_persist b8 pin_windows = 0;

	// NOTE(tjr): Drop-down menus
	if (core->is_in_editor)
	{
		TsUIPushAutoRow(core->ui, v2(0, 0), 30);
		if (TsUIDropdown(core->ui, "World..."))
		{
			TsUIButton(core->ui, "Commit");
			TsUIButton(core->ui, "Reload");
		}
		TsUIDropdownEnd(core->ui);
		if (TsUIDropdown(core->ui, "Windows..."))
		{
			if (TsUIToggler(core->ui, "Entity", is_entity_window_open))
				is_entity_window_open = 1;
			else
				is_entity_window_open = 0;

			if (TsUIToggler(core->ui, "Performance", is_performance_window_open))
				is_performance_window_open = 1;
			else
				is_performance_window_open = 0;

			if (TsUIToggler(core->ui, "Debug", is_debug_window_open))
				is_debug_window_open = 1;
			else
				is_debug_window_open = 0;
		}
		TsUIDropdownEnd(core->ui);
		if (TsUIDropdown(core->ui, "Options..."))
		{
			if (TsUIToggler(core->ui, "Pin Windows", pin_windows))
				pin_windows = 1;
			else
				pin_windows = 0;
		}
		TsUIDropdownEnd(core->ui);
		TsUIPopRow(core->ui);
	}

	// NOTE(tjr): Time dilation
	if (core->is_in_editor)
	{
		TsUIBeginInputGroup(core->ui);
		TsUIPushColumn(core->ui, v2(core->render_w / 2.0f - 125.0f, 40.0f), v2(250.0f, 30.0f));

		core->world_delta_mult = TsUISlider(core->ui, "World Time Dilation", core->world_delta_mult, 0.0f, 1.0f);

		TsUIPopColumn(core->ui);
		TsUIEndInputGroup(core->ui);
	}

	// NOTE(tjr): Draw windows.
	if (pin_windows || core->is_in_editor)
	{
		if (is_entity_window_open)
		{
			// NOTE(tjr): Entity info window.
			v4 entity_info_window_rect = {core->render_w - 360, 10, 350, 300};
			TsUIWindowBegin(core->ui, "Entity Info", entity_info_window_rect, 0, 0);
			{
				TsUIPushColumn(core->ui, v2(10, 10), v2(100, 30));

				if (!core->selected_entity)
				{
					TsUIPushAutoWidth(core->ui);
					TsUILabel(core->ui, "No Entity selected.");
					TsUIPopWidth(core->ui);
				}
				else
				{
					TsUIPushWidth(core->ui, entity_info_window_rect.width - 50);
					{
						char *label = MakeCStringOnMemoryArena(core->frame_arena, "%s #%i",
															   core->selected_entity->name, core->selected_entity->entity_id);
						TsUITitle(core->ui, label);

						for (int i = 1; i < COMPONENT_MAX; i++)
						{
							if (core->selected_entity->components[i])
							{
								PrintComponentUI(core->selected_entity->components[i], i);
							}
						}
					}
					TsUIPopWidth(core->ui);

					TsUIDivider(core->ui);

					TsUIPushAutoWidth(core->ui);
					if (!(core->selected_entity->flags & ENTITY_FLAGS_no_delete))
					{
						if (TsUIButton(core->ui, "Delete Entity"))
						{
							DeleteEntity(core->selected_entity);
							core->selected_entity = 0;
						}
					}
					TsUIPopWidth(core->ui);
				}

				TsUIPopColumn(core->ui);
			}
			TsUIWindowEnd(core->ui);

			v4 entity_list_window_rect = {core->render_w - 360, 310, 350, 500};
			TsUIWindowBegin(core->ui, "Entity List", entity_list_window_rect, 0, 0);
			{
				TsUIPushColumn(core->ui, v2(10, 10), v2(150, 30));

				local_persist b8 is_index_mode = 0;

				// NOTE(rjf): Sort Controls
				{
					TsUIPushWidth(core->ui, 90);
					TsUILabel(core->ui, "Sort by: ");
					TsUIPopWidth(core->ui);

					TsUIPushAutoWidth(core->ui);
					{
						TsUISameLine(core->ui);
						if (TsUIToggler(core->ui, "Index", is_index_mode))
						{
							is_index_mode = 1;
						}

						TsUISameLine(core->ui);
						if (TsUIToggler(core->ui, "Category", !is_index_mode))
						{
							is_index_mode = 0;
						}
					}
					TsUIPopWidth(core->ui);
				}

				TsUIDivider(core->ui);

				if (is_index_mode)
				{
					// NOTE(tjr): Index view
					for (int i = 1; i < core->world_data->entity_count; i++)
					{
						TsUIPushWidth(core->ui, 30);
						{
							char label[100];
							sprintf(label, "%i", i);
							TsUILabel(core->ui, label);
						}
						TsUIPopWidth(core->ui);

						Entity *entity = &core->world_data->entities[i];
						if (entity->entity_id > 0)
						{
							TsUISameLine(core->ui);
							TsUIPushWidth(core->ui, entity_list_window_rect.width - 80);
							if (TsUIToggler(core->ui, entity->name, (core->selected_entity ? core->selected_entity->entity_id == i : 0)))
							{
								core->selected_entity = entity;
							}
							else
							{
								if (core->selected_entity->entity_id == i)
								{
									core->selected_entity = 0;
								}
							}
							TsUIPopWidth(core->ui);
						}
						else
						{
							TsUISameLine(core->ui);
							TsUIPushSize(core->ui, v2(100, 30));
							TsUILabel(core->ui, "- - - - -");
							TsUIPopSize(core->ui);
						}
					}
				}
				else
				{
					TsUIPushWidth(core->ui, entity_list_window_rect.width - 50);

					// NOTE(tjr): Entity category (type) view
					for (int i = 0; i < ENTITY_TYPE_MAX; i++)
					{
						if (TsUICollapsable(core->ui, GetEntityTypeName(i)))
						{
							for (int j = 1; j < core->world_data->entity_count; j++) // TEMP: Need to sort these before-hand. Will eventually get too inefficient.
							{
								Entity *entity = &core->world_data->entities[j];
								if (entity->entity_id > 0 && entity->type == i)
								{
									if (TsUIToggler(core->ui, entity->name, (core->selected_entity ? core->selected_entity->entity_id == j : 0)))
									{
										core->selected_entity = entity;
									}
									else
									{
										if (core->selected_entity &&
											core->selected_entity->entity_id == j)
										{
											core->selected_entity = 0;
										}
									}
								}
							}

							TsUICollapsableEnd(core->ui);
						}
					}

					TsUIPopWidth(core->ui);
				}

				TsUIPopColumn(core->ui);
			}
			TsUIWindowEnd(core->ui);
		}

		if (is_performance_window_open)
		{
			// NOTE(tjr): Display performance data.
			TsUIWindowBegin(core->ui, "Performance", v4(10, 500, 300, 280), 0, 0);
			{
				TsUIPushColumn(core->ui, v2(10, 0), v2(100, 50));

				f32 budget_total = 0.0f; // NOTE(tjr): Not actual amount, need to calculate this more accurately. Need to create some sort of "Unnaccounted" measurement
				for (int i = 0; i < core->performance_timer_count; i++)
				{
					char label[100];

					f32 timer_budget_amount = (core->performance_timers[i].finish_time - core->performance_timers[i].start_time) / core->raw_delta_t * 100.0f;
					sprintf(label,
							"%s: %f",
							core->performance_timers[i].name,
							timer_budget_amount);

					budget_total += timer_budget_amount;

					TsUILabel(core->ui, label);
				}

				char label[100];
				sprintf(label, "Frame Budget Usage: %f", budget_total);
				TsUILabel(core->ui, label);

				TsUIPopColumn(core->ui);
			}
			TsUIWindowEnd(core->ui);
		}

		if (is_debug_window_open)
		{
			// NOTE(tjr): Debug random stuff
			TsUIWindowBegin(core->ui, "Debug", v4(310, 500, 300, 200), 0, 0);
			{
				TsUIPushColumn(core->ui, v2(10, 0), v2(200, 30));

				char label[100];
				sprintf(label, "Camera Pos: %f, %f", core->camera_position.x, core->camera_position.y);
				TsUILabel(core->ui, label);

				TsUIPopColumn(core->ui);
			}
			TsUIWindowEnd(core->ui);
		}
	}
}