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
								Entity *grabbed_item = &core->entity_set->entities[core->grabbed_inv_item_comp->entity_id];
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

	// NOTE(tjr): Draw developer UI.
	if (core->performance_view)
	{
		// NOTE(tjr): Display performance data.
		TsUIWindowBegin(core->ui, "Performance", v4(10, 10, 300, 280), 0, 0);
		{
			TsUIPushColumn(core->ui, v2(10, 0), v2(100, 50));

			f32 budget_total = 0.0f;
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

		// NOTE(tjr): Debug random stuff
		TsUIWindowBegin(core->ui, "Debug", v4(core->render_w - 310, 10, 300, 200), 0, 0);
		{
			TsUIPushColumn(core->ui, v2(10, 0), v2(200, 30));

			if (core->player)
			{
				AnimationComponent *anim_comp = core->player->components[COMPONENT_animation];
				char label[100];
				sprintf(label, "Player animation frame: %i", anim_comp->current_frame);
				TsUILabel(core->ui, label);
			}

			if (TsUIButton(core->ui, "Add dummy object"))
			{
				NewEntity("dummy", ENTITY_TYPE_undefined);
			}

			TsUIPopColumn(core->ui);
		}
		TsUIWindowEnd(core->ui);
	}
}

internal void UpdateEditor()
{
	// NOTE(tjr): Render the editor UI
	{
		// NOTE(tjr): Drop-down menu
		{
			TsUIPushRow(core->ui, v2(0.0f, 0.0f), v2(100.0f, 30.0f));
			if (TsUIDropdown(core->ui, "World..."))
			{
				TsUIButton(core->ui, "Commit");
				TsUIButton(core->ui, "Reload");
			}
			TsUIDropdownEnd(core->ui);
			TsUIPopRow(core->ui);
		}

		// NOTE(tjr): Time dilation
		{
			TsUIBeginInputGroup(core->ui);
			TsUIPushColumn(core->ui, v2(core->render_w / 2.0f - 125.0f, 40.0f), v2(250.0f, 30.0f));

			core->world_delta_mult = TsUISlider(core->ui, "World Time Dilation", core->world_delta_mult, 0.0f, 1.0f);

			TsUIPopY(core->ui);
			TsUIPopColumn(core->ui);
			TsUIEndInputGroup(core->ui);
		}

		// NOTE(tjr): Entity windows
		{
			v4 entity_list_window_rect = {core->render_w - 360, core->render_h - 510, 350, 500};
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
					for (int i = 1; i < core->entity_set->entity_count; i++)
					{
						TsUIPushAutoWidth(core->ui);

						TsUIPushWidth(core->ui, 30);
						{
							char label[100];
							sprintf(label, "%i", i);
							TsUILabel(core->ui, label);
						}
						TsUIPopWidth(core->ui);

						Entity *entity = &core->entity_set->entities[i];
						if (entity->entity_id > 0)
						{
							TsUISameLine(core->ui);
							TsUIPushWidth(core->ui, entity_list_window_rect.width - 80);
							if (TsUIToggler(core->ui, entity->name, core->selected_entity == i))
							{
								core->selected_entity = i;
							}
							else
							{
								if (core->selected_entity == i)
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

						TsUIPopWidth(core->ui);
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
							for (int j = 1; j < core->entity_set->entity_count; j++) // TEMP: Need to sort these before-hand. Will eventually get too inefficient.
							{
								Entity *entity = &core->entity_set->entities[j];
								if (entity->entity_id > 0 && entity->type == i)
								{
									if (TsUIToggler(core->ui, entity->name, core->selected_entity == j))
									{
										core->selected_entity = j;
									}
									else
									{
										if (core->selected_entity == j)
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

			char entity_window_title[100];
			sprintf(entity_window_title, core->selected_entity == 0 ? "Entity Info" : "Entity Info - %s", core->entity_set->entities[core->selected_entity].name);
			TsUIWindowBegin(core->ui, entity_window_title, v4(core->render_w - 360, 10, 350, 300), 0, 0);
			{
				if (core->selected_entity == 0)
				{
					TsUIPushAutoWidth(core->ui);
					TsUILabel(core->ui, "No Entity selected.");
					TsUIPopWidth(core->ui);
				}
				else
				{
					Entity *selected_entity = &core->entity_set->entities[core->selected_entity];
					R_DEV_ASSERT(selected_entity->entity_id > 0, "Invalid entity");

					TsUIPushAutoWidth(core->ui);
					TsUIPushHeight(core->ui, 30);
					TsUIPushPosition(core->ui, v2(100, 200));
					if (!(selected_entity->flags & ENTITY_FLAGS_no_delete))
					{
						if (TsUIButton(core->ui, "Delete Entity"))
						{
							DeleteEntity(selected_entity);
							core->selected_entity = 0;
						}
					}
					TsUIPopPosition(core->ui);
					TsUIPopHeight(core->ui);

					TsUIPushColumn(core->ui, v2(0, 0), v2(100, 30));

					TsUILabel(core->ui, core->entity_set->entities[core->selected_entity].name);
					// ...

					TsUIPopColumn(core->ui);

					TsUIPopWidth(core->ui);
				}
			}
			TsUIWindowEnd(core->ui);
		}
	}

	TransformEditorCamera();

	TsPlatformCaptureKeyboard();
}