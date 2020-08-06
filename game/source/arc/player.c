internal void PreMoveUpdatePlayer()
{
	Entity *character = GetCharacterEntity();
	Assert(character);
	
	b8 is_sprinting = 0;
	if (!core->run_data->disable_player_input &&
		core->run_data->editor_state == EDITOR_STATE_none &&
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
	else if (!core->run_data->disable_player_input &&
			 core->run_data->editor_state == EDITOR_STATE_none &&
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
	
	if (!core->run_data->disable_player_input &&
		core->run_data->editor_state == EDITOR_STATE_none &&
		platform->key_pressed[KEY_space])
	{
		character->physics.force.y = -10000.0f / character->physics.mass_data.inv_mass;
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
	
	// NOTE(randy): Player keybind stuff
	if (platform->key_pressed[KEY_1])
	{
		character->active_hotbar_slot = 0;
	}
	else if (platform->key_pressed[KEY_2])
	{
		character->active_hotbar_slot = 1;
	}
}

internal void PostMoveUpdatePlayer()
{
}