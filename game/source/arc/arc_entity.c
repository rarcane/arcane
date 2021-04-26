/*
internal void SetArcEntityGeneralState(ArcEntityComponent *arc_entity_comp, char *new_general_state)
{
	Assert(0);
}
 */

internal void SetArcEntityAnimationState(Entity *entity, ArcEntityAnimationState new_animation_state, b8 animation_flags, b8 hard_reset)
{
	if (hard_reset || entity->current_animation_state != new_animation_state)
	{
		entity->sprite_data.sprite = global_arc_entity_animation_state_data[new_animation_state].dynamic_sprite;
		
		entity->frame_start_time = GetWorldData()->elapsed_world_time;
		entity->current_frame = 0;
		entity->animation_flags = animation_flags;
		
		entity->current_animation_state = new_animation_state;
	}
}