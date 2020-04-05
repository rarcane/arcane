#include <X11/Xlib.h>
#include <assert.h>
#include <libgen.h>
#include "tsfoundation.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <tcl.h>

#include "tsfoundation_linux_timer.h"

global TsPlatform global_platform;
typedef enum LinuxCursorStyle LinuxCursorStyle;
enum LinuxCursorStyle
{
	Linux_CURSOR_normal,
	Linux_CURSOR_horizontal_resize,
	Linux_CURSOR_vertical_resize,
};

global LinuxCursorStyle global_cursor_style = 0;
global LinuxTimer global_linux_timer = {0};
global char global_app_so_path[256];
global char global_temp_app_so_path[256];

#include "tsfoundation_linux_threads.c"
#include "tsfoundation_linux_app_code.c"
#include "tsfoundation_linux_utilities.c"
#include "tsfoundation_linux_file_io.c"
#include "tsfoundation_linux_opengl.c"
#include "tsfoundation_linux_timer.c"
#include "tsfoundation_linux_input.c"

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

internal void X11WindowProc();

static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__("rdtsc"
						 : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

char *TsDevTerminalLog(i32 flags, char *format, ...)
{
	return 0;
}

internal void setup_absolute_paths()
{
	// TODO: This is unreliable. Find a better way
	char path[4096];
	int ret = readlink("/proc/self/exe", path, sizeof(path) - 1);

	if (ret == -1)
	{
		fprintf(stderr, "Failure");
		exit(1);
	}
	path[ret] = 0;
	printf("Path: %s\n", path);

	char folder_path[4096];
	strcpy(folder_path, path);
	strcat(folder_path, "/../");

	char cwd[4096];
	getcwd(cwd, sizeof(cwd));

	// TODO: Take another look at this to ensure accuracy
	global_platform.executable_absolute_path = path;
	global_platform.executable_folder_absolute_path = folder_path;
	global_platform.working_directory_path = cwd;
}

internal f32
LinuxGetTime(void)
{
	struct timespec spec = {0};
	return clock_gettime(CLOCK_REALTIME, &spec);
}

internal u64
LinuxGetCycles()
{
	return rdtsc();
}

internal void
LinuxResetCursor()
{
	global_cursor_style = Linux_CURSOR_normal;
}

internal void
LinuxSetCursorToHorizontalResize()
{
	global_cursor_style = Linux_CURSOR_horizontal_resize;
}

internal void
LinuxSetCursorToVerticalResize()
{
	global_cursor_style = Linux_CURSOR_vertical_resize;
}

int main(int argc, char *argv[])
{

	global_platform.quit = 0;

	// Get Absolute Paths
	setup_absolute_paths();

	sprintf(global_app_so_path, "./%s.so\n", TS_APP_FILENAME);
	printf(global_app_so_path);
	printf("\n");
	// swprintf(global_app_so_path, sizeof(global_executable_directory) / sizeof(*global_executable_directory), L"%s%s.so", TS_APP_FILENAME);
	// swprintf(global_temp_app_so_path, sizeof(global_executable_directory) / sizeof(*global_executable_directory), L"%stemp_%s.so", TS_APP_FILENAME);

	u32 permanent_storage_size = TS_APP_PERMANENT_STORAGE_SIZE;
	u32 scratch_storage_size = TS_APP_SCRATCH_STORAGE_SIZE;

	void *permanent_storage = malloc(permanent_storage_size);
	global_platform.permanent_arena = MemoryArenaInit(permanent_storage, permanent_storage_size);

	void *scratch_storage = malloc(scratch_storage_size);
	global_platform.scratch_arena = MemoryArenaInit(scratch_storage, scratch_storage_size);

	global_platform.vsync = 1;
	global_platform.fullscreen = 0;
	global_platform.window_width = TS_APP_DEFAULT_WINDOW_WIDTH;
	global_platform.window_height = TS_APP_DEFAULT_WINDOW_HEIGHT;

	global_platform.current_time = 0.f;
	// Bring in xrandr to pull refresh rate of monitor with X
	global_platform.target_frames_per_second = 60.0f;

	global_platform.gamepads = global_platform.gamepad_states_1;

	// TODO: Just stubbing values for audio right now
	global_platform.sample_out = malloc(48000 * sizeof(f32) * 2);
	global_platform.samples_per_second = 48000;

	global_display = XOpenDisplay(0);

	LinuxInitOpenGL();
	LinuxTimerInit(&global_linux_timer);

	global_platform.OutputError = LinuxOutputError;
	global_platform.HeapAlloc = LinuxHeapAlloc;
	global_platform.HeapFree = LinuxHeapFree;
	global_platform.SaveToFile = LinuxSaveToFile;
	global_platform.AppendToFile = LinuxAppendToFile;
	global_platform.LoadEntireFile = LinuxLoadEntireFile;
	global_platform.LoadEntireFileAndNullTerminate = LinuxLoadEntireFileAndNullTerminate;
	global_platform.FreeFileMemory = LinuxFreeFileMemory;
	global_platform.DeleteFile = LinuxDeleteFile;
	global_platform.MakeDirectory = LinuxMakeDirectory;
	global_platform.DoesFileExist = LinuxDoesFileExist;
	global_platform.DoesDirectoryExist = LinuxDoesDirectoryExist;
	global_platform.CopyFile = LinuxCopyFile;
	// global_platform.CopyDirectoryRecursively = Win32CopyDirectoryRecursively;
	global_platform.PlatformDirectoryListLoad = LinuxPlatformDirectoryListLoad;
	// global_platform.PlatformDirectoryListCleanUp = Win32PlatformDirectoryListCleanUp;
	global_platform.QueueJob = LinuxQueueJob;
	global_platform.WaitForJob = LinuxWaitForJob;
	global_platform.GetTime = LinuxGetTime;
	global_platform.GetCycles = LinuxGetCycles;
	global_platform.ResetCursor = LinuxResetCursor;
	global_platform.SetCursorToHorizontalResize = LinuxSetCursorToHorizontalResize;
	global_platform.SetCursorToVerticalResize = LinuxSetCursorToVerticalResize;
	global_platform.LoadOpenGLProcedure = LinuxLoadOpenGLProcedure;
	global_platform.RefreshScreen = LinuxOpenGLRefreshScreen;

	platform = &global_platform;

	LinuxAppCode linux_app_code = {0};
	LinuxAppCodeLoad(&linux_app_code);

	linux_app_code.PermanentLoad(&global_platform);

	while (!global_platform.quit)
	{
		LinuxTimerBeginFrame(&global_linux_timer);

		// NOTE(rjf): Update Windows events
		{
			if (XPending(global_display))
			{
				XEvent xev;
				XNextEvent(global_display, &xev);

				if (xev.type == MapNotify)
					printf("X Window Mapped!\n");
			}
		}

		// NOTE(rjf): Update window size
		{
			unsigned int x;
			unsigned int y;
			unsigned int width;
			unsigned int height;
			unsigned int border_width;
			unsigned int depth;
			Window root;

			XGetGeometry(global_display, global_window, &root, &x, &y, &width, &height, &border_width, &depth);
			global_platform.window_width = width;
			global_platform.window_height = height;
		}

		// NOTE(rjf): Update input data (post-event)
		TsPlatformBeginFrame();
		{
			unsigned int x;
			unsigned int y;
			unsigned int root_x;
			unsigned int root_y;
			unsigned int width;
			unsigned int height;
			unsigned int border_width;
			unsigned int depth;
			unsigned int mask;
			Window root;

			XQueryPointer(global_display, global_window, &root, &root, &root_x, &root_y, &x, &y, &mask);
			global_platform.mouse_x = (f32)(x);
			global_platform.mouse_y = (f32)(y);

			LinuxUpdateInput(&global_platform);
			global_platform.target_text = 0;
			global_platform.pump_events = 0;
		}

		// NOTE(rjf): Find how much sound to write and where
		// Port over to linux eventually
		// if (linux_sound_output.initialized)
		// {
		// 	global_platform.sample_count_to_output = 0;
		// 	UINT32 sound_padding_size;
		// 	if (SUCCEEDED(linux_sound_output.audio_client->lpVtbl->GetCurrentPadding(win32_sound_output.audio_client, &sound_padding_size)))
		// 	{
		// 		global_platform.samples_per_second = win32_sound_output.samples_per_second;
		// 		global_platform.sample_count_to_output = (u32)(win32_sound_output.latency_frame_count - sound_padding_size);
		// 		if (global_platform.sample_count_to_output > win32_sound_output.latency_frame_count)
		// 		{
		// 			global_platform.sample_count_to_output = win32_sound_output.latency_frame_count;
		// 		}
		// 	}

		// 	for (u32 i = 0; i < linux_sound_output.buffer_frame_count; ++i)
		// 	{
		// 		global_platform.sample_out[i] = 0;
		// 	}
		// }

		// NOTE(rjf): Call into the app layer to update
		{
			b32 last_fullscreen = global_platform.fullscreen;

			MemoryArenaClear(&global_platform.scratch_arena);
			linux_app_code.Update();

			// NOTE(rjf): Update fullscreen if necessary
			// IMPLEMENT THIS
			// if (last_fullscreen != global_platform.fullscreen)
			// {
			// 	Win32ToggleFullscreen(window_handle);
			// }

			// NOTE(rjf): Fill sound buffer with game sound
			// IMPLEMENT THIS
			// if (win32_sound_output.initialized)
			// {
			// 	Win32FillSoundBuffer(global_platform.sample_count_to_output, global_platform.sample_out, &win32_sound_output);
			// }
		}

		// NOTE(rjf): Post-update platform data update
		{
			LinuxUpdateWorkQueue();
			TsPlatformEndFrame();
		}

		LinuxTimerEndFrame(&global_linux_timer, 1000.0 * (1.0 / (f64)global_platform.target_frames_per_second));
	}
}

internal void X11WindowProc()
{
}
