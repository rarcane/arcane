internal void CreateTestLevel()
{
	R_DEV_ASSERT(!core->client_data->current_level[0], "A level is already loaded in.");

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
			Cell *cell = GetCellAtPosition(x - CHUNK_SIZE * 2, y + 200);
			CellMaterial *material = NewCellMaterial(cell, CELL_MATERIAL_TYPE_dirt);
			material->mass = 0.0f;
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

	strcpy(core->client_data->current_level, "testing");
	SaveLevel("testing");
}

internal void DrawWorld()
{
#ifdef DEVELOPER_TOOLS
	if (!(core->client_data->editor_flags & EDITOR_FLAGS_draw_world))
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
		PostUpdateWorldAnimations();
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

internal void PostUpdateWorldAnimations()
{
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

	// Chunk doesn't exist yet, so just initialise a new one at this positon
	{
		R_DEV_ASSERT(core->world_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS, "Too many chunccs bruh");

		ChunkData *chunk = &core->world_data->active_chunks[core->world_data->active_chunk_count++];
		chunk->is_valid = 1;
		chunk->x_index = FloatToChunkIndex(position.x);
		chunk->y_index = FloatToChunkIndex(position.y);
		chunk->free_cell_material_id = 1;

		for (int i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
		{
			for (int j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
			{
				CellChunk *cell_chunk = &chunk->cell_chunks[i][j];
				cell_chunk->x_index = j;
				cell_chunk->y_index = i;
				cell_chunk->parent_chunk = chunk;

				// Initialise cells
				for (int y = 0; y < CELL_CHUNK_SIZE; y++)
				{
					for (int x = 0; x < CELL_CHUNK_SIZE; x++)
					{
						Cell *cell = &cell_chunk->cells[y][x];
						cell->parent_cell_chunk = cell_chunk;
						cell->x_index = x;
						cell->y_index = y;
					}
				}
			}
		}

		UpdateChunkTextures(chunk);

		return chunk;
	}
}

internal ChunkData *GetChunkAtIndex(i32 x, i32 y)
{
	for (int i = 0; i < core->world_data->active_chunk_count; i++)
	{
		ChunkData *chunk = &core->world_data->active_chunks[i];
		if (chunk->is_valid)
		{
			if (x == chunk->x_index &&
				y == chunk->y_index)
			{
				return chunk;
			}
		}
	}

	// Chunk doesn't exist yet, so just initialise a new one at this positon
	{
		R_DEV_ASSERT(core->world_data->active_chunk_count + 1 < MAX_WORLD_CHUNKS, "Too many chunccs bruh");

		ChunkData *chunk = &core->world_data->active_chunks[core->world_data->active_chunk_count++];
		chunk->is_valid = 1;
		chunk->x_index = x;
		chunk->y_index = y;
		chunk->free_cell_material_id = 1;

		for (int i = 0; i < CHUNK_SIZE / CELL_CHUNK_SIZE; i++)
		{
			for (int j = 0; j < CHUNK_SIZE / CELL_CHUNK_SIZE; j++)
			{
				CellChunk *cell_chunk = &chunk->cell_chunks[i][j];
				cell_chunk->x_index = j;
				cell_chunk->y_index = i;
				cell_chunk->parent_chunk = chunk;

				// Initialise cells
				for (int y = 0; y < CELL_CHUNK_SIZE; y++)
				{
					for (int x = 0; x < CELL_CHUNK_SIZE; x++)
					{
						Cell *cell = &cell_chunk->cells[y][x];
						cell->parent_cell_chunk = cell_chunk;
						cell->x_index = x;
						cell->y_index = y;
					}
				}
			}
		}

		UpdateChunkTextures(chunk);

		return chunk;
	}
}

internal void GetVisibleChunks(ChunkData **chunks, i32 *chunk_count)
{
	v2 top_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 top_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_left = {-core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_right = {-core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), -core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};

	ChunkData *tl_chunk = GetChunkAtPosition(top_left);
	chunks[(*chunk_count)++] = tl_chunk;
	ChunkData *tr_chunk = GetChunkAtPosition(top_right);
	ChunkData *bl_chunk = GetChunkAtPosition(bottom_left);
	ChunkData *br_chunk = GetChunkAtPosition(bottom_right);

	i32 width = tr_chunk->x_index - tl_chunk->x_index;
	i32 height = bl_chunk->y_index - tl_chunk->y_index;

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			if (!(x == 0 && y == 0))
			{
				ChunkData *chunk = GetChunkAtIndex(tl_chunk->x_index + x, tl_chunk->y_index + y);
				chunks[(*chunk_count)++] = chunk;
			}
		}
	}

	R_DEV_ASSERT(*chunk_count < MAX_WORLD_CHUNKS, "Too many visible chunks.");
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

// internal void