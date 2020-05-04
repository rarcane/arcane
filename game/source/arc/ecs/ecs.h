// NOTE(randy): Create a new generic entity.
internal Entity *NewEntity(char *name, GeneralisedEntityType generalised_type);
// NOTE(randy): Remmoves the provided generic entity.
internal void DeleteEntity(Entity *entity);
// NOTE(randy): Gets entity with the provided ID.
// indexes into entity array at ID - 1
internal Entity *GetEntityWithID(i32 id);

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

internal AnimationComponent GetDefaultAnimationComponent()
{
	AnimationComponent comp = {0};
	comp.flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat;
	comp.frame_start_time = core->run_data->world.elapsed_world_time;
	comp.interval_mult = 1.0f;
	return comp;
}

internal PhysicsBodyComponent GetDefaultPhysicsBodyComponent()
{
	PhysicsBodyComponent comp = {0};
	return comp;
}

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
	comp.start_time = core->run_data->world.elapsed_world_time;
	return comp;
}