// NOTE(rjf): Third-Party Code
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "ext/stb_image.h"
#include "time.h"

// NOTE(rjf): Core Header Code
#define TSFOUNDATION_LOG_PROCEDURE_FILE "tsarcane/arcane_tsfoundation_log_procedure.h"
#include "tsfoundation/tsfoundation.h"
#include "ts2d/ts2d.h"
#include "tsarcane/assets.h"
#include "tsassets/tsassets.h"
#include "tsdevterminal/tsdevterminal.h"
#define ARCANE_UI_STYLE_MENU (1 << 0)
#define ARCANE_UI_STYLE_GAME (1 << 1)
#include "tsui/tsui.h"
#include "util.h"
#include "tsarcane/arcane_tsui_callbacks.h"
#include "arcane.h"
#include "core.h"
// NOTE(tjr): Game Header Code
#include "arc/entity/player/action.h"
#include "arc/sprite/sprite.h"
#include "arc/entity/arc_entity.h"
#include "arc/item/item.h"
#include "arc/ecs/ecs.h"
#include "arc/world/world.h"
#include "arc/world/collision.h"
#include "arc/entity/player/player.h"
#include "arc/ui/ui.h"
#include "generated/catchall.h"

// NOTE(rjf): Core Implementation Code
#include "ts2d/ts2d.c"
#include "tsarcane/assets.c"
#include "tsassets/tsassets.c"
#include "tsdevterminal/tsdevterminal.c"
#include "tsui/tsui.c"
#include "tsarcane/arcane_tsui_callbacks.c"
#include "tsarcane/arcane_tsfoundation_log_procedure.c"
// NOTE(tjr): Game Implementation Code
#include "util.c"
#include "tsarcane/terminalcommands.c"
#include "arc/entity/player/action.c"
#include "arc/sprite/sprite.c"
#include "arc/entity/arc_entity.c"
#include "arc/ecs/ecs.c"
#include "arc/item/item.c"
#include "arc/world/world.c"
#include "arc/world/collision.c"
#include "arc/entity/player/player.c"
#include "arc/entity/player/camera.c"
#include "arc/ui/ui.c"
#include "generated/catchall.c"

TS_APP_PROC void
PermanentLoad(TsPlatform *platform_)
{
	platform = platform_;
	core = MemoryArenaAllocateAndZero(&platform->permanent_arena, sizeof(*core));
	HardAssert(core != 0);

	// NOTE(rjf): Initialize memory arenas.
	{
		core->permanent_arena = &platform->permanent_arena;
		core->frame_arena = &platform->scratch_arena;
	}

	// NOTE(rjf): Initialize core systems.
	{
		// NOTE(rjf): Initialize TsDevTerminal.
		{
			TsDevTerminalInitInfo init_info = {0};
			{
				TsDevTerminalCommand commands[] = {
					0
					//{"col_draw", "", "Toggles whether the collision bounds are drawn.", 1, ToggleCollisionCommand},
				};

				TsDevTerminalVariable variables[] = {
					{"camera_zoom", TSDEVTERMINAL_VARIABLE_TYPE_f32, &core->camera_zoom},
					{"camera_offset", TSDEVTERMINAL_VARIABLE_TYPE_v2, &core->camera_offset},
					{"shadow_opacity", TSDEVTERMINAL_VARIABLE_TYPE_f32, &core->shadow_opacity},
					{"slow_mult", TSDEVTERMINAL_VARIABLE_TYPE_f32, &core->slow_mult},
					{"bloom", TSDEVTERMINAL_VARIABLE_TYPE_b32, &core->bloom},
					{"draw_colliders", TSDEVTERMINAL_VARIABLE_TYPE_b32, &core->draw_colliders},
					{"draw_velocity", TSDEVTERMINAL_VARIABLE_TYPE_b32, &core->draw_velocity},
					{"developer_view", TSDEVTERMINAL_VARIABLE_TYPE_b32, &core->developer_view}, // TODO: Extract into pressing F1
					{"fullscreen", TSDEVTERMINAL_VARIABLE_TYPE_b32, &platform->fullscreen},
				};

				init_info.frame_arena = core->frame_arena;
				init_info.variable_count = ArrayCount(variables);
				init_info.variables = variables;
				init_info.command_count = ArrayCount(commands);
				init_info.commands = commands;
			}

			core->dev_terminal = MemoryArenaAllocate(core->permanent_arena, sizeof(*core->dev_terminal));
			TsDevTerminalInit(core->dev_terminal, &init_info, core->permanent_arena);
		}

		// NOTE(rjf): Initialize Ts2d.
		{
			core->renderer = platform->renderer;
			Ts2dInit(core->renderer, core->permanent_arena);
		}

		// NOTE(rjf): Initialize TsAssets.
		{
			core->assets = MemoryArenaAllocate(core->permanent_arena, sizeof(*core->assets));
			char *assets_root = MakeCStringOnMemoryArena(core->permanent_arena, "%sres/", platform->executable_folder_absolute_path);
			TsAssetsInit(core->assets, assets_root, core->permanent_arena);
		}

		// NOTE(rjf): Initialize TsUI.
		{
			core->ui = MemoryArenaAllocate(core->permanent_arena, sizeof(*core->ui));
			TsUIInit(core->ui);
		}

		// NOTE(tjr): Initialise Arcane data.
		{
			InitialiseActions();
			InitialiseSpriteData();
			InitialiseItemData();

			core->entity_set = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(*core->entity_set));
			core->component_set = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(*core->component_set));
			InitialiseECS();

			core->camera_zoom = 3.3f;
			core->camera_offset = v2(0.0f, 45.0f);
			core->shadow_opacity = 0.9f;
			core->slow_mult = 0.25f;

			core->delta_mult = 1.0f;
			core->world_delta_mult = 1.0f;

			TempInitGameWorld();
		}

		Ts2dSetDefaultFont(core->renderer, TsAssetsRequestTs2dFontByName(core->assets, "mono"));
	}
}

TS_APP_PROC void
HotLoad(TsPlatform *platform_)
{
	platform = platform_;
}

TS_APP_PROC void
HotUnload(void)
{
}

TS_APP_PROC void
Update(void)
{
	// NOTE(tjr): Temp key stuff
	{
		if (platform->key_pressed[KEY_f11])
			platform->fullscreen = !platform->fullscreen;

		if (platform->key_pressed[KEY_f1])
			core->developer_view = !core->developer_view;

		static b8 slow_motion = 0;
		if (platform->key_pressed[KEY_x])
			slow_motion = !slow_motion;

		if (slow_motion)
			core->delta_mult = core->slow_mult;
		else
			core->delta_mult = 1.0f;

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

	// NOTE(rjf): Load data from platform.
	{
		core->render_w = (f32)platform->window_width;
		core->render_h = (f32)platform->window_height;
		core->raw_delta_t = (f32)(1.f / platform->target_frames_per_second);
	}

	// NOTE(tjr): Calculate delta times.
	{
		core->delta_t = core->raw_delta_t * core->delta_mult;

		b8 is_game_paused_or_some_shit = 0;
		if (!is_game_paused_or_some_shit)
		{
			core->elapsed_world_time += core->delta_t;
			core->world_delta_t = core->delta_t * core->world_delta_mult;
		}
		else
		{
			core->world_delta_t = 0.0f;
		}
	}

	// NOTE(rjf): Begin renderer frame.
	{
		Ts2dBeginFrameInfo begin_frame_info = {0};
		{
			begin_frame_info.render_width = core->render_w;
			begin_frame_info.render_height = core->render_h;
			begin_frame_info.delta_t = core->delta_t;
			begin_frame_info.flags = 0;
		}
		Ts2dBeginFrame(core->renderer, &begin_frame_info);
	}

	TsDevTerminalUpdate(core->dev_terminal, core->delta_t);

	// NOTE(rjf): Begin UI frame.
	{
		TsUIFrameData ui_frame = {0};
		{
			ui_frame.render_width = core->render_width;
			ui_frame.render_height = core->render_height;
			ui_frame.cursor_x = platform->mouse_x * (!platform->mouse_position_captured);
			ui_frame.cursor_y = platform->mouse_y * (!platform->mouse_position_captured);
			ui_frame.cursor_scroll_x = platform->mouse_scroll_x;
			ui_frame.cursor_scroll_y = platform->mouse_scroll_y;
			ui_frame.cursor_left_down = platform->left_mouse_down;
			ui_frame.cursor_right_down = platform->right_mouse_down;
			ui_frame.cursor_left_pressed = platform->left_mouse_pressed;
			ui_frame.cursor_right_pressed = platform->right_mouse_pressed;
			ui_frame.up_pressed |= !platform->keyboard_captured * platform->last_key == KEY_up;
			ui_frame.left_pressed |= !platform->keyboard_captured * platform->last_key == KEY_left;
			ui_frame.down_pressed |= !platform->keyboard_captured * platform->last_key == KEY_down;
			ui_frame.right_pressed |= !platform->keyboard_captured * platform->last_key == KEY_right;

			if (platform->last_frame_gamepads)
			{
				ui_frame.down_pressed |= platform->gamepads[0].joystick_1.y <= -0.5f && platform->last_frame_gamepads[0].joystick_1.y > -0.5f;
				ui_frame.left_pressed |= platform->gamepads[0].joystick_1.x <= -0.5f && platform->last_frame_gamepads[0].joystick_1.x > -0.5f;
				ui_frame.up_pressed |= platform->gamepads[0].joystick_1.y >= +0.5f && platform->last_frame_gamepads[0].joystick_1.y < +0.5f;
				ui_frame.right_pressed |= platform->gamepads[0].joystick_1.x >= +0.5f && platform->last_frame_gamepads[0].joystick_1.x < +0.5f;

				ui_frame.down_hold |= platform->gamepads[0].joystick_1.y <= -0.5f;
				ui_frame.left_hold |= platform->gamepads[0].joystick_1.x <= -0.5f;
				ui_frame.up_hold |= platform->gamepads[0].joystick_1.y >= +0.5f;
				ui_frame.right_hold |= platform->gamepads[0].joystick_1.x >= +0.5f;

				ui_frame.up_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_up]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_dpad_up]));
				ui_frame.left_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_left]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_dpad_left]));
				ui_frame.down_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_down]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_dpad_down]));
				ui_frame.right_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_right]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_dpad_right]));
				ui_frame.enter_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_a]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_a]));
				ui_frame.tab_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_right_shoulder]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_right_shoulder]));
				ui_frame.escape_pressed |= ((platform->gamepads[0].button_states[GAMEPAD_BUTTON_b]) && !(platform->last_frame_gamepads[0].button_states[GAMEPAD_BUTTON_b]));
				ui_frame.up_hold |= (platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_up]);
				ui_frame.left_hold |= (platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_left]);
				ui_frame.down_hold |= (platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_down]);
				ui_frame.right_hold |= (platform->gamepads[0].button_states[GAMEPAD_BUTTON_dpad_right]);
			}

			ui_frame.enter_pressed |= !platform->keyboard_captured * platform->last_key == KEY_enter;
			ui_frame.tab_pressed |= !platform->keyboard_captured * platform->last_key == KEY_tab;
			ui_frame.escape_pressed |= !platform->keyboard_captured * platform->last_key == KEY_esc;
			ui_frame.up_hold |= !platform->keyboard_captured * platform->key_down[KEY_up];
			ui_frame.left_hold |= !platform->keyboard_captured * platform->key_down[KEY_left];
			ui_frame.down_hold |= !platform->keyboard_captured * platform->key_down[KEY_down];
			ui_frame.right_hold |= !platform->keyboard_captured * platform->key_down[KEY_right];
			ui_frame.delta_t = core->delta_t;
			ui_frame.widget_arena = core->frame_arena;
			ui_frame.RenderWidget = ArcaneUIRenderWidget;
			ui_frame.RenderWindow = ArcaneUIRenderWindow;
		}
		TsUIBeginFrame(core->ui, &ui_frame);
	}

	// NOTE(rjf): Update.
	{
		START_PERF_TIMER("Update");
		if (core->is_ingame)
		{
			PreMoveUpdatePlayer();

			AdvanceVelocity(core->component_set->velocity_components, core->component_set->velocity_component_count);
			UpdateTriggers(core->component_set->trigger_components, core->component_set->trigger_component_count);

			PositionCamera();

			PostMoveUpdatePlayer();

			DrawWorld();
			DrawDebugLines();
			DrawGameUI();
		}
		else
		{
			TsUIBeginInputGroup(core->ui);
			TsUIPushCenteredColumn(core->ui, v2(200, 50), 3);
			{
				TsUIMenuTitle(core->ui, "Arcane");
				TsUIDivider(core->ui);
				if (TsUIMenuButton(core->ui, "Play"))
				{
					TempInitGameWorld();
				}
				if (TsUIMenuButton(core->ui, "Quit"))
				{
					platform->quit = 1;
				}
			}
			TsUIPopColumn(core->ui);
			TsUIEndInputGroup(core->ui);
		}
		END_PERF_TIMER;
	}

	core->performance_timer_count = 0;

	// NOTE(rjf): End UI frame.
	{
		START_PERF_TIMER("UI Rendering");
		TsUIEndFrame(core->ui);
		END_PERF_TIMER;
	}

	TsDevTerminalRender(core->dev_terminal);

	// NOTE(rjf): End renderer frame.
	{
		START_PERF_TIMER("Rendering");
		Ts2dEndFrame(core->renderer);
		Ts2dSwapBuffers(core->renderer);
		END_PERF_TIMER;
	}

	// NOTE(rjf): Update assets.
	{
		START_PERF_TIMER("Asset Update");
		TsAssetsUpdate(core->assets);
		END_PERF_TIMER;
	}
}

internal void test_enter(OverlappedColliderInfo overlap)
{
	Log("entered");
}

internal void test_exit(OverlappedColliderInfo overlap)
{
	Log("exited");
}

internal void PushDebugLine(v2 p1, v2 p2, v3 colour, f32 lifetime)
{
	DebugLine debug_line = {
		1,
		p1,
		p2,
		colour,
		lifetime,
		core->elapsed_world_time,
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

internal void PushDebugShape(Shape shape, v2 position, v3 colour, f32 lifetime)
{
	for (int i = 0; i < shape.vertex_count; i++)
	{
		int secondPoint = (i == shape.vertex_count - 1 ? 0 : i + 1);

		v2 p1 = V2AddV2(position, v2(shape.vertices[i].x, shape.vertices[i].y));
		v2 p2 = V2AddV2(position, v2(shape.vertices[secondPoint].x, shape.vertices[secondPoint].y));

		PushDebugLine(p1,
					  p2,
					  colour,
					  lifetime);
	}
}

internal void DrawDebugLines()
{
	for (int i = 0; i < core->debug_line_count; i++)
	{
		DebugLine *debug_line = &core->debug_lines[i];

		if (debug_line->is_valid)
		{
			if (debug_line->start_time + debug_line->lifetime <= core->elapsed_world_time)
			{
				DebugLine empty_debug_line = {0};
				core->debug_lines[i] = empty_debug_line;

				if (i < core->free_debug_line_index)
					core->free_debug_line_index = i;
			}
			else
			{
				f32 alpha = ((debug_line->start_time + debug_line->lifetime) - core->elapsed_world_time) / debug_line->lifetime;

				Ts2dPushLine(core->renderer,
							 v4(debug_line->colour.r, debug_line->colour.g, debug_line->colour.b, alpha),
							 v2view(debug_line->p1),
							 v2view(debug_line->p2));
			}
		}
	}
}