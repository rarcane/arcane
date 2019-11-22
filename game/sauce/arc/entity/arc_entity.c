// internal void InitialiseArcEntities()
// {
// #define Entity(entity_name, entity_general_states, entity_animation_states)
// 	{
// 		arc_entity_data[ARC_ENTITY_##entity_name].general_states = {"sadasd", "sdwegw"};
// 	}
// }

// // NOTE(tjr): Updates sprite to the relevant animation.
// internal void UpdateArcEntities(ArcEntityComponent arc_entity_components[], i32 count)
// {
// 	for (int i = 0; i < count; i++)
// 	{
// 		ArcEntityComponent *arc_entity_comp = &arc_entity_components[i];
// 		Entity *entity = &core->entities[arc_entity_comp->entity_id];
// 		SpriteComponent *sprite_comp = entity->components[COMPONENT_sprite];
// 		AnimationComponent *animation_comp = entity->components[COMPONENT_animation];
// 		R_DEV_ASSERT(sprite_comp && animation_comp, "Arc entity does not contain the components required for an update.");

// 	}
// }

//internal AnimationState

internal void SetArcEntityGeneralState(ArcEntityComponent *arc_entity_comp, char *new_general_state)
{
	R_TODO;
}

internal void SetArcEntityAnimationState(ArcEntityComponent *arc_entity_comp, AnimationStateType new_animation_state, b8 animation_flags, b8 hard_reset)
{
	if (hard_reset || arc_entity_comp->current_animation_state != new_animation_state)
	{
		Entity *entity = &core->entity_set->entities[arc_entity_comp->entity_id];
		SpriteComponent *sprite_comp = entity->components[COMPONENT_sprite];
		SubSpriteComponent *sub_sprite_comp = entity->components[COMPONENT_sub_sprite];
		AnimationComponent *animation_comp = entity->components[COMPONENT_animation];
		R_DEV_ASSERT((sprite_comp || sub_sprite_comp) && animation_comp, "Arc entity does not contain a sprite/animation component.");

		if (sprite_comp)
		{
			R_DEV_ASSERT(arc_entity_animation_state_data[new_animation_state].dynamic_sprite_count == 1, "Animation state has defined a sub-sprite animation, but a single-sprite component is being used.")
			sprite_comp->sprite_data.sprite_enum = arc_entity_animation_state_data[new_animation_state].dynamic_sprites[0];
		}
		else
		{
			for (int i = 0; i < arc_entity_animation_state_data[new_animation_state].dynamic_sprite_count; i++)
			{
				sub_sprite_comp->sub_sprites[i].sprite_enum = arc_entity_animation_state_data[new_animation_state].dynamic_sprites[i];
			}

			sub_sprite_comp->sub_sprite_count = arc_entity_animation_state_data[new_animation_state].dynamic_sprite_count;
		}

		animation_comp->frame_start_time = GetCurrentWorldTime();
		animation_comp->current_frame = 0;
		animation_comp->flags = animation_flags;

		arc_entity_comp->current_animation_state = new_animation_state;
	}
}