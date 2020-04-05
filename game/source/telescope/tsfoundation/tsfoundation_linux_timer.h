#include "stdint.h"
#include "tsfoundation.h"

typedef struct LinuxTimer
{
	int64_t counts_per_second;
	int64_t begin_frame;
	b32 sleep_is_granular;
} LinuxTimer;

internal b32 LinuxTimerInit(LinuxTimer *timer);
internal void LinuxTimerBeginFrame(LinuxTimer *timer);
internal void LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame);