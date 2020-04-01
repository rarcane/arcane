#include <X11/Xlib.h>
#include <assert.h>
#include <libgen.h>
#include "tsfoundation.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <tcl.h>

#include "tsfoundation_linux_utilities.c"
#include "tsfoundation_linux_file_io.c"
#include "tsfoundation_linux_opengl.c"

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

global TsPlatform global_platform;
typedef enum LinuxCursorStyle LinuxCursorStyle;
enum LinuxCursorStyle
{
	Linux_CURSOR_normal,
	Linux_CURSOR_horizontal_resize,
	Linux_CURSOR_vertical_resize,
};

global LinuxCursorStyle global_cursor_style = 0;
internal void X11WindowProc();

static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__("rdtsc"
						 : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
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

	// TODO: Take another look at this to ensure accuracy
	global_platform.executable_absolute_path = path;
	global_platform.executable_folder_absolute_path = path;
	global_platform.working_directory_path = path;
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

	LinuxInitOpenGL();

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
	// global_platform.QueueJob = Win32QueueJob;
	// global_platform.WaitForJob = Win32WaitForJob;
	global_platform.GetTime = LinuxGetTime;
	global_platform.GetCycles = LinuxGetCycles;
	global_platform.ResetCursor = LinuxResetCursor;
	global_platform.SetCursorToHorizontalResize = LinuxSetCursorToHorizontalResize;
	global_platform.SetCursorToVerticalResize = LinuxSetCursorToVerticalResize;
	global_platform.LoadOpenGLProcedure = LinuxLoadOpenGLProcedure;
	global_platform.RefreshScreen = LinuxOpenGLRefreshScreen;

	global_display = XOpenDisplay(0);

	XEvent xev;
	for (;;)
	{
		XNextEvent(global_display, &xev);

		if (xev.type == MapNotify)
			break;
	}

	platform = &global_platform;
}

internal void X11WindowProc()
{
}
