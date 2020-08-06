internal Entity *NewEntity()
{
	Entity *entity = &core->run_data->entities[core->run_data->entity_count++];
	
	entity->sprite_data.tint = v4u(1.0f);
	entity->sprite_data.scale = v2(1.0f, 1.0f);
	
	entity->animation_flags = ANIMATION_FLAGS_playing | ANIMATION_FLAGS_repeat;
	entity->frame_start_time = core->run_data->world.elapsed_world_time;
	entity->interval_mult = 1.0f;
	
	entity->move_speed_mult = 1.0f;
	
	entity->current_general_state = "Idle";
	
	return entity;
}

internal void DeleteEntity(Entity *entity)
{
	// TODO(randy):
}

internal Entity *GetCharacterEntity()
{
	return core->run_data->character_entity;
}

/*
enum EntityProperty
{
	EntityProperty_IsAllocated,
	EntityProperty_OnFire,
	EntityProperty_Sprite,
	EntityProperty_EmitsParticles,
	
	EntityProperty_Max
};

struct Entity
{
	u64 properties[EntityProperty_Max/64+1];
	f32 fire_strength;
	Sprite sprite;
	ParticleType particle_type;
};

internal b32
EntityHasProperty(Entity *entity, EntityProperty property)
{
	return !!(entity->properties[property/64] & ((u64)1 << (property%64)));
}

internal void
EntitySetProperty(Entity *entity, EntityProperty property)
{
	entity->properties[property/64] |= ((u64)1 << (property%64));
}

#define ENTITY_TABLE_SIZE 4096
struct GameState
{
	Entity entities[ENTITY_TABLE_SIZE];
};

internal b32
IncrementEntity(Entity **entity_ptr)
{
	b32 result = 0;
	Entity *entity = *entity_ptr;
	if(entity)
	{
		entity = the_first_entity_in_the_game_state;
	}
	else
	{
		Entity *next_entity = 0;
		for(u64 index = (entity - the_first_entity_in_the_game_state) + 1;
			index < ENTITY_TABLE_SIZE;
			index += 1)
		{
			Entity *candidate = the_first_entity_in_the_game_state + index;
			if(EntityHasProperty(candidate, EntityProperty_IsAllocated))
			{
				next_entity = candidate;
				break;
			}
		}
		entity = next_entity;
	}
	*entity_ptr = entity;
	result = !!entity;
	return result;
}
 */