#undef Assert
#define AssertStatement HardAssert
#define Assert HardAssert
#define HardAssert(b) if(!(b)) { _AssertFailure(#b, __LINE__, __FILE__, 1); }
#define SoftAssert(b) if(!(b)) { _AssertFailure(#b, __LINE__, __FILE__, 0); }

#ifdef TSFOUNDATION_LOG_PROCEDURE_FILE
#include TSFOUNDATION_LOG_PROCEDURE_FILE
#else
#define Log(...)         _DebugLog(0,           __FILE__, __LINE__, __VA_ARGS__)
#define LogWarning(...)  _DebugLog(LOG_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LogError(...)    _DebugLog(LOG_ERROR,   __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifdef TSFOUNDATION_TIMER_PROCEDURE_FILE
#include TSFOUNDATION_TIMER_PROCEDURE_FILE
#else
#define BeginTimer(...)  _BeginTimer(__FILE__, __LINE__, __VA_ARGS__)
#define EndTimer()       _EndTimer()
#endif

#define BreakDebugger _DebugBreak_TelescopeFoundationInternal_

#define LOG_WARNING (1<<0)
#define LOG_ERROR   (1<<1)

void _AssertFailure(char *expression, int line, char *file, int crash);
void _DebugLog(i32 flags, char *file, int line, char *format, ...);
void _DebugBreak_TelescopeFoundationInternal_(void);
void _BeginTimer(char *file, int line, char *format, ...);
void _EndTimer(void);