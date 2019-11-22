#include "../ts2d/ts2d.h"
#include "../ts2d/ts2d_opengl_win32.c"

internal void
TsFoundationWin32Init(void)
{
	platform->renderer = MemoryArenaAllocate(&platform->permanent_arena, sizeof(Ts2d));
	Ts2dInitWin32(platform->renderer);
}