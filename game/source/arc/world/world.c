internal void CreateTestLevel()
{
	R_DEV_ASSERT(!core->run_data->current_level[0], "A level is already loaded in.");

	core->world_data->test_ptr = &core->world_data->free_entity_id;

	{
		CharacterEntity *character = InitialiseCharacterEntity();
		character->parent_generic_entity->flags |= ENTITY_FLAGS_no_delete;
		character->position_comp->position = v2(0.0f, -100.0f);

		c2Capsule capsule = {
			.a = c2V(0.0f, -10.0f),
			.b = c2V(0.0f, -50.0f),
			.r = 10.0f,
		};
		character->physics_body_comp->shape.capsule = capsule;
		character->physics_body_comp->shape_type = C2_SHAPE_TYPE_capsule;

		character->physics_body_comp->mass_data.mass = 60.0f;
		character->physics_body_comp->mass_data.inv_mass = 1.0f / character->physics_body_comp->mass_data.mass;
		character->physics_body_comp->material.restitution = 0.4f;
		character->physics_body_comp->material.static_friction = 0.5f;
		character->physics_body_comp->material.dynamic_friction = 0.5f;
		character->physics_body_comp->gravity_multiplier = 1.0f;

		character->movement_comp->move_speed = 100.0f;
		character->arc_entity_comp->entity_type = ARC_ENTITY_TYPE_player;
		character->arc_entity_comp->current_animation_state = ARC_ENTITY_ANIMATION_STATE_player_idle;
		character->sprite_comp->sprite_data.dynamic_sprite = DYNAMIC_SPRITE_player_idle;
	}

	{
		core->sword = NewEntity("Sword", ENTITY_TYPE_generic, GENERALISED_ENTITY_TYPE_item);
		core->sword->flags |= ENTITY_FLAGS_no_delete;
		ItemComponent *sword_item = AddItemComponent(core->sword);
		sword_item->item_type = ITEM_TYPE_flint_sword;
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

	ShufflePerlinNoise();
	f32 last_height = 0.0f;
	for (int x = 0; x < CHUNK_SIZE * 4; x++)
	{
		i32 octaves = 4;
		f32 frequency = 0.5f;
		f32 amplitude = 1.0f;
		f32 max_value = 0.0f;

		f32 noise_amount = 0.0f;
		for (int k = 0; k < octaves; k++)
		{
			noise_amount += GetPerlinNoise(((f32)x / (f32)CHUNK_SIZE) * frequency, 0.0f) * amplitude;
			max_value += amplitude;
			frequency *= 2.0f;
			amplitude *= 0.5f;
		}

		f32 noise = noise_amount / max_value;

		i32 terrain_height = (i32)floorf(200.0f + 50.0f * noise);
		for (int y = -1; y > -terrain_height - 1; y--)
		{
			i32 x_pos = x - CHUNK_SIZE * 2;
			i32 y_pos = y + 200;

			Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex((f32)x_pos), WorldspaceToChunkIndex((f32)y_pos));
			if (!chunk)
				chunk = LoadChunkAtIndex(WorldspaceToChunkIndex((f32)x_pos), WorldspaceToChunkIndex((f32)y_pos));

			Cell *cell = GetCellAtPosition(x_pos, y_pos);
			cell->material_type = CELL_MATERIAL_TYPE_dirt;

			QueueChunkForTextureUpdate(chunk);
		}

		f32 width = ((f32)CHUNK_SIZE * 4.0f) / 32.0f;

		if (x == 0)
			last_height = (f32)terrain_height;
		else if (((x + 1) % (i32)width) == 0)
		{
			GroundSegmentEntity *ground = NewGroundSegmentEntity();
			ground->position_comp->position = v2(floorf(x / width) * width - CHUNK_SIZE * 2, 0.0f);

			Line line = {0};
			line.p1.y = 200.0f - (f32)last_height;
			line.p2.x = width;
			line.p2.y = 200.0f - (f32)terrain_height;

			ground->physics_body_comp->shape.line = line;
			ground->physics_body_comp->shape_type = C2_SHAPE_TYPE_line;
			ground->physics_body_comp->material.static_friction = 0.2f;
			ground->physics_body_comp->material.dynamic_friction = 0.2f;
			ground->physics_body_comp->mass_data.mass = 0.0f;
			ground->physics_body_comp->mass_data.inv_mass = 0.0f;

			last_height = (f32)terrain_height;
		}
	}

	strcpy(core->run_data->current_level, "testing");
	SaveLevel("testing");
}

internal void DrawWorld()
{
#ifdef DEVELOPER_TOOLS
	if (!(core->run_data->editor_flags & EDITOR_FLAGS_draw_world))
		return;
#endif

	Ts2dPushBackgroundBegin();
	{
		v4 top_sky_colour = v4(88.0f / 255.0f, 152.0f / 255.0f, 194.0f / 255.0f, 1.0f);
		v4 middle_sky_colour = v4(203.0f / 255.0f, 232.0f / 255.0f, 238.0f / 255.0f, 1.0f);
		v4 bottom_sky_colour = v4(130.0f / 255.0f, 81.0f / 255.0f, 77.0f / 255.0f, 1.0f);

		Ts2dPushFilledVertexColoredRect(top_sky_colour,
										middle_sky_colour,
										top_sky_colour,
										middle_sky_colour,
										v4(0.0f, 0.0f, core->render_width, core->render_height / 2.0f));

		Ts2dPushFilledVertexColoredRect(middle_sky_colour,
										bottom_sky_colour,
										middle_sky_colour,
										bottom_sky_colour,
										v4(0.0f, core->render_height / 2.0f, core->render_width, core->render_height / 2.0f));

		RenderBackgroundSprites();
	}
	Ts2dPushBackgroundEnd();

	Ts2dWorldInfo world_info = {0};
	{
		world_info.flags |= TS2D_BLUR_SHADOWS;
		world_info.flags |= TS2D_BLUR_BACKGROUNDS;
		if (core->client_data->bloom)
			world_info.flags |= TS2D_BLOOM;
		//world_info.shadow_opacity = core->shadow_opacity;
		world_info.shadow_vector = v2(32.0f, 64.0f);
		world_info.camera_pos = core->camera_position;
		world_info.camera_zoom = core->camera_zoom;
		world_info.brightness = 1.0f;
		world_info.grayscale = 0.f;
	}

	Ts2dPushWorldBegin(&world_info);

	// NOTE(tjr): Sprite rendering.
	{
		UpdateAnimations();
		// PostUpdateWorldAnimations();
		RenderForegroundSprites();
	}

	/* v2 pos = v2view(v2(-350.0f, 35.0f));
                v2 size = v2zoom(v2(700.0f, 200.0f));
                Ts2dPushReflectiveRect(
                core->renderer,
                v4(pos.x, pos.y, size.x, size.y),
                v4(1.0f, 1.0f, 1.0f, 1.0f),
                0.0f, 0.75f); */

	Ts2dPushWorldEnd();

	// NOTE(tjr): Draw velocity projection.
	/* if (core->draw_velocity)
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
    } */
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

internal void UpdateChunks()
{
	// Unload any chunks that haven't been marked as 'remain_loaded' by now
	for (i32 i = 0; i < core->world_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->world_data->active_chunks[i];
		if (!chunk->remain_loaded)
		{
			// unload chunk
		}
		else
			chunk->remain_loaded = 0; // Reset
	}

	// Load in chunks that should be visible to the player + a buffer
	if (!core->run_data->disable_chunk_view_loading)
	{
		const i32 buffer = 1;

		v2 top_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
		v2 top_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
		v2 bottom_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};

		i32 width = WorldspaceToChunkIndex(top_right.x) - WorldspaceToChunkIndex(top_left.x);
		i32 height = WorldspaceToChunkIndex(bottom_left.y) - WorldspaceToChunkIndex(top_left.y);

		for (int y = -buffer; y <= height + buffer; y++)
		{
			for (int x = -buffer; x <= width + buffer; x++)
			{
				Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex(top_left.x) + x, WorldspaceToChunkIndex(top_left.y) + y);
				if (!chunk)
				{
					chunk = LoadChunkAtIndex(WorldspaceToChunkIndex(top_left.x) + x, WorldspaceToChunkIndex(top_left.y) + y);
					chunk->remain_loaded = 1;
				}
			}
		}
	}

	// Give a new entity list for chunks
	// TODO: This shouldn't be completely stored for each chunk, should be setup as a radix sort type thingy
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
				Chunk *chunk = GetChunkAtIndex(WorldspaceToChunkIndex(world_position.x), WorldspaceToChunkIndex(world_position.y));

				chunk->entity_ids[chunk->entity_count++] = entity->entity_id;
			}
		}
	}
}

internal i32 WorldspaceToChunkIndex(f32 world_space_coordinate)
{
	return (i32)floorf(world_space_coordinate / CHUNK_SIZE);
}

internal Chunk *GetChunkAtIndex(i32 x, i32 y)
{
	for (int i = 0; i < core->world_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->world_data->active_chunks[i];
		if (chunk->is_valid)
			if (x == chunk->x_index &&
				y == chunk->y_index)
				return chunk;
	}

	// Chunk isn't loaded.
	return 0;
}

internal void GetVisibleChunks(Chunk **chunks, i32 *chunk_count)
{
	v2 top_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 top_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};

	Chunk *tl_chunk = GetChunkAtIndex(WorldspaceToChunkIndex(top_left.x), WorldspaceToChunkIndex(top_left.y));
	if (tl_chunk)
		chunks[(*chunk_count)++] = tl_chunk; // Manually add the first chunk to the algo.
	else
		LogWarning("A visible chunk is not loaded, is this intended?");

	i32 width = WorldspaceToChunkIndex(top_right.x) - WorldspaceToChunkIndex(top_left.x);
	i32 height = WorldspaceToChunkIndex(bottom_left.y) - WorldspaceToChunkIndex(top_left.y);

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			if (!(x == 0 && y == 0))
			{
				Chunk *chunk = GetChunkAtIndex(tl_chunk->x_index + x, tl_chunk->y_index + y);
				if (chunk)
					chunks[(*chunk_count)++] = chunk;
				else
					LogWarning("A visible chunk is not loaded, is this intended?");
			}
		}
	}

	R_DEV_ASSERT(*chunk_count < MAX_WORLD_CHUNKS, "Visible chunks exceed MAX_WORLD_CHUNKS, how is this possible?");
}

internal void GetSurroundingChunks(Chunk **chunks, v2 position)
{
	R_TODO; // What's this func for again?

	chunks[0] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[1] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[2] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y - CHUNK_SIZE));
	chunks[3] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y));
	chunks[4] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y));
	chunks[5] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y));
	chunks[6] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x - CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[7] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
	chunks[8] = GetChunkAtIndex(WorldspaceToChunkIndex(position.x + CHUNK_SIZE),
								WorldspaceToChunkIndex(position.y + CHUNK_SIZE));
}

// Loading isn't done from disk, for now.
internal Chunk *LoadChunkAtIndex(i32 x_index, i32 y_index)
{
	R_DEV_ASSERT(core->world_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS, "Too many chunccs are loaded bruh.");

	Chunk *chunk = &core->world_data->active_chunks[core->world_data->active_chunk_count++];
	chunk->is_valid = 1;
	chunk->x_index = x_index;
	chunk->y_index = y_index;

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			Cell *cell = &chunk->cells[y][x];
			cell->x_position = x_index * CHUNK_SIZE + x;
			cell->y_position = y_index * CHUNK_SIZE + y;
		}
	}

	QueueChunkForTextureUpdate(chunk);

	return chunk;
}

internal void UnloadChunk(Chunk *chunk_to_unload)
{
	R_TODO;
	// Will sort this out (along with proper world save structure) when we need infinite gen.
}