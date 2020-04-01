// NOTE(rjf): Third-Party Code
#define CUTE_C2_IMPLEMENTATION
#include "ext/cute_c2.h"

#define ARCANE_UI_STYLE_MENU (1 << 0)
#define ARCANE_UI_STYLE_GAME (1 << 1)
#define TSUI_STYLE_CALLS_FILE "tsarcane/arcane_tsui_style_calls.inc"

// NOTE(rjf): Core Header Code
#include "telescope/telescope.h"
#include "arc/util.h"
#include "tsarcane/arcane_tsui_callbacks.h"
#include "tsarcane/assets.h"
#include "arcane.h"
#include "core.h"
// NOTE(tjr): Game Header Code
#include "arc/sprite/sprite.h"
#include "arc/world/particle.h"
#include "generated/catchall.h"
#include "arc/ecs/ecs.h"
#include "arc/world/cell.h"
#include "arc/world/world.h"
#include "arc/world/collision.h"
#include "arc/entity/player/player.h"
#include "arc/entity/player/camera.h"
#include "arc/ui/ui.h"

// NOTE(rjf): Core Implementation Code
#include "telescope/telescope.c"
#include "tsarcane/assets.c"
#include "tsarcane/arcane_tsui_callbacks.c"
// NOTE(tjr): Game Implementation Code
#include "arc/util.c"
#include "tsarcane/terminalcommands.c"
#include "arc/sprite/sprite.c"
#include "arc/entity/arc_entity.c"
#include "arc/world/particle.c"
#include "generated/catchall.c"
#include "arc/ecs/ecs.c"
#include "arc/item/item.c"
#include "arc/world/cell.c"
#include "arc/world/world.c"
#include "arc/world/collision.c"
#include "arc/entity/player/player.c"
#include "arc/entity/player/camera.c"
#include "arc/ui/ui.c"

internal void
GameInit(void)
{
	core = MemoryArenaAllocateAndZero(&platform->permanent_arena, sizeof(*core));
	HardAssert(core != 0);

	// NOTE(rjf): Initialize memory arenas.
	{
		core->permanent_arena = &platform->permanent_arena;
		core->frame_arena = &platform->scratch_arena;
	}

	// NOTE(rjf): Initialize core systems.
	{
		core->client_data = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(ClientData));
		R_DEV_ASSERT(core->client_data, "Failed to allocate memory for ClientData.");
		core->client_data->editor_flags |= EDITOR_FLAGS_draw_world;

		core->world_data = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(WorldData));
		R_DEV_ASSERT(core->world_data, "Failed to allocate memory for WorldData.");
		core->world_data->floating_chunk.is_valid = 1;

		// NOTE(rjf): Initialize TsDevTerminal.
		{
			TsDevTerminalCommand commands[] =
				{
					0 // {"col_draw", "", "Toggles whether the collision bounds are drawn.", 1, ToggleCollisionCommand},
				};

			TsDevTerminalVariable variables[] =
				{
					{"camera_zoom", TSDEVTERMINAL_VARIABLE_TYPE_f32, &core->camera_zoom},
					{"camera_offset", TSDEVTERMINAL_VARIABLE_TYPE_v2, &core->camera_offset},
					{"fullscreen", TSDEVTERMINAL_VARIABLE_TYPE_b32, &platform->fullscreen},
					{"bloom", TSDEVTERMINAL_VARIABLE_TYPE_b32, &core->client_data->bloom},
				};

			TsDevTerminalSetCommands(ArrayCount(commands), commands, core->permanent_arena);
			TsDevTerminalSetVariables(ArrayCount(variables), variables, core->permanent_arena);
		}

		// NOTE(rjf): Initialize TsAssets.
		{
			TsAssetType asset_types[] =
				{

#define TsAssetsAssetType(name, load_info_name, max, pre_load, load, post_load, clean_up, is_loaded, folder) \
	{                                                                                                        \
		#name,                                                                                               \
		#load_info_name,                                                                                     \
		folder,                                                                                              \
		sizeof(name),                                                                                        \
		sizeof(load_info_name),                                                                              \
		max,                                                                                                 \
		pre_load,                                                                                            \
		load,                                                                                                \
		post_load,                                                                                           \
		clean_up,                                                                                            \
		is_loaded,                                                                                           \
	},
#include "arcane_asset_types.inc"
#undef TsAssetsAssetType

				};

			TsAssetsSetAssetRootPath(MakeCStringOnMemoryArena(core->permanent_arena, "%s/res/", platform->executable_folder_absolute_path));
			TsAssetsSetAssetTypes(ArrayCount(asset_types), asset_types, core->permanent_arena);
		}

		// NOTE(tjr): Initialise Arcane data.
		{
			InitialiseSpriteData();

			InitialiseECS();

			core->camera_zoom = DEFAULT_CAMERA_ZOOM;

			core->delta_mult = 1.0f;
			core->world_delta_mult = 1.0f;

			TempInitGameWorld();
		}

		Ts2dSetDefaultFont(TsAssetsRequestAssetByName(ASSET_TYPE_Ts2dFont, "mono"));
	}
}

internal void
GameHotLoad(void)
{
}

internal void
GameHotUnload(void)
{
}

internal void
GameUpdate(void)
{

	// NOTE(rjf): Load data from platform.
	{
		core->render_w = (f32)platform->window_width;
		core->render_h = (f32)platform->window_height;
		core->raw_delta_t = (f32)(1.f / platform->target_frames_per_second);
	}

	// NOTE(tjr): Key stuff
	{
		// NOTE(tjr): Entering / exiting full-screen
		if (platform->key_pressed[KEY_f11])
			platform->fullscreen = !platform->fullscreen;

		// NOTE(tjr): Enter editor mode
		if (platform->key_pressed[KEY_f1])
		{
			if (core->client_data->editor_state == EDITOR_STATE_entity)
			{
				core->client_data->editor_state = EDITOR_STATE_none;
			}
			else
			{
				core->client_data->editor_state = EDITOR_STATE_entity;
			}
		}
		else if (platform->key_pressed[KEY_f2])
		{
			if (core->client_data->editor_state == EDITOR_STATE_terrain)
			{
				core->client_data->editor_state = EDITOR_STATE_none;
			}
			else
			{
				core->client_data->editor_state = EDITOR_STATE_terrain;
			}
		}
		else if (platform->key_pressed[KEY_f3])
		{
			if (core->client_data->editor_state == EDITOR_STATE_collision)
			{
				core->client_data->editor_state = EDITOR_STATE_none;
			}
			else
			{
				core->client_data->editor_state = EDITOR_STATE_collision;
			}
		}

		{
			local_persist b8 initiated_click = 0;
			local_persist b8 has_released = 0;

			if (has_released)
			{
				core->left_mouse_released = 0;
				has_released = 0;
			}

			if (initiated_click && !platform->left_mouse_down)
			{
				// Normal release
				core->left_mouse_released = 1;
				initiated_click = 0;
				has_released = 1;
			}

			if (initiated_click && platform->left_mouse_pressed)
			{
				// double click
				core->left_mouse_released = 1;
				initiated_click = 0;
				has_released = 1;

				platform->left_mouse_pressed = 0;
			}

			if (platform->left_mouse_pressed && core->left_mouse_released)
				R_BREAK("Both a press and a release can not occur at the same time.");

			if (platform->left_mouse_pressed)
			{
				initiated_click = 1;
			}
		}

		{
			local_persist b8 initiated_click = 0;
			local_persist b8 has_released = 0;

			if (has_released)
			{
				core->right_mouse_released = 0;
				has_released = 0;
			}

			if (initiated_click && !platform->right_mouse_down)
			{
				// Normal release
				core->right_mouse_released = 1;
				initiated_click = 0;
				has_released = 1;
			}

			if (initiated_click && platform->right_mouse_pressed)
			{
				// double click
				core->right_mouse_released = 1;
				initiated_click = 0;
				has_released = 1;

				platform->right_mouse_pressed = 0;
			}

			if (platform->right_mouse_pressed && core->right_mouse_released)
				R_BREAK("Both a press and a release can not occur at the same time.");

			if (platform->right_mouse_pressed)
			{
				initiated_click = 1;
			}
		}

		if (!core->is_mid_right_click && platform->left_mouse_pressed)
		{
			core->is_mid_left_click = 1;
		}
		else if (!core->is_mid_right_click && core->left_mouse_released)
		{
			core->is_mid_left_click = 0;
		}

		if (!core->is_mid_left_click && platform->right_mouse_pressed)
		{
			core->is_mid_right_click = 1;
		}
		else if (!core->is_mid_left_click && core->right_mouse_released)
		{
			core->is_mid_right_click = 0;
		}
	}

	// NOTE(tjr): Calculate delta times.
	{
		core->delta_t = core->raw_delta_t * core->delta_mult;
		core->world_delta_t = core->delta_t * core->world_delta_mult;
		core->world_data->elapsed_world_time += core->world_delta_t;
	}

	// NOTE(tjr): Application-wide slow-motion controls
	{
		static b8 is_time_dilated = 0;
		static f32 last_multiplier = 0.25f;
		if (platform->key_pressed[KEY_x])
		{
			if (is_time_dilated)
			{
				last_multiplier = core->delta_mult;
				core->delta_mult = 1.0f;
				is_time_dilated = 0;
			}
			else
			{
				core->delta_mult = last_multiplier;
				is_time_dilated = 1;
			}
		}

		if (is_time_dilated)
		{
			TsUIPushPosition(v2(10.0f, core->render_h - 70.0f));
			TsUIPushColumn(v2(0.0f, 0.0f), v2(250.0f, 30.0f));
			TsUILabel("Application-wide time-dilation is in effect");
			core->delta_mult = TsUISlider("App Time Dilation", core->delta_mult, 0.0f, 1.0f);
			TsUIPopColumn();
			TsUIPopPosition();
		}
	}

	// NOTE(rjf): Update.
	{
		if (core->is_ingame)
		{
			/* v2 world_mouse_pos = GetMousePositionInWorldSpace();
            Log("%f %f", world_mouse_pos.x, world_mouse_pos.y);
            PushDebugLine(v2(world_mouse_pos.x - 10.0f, world_mouse_pos.y), v2(world_mouse_pos.x + 10.0f, world_mouse_pos.y), v3(1, 0, 0));
            PushDebugLine(v2(world_mouse_pos.x, world_mouse_pos.y - 10.0f), v2(world_mouse_pos.x, world_mouse_pos.y + 10.0f), v3(1, 0, 0)); */

			DrawEditorUI();
			if (core->client_data->editor_state)
				TransformEditorCamera();

			core->performance_timer_count = 0;

			START_PERF_TIMER("Update");

			// NOTE(tjr): Perform if the game is not paused.
			if (core->world_delta_t != 0.0f)
			{
				UpdateCellMaterials();

				PreMoveUpdatePlayer();

				UpdateChunks();
				UpdatePhysics();

				if (!core->client_data->editor_state)
					TransformInGameCamera();

				PostMoveUpdatePlayer();
			}

			UpdateClouds();
			UpdateParallax();

			if (core->client_data->editor_flags & EDITOR_FLAGS_draw_world)
			{
				DrawWorld();
				RenderCells();
			}

			if (core->client_data->editor_flags & EDITOR_FLAGS_draw_collision)
			{
				RenderColliders();
			}

			UpdateParticleEmitters();
			DrawGameUI();
			DrawDebugLines();

			END_PERF_TIMER;
		}
		else
		{
			TsUIBeginInputGroup();
			TsUIPushCenteredColumn(v2(200, 50), 3);
			{
				TsUIMenuTitle("Arcane");
				TsUIDivider();
				if (TsUIMenuButton("Play"))
				{
					TempInitGameWorld();
				}
				if (TsUIMenuButton("Quit"))
				{
					platform->quit = 1;
				}
			}
			TsUIPopColumn();
			TsUIEndInputGroup();
		}
	}
}

internal v2 GetMousePositionInWorldSpace()
{
	return v2(platform->mouse_x / core->camera_zoom - core->camera_position.x - GetZeroWorldPosition().x, platform->mouse_y / core->camera_zoom - core->camera_position.y - GetZeroWorldPosition().y);
}

// NOTE(tjr): Determine if the mouse position is overlapping a shape within world-space. Shape must be AABB for now.
/* internal b8 IsMouseOverlappingWorldShape(Shape shape, v2 shape_world_pos)
{
	v2 mouse_pos = GetMousePositionInWorldSpace();
	if (mouse_pos.x > shape.vertices[0].x + shape_world_pos.x && mouse_pos.x <= shape.vertices[1].x + shape_world_pos.x &&
		mouse_pos.y > shape.vertices[0].y + shape_world_pos.y && mouse_pos.y <= shape.vertices[2].y + shape_world_pos.y)
		return 1;
	else
		return 0;
} */

internal void PushDebugLine(v2 p1, v2 p2, v3 colour)
{
	DebugLine debug_line = {
		1,
		p1,
		p2,
		colour,
		0,
		0.0f,
		0.0f,
	};

	if (core->debug_line_count == core->free_debug_line_index)
	{
		core->debug_lines[core->debug_line_count++] = debug_line;
		core->free_debug_line_index++;
	}
	else
	{
		core->debug_lines[core->free_debug_line_index] = debug_line;

		b8 found_free_index = 0;
		for (int i = 0; i < core->debug_line_count + 1; i++)
		{
			if (!core->debug_lines[i].is_valid)
			{
				core->free_debug_line_index = i;
				found_free_index = 1;
				break;
			}
		}

		R_DEV_ASSERT(found_free_index, "Couldn't find a spare index.");
	}
}

internal void PushDebugLineForDuration(v2 p1, v2 p2, v3 colour, f32 lifetime)
{
	DebugLine debug_line = {
		1,
		p1,
		p2,
		colour,
		1,
		lifetime,
		core->world_data->elapsed_world_time,
	};

	if (core->debug_line_count == core->free_debug_line_index)
	{
		core->debug_lines[core->debug_line_count++] = debug_line;
		core->free_debug_line_index++;
	}
	else
	{
		core->debug_lines[core->free_debug_line_index] = debug_line;

		b8 found_free_index = 0;
		for (int i = 0; i < core->debug_line_count + 1; i++)
		{
			if (!core->debug_lines[i].is_valid)
			{
				core->free_debug_line_index = i;
				found_free_index = 1;
				break;
			}
		}

		R_DEV_ASSERT(found_free_index, "Couldn't find a spare index.");
	}
}

internal void DrawDebugLines()
{
	for (int i = 0; i < core->debug_line_count; i++)
	{
		DebugLine *debug_line = &core->debug_lines[i];

		if (debug_line->is_valid && debug_line->has_duration && debug_line->start_time + debug_line->lifetime <= core->world_data->elapsed_world_time)
		{
			DebugLine empty_debug_line = {0};
			core->debug_lines[i] = empty_debug_line;

			if (i < core->free_debug_line_index)
				core->free_debug_line_index = i;
		}

		if (debug_line->is_valid)
		{
			f32 alpha;
			if (debug_line->has_duration)
				alpha = ((debug_line->start_time + debug_line->lifetime) - core->world_data->elapsed_world_time) / debug_line->lifetime;
			else
				alpha = 1.0f;

			Ts2dPushLine(v4(debug_line->colour.r, debug_line->colour.g, debug_line->colour.b, alpha),
						 v2view(debug_line->p1),
						 v2view(debug_line->p2));

			if (!debug_line->has_duration)
			{
				DebugLine empty_debug_line = {0};
				core->debug_lines[i] = empty_debug_line;

				if (i < core->free_debug_line_index)
					core->free_debug_line_index = i;
			}
		}
	}
}