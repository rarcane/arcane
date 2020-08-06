internal void InitialiseSpriteData()
{
	for (int i = 0; i < STATIC_SPRITE_MAX; i++)
	{
		StaticSpriteData *sprite = &global_static_sprite_data[i];
		sprite->texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite->texture_path);
	}
	
	for (int i = 0; i < DYNAMIC_SPRITE_MAX; i++)
	{
		DynamicSpriteData *sprite = &global_dynamic_sprite_data[i];
		sprite->texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite->texture_path);
	}
}

internal void UpdateAnimations()
{
	for (i32 i = 0; i < core->run_data->entity_count; i++)
	{
		Entity *entity = &core->run_data->entities[i];
		if ((entity->flags & ENTITY_FLAGS_animation) == 0)
		{
			continue;
		}
		
		DynamicSpriteData *dynamic_sprite = &global_dynamic_sprite_data[entity->sprite_data.dynamic_sprite];
		
		if (entity->animation_flags & ANIMATION_FLAGS_playing) // ((animation_flags & (ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat)) == (ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat))
		{
			if (core->run_data->world.elapsed_world_time >= entity->frame_start_time + dynamic_sprite->frame_interval * entity->interval_mult)
			{
				// Frame is complete
				if (entity->animation_flags & ANIMATION_FLAGS_reversing)
				{
					if (entity->current_frame == 0)
					{
						if (entity->animation_flags & ANIMATION_FLAGS_repeat)
						{
							entity->current_frame = dynamic_sprite->frames - 1;
						}
						else
						{
							entity->animation_flags &= ~ANIMATION_FLAGS_playing;
						}
					}
					else
					{
						entity->current_frame--;
					}
				}
				else
				{
					if (entity->current_frame < dynamic_sprite->frames - 1)
					{
						entity->current_frame++;
					}
					else
					{
						if (entity->animation_flags & ANIMATION_FLAGS_repeat)
						{
							entity->current_frame = 0;
						}
						else
						{
							entity->animation_flags &= ~ANIMATION_FLAGS_playing;
						}
					}
				}
				
				entity->frame_start_time = core->run_data->world.elapsed_world_time;
			}
		}
	}
}

internal void RenderSprites()
{
	// NOTE(randy): Add all sprite components to queue
	for (i32 i = 0; i < core->run_data->entity_count; i++)
	{
		Entity *entity = &core->run_data->entities[i];
		if ((entity->flags & ENTITY_FLAGS_sprite) == 0)
		{
			continue;
		}
		
		if (entity->sprite_data.dynamic_sprite)
		{
			DynamicSpriteData *dynamic_sprite = &global_dynamic_sprite_data[entity->sprite_data.dynamic_sprite];
			
			Assert(entity->flags & ENTITY_FLAGS_animation);
			
			v2 render_size = v2zoom(v2(dynamic_sprite->source.z * entity->sprite_data.scale.x * (entity->is_flipped ? -1.0f : 1.0f), dynamic_sprite->source.w * entity->sprite_data.scale.y));
			
			// NOTE(randy): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(entity->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((dynamic_sprite->offset.x + entity->sprite_data.offset.x) * (entity->is_flipped ? -1.0f : 1.0f), dynamic_sprite->offset.y + entity->sprite_data.offset.y)));
			
			// NOTE(randy): The X source pos is translated to the right depending on the current frame.
			v2 source_pos = v2(dynamic_sprite->source.x + dynamic_sprite->source.z * entity->current_frame, dynamic_sprite->source.y);
			
			ArcPushTexture(dynamic_sprite->texture_atlas,
						   0,
						   v4(source_pos.x, source_pos.y, dynamic_sprite->source.z - 0.5f, dynamic_sprite->source.w - 0.5f),
						   v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
						   (core->run_data->editor_state && entity == core->run_data->entity_editor.selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), entity->sprite_data.tint) : entity->sprite_data.tint),
						   entity->sprite_data.render_layer);
		}
		else if (entity->sprite_data.static_sprite)
		{
			StaticSpriteData *static_sprite = &global_static_sprite_data[entity->sprite_data.static_sprite];
			
			v2 render_size = v2zoom(v2(static_sprite->source.z * entity->sprite_data.scale.x * (entity->is_flipped ? -1.0f : 1.0f), static_sprite->source.w * entity->sprite_data.scale.y));
			
			// NOTE(randy): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(entity->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((static_sprite->offset.x + entity->sprite_data.offset.x) * (entity->is_flipped ? -1.0f : 1.0f), static_sprite->offset.y + entity->sprite_data.offset.y)));
			
			ArcPushTexture(static_sprite->texture_atlas,
						   0,
						   v4(static_sprite->source.x, static_sprite->source.y, static_sprite->source.z - 0.5f, static_sprite->source.w - 0.5f),
						   v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
						   entity->sprite_data.tint,
						   entity->sprite_data.render_layer);
		}
		else
		{
			Assert(0);
		}
	}
	
	// NOTE(randy): Sort & render everything in the queue
	for (i32 step = 0; step < core->run_data->queued_texture_count - 1; step++)
	{
		for (i32 i = 0; i < (core->run_data->queued_texture_count - step - 1); i++)
		{
			QueuedTexture *queued_texture = &core->run_data->queued_textures[i];
			if (queued_texture->layer < (queued_texture + 1)->layer)
			{
				QueuedTexture temp_texture = core->run_data->queued_textures[i];
				core->run_data->queued_textures[i] = core->run_data->queued_textures[i + 1];
				core->run_data->queued_textures[i + 1] = temp_texture;
			}
		}
	}
	
	for (i32 i = 0; i < core->run_data->queued_texture_count; i++)
	{
		QueuedTexture *queued_texture = &core->run_data->queued_textures[i];
		Ts2dPushTintedTextureWithFlags(queued_texture->texture, queued_texture->flags, queued_texture->source, queued_texture->destination, queued_texture->tint);
	}
	
	core->run_data->queued_texture_count = 0;
}

internal void ArcPushTexture(Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint, f32 layer)
{
	QueuedTexture new_texture = {
		.texture = texture,
		.flags = flags,
		.source = source,
		.destination = destination,
		.tint = tint,
		.layer = layer,
	};
	core->run_data->queued_textures[core->run_data->queued_texture_count++] = new_texture;
}