
internal void *
LinuxHeapAlloc(u32 size)
{
    return malloc(size);
}

internal void
LinuxHeapFree(void *data)
{
    free(data);
}

internal void
LinuxOutputError(char *title, char *format, ...)
{
    local_persist volatile LONG locked = 0;
    
    while(locked);
    InterlockedExchange(&locked, 1);
    {
        va_list args;
        va_start(args, format);
        u32 required_characters = vsnprintf(0, 0, format, args)+1;
        va_end(args);
        
        local_persist char text[4096] = {0};
        
        if(required_characters > 4096)
        {
            required_characters = 4096;
        }
        
        va_start(args, format);
        vsnprintf(text, required_characters, format, args);
        va_end(args);
        
        text[required_characters-1] = 0;
        
        fprintf(stderr, "[%s]: %s\n", text, title);
    }
    InterlockedExchange(&locked, 0);
}

internal void
LinuxToggleFullscreen(void)
{}
