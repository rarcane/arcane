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
		core->camera_position.x -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_d])
	{
		core->camera_position.x += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	if (platform->key_down[KEY_w])
	{
		core->camera_position.y -= 600.0f * transform_mult / core->camera_zoom * core->delta_t;
	}
	else if (platform->key_down[KEY_s])
	{
		core->camera_position.y += 600.0f * transform_mult / core->camera_zoom * core->delta_t;
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
	
	if (core->run_data->lock_camera)
	{
		f32 alpha = 1.0f - (core->run_data->move_to_start_time + core->run_data->move_to_time - platform->GetTime()) / core->run_data->move_to_time;
		alpha = ClampF32(alpha, 0.0f, 1.0f);
		
		// v2 diff = V2SubtractV2(core->run_data->initial_camera_pos, core->run_data->move_to_location);
		
		core->camera_position.x = LerpF32(Fade(alpha), core->run_data->initial_camera_pos.x, core->run_data->move_to_location.x);
		core->camera_position.y = LerpF32(Fade(alpha), core->run_data->initial_camera_pos.y, core->run_data->move_to_location.y);
		
		core->camera_zoom_mult = LerpF32(Fade(alpha), core->run_data->inital_zoom, core->run_data->move_to_zoom);
	}
	else
	{
		f32 length = 50.0f;
		f32 alpha = fabsf(core->camera_offset.x) / length;
		local_persist f32 idle_end_buffer = 0.0f;
		local_persist f32 start_buffer = 0.0f;
		local_persist f32 last_axis = 0.0f;
		
		if (character->axis_x == 0.0f)
		{
			idle_end_buffer += core->delta_t;
			
			if (idle_end_buffer >= 1.0f)
			{
				start_buffer = 0.0f;
				
				i32 sign = GetSign(core->camera_offset.x);
				if (sign != 0.0f)
				{
					core->camera_offset.x = fabsf(core->camera_offset.x);
					
					// f32 fade = -powf((alpha - 0.5f), 2.0f) * 4.0f + 1.0f;
					// f32 fade = powf(alpha, 3.0f) * (-powf((alpha - 0.5f), 2.0f) * 4.0f + 1.0f) * 3.0f;
					
					f32 fade = 16.0f * powf(powf(alpha, 2.0f) - alpha, 2.0f);
					
					f32 x_velocity = LerpF32(fade, 3.0f, 50.0f);
					
					core->camera_offset.x -= core->world_delta_t * x_velocity;
					core->camera_offset.x = ClampF32(core->camera_offset.x, 0.0f, length);
					
					core->camera_offset.x *= sign;
				}
			}
		}
		else
		{
			idle_end_buffer = 0.0f;
			
			start_buffer += core->delta_t;
			
			if (start_buffer >= 1.0f)
			{
				// f32 fade = powf(alpha, 2.0f) * (-powf((alpha - 0.5f), 2.0f) * 4.0f + 1.0f) * 2.35f;
				// f32 fade = 1 - (1 - alpha) * (1 - alpha);
				f32 fade = powf(alpha, 3.0f);
				// f32 fade = powf((2 * alpha) / (powf(alpha, 2.0f) + 1), 3.0f);
				
				f32 x_velocity = LerpF32(fade, 30.0f, 3.0f) * GetSign(character->axis_x);
				
				core->camera_offset.x += core->world_delta_t * x_velocity;
				core->camera_offset.x = ClampF32(core->camera_offset.x, -length, length);
			}
			else
			{
				if (GetSign(last_axis) != GetSign(character->axis_x))
				{
					start_buffer = 0.0f;
				}
			}
		}
		
		last_axis = character->axis_x;
		
		core->camera_position = V2AddV2(v2(core->camera_offset.x, core->camera_offset.y), idle_camera_pos);
	}
	
	core->camera_offset.y = -DEFAULT_CAMERA_OFFSET_Y;
	
	//core->camera_zoom = (core->render_h / (1080.0f / DEFAULT_CAMERA_ZOOM)) * core->camera_zoom_mult;
	//core->camera_zoom = 1.0f;
	
	f32 transform_mult = platform->key_down[KEY_shift] ? 2.0f : 1.0f;
	
	if (platform->mouse_scroll_y != 0.0f)
	{
		core->camera_zoom += platform->mouse_scroll_y / 120.0f / 4 * transform_mult;
		if (core->camera_zoom <= 0.1f)
			core->camera_zoom = 0.1f;
		else if (core->camera_zoom > 10.0f)
			core->camera_zoom = 10.0f;
	}
	
	TsUIWindowBegin("debug", v4(0.0f, 0.0f, 600.0f, 300.0f), 0, 0);
	{
		TsUIPushColumn(v2(0.0f, 0.0f), v2(100.0f, 20.0f));
		
		{
			char lbl[100];
			sprintf(lbl, "camera pos: %f, %f", core->camera_position.x, core->camera_position.y);
			TsUILabel(lbl);
		}
		
		{
			char lbl[100];
			sprintf(lbl, "camera zoom: %f", core->camera_zoom);
			TsUILabel(lbl);
		}
		
		global_ts2d->ground_scale = TsUISlider("Scale", global_ts2d->ground_scale, 0.004f, 0.5f);
		global_ts2d->ground_vor_step = TsUISlider("Voronoi Step", global_ts2d->ground_vor_step, 0.004f, 0.5f);
		global_ts2d->ground_band_height = TsUISlider("Band Height", global_ts2d->ground_band_height, 1.0f, 100.0f);
		
		{
			v4 camera_region = GetCameraRegionRect();
			char lbl[100];
			sprintf(lbl, "camera region: %f, %f, %f, %f", camera_region.x, camera_region.y, camera_region.z, camera_region.w);
			TsUILabel(lbl);
		}
		/*
				{
					char lbl[100];
					sprintf(lbl, "alpha: %f", alpha);
					TsUILabel(lbl);
				}
		 */
		
		TsUIPopColumn();
	}
	TsUIWindowEnd();
	
	// NOTE(randy): Clear camera cue buffer
	//MemorySet(core->run_data->camera_cues, 0, sizeof(core->run_data->camera_cues));
	//core->run_data->camera_cue_count = 0;
}

internal void MoveCameraToLocation(v2 location, f32 zoom, f32 time)
{
	Assert(!core->run_data->lock_camera);
	core->run_data->lock_camera = 1;
	
	core->run_data->initial_camera_pos = core->camera_position;
	core->run_data->inital_zoom = core->camera_zoom_mult;
	core->run_data->move_to_location = location;
	core->run_data->move_to_zoom = zoom;
	core->run_data->move_to_time = time;
	core->run_data->move_to_start_time = platform->GetTime();
}