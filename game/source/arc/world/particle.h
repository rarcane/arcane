typedef struct Particle
{
	b8 is_valid;
	f32 life_time;
	f32 spawn_time;
	v2 position;
	v2 velocity;
	v4 colour;
} Particle;

typedef struct ParticleEmitterComponent ParticleEmitterComponent;
typedef void (*EmitterBeginCallback)(ParticleEmitterComponent *);
typedef void (*EmitterFinishCallback)(ParticleEmitterComponent *);

internal void
DeleteParticle(ParticleEmitterComponent *emitter, i32 particle_index);