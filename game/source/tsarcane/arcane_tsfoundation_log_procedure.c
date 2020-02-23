internal void
ArcaneDebugLog(i32 flags, char *file, int line, char *format, ...)
{
    local_persist char string[1024] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(string, sizeof(string), format, args);
    va_end(args);
    TsDevTerminalLog(core->dev_terminal, flags, "%s", string);
    _DebugLog(flags, file, line, string);
    
#if BUILD_WIN32
    OutputDebugStringA(string);
    OutputDebugStringA("\n");
#endif
}