/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

// NOTE(rjf): Linux/Unix/whateverthefuckpeoplewanttocallit
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

// NOTE(rjf): Telescope Headers
#include "tsfoundation.h"
#include "tsfoundation_linux_timer.h"
#include "tsfoundation_linux_threads.h"

// NOTE(rjf): Globals
global char global_executable_path[256];
global char global_executable_directory[256];
global char global_app_dll_path[256];
global char global_temp_app_dll_path[256];
global TsPlatform global_platform;
global Display *global_display;
global Window global_window;
global Window global_root;
global LinuxTimer global_linux_timer = {0};
global u32 global_linux_worker_thread_max = 0;
global LinuxWorkerThreadData global_linux_worker_threads[64] = {0};

// NOTE(rjf): Telescope Implementations
#include "tsfoundation_linux_utilities.c"
#include "tsfoundation_linux_timer.c"
#include "tsfoundation_linux_file_io.c"
#include "tsfoundation_linux_app_code.c"
#include "tsfoundation_linux_threads.c"

#ifndef TSFOUNDATION_LINUX_FILE
#define TSFOUNDATION_LINUX_FILE "tsfoundation_linux_default.c"
#endif
#include TSFOUNDATION_LINUX_FILE

internal void
LinuxHandleXlibEvent(XEvent event)
{
    switch(event.type)
    {
        case ButtonPress:
        {
            if(event.xbutton.button == Button1)
            {
                if(!global_platform.left_mouse_down)
		        {
		            global_platform.left_mouse_pressed = 1;
		        }
		        global_platform.left_mouse_down = 1;
            }
            else if(event.xbutton.button == Button3)
            {
                if(!global_platform.right_mouse_down)
		        {
		            global_platform.right_mouse_pressed = 1;
		        }
		        global_platform.right_mouse_down = 1;
            }
            else if(event.xbutton.button == Button4)
            {
                global_platform.mouse_scroll_y = -1;
            }
            else if(event.xbutton.button == Button5)
            {
                global_platform.mouse_scroll_y = 1;
            }
            break;
        }
        case ButtonRelease:
        {
            if(event.xbutton.button == Button1)
            {
                global_platform.left_mouse_down = 0;
            }
            else if(event.xbutton.button == Button3)
            {
                global_platform.right_mouse_down = 0;
            }
            break;
        }
        case MotionNotify:
        {
            global_platform.mouse_x = (f32)event.xmotion.x;
            global_platform.mouse_y = (f32)event.xmotion.y;
            break;
        }
        case KeyPress:
        case KeyRelease:
        {
            b32 down = event.type == KeyPress;
            
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            
            i32 key_input = -1;
            
            if(keysym >= XK_a && keysym <= XK_z)
            {
                key_input = KEY_a + (keysym-XK_a);
            }
            else if(keysym == XK_space)
            {
                key_input = KEY_space;
            }
            else if(keysym == XK_Shift_L || keysym == XK_Shift_R)
            {
                key_input = KEY_shift;
            }
            else if(keysym == XK_Control_L || keysym == XK_Control_R)
            {
                key_input = KEY_ctrl;
            }
            else if(keysym == XK_Escape)
            {
                key_input = KEY_escape;
            }
            else if(keysym == XK_Return)
            {
                key_input = KEY_enter;
            }
            else if(keysym == XK_Up)
            {
                key_input = KEY_up;
            }
            else if(keysym == XK_Down)
            {
                key_input = KEY_down;
            }
            else if(keysym == XK_Left)
            {
                key_input = KEY_left;
            }
            else if(keysym == XK_Right)
            {
                key_input = KEY_right;
            }

            // TODO(rjf): Translate these keys to Xlib
#if 0
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
            else if(vkey_code == VK_MENU)
            {
                key_input = KEY_alt;
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
#endif
            
            if(key_input >= 0 && key_input < KEY_MAX)
            {
            	b32 was_down = global_platform.key_down[key_input];

                if(down)
                {
                    global_platform.key_pressed[key_input] = (!global_platform.key_down[key_input]);
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
	                
                global_platform.key_down[key_input] = down;

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

            break;
        }
		
		// TODO(rjf): Xlib keyboard input
#if 0
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
#endif

        default: break;
    }
}


internal f32
LinuxGetTime(void)
{
    LinuxTimer *timer = &global_linux_timer;
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    f32 seconds_since_beginning_of_frame = 0.f;
    seconds_since_beginning_of_frame += (f32)(current_time.tv_sec - timer->begin_frame.tv_sec);
    seconds_since_beginning_of_frame += (f32)(current_time.tv_nsec - timer->begin_frame.tv_nsec) / 1000000000.f;
    return global_platform.current_time + seconds_since_beginning_of_frame;
}

internal u64
LinuxGetCycles(void)
{
    u64 result = __rdtsc();
    return result;
}

internal void
LinuxResetCursor(void)
{}

internal void
LinuxSetCursorToHorizontalResize(void)
{}

internal void
LinuxSetCursorToVerticalResize(void)
{}

int
main(int argument_count, char **arguments)
{
    global_instance_handle = instance;
    
    LinuxTimerInit(&global_linux_timer);
    LinuxAppCode linux_game_code = {0};
    // LinuxSoundOutput linux_sound_output = {0};
    
    // TODO(rjf): Calculate executable name and path to DLL
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
    
    // NOTE(rjf): Window initialization
    {
        global_display = XOpenDisplay(0);
        global_root = DefaultRootWindow(global_display);
        if(!global_display)
        {
            LinuxOutputError("Fatal Error", "Could not open Xlib display.");
            goto quit;
        }
	}
    
    // NOTE(rjf): Load application code
    LinuxAppCode linux_app_code = {0};
    {
        if(!LinuxAppCodeLoad(&linux_app_code))
        {
            // NOTE(rjf): ERROR: Application code load failure
            LinuxOutputError("Fatal Error", "Application code load failure.");
            goto quit;
        }
    }
    
    // NOTE(rjf): Sound initialization
#if 0
    {
        win32_sound_output.channels = 2;
        win32_sound_output.samples_per_second = 48000;
        win32_sound_output.latency_frame_count = 48000;
        Win32LoadWASAPI();
        Win32InitWASAPI(&win32_sound_output);
    }
#endif
    
    // NOTE(rjf): Initialize platform
    {
        global_platform.executable_folder_absolute_path = global_executable_directory;
        global_platform.executable_absolute_path = global_executable_path;
        
        u32 permanent_storage_size       = TS_APP_PERMANENT_STORAGE_SIZE;
        void *permanent_storage          = mmap(0, permanent_storage_size, MEM_RESERVE | MEM_COMMIT, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
        global_platform.permanent_arena  = MemoryArenaInit(permanent_storage, permanent_storage_size);
        u32 scratch_storage_size         = TS_APP_SCRATCH_STORAGE_SIZE;
        void *scratch_storage            = mmap(0, scratch_storage_size, MEM_RESERVE | MEM_COMMIT, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
        global_platform.scratch_arena    = MemoryArenaInit(scratch_storage, scratch_storage_size);
        
        global_platform.quit                      = 0;
        global_platform.vsync                     = 1;
        global_platform.fullscreen                = 0;
        global_platform.window_width              = TS_APP_DEFAULT_WINDOW_WIDTH;
        global_platform.window_height             = TS_APP_DEFAULT_WINDOW_HEIGHT;
        global_platform.current_time              = 0.f;
        global_platform.target_frames_per_second  = 60.f;
        
        global_platform.gamepads = global_platform.gamepad_states_1;
        
        global_platform.samples_per_second = 48000;
        global_platform.sample_out = LinuxHeapAlloc(global_platform.samples_per_second * sizeof(f32) * 2);
        
        global_platform.OutputError                    = LinuxOutputError;
        global_platform.HeapAlloc                      = LinuxHeapAlloc;
        global_platform.HeapFree                       = LinuxHeapFree;
        global_platform.SaveToFile                     = LinuxSaveToFile;
        global_platform.AppendToFile                   = LinuxAppendToFile;
        global_platform.LoadEntireFile                 = LinuxLoadEntireFile;
        global_platform.LoadEntireFileAndNullTerminate = LinuxLoadEntireFileAndNullTerminate;
        global_platform.FreeFileMemory                 = LinuxFreeFileMemory;
        global_platform.DeleteFile                     = LinuxDeleteFile;
        global_platform.MakeDirectory                  = LinuxMakeDirectory;
        global_platform.DoesFileExist                  = LinuxDoesFileExist;
        global_platform.PlatformDirectoryListLoad      = LinuxPlatformDirectoryListLoad;
        global_platform.PlatformDirectoryListCleanUp   = LinuxPlatformDirectoryListCleanUp;
        global_platform.QueueJob                       = LinuxQueueJob;
        global_platform.WaitForJob                     = LinuxWaitForJob;
        global_platform.GetTime                        = LinuxGetTime;
        global_platform.GetCycles                      = LinuxGetCycles;
        global_platform.ResetCursor                    = LinuxResetCursor;
        global_platform.SetCursorToHorizontalResize    = LinuxSetCursorToHorizontalResize;
        global_platform.SetCursorToVerticalResize      = LinuxSetCursorToVerticalResize;
        
        global_platform.work_queue_free_index_count = TS_MAX_WORK_QUEUE_JOBS;
        for(i32 i = 0; i < TS_MAX_WORK_QUEUE_JOBS; ++i)
        {
            global_platform.work_queue_free_indices[i] = i;
        }
        
        platform = &global_platform;
    }
    
    // NOTE(rjf): Initialize job system threads
    {
        // SYSTEM_INFO system_info = {0};
        // GetSystemInfo(&system_info);
        // DWORD number_of_logical_processors = system_info.dwNumberOfProcessors;
        u32 number_of_worker_threads = number_of_logical_processors - 2;
        global_linux_worker_thread_max = number_of_worker_threads;
        
        for(u32 i = 0; i < number_of_worker_threads; ++i)
        {
            LinuxWorkerThreadDataInit(global_linux_worker_threads + i);
        }
    }
    
    global_device_context = GetDC(window_handle);
    // Win32LoadXInput();
    
    TsFoundationLinuxInit();
    
    linux_app_code.PermanentLoad(&global_platform);
    linux_app_code.HotLoad(&global_platform);
    
	XMapWindow(global_display, global_window);
    XStoreName(global_display, global_window, TS_APP_WINDOW_TITLE);
    XFlush(global_display);

    while(!global_platform.quit)
    {
        LinuxTimerBeginFrame(&global_linux_timer);
        
        // NOTE(rjf): Handle window events
        {
            XEvent event;
            XLockDisplay(display);

            // NOTE(rjf): Update X11 events
            while(XPending(display) > 0)
            {
                XNextEvent(display, &event);
                LinuxHandleXlibEvent(event);
            }
            XUnlockDisplay(display);
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
        
        // NOTE(rjf): Find how much sound to write and where
#if 0
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
#endif

        // NOTE(rjf): Call into the app layer to update
        {
            b32 last_fullscreen = global_platform.fullscreen;
            MemoryArenaClear(&global_platform.scratch_arena);
            linux_app_code.Update();
            
            // NOTE(rjf): Update fullscreen if necessary
            if(last_fullscreen != global_platform.fullscreen)
            {
                LinuxToggleFullscreen(window_handle);
            }
            
            // NOTE(rjf): Fill sound buffer with game sound
#if 0
            if(win32_sound_output.initialized)
            {
                Win32FillSoundBuffer(global_platform.sample_count_to_output, global_platform.sample_out, &win32_sound_output);
            }
#endif
        }
        
        // NOTE(rjf): Post-update platform data update
        {
            LinuxUpdateWorkQueue();
            TsPlatformEndFrame();
        }
        
        LinuxTimerEndFrame(&global_linux_timer, 1000.0 * (1.0 / (f64)global_platform.target_frames_per_second));
    }
    
    XDestroyWindow(global_display, global_window);
    XCloseDisplay(global_display);
    
    for(u32 i = 0; i < global_linux_worker_thread_max; ++i)
    {
        ReleaseSemaphore(global_linux_worker_threads[i].job_semaphore, 1, 0);
        WaitForSingleObject(global_linux_worker_threads[i].job_semaphore, INFINITE);
    }
    
    LinuxAppCodeUnload(&linux_app_code);
    
    quit:;
    
    return 0;
}
