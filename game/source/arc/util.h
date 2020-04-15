// TODO: Properly organise for Ts integration with asserts & logging
#define R_TODO __debugbreak()

// NOTE(tjr): Breaks excecution with a error message
#define R_BREAK(...)           \
	{                          \
		LogError(__VA_ARGS__); \
		__debugbreak();        \
	}

// NOTE(tjr): Shipped with release build. Should be used sparingly for critical system failure.
#define R_HARD_ASSERT(x, ...)     \
	{                             \
		if (!(x))                 \
		{                         \
			R_BREAK(__VA_ARGS__); \
		}                         \
	}

// NOTE(tjr): Can be disabled for release builds. Just used to ensure no fuckery is going on while developing new systems; with enough usage this can safely be ruled out.
#define R_DEV_ASSERT(x, ...)      \
	{                             \
		if (!(x))                 \
		{                         \
			R_BREAK(__VA_ARGS__); \
		}                         \
	}

#define R_DEBUG_BREAK_KEY(x)                 \
	{                                        \
		if (platform->key_down[KEY_f2] && x) \
			__debugbreak();                  \
	}

#define START_PERF_TIMER(name) BlockTimer block_timer = {name, platform->GetTime(), 0}
#define END_PERF_TIMER                             \
	block_timer.finish_time = platform->GetTime(); \
	core->performance_timers[core->performance_timer_count++] = block_timer

typedef struct BlockTimer
{
	char name[50];
	f32 start_time;
	f32 finish_time;
} BlockTimer;

internal f32 Fade(f32 alpha);
internal f32 PythagSolve(f32 a, f32 b);

internal f32 GetPerlinNoise(f32 x_pos, f32 y_pos);

typedef struct SerialisationPointer
{
	void **pointer_address;
	i32 offset;
} SerialisationPointer;

#define MAX_SERIALISATION_POINTERS 10240
global i32 serialisation_pointer_count = 0;
global SerialisationPointer serialisation_pointers[MAX_SERIALISATION_POINTERS];