/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

// NOTE(rjf): Keys

enum
{
#define Key(name, str) KEY_##name,
#include "tsfoundation_key_list.inc"
#undef Key
    KEY_MAX
};

internal char *
KeyName(i32 key)
{
    local_persist char *key_names[KEY_MAX] = {
#define Key(name, str) str,
#include "tsfoundation_key_list.inc"
#undef Key
    };
    char *key_name = "(Invalid Key)";
    if(key >= 0 && key < KEY_MAX)
    {
        key_name = key_names[key];
    }
    return key_name;
}

// NOTE(rjf): Gamepad Buttons

enum
{
#define GamepadButton(name, str) GAMEPAD_BUTTON_##name,
#include "tsfoundation_gamepad_button_list.inc"
#undef GamepadButton
    GAMEPAD_BUTTON_MAX
};

internal char *
GamepadButtonName(i32 GamepadButton)
{
    local_persist char *GamepadButton_names[GAMEPAD_BUTTON_MAX] = {
#define GamepadButton(name, str) str,
#include "tsfoundation_gamepad_button_list.inc"
#undef GamepadButton
    };
    return GamepadButton_names[GamepadButton];
}

#define TS_PLATFORM_DIRECTORY_LIST_DIRECTORIES (1<<0)
#define TS_PLATFORM_DIRECTORY_LIST_EXTENSIONS  (1<<1)
#define TS_PLATFORM_DIRECTORY_LIST_RECURSIVE   (1<<2)

typedef struct TsPlatformDirectoryList TsPlatformDirectoryList;
struct TsPlatformDirectoryList
{
    i32 flags;
    u32 item_count;
    char **items;
};

typedef int TsWorkerThreadDoJobWorkCallback(void *job_data);
typedef void TsWorkerThreadJobCompleteCallback(void *job_data);

enum
{
    TS_JOB_STATUS_QUEUED,
    TS_JOB_STATUS_WORKING,
    TS_JOB_STATUS_DONE,
};

typedef struct TsWorkQueueJob TsWorkQueueJob;
struct TsWorkQueueJob
{
    void *job_data;
    TsWorkerThreadDoJobWorkCallback *DoWork;
    TsWorkerThreadJobCompleteCallback *JobComplete;
    i32 thread_index;
    volatile i32 status;
};

typedef struct TsGamepadInput TsGamepadInput;
struct TsGamepadInput
{
    b32 connected;
    v2 joystick_1;
    v2 joystick_2;
    f32 trigger_left;
    f32 trigger_right;
    i32 button_states[GAMEPAD_BUTTON_MAX];
};

typedef struct TsPlatform TsPlatform;
struct TsPlatform
{
    // NOTE(rjf): Application Metadata
    char *executable_folder_absolute_path;
    char *executable_absolute_path;
    
    // NOTE(rjf): Application Memory
    MemoryArena permanent_arena;
    MemoryArena scratch_arena;
    
    // NOTE(rjf): Engine Modules
#if TS2D
    void *ts2d;
#endif
#if TS3D
    void *ts3d;
#endif
    
    // NOTE(rjf): Options
    volatile b32 quit;
    b32 vsync;
    b32 fullscreen;
    int window_width;
    int window_height;
    f32 current_time;
    f32 target_frames_per_second;
    b32 wait_for_events_to_update;
    b32 pump_events;
    
    // NOTE(rjf): Mouse Input Data
    f32   mouse_x;
    f32   mouse_y;
    f32   last_mouse_x;
    f32   last_mouse_y;
    f32   mouse_scroll_x;
    f32   mouse_scroll_y;
    b32   left_mouse_down;
    b32   left_mouse_pressed;
    b32   right_mouse_down;
    b32   right_mouse_pressed;
    b32   mouse_position_captured;
    b32   mouse_buttons_captured;
    b32   pre_frame_left_mouse_down;
    b32   pre_frame_right_mouse_down;
    
    // NOTE(rjf): Keyboard Input Data
    b8    key_down            [KEY_MAX];
    b8    key_pressed         [KEY_MAX];
    b8    key_released        [KEY_MAX];
    b8    pre_frame_key_down  [KEY_MAX];
    i32   last_key;
    char *target_text;
    u32   target_text_max_characters;
    u32   target_text_edit_pos;
    b32   keyboard_captured;
    
    // NOTE(rjf): Gamepad Input Data
#ifndef TS_MAX_GAMEPAD_COUNT
#define TS_MAX_GAMEPAD_COUNT 1
#endif
    TsGamepadInput *gamepads;
    TsGamepadInput *last_frame_gamepads;
    TsGamepadInput gamepad_states_1[TS_MAX_GAMEPAD_COUNT];
    TsGamepadInput gamepad_states_2[TS_MAX_GAMEPAD_COUNT];
    
    // NOTE(rjf): Audio Output Data
    f32 *sample_out;
    u32 sample_count_to_output;
    u32 samples_per_second;
    
    // NOTE(rjf): Functions
    void (*OutputError)(char *error_type, char *error_format, ...);
    void (*SaveToFile)(char *path, void *data, u32 data_len);
    void (*AppendToFile)(char *path, void *data, u32 data_len);
    void (*LoadEntireFile)(char *path, void **data, u32 *data_len, b32 error_on_non_existence);
    char *(*LoadEntireFileAndNullTerminate)(char *path);
    void (*FreeFileMemory)(void *data);
    void (*DeleteFile)(char *path);
    b32 (*MakeDirectory)(char *path);
    b32 (*DoesFileExist)(char *path);
    b32 (*DoesDirectoryExist)(char *path);
    b32 (*CopyFile)(char *dest, char *source);
    b32 (*CopyDirectoryRecursively)(char *dest, char *source);
    TsPlatformDirectoryList (*PlatformDirectoryListLoad)(char *path, i32 flags);
    void (*PlatformDirectoryListCleanUp)(TsPlatformDirectoryList *file_list);
    void *(*HeapAlloc)(u32 size);
    void (*HeapFree)(void *memory);
    i32 (*QueueJob)(void *job_data, TsWorkerThreadDoJobWorkCallback *DoWork, TsWorkerThreadJobCompleteCallback *JobComplete);
#define TS_WAIT_FOREVER (0xFFFFFFFF)
    b32 (*WaitForJob)(i32 job_index, u32 milliseconds);
    f32 (*GetTime)(void);
    u64 (*GetCycles)(void);
    void (*ResetCursor)(void);
    void (*SetCursorToHorizontalResize)(void);
    void (*SetCursorToVerticalResize)(void);
    
    // NOTE(rjf): Work Queue
#define TS_MAX_WORK_QUEUE_JOBS 4096
    TsWorkQueueJob work_queue_jobs[TS_MAX_WORK_QUEUE_JOBS];
    u32 work_queue_free_index_count;
    i32 work_queue_free_indices[TS_MAX_WORK_QUEUE_JOBS];
};

global TsPlatform *platform = 0;

inline internal void
TsPlatformCaptureMousePosition(void)
{
    platform->mouse_x = -1000;
    platform->mouse_y = -1000;
    platform->mouse_scroll_x = 0;
    platform->mouse_scroll_y = 0;
    platform->mouse_position_captured = 1;
}

inline internal void
TsPlatformCaptureMouseButtons(void)
{
    platform->left_mouse_pressed = 0;
    platform->left_mouse_down = 0;
    platform->right_mouse_pressed = 0;
    platform->right_mouse_down = 0;
    platform->mouse_buttons_captured = 1;
}

inline internal void
TsPlatformCaptureKeyboard(void)
{
    MemorySet(platform->key_down, 0, sizeof(platform->key_down));
    MemorySet(platform->key_pressed, 0, sizeof(platform->key_pressed));
    MemorySet(platform->key_released, 0, sizeof(platform->key_released));
    platform->last_key = 0;
    platform->keyboard_captured = 1;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
TsPlatformBeginFrame(void)
{
    platform->pre_frame_left_mouse_down = platform->left_mouse_down;
    platform->pre_frame_right_mouse_down = platform->right_mouse_down;
    MemoryCopy(platform->pre_frame_key_down, platform->key_down, sizeof(platform->key_down[0]) * KEY_MAX);
    platform->target_text = 0;
    platform->pump_events = 0;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
TsPlatformEndFrame(void)
{
    platform->last_mouse_x = platform->mouse_x;
    platform->last_mouse_y = platform->mouse_y;
    
    // NOTE(rjf): Prepare input data for next frame
    {
        platform->last_key = 0;
        for(u32 i = 0; i < KEY_MAX; ++i)
        {
            platform->key_pressed[i] = 0;
            platform->key_released[i] = 0;
        }
        platform->keyboard_captured = 0;
        
        platform->mouse_position_captured = 0;
        platform->mouse_buttons_captured = 0;
        platform->mouse_scroll_x = 0.f;
        platform->mouse_scroll_y = 0.f;
        platform->left_mouse_down = platform->pre_frame_left_mouse_down;
        platform->right_mouse_down = platform->pre_frame_right_mouse_down;
        MemoryCopy(platform->key_down, platform->pre_frame_key_down, sizeof(platform->key_down[0]) * KEY_MAX);
        platform->left_mouse_pressed = 0;
        platform->right_mouse_pressed = 0;
        
        platform->last_frame_gamepads = platform->gamepads;
        if(platform->gamepads == platform->gamepad_states_1)
        {
            platform->gamepads = platform->gamepad_states_2;
        }
        else
        {
            platform->gamepads = platform->gamepad_states_1;
        }
    }
    
    platform->current_time += 1.f / platform->target_frames_per_second;
}

#ifdef _MSC_VER
#define TS_APP_PROC __declspec(dllexport)
#else
#define TS_APP_PROC
#endif

/* Loaded as "PermanentLoad" */
#define TS_APPLICATION_PERMANENT_LOAD(name) void name(TsPlatform *platform_)
typedef TS_APPLICATION_PERMANENT_LOAD(TsApplicationPermanentLoadCallback);
TS_APPLICATION_PERMANENT_LOAD(TsApplicationPermanentLoadStub) {}

/* Loaded as "HotLoad" */
#define TS_APPLICATION_HOT_LOAD(name) void name(TsPlatform *platform_)
typedef TS_APPLICATION_HOT_LOAD(TsApplicationHotLoadCallback);
TS_APPLICATION_HOT_LOAD(TsApplicationHotLoadStub) {}

/* Loaded as "HotUnload" */
#define TS_APPLICATION_HOT_UNLOAD(name) void name(void)
typedef TS_APPLICATION_HOT_UNLOAD(TsApplicationHotUnloadCallback);
TS_APPLICATION_HOT_UNLOAD(TsApplicationHotUnloadStub) {}

/* Loaded as "Update" */
#define TS_APPLICATION_UPDATE(name) void name(void)
typedef TS_APPLICATION_UPDATE(TsApplicationUpdateCallback);
TS_APPLICATION_UPDATE(TsApplicationUpdateStub) {}
