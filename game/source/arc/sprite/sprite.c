
internal void InitialiseSpriteData()
{
	for (int i = 0; i < STATIC_SPRITE_MAX; i++)
	{
		StaticSpriteData *sprite = &static_sprite_data[i];
		sprite->texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite->texture_path);
	}

	for (int i = 0; i < DYNAMIC_SPRITE_MAX; i++)
	{
		DynamicSpriteData *sprite = &dynamic_sprite_data[i];
		sprite->texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite->texture_path);
	}
}

internal void UpdateAnimations()
{
	for (int j = 0; j < core->world_data->entity_components.animation_component_count; j++)
	{
		AnimationComponent *animation_comp = &core->world_data->entity_components.animation_components[j];
		if (animation_comp->component_id)
		{
			Entity *entity = &core->world_data->entities[animation_comp->parent_entity_id - 1];
			SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(entity->entity_id);

			DynamicSpriteData *dynamic_sprite = &dynamic_sprite_data[sprite_comp->sprite_data.dynamic_sprite];

			b8 animation_flags = animation_comp->flags;
			if (animation_flags & ANIMATION_FLAGS_playing) // ((animation_flags & (ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat)) == (ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat))
			{
				if (core->world_data->elapsed_world_time >= animation_comp->frame_start_time + dynamic_sprite->frame_interval * animation_comp->interval_mult)
				{
					// Frame is complete
					if (animation_flags & ANIMATION_FLAGS_reversing)
					{
						if (animation_comp->current_frame == 0)
						{
							if (animation_flags & ANIMATION_FLAGS_repeat)
							{
								animation_comp->current_frame = dynamic_sprite->frames - 1;
							}
							else
							{
								animation_comp->flags &= ~ANIMATION_FLAGS_playing;
							}
						}
						else
						{
							animation_comp->current_frame--;
						}
					}
					else
					{
						if (animation_comp->current_frame < dynamic_sprite->frames - 1)
						{
							animation_comp->current_frame++;
						}
						else
						{
							if (animation_flags & ANIMATION_FLAGS_repeat)
							{
								animation_comp->current_frame = 0;
							}
							else
							{
								animation_comp->flags &= ~ANIMATION_FLAGS_playing;
							}
						}
					}

					animation_comp->frame_start_time = core->world_data->elapsed_world_time;
				}
			}
		}
	}
}

internal void RenderBackgroundSprites()
{
	typedef struct SpriteRenderable
	{
		Entity *entity;
		StaticSprite static_sprite;
		DynamicSprite dynamic_sprite;
		v2 offset;
		f32 render_layer;
		v2 scale;
		b8 is_flipped;
		v4 tint;
		PositionComponent *position_comp;
		AnimationComponent *animation_comp;
	} SpriteRenderable;

	SpriteRenderable ordered_sprites[MAX_ACTIVE_ENTITIES * MAX_SUB_SPRITES] = {0};
	i32 ordered_sprite_count = 0;

	for (int j = 0; j < core->world_data->entity_components.sprite_component_count; j++)
	{
		SpriteComponent *sprite_component = &core->world_data->entity_components.sprite_components[j];
		if (sprite_component->component_id && sprite_component->is_background_sprite) // Validate entity & check if it's a background sprite
		{
			Entity *entity = GetEntityWithID(sprite_component->parent_entity_id);

			SpriteRenderable sprite_renderable = {
				entity,
				sprite_component->sprite_data.static_sprite,
				sprite_component->sprite_data.dynamic_sprite,
				sprite_component->sprite_data.offset,
				sprite_component->sprite_data.render_layer,
				sprite_component->sprite_data.scale,
				sprite_component->is_flipped,
				sprite_component->sprite_data.tint,
				0,
				0,
			};
			sprite_renderable.position_comp = GetPositionComponentFromEntityID(entity->entity_id);
			sprite_renderable.animation_comp = GetAnimationComponentFromEntityID(entity->entity_id);

			ordered_sprites[ordered_sprite_count++] = sprite_renderable;
		}
	}

	// Bubble sort via the layer_order. Lowest layer_order is rendered in front.
	for (int step = 0; step < ordered_sprite_count - 1; step++)
	{
		for (int i = 0; i < (ordered_sprite_count - step - 1); i++)
		{
			if (ordered_sprites[i].render_layer < ordered_sprites[i + 1].render_layer)
			{
				SpriteRenderable temp_sprite = ordered_sprites[i];
				ordered_sprites[i] = ordered_sprites[i + 1];
				ordered_sprites[i + 1] = temp_sprite;
			}
		}
	}

	// Push sprites in newly arranged order.
	for (int i = 0; i < ordered_sprite_count; i++)
	{
		if (ordered_sprites[i].dynamic_sprite)
		{
			DynamicSpriteData *dynamic_sprite = &dynamic_sprite_data[ordered_sprites[i].dynamic_sprite];

			AnimationComponent *animation_comp = ordered_sprites[i].animation_comp;
			R_DEV_ASSERT(animation_comp, "Animation component does not exist, even though a DynamicSprite is being used.");

			v2 render_size = v2zoom(v2(dynamic_sprite->source.z * ordered_sprites[i].scale.x * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), dynamic_sprite->source.w * ordered_sprites[i].scale.y));

			// NOTE(tjr): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(ordered_sprites[i].position_comp->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((dynamic_sprite->offset.x + ordered_sprites[i].offset.x) * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), dynamic_sprite->offset.y + ordered_sprites[i].offset.y)));

			// NOTE(tjr): The X source pos is translated to the right depending on the current frame.
			v2 source_pos = v2(dynamic_sprite->source.x + dynamic_sprite->source.z * animation_comp->current_frame, dynamic_sprite->source.y);

			Ts2dPushTintedTexture(dynamic_sprite->texture_atlas,
								  v4(source_pos.x, source_pos.y, dynamic_sprite->source.z - 0.5f, dynamic_sprite->source.w - 0.5f),
								  v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
								  (core->run_data->editor_state && ordered_sprites[i].entity == core->run_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else if (ordered_sprites[i].static_sprite)
		{
			StaticSpriteData *static_sprite = &static_sprite_data[ordered_sprites[i].static_sprite];

			AnimationComponent *animation_comp = ordered_sprites[i].animation_comp;
			R_DEV_ASSERT(!animation_comp, "There should be no animation component if this sprite is static.");

			v2 render_size = v2zoom(v2(static_sprite->source.z * ordered_sprites[i].scale.x * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), static_sprite->source.w * ordered_sprites[i].scale.y));

			// NOTE(tjr): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(ordered_sprites[i].position_comp->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((static_sprite->offset.x + ordered_sprites[i].offset.x) * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), static_sprite->offset.y + ordered_sprites[i].offset.y)));

			Ts2dPushTintedTexture(static_sprite->texture_atlas,
								  v4(static_sprite->source.x, static_sprite->source.y, static_sprite->source.z - 0.5f, static_sprite->source.w - 0.5f),
								  v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
								  (core->run_data->editor_state && ordered_sprites[i].entity == core->run_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else
		{
			R_BREAK("Naaa fam.");
		}
	}
}

internal void RenderForegroundSprites()
{
	typedef struct SpriteRenderable
	{
		Entity *entity;
		StaticSprite static_sprite;
		DynamicSprite dynamic_sprite;
		v2 offset;
		f32 render_layer;
		v2 scale;
		b8 is_flipped;
		v4 tint;
		PositionComponent *position_comp;
		AnimationComponent *animation_comp;
	} SpriteRenderable;

	SpriteRenderable ordered_sprites[MAX_ACTIVE_ENTITIES * MAX_SUB_SPRITES] = {0};
	i32 ordered_sprite_count = 0;

	for (int j = 0; j < core->world_data->entity_components.sprite_component_count; j++)
	{
		SpriteComponent *sprite_component = &core->world_data->entity_components.sprite_components[j];
		if (sprite_component->component_id && !sprite_component->is_background_sprite) // Validate entity & check if it's not a background sprite
		{
			Entity *entity = GetEntityWithID(sprite_component->parent_entity_id);

			SpriteRenderable sprite_renderable = {
				entity,
				sprite_component->sprite_data.static_sprite,
				sprite_component->sprite_data.dynamic_sprite,
				sprite_component->sprite_data.offset,
				sprite_component->sprite_data.render_layer,
				sprite_component->sprite_data.scale,
				sprite_component->is_flipped,
				sprite_component->sprite_data.tint,
				0,
				0,
			};
			sprite_renderable.position_comp = GetPositionComponentFromEntityID(entity->entity_id);
			sprite_renderable.animation_comp = GetAnimationComponentFromEntityID(entity->entity_id);

			ordered_sprites[ordered_sprite_count++] = sprite_renderable;
		}
	}

	// Bubble sort via the layer_order. Lowest layer_order is rendered in front.
	for (int step = 0; step < ordered_sprite_count - 1; step++)
	{
		for (int i = 0; i < (ordered_sprite_count - step - 1); i++)
		{
			if (ordered_sprites[i].render_layer < ordered_sprites[i + 1].render_layer)
			{
				SpriteRenderable temp_sprite = ordered_sprites[i];
				ordered_sprites[i] = ordered_sprites[i + 1];
				ordered_sprites[i + 1] = temp_sprite;
			}
		}
	}

	// Push sprites in newly arranged order.
	for (int i = 0; i < ordered_sprite_count; i++)
	{
		if (ordered_sprites[i].dynamic_sprite)
		{
			DynamicSpriteData *dynamic_sprite = &dynamic_sprite_data[ordered_sprites[i].dynamic_sprite];

			AnimationComponent *animation_comp = ordered_sprites[i].animation_comp;
			R_DEV_ASSERT(animation_comp, "Animation component does not exist, even though a DynamicSprite is being used.");

			v2 render_size = v2zoom(v2(dynamic_sprite->source.z * ordered_sprites[i].scale.x * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), dynamic_sprite->source.w * ordered_sprites[i].scale.y));

			// NOTE(tjr): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(ordered_sprites[i].position_comp->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((dynamic_sprite->offset.x + ordered_sprites[i].offset.x) * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), dynamic_sprite->offset.y + ordered_sprites[i].offset.y)));

			// NOTE(tjr): The X source pos is translated to the right depending on the current frame.
			v2 source_pos = v2(dynamic_sprite->source.x + dynamic_sprite->source.z * animation_comp->current_frame, dynamic_sprite->source.y);

			Ts2dPushTintedTexture(dynamic_sprite->texture_atlas,
								  v4(source_pos.x, source_pos.y, dynamic_sprite->source.z - 0.5f, dynamic_sprite->source.w - 0.5f),
								  v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
								  (core->run_data->editor_state && ordered_sprites[i].entity == core->run_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else if (ordered_sprites[i].static_sprite)
		{
			StaticSpriteData *static_sprite = &static_sprite_data[ordered_sprites[i].static_sprite];

			AnimationComponent *animation_comp = ordered_sprites[i].animation_comp;
			R_DEV_ASSERT(!animation_comp, "There should be no animation component if this sprite is static.");

			v2 render_size = v2zoom(v2(static_sprite->source.z * ordered_sprites[i].scale.x * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), static_sprite->source.w * ordered_sprites[i].scale.y));

			// NOTE(tjr): Determine proper position of the Sprite - Offset is defaulted to BottomCentre
			v2 render_pos = v2view(ordered_sprites[i].position_comp->position);
			render_pos = V2AddV2(render_pos, v2(render_size.x / -2.0f, -render_size.y));
			render_pos = V2AddV2(render_pos, v2zoom(v2((static_sprite->offset.x + ordered_sprites[i].offset.x) * (ordered_sprites[i].is_flipped ? -1.0f : 1.0f), static_sprite->offset.y + ordered_sprites[i].offset.y)));

			Ts2dPushTintedTexture(static_sprite->texture_atlas,
								  v4(static_sprite->source.x, static_sprite->source.y, static_sprite->source.z - 0.5f, static_sprite->source.w - 0.5f),
								  v4(render_pos.x, render_pos.y, render_size.x, render_size.y),
								  (core->run_data->editor_state && ordered_sprites[i].entity == core->run_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else
		{
			R_BREAK("Naaa fam.");
		}
	}
}