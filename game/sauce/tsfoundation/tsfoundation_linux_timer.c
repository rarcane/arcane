internal b32
LinuxTimerInit(LinuxTimer *timer)
{
    b32 result = 1;
    return result;
}

internal void
LinuxTimerBeginFrame(LinuxTimer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->begin_frame);
}

internal void
LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame)
{
    struct timespec end_frame;
    clock_gettime(CLOCK_MONOTONIC, &end_frame);
    
    // NOTE(rjf): End frame, and wait if necessary.
    {
        f64 desired_nanoseconds_per_frame = (milliseconds_per_frame * 1000.0);
        i64 elapsed_seconds = end_frame_time_spec.tv_sec - begin_frame_time_spec.tv_sec;
        i64 elapsed_nanoseconds = end_frame_time_spec.tv_nsec - begin_frame_time_spec.tv_nsec;
        i64 frame_nanoseconds = elapsed_seconds * 1000000000 + elapsed_nanoseconds;
        i64 nanoseconds_to_wait = desired_frame_nanoseconds - frame_nanoseconds;
        i64 last_step = 0;
        struct timespec begin_wait_time_spec;
        struct timespec end_wait_time_spec;
        clock_gettime(CLOCK_MONOTONIC, &begin_wait_time_spec);
        while(nanoseconds_to_wait > last_step)
        {
            clock_gettime(CLOCK_MONOTONIC, &end_wait_time_spec);
            i64 wait_seconds = end_wait_time_spec.tv_sec - begin_wait_time_spec.tv_sec;
            i64 wait_nanoseconds = end_wait_time_spec.tv_nsec - begin_wait_time_spec.tv_nsec;
            last_step = (wait_seconds * 1000000000 + wait_nanoseconds);
            nanoseconds_to_wait -= last_step;
            begin_wait_time_spec = end_wait_time_spec;
        }
    }
}