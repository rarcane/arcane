internal void InitialiseSpriteData()
{
	for (int i = 0; i < SPRITE_MAX; i++)
	{
		SpriteData *sprite = &global_sprite_data[i];
		sprite->texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite->texture_path);
	}
}

internal void UpdateAnimations()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_flipbook);)
	{
		SpriteData *dynamic_sprite = &global_sprite_data[entity->sprite_data.sprite];
		
		if (entity->animation_flags & ANIMATION_FLAGS_playing)
		{
			if (core->run_data->world_data.elapsed_world_time >= entity->frame_start_time + dynamic_sprite->frame_interval * entity->interval_mult)
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
				
				entity->frame_start_time = GetRunData()->world_data.elapsed_world_time;
			}
		}
	}
}

internal void RenderSprites()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_sprite);)
	{
		SpriteData *sprite = &global_sprite_data[entity->sprite_data.sprite];
		
		v2 render_size = v2zoom(v2(sprite->source.z * entity->sprite_data.scale.x * (entity->is_flipped ? -1.0f : 1.0f), sprite->source.w * entity->sprite_data.scale.y));
		
		v2 render_pos;
		if (EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
		{
			v2 par_amt = GetEntityParallaxAmount(entity);
			render_pos = v2view(ParallaxPosition(entity->position, par_amt));
		}
		else
		{
			render_pos = v2view(entity->position);
		}
		
		// NOTE(randy): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
		render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
		render_pos = V2AddV2(render_pos, v2zoom(v2((sprite->offset.x + entity->sprite_data.offset.x) * (entity->is_flipped ? -1.0f : 1.0f), sprite->offset.y + entity->sprite_data.offset.y)));
		
		// NOTE(randy): The X source pos is translated to the right depending on the current frame.
		v2 source_pos = v2(sprite->source.x, sprite->source.y);
		if (entity->sprite_data.sprite >= SPRITE_default_dynamic)
		{
			source_pos.x += sprite->source.z * entity->current_frame;
		}
		
		v4 tint = v4u(1.0f);
		if (entity == GetRunData()->selected_entity)
		{
			tint = v4(1.0f, 0.6f, 0.6f, 1.0f);
		}
		
		ArcPushTexture(sprite->texture_atlas,
					   0,
					   v4(source_pos.x, source_pos.y, sprite->source.z - 0.5f, sprite->source.w - 0.5f),
					   v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
					   V4MultiplyV4(entity->sprite_data.tint, tint),
					   entity->sprite_data.render_layer);
		
		if (GetRunData()->editor_state && EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
		{
			// TODO(randy): draw parallax bounds
			
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
			
			case RENDERABLE_TYPE_rect :
			{
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPushClip(queued_renderable->clip);
				}
				
				Ts2dPushRect(queued_renderable->data.rect.colour,
							 queued_renderable->data.rect.rect);
				
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
				
				Ts2dPushFilledRect(queued_renderable->data.rect.colour,
								   queued_renderable->data.rect.rect);
				
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPopClip();
				}
			} break;
			
			case RENDERABLE_TYPE_line :
			{
				if (queued_renderable->clip.z != 0.0f &&
					queued_renderable->clip.w != 0.0f)
				{
					Ts2dPushClip(queued_renderable->clip);
				}
				
				Line *line = &queued_renderable->data.line;
				Ts2dPushLine(line->colour,
							 line->p1,
							 line->p2);
				
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
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
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
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
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
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
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
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
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

internal void ArcPushRect(v4 colour, v4 rect, f32 layer)
{
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
	SortRenderable new_renderable = {
		.data = {
			.rect = {
				.colour = colour,
				.rect = rect,
			},
		},
		.type = RENDERABLE_TYPE_rect,
		.layer = layer,
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_renderable;
}

internal void ArcPushFilledRect(v4 colour, v4 rect, f32 layer)
{
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
	SortRenderable new_renderable = {
		.data = {
			.rect = {
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
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
	SortRenderable new_renderable = {
		.data = {
			.rect = {
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

internal void ArcPushLine(v4 colour, v2 p1, v2 p2, f32 layer)
{
	Assert(core->run_data->queued_renderable_count + 1 < MAX_QUEUED_RENDERABLES);
	
	SortRenderable new_renderable = {
		.data = {
			.line = {
				.colour = colour,
				.p1 = p1,
				.p2 = p2,
			},
		},
		.type = RENDERABLE_TYPE_line,
		.layer = layer
	};
	core->run_data->queued_renderables[core->run_data->queued_renderable_count++] = new_renderable;
}