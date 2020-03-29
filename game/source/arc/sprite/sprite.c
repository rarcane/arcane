
internal void
InitialiseSpriteData()
{
	static_sprite_data[INVALID_SPRITE].texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, "invalid_texture");
	static_sprite_data[INVALID_SPRITE].source = v4(0.0f, 0.0f, 64.0f, 64.0f);
	static_sprite_data[INVALID_SPRITE].offset = v2(0.0f, 0.0f);

#define StaticSprite(sprite_name, sprite_atlas_path, sprite_source, sprite_offset)                                                             \
	{                                                                                                                                          \
		static_sprite_data[STATIC_SPRITE_##sprite_name].texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite_atlas_path); \
		static_sprite_data[STATIC_SPRITE_##sprite_name].source = sprite_source;                                                                \
		static_sprite_data[STATIC_SPRITE_##sprite_name].offset = sprite_offset;                                                                \
		R_DEV_ASSERT(static_sprite_data[STATIC_SPRITE_##sprite_name].texture_atlas, "Failed to load texture atlas %s", sprite_atlas_path);     \
	}
#include "sprites_static.inc"
#undef StaticSprite
#define DynamicSprite(sprite_name, sprite_atlas_path, sprite_source, sprite_offset, sprite_frames, sprite_frame_interval)                                                \
	{                                                                                                                                                                    \
		dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].texture_atlas = TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dTexture, sprite_atlas_path); \
		dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].source = sprite_source;                                                                \
		dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].offset = sprite_offset;                                                                \
		dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].frames = sprite_frames;                                                                \
		dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].frame_interval = sprite_frame_interval;                                                \
		R_DEV_ASSERT(dynamic_sprite_data[DYNAMIC_SPRITE_##sprite_name - STATIC_SPRITE_MAX - 1].texture_atlas, "Failed to load texture atlas %s", sprite_atlas_path);     \
	}
#include "sprites_dynamic.inc"
#undef DynamicSprite

	// Temporary socket positioning
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[0].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[0].position = v2(-5.0f, -12.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[1].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[1].position = v2(-5.0f, -12.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[2].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[2].position = v2(-5.0f, -12.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[3].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_idle_arm - STATIC_SPRITE_MAX - 1].sockets[3].position = v2(-5.0f, -12.0);

	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[0].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[0].position = v2(-2.0f, -12.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[1].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[1].position = v2(3.0f, -12.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[2].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[2].position = v2(5.0f, -15.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[3].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[3].position = v2(6.0f, -17.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[4].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[4].position = v2(5.0f, -14.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[5].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[5].position = v2(4.0f, -13.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[6].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[6].position = v2(1.0f, -13.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[7].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_walking_arm - STATIC_SPRITE_MAX - 1].sockets[7].position = v2(0.0f, -13.0);

	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[0].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[0].position = v2(-9.0f, -19.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[1].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[1].position = v2(-3.0f, -14.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[2].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[2].position = v2(10.0f, -20.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[3].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[3].position = v2(9.0f, -21.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[4].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[4].position = v2(9.0f, -22.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[5].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[5].position = v2(2.0f, -16.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[6].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[6].position = v2(-3.0f, -18.0);
	strcpy(dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[7].name, "Hand");
	dynamic_sprite_data[DYNAMIC_SPRITE_player_sprinting_arm - STATIC_SPRITE_MAX - 1].sockets[7].position = v2(-11.0f, -22.0);
}

internal b8 IsSpriteDynamic(SpriteType sprite_enum)
{
	return (sprite_enum > STATIC_SPRITE_MAX);
}

internal StaticSprite *GetStaticSprite(SpriteType static_sprite_enum)
{
	R_DEV_ASSERT(static_sprite_enum < STATIC_SPRITE_MAX, "Must be within range of the static sprites.");
	return &static_sprite_data[static_sprite_enum];
}

internal DynamicSprite *GetDynamicSprite(SpriteType dynamic_sprite_enum)
{
	R_DEV_ASSERT(dynamic_sprite_enum < DYNAMIC_SPRITE_MAX && dynamic_sprite_enum > STATIC_SPRITE_MAX, "Must be within range of the dynamic sprites.");
	return &dynamic_sprite_data[dynamic_sprite_enum - STATIC_SPRITE_MAX - 1];
}

internal SpriteData *GetSubSpriteByType(SubSpriteComponent *sub_sprite_comp, SpriteType sprite_enum)
{
	for (int i = 0; i < sub_sprite_comp->sub_sprite_count; i++)
	{
		if (sub_sprite_comp->sub_sprites[i].sprite_enum == sprite_enum)
			return &sub_sprite_comp->sub_sprites[i];
	}

	R_BREAK("Couldn't find subsprite");
	return 0;
}

internal void UpdateAnimations()
{
	for (int j = 0; j < core->world_data->entity_components.animation_component_count; j++)
	{
		AnimationComponent *animation_comp = &core->world_data->entity_components.animation_components[j];
		if (animation_comp->parent_entity)
		{
			Entity *entity = animation_comp->parent_entity;
			SpriteComponent *sprite_comp = entity->components[COMPONENT_sprite];
			SubSpriteComponent *sub_sprite_comp = entity->components[COMPONENT_sub_sprite];

			DynamicSprite *dynamic_sprites[MAX_SUB_SPRITES] = {0};
			i32 dynamic_sprite_count = 0;
			if (sprite_comp)
			{
				dynamic_sprites[dynamic_sprite_count++] = GetDynamicSprite(sprite_comp->sprite_data.sprite_enum);
			}
			else if (sub_sprite_comp)
			{
				for (int k = 0; k < sub_sprite_comp->sub_sprite_count; k++)
				{
					dynamic_sprites[dynamic_sprite_count++] = GetDynamicSprite(sub_sprite_comp->sub_sprites[k].sprite_enum);
				}
			}
			else
			{
				R_BREAK("No valid sprite component attached.");
			}

			for (int k = 0; k < dynamic_sprite_count; k++)
			{
				DynamicSprite *dynamic_sprite = dynamic_sprites[k];

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
}

internal void RenderBackgroundSprites()
{
	typedef struct SpriteRenderable
	{
		Entity *entity;
		SpriteType sprite_enum;
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
		if (sprite_component->parent_entity && sprite_component->is_background_sprite) // Validate entity & check if it's a background sprite
		{
			Entity *entity = sprite_component->parent_entity;

			SpriteRenderable sprite_renderable = {
				entity,
				sprite_component->sprite_data.sprite_enum,
				sprite_component->sprite_data.offset,
				sprite_component->sprite_data.render_layer,
				sprite_component->sprite_data.scale,
				sprite_component->is_flipped,
				sprite_component->sprite_data.tint,
				entity->components[COMPONENT_position],
				entity->components[COMPONENT_animation],
			};

			ordered_sprites[ordered_sprite_count++] = sprite_renderable;
		}
	}

	for (int j = 0; j < core->world_data->entity_components.sub_sprite_component_count; j++)
	{
		SubSpriteComponent *sub_sprite_component = &core->world_data->entity_components.sub_sprite_components[j];
		if (sub_sprite_component->parent_entity) // Validate entity
		{
			Entity *entity = sub_sprite_component->parent_entity;

			for (int j = 0; j < sub_sprite_component->sub_sprite_count; j++)
			{
				SpriteRenderable sprite_renderable = {
					entity,
					sub_sprite_component->sub_sprites[j].sprite_enum,
					sub_sprite_component->sub_sprites[j].offset,
					sub_sprite_component->sub_sprites[j].render_layer,
					sub_sprite_component->sub_sprites[j].scale,
					sub_sprite_component->is_flipped,
					sub_sprite_component->sub_sprites[j].tint,
					entity->components[COMPONENT_position],
					entity->components[COMPONENT_animation],
				};

				ordered_sprites[ordered_sprite_count++] = sprite_renderable;
			}
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
		if (IsSpriteDynamic(ordered_sprites[i].sprite_enum))
		{
			DynamicSprite *dynamic_sprite = GetDynamicSprite(ordered_sprites[i].sprite_enum);

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
								  (core->client_data->editor_state && ordered_sprites[i].entity == core->client_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else
		{
			StaticSprite *static_sprite = GetStaticSprite(ordered_sprites[i].sprite_enum);

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
								  (core->client_data->editor_state && ordered_sprites[i].entity == core->client_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
	}
}

internal void RenderForegroundSprites()
{
	typedef struct SpriteRenderable
	{
		Entity *entity;
		SpriteType sprite_enum;
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
		if (sprite_component->parent_entity && !sprite_component->is_background_sprite) // Validate entity & check if it's a foreground sprite
		{
			Entity *entity = sprite_component->parent_entity;

			SpriteRenderable sprite_renderable = {
				entity,
				sprite_component->sprite_data.sprite_enum,
				sprite_component->sprite_data.offset,
				sprite_component->sprite_data.render_layer,
				sprite_component->sprite_data.scale,
				sprite_component->is_flipped,
				sprite_component->sprite_data.tint,
				entity->components[COMPONENT_position],
				entity->components[COMPONENT_animation],
			};

			ordered_sprites[ordered_sprite_count++] = sprite_renderable;
		}
	}

	for (int j = 0; j < core->world_data->entity_components.sub_sprite_component_count; j++)
	{
		SubSpriteComponent *sub_sprite_component = &core->world_data->entity_components.sub_sprite_components[j];
		if (sub_sprite_component->parent_entity) // Validate entity
		{
			Entity *entity = sub_sprite_component->parent_entity;

			for (int j = 0; j < sub_sprite_component->sub_sprite_count; j++)
			{
				SpriteRenderable sprite_renderable = {
					entity,
					sub_sprite_component->sub_sprites[j].sprite_enum,
					sub_sprite_component->sub_sprites[j].offset,
					sub_sprite_component->sub_sprites[j].render_layer,
					sub_sprite_component->sub_sprites[j].scale,
					sub_sprite_component->is_flipped,
					sub_sprite_component->sub_sprites[j].tint,
					entity->components[COMPONENT_position],
					entity->components[COMPONENT_animation],
				};

				ordered_sprites[ordered_sprite_count++] = sprite_renderable;
			}
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
		if (IsSpriteDynamic(ordered_sprites[i].sprite_enum))
		{
			DynamicSprite *dynamic_sprite = GetDynamicSprite(ordered_sprites[i].sprite_enum);

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
								  (core->client_data->editor_state && ordered_sprites[i].entity == core->client_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
		else
		{
			StaticSprite *static_sprite = GetStaticSprite(ordered_sprites[i].sprite_enum);

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
								  (core->client_data->editor_state && ordered_sprites[i].entity == core->client_data->selected_entity ? V4MultiplyV4(v4(1.0f, 0.8f, 0.8f, 1.0f), ordered_sprites[i].tint) : ordered_sprites[i].tint));
		}
	}
}