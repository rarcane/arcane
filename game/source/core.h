#define MAX_DEBUG_LINES 1024

#define PERLIN_NOISE_LENGTH 256

typedef struct Entity Entity;
typedef struct CollisionInfo CollisionInfo;
typedef struct ItemComponent ItemComponent;
typedef struct StorageComponent StorageComponent;

typedef struct WorldData WorldData;
typedef struct RunData RunData;
typedef struct ClientData ClientData;
typedef struct EditorData EditorData;

typedef struct DebugLine
{
	b8 is_valid;
	v2 p1;
	v2 p2;
	v3 colour;
	b8 has_duration;
	f32 lifetime;
	f32 start_time;
} DebugLine;

typedef struct Core Core;
struct Core
{
	MemoryArena *permanent_arena;
	MemoryArena *frame_arena;
	
	char *res_path;
	
	v2 camera_position;
	v2 camera_offset;
	f32 camera_zoom;
	f32 camera_zoom_mult;
	
	// Temp entity stores
	//Entity *backpack;
	//Entity *sword;
	
	// Grabbed inventory item.
	//ItemComponent *grabbed_inv_item_comp;
	//i32 grabbed_inv_item_origin_slot;
	//StorageComponent *grabbed_inv_item_origin_storage_comp;
	//v2 grabbed_inventory_item_offset;
	// Hotbar & held item.
	//Entity *held_item;
	//Entity *hotbar;
	//i32 active_hotbar_slot;
	
	// Temp
	b32 is_ingame;
	
	v2 random_field[PERLIN_NOISE_LENGTH][PERLIN_NOISE_LENGTH];
	
	// Data stores
	RunData *run_data;
	ClientData *client_data;
	EditorData *editor_data;
	
	// NOTE(rjf): Render size data.
	union {
		struct
		{
			f32 render_w;
			f32 render_h;
		};
		struct
		{
			f32 render_width;
			f32 render_height;
		};
		v2 render_size;
	};
	
	// Input stuff.
	b8 is_mid_left_click;
	b8 left_mouse_released;
	b8 is_mid_right_click;
	b8 right_mouse_released;
	b8 capture_release_key;
	
	// Application time data.
	f32 raw_delta_t;
	f32 delta_mult;
	f32 delta_t;
	// World time data.
	f32 world_delta_mult;
	f32 world_delta_t;
	
	// Perfomance profiling.
	BlockTimer performance_timers[20];
	i32 performance_timer_count;
	// Debug line rendering.
	DebugLine debug_lines[MAX_DEBUG_LINES];
	i32 debug_line_count;
	i32 free_debug_line_index;
};
global Core *core = 0;