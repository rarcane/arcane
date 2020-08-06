/*
internal void UpdateParticleEmitters()
{
	for (int i = 0; i < core->run_data->entity_components.particle_emitter_component_count; i++)
	{
		ParticleEmitterComponent *emitter_comp = &core->run_data->entity_components.particle_emitter_components[i];
		if (emitter_comp->component_id)
		{
			PositionComponent *position_comp = GetPositionComponentFromEntityID(emitter_comp->parent_entity_id);
			
			if (core->run_data->world.elapsed_world_time >= emitter_comp->start_time + emitter_comp->life_time)
			{
				if (emitter_comp->flags & PARTICLE_EMITTER_FLAGS_repeat)
				{
					emitter_comp->start_time = core->run_data->world.elapsed_world_time;
					emitter_comp->begin_callback(emitter_comp);
				}
				else
				{
					Assert(0); // TODO(randy): Delete emitter
				}
				
				if (emitter_comp->finish_callback)
					emitter_comp->finish_callback(emitter_comp);
			}
			
			// NOTE(randy): Update particles
			for (int j = 0; j < emitter_comp->particle_count; j++)
			{
				Particle *particle = &emitter_comp->particles[j];
				
				if (particle->is_valid)
				{
					if (core->run_data->world.elapsed_world_time >= particle->spawn_time + particle->life_time)
						DeleteParticle(emitter_comp, j);
					else
					{
						f32 life_alpha = 1.0f - (core->run_data->world.elapsed_world_time - particle->spawn_time) / particle->life_time;
						
						particle->position.x += particle->velocity.x * core->world_delta_t;
						particle->position.y += particle->velocity.y * core->world_delta_t;
						
						v2 particle_position = v2view(V2AddV2(position_comp->position, particle->position));
						
						Ts2dPushFilledRect(V4MultiplyF32(particle->colour, life_alpha),
										   v4(particle_position.x,
											  particle_position.y,
											  core->camera_zoom,
											  core->camera_zoom));
					}
				}
			}
		}
	}
}

internal void NewParticle(ParticleEmitterComponent *emitter, f32 life_time, v2 initial_pos, v2 velocity, v4 colour)
{
	Assert(emitter->particle_count < MAX_PARTICLE_AMOUNT - 1);
	
	i32 index;
	if (emitter->particle_count == emitter->free_particle_index)
	{
		index = emitter->particle_count;
		emitter->particle_count++;
		emitter->free_particle_index++;
	}
	else
	{
		index = emitter->free_particle_index;
		for (int i = 0; i < emitter->particle_count; i++)
		{
			if (!emitter->particles[i].is_valid)
			{
				emitter->free_particle_index = i;
				break;
			}
		}
	}
	
	emitter->particles[index].is_valid = 1;
	emitter->particles[index].life_time = life_time;
	emitter->particles[index].spawn_time = core->run_data->world.elapsed_world_time;
	emitter->particles[index].position = initial_pos;
	emitter->particles[index].velocity = velocity;
	emitter->particles[index].colour = colour;
}

internal void DeleteParticle(ParticleEmitterComponent *emitter, i32 particle_index)
{
	Particle empty_particle = {0};
	emitter->particles[particle_index] = empty_particle;
	
	if (particle_index < emitter->free_particle_index)
		emitter->free_particle_index = particle_index;
}

internal void GeneratePineParticles(ParticleEmitterComponent *emitter)
{
	for (int i = 0; i < 3; i++)
	{
		v4 colour = {27.0f / 255.0f, 77.0f / 255.0f, 80.0f / 255.0f, 1.0f};
		NewParticle(emitter,
					RandomF32(5.0, 10.0f),
					v2(RandomF32(-20.0f, 20.0f), -30.0f - RandomF32(0.0f, 130.0f)),
					v2(RandomF32(-7.0f, 7.0f), RandomF32(6.0f, 12.0f)),
					colour);
	}
}

internal void GenerateGroundParticles(ParticleEmitterComponent *emitter)
{
	 for (int i = 0; i < 3; i++)
    {
       v4 colour = {155.0f / 255.0f, 117.0f / 255.0f, 82.0f / 255.0f, 1.0f};
       NewParticle(emitter,
                RandomF32(3.0, 6.0f),
                v2(RandomF32(-50.0f, 50.0f), 0.0f),
                v2(RandomF32(-3.0f, 3.0f), RandomF32(-2.0f, -5.0f)),
                colour);
    }
}
 */