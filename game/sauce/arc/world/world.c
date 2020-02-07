internal void TempInitGameWorld()
{
	core->is_ingame = 1;

	{
		CharacterEntity *character = InitialiseCharacterEntity();
		character->parent_generic_entity->flags |= ENTITY_FLAGS_no_delete;
		character->position_comp->position = v2(0.0f, 0.0f);
		character->collider_comp->shape = GetRectangleShape(v2(14.0f, 35.0f), v2(0.0f, 0.0f));
		character->collider_comp->flags = COLLIDER_FLAGS_player;
		character->velocity_comp->acceleration = v2(250.0f, 0.0f);
		character->velocity_comp->collide_against = COLLIDER_FLAGS_ground;
		character->movement_comp->move_speed = 100.0f;
		character->arc_entity_comp->entity_type = ARC_ENTITY_player;
		character->arc_entity_comp->current_animation_state = ANIMATION_STATE_player_idle;
		character->sub_sprite_comp->sub_sprites[0].sprite_enum = arc_entity_animation_state_data[ANIMATION_STATE_player_idle].dynamic_sprites[0];
		character->sub_sprite_comp->sub_sprites[0].render_layer = 0.0f;
		character->sub_sprite_comp->sub_sprites[1].sprite_enum = arc_entity_animation_state_data[ANIMATION_STATE_player_idle].dynamic_sprites[1];
		character->sub_sprite_comp->sub_sprites[1].render_layer = -0.05f;
		character->sub_sprite_comp->sub_sprite_count = 2;
	}

	{
		core->sword = NewEntity("Sword", ENTITY_TYPE_generic, GENERALISED_ENTITY_TYPE_item);
		core->sword->flags |= ENTITY_FLAGS_no_delete;
		ItemComponent *sword_item = AddItemComponent(core->sword);
		sword_item->item_type = ITEM_flint_sword;
		sword_item->stack_size = 1;

		core->backpack = NewEntity("Backpack", ENTITY_TYPE_generic, GENERALISED_ENTITY_TYPE_storage);
		core->backpack->flags |= ENTITY_FLAGS_no_delete;
		StorageComponent *backpack_storage = AddStorageComponent(core->backpack);
		backpack_storage->storage_size = 9;

		core->hotbar = NewEntity("Hotbar", ENTITY_TYPE_generic, GENERALISED_ENTITY_TYPE_storage); // Hotbar should technically be attached to the player entity
		core->hotbar->flags |= ENTITY_FLAGS_no_delete;
		StorageComponent *hotbar_storage = AddStorageComponent(core->hotbar);
		hotbar_storage->storage_size = 2;

		AddItemToStorage(sword_item, backpack_storage, 2);
	}

	for (int i = -10; i < 11; i++)
	{
		GroundEntity *ground = NewGroundEntity();
		ground->position_comp->position = v2(i * 100.0f, 0.0f);
		// ground->sprite_comp->sprite_data.sprite_enum = STATIC_SPRITE_ground_forest_flat;
		// ground->sprite_comp->sprite_data.render_layer = -1.0f;
		ground->collider_comp->shape = GetRectangleShape(v2(100.0f, 87.0f), v2(0.0f, 87.0f));
		ground->collider_comp->flags = COLLIDER_FLAGS_ground;
		ground->physics_comp->bounce_mult = 1.0f;
	}

	{
		PixelClusterEntity *pixel_cluster = NewPixelClusterEntity();
		pixel_cluster->position_comp->position = v2(0.0f, -100.0f);

		LoadPixelClusterFromPNG(pixel_cluster, "texture/scenic/rock");
		UpdatePixelClusterTexture(pixel_cluster);
	}

	srand(5);

	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			core->random_field[j][i].x = (RandomI32(0, 1) ? -1.0f : 1.0f);
			core->random_field[i][j].y = (RandomI32(0, 1) ? -1.0f : 1.0f);
		}
	}

	f32 pixel_cluster_noise[64][64];

	i32 octaves = 8;

	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++)
		{
			f32 frequency = 1.0f;
			f32 amplitude = 1.0f;
			f32 max_value = 0.0f;

			f32 noise_amount = 0.0f;
			for (int k = 0; k < octaves; k++)
			{
				noise_amount += GenerateNoise((j / 64.0f) * frequency, (i / 64.0f) * frequency, frequency) * amplitude;
				max_value += amplitude;
				frequency *= 2.0f;
				amplitude *= 0.5f;
			}

			pixel_cluster_noise[i][j] = noise_amount / max_value;
		}

	for (int h = 0; h < 5; h++)
	{
		for (int i = -10; i < 11; i++)
		{
			PixelClusterEntity *pixel_cluster = NewPixelClusterEntity();
			pixel_cluster->position_comp->position = v2(i * (f32)MAX_PIXEL_CLUSTER_LENGTH, h * (f32)MAX_PIXEL_CLUSTER_LENGTH);

			for (int j = 0; j < MAX_PIXEL_CLUSTER_LENGTH; j++)
			{
				for (int k = 0; k < MAX_PIXEL_CLUSTER_LENGTH; k++)
				{
					pixel_cluster->pixels[k + MAX_PIXEL_CLUSTER_LENGTH * j].position = v2(i * MAX_PIXEL_CLUSTER_LENGTH + (f32)k, (f32)j);
					pixel_cluster->pixels[k + MAX_PIXEL_CLUSTER_LENGTH * j].colour = v4(121.0f / 255.0f + RandomF32(0.0f, 0.1f),
																						87.0f / 255.0f + RandomF32(0.0f, 0.1f),
																						70.0f / 255.0f + RandomF32(0.0f, 0.1f),
																						1.0f);
				}
			}

			UpdatePixelClusterTexture(pixel_cluster);
		}
	}

	{
		PixelClusterEntity *pixel_cluster = &core->world_data->pixel_cluster_entities[10];

		for (int j = 0; j < MAX_PIXEL_CLUSTER_LENGTH; j++)
		{
			for (int k = 0; k < MAX_PIXEL_CLUSTER_LENGTH; k++)
			{
				pixel_cluster->pixels[k + MAX_PIXEL_CLUSTER_LENGTH * j].colour = v4((pixel_cluster_noise[j][k] + 1) / 2.0f,
																					(pixel_cluster_noise[j][k] + 1) / 2.0f,
																					(pixel_cluster_noise[j][k] + 1) / 2.0f,
																					1.0f);
			}
		}

		UpdatePixelClusterTexture(pixel_cluster);
	}

	/* for (int i = 0; i < 12; i++)
	{
		Entity *cloud = NewEntity("Cloud", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(cloud,
							  v2(-300.0f + i * 50.0f, RandomF32(-87.0f, -105.0f)),
							  STATIC_SPRITE_cloud_v1 + RandomI32(0, 5),
							  RandomI32(0, 2) != 0 ? 8.5f : 9.5f,
							  v2(0.95f, 0.7f));

		SpriteComponent *sprite_comp = cloud->components[COMPONENT_sprite];
		sprite_comp->sprite_data.tint = v4u(0.5f);

		core->clouds[core->cloud_count++] = cloud;
	} */
}

internal void DrawWorld()
{
	Ts2dPushBackgroundBegin(core->renderer);
	{
		v4 top_sky_colour = v4(88.0f / 255.0f, 152.0f / 255.0f, 194.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(203.0f / 255.0f, 232.0f / 255.0f, 238.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(130.0f / 255.0f, 81.0f / 255.0f, 77.0f / 255.0f, 1.0f);

		Ts2dPushFilledVertexColoredRect(
			core->renderer,
			top_sky_colour,
			middle_sky_colour,
			top_sky_colour,
			middle_sky_colour,
			v4(0.0f, 0.0f, core->renderer->render_width, core->render_height / 2.0f));

		Ts2dPushFilledVertexColoredRect(
			core->renderer,
			middle_sky_colour,
			bottom_sky_colour,
			middle_sky_colour,
			bottom_sky_colour,
			v4(0.0f, core->render_height / 2.0f, core->renderer->render_width, core->render_height / 2.0f));

		RenderBackgroundSprites();
	}
	Ts2dPushBackgroundEnd(core->renderer);

	Ts2dWorldInfo world_info = {0};
	{
		world_info.flags |= TS2D_BLUR_SHADOWS;
		world_info.flags |= TS2D_BLUR_BACKGROUNDS;
		if (core->bloom)
			world_info.flags |= TS2D_BLOOM;
		world_info.shadow_opacity = core->shadow_opacity;
		world_info.shadow_vector = v2(32.0f, 64.0f);
		world_info.camera_pos = core->camera_position;
		world_info.camera_zoom = core->camera_zoom;
		world_info.brightness = 1.0f;
		world_info.grayscale = 0.f;
	}

	Ts2dPushWorldBegin(core->renderer, &world_info);

	// NOTE(tjr): Sprite rendering.
	{
		UpdateAnimations();
		PostUpdateWorldAnimations();
		RenderForegroundSprites();
	}

	// NOTE(tjr): Collision bounds rendering.
	if (core->draw_colliders)
	{
		RenderColliders();
	}

	/* v2 pos = v2view(v2(-350.0f, 35.0f));
					v2 size = v2zoom(v2(700.0f, 200.0f));
					Ts2dPushReflectiveRect(
					core->renderer,
					v4(pos.x, pos.y, size.x, size.y),
					v4(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f, 0.75f); */

	Ts2dPushWorldEnd(core->renderer);

	// NOTE(tjr): Draw velocity projection.
	if (core->draw_velocity)
	{
		for (int j = 0; j < core->world_data->entity_components.velocity_component_count; j++)
		{
			VelocityComponent *velocity_comp = &core->world_data->entity_components.velocity_components[j];
			PositionComponent *PositionComponent = velocity_comp->parent_entity->components[COMPONENT_position];

			Ts2dPushLine(
				core->renderer,
				v4(1.0f, 0.0f, 0.0f, 1.0f),
				v2view(PositionComponent->position),
				v2view(V2AddV2(PositionComponent->position, V2MultiplyV2(velocity_comp->velocity, v2(0.5f, 0.5f)))));
		}
	}
}

internal void PostUpdateWorldAnimations()
{
	// NOTE(tjr): Update held item to player hand socket.
	if (core->held_item)
	{
		PositionComponent *position_comp = core->world_data->character_entity.position_comp;
		AnimationComponent *animation_comp = core->world_data->character_entity.animation_comp;
		SubSpriteComponent *sub_sprite_comp = core->world_data->character_entity.sub_sprite_comp;

		PositionComponent *held_item_pos_comp = core->held_item->components[COMPONENT_position];
		// TODO: Extract into some sort of socket finding function?
		for (int i = 0; i < sub_sprite_comp->sub_sprite_count; i++)
		{
			DynamicSprite *sprite = GetDynamicSprite(sub_sprite_comp->sub_sprites[i].sprite_enum);
			SpriteSocket *sprite_socket = &sprite->sockets[animation_comp->current_frame];

			if (strcmp(sprite_socket->name, "Hand") == 0)
				held_item_pos_comp->position = V2AddV2(position_comp->position, v2((sub_sprite_comp->is_flipped ? -sprite_socket->position.x : sprite_socket->position.x), sprite_socket->position.y));
		}

		SpriteComponent *held_item_sprite_comp = core->held_item->components[COMPONENT_sprite];
		held_item_sprite_comp->is_flipped = sub_sprite_comp->is_flipped;
	}
}

internal void UpdateParallax()
{
	for (int j = 0; j < core->world_data->entity_components.parallax_component_count; j++)
	{
		ParallaxComponent *parallax_comp = &core->world_data->entity_components.parallax_components[j];
		if (parallax_comp->parent_entity)
		{
			Entity *entity = parallax_comp->parent_entity;
			PositionComponent *position_comp = entity->components[COMPONENT_position];
			R_DEV_ASSERT(position_comp, "Parallax must be attached with a position to update.");

			// TODO: Need to find a way to centralise the desired_position of the parallax, whilst still maintaining spatial consistency across sprites
			position_comp->position.x = parallax_comp->desired_position.x + -core->camera_position.x * parallax_comp->parallax_amount.x;
			position_comp->position.y = parallax_comp->desired_position.y + (-core->camera_position.y + DEFAULT_CAMERA_OFFSET_Y) * parallax_comp->parallax_amount.y;

			// position_comp->position.x = parallax_comp->desired_position.x - (parallax_comp->desired_position.x - player_pos->position.x + core->camera_offset.x) * parallax_comp->parallax_amount.x;
			// position_comp->position.y = parallax_comp->desired_position.y - (parallax_comp->desired_position.y - player_pos->position.y + core->camera_offset.y) * parallax_comp->parallax_amount.y;
		}
	}
}

internal void UpdateClouds()
{
	/* for (int i = 0; i < core->cloud_count; i++)
	{
		Entity *cloud = core->clouds[i];
		if (cloud)
		{
			ParallaxComponent *cloud_parallax = cloud->components[COMPONENT_parallax];
			PositionComponent *cloud_pos = cloud->components[COMPONENT_position];
			cloud_parallax->desired_position.x += core->world_delta_t * 1.0f;

			f32 bound = (-core->camera_position.x + 300.0f) / cloud_parallax->parallax_amount.x; // Temp fix

			if (-core->camera_position.x + cloud_parallax->desired_position.x >= bound)
			{
				DeleteEntity(cloud);
				core->clouds[i] = 0;

				Entity *new_cloud = NewEntity("Cloud", ENTITY_TYPE_scenic);
				SetupBackgroundEntity(new_cloud,
									  v2(-300.0f, RandomF32(-55.0f, -70.0f)),
									  STATIC_SPRITE_cloud_v1 + RandomI32(0, 5),
									  RandomI32(0, 3) != 0 ? 8.5f : 9.5f,
									  v2(0.95f, 0.7f));
				SpriteComponent *sprite_comp = new_cloud->components[COMPONENT_sprite];
				sprite_comp->sprite_data.tint = v4u(0.5f);

				core->clouds[i] = new_cloud;
			}
		}
	} */
}

internal i32 FloatToChunkIndex(f32 position)
{
	return (i32)floorf(position / CHUNK_SIZE);
}

internal ChunkData *GetChunkAtPosition(v2 position)
{
	for (int i = 0; i < core->world_data->active_chunk_count; i++)
	{
		ChunkData *chunk = &core->world_data->active_chunks[i];
		if (chunk->is_valid)
		{
			if (FloatToChunkIndex(position.x) == chunk->x_index &&
				FloatToChunkIndex(position.y) == chunk->y_index)
			{
				return chunk;
			}
		}
	}

	// Chunk GenerateEntityArraysdoesn't exist yet, so just create it at this positon
	ChunkData *chunk = &core->world_data->active_chunks[core->world_data->active_chunk_count];
	chunk->is_valid = 1;
	chunk->x_index = FloatToChunkIndex(position.x);
	chunk->y_index = FloatToChunkIndex(position.y);
	core->world_data->active_chunk_count++;
	return chunk;
}

// NOTE(tjr): Returns 9 surrounding chunks (including the central chunk) in relation to a position
internal void GetSurroundingChunks(ChunkData **chunks, v2 position)
{
	chunks[0] = GetChunkAtPosition(v2(position.x - CHUNK_SIZE, position.y - CHUNK_SIZE));
	chunks[1] = GetChunkAtPosition(v2(position.x, position.y - CHUNK_SIZE));
	chunks[2] = GetChunkAtPosition(v2(position.x + CHUNK_SIZE, position.y - CHUNK_SIZE));
	chunks[3] = GetChunkAtPosition(v2(position.x - CHUNK_SIZE, position.y));
	chunks[4] = GetChunkAtPosition(v2(position.x, position.y));
	chunks[5] = GetChunkAtPosition(v2(position.x + CHUNK_SIZE, position.y));
	chunks[6] = GetChunkAtPosition(v2(position.x - CHUNK_SIZE, position.y + CHUNK_SIZE));
	chunks[7] = GetChunkAtPosition(v2(position.x, position.y + CHUNK_SIZE));
	chunks[8] = GetChunkAtPosition(v2(position.x + CHUNK_SIZE, position.y + CHUNK_SIZE));
}

internal void UpdateChunks()
{
	// Check if any chunks need to get loaded in / unloaded
	// Create / remove entity data as required

	for (int i = 0; i < core->world_data->active_chunk_count; i++)
	{
		core->world_data->active_chunks[i].entity_count = 0;
	}

	for (int i = 0; i < core->world_data->entity_count; i++)
	{
		if (core->world_data->entities[i].entity_id > 0)
		{
			Entity *entity = &core->world_data->entities[i];

			b8 is_positional_entity = 0;
			v2 world_position;

			ParallaxComponent *parallax_comp = entity->components[COMPONENT_parallax];
			if (parallax_comp)
			{
				PositionComponent *position_comp = core->world_data->entities[i].components[COMPONENT_position];
				R_DEV_ASSERT(position_comp, "Uhh");

				is_positional_entity = 1;
				world_position = parallax_comp->desired_position;
			}
			else
			{
				PositionComponent *position_comp = core->world_data->entities[i].components[COMPONENT_position];
				if (position_comp)
				{
					is_positional_entity = 1;
					world_position = position_comp->position;
				}
			}

			if (is_positional_entity)
			{
				ChunkData *chunk = GetChunkAtPosition(world_position);

				chunk->entity_ids[chunk->entity_count++] = entity->entity_id;

				entity->active_chunk = chunk;
			}
			else
			{
				core->world_data->floating_chunk.entity_ids[core->world_data->floating_chunk.entity_count++] = entity->entity_id;
				entity->active_chunk = &core->world_data->floating_chunk;
			}
		}
	}
}

static f32 GenerateNoise(f32 x_pos, f32 y_pos, f32 frequency)
{
	i32 x_min = (i32)floorf(x_pos);
	i32 x_max = (i32)floorf(x_pos) + 1;

	i32 y_min = (i32)floorf(y_pos);
	i32 y_max = (i32)floorf(y_pos) + 1;

	R_DEV_ASSERT(x_min < 128 && y_min < 128, "Noise out of bonds.");

	v2 g0 = core->random_field[y_min][x_min];
	v2 g1 = core->random_field[y_min][x_min + 1];
	v2 g2 = core->random_field[y_min + 1][x_min];
	v2 g3 = core->random_field[y_min + 1][x_min + 1];

	v2 d0 = {x_pos - x_min, y_pos - y_min};
	v2 d1 = {x_pos - x_max, y_pos - y_min};
	v2 d2 = {x_pos - x_min, y_pos - y_max};
	v2 d3 = {x_pos - x_max, y_pos - y_max};

	f32 p0 = g0.x * d0.x + g0.y * d0.y;
	f32 p1 = g1.x * d1.x + g1.y * d1.y;
	f32 p2 = g2.x * d2.x + g2.y * d2.y;
	f32 p3 = g3.x * d3.x + g3.y * d3.y;

	f32 x_fade = 6 * powf((x_pos - x_min), 5) - 15 * powf((x_pos - x_min), 4) + 10 * powf((x_pos - x_min), 3);
	f32 y_fade = 6 * powf((y_pos - y_min), 5) - 15 * powf((y_pos - y_min), 4) + 10 * powf((y_pos - y_min), 3);

	f32 lerp1 = p0 + x_fade * (p1 - p0);
	f32 lerp2 = p2 + x_fade * (p3 - p2);
	f32 lerp3 = lerp1 + y_fade * (lerp2 - lerp1);

	return lerp3;
}