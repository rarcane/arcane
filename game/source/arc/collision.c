#ifdef DEVELOPER_TOOLS
internal void RenderColliders()
{
	if (!(core->run_data->debug_flags & DEBUG_FLAGS_draw_collision))
		return;
	
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_physical);)
	{
		if (platform->left_mouse_pressed &&
			IsV2OverlappingShape(GetMousePositionInWorldSpace(), GetEntityShapeInWorldspace(entity), entity->physics.shape_type))
		{
			core->run_data->selected_entity = entity;
			platform->left_mouse_pressed = 0;
		}
		
		v3 col = {1.0f, 1.0f, 1.0f};
		if (core->run_data->selected_entity &&
			entity == core->run_data->selected_entity)
			col = v3(1.0f, 0.0f, 0.0f);
		
		PushDebugShape(entity->physics.shape,
					   entity->physics.shape_type,
					   entity->position,
					   col);
	}
	
	if (platform->left_mouse_pressed)
	{
		core->run_data->selected_entity = 0;
	}
	
	if (core->run_data->debug_flags & DEBUG_FLAGS_draw_chunk_grid)
		for (int i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
		{
			v3 colour = v3(1.0f, 1.0f, 1.0f);
			/*
						if (core->run_data->chunk_editor.is_chunk_selected && chunk->x_index == core->run_data->chunk_editor.selected_chunk.x_index && chunk->y_index == core->run_data->chunk_editor.selected_chunk.y_index)
							colour = v3(1.0f, 0.0f, 0.0f);
			 */
			
			PushDebugLine(v2((f32)CHUNK_SIZE * chunk->x_index, (f32)CHUNK_SIZE * chunk->y_index),
						  v2((f32)CHUNK_SIZE * chunk->x_index, (f32)CHUNK_SIZE * chunk->y_index + CHUNK_SIZE),
						  colour);
			PushDebugLine(v2((f32)CHUNK_SIZE * chunk->x_index + CHUNK_SIZE, (f32)CHUNK_SIZE * chunk->y_index),
						  v2((f32)CHUNK_SIZE * chunk->x_index + CHUNK_SIZE, (f32)CHUNK_SIZE * chunk->y_index + CHUNK_SIZE),
						  colour);
			PushDebugLine(v2((f32)CHUNK_SIZE * chunk->x_index, (f32)CHUNK_SIZE * chunk->y_index),
						  v2((f32)CHUNK_SIZE * chunk->x_index + CHUNK_SIZE, (f32)CHUNK_SIZE * chunk->y_index),
						  colour);
			PushDebugLine(v2((f32)CHUNK_SIZE * chunk->x_index, (f32)CHUNK_SIZE * chunk->y_index + CHUNK_SIZE),
						  v2((f32)CHUNK_SIZE * chunk->x_index + CHUNK_SIZE, (f32)CHUNK_SIZE * chunk->y_index + CHUNK_SIZE),
						  colour);
		}
	}
}
#endif

internal void UpdatePhysics()
{
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_physical);)
	{
		// Apply gravity
		if (entity->physics.gravity_multiplier != 0.0f && entity->physics.mass_data.mass != 0.0f)
			entity->physics.force.y += (WORLD_GRAVITY * entity->physics.gravity_multiplier) / entity->physics.mass_data.inv_mass;
		
		v2 previous_pos = entity->position;
		
		// Integrate next position
		v2 acceleration = V2MultiplyF32(entity->physics.force, entity->physics.mass_data.inv_mass);
		entity->physics.velocity = V2AddV2(entity->physics.velocity, V2MultiplyF32(acceleration, core->world_delta_t));
		entity->position = V2AddV2(entity->position, V2MultiplyF32(entity->physics.velocity, core->world_delta_t));
		
		// NOTE(randy): Scuffed true velocity
		v2 pos_diff = V2SubtractV2(entity->position, previous_pos);
		entity->smooth_velocity = V2DivideF32(pos_diff, core->world_delta_t);
		
		entity->physics.force = v2(0.0f, 0.0f);
	}
	
	CollisionPair collision_pairs[MAX_COLLISION_PAIRS];
	i32 pair_count = 0;
	GenerateCollisionPairs(collision_pairs, &pair_count);
	
	for (int i = 0; i < pair_count; i++)
	{
		Entity *entity_a = collision_pairs[i].a;
		Entity *entity_b = collision_pairs[i].b;
		
		c2Manifold manifold = {0};
		
		c2Shape a_shape = entity_a->physics.shape;
		AddPositionOffsetToShape(&a_shape, entity_a->physics.shape_type, entity_a->position);
		
		c2Shape b_shape = entity_b->physics.shape;
		AddPositionOffsetToShape(&b_shape, entity_b->physics.shape_type, entity_b->position);
		
		GenerateCollisionManifold(a_shape, entity_a->physics.shape_type,
								  b_shape, entity_b->physics.shape_type,
								  &manifold);
		//GenerateCollisionManifold(a_body_comp, entity_a->position, b_body_comp, entity_b->position, &manifold);
		
		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			Assert(!(entity_a->physics.mass_data.mass == 0 && entity_b->physics.mass_data.mass == 0)); // NOTE(randy): Two static bodies are colliding?
			
			v2 normal = v2(manifold.n.x, manifold.n.y);
			
			v2 relative_velocity = V2SubtractV2(entity_b->physics.velocity, entity_a->physics.velocity);
			f32 velocity_along_normal = relative_velocity.x * normal.x + relative_velocity.y * normal.y;
			
			if (velocity_along_normal <= 0)
			{
				// Impulse resolution
				f32 restitution = MinimumF32(entity_a->physics.material.restitution, entity_b->physics.material.restitution);
				
				f32 j = -(1 + restitution) * velocity_along_normal;
				j = j / (entity_a->physics.mass_data.inv_mass + entity_b->physics.mass_data.inv_mass);
				
				v2 impulse = V2MultiplyF32(normal, j);
				
				v2 impulse_a = V2MultiplyF32(impulse, entity_a->physics.mass_data.inv_mass);
				entity_a->physics.velocity = V2AddV2(entity_a->physics.velocity, impulse_a);
				
				v2 impulse_b = V2MultiplyF32(impulse, entity_b->physics.mass_data.inv_mass);
				entity_b->physics.velocity = V2AddV2(entity_b->physics.velocity, impulse_b);
				
				// Friction
				v2 new_rv = V2SubtractV2(entity_b->physics.velocity, entity_a->physics.velocity);
				v2 tangent = V2SubtractV2(new_rv, V2MultiplyF32(normal, new_rv.x * normal.x + new_rv.y * normal.y));
				v2Normalise(&tangent);
				
				f32 jt = -(new_rv.x * tangent.x + new_rv.y * tangent.y);
				jt = jt / (entity_a->physics.mass_data.inv_mass + entity_b->physics.mass_data.inv_mass);
				
				f32 mu = MinimumF32(entity_a->physics.material.static_friction, entity_b->physics.material.static_friction); //PythagSolve(entity_a->physics.material.static_friction, entity_b->physics.material.static_friction);
				
				v2 friction_impulse;
				if (fabsf(jt) < j * mu)
					friction_impulse = V2MultiplyF32(tangent, jt);
				else
				{
					f32 dynamic_friction = MinimumF32(entity_a->physics.material.dynamic_friction, entity_b->physics.material.dynamic_friction); //PythagSolve(entity_a->physics.material.dynamic_friction, entity_b->physics.material.dynamic_friction);
					friction_impulse = V2MultiplyF32(tangent, -j * dynamic_friction);
				}
				
				v2 friction_impulse_a = V2MultiplyF32(friction_impulse, entity_a->physics.mass_data.inv_mass);
				entity_a->physics.velocity = V2SubtractV2(entity_a->physics.velocity, friction_impulse_a);
				
				v2 friction_impulse_b = V2MultiplyF32(friction_impulse, entity_b->physics.mass_data.inv_mass);
				entity_b->physics.velocity = V2AddV2(entity_b->physics.velocity, friction_impulse_b);
				
				// Positional correction
				const f32 percent = 0.2f;
				const f32 slop = 0.01f;
				v2 correction = V2MultiplyF32(normal, (MaximumF32(manifold.depths[0] - slop, 0.0f) / (entity_a->physics.mass_data.inv_mass + entity_b->physics.mass_data.inv_mass)) * percent);
				entity_a->position = V2SubtractV2(entity_a->position, V2MultiplyF32(correction, entity_a->physics.mass_data.inv_mass));
				entity_b->position = V2AddV2(entity_b->position, V2MultiplyF32(correction, entity_b->physics.mass_data.inv_mass));
			}
		}
	}
}

internal void GenerateCollisionPairs(CollisionPair pairs[], i32 *count)
{
	for (Entity *entity_a = 0; IncrementEntityWithProperty(&entity_a, ENTITY_PROPERTY_physical);)
	{
		for (Entity *entity_b = 0; IncrementEntityWithProperty(&entity_b, ENTITY_PROPERTY_physical);)
		{
			if (entity_a != entity_b &&
				!(entity_a->physics.mass_data.mass == 0.0f &&
				  entity_b->physics.mass_data.mass == 0.0f) &&
				(entity_a->physics.collide_against & entity_b->physics.type))
			{
				CollisionPair new_pair = {entity_a, entity_b};
				Assert(*count + 1 < MAX_COLLISION_PAIRS)
					pairs[(*count)++] = new_pair;
			}
		}
	}
}

internal void GenerateCollisionManifold(c2Shape a_shape, c2ShapeType a_shape_type,
										c2Shape b_shape, c2ShapeType b_shape_type,
										c2Manifold *manifold)
{
	switch (a_shape_type)
	{
		case C2_SHAPE_TYPE_aabb:
		{
			switch (b_shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					// NOTE(randy): $AABB -> AABB
					c2AABBtoAABBManifold(a_shape.aabb, b_shape.aabb, manifold);
				} break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					// NOTE(randy): $AABB - > capsule
					c2AABBtoCapsuleManifold(a_shape.aabb, b_shape.capsule, manifold);
				} break;
				
				case C2_SHAPE_TYPE_poly:
				{
					// NOTE(randy): $AABB -> Poly
					c2AABBtoPolyManifold(a_shape.aabb, &b_shape.poly, 0, manifold);
				} break;
				
				case C2_SHAPE_TYPE_line:
				{
					//Assert(0);
				} break;
				
				case C2_SHAPE_TYPE_circle :
				{
					// NOTE(randy): $AABB -> Circle
					c2CircletoAABBManifold(b_shape.circle, a_shape.aabb, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				} break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_capsule:
		{
			switch (b_shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					c2AABBtoCapsuleManifold(b_shape.aabb, a_shape.capsule, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					c2CapsuletoPolyManifold(a_shape.capsule, &b_shape.poly, 0, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_line:
				{
					v2 line_vector = V2SubtractV2(b_shape.line.p2, b_shape.line.p1);
					v2 capsule_point_a = v2(a_shape.capsule.a.x, a_shape.capsule.a.y);
					
					v2 point_a_vector_from_line = V2SubtractV2(capsule_point_a,
															   b_shape.line.p1);
					f32 dot = point_a_vector_from_line.x * line_vector.x + point_a_vector_from_line.y * line_vector.y;
					// NOTE(randy): proj a->b = (a dot b / mag^2) * b
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y));
					
					v2 collision_normal = V2SubtractV2(capsule_point_a, V2AddV2(b_shape.line.p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance, collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					//PushDebugLine(p1, V2AddV2(p1, collision_normal), v3(1.0f, 0.0f, 0.0f));
					
					if (collision_distance < a_shape.capsule.r &&
						capsule_point_a.x >= b_shape.line.p1.x &&
						capsule_point_a.x <= b_shape.line.p2.x)
					{
						manifold->n = c2V(-normalised_collision_normal.x, -normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = a_shape.capsule.r - collision_distance;
					}
				} break;
				
				case C2_SHAPE_TYPE_circle :
				{
					// NOTE(randy): $Capsule -> Circle
					c2CircletoCapsuleManifold(b_shape.circle, a_shape.capsule, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				} break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_poly:
		{
			switch (b_shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					c2AABBtoPolyManifold(b_shape.aabb, &a_shape.poly, 0, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					c2CapsuletoPolyManifold(b_shape.capsule, &a_shape.poly, 0, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					c2PolytoPolyManifold(&a_shape.poly, 0, &b_shape.poly, 0, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_line:
				{
					Assert(0); // TODO(randy)
				}
				break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_line:
		{
			switch (b_shape_type)
			{
				case C2_SHAPE_TYPE_capsule:
				{
					// NOTE(randy): Line -> Capsule
					v2 line_vector = V2SubtractV2(a_shape.line.p2, a_shape.line.p1);
					v2 capsule_point_a = v2(b_shape.capsule.a.x, b_shape.capsule.a.y);
					
					v2 point_a_vector_from_line = V2SubtractV2(capsule_point_a,
															   a_shape.line.p1);
					f32 dot = point_a_vector_from_line.x * line_vector.x + point_a_vector_from_line.y * line_vector.y;
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y));
					
					v2 collision_normal = V2SubtractV2(capsule_point_a, V2AddV2(a_shape.line.p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance, collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					if (collision_distance < b_shape.capsule.r &&
						capsule_point_a.x >= a_shape.line.p1.x &&
						capsule_point_a.x <= a_shape.line.p2.x)
					{
						manifold->n = c2V(normalised_collision_normal.x, normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = b_shape.capsule.r - collision_distance;
					}
				}
				break;
				
				case C2_SHAPE_TYPE_circle :
				{
					// NOTE(randy): $Line -> Circle
					v2 line_vector = V2SubtractV2(a_shape.line.p2, a_shape.line.p1);
					
					v2 vector_from_circle = V2SubtractV2(v2(b_shape.circle.p.x,
															b_shape.circle.p.y),
														 a_shape.line.p1);
					f32 dot = vector_from_circle.x * line_vector.x + vector_from_circle.y * line_vector.y;
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y));
					
					v2 collision_normal = V2SubtractV2(v2(b_shape.circle.p.x,
														  b_shape.circle.p.y),
													   V2AddV2(a_shape.line.p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance, collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					if (collision_distance < b_shape.circle.r &&
						b_shape.circle.p.x >= a_shape.line.p1.x &&
						b_shape.circle.p.x <= a_shape.line.p2.x)
					{
						manifold->n = c2V(normalised_collision_normal.x,
										  normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = b_shape.circle.r - collision_distance;
					}
				} break;
				
				case C2_SHAPE_TYPE_aabb:
				{
					// NOTE(randy): $Line -> AABB
					// TODO(randy): SAT implementation
				} break;
				
				case C2_SHAPE_TYPE_line:
				{
					// NOTE(randy): $Line -> Line
				} break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_circle :
		{
			switch (b_shape_type)
			{
				case C2_SHAPE_TYPE_line :
				{
					// NOTE(randy): $Circle -> Line
					v2 line_vector = V2SubtractV2(b_shape.line.p2, b_shape.line.p1);
					
					v2 point_a_vector_from_line = V2SubtractV2(v2(a_shape.circle.p.x,
																  a_shape.circle.p.y),
															   b_shape.line.p1);
					f32 dot = point_a_vector_from_line.x * line_vector.x + point_a_vector_from_line.y * line_vector.y;
					// NOTE(randy): proj a->b = (a dot b / mag^2) * b
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y));
					
					v2 collision_normal = V2SubtractV2(v2(a_shape.circle.p.x,
														  a_shape.circle.p.y),
													   V2AddV2(b_shape.line.p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance,
														collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					//PushDebugLine(p1, V2AddV2(p1, collision_normal), v3(1.0f, 0.0f, 0.0f));
					
					if (collision_distance < a_shape.circle.r &&
						a_shape.circle.p.x >= b_shape.line.p1.x &&
						a_shape.circle.p.x <= b_shape.line.p2.x)
					{
						manifold->n = c2V(-normalised_collision_normal.x,
										  -normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = a_shape.circle.r - collision_distance;
					}
				} break;
				
				case C2_SHAPE_TYPE_capsule :
				{
					// NOTE(randy): $Circle -> Capsule
					c2CircletoCapsuleManifold(a_shape.circle, b_shape.capsule, manifold);
				} break;
				
				case C2_SHAPE_TYPE_circle :
				{
					// NOTE(randy): $Circle -> Circle
					c2CircletoCircleManifold(a_shape.circle, b_shape.circle, manifold);
				} break;
				
				case C2_SHAPE_TYPE_aabb :
				{
					// NOTE(randy): $Circle -> AABB
					c2CircletoAABBManifold(a_shape.circle, b_shape.aabb, manifold);
				} break;
				
				default:
				Assert(0);
				break;
			}
		} break;
		
		default:
		Assert(0);
		break;
	}
}

internal i32 GetOverlappingBodiesWithShape(Entity **overlapping_entities,
										   c2Shape shape,
										   c2ShapeType shape_type)
{
	i32 overlap_count = 0;
	
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_physical);)
	{
		c2Shape against_shape = entity->physics.shape;
		AddPositionOffsetToShape(&against_shape, entity->physics.shape_type, entity->position);
		
		c2Manifold manifold = {0};
		GenerateCollisionManifold(shape, shape_type,
								  against_shape, entity->physics.shape_type,
								  &manifold);
		
		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			Assert(overlap_count + 1 < MAX_OVERLAPPING_COLLIDERS);
			overlapping_entities[overlap_count++] = entity;
		}
	}
	
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, ENTITY_PROPERTY_queryable);)
	{
		c2Shape against_shape = entity->physics.shape;
		AddPositionOffsetToShape(&against_shape, entity->physics.shape_type, entity->position);
		
		c2Manifold manifold = {0};
		GenerateCollisionManifold(shape, shape_type,
								  against_shape, entity->physics.shape_type,
								  &manifold);
		
		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			Assert(overlap_count + 1 < MAX_OVERLAPPING_COLLIDERS);
			overlapping_entities[overlap_count++] = entity;
		}
	}
	
	return overlap_count;
}

internal b8 IsV2OverlappingShape(v2 pos, c2Shape shape, c2ShapeType shape_type)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb:
		{
			return (pos.x >= shape.aabb.min.x && pos.x < shape.aabb.max.x &&
					pos.y >= shape.aabb.min.y && pos.y < shape.aabb.max.y);
		} break;
		
		case C2_SHAPE_TYPE_line:
		{
			f32 thicc = 1.0f;
			
			return (pos.x >= shape.line.p1.x && pos.x <= shape.line.p2.x &&
					pos.y >= (shape.line.p1.y + shape.line.p2.y) / 2.0f - thicc && pos.y <= (shape.line.p1.y + shape.line.p2.y) / 2.0f + thicc);
		} break;
		
		default:
		return 0;
		break;
	}
	
	return 0;
}

// https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
/* int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
	if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
	   c = !c;
  }
  return c;
} */