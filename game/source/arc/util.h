#define BLOCK_TIMER(timer_name, ...) { f32 timer_start = platform->GetTime(); {__VA_ARGS__} LogWarning(timer_name " timer took %fms", (platform->GetTime() - timer_start) * 1000.0f); }
#define BLOCK_TIMER_IF(timer_name, conditional, ...) { f32 timer_start = platform->GetTime(); {__VA_ARGS__} f32 timer_length = (platform->GetTime() - timer_start) * 1000.0f;  if (conditional) {LogWarning(timer_name " timer took %fms", timer_length);} }

#define START_TIMER f32 timer_start = platform->GetTime()
#define END_TIMER f32 timer_length = platform->GetTime() - timer_start

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

typedef struct SerialisationPointer
{
	void **pointer_address;
	i32 offset;
} SerialisationPointer;

typedef uint32 EntityFlags;
typedef enum GeneralisedEntityType GeneralisedEntityType;
typedef struct EntitySave
{
	char name[20]; // Do we really need this?
	EntityFlags flags;
	GeneralisedEntityType type;
} EntitySave;

typedef struct ComponentSaveHelper
{
	i32 entity_offset;
	void *comp_data;
} ComponentSaveHelper;

typedef struct SkeletonChunk
{
	i32 x_index;
	i32 y_index;
} SkeletonChunk;

#define MAX_SERIALISATION_POINTERS 10240
global i32 serialisation_pointer_count = 0;
global SerialisationPointer serialisation_pointers[MAX_SERIALISATION_POINTERS];

internal f32 Fade(f32 alpha);
internal f32 PythagSolve(f32 a, f32 b);

internal f32 GetPerlinNoise(f32 x_pos, f32 y_pos);

internal v2 GetMousePositionInWorldSpace();