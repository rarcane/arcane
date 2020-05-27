internal void SetArcEntityGeneralState(ArcEntityComponent *arc_entity_comp, char *new_general_state)
{
	Assert(0);
}

internal void SetArcEntityAnimationState(ArcEntityComponent *arc_entity_comp, ArcEntityAnimationState new_animation_state, b8 animation_flags, b8 hard_reset)
{
	if (hard_reset || arc_entity_comp->current_animation_state != new_animation_state)
	{
		SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(arc_entity_comp->parent_entity_id);
		AnimationComponent *animation_comp = GetAnimationComponentFromEntityID(arc_entity_comp->parent_entity_id);
		
		sprite_comp->sprite_data.dynamic_sprite = arc_entity_animation_state_data[new_animation_state].dynamic_sprite;
		
		animation_comp->frame_start_time = core->run_data->world.elapsed_world_time;
		animation_comp->current_frame = 0;
		animation_comp->flags = animation_flags;
		
		arc_entity_comp->current_animation_state = new_animation_state;
	}
}