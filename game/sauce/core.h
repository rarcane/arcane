#define MAX_DEBUG_LINES 100

typedef struct Entity Entity;
typedef struct EntitySet EntitySet;
typedef struct ComponentSet ComponentSet;
typedef struct CollisionInfo CollisionInfo;
typedef struct ItemComponent ItemComponent;
typedef struct StorageComponent StorageComponent;

typedef struct EntityReference
{
	Entity *entity;
	i32 reference_index;
} EntityReference;

typedef struct DebugLine
{
	b8 is_valid;
	v2 p1;
	v2 p2;
	v3 colour;
	f32 lifetime;
	f32 start_time;
} DebugLine;

typedef struct Core Core;
struct Core
{
	MemoryArena *permanent_arena;
	MemoryArena *frame_arena;
	Ts2d *renderer;
	TsAssets *assets;
	TsDevTerminal *dev_terminal;
	TsUI *ui;

	v2 camera_position;
	float camera_zoom;
	v2 camera_offset;

	EntitySet *entity_set;
	ComponentSet *component_set;

	// NOTE(tjr): Temp entity stores
	Entity *player;
	Entity *backpack;
	Entity *sword;

	// NOTE(tjr): Grabbed inventory item.
	ItemComponent *grabbed_inv_item_comp;
	i32 grabbed_inv_item_origin_slot;
	StorageComponent *grabbed_inv_item_origin_storage_comp;
	v2 grabbed_inventory_item_offset;
	// NOTE(tjr): Hotbar & held item.
	Entity *held_item;
	Entity *hotbar;
	i32 active_hotbar_slot;

	// NOTE(tjr): Developer debug
	b32 draw_colliders;
	b32 draw_velocity;
	b32 bloom;
	b32 developer_view;
	f32 shadow_opacity;
	f32 slow_mult;

	// Temp
	b32 is_ingame;

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

	// NOTE(tjr): Input stuff.
	b8 is_mid_left_click;
	b8 left_mouse_released;
	b8 is_mid_right_click;
	b8 right_mouse_released;
	b8 capture_release_key;

	// NOTE(tjr): Application time data.
	f32 raw_delta_t;
	f32 delta_mult;
	f32 delta_t;
	// NOTE(tjr): World time data. TODO: Extract to world data.
	f32 elapsed_world_time;
	f32 world_delta_mult;
	f32 world_delta_t;

	// NOTE(tjr): Perfomance profiling.
	BlockTimer performance_timers[20];
	i32 performance_timer_count;
	// NOTE(tjr): Debug line rendering.
	DebugLine debug_lines[MAX_DEBUG_LINES];
	i32 debug_line_count;
	i32 free_debug_line_index;
};
global Core *core = 0;

internal f32 GetCurrentWorldTime()
{
	return core->elapsed_world_time;
}