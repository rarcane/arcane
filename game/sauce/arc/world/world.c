internal void TempInitGameWorld()
{
	core->is_ingame = 1;

	/* {
		Entity *tree1 = NewEntity("Tree");
		AttachPosition(tree1, v2(100.0f, 0.0f));
		AttachSprite(tree1, DYNAMIC_SPRITE_birch_tree2, 2.0f);
		AttachPhysics(tree1, 1.0f, 0.0f);

		Entity *tree2 = NewEntity("Tree");
		AttachPosition(tree2, v2(-100.0f, 0.0f));
		AttachSprite(tree2, DYNAMIC_SPRITE_birch_tree1, 2.0f);
		AttachPhysics(tree2, 1.0f, 0.0f);
	} */

	{
		/* Entity *tree1 = NewEntity("Tree");
		AttachPosition(tree1, v2(-100.0f, 0.0f));
		AttachSprite(tree1, STATIC_SPRITE_pine_tree_1, 2.0f);
		AttachParallax(tree1, v2(0.25, 0.25), v2(-100.0f, 0.0f)); */

		Entity *tree2 = NewEntity("Tree");
		AttachPosition(tree2, v2(-100.0f, 0.0f));
		AttachSprite(tree2, STATIC_SPRITE_pine_tree_v1, 2.0f);
	}

	{
		Entity *hills3 = NewEntity("Hill");
		SetupBackgroundEntity(hills3, v2(-272.0f, 0.0f), STATIC_SPRITE_hills_1_v1, 5.1f, v2(0.2f, 0.1f));
		Entity *hills1 = NewEntity("Hill");
		SetupBackgroundEntity(hills1, v2(-93.0f, 0.0f), STATIC_SPRITE_hills_1_v2, 5.0f, v2(0.2f, 0.1f));
		Entity *hills2 = NewEntity("Hill");
		SetupBackgroundEntity(hills2, v2(93.0f, 0.0f), STATIC_SPRITE_hills_1_v1, 5.1f, v2(0.2f, 0.1f));
		Entity *hills4 = NewEntity("Hill");
		SetupBackgroundEntity(hills4, v2(272.0f, 0.0f), STATIC_SPRITE_hills_1_v2, 5.0f, v2(0.2f, 0.1f));

		Entity *canopy4 = NewEntity("Canopy");
		SetupBackgroundEntity(canopy4, v2(-260.0f, -115.0f), STATIC_SPRITE_bg1_canopy_v1, 5.0f, v2(0.2f, 0.1f));
		Entity *canopy2 = NewEntity("Canopy");
		SetupBackgroundEntity(canopy2, v2(-89.0f, -115.0f), STATIC_SPRITE_bg1_canopy_v2, 5.1f, v2(0.2f, 0.1f));
		Entity *canopy1 = NewEntity("Canopy");
		SetupBackgroundEntity(canopy1, v2(89.0f, -115.0f), STATIC_SPRITE_bg1_canopy_v1, 5.0f, v2(0.2f, 0.1f));
		Entity *canopy3 = NewEntity("Canopy");
		SetupBackgroundEntity(canopy3, v2(260.0f, -115.0f), STATIC_SPRITE_bg1_canopy_v2, 5.1f, v2(0.2f, 0.1f));

		Entity *bg1_tree_1 = NewEntity("bgtree");
		SetupBackgroundEntity(bg1_tree_1, v2(10.0f, -5.0f), STATIC_SPRITE_bg1_pine_tree_v1, 5.2f, v2(0.2f, 0.1f));

		Entity *bg2_hills3 = NewEntity("hills");
		SetupBackgroundEntity(bg2_hills3, v2(-300.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.1f, v2(0.4f, 0.2f));
		Entity *bg2_hills2 = NewEntity("hills");
		SetupBackgroundEntity(bg2_hills2, v2(-100.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.1f, v2(0.4f, 0.2f));
		Entity *bg2_hills4 = NewEntity("hills");
		SetupBackgroundEntity(bg2_hills4, v2(100.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.0f, v2(0.4f, 0.2f));
		Entity *bg2_hills1 = NewEntity("hills");
		SetupBackgroundEntity(bg2_hills1, v2(300.0f, 0.0f), STATIC_SPRITE_bg2_hills_v1, 6.0f, v2(0.4f, 0.2f));

		Entity *bg2_tree_1 = NewEntity("bgtree");
		SetupBackgroundEntity(bg2_tree_1, v2(50.0f, -5.0f), STATIC_SPRITE_bg2_pine_tree_v1, 6.5f, v2(0.4f, 0.2f));

		Entity *bg3_hills1 = NewEntity("hills");
		SetupBackgroundEntity(bg3_hills1, v2(100.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.6f, 0.3f));
		Entity *bg3_hills2 = NewEntity("hills");
		SetupBackgroundEntity(bg3_hills2, v2(-100.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.6f, 0.3f));
		Entity *bg3_hills3 = NewEntity("hills");
		SetupBackgroundEntity(bg3_hills3, v2(300.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.6f, 0.3f));
		Entity *bg3_hills4 = NewEntity("hills");
		SetupBackgroundEntity(bg3_hills4, v2(-300.0f, 0.0f), STATIC_SPRITE_bg3_hills_v1, 7.0f, v2(0.6f, 0.3f));

		Entity *bg3_tree_1 = NewEntity("bgtree");
		SetupBackgroundEntity(bg3_tree_1, v2(-20.0f, -5.0f), STATIC_SPRITE_bg3_pine_tree_v1, 7.5f, v2(0.6f, 0.3f));
	}

	{
		core->player = NewEntity("Player");

		AttachPosition(core->player, v2(0.0f, -100.0f));
		AttachCollider(core->player, GetRectangleShape(v2(14.0f, 35.0f), v2(0.0f, 0.0f)), COLLIDER_FLAG_player);
		AttachPhysics(core->player, 1.0f, 0.0f);
		AttachVelocity(core->player, v2(0, 0), v2(250.0f, 0.0f), COLLIDER_FLAG_ground);
		AttachMovement(core->player, 100.0f);
		AttachArcEntity(core->player, ARC_ENTITY_player, ANIMATION_STATE_player_idle, 0.0f);

		SubSpriteComponent *sub_sprite_comp = core->player->components[COMPONENT_sub_sprite];
		SpriteData *arm_sub_sprite = GetSubSpriteByType(sub_sprite_comp, DYNAMIC_SPRITE_player_idle_arm);
		arm_sub_sprite->render_layer -= 0.05f;
	}

	{
		core->sword = NewEntity("Sword");
		AttachItem(core->sword, ITEM_flint_sword, 1);
	}

	{
		core->backpack = NewEntity("Backpack");
		AttachStorage(core->backpack, 9);

		core->hotbar = NewEntity("Hotbar");
		AttachStorage(core->hotbar, 2);

		StorageComponent *storage = core->backpack->components[COMPONENT_storage];
		AddItemToStorage(core->sword->components[COMPONENT_item], storage, 2);
	}

	for (int i = -10; i < 11; i++)
	{
		Entity *ground = NewEntity("Ground");
		AttachPosition(ground, v2(i * 100.0f, 0.0f));
		// AttachSprite(ground, STATIC_SPRITE_ground_plains, -1.0f);
		// AttachCollider(ground, GetRectangleShape(v2(100.0f, 147.0f), v2(0.0f, 147.0f)), COLLIDER_FLAG_ground);
		AttachSprite(ground, STATIC_SPRITE_ground_forest, -1.0f);
		AttachCollider(ground, GetRectangleShape(v2(100.0f, 87.0f), v2(0.0f, 87.0f)), COLLIDER_FLAG_ground);
		AttachPhysics(ground, 1.0f, 1.0f);
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
		// TODO: Cleanliness: Do I really need to be passing in all of this data in via a variable? It can just be globally accessed lmao why I am doing this?
		UpdateAnimations(core->component_set->animation_components, core->component_set->animation_component_count);
		PostUpdateWorldAnimations();
		RenderForegroundSprites(core->component_set->sprite_components, core->component_set->sub_sprite_components, core->component_set->sprite_component_count, core->component_set->sub_sprite_component_count);
	}

	// NOTE(tjr): Collision bounds rendering.
	if (core->draw_colliders)
	{
		RenderColliders(core->component_set->collider_components, core->component_set->collider_component_count);
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
		for (int i = 0; i < core->component_set->velocity_component_count; i++)
		{
			VelocityComponent *velocity_comp = &core->component_set->velocity_components[i];
			PositionComponent *PositionComponent = core->entity_set->entities[velocity_comp->entity_id].components[COMPONENT_position];

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
	for (int i = 0; i < core->component_set->parallax_component_count; i++)
	{
		ParallaxComponent *parallax_comp = &core->component_set->parallax_components[i];
		if (parallax_comp->entity_id > 0)
		{
			Entity *entity = &core->entity_set->entities[parallax_comp->entity_id];
			PositionComponent *position_comp = entity->components[COMPONENT_position];
			R_DEV_ASSERT(position_comp, "Parallax must be attached with a position to update.");

			PositionComponent *player_pos = core->player->components[COMPONENT_position];

			// TODO: Need to find a way to centralise the desired_position of the parallax, whilst still maintaining spatial consistency across sprites
			position_comp->position.x = parallax_comp->desired_position.x + (player_pos->position.x + core->camera_offset.x) * parallax_comp->parallax_amount.x;
			position_comp->position.y = parallax_comp->desired_position.y + (player_pos->position.y + core->camera_offset.y) * parallax_comp->parallax_amount.y;

			// position_comp->position.x = parallax_comp->desired_position.x - (parallax_comp->desired_position.x - player_pos->position.x + core->camera_offset.x) * parallax_comp->parallax_amount.x;
			// position_comp->position.y = parallax_comp->desired_position.y - (parallax_comp->desired_position.y - player_pos->position.y + core->camera_offset.y) * parallax_comp->parallax_amount.y;
		}
	}
}