internal void TempInitGameWorld()
{
	core->is_ingame = 1;

	{
		Entity *tree1 = NewEntity("Tree");
		AttachPosition(tree1, v2(100.0f, 0.0f));
		AttachSprite(tree1, DYNAMIC_SPRITE_birch_tree2, 2.0f);
		AttachPhysics(tree1, 1.0f, 0.0f);

		Entity *tree2 = NewEntity("Tree");
		AttachPosition(tree2, v2(-100.0f, 0.0f));
		AttachSprite(tree2, DYNAMIC_SPRITE_birch_tree1, 2.0f);
		AttachPhysics(tree2, 1.0f, 0.0f);
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
		UpdateAnimations(core->component_set->animation_components, core->component_set->animation_component_count);
		PostUpdateWorldAnimations();
		RenderSprites(core->component_set->sprite_components, core->component_set->sub_sprite_components, core->component_set->sprite_component_count, core->component_set->sub_sprite_component_count);
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