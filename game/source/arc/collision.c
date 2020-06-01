#ifdef DEVELOPER_TOOLS
internal void RenderColliders()
{
	if (!(core->run_data->debug_flags & DEBUG_FLAGS_draw_collision))
		return;
	
	for (int i = 0; i < core->run_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->run_data->entity_components.physics_body_components[i];
		if (body_comp->component_id)
		{
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(body_comp->parent_entity_id);
			Assert(pos_comp);
			
			v3 col = {1.0f, 1.0f, 1.0f};
			if (core->run_data->collision_editor.selected_ground_seg)
				if (body_comp->parent_entity_id == core->run_data->collision_editor.selected_ground_seg->entity_id)
				col = v3(1.0f, 0.0f, 0.0f);
			
			PushDebugShape(body_comp->shape, body_comp->shape_type, pos_comp->position, col);
		}
	}
	
	if (core->run_data->debug_flags & DEBUG_FLAGS_draw_chunk_grid)
		for (int i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
		{
			v3 colour = v3(1.0f, 1.0f, 1.0f);
			if (core->run_data->chunk_editor.is_chunk_selected && chunk->x_index == core->run_data->chunk_editor.selected_chunk.x_index && chunk->y_index == core->run_data->chunk_editor.selected_chunk.y_index)
				colour = v3(1.0f, 0.0f, 0.0f);
			
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
	for (int i = 0; i < core->run_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->run_data->entity_components.physics_body_components[i];
		if (body_comp->component_id)
		{
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(body_comp->parent_entity_id);
			Assert(pos_comp);
			
			// Apply gravity
			if (body_comp->gravity_multiplier != 0.0f && body_comp->mass_data.mass != 0.0f)
				body_comp->force.y += (WORLD_GRAVITY * body_comp->gravity_multiplier) / body_comp->mass_data.inv_mass;
			
			// Integrate next position
			v2 acceleration = V2MultiplyF32(body_comp->force, body_comp->mass_data.inv_mass);
			body_comp->velocity = V2AddV2(body_comp->velocity, V2MultiplyF32(acceleration, core->world_delta_t));
			pos_comp->position = V2AddV2(pos_comp->position, V2MultiplyF32(body_comp->velocity, core->world_delta_t));
			
			body_comp->force = v2(0.0f, 0.0f);
		}
	}
	
	CollisionPair collision_pairs[MAX_COLLISION_PAIRS];
	i32 pair_count = 0;
	GenerateCollisionPairs(collision_pairs, &pair_count);
	
	for (int i = 0; i < pair_count; i++)
	{
		PhysicsBodyComponent *a_body_comp = collision_pairs[i].a;
		PositionComponent *a_pos_comp = GetPositionComponentFromEntityID(a_body_comp->parent_entity_id);
		PhysicsBodyComponent *b_body_comp = collision_pairs[i].b;
		PositionComponent *b_pos_comp = GetPositionComponentFromEntityID(b_body_comp->parent_entity_id);
		
		c2Manifold manifold = {0};
		
		c2Shape a_shape = a_body_comp->shape;
		AddPositionOffsetToShape(&a_shape, a_body_comp->shape_type, a_pos_comp->position);
		
		c2Shape b_shape = b_body_comp->shape;
		AddPositionOffsetToShape(&b_shape, b_body_comp->shape_type, b_pos_comp->position);
		
		GenerateCollisionManifold(a_shape, a_body_comp->shape_type,
								  b_shape, b_body_comp->shape_type,
								  &manifold);
		//GenerateCollisionManifold(a_body_comp, a_pos_comp->position, b_body_comp, b_pos_comp->position, &manifold);
		
		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			Assert(!(a_body_comp->mass_data.mass == 0 && b_body_comp->mass_data.mass == 0)); // NOTE(randy): Two static bodies are colliding?
			
			v2 normal = v2(manifold.n.x, manifold.n.y);
			
			v2 relative_velocity = V2SubtractV2(b_body_comp->velocity, a_body_comp->velocity);
			f32 velocity_along_normal = relative_velocity.x * normal.x + relative_velocity.y * normal.y;
			
			if (velocity_along_normal <= 0)
			{
				// Impulse resolution
				f32 restitution = MinimumF32(a_body_comp->material.restitution, b_body_comp->material.restitution);
				
				f32 j = -(1 + restitution) * velocity_along_normal;
				j = j / (a_body_comp->mass_data.inv_mass + b_body_comp->mass_data.inv_mass);
				
				v2 impulse = V2MultiplyF32(normal, j);
				
				v2 impulse_a = V2MultiplyF32(impulse, a_body_comp->mass_data.inv_mass);
				a_body_comp->velocity = V2AddV2(a_body_comp->velocity, impulse_a);
				
				v2 impulse_b = V2MultiplyF32(impulse, b_body_comp->mass_data.inv_mass);
				b_body_comp->velocity = V2AddV2(b_body_comp->velocity, impulse_b);
				
				// Friction
				v2 new_rv = V2SubtractV2(b_body_comp->velocity, a_body_comp->velocity);
				v2 tangent = V2SubtractV2(new_rv, V2MultiplyF32(normal, new_rv.x * normal.x + new_rv.y * normal.y));
				v2Normalise(&tangent);
				
				f32 jt = -(new_rv.x * tangent.x + new_rv.y * tangent.y);
				jt = jt / (a_body_comp->mass_data.inv_mass + b_body_comp->mass_data.inv_mass);
				
				f32 mu = MinimumF32(a_body_comp->material.static_friction, b_body_comp->material.static_friction); //PythagSolve(a_body_comp->material.static_friction, b_body_comp->material.static_friction);
				
				v2 friction_impulse;
				if (fabsf(jt) < j * mu)
					friction_impulse = V2MultiplyF32(tangent, jt);
				else
				{
					f32 dynamic_friction = MinimumF32(a_body_comp->material.dynamic_friction, b_body_comp->material.dynamic_friction); //PythagSolve(a_body_comp->material.dynamic_friction, b_body_comp->material.dynamic_friction);
					friction_impulse = V2MultiplyF32(tangent, -j * dynamic_friction);
				}
				
				v2 friction_impulse_a = V2MultiplyF32(friction_impulse, a_body_comp->mass_data.inv_mass);
				a_body_comp->velocity = V2SubtractV2(a_body_comp->velocity, friction_impulse_a);
				
				v2 friction_impulse_b = V2MultiplyF32(friction_impulse, b_body_comp->mass_data.inv_mass);
				b_body_comp->velocity = V2AddV2(b_body_comp->velocity, friction_impulse_b);
				
				// Positional correction
				const f32 percent = 0.2f;
				const f32 slop = 0.01f;
				v2 correction = V2MultiplyF32(normal, (MaximumF32(manifold.depths[0] - slop, 0.0f) / (a_body_comp->mass_data.inv_mass + b_body_comp->mass_data.inv_mass)) * percent);
				a_pos_comp->position = V2SubtractV2(a_pos_comp->position, V2MultiplyF32(correction, a_body_comp->mass_data.inv_mass));
				b_pos_comp->position = V2AddV2(b_pos_comp->position, V2MultiplyF32(correction, b_body_comp->mass_data.inv_mass));
			}
		}
	}
}

internal void GenerateCollisionPairs(CollisionPair pairs[], i32 *count)
{
	for (int i = 0; i < core->run_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp_a = &core->run_data->entity_components.physics_body_components[i];
		if (body_comp_a->component_id)
		{
			for (int j = 0; j < core->run_data->entity_components.physics_body_component_count; j++)
			{
				PhysicsBodyComponent *body_comp_b = &core->run_data->entity_components.physics_body_components[j];
				if (body_comp_b->component_id)
				{
					if (body_comp_a != body_comp_b)
					{
						if (!(body_comp_a->mass_data.mass == 0.0f && body_comp_b->mass_data.mass == 0.0f))
						{
							CollisionPair new_pair = {body_comp_a, body_comp_b};
							Assert(*count + 1 < MAX_COLLISION_PAIRS)
								pairs[(*count)++] = new_pair;
						}
					}
				}
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
					//c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
					//c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					c2AABBtoAABBManifold(a_shape.aabb, b_shape.aabb, manifold);
				} break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					//c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
					
					//c2Capsule b_capsule = b_body_comp->shape.capsule;
					//CapsuleToWorldSpace(&b_capsule, b_body_world_pos);
					
					c2AABBtoCapsuleManifold(a_shape.aabb, b_shape.capsule, manifold);
				} break;
				
				case C2_SHAPE_TYPE_poly:
				{
					/*
										c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
										
										c2Poly b_poly = b_body_comp->shape.poly;
										c2x world_pos = c2xIdentity();
										world_pos.p.x = b_body_world_pos.x;
										world_pos.p.y = b_body_world_pos.y;
					 */
					
					c2AABBtoPolyManifold(a_shape.aabb, &b_shape.poly, 0, manifold);
				} break;
				
				case C2_SHAPE_TYPE_line:
				{
					//Assert(0);
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
					/*
										c2Capsule a_capsule = a_body_comp->shape.capsule;
										CapsuleToWorldSpace(&a_capsule, a_body_world_pos);
										
										c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					 */
					
					c2AABBtoCapsuleManifold(b_shape.aabb, a_shape.capsule, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					/*
										c2Capsule a_capsule = a_body_comp->shape.capsule;
										CapsuleToWorldSpace(&a_capsule, a_body_world_pos);
										
										c2Poly b_poly = b_body_comp->shape.poly;
										c2x world_pos = c2xIdentity();
										world_pos.p.x = b_body_world_pos.x;
										world_pos.p.y = b_body_world_pos.y;
					 */
					
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
					/*
										// NOTE(randy): $Capsule -> Circle
										// Temp, just treating it like another circle for now
										v2 collision_normal = v2(a_shape.capsule.a.x - b_shape.circle.p.x,
																 a_shape.capsule.a.y - b_shape.circle.p.y);
										f32 mag = PythagSolve(collision_normal.x, collision_normal.y);
										
										v2 normalised_normal = v2(collision_normal.x / mag,
																  collision_normal.y / mag);
										v2Realise(&normalised_normal);
										
										if (mag < a_shape.capsule.r + b_shape.circle.r)
										{
											manifold->n = c2V(normalised_normal.x,
															  normalised_normal.y);
											manifold->count = 1;
											manifold->depths[0] = a_shape.capsule.r + b_shape.circle.r - mag;
										}
					 */
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
					/*
										c2Poly a_poly = a_body_comp->shape.poly;
										c2x world_pos = c2xIdentity();
										world_pos.p.x = a_body_world_pos.x;
										world_pos.p.y = a_body_world_pos.y;
										
										c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					 */
					
					c2AABBtoPolyManifold(b_shape.aabb, &a_shape.poly, 0, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					/*
										c2Poly a_poly = a_body_comp->shape.poly;
										c2x world_pos = c2xIdentity();
										world_pos.p.x = a_body_world_pos.x;
										world_pos.p.y = a_body_world_pos.y;
										
										c2Capsule b_capsule = b_body_comp->shape.capsule;
										CapsuleToWorldSpace(&b_capsule, b_body_world_pos);
					 */
					
					c2CapsuletoPolyManifold(b_shape.capsule, &a_shape.poly, 0, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					/*
										c2Poly a_poly = a_body_comp->shape.poly;
										c2x world_pos_a = c2xIdentity();
										world_pos_a.p.x = a_body_world_pos.x;
										world_pos_a.p.y = a_body_world_pos.y;
										
										c2Poly b_poly = b_body_comp->shape.poly;
										c2x world_pos_b = c2xIdentity();
										world_pos_b.p.x = b_body_world_pos.x;
										world_pos_b.p.y = b_body_world_pos.y;
					 */
					
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
						manifold->n = c2V(normalised_collision_normal.x, normalised_collision_normal.y);
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
					// TODO(randy): Not working.
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
						manifold->n = c2V(-normalised_collision_normal.x, -normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = a_shape.circle.r - collision_distance;
					}
				} break;
				
				case C2_SHAPE_TYPE_capsule :
				{
					/*
										// NOTE(randy): $Circle -> Capsule
										// Temp, just treating it like another circle for now
										v2 collision_normal = v2(a_shape.circle.p.x - b_shape.capsule.a.x,
																 a_shape.circle.p.y - b_shape.capsule.a.y);
										f32 mag = PythagSolve(collision_normal.x, collision_normal.y);
										
										v2 normalised_normal = v2(collision_normal.x / mag,
																  collision_normal.y / mag);
										v2Realise(&normalised_normal);
										
										if (mag < a_shape.circle.r + b_shape.capsule.r)
										{
											manifold->n = c2V(normalised_normal.x,
															  normalised_normal.y);
											manifold->count = 1;
											manifold->depths[0] = a_shape.circle.r + b_shape.capsule.r - mag;
										}
					 */
				} break;
				
				case C2_SHAPE_TYPE_circle :
				{
					// NOTE(randy): $Circle -> Circle
					v2 collision_normal = v2(a_shape.circle.p.x - b_shape.circle.p.x,
											 a_shape.circle.p.y - b_shape.circle.p.y);
					f32 mag = PythagSolve(collision_normal.x, collision_normal.y);
					
					v2 normalised_normal = v2(collision_normal.x / mag,
											  collision_normal.y / mag);
					v2Realise(&normalised_normal);
					
					if (mag < a_shape.circle.r + b_shape.circle.r)
					{
						manifold->n = c2V(normalised_normal.x,
										  normalised_normal.y);
						manifold->count = 1;
						manifold->depths[0] = a_shape.circle.r + b_shape.circle.r - mag;
					}
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

internal i32 GetOverlappingBodiesWithShape(PhysicsBodyComponent **overlapping_bodies,
										   c2Shape shape,
										   c2ShapeType shape_type)
{
	i32 overlap_count = 0;
	
	for (i32 i = 0; i < core->run_data->entity_count; i++)
	{
		Entity *entity = &core->run_data->entities[i - 1];
		if (entity->component_ids[COMPONENT_physics_body])
		{
			PhysicsBodyComponent *phys_body = GetPhysicsBodyComponentFromEntityID(entity->entity_id);
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(entity->entity_id);
			Assert(phys_body && pos_comp);
			
			c2Shape against_shape = phys_body->shape;
			AddPositionOffsetToShape(&against_shape, phys_body->shape_type, pos_comp->position);
			
			c2Manifold manifold = {0};
			GenerateCollisionManifold(shape, shape_type,
									  against_shape, phys_body->shape_type,
									  &manifold);
			
			if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
			{
				Assert(overlap_count + 1 < MAX_OVERLAPPING_COLLIDERS);
				overlapping_bodies[overlap_count++] = phys_body;
			}
		}
	}
	
	return overlap_count;
}

internal b32 IsMouseOverlappingShape(v2 mouse_pos, c2Shape shape, c2ShapeType shape_type)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb:
		{
			if (mouse_pos.x >= shape.aabb.min.x && mouse_pos.x < shape.aabb.max.x &&
				mouse_pos.y >= shape.aabb.min.y && mouse_pos.y < shape.aabb.max.y)
				return 1;
			else
				return 0;
			
			break;
		}
		default:
		Assert(0);
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

internal void AddPositionOffsetToShape(c2Shape *shape, c2ShapeType shape_type, v2 position)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb :
		{
			shape->aabb.min.x += position.x;
			shape->aabb.min.y += position.y;
			shape->aabb.max.x += position.x;
			shape->aabb.max.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_capsule :
		{
			shape->capsule.a.x += position.x;
			shape->capsule.a.y += position.y;
			shape->capsule.b.x += position.x;
			shape->capsule.b.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_poly :
		{
			// TODO(randy): Test this out to ensure it's working.
			for (int i = 0; i < shape->poly.count; i++)
			{
				shape->poly.verts[i].x += position.x;
				shape->poly.verts[i].y += position.y;
			}
			c2MakePoly(&shape->poly);
		} break;
		
		case C2_SHAPE_TYPE_line :
		{
			shape->line.p1 = V2AddV2(shape->line.p1, position);
			shape->line.p2 = V2AddV2(shape->line.p2, position);
		} break;
	}
}