// NOTE(tjr): Everything in the game world uses a BottomCentre pivot point by default.
internal void GetRectVerticies(v2 vertices[], v2 size, v2 offset)
{
	vertices[0] = V2AddV2(v2(size.x / -2.0f, -size.y), offset);
	vertices[1] = V2AddV2(v2(size.x / 2.0f, -size.y), offset);
	vertices[2] = V2AddV2(v2(size.x / 2.0f, 0.0f), offset);
	vertices[3] = V2AddV2(v2(size.x / -2.0f, 0.0f), offset);
}

internal i32 GetSerparatingAxes(v2 in_separating_axes[8], Shape shape1, Shape shape2)
{
	int separating_axes_count = 0;
	for (int i = 0; i < shape1.vertex_count; i++)
	{
		v2 vert = shape1.vertices[i];
		v2 nextVert = shape1.vertices[i + 1 == shape1.vertex_count ? 0 : i + 1];

		v2 edgeVector = V2SubtractV2(nextVert, vert);
		v2 normalVector = {-edgeVector.y, edgeVector.x};
		float normalMagnitude = SquareRoot(normalVector.x * normalVector.x + normalVector.y * normalVector.y);
		v2 normalisedNormalVector = {normalVector.x / normalMagnitude, normalVector.y / normalMagnitude}; // lmao

		in_separating_axes[separating_axes_count] = normalisedNormalVector;
		separating_axes_count++;

		b8 isParallel = 0;
		for (int j = 0; j < separating_axes_count; j++)
		{
			/* float dot = in_separating_axes[j].x * normalisedNormalVector.x + in_separating_axes[j].y * normalisedNormalVector.y;
			float existingAxisMagnitute = SquareRoot(in_separating_axes[j].x * in_separating_axes[j].x + in_separating_axes[j].y + in_separating_axes[j].y);
			float differenceAngle = (float)acos(dot / (existingAxisMagnitute * normalMagnitude)); */

			if (in_separating_axes[j].x == normalisedNormalVector.x && in_separating_axes[j].y == normalisedNormalVector.y)
			{
				isParallel = 1;
				break;
			}
		}

		if (!isParallel)
		{
			in_separating_axes[separating_axes_count] = normalisedNormalVector;
			separating_axes_count++;
		}
	}

	for (int i = 0; i < shape2.vertex_count; i++)
	{
		v2 vert = shape2.vertices[i];
		v2 nextVert = shape2.vertices[i + 1 == shape2.vertex_count ? 0 : i + 1];

		v2 edgeVector = V2SubtractV2(nextVert, vert);
		v2 normalVector = {-edgeVector.y, edgeVector.x};
		float normalMagnitude = SquareRoot(normalVector.x * normalVector.x + normalVector.y * normalVector.y);
		v2 normalisedNormalVector = {normalVector.x / normalMagnitude, normalVector.y / normalMagnitude}; // lmao

		in_separating_axes[separating_axes_count] = normalisedNormalVector;
		separating_axes_count++;

		b8 isParallel = 0;
		for (int j = 0; j < separating_axes_count; j++)
		{
			if (in_separating_axes[j].x == normalisedNormalVector.x && in_separating_axes[j].y == normalisedNormalVector.y)
			{
				isParallel = 1;
				break;
			}
		}

		// Still a duplicate axis goin in the negative direciton. Can this be removed as well?

		if (!isParallel)
		{
			in_separating_axes[separating_axes_count] = normalisedNormalVector;
			separating_axes_count++;
		}
	}

	return separating_axes_count;
}

internal Shape GetRectangleShape(v2 size, v2 offset)
{
	Shape shape = {0};
	GetRectVerticies(shape.vertices, size, offset);
	shape.vertex_count = 4;

	return shape;
}

internal Shape GetSlopeShape(v2 size)
{
	Shape shape = {
		.vertices = {
			v2(0.0f, 0.0f),
			size.x < 0.0f ? v2(size.x, 0.0f) : v2(size.x, size.y),
			size.x < 0.0f ? v2(size.x, size.y) : v2(size.x, 0.0f),
		},
		.vertex_count = 3,
	};

	return shape;
}

// NOTE(tjr): Returns the minimum (x) and maximum (y) of an array of vertices onto a particular axis
internal v2 ProjectVerticesOntoAxis(v2 vertices[], int vertex_count, v2 axis, v2 position_offset)
{
	v2 projection;
	b8 first = 1;
	for (int i = 0; i < vertex_count; i++)
	{
		v2 vertexPos = V2AddV2(vertices[i], position_offset);
		float vertexProjection = vertexPos.x * axis.x + vertexPos.y * axis.y; // Is equal to the dot product between this vertex position and the axis

		// Ts2dPushLine(core->renderer, v4(0.8f, 0.8f, 0.8f, 1.0f), vertexPos, v2(vertexPos.x + 20.0f, vertexPos.y + 20.0f));

		if (first)
		{
			projection.x = vertexProjection;
			projection.y = vertexProjection;
			first = 0;
		}
		else
		{
			if (vertexProjection < projection.x)
			{
				projection.x = vertexProjection; // New minimum vertex
			}
			else if (vertexProjection > projection.y)
			{
				projection.y = vertexProjection; // New maximum vertex
			}
		}
	}

	return projection;
}

// NOTE(tjr): Get the colliders that are overlapping with the provided shape. Returns a count of the amount.
internal i32 GetOverlappingCollidersAtPosition(ColliderComponent *in_overlapping_colliders[], Shape shape, v2 position, b8 overlap_with)
{
	PushDebugShapeForDuration(shape, position, v3(1.0f, 0.0f, 0.0f), 2.5f);

	int overlap_count = 0;

	ChunkData *chunks[1];
	chunks[0] = GetChunkAtPosition(position); // Need to think about how this works?
	for (int i = 0; i < 1; i++)
	{
		ChunkData *chunk = chunks[i];

		for (int j = 0; j < chunk->entity_count; j++)
		{
			ColliderComponent *supposed_collider_comp = &core->world_data->entity_components.collider_components[chunk->entity_ids[j]];
			if (supposed_collider_comp->parent_entity)
			{
				Entity *supposed_entity = supposed_collider_comp->parent_entity;
				PositionComponent *supposed_position_comp = supposed_entity->components[COMPONENT_position];

				R_DEV_ASSERT(supposed_position_comp && supposed_collider_comp, "Invalid components");

				if (overlap_with & supposed_collider_comp->flags) // Ensure it can collide against this type
				{
					v2 separating_axes[MAX_COLLIDER_VERTICES * 2] = {0};
					int separating_axes_count = GetSerparatingAxes(separating_axes, shape, supposed_collider_comp->shape);

					// NOTE(tjr): Determine if the entity is overlapping this trigger.
					b8 is_overlapping = 1;
					OverlappedColliderInfo overlap_info = {0};
					for (int k = 0; k < separating_axes_count; k++)
					{
						v2 proj1 = ProjectVerticesOntoAxis(shape.vertices, shape.vertex_count, separating_axes[k], position);
						v2 proj2 = ProjectVerticesOntoAxis(supposed_collider_comp->shape.vertices, supposed_collider_comp->shape.vertex_count, separating_axes[k], supposed_position_comp->position);

						if (!(proj1.x < proj2.y && proj1.y > proj2.x))
						{
							is_overlapping = 0;
							break;
						}
					}

					if (is_overlapping)
					{
						R_DEV_ASSERT(overlap_count + 1 < MAX_OVERLAPPING_COLLIDERS, "Did not expect this many overlapping colliders.");
						in_overlapping_colliders[overlap_count++] = supposed_collider_comp;
					}
				}
			}
		}
	}

	return overlap_count;
}

// NOTE(tjr): Update all velocity-based colliders
internal void AdvanceVelocity()
{
	for (int j = 0; j < core->world_data->entity_components.velocity_component_count; j++)
	{
		VelocityComponent *instigator_velocity_comp = &core->world_data->entity_components.velocity_components[j];
		if (instigator_velocity_comp->parent_entity)
		{
			Entity *instigator_entity = instigator_velocity_comp->parent_entity;

			PositionComponent *instigator_position_comp = instigator_entity->components[COMPONENT_position];
			ColliderComponent *instigator_collider_comp = instigator_entity->components[COMPONENT_collider];
			PhysicsComponent *instigator_physics_comp = instigator_entity->components[COMPONENT_physics];
			R_DEV_ASSERT(instigator_position_comp && instigator_collider_comp && instigator_physics_comp, "Missing components required for update.");

			// NOTE(tjr): Update velocity
			v2 previous_velocity = instigator_velocity_comp->velocity;
			{
				instigator_velocity_comp->ideal_velocity.y = TERMINAL_VELOCITY;
				instigator_velocity_comp->acceleration.y = WORLD_GRAVITY * instigator_velocity_comp->acceleration_mult.y;

				v2 *velocity = &instigator_velocity_comp->velocity;
				v2 *ideal_velocity = &instigator_velocity_comp->ideal_velocity;

				f32 friction = WORLD_FRICTION * instigator_velocity_comp->previous_friction * instigator_physics_comp->friction_mult;

				// Need to accelerate + decelerate at the same time to prevent air-stasis

				// Determine new X velocity
				if (velocity->x < 0.0f) // Negative velocity
				{
					if (ideal_velocity->x < 0.0f) // Negative ideal
					{
						if (velocity->x < ideal_velocity->x)
						{
							velocity->x += core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x > ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
						else if (velocity->x > ideal_velocity->x)
						{
							velocity->x -= core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x < ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
					}
					else if (ideal_velocity->x > 0.0f) // Positive ideal
					{
						if (velocity->x < 0.0f)
						{
							velocity->x += core->world_delta_t * (instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x + friction);
						}
						else
						{
							velocity->x += core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x > ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
					}
					else // Zeroed ideal
					{
						velocity->x += core->world_delta_t * friction;
						if (velocity->x > 0.0f)
							velocity->x = 0.0f;
					}
				}
				else // Positive velocity
				{
					if (ideal_velocity->x < 0.0f) // Negative ideal
					{
						if (velocity->x > 0.0f)
						{
							velocity->x -= core->world_delta_t * (instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x + friction);
						}
						else
						{
							velocity->x -= core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x < ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
					}
					else if (ideal_velocity->x > 0.0f) // Positive ideal
					{
						if (velocity->x < ideal_velocity->x)
						{
							velocity->x += core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x > ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
						else if (velocity->x > ideal_velocity->x)
						{
							velocity->x -= core->world_delta_t * instigator_velocity_comp->acceleration.x * instigator_velocity_comp->acceleration_mult.x;
							if (velocity->x < ideal_velocity->x)
							{
								velocity->x = ideal_velocity->x;
							}
						}
					}
					else // Zeroed ideal
					{
						velocity->x -= core->world_delta_t * friction;
						if (velocity->x < 0.0f)
							velocity->x = 0.0f;
					}
				}

				// Determine new Y velocity
				// NOTE(tjr): Only temporarily factors in a downward gravity force. Needs to be updated if this will ever change.
				R_DEV_ASSERT(ideal_velocity->y > 0.0f, "Not designed for negative gravity yet");
				if (velocity->y < ideal_velocity->y)
				{
					velocity->y += core->world_delta_t * instigator_velocity_comp->acceleration.y * instigator_velocity_comp->acceleration_mult.y;
					if (velocity->y > ideal_velocity->y)
					{
						velocity->y = ideal_velocity->y;
					}
				}
				else
				{
					velocity->y = ideal_velocity->y;
				}
			}

			v2 extrapolated_position = {instigator_position_comp->position.x + instigator_velocity_comp->velocity.x * core->world_delta_t,
										instigator_position_comp->position.y + instigator_velocity_comp->velocity.y * core->world_delta_t};

			CollisionInfo collisions[MAX_SIMULTANEOUS_COLLISIONS] = {0};
			int collision_count = 0;

			ChunkData *surrounding_chunks[9];
			GetSurroundingChunks(surrounding_chunks, instigator_position_comp->position);

			for (int k = 0; k < 9; k++)
			{
				ChunkData *against_chunk = surrounding_chunks[k];

				for (int l = 0; l < against_chunk->entity_count; l++)
				{
					Entity *supposed_entity = &core->world_data->entities[against_chunk->entity_ids[l]];
					if (supposed_entity->entity_id)
					{
						ColliderComponent *supposed_collider_comp = supposed_entity->components[COMPONENT_collider];

						if (supposed_collider_comp &&
							supposed_collider_comp != instigator_collider_comp) // Make sure it's a collider entity
						{
							PositionComponent *supposed_position_comp = supposed_entity->components[COMPONENT_position];

							R_DEV_ASSERT(supposed_position_comp && supposed_collider_comp, "Supposed entity is not a collider.");

							if ((instigator_velocity_comp->collide_against & supposed_collider_comp->flags) == supposed_collider_comp->flags) // Ensure it can collide against this type
							{
								v2 separating_axes[MAX_COLLIDER_VERTICES * 2] = {0};
								int separating_axes_count = GetSerparatingAxes(separating_axes, instigator_collider_comp->shape, supposed_collider_comp->shape);
								R_DEV_ASSERT(separating_axes_count <= 8 && separating_axes_count >= 3, "uhhh");

								// NOTE(tjr): Determine if the entities are colliding, if so provide the relevant data.
								b8 is_colliding = 1;
								CollisionInfo collision_info = {0};
								float minimumOverlap;
								v2 minimum_axis;
								b8 first_iteration = 1;
								for (int m = 0; m < separating_axes_count; m++)
								{
									v2 proj1 = ProjectVerticesOntoAxis(instigator_collider_comp->shape.vertices, instigator_collider_comp->shape.vertex_count, separating_axes[m], extrapolated_position);
									v2 proj2 = ProjectVerticesOntoAxis(supposed_collider_comp->shape.vertices, supposed_collider_comp->shape.vertex_count, separating_axes[m], supposed_position_comp->position);

									if (proj1.x < proj2.y && proj1.y > proj2.x) // Do the projections between each collider overlap?
									{
										float overlapAmount = proj2.x - proj1.y;
										if (first_iteration || fabsf(overlapAmount) < fabsf(minimumOverlap))
										{
											first_iteration = 0;
											minimumOverlap = overlapAmount;
											minimum_axis = separating_axes[m];
										}
									}
									else
									{
										is_colliding = 0;
										break;
									}
								}

								if (is_colliding)
								{
									v2 mtv = {minimum_axis.x * minimumOverlap, minimum_axis.y * minimumOverlap};
									// float newMag = SquareRoot(mtv.x * mtv.x + mtv.y * mtv.y);
									// float currentMag = SquareRoot(closestMTV.x * closestMTV.x + closestMTV.y * closestMTV.y);
									// if (fabsf(newMag) > fabsf(currentMag))
									// {
									// 	closestMTV = mtv;
									// 	closest_mtv_axis = minimum_axis;
									// }

									collision_info.is_valid = 1;
									collision_info.instigator = instigator_entity;
									collision_info.victim = supposed_entity;
									collision_info.instigatorMTV = mtv;
									collision_info.collision_axis = minimum_axis;

									collisions[collision_count++] = collision_info;
								}
							}
						}
					}
				}
			}
			R_DEV_ASSERT(collision_count <= MAX_SIMULTANEOUS_COLLISIONS, "Why are there so many collisions?");

			if (collision_count > 0)
			{
				v2 *velocity = &instigator_velocity_comp->velocity;

				//v2 proposed_movement_vector = V2SubtractV2(extrapolated_position, previous_position);

				// f32 frictions[MAX_SIMULTANEOUS_COLLISIONS];
				// i32 friction_count = 0;

				b8 first_iteration = 1;
				v2 closest_mtv = {0.0f, 0.0f};
				v2 closest_mtv_axis = {0.0f, 0.0f};
				i32 closest_entity_index = 0;

				float x_greatest_mtv = 0.0f;
				float y_greatest_mtv = 0.0f;

				float closest_bounce_mult = 1.0f;
				float closest_friction_mult = 1.0f;

				for (int j = 0; j < collision_count; j++)
				{
					CollisionInfo *collision = &collisions[j];

					PhysicsComponent *victim_physics_comp = collision->victim->components[COMPONENT_physics];
					R_DEV_ASSERT(victim_physics_comp, "Collision entity has no physics component for friction calculation.");

					float newMag = SquareRoot(collision->instigatorMTV.x * collision->instigatorMTV.x + collision->instigatorMTV.y * collision->instigatorMTV.y);
					float currentMag = SquareRoot(closest_mtv.x * closest_mtv.x + closest_mtv.y * closest_mtv.y);
					if (first_iteration || fabsf(newMag) > fabsf(currentMag))
					{
						closest_entity_index = collision->victim->entity_id;
						closest_mtv = collision->instigatorMTV;
						closest_mtv_axis = collision->collision_axis;
						closest_bounce_mult = victim_physics_comp->bounce_mult;
						closest_friction_mult = victim_physics_comp->friction_mult;
					}

					if (first_iteration || fabsf(collision->instigatorMTV.x) > fabsf(x_greatest_mtv))
					{
						x_greatest_mtv = collision->instigatorMTV.x;
						//x_greatest_mtv_axis = minimum_axis;
					}
					if (first_iteration || fabsf(collision->instigatorMTV.y) > fabsf(y_greatest_mtv))
					{
						y_greatest_mtv = collision->instigatorMTV.y;
						//y_greatest_mtv_axis = minimum_axis;
					}
					first_iteration = 0;
				}

				if (fabsf(closest_mtv_axis.x) > fabsf(closest_mtv_axis.y)) // Closest collision is horizontal
				{
					velocity->x = previous_velocity.x * (-closest_bounce_mult * instigator_physics_comp->bounce_mult);
				}
				else if (fabsf(closest_mtv_axis.y) > fabsf(closest_mtv_axis.x)) // Closest collision is vertical
				{
					velocity->y = previous_velocity.y * (-closest_bounce_mult * instigator_physics_comp->bounce_mult);
				}
				else
				{
					R_BREAK("Perfect right angle collision?");
					velocity->x = previous_velocity.x * (-closest_bounce_mult * instigator_physics_comp->bounce_mult);
					velocity->y = previous_velocity.y * (-closest_bounce_mult * instigator_physics_comp->bounce_mult);
				}

				if (fabsf(velocity->x) < 1.0f)
					velocity->x = 0.0f;
				if (fabsf(velocity->y) < 1.0f)
					velocity->y = 0.0f;

				//Log("Closest Collision: %i", closest_entity_index);

				// f32 average_friction = 0.0f;
				// if (friction_count > 0)
				// {
				// 	f32 friction_sum = 0.0f;
				// 	for (int j = 0; j < friction_count; j++)
				// 		friction_sum += frictions[j];

				// 	average_friction = friction_sum / friction_count;
				// }

				instigator_position_comp->position = V2AddV2(extrapolated_position, v2(x_greatest_mtv, y_greatest_mtv));
				instigator_velocity_comp->previous_friction = 1.0f; //average_friction;
			}
			else
			{
				instigator_position_comp->position = extrapolated_position;
				instigator_velocity_comp->previous_friction = 0.0f;
			}
		}
	}
}

// NOTE(tjr): Update all trigger-based colliders
internal void UpdateTriggers()
{
	for (int i = 0; i < core->world_data->entity_components.trigger_component_count; i++)
	{
		TriggerComponent *trigger_comp = &core->world_data->entity_components.trigger_components[i];
		if (trigger_comp->parent_entity)
		{
			Entity *trigger_entity = trigger_comp->parent_entity;
			ColliderComponent *trigger_collider_comp = trigger_entity->components[COMPONENT_collider];
			PositionComponent *trigger_position_comp = trigger_entity->components[COMPONENT_position];
			R_DEV_ASSERT(trigger_collider_comp && trigger_position_comp, "Invalid components.");
			R_DEV_ASSERT(trigger_collider_comp->flags & COLLIDER_FLAGS_trigger, "Collider is not set-up as a trigger type.");

			OverlappedColliderInfo overlaps[MAX_OVERLAPPING_COLLIDERS] = {0};
			int overlap_count = 0;

			ChunkData *surrounding_chunks[9];
			GetSurroundingChunks(surrounding_chunks, trigger_position_comp->position);

			for (int k = 0; k < 9; k++)
			{
				ChunkData *against_chunk = surrounding_chunks[k];

				for (int l = 0; l < against_chunk->entity_count; l++)
				{
					ColliderComponent *supposed_collider_comp = &core->world_data->entity_components.collider_components[against_chunk->entity_ids[l]];
					if (supposed_collider_comp->parent_entity)
					{
						Entity *supposed_entity = supposed_collider_comp->parent_entity;

						if (&core->world_data->entity_components.collider_components[against_chunk->entity_ids[l]] !=
							trigger_collider_comp) // Ensure it's not being tested against itself
						{
							PositionComponent *supposed_position_comp = supposed_entity->components[COMPONENT_position];

							R_DEV_ASSERT(supposed_position_comp && supposed_collider_comp, "Invalid components");

							if ((trigger_comp->trigger_against & supposed_collider_comp->flags) == supposed_collider_comp->flags) // Ensure it can collide against this type
							{
								v2 separating_axes[MAX_COLLIDER_VERTICES * 2] = {0};
								int separating_axes_count = GetSerparatingAxes(separating_axes, trigger_collider_comp->shape, supposed_collider_comp->shape);

								// NOTE(tjr): Determine if the entity is overlapping this trigger.
								b8 is_overlapping = 1;
								OverlappedColliderInfo overlap_info = {0};
								for (int m = 0; m < separating_axes_count; m++)
								{
									v2 proj1 = ProjectVerticesOntoAxis(trigger_collider_comp->shape.vertices, trigger_collider_comp->shape.vertex_count, separating_axes[m], trigger_position_comp->position);
									v2 proj2 = ProjectVerticesOntoAxis(supposed_collider_comp->shape.vertices, supposed_collider_comp->shape.vertex_count, separating_axes[m], supposed_position_comp->position);

									if (proj1.x < proj2.y && proj1.y > proj2.x) // Do the projections between each collider overlap?
									{
										// float overlapAmount = proj2.x - proj1.y;
										// if (first_iteration || fabsf(overlapAmount) < fabsf(minimumOverlap))
										// {
										// 	first_iteration = 0;
										// 	minimumOverlap = overlapAmount;
										// 	minimum_axis = separatingAxes[k];
										// }
									}
									else
									{
										is_overlapping = 0;
										break;
									}
								}

								if (is_overlapping)
								{
									//v2 mtv = {minimum_axis.x * minimumOverlap, minimum_axis.y * minimumOverlap};

									overlap_info.overlapped_entity = supposed_entity;

									R_DEV_ASSERT(overlap_count + 1 < MAX_OVERLAPPING_COLLIDERS, "Did not expect this many overlapping colliders.");
									overlaps[overlap_count++] = overlap_info;
								}
							}
						}
					}
				}
			}

			// Exit trigger testing
			for (int j = 0; j < trigger_comp->previous_overlaps_count; j++)
			{
				// NOTE(tjr): In rare circumstances this deleted entity could be replaced over the frame and pass as the previous entity, need to find a way around this somehow?Z
				R_DEV_ASSERT(trigger_comp->previous_overlaps[j].overlapped_entity->entity_id > 0, "Previous entity has been deleted. How should this behave?");

				b8 does_exit_trigger = 1;
				for (int k = 0; k < overlap_count; k++)
				{
					if (trigger_comp->previous_overlaps[j].overlapped_entity == overlaps[k].overlapped_entity)
					{
						does_exit_trigger = 0;
						break;
					}
				}

				if (does_exit_trigger)
				{
					// Notify this trigger that something has exited.
					(*trigger_comp->exit_trigger_callback)(trigger_comp->previous_overlaps[j]);
				}
			}

			// Enter trigger testing
			for (int j = 0; j < overlap_count; j++)
			{
				b8 does_exist = 0;
				for (int k = 0; k < trigger_comp->previous_overlaps_count; k++)
				{
					R_DEV_ASSERT(trigger_comp->previous_overlaps[k].overlapped_entity->entity_id > 0, "Previous entity has been deleted. How should this behave?");

					if (overlaps[j].overlapped_entity == trigger_comp->previous_overlaps[k].overlapped_entity)
					{
						does_exist = 1;
						break;
					}
				}

				if (!does_exist)
				{
					// Notify this trigger that something has entered.
					(*trigger_comp->enter_trigger_callback)(overlaps[j]);
				}
			}

			MemoryCopy(trigger_comp->previous_overlaps, overlaps, sizeof(OverlappedColliderInfo) * MAX_OVERLAPPING_COLLIDERS);
			trigger_comp->previous_overlaps_count = overlap_count;
		}
	}
}

internal void RenderColliders()
{
	for (int i = 0; i < core->world_data->entity_components.collider_component_count; i++)
	{
		ColliderComponent *collider_comp = &core->world_data->entity_components.collider_components[i];
		if (collider_comp->parent_entity)
		{
			PositionComponent *position_comp = collider_comp->parent_entity->components[COMPONENT_position];
			R_DEV_ASSERT(position_comp, "Position component not found?");

			v2 world_pos = position_comp->position;
			v2 view_pos = v2view(world_pos);

			for (int j = 0; j < collider_comp->shape.vertex_count; j++)
			{
				int secondPoint = (j == collider_comp->shape.vertex_count - 1 ? 0 : j + 1);
				Ts2dPushLine(
					core->renderer,
					v4(1.0f, 1.0f, 1.0f, 1.0f),
					V2AddV2(view_pos, v2(collider_comp->shape.vertices[j].x * core->camera_zoom, collider_comp->shape.vertices[j].y * core->camera_zoom)),
					V2AddV2(view_pos, v2(collider_comp->shape.vertices[secondPoint].x * core->camera_zoom, collider_comp->shape.vertices[secondPoint].y * core->camera_zoom)));
			}
		}
	}
}