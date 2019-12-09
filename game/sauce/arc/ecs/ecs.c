internal void InitialiseECS()
{
	core->entity_set->entity_count = 1;
	core->entity_set->free_entity_id = core->entity_set->entity_count;
}

internal Entity *NewEntity(char name[], EntityType type)
{
	if (core->entity_set->free_entity_id == core->entity_set->entity_count) // Entity ID is caught up
	{
		i32 entity_id = core->entity_set->entity_count;
		core->entity_set->entity_count++;
		core->entity_set->free_entity_id = core->entity_set->entity_count;

		core->entity_set->entities[entity_id].entity_id = entity_id;
		strcpy(core->entity_set->entities[entity_id].name, name);
		core->entity_set->entities[entity_id].type = type;

		return &core->entity_set->entities[entity_id];
	}
	else // Avalable entity ID isn't the latest count
	{
		i32 entity_id = core->entity_set->free_entity_id;

		core->entity_set->entities[entity_id].entity_id = entity_id;
		strcpy(core->entity_set->entities[entity_id].name, name);
		core->entity_set->entities[entity_id].type = type;

		// Determine the next free ID
		for (int i = 1; i < core->entity_set->entity_count + 1; i++)
		{
			if (core->entity_set->entities[i].entity_id == 0)
			{
				core->entity_set->free_entity_id = i;
				break;
			}
		}

		return &core->entity_set->entities[entity_id];
	}
}

/* --- Entity component helper functions --- */
// Used for quick setup of a new entity, if more indepth creation is required - create the components directly through the AddComponent macro

internal void AttachPosition(Entity *entity, v2 world_position)
{
	PositionComponent position = {
		.position = world_position,
	};
	AddPositionComponent(entity, &position);
}

// NOTE(tjr): Gives a visual representation to the entity.
internal void AttachSprite(Entity *entity, SpriteType sprite_enum, f32 render_layer)
{
	SpriteComponent sprite = {
		.sprite_data = {
			sprite_enum,
			v2(0, 0),
			render_layer,
			v2(1.0f, 1.0f),
		},
		.is_flipped = 0,
	};
	AddSpriteComponent(entity, &sprite);

	if (IsSpriteDynamic(sprite_enum))
	{
		AnimationComponent animation = {
			.flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat,
			.current_frame = 0,
			.interval_mult = 1.0f,
			.frame_start_time = core->elapsed_world_time,
		};
		AddAnimationComponent(entity, &animation);
	}
}

// NOTE(tjr): Provide multiple sprites. If dynamic, the animations will be synced.
internal void AttachSubSprite(Entity *entity, SpriteData sub_sprites[], i32 sub_sprite_count)
{
	SubSpriteComponent sub_sprite = {
		.sub_sprite_count = sub_sprite_count,
		.is_flipped = 0,
	};
	MemoryCopy(sub_sprite.sub_sprites, sub_sprites, sizeof(SpriteData) * MAX_SUB_SPRITES);
	AddSubSpriteComponent(entity, &sub_sprite);

	if (IsSpriteDynamic(sub_sprites[0].sprite_enum))
	{
		AnimationComponent animation = {
			.entity_id = entity->entity_id,
			.flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat,
			.current_frame = 0,
			.interval_mult = 1.0f,
			.frame_start_time = core->elapsed_world_time,
		};
		AddAnimationComponent(entity, &animation);
	}
}

internal void AttachCollider(Entity *entity, Shape shape, b8 flags)
{
	ColliderComponent collider = {
		.shape = shape,
		.flags = flags,
	};
	AddColliderComponent(entity, &collider);
}

internal void AttachVelocity(Entity *entity, v2 initial_velocity, v2 acceleration, b8 collide_against)
{
	VelocityComponent velocity = {
		.velocity = initial_velocity,
		.acceleration = acceleration,
		.acceleration_mult = {1.0f, 1.0f},
		.collide_against = collide_against,
	};
	AddVelocityComponent(entity, &velocity);
}

internal void AttachPhysics(Entity *entity, f32 friction_multiplier, f32 bounce_multiplier)
{
	PhysicsComponent physics = {
		.friction_mult = friction_multiplier,
		.bounce_mult = bounce_multiplier,
	};
	AddPhysicsComponent(entity, &physics);
}

// NOTE(tjr): Provides data for axis-based movement.
internal void AttachMovement(Entity *entity, f32 move_speed)
{
	MovementComponent movement = {
		.move_speed = move_speed,
		.move_speed_mult = 1.0f,
	};
	AddMovementComponent(entity, &movement);
}

// NOTE(tjr): An Arcane Entity includes a state-based automatic sprite switcher, provided via the ArcEntityType enum
internal void AttachArcEntity(Entity *entity, ArcEntityType entity_type, AnimationStateType idle_anim, f32 base_render_layer)
{
	ArcEntityComponent arc_entity = {
		.current_general_state = "Idle",
		.current_animation_state = idle_anim,
	};
	AddArcEntityComponent(entity, &arc_entity);

	if (arc_entity_animation_state_data[idle_anim].dynamic_sprite_count == 1)
	{
		AttachSprite(entity, arc_entity_animation_state_data[idle_anim].dynamic_sprites[0], base_render_layer);
	}
	else
	{
		SpriteData sub_sprites[MAX_SUB_SPRITES] = {0};
		for (int i = 0; i < arc_entity_animation_state_data[idle_anim].dynamic_sprite_count; i++)
		{
			sub_sprites[i].sprite_enum = arc_entity_animation_state_data[idle_anim].dynamic_sprites[i];
			sub_sprites[i].render_layer = base_render_layer;
			sub_sprites[i].scale = v2(1.0f, 1.0f);
		}

		AttachSubSprite(entity, sub_sprites, arc_entity_animation_state_data[idle_anim].dynamic_sprite_count);
	}
}

// NOTE(tjr): Data for an Item, provided via the ItemType enum
internal void AttachItem(Entity *entity, ItemType item_type, i32 stack_size)
{
	ItemComponent item = {
		.item_type = item_type,
		.stack_size = stack_size,
	};
	AddItemComponent(entity, &item);
}

// NOTE(tjr): Triggers callbacks upon entering/exiting the bounds. Requires a collider component that is setup as a COLLIDER_TYPE_trigger
internal void AttachTrigger(Entity *entity, void (*enter_trigger_callback)(OverlappedColliderInfo), void (*exit_trigger_callback)(OverlappedColliderInfo), b8 trigger_against)
{
	TriggerComponent trigger = {
		.trigger_against = trigger_against,
		.enter_trigger_callback = enter_trigger_callback,
		exit_trigger_callback = exit_trigger_callback,
	};
	AddTriggerComponent(entity, &trigger);
}

// NOTE(tjr): A storage system that can hold multiple ItemComponents
internal void AttachStorage(Entity *entity, i32 storage_size)
{
	StorageComponent storage = {
		.storage_size = storage_size,
	};
	AddStorageComponent(entity, &storage);
}

// NOTE(tjr): A component that updates the entity's position in the world based on camera movement. Used to simulate a 3D effect.
internal void AttachParallax(Entity *entity, v2 parallax_amount, v2 desired_position)
{
	ParallaxComponent parallax = {
		.parallax_amount = parallax_amount,
		.desired_position = desired_position,
	};
	AddParallaxComponent(entity, &parallax);
}

internal void AttachParticleEmitter(Entity *entity, f32 length, EmitterBeginCallback begin_callback, EmitterFinishCallback finish_callback)
{
	ParticleEmitterComponent emitter = {
		.life_time = length,
		.start_time = core->elapsed_world_time,
		.begin_callback = begin_callback,
		.finish_callback = finish_callback,
	};
	AddParticleEmitterComponent(entity, &emitter);

	emitter.begin_callback(entity->components[COMPONENT_particle_emitter]);
}

internal void AttachLoopedParticleEmitter(Entity *entity, f32 length, EmitterBeginCallback begin_callback)
{
	ParticleEmitterComponent emitter = {
		.life_time = length,
		.start_time = core->elapsed_world_time,
		.flags = PARTICLE_EMITTER_FLAGS_repeat,
		.begin_callback = begin_callback,
	};
	AddParticleEmitterComponent(entity, &emitter);

	emitter.begin_callback(entity->components[COMPONENT_particle_emitter]);
}

// TODO: Think of a more elegant solution to this whole background/foreground problem?
// NOTE(tjr): Attaches a position, parallax, and sprite component (marked as background for the backdrop rendering).
internal void SetupBackgroundEntity(Entity *entity, v2 desired_position, SpriteType sprite_type, f32 render_layer, v2 parallax_amount)
{
	PositionComponent position = {
		.position = desired_position,
	};
	AddPositionComponent(entity, &position);

	SpriteComponent sprite = {
		.sprite_data = {
			sprite_type,
			v2(0, 0),
			render_layer,
			v2(1.0f, 1.0f),
		},
		.is_background_sprite = 1,
	};
	AddSpriteComponent(entity, &sprite);

	ParallaxComponent parallax = {
		.parallax_amount = parallax_amount,
		.desired_position = desired_position,
	};
	AddParallaxComponent(entity, &parallax);
}