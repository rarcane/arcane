#include "tsfoundation.h"

internal void
LinuxOutputError(char *title, char *format, ...)
{
	// local_persist volatile int32_t locked = 0;

	// while (locked)
	// 	;
	// locked = 1;
	// fprintf(stderr, title);
	// locked = 0;
	fprintf(stderr, title);
	fprintf(stderr, ": ");
	fprintf(stderr, format);
}

internal void *
LinuxHeapAlloc(u32 size)
{
	return malloc(size);
}

internal void
LinuxHeapFree(void *ptr)
{
	free(ptr);
}
