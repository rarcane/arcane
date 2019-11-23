// NOTE(rjf): Windows
#include "tsfoundation_windows.h"
#include <windowsx.h>
#include <xinput.h>
#include <gl/gl.h>
#include <objbase.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#undef DeleteFile

// NOTE(rjf): CRT
#include <stdio.h>

// NOTE(rjf): Telescope Headers
#include "tsfoundation.h"
#include "tsfoundation_win32_timer.h"
#include "tsfoundation_win32_threads.h"

// NOTE(rjf): Globals
global char global_executable_path[256];
global char global_executable_directory[256];
global char global_app_dll_path[256];
global char global_temp_app_dll_path[256];
global TsPlatform global_platform;
global HDC global_device_context;
global HINSTANCE global_instance_handle;
global Win32Timer global_win32_timer = {0};
global u32 global_win32_worker_thread_max = 0;
global Align(64) Win32WorkerThreadData global_win32_worker_threads[64] = {0};

// NOTE(rjf): Telescope Implementations
#include "tsfoundation_win32_utilities.c"
#include "tsfoundation_win32_timer.c"
#include "tsfoundation_win32_file_io.c"
#include "tsfoundation_win32_app_code.c"
#include "tsfoundation_win32_xinput.c"
#include "tsfoundation_win32_wasapi.c"
#include "tsfoundation_win32_threads.c"

#ifndef TSFOUNDATION_WIN32_FILE
#define TSFOUNDATION_WIN32_FILE "tsfoundation_win32_default.c"
#endif
#include TSFOUNDATION_WIN32_FILE

typedef enum Win32CursorStyle Win32CursorStyle;
enum Win32CursorStyle
{
    WIN32_CURSOR_normal,
    WIN32_CURSOR_horizontal_resize,
    WIN32_CURSOR_vertical_resize,
};

global Win32CursorStyle global_cursor_style = 0;

internal LRESULT
Win32WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result;
    if(message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT)
    {
        global_platform.quit = 1;
        result = 0;
    }
    else if(message == WM_LBUTTONDOWN)
    {
        if(!global_platform.left_mouse_down)
        {
            global_platform.left_mouse_pressed = 1;
        }
        global_platform.left_mouse_down = 1;
    }
    else if(message == WM_LBUTTONUP)
    {
        global_platform.left_mouse_down = 0;
        global_platform.left_mouse_pressed = 0;
    }
    else if(message == WM_RBUTTONDOWN)
    {
        if(!global_platform.right_mouse_down)
        {
            global_platform.right_mouse_pressed = 1;
        }
        global_platform.right_mouse_down = 1;
    }
    else if(message == WM_RBUTTONUP)
    {
        global_platform.right_mouse_down = 0;
        global_platform.right_mouse_pressed = 0;
    }
    else if(message == WM_MOUSEWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        global_platform.mouse_scroll_y = (f32)wheel_delta;
    }
    else if(message == WM_MOUSEHWHEEL)
    {
        i16 wheel_delta = HIWORD(w_param);
        global_platform.mouse_scroll_x = (f32)wheel_delta;
    }
    else if(message == WM_SETCURSOR)
    {
        switch(global_cursor_style)
        {
            case WIN32_CURSOR_horizontal_resize:
            {
                SetCursor(LoadCursorA(0, IDC_SIZEWE));
                break;
            }
            case WIN32_CURSOR_vertical_resize:
            {
                SetCursor(LoadCursorA(0, IDC_SIZENS));
                break;
            }
            case WIN32_CURSOR_normal:
            {
                SetCursor(LoadCursorA(0, IDC_ARROW));
                break;
            }
            default: break;
        }
    }
    else if(message == WM_SYSKEYDOWN || message == WM_SYSKEYUP ||
            message == WM_KEYDOWN || message == WM_KEYUP)
    {
        u64 vkey_code = w_param;
        i8 was_down = !!(l_param & (1 << 30));
        i8 is_down =   !(l_param & (1 << 31));
        
        u64 key_input = 0;
        
        if((vkey_code >= 'A' && vkey_code <= 'Z') || 
           (vkey_code >= '0' && vkey_code <= '9'))
        {
            // NOTE(rjf): Letter/number buttons
            key_input = (vkey_code >= 'A' && vkey_code <= 'Z') ? KEY_a + (vkey_code-'A') : KEY_0 + (vkey_code-'0');
        }
        else
        {
            if(vkey_code == VK_ESCAPE)
            {
                key_input = KEY_esc;
            }
            else if(vkey_code >= VK_F1 && vkey_code <= VK_F12)
            {
                key_input = KEY_f1 + vkey_code - VK_F1;
            }
            else if(vkey_code == VK_OEM_3)
            {
                key_input = KEY_grave_accent;
            }
            else if(vkey_code == VK_OEM_MINUS)
            {
                key_input = KEY_minus;
            }
            else if(vkey_code == VK_OEM_PLUS)
            {
                key_input = KEY_equal;
            }
            else if(vkey_code == VK_BACK)
            {
                key_input = KEY_backspace;
            }
            else if(vkey_code == VK_TAB)
            {
                key_input = KEY_tab;
            }
            else if(vkey_code == VK_SPACE)
            {
                key_input = KEY_space;
            }
            else if(vkey_code == VK_RETURN)
            {
                key_input = KEY_enter;
            }
            else if(vkey_code == VK_CONTROL)
            {
                key_input = KEY_ctrl;
            }
            else if(vkey_code == VK_SHIFT)
            {
                key_input = KEY_shift;
            }
            else if(vkey_code == VK_MENU)
            {
                key_input = KEY_alt;
            }
            else if(vkey_code == VK_UP)
            {
                key_input = KEY_up;
            }
            else if(vkey_code == VK_LEFT)
            {
                key_input = KEY_left;
            }
            else if(vkey_code == VK_DOWN)
            {
                key_input = KEY_down;
            }
            else if(vkey_code == VK_RIGHT)
            {
                key_input = KEY_right;
            }
            else if(vkey_code == VK_DELETE)
            {
                key_input = KEY_delete;
            }
            else if(vkey_code == VK_PRIOR)
            {
                key_input = KEY_page_up;
            }
            else if(vkey_code == VK_NEXT)
            {
                key_input = KEY_page_down;
            }
        }
        
        if(is_down)
        {
            if(!global_platform.key_down[key_input])
            {
                ++global_platform.key_pressed[key_input];
            }
            ++global_platform.key_down[key_input];
            global_platform.last_key = (i32)key_input;
            
            if(key_input == KEY_backspace && global_platform.target_text)
            {
                if(global_platform.key_down[KEY_ctrl])
                {
                    for(u32 i = global_platform.target_text_edit_pos-2;
                        i >= 0 && i < global_platform.target_text_max_characters; 
                        --i)
                    {
                        if(!i || global_platform.target_text[i-1] == ' ')
                        {
                            global_platform.target_text[i] = 0;
                            global_platform.target_text_edit_pos = i;
                            break;
                        }
                    }
                }
                else
                {
                    if(global_platform.target_text_edit_pos)
                    {
                        // NOTE(rjf): This assumes editing only takes place at
                        //            the end of the string!!!
                        global_platform.target_text[--global_platform.target_text_edit_pos] = 0;
                    }
                }
            }
            else if(key_input == KEY_f4 && global_platform.key_down[KEY_alt])
            {
                global_platform.quit = 1;
            }
        }
        else
        {
            if(was_down)
            {
                global_platform.key_released[key_input] = 1;
            }
            global_platform.key_down[key_input] = 0;
            global_platform.key_pressed[key_input] = 0;
        }
    }
    else if(message == WM_CHAR)
    {
        u64 char_input = w_param;
        if(char_input >= 32 && char_input < 127)
        {
            if(global_platform.target_text && global_platform.target_text_edit_pos < global_platform.target_text_max_characters - 1)
            {
                i8 was_down = !!(l_param & (1 << 30));
                i8 is_down = !(l_param & (1 << 31));
                
                if(is_down)
                {
                    global_platform.target_text[global_platform.target_text_edit_pos++] = (i8)char_input;
                    global_platform.target_text[global_platform.target_text_edit_pos] = 0;
                }
            }
        }
        
    }
    else
    {
        result = DefWindowProc(window_handle, message, w_param, l_param);
    }
    
    return result;
}

internal f32
Win32GetTime(void)
{
    Win32Timer *timer = &global_win32_timer;
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return global_platform.current_time + (f32)(current_time.QuadPart - timer->begin_frame.QuadPart) / (f32)timer->counts_per_second.QuadPart;
}

internal u64
Win32GetCycles(void)
{
    u64 result = __rdtsc();
    return result;
}

internal void
Win32ResetCursor(void)
{
    global_cursor_style = WIN32_CURSOR_normal;
}

internal void
Win32SetCursorToHorizontalResize(void)
{
    global_cursor_style = WIN32_CURSOR_horizontal_resize;
}

internal void
Win32SetCursorToVerticalResize(void)
{
    global_cursor_style = WIN32_CURSOR_vertical_resize;
}

int
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR lp_cmd_line, int n_show_cmd)
{
    global_instance_handle = instance;
    
    Win32TimerInit(&global_win32_timer);
    Win32AppCode win32_game_code = {0};
    Win32SoundOutput win32_sound_output = {0};
    
    // NOTE(rjf): Calculate executable name and path to DLL
    {
        DWORD size_of_executable_path =
            GetModuleFileNameA(0, global_executable_path, sizeof(global_executable_path));
        
        // NOTE(rjf): Calculate executable directory
        {
            MemoryCopy(global_executable_directory, global_executable_path, size_of_executable_path);
            char *one_past_last_slash = global_executable_directory;
            for(i32 i = 0; global_executable_directory[i]; ++i)
            {
                if(global_executable_directory[i] == '\\')
                {
                    one_past_last_slash = global_executable_directory + i + 1;
                }
            }
            *one_past_last_slash = 0;
        }
        
        // NOTE(rjf): Create DLL filenames
        {
            wsprintf(global_app_dll_path, "%s%s.dll", global_executable_directory, TS_APP_FILENAME);
            wsprintf(global_temp_app_dll_path, "%stemp_%s.dll", global_executable_directory, TS_APP_FILENAME);
        }
    }
    
    WNDCLASS window_class = {0};
    {
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = Win32WindowProc;
        window_class.hInstance = instance;
        window_class.lpszClassName = "TsApplicationWindowClass";
        window_class.hCursor = LoadCursor(0, IDC_ARROW);
    }
    
    if(!RegisterClass(&window_class))
    {
        // NOTE(rjf): ERROR: Window class registration failure
        Win32OutputError("Fatal Error", "Window class registration failure.");
        goto quit;
    }
    
    HWND window_handle = CreateWindow("TsApplicationWindowClass", TS_APP_WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      TS_APP_DEFAULT_WINDOW_WIDTH,
                                      TS_APP_DEFAULT_WINDOW_HEIGHT,
                                      0, 0, instance, 0);
    
    if(!window_handle)
    {
        // NOTE(rjf): ERROR: Window creation failure
        Win32OutputError("Fatal Error", "Window creation failure.");
        goto quit;
    }
    
    // NOTE(rjf): Load application code
    Win32AppCode win32_app_code = {0};
    {
        if(!Win32AppCodeLoad(&win32_app_code))
        {
            // NOTE(rjf): ERROR: Application code load failure
            Win32OutputError("Fatal Error", "Application code load failure.");
            goto quit;
        }
    }
    
    // NOTE(rjf): Sound initialization
    {
        win32_sound_output.channels = 2;
        win32_sound_output.samples_per_second = 48000;
        win32_sound_output.latency_frame_count = 48000;
        Win32LoadWASAPI();
        Win32InitWASAPI(&win32_sound_output);
    }
    
    // NOTE(rjf): Initialize platform
    {
        global_platform.executable_folder_absolute_path = global_executable_directory;
        global_platform.executable_absolute_path = global_executable_path;
        
        u32 permanent_storage_size       = TS_APP_PERMANENT_STORAGE_SIZE;
        void *permanent_storage          = VirtualAlloc(0, permanent_storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        global_platform.permanent_arena  = MemoryArenaInit(permanent_storage, permanent_storage_size);
        u32 scratch_storage_size         = TS_APP_SCRATCH_STORAGE_SIZE;
        void *scratch_storage            = VirtualAlloc(0, scratch_storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        global_platform.scratch_arena    = MemoryArenaInit(scratch_storage, scratch_storage_size);
        
        global_platform.quit                      = 0;
        global_platform.vsync                     = 1;
        global_platform.fullscreen                = 0;
        global_platform.window_width              = TS_APP_DEFAULT_WINDOW_WIDTH;
        global_platform.window_height             = TS_APP_DEFAULT_WINDOW_HEIGHT;
        global_platform.current_time              = 0.f;
        global_platform.target_frames_per_second  = 60.f;
        
        global_platform.gamepads = global_platform.gamepad_states_1;
        
        global_platform.sample_out = Win32HeapAlloc(win32_sound_output.samples_per_second * sizeof(f32) * 2);
        global_platform.samples_per_second = win32_sound_output.samples_per_second;
        
        global_platform.OutputError                    = Win32OutputError;
        global_platform.HeapAlloc                      = Win32HeapAlloc;
        global_platform.HeapFree                       = Win32HeapFree;
        global_platform.SaveToFile                     = Win32SaveToFile;
        global_platform.AppendToFile                   = Win32AppendToFile;
        global_platform.LoadEntireFile                 = Win32LoadEntireFile;
        global_platform.LoadEntireFileAndNullTerminate = Win32LoadEntireFileAndNullTerminate;
        global_platform.FreeFileMemory                 = Win32FreeFileMemory;
        global_platform.DeleteFile                     = Win32DeleteFile;
        global_platform.MakeDirectory                  = Win32MakeDirectory;
        global_platform.DoesFileExist                  = Win32DoesFileExist;
        global_platform.PlatformDirectoryListLoad      = Win32PlatformDirectoryListLoad;
        global_platform.PlatformDirectoryListCleanUp   = Win32PlatformDirectoryListCleanUp;
        global_platform.QueueJob                       = Win32QueueJob;
        global_platform.WaitForJob                     = Win32WaitForJob;
        global_platform.GetTime                        = Win32GetTime;
        global_platform.GetCycles                      = Win32GetCycles;
        global_platform.ResetCursor                    = Win32ResetCursor;
        global_platform.SetCursorToHorizontalResize    = Win32SetCursorToHorizontalResize;
        global_platform.SetCursorToVerticalResize      = Win32SetCursorToVerticalResize;
        
        global_platform.work_queue_free_index_count = TS_MAX_WORK_QUEUE_JOBS;
        for(i32 i = 0; i < TS_MAX_WORK_QUEUE_JOBS; ++i)
        {
            global_platform.work_queue_free_indices[i] = i;
        }
        
        platform = &global_platform;
    }
    
    // NOTE(rjf): Initialize job system threads
    {
        SYSTEM_INFO system_info = {0};
        GetSystemInfo(&system_info);
        DWORD number_of_logical_processors = system_info.dwNumberOfProcessors;
        u32 number_of_worker_threads = number_of_logical_processors - 2;
        global_win32_worker_thread_max = number_of_worker_threads;
        
        for(u32 i = 0; i < number_of_worker_threads; ++i)
        {
            Win32WorkerThreadDataInit(global_win32_worker_threads + i);
        }
    }
    
    global_device_context = GetDC(window_handle);
    Win32LoadXInput();
    
    TsFoundationWin32Init();
    
    win32_app_code.PermanentLoad(&global_platform);
    win32_app_code.HotLoad(&global_platform);
    
    ShowWindow(window_handle, n_show_cmd);
    UpdateWindow(window_handle);
    
    while(!global_platform.quit)
    {
        Win32TimerBeginFrame(&global_win32_timer);
        
        // NOTE(rjf): Update Windows events
        {
            MSG message;
            if(global_platform.wait_for_events_to_update && !global_platform.pump_events)
            {
                WaitMessage();
            }
            
            while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        
        // NOTE(rjf): Update window size
        {
            RECT client_rect;
            GetClientRect(window_handle, &client_rect);
            global_platform.window_width = client_rect.right - client_rect.left;
            global_platform.window_height = client_rect.bottom - client_rect.top;
        }
        
        // NOTE(rjf): Update input data (post-event)
        TsPlatformBeginFrame();
        {
            POINT mouse;
            GetCursorPos(&mouse);
            ScreenToClient(window_handle, &mouse);
            global_platform.mouse_x = (f32)(mouse.x);
            global_platform.mouse_y = (f32)(mouse.y);
            Win32UpdateXInput(&global_platform);
            global_platform.target_text = 0;
            global_platform.pump_events = 0;
        }
        
        // NOTE(rjf): Find how much sound to write and where
        if(win32_sound_output.initialized)
        {
            global_platform.sample_count_to_output = 0;
            UINT32 sound_padding_size;
            if(SUCCEEDED(win32_sound_output.audio_client->lpVtbl->GetCurrentPadding(win32_sound_output.audio_client, &sound_padding_size)))
            {
                global_platform.samples_per_second = win32_sound_output.samples_per_second;
                global_platform.sample_count_to_output = (u32)(win32_sound_output.latency_frame_count - sound_padding_size);
                if(global_platform.sample_count_to_output > win32_sound_output.latency_frame_count)
                {
                    global_platform.sample_count_to_output = win32_sound_output.latency_frame_count;
                }
            }
            
            for(u32 i = 0; i < win32_sound_output.buffer_frame_count; ++i)
            {
                global_platform.sample_out[i] = 0;
            }
        }
        
        // NOTE(rjf): Call into the app layer to update
        {
            b32 last_fullscreen = global_platform.fullscreen;
            
            MemoryArenaClear(&global_platform.scratch_arena);
            win32_app_code.Update();
            
            // NOTE(rjf): Update fullscreen if necessary
            if(last_fullscreen != global_platform.fullscreen)
            {
                Win32ToggleFullscreen(window_handle);
            }
            
            // NOTE(rjf): Fill sound buffer with game sound
            if(win32_sound_output.initialized)
            {
                Win32FillSoundBuffer(global_platform.sample_count_to_output, global_platform.sample_out, &win32_sound_output);
            }
        }
        
        // NOTE(rjf): Post-update platform data update
        {
            Win32UpdateWorkQueue();
            TsPlatformEndFrame();
        }
        
        Win32TimerEndFrame(&global_win32_timer, 1000.0 * (1.0 / (f64)global_platform.target_frames_per_second));
    }
    
    ShowWindow(window_handle, SW_HIDE);
    
    for(u32 i = 0; i < global_win32_worker_thread_max; ++i)
    {
        ReleaseSemaphore(global_win32_worker_threads[i].job_semaphore, 1, 0);
        WaitForSingleObject(global_win32_worker_threads[i].job_semaphore, INFINITE);
    }
    
    Win32AppCodeUnload(&win32_app_code);
    
    quit:;
    
    return 0;
}
