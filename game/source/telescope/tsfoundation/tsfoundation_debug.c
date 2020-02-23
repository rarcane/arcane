/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

void
_AssertFailure(char *expression, int line, char *file, int crash)
{
    if(crash)
    {
        platform->OutputError("Assertion Failure", "Assertion of %s at %s:%i failed. Trying to crash...",
                              expression, file, line);
        BreakDebugger();
        *(volatile int *)0 = 0;
    }
    else
    {
        LogError("[Soft Assertion] Assertion of %s at %s:%i failed.", expression, file, line);
    }
}

#ifdef TSDEVTERMINAL
char *TsDevTerminalLog(i32 flags, char *format, ...);
#endif

void
_DebugLog(i32 flags, char *file, int line, char *format, ...)
{
    // NOTE(rjf): Log to stdout
    {
        char *name = "Info";
        if(flags & LOG_ERROR)
        {
            name = "Error";
        }
        else if(flags & LOG_WARNING)
        {
            name = "Warning";
        }
        fprintf(stdout, "%s (%s:%i) ", name, file, line);
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fprintf(stdout, "%s", "\n");
    }
    
    // NOTE(rjf): Log to TsDevTerminal, VS output, etc.
    {
        local_persist char string[4096] = {0};
        va_list args;
        va_start(args, format);
        vsnprintf(string, sizeof(string), format, args);
        va_end(args);
#if TSDEVTERMINAL
        TsDevTerminalLog(flags, "%s", string);
#endif
#if BUILD_WIN32
        OutputDebugStringA(string);
        OutputDebugStringA("\n");
#endif
    }
    
}

void
_DebugBreak_TelescopeFoundationInternal_(void)
{
#if _MSC_VER
    __debugbreak();
#else
    *(volatile int *)0 = 0;
#endif
}

void _BeginTimer(char *file, int line, char *format, ...)
{
}

void _EndTimer(void)
{
}
