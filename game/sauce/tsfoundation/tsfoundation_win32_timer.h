/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

typedef struct Win32Timer
{
    LARGE_INTEGER counts_per_second;
    LARGE_INTEGER begin_frame;
    b32 sleep_is_granular;
}
Win32Timer;

internal b32 Win32TimerInit(Win32Timer *timer);
internal void Win32TimerBeginFrame(Win32Timer *timer);
internal void Win32TimerEndFrame(Win32Timer *timer, f64 milliseconds_per_frame);