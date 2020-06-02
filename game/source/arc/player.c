internal void PreMoveUpdatePlayer()
{
	Assert(core->run_data->character_entity && core->run_data->character_entity->entity_id);
	
	PositionComponent *position_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	PhysicsBodyComponent *body_comp = GetPhysicsBodyComponentFromEntityID(core->run_data->character_entity->entity_id);
	MovementComponent *movement_comp = GetMovementComponentFromEntityID(core->run_data->character_entity->entity_id);
	ArcEntityComponent *arc_entity_comp = GetArcEntityComponentFromEntityID(core->run_data->character_entity->entity_id);
	SpriteComponent *sprite_comp = GetSpriteComponentFromEntityID(core->run_data->character_entity->entity_id);
	AnimationComponent *animation_comp = GetAnimationComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	b8 is_sprinting = 0;
	if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_down[KEY_a])
	{
		if (platform->key_down[KEY_d])
			movement_comp->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				movement_comp->axis_x = -1.5;
				is_sprinting = 1;
			}
			else
			{
				movement_comp->axis_x = -0.5f;
			}
		}
	}
	else if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_down[KEY_d])
	{
		if (platform->key_down[KEY_a])
			movement_comp->axis_x = 0.0f;
		else
		{
			if (platform->key_down[KEY_shift])
			{
				movement_comp->axis_x = 1.5;
				is_sprinting = 1;
			}
			else
			{
				movement_comp->axis_x = 0.5f;
			}
		}
	}
	else
	{
		movement_comp->axis_x = 0.0f;
	}
	
	if (fabsf(body_comp->velocity.x) < fabsf(movement_comp->move_speed * movement_comp->axis_x))
		body_comp->force.x = movement_comp->axis_x * (1200 / body_comp->mass_data.inv_mass);
	
	if (core->run_data->editor_state == EDITOR_STATE_none && platform->key_pressed[KEY_space])
	{
		body_comp->force.y = -10000.0f / body_comp->mass_data.inv_mass;
	}
	
	// NOTE(randy): Update animation state
	{
		if (movement_comp->axis_x < 0.0f)
		{
			sprite_comp->is_flipped = 1;
		}
		else if (movement_comp->axis_x > 0.0f)
		{
			sprite_comp->is_flipped = 0;
		}
		
		if (movement_comp->axis_x == 0)
		{
			SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_idle, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
		}
		else
		{
			if (is_sprinting)
			{
				SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_sprinting, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
			else
			{
				SetArcEntityAnimationState(arc_entity_comp, ARC_ENTITY_ANIMATION_STATE_player_walking, ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat, 0);
			}
		}
	}
	
	// Acceleration calculations?
}

internal void PostMoveUpdatePlayer()
{
}

internal void InteractableUpdate()
{
	PositionComponent *player_pos_comp = GetPositionComponentFromEntityID(core->run_data->character_entity->entity_id);
	PhysicsBodyComponent *player_body_comp = GetPhysicsBodyComponentFromEntityID(core->run_data->character_entity->entity_id);
	
	InteractableComponent *highest_priority_interactable = 0;
	for (i32 i = 0; i < core->run_data->entity_components.interactable_component_count; i++)
	{
		InteractableComponent *inter_comp = &core->run_data->entity_components.interactable_components[i];
		if (inter_comp->parent_entity_id)
		{
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(inter_comp->parent_entity_id);
			Assert(pos_comp);
			
			c2Shape inter_shape = inter_comp->bounds;
			AddPositionOffsetToShape(&inter_shape,
									 inter_comp->bounds_type,
									 pos_comp->position);
			
			c2Shape player_shape = player_body_comp->shape;
			AddPositionOffsetToShape(&player_shape,
									 player_body_comp->shape_type,
									 player_pos_comp->position);
			
			c2Manifold manifold = {0};
			GenerateCollisionManifold(inter_shape, inter_comp->bounds_type,
									  player_shape, player_body_comp->shape_type,
									  &manifold);
			if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
			{
				// NOTE(randy): Player is within interaction bounds.
				if (!highest_priority_interactable ||
					inter_comp->priority > highest_priority_interactable->priority)
				{
					highest_priority_interactable = inter_comp;
				}
			}
		}
	}
	
	core->run_data->current_interactable = highest_priority_interactable;
	if (highest_priority_interactable && platform->key_pressed[KEY_e])
	{
		if (highest_priority_interactable->interact_callback)
		{
			highest_priority_interactable->interact_callback(GetEntityWithID(highest_priority_interactable->parent_entity_id));
		}
	}
}