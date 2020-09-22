static void TransformEditorCamera()
{
	f32 transform_mult = platform->key_down[KEY_shift] ? 2.0f : 1.0f;
	
	if (platform->mouse_scroll_y != 0.0f)
	{
		core->camera_zoom += platform->mouse_scroll_y / 120.0f / 4 * transform_mult;
		if (core->camera_zoom <= 0.1f)
			core->camera_zoom = 0.1f;
		else if (core->camera_zoom > 10.0f)
			core->camera_zoom = 10.0f;
	}
	
	if (platform->key_down[KEY_a])
	{
		core->camera_position.x += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_d])
	{
		core->camera_position.x -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	if (platform->key_down[KEY_w])
	{
		core->camera_position.y += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_s])
	{
		core->camera_position.y -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
}

static void TransformInGameCamera()
{
	Entity *character = GetCharacterEntity();
	
	v2 idle_camera_pos = character->position;
	
	/*
	v2 total_weighted_positions = {0};
	f32 total_weight = 0.0f;
	
						total_weighted_positions = V2AddV2(total_weighted_positions, idle_camera_pos);
						total_weight += 1.0f;
	
		for (i32 i = 0; i < core->run_data->camera_cue_count; i++)
		{
			CameraCue *cue = &core->run_data->camera_cues[i];
			
			total_weighted_positions = V2AddV2(total_weighted_positions, V2MultiplyF32(cue->position, cue->weight));
			
			total_weight += cue->weight;
		}
		
		if (core->run_data->camera_cue_count)
		{
			v2 weighted_pos = V2DivideF32(total_weighted_positions, total_weight);
			
			{
				f32 alpha = 0.0f;
				
				
			}
			
			core->camera_offset = V2SubtractV2(weighted_pos, idle_camera_pos);
		}
		else
		{
			core->camera_offset = v2(0.0f, 0.0f);
		}
	 */
	
	local_persist f32 alpha = 0.0f;
	
	f32 buffer = 0.0f;
	f32 length = 50.0f;
	
	if (fabsf(alpha) < 1.0f &&
		fabsf(alpha + character->axis_x * core->world_delta_t) > 1.0f)
	{
		alpha = (1.0f + buffer) * GetSign(alpha);
	}
	else
	{
		alpha += character->axis_x * core->world_delta_t;
	}
	
	if (fabsf(alpha) > (1.0f + buffer))
	{
		alpha = (1.0f + buffer) * GetSign(alpha);
	}
	
	if (alpha >= 0.0f)
	{
		core->camera_offset.x = LerpF32(EaseOut(Clamp(0.0f, 1.0f, alpha)), 0.0f, length);
	}
	else
	{
		core->camera_offset.x = LerpF32(EaseOut(Clamp(0.0f, 1.0f, -alpha)) * -1.0f, 0.0f, length);
	}
	
	
	//core->camera_offset.x =
	
	//f32 desired_camera_offset = character->axis_x * 100.0f;
	
	//4core->camera_offset.x = character->velocity.x;
	
	core->camera_zoom = core->render_h / (1080.0f / DEFAULT_CAMERA_ZOOM);
	core->camera_position = V2AddV2(v2(core->camera_offset.x, core->camera_offset.y), idle_camera_pos);
	
	core->run_data->disable_chunk_loaded_based_off_view = 0;
	
	// NOTE(randy): Clear camera cue buffer
	//MemorySet(core->run_data->camera_cues, 0, sizeof(core->run_data->camera_cues));
	//core->run_data->camera_cue_count = 0;
	
	TsUIWindowBegin("window", v4(0.0f, 0.0f, 200.0f, 300.0f), 0, 0);
	{
		TsUIPushColumn(v2(0.0f, 0.0f), v2(100.0f, 20.0f));
		
		{
			char lbl[100];
			sprintf(lbl, "velocity: %f", character->smooth_velocity.x);
			TsUILabel(lbl);
		}
		
		/*
				{
					char lbl[100];
					sprintf(lbl, "test: %f", total_weighted_positions.y);
					TsUILabel(lbl);
				}
		 */
		
		TsUIPopColumn();
	}
	TsUIWindowEnd();
}