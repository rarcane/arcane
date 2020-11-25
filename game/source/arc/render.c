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
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_flipbook);)
	{
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
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_sprite);)
	{
		if (entity->sprite_data.dynamic_sprite)
		{
			DynamicSpriteData *dynamic_sprite = &global_dynamic_sprite_data[entity->sprite_data.dynamic_sprite];
			
			Assert(EntityHasProperty(entity, ENTITY_PROPERTY_flipbook));
			
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
	for (i32 step = 0; step < core->run_data->queued_renderable_count - 1; step++)
	{
		for (i32 i = 0; i < (core->run_data->queued_renderable_count - step - 1); i++)
		{
			SortRenderable *queued_texture = &core->run_data->queued_renderables[i];
			if (queued_texture->layer < (queued_texture + 1)->layer)
			{
				SortRenderable temp_renderable = core->run_data->queued_renderables[i];
				core->run_data->queued_renderables[i] = core->run_data->queued_renderables[i + 1];
				core->run_data->queued_renderables[i + 1] = temp_renderable;
			}
		}
	}
	
	for (i32 i = 0; i < core->run_data->queued_renderable_count; i++)
	{
		SortRenderable *queued_renderable = &core->run_data->queued_renderables[i];
		switch (queued_renderable->type)
		{
			case RENDERABLE_TYPE_texture :
			{
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPushClip(queued_renderable->clip);
				}
				
				Ts2dPushTintedTextureWithFlags(queued_renderable->data.texture.texture,
											   queued_renderable->data.texture.flags,
											   queued_renderable->data.texture.source,
											   queued_renderable->data.texture.destination,
											   queued_renderable->data.texture.tint);
				
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPopClip();
				}
			} break;
			
			case RENDERABLE_TYPE_text :
			{
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPushClip(queued_renderable->clip);
				}
				
				Ts2dPushText(queued_renderable->data.text.font,
							 queued_renderable->data.text.flags,
							 queued_renderable->data.text.colour,
							 queued_renderable->data.text.pos,
							 queued_renderable->data.text.font_scale,
							 queued_renderable->data.text.text);
				
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPopClip();
				}
			} break;
			
			case RENDERABLE_TYPE_filled_rect :
			{
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPushClip(queued_renderable->clip);
				}
				
				Ts2dPushFilledRect(queued_renderable->data.filled_rect.colour,
								   queued_renderable->data.filled_rect.rect);
				
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPopClip();
				}
			} break;
			
			default :
			Assert(0);
			break;
		}
	}
	
	core->run_data->queued_renderable_count = 0;
}

internal void ArcPushText(Ts2dFont *font, i32 flags, v4 colour, v2 pos, f32 font_scale, char *text, f32 layer)
{
	SortRenderable new_text = {
		.data = {
			.text = {
				.font = font,
				.flags = flags,
				.colour = colour,
				.pos = pos,
				.font_scale = font_scale,
			},
		},
		.type = RENDERABLE_TYPE_text,
		.layer = layer,
	};
	strcpy(new_text.data.text.text, text);
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_text;
}

internal void ArcPushTextWithClip(Ts2dFont *font, i32 flags, v4 colour, v2 pos, f32 font_scale, char *text, f32 layer, v4 clip)
{
	SortRenderable new_text = {
		.data = {
			.text = {
				.font = font,
				.flags = flags,
				.colour = colour,
				.pos = pos,
				.font_scale = font_scale,
			},
		},
		.type = RENDERABLE_TYPE_text,
		.layer = layer,
		.clip = clip,
	};
	strcpy(new_text.data.text.text, text);
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_text;
}

internal void ArcPushTexture(Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint, f32 layer)
{
	SortRenderable new_texture = {
		.data = {
			.texture = {
				.texture = texture,
				.flags = flags,
				.source = source,
				.destination = destination,
				.tint = tint,
			},
		},
		.type = RENDERABLE_TYPE_texture,
		.layer = layer,
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_texture;
}

internal void ArcPushTextureWithClip(Ts2dTexture *texture_atlas, i32 flags, v4 source, v4 destination, v4 tint, f32 layer, v4 clip)
{
	SortRenderable new_texture = {
		.data = {
			.texture = {
				.texture = texture_atlas,
				.flags = flags,
				.source = source,
				.destination = destination,
				.tint = tint,
			},
		},
		.type = RENDERABLE_TYPE_texture,
		.layer = layer,
		.clip = clip,
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_texture;
}

internal void ArcPushFilledRect(v4 colour, v4 rect, f32 layer)
{
	SortRenderable new_renderable = {
		.data = {
			.filled_rect = {
				.colour = colour,
				.rect = rect,
			},
		},
		.type = RENDERABLE_TYPE_filled_rect,
		.layer = layer,
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_renderable;
}

internal void ArcPushFilledRectWithClip(v4 colour, v4 rect, f32 layer, v4 clip)
{
	SortRenderable new_renderable = {
		.data = {
			.filled_rect = {
				.colour = colour,
				.rect = rect,
			},
		},
		.type = RENDERABLE_TYPE_filled_rect,
		.layer = layer,
		.clip = clip,
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_renderable;
}