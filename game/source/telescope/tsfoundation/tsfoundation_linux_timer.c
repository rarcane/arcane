#include <time.h>

#define NANOS 1000000000LL

internal b32
LinuxTimerInit(LinuxTimer *timer)
{
	// TODO: Find a way to actually check if clock frequency is granular enough
	struct timespec t_spec = {0};
	clock_getres(CLOCK_MONOTONIC, &t_spec);
	timer->sleep_is_granular = 1;
	return 1;
}

internal void
LinuxTimerBeginFrame(LinuxTimer *timer)
{
	struct timespec t_spec = {0};
	clock_gettime(CLOCK_MONOTONIC, &t_spec);
	timer->begin_frame = t_spec.tv_sec * NANOS + t_spec.tv_nsec;
	// IMPLEMENT THIS
	// QueryPerformanceCounter(&timer->begin_frame);
}

internal void
LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame)
{
	// IMPLEMENT THIS
	// LARGE_INTEGER end_frame;
	// QueryPerformanceCounter(&end_frame);

	struct timespec t_spec = {0};
	clock_gettime(CLOCK_MONOTONIC, &t_spec);

	long elapsed_milliseconds = ((t_spec.tv_sec * NANOS + t_spec.tv_nsec) - timer->begin_frame) / 1000000;
	long milliseconds_to_sleep = milliseconds_per_frame - elapsed_milliseconds;

	struct timespec start_spec = {0};
	struct timespec end_spec = {0};

	clock_gettime(CLOCK_MONOTONIC, &start_spec);
	long start_t = start_spec.tv_sec * NANOS + start_spec.tv_nsec;

	long end_t = 0;

	while (milliseconds_to_sleep > 0)
	{

		if (timer->sleep_is_granular)
		{
			if (milliseconds_to_sleep > 0)
			{
				// clock_nanosleep(CLOCK_MONOTONIC);
				sleep(milliseconds_to_sleep / 1000);
			}
			else
			{
				break;
			}
		}

		clock_gettime(CLOCK_MONOTONIC, &end_spec);
		end_t = end_spec.tv_sec * NANOS + end_spec.tv_nsec;

		long nanoseconds_to_sleep = milliseconds_to_sleep * 1000000;
		milliseconds_to_sleep = (nanoseconds_to_sleep - (end_t - start_t)) / 1000000;
		start_t = end_t;
	}
}