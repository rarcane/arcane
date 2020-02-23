internal Entity *NewEntity(char *name, EntityType type, GeneralisedEntityType generalised_type);
// internal void AddComponent(Entity *entity, ComponentType component_type, void *component);
// internal void RemoveComponent(Entity *entity, ComponentType component_type);

internal PositionComponent GetDefaultPositionComponent()
{
	PositionComponent comp = {0};
	return comp;
}

internal SpriteComponent GetDefaultSpriteComponent()
{
	SpriteComponent comp = {0};
	comp.sprite_data.scale = v2(1.0f, 1.0f);
	comp.sprite_data.tint = v4u(1.0f);
	return comp;
}

internal SubSpriteComponent GetDefaultSubSpriteComponent()
{
	SubSpriteComponent comp = {0};
	for (int i = 0; i < MAX_SUB_SPRITES; i++)
	{
		comp.sub_sprites[i].scale = v2(1.0f, 1.0f);
		comp.sub_sprites[i].tint = v4u(1.0f);
	}
	return comp;
}

internal AnimationComponent GetDefaultAnimationComponent()
{
	AnimationComponent comp = {0};
	comp.flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat;
	comp.frame_start_time = core->world_data->elapsed_world_time;
	comp.interval_mult = 1.0f;
	return comp;
}

internal PhysicsBodyComponent GetDefaultPhysicsBodyComponent()
{
	PhysicsBodyComponent comp = {0};
	return comp;
}

/* internal ColliderComponent GetDefaultColliderComponent()
{
	ColliderComponent comp = {0};
	return comp;
}

internal VelocityComponent GetDefaultVelocityComponent()
{
	VelocityComponent comp = {0};
	comp.acceleration_mult = v2(1.0f, 1.0f);
	return comp;
}

internal PhysicsComponent GetDefaultPhysicsComponent()
{
	PhysicsComponent comp = {0};
	return comp;
} */

internal MovementComponent GetDefaultMovementComponent()
{
	MovementComponent comp = {0};
	comp.move_speed_mult = 1.0f;
	return comp;
}

internal ArcEntityComponent GetDefaultArcEntityComponent()
{
	ArcEntityComponent comp = {0};
	comp.current_general_state = "Idle";
	return comp;
}

internal ItemComponent GetDefaultItemComponent()
{
	ItemComponent comp = {0};
	return comp;
}

internal TriggerComponent GetDefaultTriggerComponent()
{
	TriggerComponent comp = {0};
	return comp;
}

internal StorageComponent GetDefaultStorageComponent()
{
	StorageComponent comp = {0};
	return comp;
}

internal ParallaxComponent GetDefaultParallaxComponent()
{
	ParallaxComponent comp = {0};
	return comp;
}

internal ParticleEmitterComponent GetDefaultParticleEmitterComponent()
{
	ParticleEmitterComponent comp = {0};
	comp.start_time = core->world_data->elapsed_world_time;
	return comp;
}