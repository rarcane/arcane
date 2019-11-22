
void
_AssertFailure(char *expression, int line, char *file, int crash)
{
    if(crash)
    {
        platform->OutputError("Assertion Failure", "Assertion of %s at %s:%i failed. Trying to crash...",
                              expression, file, line);
        *(int *)0 = 0;
    }
    else
    {
        LogError("[Soft Assertion] Assertion of %s at %s:%i failed.", expression, file, line);
    }
}

void
_DebugLog(i32 flags, char *file, int line, char *format, ...)
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

void
_DebugBreak_TelescopeFoundationInternal_(void)
{
#if _MSC_VER
    __debugbreak();
#endif
}

void _BeginTimer(char *file, int line, char *format, ...)
{
}

void _EndTimer(void)
{
}