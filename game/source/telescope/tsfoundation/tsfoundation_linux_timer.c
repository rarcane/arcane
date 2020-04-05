/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

internal b32
LinuxTimerInit(LinuxTimer *timer)
{
	// IMPLEMENT THIS
	// b32 result = 0;

	// if (QueryPerformanceFrequency(&timer->counts_per_second))
	// {
	// 	result = 1;
	// }

	// timer->sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);

	// return result;
}

internal void
LinuxTimerBeginFrame(LinuxTimer *timer)
{
	// IMPLEMENT THIS
	// QueryPerformanceCounter(&timer->begin_frame);
}

internal void
LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame)
{
	// IMPLEMENT THIS
	// LARGE_INTEGER end_frame;
	// QueryPerformanceCounter(&end_frame);

	// f64 desired_seconds_per_frame = (milliseconds_per_frame / 1000.0);
	// i64 elapsed_counts = end_frame.QuadPart - timer->begin_frame.QuadPart;
	// i64 desired_counts = (i64)(desired_seconds_per_frame * timer->counts_per_second.QuadPart);
	// i64 counts_to_wait = desired_counts - elapsed_counts;

	// LARGE_INTEGER start_wait;
	// LARGE_INTEGER end_wait;

	// QueryPerformanceCounter(&start_wait);

	// while (counts_to_wait > 0)
	// {
	// 	if (timer->sleep_is_granular)
	// 	{
	// 		DWORD milliseconds_to_sleep = (DWORD)(1000.0 * ((f64)(counts_to_wait) / timer->counts_per_second.QuadPart));
	// 		if (milliseconds_to_sleep > 0)
	// 		{
	// 			Sleep(milliseconds_to_sleep);
	// 		}
	// 	}

	// 	QueryPerformanceCounter(&end_wait);
	// 	counts_to_wait -= end_wait.QuadPart - start_wait.QuadPart;
	// 	start_wait = end_wait;
	// }
}