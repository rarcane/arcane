internal void TempInitGameWorld()
{
	core->is_ingame = 1;

	// avert your eyes lmao

	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(-260.0f, 13.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 0;
	}
	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(-190.0f, 0.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 1;
	}
	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(-110.0f, 10.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 0;
	}
	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(103.0f, 10.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 1;
	}
	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(182.0f, 0.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 0;
	}
	{
		Entity *tree = NewEntity("Tree", ENTITY_TYPE_resource);
		AttachPosition(tree, v2(250.0f, 13.0f));
		AttachSprite(tree, STATIC_SPRITE_pine_tree_v1, 2.0f);
		AttachLoopedParticleEmitter(tree, 0.5f, GeneratePineParticles);

		SpriteComponent *sprite_comp = tree->components[COMPONENT_sprite];
		sprite_comp->is_flipped = 1;
	}

	{
		Entity *hills3 = NewEntity("Hill", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(hills3, v2(-272.0f, 0.0f), STATIC_SPRITE_hills_1_v1, 5.1f, v2(0.4f, 0.1f));
		Entity *hills1 = NewEntity("Hill", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(hills1, v2(-93.0f, 0.0f), STATIC_SPRITE_hills_1_v2, 5.0f, v2(0.4f, 0.1f));
		Entity *hills2 = NewEntity("Hill", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(hills2, v2(93.0f, 0.0f), STATIC_SPRITE_hills_1_v1, 5.1f, v2(0.4f, 0.1f));
		Entity *hills4 = NewEntity("Hill", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(hills4, v2(272.0f, 0.0f), STATIC_SPRITE_hills_1_v2, 5.0f, v2(0.4f, 0.1f));

		{
			Entity *bg1_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_tree_1, v2(-212.0f, 2.0f), STATIC_SPRITE_bg1_pine_tree_v1, 5.2f, v2(0.4f, 0.1f));

			SpriteComponent *sprite_comp = bg1_tree_1->components[COMPONENT_sprite];
			sprite_comp->is_flipped = 1;
		}
		{
			Entity *bg1_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_tree_1, v2(-93.0f, 2.0f), STATIC_SPRITE_bg1_pine_tree_v2, 5.2f, v2(0.4f, 0.1f));

			SpriteComponent *sprite_comp = bg1_tree_1->components[COMPONENT_sprite];
			sprite_comp->is_flipped = 1;
		}
		{
			Entity *bg1_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_tree_1, v2(80.0f, 6.0f), STATIC_SPRITE_bg1_pine_tree_v1, 5.2f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_tree_1, v2(155.0f, 2.0f), STATIC_SPRITE_bg1_pine_tree_v2, 5.2f, v2(0.4f, 0.1f));
		}

		{
			Entity *bg1_shrub = NewEntity("Bg1 Shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_shrub, v2(-80, 2.0f), STATIC_SPRITE_bg1_shrub_v1, 5.15f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_shrub = NewEntity("Bg1 Shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_shrub, v2(-171, 0.0f), STATIC_SPRITE_bg1_shrub_v2, 5.15f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_shrub = NewEntity("Bg1 Shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_shrub, v2(-22, -3.0f), STATIC_SPRITE_bg1_shrub_v3, 5.15f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_shrub = NewEntity("Bg1 Shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_shrub, v2(196, -4.0f), STATIC_SPRITE_bg1_shrub_v2, 5.15f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_shrub = NewEntity("Bg1 Shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_shrub, v2(91, 1.0f), STATIC_SPRITE_bg1_shrub_v3, 5.15f, v2(0.4f, 0.1f));
		}

		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(-160.0f, 0.0f), STATIC_SPRITE_bg1_sapling_v1, 5.17f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(-46.0f, -8.0f), STATIC_SPRITE_bg1_sapling_v2, 5.17f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(36.0f, -8.0f), STATIC_SPRITE_bg1_sapling_v3, 5.17f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(-198.0f, 11.0f), STATIC_SPRITE_bg1_sapling_v1, 5.17f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(178.0f, -8.0f), STATIC_SPRITE_bg1_sapling_v2, 5.17f, v2(0.4f, 0.1f));
		}
		{
			Entity *bg1_sapling = NewEntity("Bg1 Sapling", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg1_sapling, v2(209.0f, -8.0f), STATIC_SPRITE_bg1_sapling_v3, 5.17f, v2(0.4f, 0.1f));
		}
	}

	{
		Entity *bg2_hills3 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg2_hills3, v2(-300.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.1f, v2(0.6f, 0.2f));
		Entity *bg2_hills2 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg2_hills2, v2(-100.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.1f, v2(0.6f, 0.2f));
		Entity *bg2_hills4 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg2_hills4, v2(100.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.0f, v2(0.6f, 0.2f));
		Entity *bg2_hills1 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg2_hills1, v2(300.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.0f, v2(0.6f, 0.2f));

		{
			Entity *bg2_tree = NewEntity("bg2 tree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_tree, v2(-169, -5), STATIC_SPRITE_bg2_pine_tree_v1, 6.5f, v2(0.6f, 0.2f));
		}
		{
			Entity *bg2_tree = NewEntity("bg2 tree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_tree, v2(-127, -5), STATIC_SPRITE_bg2_pine_tree_v2, 6.5f, v2(0.6f, 0.2f));
		}
		{
			Entity *bg2_tree = NewEntity("bg2 tree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_tree, v2(54, -5), STATIC_SPRITE_bg2_pine_tree_v1, 6.5f, v2(0.6f, 0.2f));
			SpriteComponent *sprite_comp = bg2_tree->components[COMPONENT_sprite];
			sprite_comp->is_flipped = 1;
		}
		{
			Entity *bg2_tree = NewEntity("bg2 tree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_tree, v2(104, -5), STATIC_SPRITE_bg2_pine_tree_v1, 6.5f, v2(0.6f, 0.2f));
			SpriteComponent *sprite_comp = bg2_tree->components[COMPONENT_sprite];
			sprite_comp->is_flipped = 1;
		}

		{
			Entity *bg2_shrub = NewEntity("bg2 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_shrub, v2(-150, -9), STATIC_SPRITE_bg2_shrub_v1, 6.4f, v2(0.6f, 0.2f));
		}
		{
			Entity *bg2_shrub = NewEntity("bg2 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_shrub, v2(54, -6), STATIC_SPRITE_bg2_shrub_v2, 6.4f, v2(0.6f, 0.2f));
		}
		{
			Entity *bg2_shrub = NewEntity("bg2 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg2_shrub, v2(126, -6), STATIC_SPRITE_bg2_shrub_v3, 6.4f, v2(0.6f, 0.2f));
		}
	}

	{
		Entity *bg3_hills1 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg3_hills1, v2(100.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.75f, 0.3f));
		Entity *bg3_hills2 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg3_hills2, v2(-100.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.75f, 0.3f));
		Entity *bg3_hills3 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg3_hills3, v2(300.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.75f, 0.3f));
		Entity *bg3_hills4 = NewEntity("hills", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(bg3_hills4, v2(-300.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.75f, 0.3f));

		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(30.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v1, 7.5f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(43.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v2, 7.6f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(56.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v3, 7.5f, v2(0.75f, 0.3f));
		}

		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(15.0f, -21.0f), STATIC_SPRITE_bg3_shrub_v1, 7.3f, v2(0.75, 0.3f));
		}
		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(44.0f, -18.0f), STATIC_SPRITE_bg3_shrub_v4, 7.3f, v2(0.75, 0.3f));
		}

		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(-49.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v4, 7.5f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(-36.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v5, 7.6f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(-25.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v6, 7.5f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(-87.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v7, 7.5f, v2(0.75f, 0.3f));
		}
		{
			Entity *bg3_tree_1 = NewEntity("bgtree", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_tree_1, v2(-73.0f, -17.0f), STATIC_SPRITE_bg3_pine_tree_v8, 7.5f, v2(0.75f, 0.3f));
		}

		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(-29.0f, -18.0f), STATIC_SPRITE_bg3_shrub_v1, 7.3f, v2(0.75, 0.3f));
		}
		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(-59.0f, -19.0f), STATIC_SPRITE_bg3_shrub_v2, 7.3f, v2(0.75, 0.3f));
		}
		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(-147.0f, -18.0f), STATIC_SPRITE_bg3_shrub_v3, 7.3f, v2(0.75, 0.3f));
		}
		{
			Entity *bg3_shrub = NewEntity("bg3 shrub", ENTITY_TYPE_scenic);
			SetupBackgroundEntity(bg3_shrub, v2(193.0f, -18.0f), STATIC_SPRITE_bg3_shrub_v4, 7.3f, v2(0.75, 0.3f));
		}
	}

	Entity *mid_mountains_1 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(mid_mountains_1, v2(-250.0f, 0.0f), STATIC_SPRITE_mid_mountains, 8.0f, v2(0.93f, 0.68f));
	Entity *mid_mountains_2 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(mid_mountains_2, v2(-850.0f, 0.0f), STATIC_SPRITE_mid_mountains, 8.0f, v2(0.93f, 0.68f));
	Entity *mid_mountains_3 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(mid_mountains_3, v2(350.0f, 0.0f), STATIC_SPRITE_mid_mountains, 8.0f, v2(0.93f, 0.68f));

	Entity *far_mountains_1 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(far_mountains_1, v2(0.0f, 0.0f), STATIC_SPRITE_far_mountains, 9.0f, v2(0.95f, 0.7f));
	Entity *far_mountains_2 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(far_mountains_2, v2(-600.0f, 0.0f), STATIC_SPRITE_far_mountains, 9.0f, v2(0.95f, 0.7f));
	Entity *far_mountains_3 = NewEntity("Mountains", ENTITY_TYPE_scenic);
	SetupBackgroundEntity(far_mountains_3, v2(600.0f, 0.0f), STATIC_SPRITE_far_mountains, 9.0f, v2(0.95f, 0.7f));

	{
		core->player = NewEntity("Player", ENTITY_TYPE_character);
		core->player->flags |= ENTITY_FLAGS_no_delete;

		AttachPosition(core->player, v2(0.0f, -100.0f));
		AttachCollider(core->player, GetRectangleShape(v2(14.0f, 35.0f), v2(0.0f, 0.0f)), COLLIDER_FLAGS_player);
		AttachPhysics(core->player, 1.0f, 0.0f);
		AttachVelocity(core->player, v2(0, 0), v2(250.0f, 0.0f), COLLIDER_FLAGS_ground);
		AttachMovement(core->player, 100.0f);
		AttachArcEntity(core->player, ARC_ENTITY_player, ANIMATION_STATE_player_idle, 0.0f);

		SubSpriteComponent *sub_sprite_comp = core->player->components[COMPONENT_sub_sprite];
		SpriteData *arm_sub_sprite = GetSubSpriteByType(sub_sprite_comp, DYNAMIC_SPRITE_player_idle_arm);
		arm_sub_sprite->render_layer -= 0.05f;
	}

	{
		core->sword = NewEntity("Sword", ENTITY_TYPE_item);
		core->sword->flags |= ENTITY_FLAGS_no_delete;
		AttachItem(core->sword, ITEM_flint_sword, 1);
	}

	{
		core->backpack = NewEntity("Backpack", ENTITY_TYPE_storage);
		core->backpack->flags |= ENTITY_FLAGS_no_delete;
		AttachStorage(core->backpack, 9);

		core->hotbar = NewEntity("Hotbar", ENTITY_TYPE_storage); // Hotbar should technically be attached to the player entity
		core->hotbar->flags |= ENTITY_FLAGS_no_delete;
		AttachStorage(core->hotbar, 2);

		StorageComponent *storage = core->backpack->components[COMPONENT_storage];
		AddItemToStorage(core->sword->components[COMPONENT_item], storage, 2);
	}

	for (int i = -10; i < 11; i++)
	{
		Entity *ground = NewEntity("Ground", ENTITY_TYPE_ground);
		AttachPosition(ground, v2(i * 100.0f, 0.0f));
		AttachSprite(ground, STATIC_SPRITE_ground_forest, -1.0f);
		AttachCollider(ground, GetRectangleShape(v2(100.0f, 87.0f), v2(0.0f, 87.0f)), COLLIDER_FLAGS_ground);
		AttachPhysics(ground, 1.0f, 1.0f);
		//AttachLoopedParticleEmitter(ground, 1.0f, GenerateGroundParticles);

		Entity *ground_rocks = NewEntity("GroundRocks", ENTITY_TYPE_scenic);
		SetupBackgroundEntity(ground_rocks,
							  v2(i * 100.0f, 0.0f),
							  STATIC_SPRITE_ground_rocks_v1 + RandomI32(0, 1),
							  2.5f,
							  v2(0.1f, 0.025f));
		/* AttachPosition(ground_rocks, v2(i * 100.0f, 0.0f));
		AttachSprite(ground_rocks, STATIC_SPRITE_ground_rocks_v1 + RandomI32(0, 1), 2.5f); */
	}

	for (int i = 0; i < 12; i++)
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
	}
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
		PositionComponent *position_comp = core->player->components[COMPONENT_position];
		AnimationComponent *animation_comp = core->player->components[COMPONENT_animation];
		SubSpriteComponent *sub_sprite_comp = core->player->components[COMPONENT_sub_sprite];

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
	for (int i = 0; i < core->cloud_count; i++)
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

	// Chunk doesn't exist yet, so just create it at this positon
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