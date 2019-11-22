#define Log(...) ArcaneDebugLog(0, __FILE__, __LINE__, __VA_ARGS__)
#define LogWarning(...) ArcaneDebugLog(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LogError(...) ArcaneDebugLog(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
internal void ArcaneDebugLog(i32 flags, char *file, int line, char *format, ...);