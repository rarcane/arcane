internal void PreMoveUpdatePlayer()
{
	Entity *character = GetCharacterEntity();
	if (!character)
		return;
	
	b8 is_sprinting = 0;
	if (CanPlayerMove() &&
		platform->key_down[KEY_a])
	{
		if (platform->key_down[KEY_d])
			character->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				character->axis_x = -1.5;
				is_sprinting = 1;
			}
			else
			{
				character->axis_x = -0.5f;
			}
		}
	}
	else if (CanPlayerMove() &&
			 platform->key_down[KEY_d])
	{
		if (platform->key_down[KEY_a])
			character->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				character->axis_x = 1.5;
				is_sprinting = 1;
			}
			else
			{
				character->axis_x = 0.5f;
			}
		}
	}
	else
	{
		character->axis_x = 0.0f;
	}
	
	if (fabsf(character->physics.velocity.x) < fabsf(character->move_speed * character->axis_x))
		character->physics.force.x = character->axis_x * (1200 / character->physics.mass_data.inv_mass);
	
	if (CanPlayerMove() &&
		platform->key_pressed[KEY_space])
	{
		character->physics.force.y = -30000.0f / character->physics.mass_data.inv_mass;
	}
	
	// NOTE(randy): Update animation state
	{
		if (character->axis_x < 0.0f)
		{
			character->is_flipped = 1;
		}
		else if (character->axis_x > 0.0f)
		{
			character->is_flipped = 0;
		}
		
		if (character->axis_x == 0)
		{
			SetArcEntityAnimationState(character, ARC_ENTITY_ANIMATION_STATE_player_idle, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
		}
		else
		{
			if (is_sprinting)
			{
				SetArcEntityAnimationState(character, ARC_ENTITY_ANIMATION_STATE_player_sprinting, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
			else
			{
				SetArcEntityAnimationState(character, ARC_ENTITY_ANIMATION_STATE_player_walking, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
		}
	}
	
	// Acceleration calculations?
	
	CharacterData *character_data = GetCharacterData();
	
	// NOTE(randy): Player keybind stuff
	if (CanPlayerMove() &&
		platform->key_pressed[KEY_1])
	{
		character_data->active_hotbar_slot = 0;
	}
	else if (CanPlayerMove() &&
			 platform->key_pressed[KEY_2])
	{
		character_data->active_hotbar_slot = 1;
	}
	
	// NOTE(randy): Arcane mode
	if (platform->key_pressed[KEY_x])
	{
		SetArcaneMode(!(core->run_data->character_state & CHARACTER_STATE_arcane_mode));
	}
	
	// NOTE(randy): Spell casting
	if (character_data->freehand_spell_count)
	{
		local_persist b8 was_down = 0;
		local_persist Spell *selected_spell = 0;
		if (platform->key_down[KEY_q])
		{
			core->world_delta_mult = 0.5f;
			
			{
				Spell *spell = &character_data->freehand_spell_slots[0];
				
				v2 render_pos = v2screen(v2(0.2f, 0.5f));
				v2 render_size = v2GUI(v2(0.1f, 0.1f));
				
				render_pos = V2SubtractV2(render_pos, V2MultiplyF32(render_size, 0.5f));
				
				StaticSpriteData *sprite = &global_static_sprite_data[STATIC_SPRITE_shia];
				ArcPushTexture(sprite->texture_atlas,
							   0,
							   sprite->source,
							   v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
							   v4u(1.0f),
							   LAYER_FRONT_UI);
				
				if (platform->mouse_x < core->render_w / 2.0f)
					selected_spell = spell;
			}
			
			{
				Spell *spell = &character_data->freehand_spell_slots[1];
				
				v2 render_pos = v2screen(v2(0.8f, 0.5f));
				v2 render_size = v2GUI(v2(0.1f, 0.1f));
				
				render_pos = V2SubtractV2(render_pos, V2MultiplyF32(render_size, 0.5f));
				
				StaticSpriteData *sprite = &global_static_sprite_data[STATIC_SPRITE_shia2];
				ArcPushTexture(sprite->texture_atlas,
							   0,
							   sprite->source,
							   v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
							   v4u(1.0f),
							   LAYER_FRONT_UI);
				
				if (platform->mouse_x >= core->render_w / 2.0f)
					selected_spell = spell;
			}
			
			was_down = 1;
		}
		else if (was_down)
		{
			core->world_delta_mult = 1.0f;
			
			if (selected_spell)
			{
				SpellTypeData *spell = &global_spell_type_data[selected_spell->type];
				if (spell)
				{
					spell->cast_callback();
				}
			}
			
			was_down = 0;
		}
	}
}

internal void PostMoveUpdatePlayer()
{
}

internal b8 CanPlayerMove()
{
	return !(core->run_data->character_state & CHARACTER_STATE_is_crafting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_blueprinting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_enchanting) &&
		!(core->run_data->character_state & CHARACTER_STATE_is_in_elemental_skill_tree);
}

internal void SetArcaneMode(i32 on)
{
	if (on)
	{
		core->run_data->character_state |= CHARACTER_STATE_arcane_mode;
		core->run_data->character_entity->sprite_data.tint = v4(1.0f, 0.0f, 0.0f, 0.5f);
	}
	else
	{
		core->run_data->character_state &= ~CHARACTER_STATE_arcane_mode;
		core->run_data->character_entity->sprite_data.tint = v4u(1.0f);
		
		// NOTE(randy): just force disable other modes?
		core->run_data->character_state &= ~CHARACTER_STATE_is_crafting;
		core->run_data->character_state &= ~CHARACTER_STATE_is_enchanting;
		core->run_data->character_state &= ~CHARACTER_STATE_is_in_elemental_skill_tree;
	}
}