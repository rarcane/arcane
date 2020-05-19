#ifdef DEVELOPER_TOOLS
internal void RenderColliders()
{
	if (!(core->run_data->editor_flags & EDITOR_FLAGS_draw_collision))
		return;
	
	for (int i = 0; i < core->run_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->run_data->entity_components.physics_body_components[i];
		if (body_comp->component_id)
		{
			PositionComponent *pos_comp = GetPositionComponentFromEntityID(body_comp->parent_entity_id);
			Assert(pos_comp);
			
			v3 col = {1.0f, 1.0f, 1.0f};
			if (core->run_data->selected_ground_seg)
				if (body_comp->parent_entity_id == core->run_data->selected_ground_seg->entity_id)
				col = v3(1.0f, 0.0f, 0.0f);
			
			PushDebugShape(body_comp->shape, body_comp->shape_type, pos_comp->position, col);
		}
	}
	
	if (core->run_data->editor_flags & EDITOR_FLAGS_draw_chunk_grid)
		for (int i = 0; i < core->run_data->active_chunk_count; i++)
	{
		Chunk *chunk = &core->run_data->active_chunks[i];
		if (chunk->is_valid)
		{
			v3 colour = v3(1.0f, 1.0f, 1.0f);
			if (chunk == core->run_data->selected_chunk)
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
		GenerateCollisionManifold(a_body_comp, a_pos_comp->position, b_body_comp, b_pos_comp->position, &manifold);
		
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

internal void GenerateCollisionManifold(PhysicsBodyComponent *a_body_comp, v2 a_body_world_pos, PhysicsBodyComponent *b_body_comp, v2 b_body_world_pos, c2Manifold *manifold)
{
	switch (a_body_comp->shape_type)
	{
		case C2_SHAPE_TYPE_aabb:
		{
			switch (b_body_comp->shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
					c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					c2AABBtoAABBManifold(a_aabb, b_aabb, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
					
					c2Capsule b_capsule = b_body_comp->shape.capsule;
					CapsuleToWorldSpace(&b_capsule, b_body_world_pos);
					
					c2AABBtoCapsuleManifold(a_aabb, b_capsule, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					c2AABB a_aabb = v2AddAABB(a_body_world_pos, a_body_comp->shape.aabb);
					
					c2Poly b_poly = b_body_comp->shape.poly;
					c2x world_pos = c2xIdentity();
					world_pos.p.x = b_body_world_pos.x;
					world_pos.p.y = b_body_world_pos.y;
					
					c2AABBtoPolyManifold(a_aabb, &b_poly, &world_pos, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_line:
				{
					Assert(0);
				} break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_capsule:
		{
			switch (b_body_comp->shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					c2Capsule a_capsule = a_body_comp->shape.capsule;
					CapsuleToWorldSpace(&a_capsule, a_body_world_pos);
					
					c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					
					c2AABBtoCapsuleManifold(b_aabb, a_capsule, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					c2Capsule a_capsule = a_body_comp->shape.capsule;
					CapsuleToWorldSpace(&a_capsule, a_body_world_pos);
					
					c2Poly b_poly = b_body_comp->shape.poly;
					c2x world_pos = c2xIdentity();
					world_pos.p.x = b_body_world_pos.x;
					world_pos.p.y = b_body_world_pos.y;
					
					c2CapsuletoPolyManifold(a_capsule, &b_poly, &world_pos, manifold);
				}
				break;
				
				case C2_SHAPE_TYPE_line:
				{
					/* f32 m = b_line.d.y / b_line.d.x;
					f32 c = -m * p1.x + p1.y;
					f32 perp_segment = -m * capsule_point_a.x + capsule_point_a.y - c;
					f32 dist_from_segment = fabsf(perp_segment) / SquareRoot(-m * -m + 1); */
					
					/* f32 side = (p2.x - p1.x) * (capsule_point_a.y - p1.y) - (p2.y - p1.y) * (capsule_point_a.x - p1.x);
					if (side > 0.0f)
					{
						manifold->n = c2V(-b_line.d.y, b_line.d.x);
						manifold->count = 1;
						manifold->depths[0] = dist_from_segment;
					} */
					
					/* v2 dist_vector = V2SubtractV2(capsule_point_a, p1);
						f32 dist = PythagSolve(dist_vector.x, dist_vector.y);
						if (dist < a_capsule.r)
						{
							manifold->n = c2V(-dist_vector.x / dist, -dist_vector.y / dist);
							manifold->count = 1;
							manifold->depths[0] = a_capsule.r - dist;
						} */
					
					c2Capsule a_capsule = a_body_comp->shape.capsule;
					CapsuleToWorldSpace(&a_capsule, a_body_world_pos);
					
					Line b_line = b_body_comp->shape.line;
					
					v2 p1 = V2AddV2(b_line.p1, b_body_world_pos);
					v2 p2 = V2AddV2(b_line.p2, b_body_world_pos);
					v2 line_vector = V2SubtractV2(p2, p1);
					v2 capsule_point_a = v2(a_capsule.a.x, a_capsule.a.y);
					
					v2 point_a_vector_from_line = V2SubtractV2(capsule_point_a, p1);
					f32 dot = point_a_vector_from_line.x * line_vector.x + point_a_vector_from_line.y * line_vector.y;
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y)); // proj a->b = (a dot b / mag^2) * b
					
					v2 collision_normal = V2SubtractV2(capsule_point_a, V2AddV2(p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance, collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					//PushDebugLine(p1, V2AddV2(p1, collision_normal), v3(1.0f, 0.0f, 0.0f));
					
					if (collision_distance < a_capsule.r && capsule_point_a.x >= p1.x && capsule_point_a.x <= p2.x)
					{
						manifold->n = c2V(-normalised_collision_normal.x, -normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = a_capsule.r - collision_distance;
					}
				}
				break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		case C2_SHAPE_TYPE_poly:
		{
			switch (b_body_comp->shape_type)
			{
				case C2_SHAPE_TYPE_aabb:
				{
					c2Poly a_poly = a_body_comp->shape.poly;
					c2x world_pos = c2xIdentity();
					world_pos.p.x = a_body_world_pos.x;
					world_pos.p.y = a_body_world_pos.y;
					
					c2AABB b_aabb = v2AddAABB(b_body_world_pos, b_body_comp->shape.aabb);
					
					c2AABBtoPolyManifold(b_aabb, &a_poly, &world_pos, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_capsule:
				{
					c2Poly a_poly = a_body_comp->shape.poly;
					c2x world_pos = c2xIdentity();
					world_pos.p.x = a_body_world_pos.x;
					world_pos.p.y = a_body_world_pos.y;
					
					c2Capsule b_capsule = b_body_comp->shape.capsule;
					CapsuleToWorldSpace(&b_capsule, b_body_world_pos);
					
					c2CapsuletoPolyManifold(b_capsule, &a_poly, &world_pos, manifold);
					manifold->n.x *= -1.0f;
					manifold->n.y *= -1.0f;
				}
				break;
				
				case C2_SHAPE_TYPE_poly:
				{
					c2Poly a_poly = a_body_comp->shape.poly;
					c2x world_pos_a = c2xIdentity();
					world_pos_a.p.x = a_body_world_pos.x;
					world_pos_a.p.y = a_body_world_pos.y;
					
					c2Poly b_poly = b_body_comp->shape.poly;
					c2x world_pos_b = c2xIdentity();
					world_pos_b.p.x = b_body_world_pos.x;
					world_pos_b.p.y = b_body_world_pos.y;
					
					c2PolytoPolyManifold(&a_poly, &world_pos_a, &b_poly, &world_pos_b, manifold);
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
			switch (b_body_comp->shape_type)
			{
				case C2_SHAPE_TYPE_capsule:
				{
					Line a_line = a_body_comp->shape.line;
					
					c2Capsule b_capsule = b_body_comp->shape.capsule;
					CapsuleToWorldSpace(&b_capsule, b_body_world_pos);
					
					v2 p1 = V2AddV2(a_line.p1, a_body_world_pos);
					v2 p2 = V2AddV2(a_line.p2, a_body_world_pos);
					v2 line_vector = V2SubtractV2(p2, p1);
					v2 capsule_point_a = v2(b_capsule.a.x, b_capsule.a.y);
					
					v2 point_a_vector_from_line = V2SubtractV2(capsule_point_a, p1);
					f32 dot = point_a_vector_from_line.x * line_vector.x + point_a_vector_from_line.y * line_vector.y;
					v2 proj = V2MultiplyF32(line_vector, dot / (line_vector.x * line_vector.x + line_vector.y * line_vector.y));
					
					v2 collision_normal = V2SubtractV2(capsule_point_a, V2AddV2(p1, proj));
					f32 collision_distance = PythagSolve(collision_normal.x, collision_normal.y);
					v2 normalised_collision_normal = v2(collision_normal.x / collision_distance, collision_normal.y / collision_distance);
					v2Realise(&normalised_collision_normal);
					
					if (collision_distance < b_capsule.r && capsule_point_a.x >= p1.x && capsule_point_a.x <= p2.x)
					{
						manifold->n = c2V(normalised_collision_normal.x, normalised_collision_normal.y);
						manifold->count = 1;
						manifold->depths[0] = b_capsule.r - collision_distance;
					}
				}
				break;
				
				case C2_SHAPE_TYPE_line:
				{
				}
				break;
				
				default:
				Assert(0);
				break;
			}
		}
		break;
		
		default:
		Assert(0);
		break;
	}
}

b32 IsMouseOverlappingShape(v2 mouse_pos, c2Shape shape, c2ShapeType shape_type)
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