/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifndef TSDEVTERMINAL_H_INCLUDED
#define TSDEVTERMINAL_H_INCLUDED

typedef struct TsDevTerminal TsDevTerminal;
typedef i32 TsDevTerminalCommandProcedure(TsDevTerminal *terminal, u32 word_count, char **words);
#define TSDEVTERMINAL_COMMAND_PROC(name) i32 name(TsDevTerminal *terminal, u32 word_count, char **words)

typedef struct TsDevTerminalCommand TsDevTerminalCommand;
struct TsDevTerminalCommand
{
    char *name;
    char *parameter_description;
    char *description;
    b32 display_in_list;
    TsDevTerminalCommandProcedure *Procedure;
};

typedef enum TsDevTerminalVariableType TsDevTerminalVariableType;
enum TsDevTerminalVariableType
{
    TSDEVTERMINAL_VARIABLE_TYPE_b32,
    TSDEVTERMINAL_VARIABLE_TYPE_f32,
    TSDEVTERMINAL_VARIABLE_TYPE_v2,
};

typedef struct TsDevTerminalVariable TsDevTerminalVariable;
struct TsDevTerminalVariable
{
    char *name;
    TsDevTerminalVariableType type;
    void *address;
};

typedef struct TsDevTerminalTextHeader TsDevTerminalTextHeader;
struct TsDevTerminalTextHeader
{
    i32 flags;
};

typedef struct TsDevTerminalTextChunk TsDevTerminalTextChunk;
struct TsDevTerminalTextChunk
{
#define TSDEVTERMINAL_MAX_LOG_LENGTH 1024
    char text_arena_memory[(TSDEVTERMINAL_MAX_LOG_LENGTH + sizeof(TsDevTerminalTextHeader)) * 64];
    MemoryArena text_arena;
    u32 string_count;
    TsDevTerminalTextChunk *next;
    TsDevTerminalTextChunk *previous;
};

typedef void TsDevTerminalRenderFilledRect(v4 color, v4 rect);
typedef f32 TsDevTerminalRenderText(v4 color, v2 position, f32 scale, char *text);
typedef f32 TsDevTerminalRenderTextN(v4 color, v2 position, f32 scale, char *text, int n);

typedef struct TsDevTerminal TsDevTerminal;
struct TsDevTerminal
{
    // NOTE(rjf): Command Data
    struct
    {
        char current_command_input[256];
        char current_command_input_backup[256];
        u32 command_history_count;
        char command_history[256][256];
        i32 old_command_index;
    };
    
    // NOTE(rjf): Log Data
    struct
    {
#ifndef TSDEVTERMINAL_MAX_LOG_CHUNKS
#define TSDEVTERMINAL_MAX_LOG_CHUNKS 128
#endif
        char log_chunk_memory[TSDEVTERMINAL_MAX_LOG_CHUNKS * sizeof(TsDevTerminalTextChunk)];
        MemoryPool log_chunk_pool;
        TsDevTerminalTextChunk *first_text_chunk;
        TsDevTerminalTextChunk *last_text_chunk;
    };
    
    // NOTE(rjf): UI Data
    struct
    {
        b32 open;
        f32 open_transition;
        f32 caret_x_pos;
        f32 scrollback_scroll_y;
        f32 scrollback_scroll_y_target;
        f32 scrollback_scroll_y_upper_bound;
        b32 stuck_to_bottom;
        u32 new_errors_or_warnings;
        b32 log_errors;
        b32 log_warnings;
    };
    
    // NOTE(rjf): Variables
    struct
    {
        int variable_count;
        TsDevTerminalVariable *variables;
    };
    
    // NOTE(rjf): Commands
    struct
    {
        int command_count;
        TsDevTerminalCommand *commands;
    };
    
    // NOTE(rjf): Rendering callbacks
    struct
    {
        TsDevTerminalRenderFilledRect *RenderFilledRect;
        TsDevTerminalRenderText *RenderText;
        TsDevTerminalRenderTextN *RenderTextN;
    };
    
    // NOTE(rjf): Frame data
    struct
    {
        f32 delta_t;
        MemoryArena *frame_arena;
    };
};

typedef struct TsDevTerminalInitInfo TsDevTerminalInitInfo;
struct TsDevTerminalInitInfo
{
    MemoryArena *frame_arena;
    int variable_count;
    TsDevTerminalVariable *variables;
    int command_count;
    TsDevTerminalCommand *commands;
    
    // NOTE(rjf): Rendering callbacks.
    TsDevTerminalRenderFilledRect *RenderFilledRect;
    TsDevTerminalRenderText *RenderText;
    TsDevTerminalRenderTextN *RenderTextN;
};

void TsDevTerminalInit(TsDevTerminal *terminal, TsDevTerminalInitInfo *info, MemoryArena *arena);
char *TsDevTerminalLog(TsDevTerminal *terminal, i32 flags, char *format, ...);
void TsDevTerminalPushCommandToHistory(TsDevTerminal *terminal, char *command);
void TsDevTerminalUpdate(TsDevTerminal *terminal, f32 delta_t);
void TsDevTerminalRender(TsDevTerminal *terminal);

#endif // TSDEVTERMINAL_H_INCLUDED