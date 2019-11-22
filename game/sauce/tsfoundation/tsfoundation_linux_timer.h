typedef struct LinuxTimer
{
    struct timespec begin_frame;
    b32 sleep_is_granular;
}
LinuxTimer;

internal b32 LinuxTimerInit(LinuxTimer *timer);
internal void LinuxTimerBeginFrame(LinuxTimer *timer);
internal void LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame);