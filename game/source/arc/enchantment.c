internal void DrawEnchantingUI()
{
	if (!(core->run_data->character_state & CHARACTER_STATE_arcane_mode) ||
		!(core->run_data->character_state & CHARACTER_STATE_is_enchanting))
		return;
	
	Entity *character = GetCharacterEntity();
	Entity *enchanter = core->run_data->engaged_station_entity;
	Assert(enchanter);
	
	Item *enchantable_items[32] = {0};
	i32 enchantable_items_count = 0;
	
	// NOTE(randy): Search player for enchantable items
	for (i32 i = 0; i < character->inventory_size; i++)
	{
		Item *item = &character->inventory[i];
		ItemTypeData *item_data = &global_item_type_data[item->type];
		
		if (item_data->flags & ITEM_FLAGS_enchantable)
		{
			enchantable_items[enchantable_items_count++] = item;
		}
	}
	for (i32 i = 0; i < character->hotbar_size; i++)
	{
		Item *item = &character->hotbar[i];
		ItemTypeData *item_data = &global_item_type_data[item->type];
		
		if (item_data->flags & ITEM_FLAGS_enchantable)
		{
			enchantable_items[enchantable_items_count++] = item;
		}
	}
	
	f32 first_pos = enchantable_items_count * 20.0f / -2.0f;
	
	local_persist i32 selected_item_index = -1;
	if (selected_item_index == -1 && enchantable_items_count > 0)
	{
		selected_item_index = enchantable_items_count / 2;
	}
	
	local_persist b8 has_item_been_selected = 0;
	if (has_item_been_selected)
	{
		Item *item = enchantable_items[selected_item_index];
		
		// NOTE(randy): Get a list of possible enchantments
		EnchantmentType enchantments[ENCHANTMENT_TYPE_MAX] = {0};
		i32 enchantment_count = 0;
		for (i32 enchantment_type = 1; enchantment_type < ENCHANTMENT_TYPE_MAX; enchantment_type++)
		{
			if (IsEnchantmentApplicable(enchantment_type, item->type) &&
				!item->enchantments[enchantment_type].type)
			{
				enchantments[enchantment_count++] = enchantment_type;
			}
		}
		
		// NOTE(randy): Determine the selected enchantment
		local_persist i32 selected_enchantment_index = -1;
		if (platform->key_pressed[KEY_s])
		{
			if (selected_enchantment_index > 0)
			{
				selected_enchantment_index--;
			}
			else
			{
				selected_enchantment_index = enchantment_count - 1;
			}
		}
		
		if (platform->key_pressed[KEY_w])
		{
			if (selected_enchantment_index < enchantment_count - 1)
			{
				selected_enchantment_index++;
			}
			else
			{
				selected_enchantment_index= 0;
			}
		}
		
		if (selected_enchantment_index == -1 &&
			enchantment_count > 0)
		{
			selected_enchantment_index = 0;
		}
		
		// NOTE(randy): Display a list of enchantments
		for (i32 i = 0; i < enchantment_count; i++)
		{
			EnchantmentTypeData *enchantment_data = &global_enchantment_type_data[enchantments[i]];
			
			char enchant_name[100];
			sprintf(enchant_name, "%s", enchantment_data->print_name);
			
			v2 render_pos = v2view(V2AddV2(enchanter->position, v2(0.0f, -40.0f - 10.0f * i)));
			
			ArcPushText(Ts2dGetDefaultFont(),
						TS2D_TEXT_ALIGN_CENTER_X,
						selected_enchantment_index == i ? v4(1.0f, 0.0f, 0.0f, 1.0f) : v4u(1.0f),
						render_pos,
						0.75f,
						enchant_name,
						LAYER_HUD);
		}
	}
	else
	{
		if (platform->key_pressed[KEY_e] &&
			selected_item_index != -1)
		{
			has_item_been_selected = 1;
		}
		
		if (platform->key_pressed[KEY_a])
		{
			if (selected_item_index > 0)
			{
				selected_item_index--;
			}
			else
			{
				selected_item_index = enchantable_items_count - 1;
			}
		}
		
		if (platform->key_pressed[KEY_d])
		{
			if (selected_item_index < enchantable_items_count - 1)
			{
				selected_item_index++;
			}
			else
			{
				selected_item_index = 0;
			}
		}
		
		for (i32 i = 0; i < enchantable_items_count; i++)
		{
			Item *item = enchantable_items[i];
			ItemTypeData *item_data = &global_item_type_data[item->type];
			StaticSpriteData *sprite = &global_static_sprite_data[item_data->icon_sprite];
			
			v2 render_pos = v2view(V2AddV2(enchanter->position, v2(first_pos + i * 20.0f, -50.0f)));
			v2 render_size = v2zoom(v2(20.0f, 20.0f));
			
			ArcPushTexture(sprite->texture_atlas,
						   0,
						   sprite->source,
						   v4(render_pos.x, render_pos.y,
							  render_size.x, render_size.y),
						   item == enchantable_items[selected_item_index] ? v4(1.0f, 0.5f, 0.5f, 1.0f) : v4u(1.0f),
						   LAYER_HUD);
		}
	}
}

internal void OnEnchanterInteract(Entity *entity)
{
	Entity *character = GetCharacterEntity();
	
	SetArcaneMode(1);
	core->run_data->character_state |= CHARACTER_STATE_is_enchanting;
	
	core->run_data->engaged_station_entity = entity;
}

internal b8 IsEnchantmentApplicable(EnchantmentType enchantment, ItemType item)
{
	ItemTypeData *item_type_data = &global_item_type_data[item];
	
	switch (item_type_data->category)
	{
		case ITEM_CATEGORY_sword :
		{
			return enchantment == ENCHANTMENT_TYPE_test ||
				enchantment == ENCHANTMENT_TYPE_yeet ||
				enchantment == ENCHANTMENT_TYPE_uhhhh;
		} break;
		
		case ITEM_CATEGORY_lumber_axe :
		{
			return enchantment == ENCHANTMENT_TYPE_test;
		} break;
		
		default :
		{
			Assert(0);
			return 0;
		} break;
	}
}