internal void SetArcEntityGeneralState(ArcEntityComponent *arc_entity_comp, char *new_general_state)
{
	R_TODO;
}

internal void SetArcEntityAnimationState(ArcEntityComponent *arc_entity_comp, ArcEntityAnimationState new_animation_state, b8 animation_flags, b8 hard_reset)
{
	if (hard_reset || arc_entity_comp->current_animation_state != new_animation_state)
	{
		Entity *entity = arc_entity_comp->parent_entity;
		SpriteComponent *sprite_comp = entity->components[COMPONENT_sprite];
		AnimationComponent *animation_comp = entity->components[COMPONENT_animation];
		R_DEV_ASSERT(sprite_comp && animation_comp, "Arc entity does not contain a sprite/animation component.");

		sprite_comp->sprite_data.dynamic_sprite = arc_entity_animation_state_data[new_animation_state].dynamic_sprite;

		animation_comp->frame_start_time = core->world_data->elapsed_world_time;
		animation_comp->current_frame = 0;
		animation_comp->flags = animation_flags;

		arc_entity_comp->current_animation_state = new_animation_state;
	}
}