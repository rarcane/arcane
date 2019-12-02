internal void InitialiseECS()
{
	core->entity_set->entity_count = 1;
	core->entity_set->free_entity_id = core->entity_set->entity_count;

	// Is it useful to have a null component at 0?
	/* for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		switch (i)
		{
#define Component(component_struct, component_name)              \
	case COMPONENT_##component_name:                             \
	{                                                            \
		core->component_set->component_name##_component_count++; \
	}                                                            \
	break;
#include "components.inc"
#undef Component
		}
	} */
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

		return &core->entity_set->entities[entity_id];
	}
	else // Avalable entity ID isn't the latest count
	{
		i32 entity_id = core->entity_set->free_entity_id;

		core->entity_set->entities[entity_id].entity_id = entity_id;
		strcpy(core->entity_set->entities[entity_id].name, name);

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

internal void AddComponent(Entity *entity, ComponentType component_type, void *component_data)
{
	switch (component_type)
	{
#define Component(component_struct, component_name)                                                                      \
	case COMPONENT_##component_name:                                                                                     \
	{                                                                                                                    \
		i32 comp_count = core->component_set->component_name##_component_count;                                          \
		core->component_set->component_name##_components[comp_count] = *((component_struct##Component *)component_data); \
		entity->components[COMPONENT_##component_name] = &core->component_set->component_name##_components[comp_count];  \
		core->component_set->component_name##_components[comp_count].entity_id = entity->entity_id;                      \
		core->component_set->component_name##_components[comp_count].component_id = comp_count;                          \
		core->component_set->component_name##_component_count++;                                                         \
	}                                                                                                                    \
	break;
#include "components.inc"
#undef Component
	}

	// TODO: Free component adding. Instead of just the count.

	// if (core->entity_set->free_entity_id == core->entity_set->entity_count) // Entity ID is caught up
	// {
	// 	i32 entity_id = core->entity_set->entity_count;
	// 	core->entity_set->entity_count++;
	// 	core->entity_set->free_entity_id = core->entity_set->entity_count;

	// 	core->entity_set->entities[entity_id].entity_id = entity_id;
	// 	strcpy(core->entity_set->entities[entity_id].name, name);

	// 	return &core->entity_set->entities[entity_id];
	// }
	// else // Avalable entity ID isn't the latest count
	// {
	// 	i32 entity_id = core->entity_set->free_entity_id;

	// 	core->entity_set->entities[entity_id].entity_id = entity_id;
	// 	strcpy(core->entity_set->entities[entity_id].name, name);

	// 	// Determine the next free ID
	// 	for (int i = 1; i < core->entity_set->entity_count + 1; i++)
	// 	{
	// 		if (core->entity_set->entities[i].entity_id == 0)
	// 		{
	// 			core->entity_set->free_entity_id = i;
	// 			break;
	// 		}
	// 	}

	// 	return &core->entity_set->entities[entity_id];
	// }
}

internal void RemoveComponent(Entity *entity, ComponentType component_type)
{
	switch (component_type)
	{
#define Component(component_struct, component_name)                                              \
	case COMPONENT_##component_name:                                                             \
	{                                                                                            \
		component_struct##Component *component = entity->components[COMPONENT_##component_name]; \
		R_DEV_ASSERT(component, "Entity does not have this component, so it can't remove it.");  \
		component_struct##Component empty_comp = {0};                                            \
		core->component_set->component_name##_components[component->component_id] = empty_comp;  \
		entity->components[COMPONENT_##component_name] = 0;                                      \
	}                                                                                            \
	break;
#include "components.inc"
#undef Component
	}
}

// NOTE(tjr): Wraps an entity up into a reference that can be safely accessed and tested for validity.
internal void RequestEntity(Entity *entity, EntityReference *entity_reference)
{
	R_DEV_ASSERT(entity->reference_count + 1 < MAX_ENTITY_REFERENCES, "Did not account for this many references.");

	entity_reference->entity = entity;
	if (entity->reference_count == entity->free_reference_index)
	{
		entity_reference->reference_index = entity->reference_count;

		entity->references[entity->reference_count] = entity_reference;
		entity->reference_count++;
		entity->free_reference_index++;
	}
	else
	{
		entity_reference->reference_index = entity->free_reference_index;

		entity->references[entity->free_reference_index] = entity_reference;
		b8 found_free_index = 0;
		for (int i = 1; i < entity->reference_count + 1; i++)
		{
			if (entity->references[i] == 0)
			{
				found_free_index = 1;
				entity->free_reference_index = i;
				break;
			}
		}
		R_DEV_ASSERT(found_free_index, "A free index could not be found.");
	}
}

internal void FreeEntityReference(EntityReference *reference)
{
	R_DEV_ASSERT(reference->entity, "Entity has already been freed.");

	reference->entity->references[reference->reference_index] = 0;

	b8 found_free_index = 0;
	for (int i = 1; i < reference->entity->reference_count + 1; i++)
	{
		if (!reference->entity->references[i])
		{
			found_free_index = 1;
			reference->entity->free_reference_index = i;
			break;
		}
	}
	R_DEV_ASSERT(found_free_index, "A free index could not be found.");

	reference->entity = 0;
	reference->reference_index = -1;
}

internal void DeleteEntity(Entity *entity)
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		switch (i)
		{
#define Component(component_struct, component_name)                                              \
	case COMPONENT_##component_name:                                                             \
	{                                                                                            \
		component_struct##Component *component = entity->components[COMPONENT_##component_name]; \
		if (component)                                                                           \
		{                                                                                        \
			RemoveComponent(entity, COMPONENT_##component_name);                                 \
		}                                                                                        \
	}                                                                                            \
	break;
#include "components.inc"
#undef Component
		}
	}

	// Free all references held by the entity
	for (int i = 0; i < entity->reference_count; i++)
	{
		EntityReference *entity_reference = entity->references[i];
		if (entity_reference)
		{
			entity_reference->entity = 0;
			entity_reference->reference_index = -1;
		}
	}

	i32 deleted_entity_ID = entity->entity_id;

	Entity null_entity = {0};
	*entity = null_entity;

	if (deleted_entity_ID < core->entity_set->free_entity_id)
	{
		core->entity_set->free_entity_id = deleted_entity_ID;
	}
}

/* --- Entity component helper functions --- */
// Used for quick setup of a new entity, if more indepth creation is required - create the components directly through the AddComponent macro

internal void AttachPosition(Entity *entity, v2 world_position)
{
	PositionComponent position = {
		.position = world_position,
	};
	AddComponent(entity, COMPONENT_position, &position);
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
	AddComponent(entity, COMPONENT_sprite, &sprite);

	if (IsSpriteDynamic(sprite_enum))
	{
		AnimationComponent animation = {
			.flags = ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat,
			.current_frame = 0,
			.interval_mult = 1.0f,
			.frame_start_time = core->elapsed_world_time,
		};
		AddComponent(entity, COMPONENT_animation, &animation);
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
	AddComponent(entity, COMPONENT_sub_sprite, &sub_sprite);

	if (IsSpriteDynamic(sub_sprites[0].sprite_enum))
	{
		AnimationComponent animation = {
			.entity_id = entity->entity_id,
			.flags = ANIMATION_FLAG_playing | ANIMATION_FLAG_repeat,
			.current_frame = 0,
			.interval_mult = 1.0f,
			.frame_start_time = core->elapsed_world_time,
		};
		AddComponent(entity, COMPONENT_animation, &animation);
	}
}

internal void AttachCollider(Entity *entity, Shape shape, b8 flags)
{
	ColliderComponent collider = {
		.shape = shape,
		.flags = flags,
	};
	AddComponent(entity, COMPONENT_collider, &collider);
}

internal void AttachVelocity(Entity *entity, v2 initial_velocity, v2 acceleration, b8 collide_against)
{
	VelocityComponent velocity = {
		.velocity = initial_velocity,
		.acceleration = acceleration,
		.acceleration_mult = {1.0f, 1.0f},
		.collide_against = collide_against,
	};
	AddComponent(entity, COMPONENT_velocity, &velocity);
}

internal void AttachPhysics(Entity *entity, f32 friction_multiplier, f32 bounce_multiplier)
{
	PhysicsComponent physics = {
		.friction_mult = friction_multiplier,
		.bounce_mult = bounce_multiplier,
	};
	AddComponent(entity, COMPONENT_physics, &physics);
}

// NOTE(tjr): Provides data for axis-based movement.
internal void AttachMovement(Entity *entity, f32 move_speed)
{
	MovementComponent movement = {
		.move_speed = move_speed,
		.move_speed_mult = 1.0f,
	};
	AddComponent(entity, COMPONENT_movement, &movement);
}

// NOTE(tjr): An Arcane Entity includes a state-based automatic sprite switcher, provided via the ArcEntityType enum
internal void AttachArcEntity(Entity *entity, ArcEntityType entity_type, AnimationStateType idle_anim, f32 base_render_layer)
{
	ArcEntityComponent arc_entity = {
		.current_general_state = "Idle",
		.current_animation_state = idle_anim,
	};
	AddComponent(entity, COMPONENT_arc_entity, &arc_entity);

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
	AddComponent(entity, COMPONENT_item, &item);
}

// NOTE(tjr): Triggers callbacks upon entering/exiting the bounds. Requires a collider component that is setup as a COLLIDER_TYPE_trigger
internal void AttachTrigger(Entity *entity, void (*enter_trigger_callback)(OverlappedColliderInfo), void (*exit_trigger_callback)(OverlappedColliderInfo), b8 trigger_against)
{
	TriggerComponent trigger = {
		.trigger_against = trigger_against,
		.enter_trigger_callback = enter_trigger_callback,
		exit_trigger_callback = exit_trigger_callback,
	};
	AddComponent(entity, COMPONENT_trigger, &trigger);
}

// NOTE(tjr): A storage system that can hold multiple ItemComponents
internal void AttachStorage(Entity *entity, i32 storage_size)
{
	StorageComponent storage = {
		.storage_size = storage_size,
	};
	AddComponent(entity, COMPONENT_storage, &storage);
}

// NOTE(tjr): A component that updates the entity's position in the world based on camera movement. Used to simulate a 3D effect.
internal void AttachParallax(Entity *entity, v2 parallax_amount, v2 desired_position)
{
	ParallaxComponent parallax = {
		.parallax_amount = parallax_amount,
		.desired_position = desired_position,
	};
	AddComponent(entity, COMPONENT_parallax, &parallax);
}

// TODO: Think of a more elegant solution to this whole background/foreground problem?
// NOTE(tjr): Attaches a position, parallax, and sprite component (marked as background for the backdrop rendering).
internal void SetupBackgroundEntity(Entity *entity, v2 desired_position, SpriteType sprite_type, f32 render_layer, v2 parallax_amount)
{
	PositionComponent position = {
		.position = desired_position,
	};
	AddComponent(entity, COMPONENT_position, &position);

	SpriteComponent sprite = {
		.sprite_data = {
			sprite_type,
			v2(0, 0),
			render_layer,
			v2(1.0f, 1.0f),
		},
		.is_background_sprite = 1,
	};
	AddComponent(entity, COMPONENT_sprite, &sprite);

	ParallaxComponent parallax = {
		.parallax_amount = parallax_amount,
		.desired_position = desired_position,
	};
	AddComponent(entity, COMPONENT_parallax, &parallax);
}