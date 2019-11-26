/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

typedef struct LinuxTimer
{
    struct timespec begin_frame;
    b32 sleep_is_granular;
}
LinuxTimer;

internal b32 LinuxTimerInit(LinuxTimer *timer);
internal void LinuxTimerBeginFrame(LinuxTimer *timer);
internal void LinuxTimerEndFrame(LinuxTimer *timer, f64 milliseconds_per_frame);