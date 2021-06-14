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
#include "../../../../telescope-dev/telescope-model/sauce/tsm.h"
#include "core.h"
// NOTE(randy): Game Header Code
#include "arc/entity_presets.h"
#include "arc/interaction.h"
#include "arc/render.h"
#include "arc/particle.h"
#include "arc/editor.h"
#include "arc/blueprint.h"
#include "generated/catchall.h"
#include "arc/crafting.h"
#include "arc/enchantment.h"
#include "arc/skilltree.h"
#include "arc/entity.h"
#include "arc/item.h"
#include "arc/world.h"
#include "arc/collision.h"
#include "arc/player.h"
#include "arc/camera.h"
#include "arc/ui.h"

// NOTE(rjf): Core Implementation Code
#include "telescope/telescope.c"
#include "tsarcane/assets.c"
#include "tsarcane/arcane_tsui_callbacks.c"
// NOTE(randy): Game Implementation Code
#include "arc/entity_presets.c"
#include "arc/util.c"
#include "tsarcane/terminalcommands.c"
#include "arc/interaction.c"
#include "arc/render.c"
#include "arc/arc_entity.c"
#include "arc/particle.c"
#include "generated/catchall.c"
#include "arc/editor.c"
#include "arc/blueprint.c"
#include "arc/crafting.c"
#include "arc/enchantment.c"
#include "arc/skilltree.c"
#include "arc/entity.c"
#include "arc/item.c"
#include "arc/world.c"
#include "arc/collision.c"
#include "arc/player.c"
#include "arc/camera.c"
#include "arc/ui.c"

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
		core->res_path = MakeCStringOnMemoryArena(core->permanent_arena, "%sres\\", platform->executable_folder_absolute_path);
        
		core->client_data = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(ClientData));
		Assert(core->client_data);
        
		core->run_data = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(RunData));
		Assert(core->run_data);
        
		core->editor_data = MemoryArenaAllocateAndZero(core->permanent_arena, sizeof(EditorData));
		Assert(core->run_data);
		
		// NOTE(rjf): Initialize TsDevTerminal.
		{
			TsDevTerminalCommand commands[] =
            {
                {"save", "[Level name]", "Saves current data to the provided level name. If left blank, uses the current level name.", 1, SaveLevelCommand},
                {"load", "[Level name]", "Loads data in from the specified level.", 1, LoadLevelCommand},
				{"draw_colliders", "", "Toggles collision drawing.", 0, DrawCollidersCommand}
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
            
			TsAssetsSetAssetRootPath(core->res_path);
			TsAssetsSetAssetTypes(ArrayCount(asset_types), asset_types, core->permanent_arena);
		}
        
		// NOTE(randy): Initialise Arcane data.
		{
			InitialiseSpriteData();
			ShufflePerlinNoise();
            
			{
				core->tsm = MemoryArenaAllocate(core->permanent_arena, sizeof(TSM));
				
				char path[256];
				sprintf(path, "%s/models/ron.tsm", core->res_path);
				ReadTSMFromFile(core->tsm, path);
				
				Ts2dSubModel sub_model = Ts2dSubModelInitFromTSMData(core->tsm);
				core->model = Ts2dModelInit(1, &sub_model, 0, 0);
				
				// TODO(randy): fill out skele
				// core->skeleton;
			}
			
			/*
						{
			#include "sphere_thing.tsm"
							Ts2dSubModel sub_model = Ts2dSubModelInit(TS2D_VERTEX_POSITION | TS2D_VERTEX_UV | TS2D_VERTEX_NORMAL,
																	  ArrayCount(global_test_model_data) / 8, global_test_model_data,
																	  0, 0, 0);
							core->model_sphere = Ts2dModelInit(1, &sub_model, 0, 0);
						}
						
						{
			#include "test_model.c"
							Ts2dSubModel sub_model = Ts2dSubModelInit(TS2D_VERTEX_POSITION | TS2D_VERTEX_UV | TS2D_VERTEX_NORMAL,
																	  ArrayCount(global_test_model_data) / 8, global_test_model_data,
																	  0, 0, 0);
							core->model_link = Ts2dModelInit(1, &sub_model, 0, 0);
						}
			 */
			
			core->camera_zoom_mult = 1.0f;
			
			core->delta_mult = 1.0f;
			core->world_delta_mult = 1.0f;
            
#ifdef DEVELOPER_ENVIRONMENT
			if (!LoadWorld("testing"))
				CreateWorld("testing");
#else
			core->is_ingame = 0;
#endif
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
    
	// NOTE(randy): Key stuff
	{
		// NOTE(randy): Entering / exiting full-screen
		if (platform->key_pressed[KEY_f11])
			platform->fullscreen = !platform->fullscreen;
        
#ifdef DEVELOPER_TOOLS
		// NOTE(randy): Enter editor mode
		for (i32 i = 1; i < EDITOR_STATE_MAX; i++)
		{
			if (platform->key_pressed[KEY_f1 + i - 1])
			{
				SwitchEditorState(GetRunData()->editor_state == i ? EDITOR_STATE_none : i);
			}
		}
#endif
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
			{
				Assert(0); // NOTE(randy): Both a press and a release can not occur at the same time.
			}
            
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
			{
				// NOTE(randy): Both a press and a release can not occur at the same time.
				Assert(0);
            }
			
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
    
	// NOTE(randy): Calculate delta times.
	{
		core->delta_t = core->raw_delta_t * core->delta_mult;
		core->world_delta_t = core->delta_t * core->world_delta_mult;
		GetWorldData()->elapsed_world_time += core->world_delta_t;
	}
    
#ifdef DEVELOPER_TOOLS
	// NOTE(randy): Application-wide slow-motion controls
	{
		static b8 is_time_dilated = 0;
		static f32 last_multiplier = 0.25f;
		if (platform->key_pressed[KEY_z])
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
#endif
    
	// NOTE(rjf): Update.
	if (core->is_ingame)
	{
		if (GetRunData()->editor_state == EDITOR_STATE_none)
		{
			WorldUpdate();
		}
		else
		{
			EditorUpdate();
		}
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
				if (!LoadWorld("testing"))
					CreateWorld("testing");
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

internal void InitialiseRunData()
{
	GetCharacterData()->elemental_skill_points = 3;
	
	core->camera_zoom = DEFAULT_CAMERA_ZOOM;
	
	global_ts2d->ground_scale = 0.2f;
	global_ts2d->ground_vor_step = 0.13f;
	global_ts2d->ground_band_height = 10.0f;
	
	for (i32 i = 0; i < TERRAIN_TABLE_SIZE; i++)
	{
		GetRunData()->terrain[i] = v2(INFINITY, INFINITY);
	}
}

internal void FreeRunData()
{
	
}