internal void RenderColliders()
{
	for (int i = 0; i < core->world_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->world_data->entity_components.physics_body_components[i];
		Entity *entity = body_comp->parent_entity;
		if (entity)
		{
			PositionComponent *pos_comp = entity->components[COMPONENT_position];
			R_DEV_ASSERT(pos_comp, "Physics entity doesn't have a position component.");

			PushDebugShape(body_comp->shape, body_comp->shape_type, pos_comp->position, v3(1.0f, 1.0f, 1.0f));
		}
	}
}

internal void UpdatePhysics()
{
	for (int i = 0; i < core->world_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp = &core->world_data->entity_components.physics_body_components[i];
		Entity *entity = body_comp->parent_entity;
		if (entity)
		{
			PositionComponent *pos_comp = entity->components[COMPONENT_position];
			R_DEV_ASSERT(pos_comp, "Physics entity doesn't have a position component.");

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
		PositionComponent *a_pos_comp = a_body_comp->parent_entity->components[COMPONENT_position];
		PhysicsBodyComponent *b_body_comp = collision_pairs[i].b;
		PositionComponent *b_pos_comp = b_body_comp->parent_entity->components[COMPONENT_position];

		c2Manifold manifold = {0};
		GenerateCollisionManifold(a_body_comp, a_pos_comp->position, b_body_comp, b_pos_comp->position, &manifold);

		if (manifold.count > 0 && fabsf(manifold.depths[0]) != 0.0f)
		{
			R_DEV_ASSERT(!(a_body_comp->mass_data.mass == 0 && b_body_comp->mass_data.mass == 0), "Why are two static bodies colliding?");

			v2 normal = v2(manifold.n.x, manifold.n.y);

			v2 relative_velocity = V2SubtractV2(b_body_comp->velocity, a_body_comp->velocity);
			f32 velocity_along_normal = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

			if (velocity_along_normal <= 0)
			{
				f32 restitution = MinimumF32(a_body_comp->material.restitution, b_body_comp->material.restitution);

				f32 impulse_scalar = -(1 + restitution) * velocity_along_normal;
				impulse_scalar = impulse_scalar / (a_body_comp->mass_data.inv_mass + b_body_comp->mass_data.inv_mass);

				v2 impulse = V2MultiplyF32(normal, impulse_scalar);

				v2 impulse_a = V2MultiplyF32(impulse, a_body_comp->mass_data.inv_mass);
				a_body_comp->velocity = V2AddV2(a_body_comp->velocity, impulse_a);

				v2 impulse_b = V2MultiplyF32(impulse, b_body_comp->mass_data.inv_mass);
				b_body_comp->velocity = V2AddV2(b_body_comp->velocity, impulse_b);
			}

			const f32 percent = 0.2f;
			const f32 slop = 0.01f;
			v2 correction = V2MultiplyF32(normal, (MaximumF32(manifold.depths[0] - slop, 0.0f) / (a_body_comp->mass_data.inv_mass + b_body_comp->mass_data.inv_mass)) * percent);
			a_pos_comp->position = V2SubtractV2(a_pos_comp->position, V2MultiplyF32(correction, a_body_comp->mass_data.inv_mass));
			b_pos_comp->position = V2AddV2(b_pos_comp->position, V2MultiplyF32(correction, b_body_comp->mass_data.inv_mass));
		}
	}
}

internal void GenerateCollisionPairs(CollisionPair pairs[], i32 *count)
{
	for (int i = 0; i < core->world_data->entity_components.physics_body_component_count; i++)
	{
		PhysicsBodyComponent *body_comp_a = &core->world_data->entity_components.physics_body_components[i];
		Entity *entity_a = body_comp_a->parent_entity;
		if (entity_a)
		{
			for (int j = 0; j < core->world_data->entity_components.physics_body_component_count; j++)
			{
				PhysicsBodyComponent *body_comp_b = &core->world_data->entity_components.physics_body_components[j];
				Entity *entity_b = body_comp_b->parent_entity;
				if (entity_b)
				{
					if (entity_a != entity_b)
					{
						CollisionPair new_pair = {body_comp_a, body_comp_b};
						R_DEV_ASSERT(*count + 1 < MAX_COLLISION_PAIRS, "Too many collisions.")
						pairs[(*count)++] = new_pair;
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
			R_TODO;
		}
		break;

		default:
			R_TODO;
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
			R_TODO;
		}
		break;

		default:
			R_TODO;
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
			R_TODO;
		}
		break;

		default:
			R_TODO;
			break;
		}
	}
	break;

	case C2_SHAPE_TYPE_line:
	{
		switch (b_body_comp->shape_type)
		{
		case C2_SHAPE_TYPE_aabb:
		{
			R_TODO;
		}
		break;
		}
	}
	break;

	default:
		R_TODO;
		break;
	}
}