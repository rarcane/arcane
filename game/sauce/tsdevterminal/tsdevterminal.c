/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifdef TS2D_H_INCLUDED

internal void
TsDevTerminalTs2dRenderFilledRect(v4 color, v4 rect)
{
    Ts2dPushFilledRect(platform->ts2d, color, rect);
}

internal f32
TsDevTerminalTs2dRenderTextN(v4 color, v2 position, f32 scale, char *text, int n)
{
    Ts2dFont *font = Ts2dGetDefaultFont(platform->ts2d);
    if(font)
    {
        return Ts2dPushTextN(platform->ts2d, font, 0, color, position, scale, text, n);
    }
    return 0.f;
}

internal f32
TsDevTerminalTs2dRenderText(v4 color, v2 position, f32 scale, char *text)
{
    return TsDevTerminalTs2dRenderTextN(color, position, scale, text, (int)(u32)-1);
}

#endif

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandHelp)
{
    for(int i = 0; i < terminal->command_count; ++i)
    {
        if(terminal->commands[i].display_in_list)
        {
            char *name = terminal->commands[i].name;
            char *params = terminal->commands[i].parameter_description;
            char *description = terminal->commands[i].description;
            char space[] = "                                ";
            int spaces_to_print = 32 - (CalculateCStringLength(name) + CalculateCStringLength(params) + 4);
            if(spaces_to_print < 0)
            {
                spaces_to_print = 0;
            }
            TsDevTerminalLog(terminal, 0, " * %s %s%.*s: %s", name, params, spaces_to_print, space, description);
        }
    }
    return 0;
}

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandClose)
{
    terminal->open = 0;
    return 0;
}

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandClear)
{
    terminal->command_history_count = 0;
    terminal->first_text_chunk = terminal->last_text_chunk = 0;
    MemoryPoolClear(&terminal->log_chunk_pool);
    return 0;
}

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandWarnings)
{
    if(word_count > 0)
    {
        if(CStringMatchCaseInsensitive(words[0], "on"))
        {
            terminal->log_warnings = 1;
            TsDevTerminalLog(terminal, 0, "Warning logging enabled.");
        }
        else if(CStringMatchCaseInsensitive(words[0], "off"))
        {
            terminal->log_warnings = 0;
            TsDevTerminalLog(terminal, 0, "Warning logging disabled.");
        }
        else
        {
            TsDevTerminalLog(terminal, 0, "'%s' is not a valid value. Use 'on' or 'off'.", words[0]);
        }
    }
    return 0;
}

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandErrors)
{
    if(word_count > 0)
    {
        if(CStringMatchCaseInsensitive(words[0], "on"))
        {
            terminal->log_errors = 1;
            TsDevTerminalLog(terminal, 0, "Error logging enabled.");
        }
        else if(CStringMatchCaseInsensitive(words[0], "off"))
        {
            terminal->log_errors = 0;
            TsDevTerminalLog(terminal, 0, "Error logging disabled.");
        }
        else
        {
            TsDevTerminalLog(terminal, 0, "'%s' is not a valid value. Use 'on' or 'off'.", words[0]);
        }
    }
    return 0;
}

TSDEVTERMINAL_COMMAND_PROC(TsDevTerminalCommandSet)
{
    if(word_count >= 2)
    {
        char *var = words[0];
        char *val = words[1];
        
        for(int i = 0; i < terminal->variable_count; ++i)
        {
            if(CStringMatchCaseInsensitive(terminal->variables[i].name, var))
            {
                i32 type = terminal->variables[i].type;
                
                switch(type)
                {
                    case TSDEVTERMINAL_VARIABLE_TYPE_b32:
                    {
                        *(b32 *)terminal->variables[i].address =
                            CStringMatchCaseInsensitive(val, "true") ||
                            CStringMatchCaseInsensitive(val, "1")
                            ? 1
                            : 0;
                        break;
                    }
                    
                    case TSDEVTERMINAL_VARIABLE_TYPE_f32:
                    {
                        *(f32 *)terminal->variables[i].address = GetFirstF32FromCString(val);
                        break;
                    }
                    
                    case TSDEVTERMINAL_VARIABLE_TYPE_v2:
                    {
                        if(word_count > 2)
                        {
                            *(v2 *)terminal->variables[i].address = v2(GetFirstF32FromCString(val), GetFirstF32FromCString(words[2]));
                        }
                        break;
                    }
                    
                    default: break;
                }
                
                TsDevTerminalLog(terminal, 0, "%s set to %s.", var, val);
                
                break;
            }
            else if(i == terminal->variable_count - 1)
            {
                TsDevTerminalLog(terminal, 0, "'%s' is not a developer variable.", var);
            }
        }
    }
    return 0;
}

void
TsDevTerminalInit(TsDevTerminal *terminal, TsDevTerminalInitInfo *info, MemoryArena *arena)
{
    terminal->current_command_input[0] = 0;
    terminal->open = 0;
    terminal->open_transition = 0;
    terminal->command_history_count = 0;
    terminal->old_command_index = -1;
    terminal->caret_x_pos = 0;
    terminal->scrollback_scroll_y = 0.f;
    terminal->scrollback_scroll_y_target = 0.f;
    terminal->log_chunk_pool = MemoryPoolInit(terminal->log_chunk_memory, sizeof(terminal->log_chunk_memory), sizeof(TsDevTerminalTextChunk));
    terminal->first_text_chunk = terminal->last_text_chunk = 0;
    terminal->log_errors = 1;
    terminal->log_warnings = 1;
    
    TsDevTerminalCommand default_commands[] =
    {
        { "help",        "",                   "Displays a list of commands.",               1, TsDevTerminalCommandHelp,     },
        { "close",       "",                   "Closes the developer terminal.",             1, TsDevTerminalCommandClose,    },
        { "clear",       "",                   "Clears the developer terminal.",             1, TsDevTerminalCommandClear,    },
        { "cls",         "",                   "",                                           0, TsDevTerminalCommandClear,    },
        { "warnings",    "<on or off>",        "Enables or disables warning logging.",       1, TsDevTerminalCommandWarnings, },
        { "errors",      "<on or off>",        "Enables or disables error logging.",         1, TsDevTerminalCommandErrors,   },
        { "set",         "<variable> <value>", "Sets the value of a developer variable.",    1, TsDevTerminalCommandSet,      },
    };
    
    terminal->command_count = info->command_count + ArrayCount(default_commands);
    terminal->commands = MemoryArenaAllocate(arena, sizeof(TsDevTerminalCommand) * terminal->command_count);
    HardAssert(terminal->commands != 0);
    MemoryCopy(terminal->commands, default_commands, sizeof(TsDevTerminalCommand) * ArrayCount(default_commands));
    MemoryCopy(terminal->commands + ArrayCount(default_commands), info->commands, sizeof(TsDevTerminalCommand) * info->command_count);
    
    terminal->variable_count = info->variable_count;
    terminal->variables = MemoryArenaAllocate(arena, sizeof(TsDevTerminalVariable) * terminal->variable_count);
    HardAssert(terminal->variables != 0);
    MemoryCopy(terminal->variables, info->variables, info->variable_count * sizeof(TsDevTerminalVariable));
    
    terminal->frame_arena = info->frame_arena;
    
    terminal->RenderFilledRect = info->RenderFilledRect;
    terminal->RenderText = info->RenderText;
    terminal->RenderTextN = info->RenderTextN;
    
    if(!terminal->RenderFilledRect)
    {
#ifdef TS2D_H_INCLUDED
        terminal->RenderFilledRect = TsDevTerminalTs2dRenderFilledRect;
#endif
    }
    
    if(!terminal->RenderText)
    {
#ifdef TS2D_H_INCLUDED
        terminal->RenderText = TsDevTerminalTs2dRenderText;
#endif
    }
    
    if(!terminal->RenderTextN)
    {
#ifdef TS2D_H_INCLUDED
        terminal->RenderTextN = TsDevTerminalTs2dRenderTextN;
#endif
    }
    
}

v4
TsDevTerminalGetRect(TsDevTerminal *terminal)
{
    f32 transition = terminal->open_transition;
    f32 terminal_height = platform->window_height * 0.85f;
    
    v4 overlay_rect =
    {
        0,
        -terminal_height + terminal_height * transition,
        (f32)platform->window_width,
        terminal_height
    };
    
    return overlay_rect;
}

char *
TsDevTerminalLog(TsDevTerminal *terminal, i32 flags, char *format, ...)
{
    char *string = 0;
    
    if((!(flags & LOG_ERROR) || terminal->log_errors) &&
       (!(flags & LOG_WARNING) || terminal->log_warnings))
    {
        
        va_list args;
        va_start(args, format);
        u32 needed_bytes = vsnprintf(0, 0, format, args)+1;
        va_end(args);
        
        if(needed_bytes > TSDEVTERMINAL_MAX_LOG_LENGTH)
        {
            needed_bytes = TSDEVTERMINAL_MAX_LOG_LENGTH;
        }
        
        // NOTE(rjf): Try to grab the last chunk for allocation.
        TsDevTerminalTextChunk *chunk = terminal->last_text_chunk;
        if(chunk == 0)
        {
            chunk = MemoryPoolAllocate(&terminal->log_chunk_pool);
            terminal->first_text_chunk = chunk;
            terminal->last_text_chunk = chunk;
            chunk->previous = 0;
            chunk->next = 0;
            chunk->text_arena = MemoryArenaInitAligned(chunk->text_arena_memory, sizeof(chunk->text_arena_memory), 1);
            chunk->text_arena_memory[0] = 0;
            chunk->string_count = 0;
        }
        
        // NOTE(rjf): We need to allocate a new chunk if there isn't enough memory in the last chunk.
        if(chunk->text_arena.memory_left < needed_bytes + sizeof(TsDevTerminalTextHeader))
        {
            chunk = MemoryPoolAllocate(&terminal->log_chunk_pool);
            
            // NOTE(rjf): If we cannot successfully allocate a new log chunk, we need to free
            // the first log chunk to make room.
            if(chunk == 0)
            {
                TsDevTerminalTextChunk *first_chunk = terminal->first_text_chunk;
                terminal->first_text_chunk = first_chunk->next;
                MemoryPoolFree(&terminal->log_chunk_pool, first_chunk);
                chunk = MemoryPoolAllocate(&terminal->log_chunk_pool);
            }
            
            HardAssert(chunk != 0);
            
            chunk->text_arena = MemoryArenaInitAligned(chunk->text_arena_memory, sizeof(chunk->text_arena_memory), 1);
            chunk->text_arena_memory[0] = 0;
            chunk->next = 0;
            chunk->previous = terminal->last_text_chunk;
            chunk->string_count = 0;
            terminal->last_text_chunk->next = chunk;
            terminal->last_text_chunk = chunk;
        }
        
        HardAssert(chunk != 0);
        
        if(chunk)
        {
            string = MemoryArenaAllocate(&chunk->text_arena, needed_bytes + sizeof(TsDevTerminalTextHeader));
            TsDevTerminalTextHeader *header = (TsDevTerminalTextHeader *)string;
            header->flags = flags;
            string += sizeof(TsDevTerminalTextHeader);
            HardAssert(string != 0);
            va_start(args, format);
            vsnprintf(string, needed_bytes, format, args);
            va_end(args);
            ++chunk->string_count;
            
            if((flags & LOG_WARNING) || (flags & LOG_ERROR))
            {
                ++terminal->new_errors_or_warnings;
            }
        }
    }
    
    return string;
}

void
TsDevTerminalPushCommandToHistory(TsDevTerminal *terminal, char *command)
{
    if(terminal->command_history_count >= ArrayCount(terminal->command_history))
    {
        MemoryMove(terminal->command_history, terminal->command_history + 1, (terminal->command_history_count - 1) * sizeof(char *));
        --terminal->command_history_count;
    }
    u32 bytes_to_copy = CalculateCStringLength(command);
    if(bytes_to_copy > 256)
    {
        bytes_to_copy = 256;
    }
    MemoryCopy(terminal->command_history[terminal->command_history_count], command, bytes_to_copy);
    ++terminal->command_history_count;
}

void
TsDevTerminalUpdate(TsDevTerminal *terminal, f32 delta_t)
{
    terminal->delta_t = delta_t;
    
    v4 overlay_rect = TsDevTerminalGetRect(terminal);
    
    if(!platform->keyboard_captured)
    {
        if(terminal->open)
        {
            if(platform->key_pressed[KEY_esc])
            {
                terminal->open = 0;
                TsPlatformCaptureKeyboard();
            }
        }
        else
        {
            if(platform->key_pressed[KEY_grave_accent])
            {
                terminal->open = 1;
                TsPlatformCaptureKeyboard();
            }
        }
    }
    
    b32 command_submitted = 0;
    b32 moved_down_this_frame = 0;
    
    if(terminal->open)
    {
        terminal->new_errors_or_warnings = 0;
        
        // NOTE(rjf): Keyboard input
        {
            platform->target_text = terminal->current_command_input;
            platform->target_text_max_characters = sizeof(terminal->current_command_input);
            platform->target_text_edit_pos = CalculateCStringLength(terminal->current_command_input);
            
            if(terminal->old_command_index < 0)
            {
                if(platform->key_pressed[KEY_up])
                {
                    terminal->old_command_index = (i32)terminal->command_history_count - 1;
                    MemoryCopy(terminal->current_command_input_backup, terminal->current_command_input, sizeof(terminal->current_command_input));
                    MemoryCopy(terminal->current_command_input, terminal->command_history[terminal->old_command_index], sizeof(terminal->current_command_input));
                }
            }
            else
            {
                if(platform->key_pressed[KEY_up])
                {
                    if(--terminal->old_command_index <= 0)
                    {
                        terminal->old_command_index = 0;
                    }
                    MemoryCopy(terminal->current_command_input, terminal->command_history[terminal->old_command_index], sizeof(terminal->current_command_input));
                }
                if(platform->key_pressed[KEY_down])
                {
                    ++terminal->old_command_index;
                    if(terminal->old_command_index >= (i32)terminal->command_history_count)
                    {
                        terminal->old_command_index = -1;
                        MemoryCopy(terminal->current_command_input, terminal->current_command_input_backup, sizeof(terminal->current_command_input));
                    }
                    else
                    {
                        MemoryCopy(terminal->current_command_input, terminal->command_history[terminal->old_command_index], sizeof(terminal->current_command_input));
                    }
                }
            }
            
            if(platform->key_pressed[KEY_enter])
            {
                command_submitted = 1;
            }
            
            if(platform->last_key == KEY_page_up)
            {
                terminal->scrollback_scroll_y_target -= 512;
                terminal->stuck_to_bottom = 0;
            }
            else if(platform->last_key == KEY_page_down)
            {
                terminal->scrollback_scroll_y_target += 512;
                terminal->stuck_to_bottom = 0;
                moved_down_this_frame = 1;
            }
            
            TsPlatformCaptureKeyboard();
        }
        
        // NOTE(rjf): Mouse input
        {
            if(V4HasPoint(overlay_rect, v2(platform->mouse_x, platform->mouse_y)))
            {
                terminal->scrollback_scroll_y_target -= platform->mouse_scroll_y;
                if(AbsoluteValue(platform->mouse_scroll_y) > 0.01f)
                {
                    terminal->stuck_to_bottom = 0;
                    if(platform->mouse_scroll_y < 0)
                    {
                        moved_down_this_frame = 1;
                    }
                }
                
                TsPlatformCaptureMousePosition();
                TsPlatformCaptureMouseButtons();
            }
        }
    }
    else
    {
        terminal->stuck_to_bottom = 1;
    }
    
    terminal->scrollback_scroll_y += (terminal->scrollback_scroll_y_target - terminal->scrollback_scroll_y) * terminal->delta_t * 16.f;
    if(AbsoluteValue(terminal->scrollback_scroll_y_target - terminal->scrollback_scroll_y) >= 0.2f)
    {
        platform->pump_events = 1;
    }
    
    if(terminal->scrollback_scroll_y_target < 0)
    {
        terminal->scrollback_scroll_y_target = 0;
    }
    if(terminal->scrollback_scroll_y_target >= terminal->scrollback_scroll_y_upper_bound - overlay_rect.height + 50)
    {
        terminal->scrollback_scroll_y_target = terminal->scrollback_scroll_y_upper_bound - overlay_rect.height + 50;
        if(moved_down_this_frame)
        {
            terminal->stuck_to_bottom = 1;
        }
    }
    
    terminal->open_transition += ((f32)(!!terminal->open) - terminal->open_transition) * terminal->delta_t * 16.f;
    if(AbsoluteValue((f32)(!!terminal->open) - terminal->open_transition) >= 0.005f)
    {
        platform->pump_events = 1;
    }
    
    if(terminal->stuck_to_bottom)
    {
        terminal->scrollback_scroll_y_target = terminal->scrollback_scroll_y_upper_bound - overlay_rect.height + 50;
    }
    
    if(command_submitted)
    {
        terminal->stuck_to_bottom = 1;
        char *command = terminal->current_command_input;
        
        // NOTE(rjf): Push command to log.
        {
            TsDevTerminalLog(terminal, 0, "%s", command);
        }
        
        // NOTE(rjf): Push command to command history.
        {
            TsDevTerminalPushCommandToHistory(terminal, command);
        }
        
        // NOTE(rjf): Parse command.
        {
            char parse_command[ArrayCount(terminal->current_command_input)] = {0};
            MemoryCopy(parse_command, terminal->current_command_input, sizeof(terminal->current_command_input));
            u32 parse_command_length = CalculateCStringLength(parse_command);
            u32 word_count = 0;
            char **words = MemoryArenaAllocate(terminal->frame_arena, sizeof(char *));
            b32 word_started = 0;
            HardAssert(words != 0);
            for(u32 i = 0; i < parse_command_length; ++i)
            {
                if(word_started)
                {
                    if(CharIsSpace(parse_command[i]))
                    {
                        word_started = 0;
                        parse_command[i] = 0;
                    }
                }
                else
                {
                    if(!CharIsSpace(parse_command[i]))
                    {
                        word_started = 1;
                        HardAssert(MemoryArenaAllocate(terminal->frame_arena, sizeof(char *)));
                        words[word_count++] = parse_command + i;
                    }
                }
            }
            
            // NOTE(rjf): Linear search through each command, because we probably won't have
            // enough commands to ever care.
            if(word_count)
            {
                for(int i = 0; i < terminal->command_count; ++i)
                {
                    if(CStringMatchCaseInsensitive(terminal->commands[i].name, words[0]))
                    {
                        terminal->commands[i].Procedure(terminal, word_count-1, words+1);
                        break;
                    }
                    else if(i == terminal->command_count - 1)
                    {
                        TsDevTerminalLog(terminal, 0, "'%s' is not a valid command. Type 'help'.", words[0]);
                    }
                }
            }
        }
        
        terminal->current_command_input[0] = 0;
        terminal->current_command_input_backup[0] = 0;
        terminal->old_command_index = -1;
    }
}

void
TsDevTerminalRender(TsDevTerminal *terminal)
{
    v4 overlay_rect = TsDevTerminalGetRect(terminal);
    
    if(overlay_rect.y + overlay_rect.height > 0)
    {
        f32 text_scale = 0.3f;
        
        terminal->RenderFilledRect(v4(0, 0, 0, 0.8f), overlay_rect);
        
        // NOTE(rjf): Render log.
        {
            v2 text_position =
            {
                overlay_rect.x + 12,
                overlay_rect.y + 12 - terminal->scrollback_scroll_y,
            };
            
            for(TsDevTerminalTextChunk *chunk = terminal->first_text_chunk;
                chunk; chunk = chunk->next)
            {
                char *text = chunk->text_arena_memory;
                for(u32 i = 0; i < chunk->string_count; ++i)
                {
                    TsDevTerminalTextHeader *header = (TsDevTerminalTextHeader *)text;
                    text += sizeof(*header);
                    
                    char *category_tag = 0;
                    int category_tag_length = 0;
                    if(text[0] == '[')
                    {
                        category_tag = text + 0;
                        for(category_tag_length = 0; category_tag[category_tag_length] && category_tag[category_tag_length] != ']'; ++category_tag_length);
                        ++category_tag_length;
                    }
                    
                    v4 color = { 0.8f, 0.8f, 0.8f, 0.8f };
                    if(header->flags & LOG_ERROR)
                    {
                        color = (v4){ 1.f, 0.4f, 0.4f, 1.f };
                    }
                    else if(header->flags & LOG_WARNING)
                    {
                        color = (v4){ 1.f, 0.9f, 0.6f, 1.f };
                    }
                    
                    int text_length = 0;
                    for(; text[text_length]; ++text_length);
                    if(text_position.y >= overlay_rect.y - 80 && text_position.y < overlay_rect.y + overlay_rect.height - 20)
                    {
                        f32 text_x_offset = 0.f;
                        if(category_tag)
                        {
                            local_persist v4 category_colors[] =
                            {
                                { 1.f, 0.2f, 0.2f, 1.f, },
                                { 0.2f, 1.f, 0.2f, 1.f, },
                                { 0.2f, 0.2f, 1.f, 1.f, },
                                { 0.2f, 0.7f, 1.f, 1.f, },
                                { 1.f, 0.75f, 0.2f, 1.f, },
                                { 0.2f, 0.6f, 0.4f, 1.f, },
                                { 0.2f, 0.6f, 0.9f, 1.f, },
                                { 0.9f, 0.6f, 0.3f, 1.f, },
                                { 0.9f, 0.6f, 0.4f, 1.f, },
                            };
                            u32 tag_hash = CStringCRC32N(category_tag, category_tag_length);
                            v4 category_color = category_colors[tag_hash % ArrayCount(category_colors)];
                            text_x_offset += terminal->RenderTextN(category_color, text_position, text_scale, category_tag, category_tag_length);
                        }
                        text_position.x += text_x_offset;
                        
                        terminal->RenderText(color, text_position, text_scale,
                                             category_tag ? text + category_tag_length : text);
                        text_position.x -= text_x_offset;
                    }
                    text += text_length + 1;
                    text_position.y += 24;
                }
            }
            
            terminal->scrollback_scroll_y_upper_bound = text_position.y - (overlay_rect.y + 12 - terminal->scrollback_scroll_y);
        }
        
        // NOTE(rjf): Render variables.
        {
            v4 variable_section_rect =
            {
                overlay_rect.x + 3 * overlay_rect.width / 4,
                overlay_rect.y,
                overlay_rect.width/4,
                overlay_rect.height,
            };
            
            terminal->RenderFilledRect(v4(0.1f, 0.1f, 0.1f, 0.7f), variable_section_rect);
            v2 variable_text_position =
            {
                variable_section_rect.x + 16,
                variable_section_rect.y + 16,
            };
            for(int i = 0; i < terminal->variable_count; ++i)
            {
                terminal->RenderText(v4(1, 1, 1, 1), variable_text_position, text_scale, terminal->variables[i].name);
                
                char value_str[64] = "";
                v4 value_color = { 0.6f, 0.6f, 0.6f, 0.6f };
                switch(terminal->variables[i].type)
                {
                    case TSDEVTERMINAL_VARIABLE_TYPE_b32:
                    {
                        b32 bool_val = *(b32 *)terminal->variables[i].address;
                        if(bool_val)
                        {
                            value_color = v4(0.4f, 0.9f, 0.3f, 0.9f);
                        }
                        else
                        {
                            value_color = v4(0.9f, 0.4f, 0.3f, 0.9f);
                        }
                        snprintf(value_str, sizeof(value_str), "%s", bool_val ? "true" : "false");
                        break;
                    }
                    case TSDEVTERMINAL_VARIABLE_TYPE_f32:
                    {
                        snprintf(value_str, sizeof(value_str), "%f", *(f32 *)terminal->variables[i].address);
                        value_color = v4(0.9f, 0.8f, 0.4f, 0.9f);
                        break;
                    }
                    case TSDEVTERMINAL_VARIABLE_TYPE_v2:
                    {
                        snprintf(value_str, sizeof(value_str), "(%f, %f)", ((v2 *)terminal->variables[i].address)->x, ((v2 *)terminal->variables[i].address)->y);
                        value_color = v4(0.9f, 0.8f, 0.4f, 0.9f);
                        break;
                    }
                    default: break;
                }
                
                terminal->RenderText(value_color, v2(variable_text_position.x + 250, variable_text_position.y), text_scale, value_str);
                
                variable_text_position.y += 24;
            }
        }
        
        // NOTE(rjf): Render command input.
        {
            v2 command_input_pos =
            {
                overlay_rect.x + 12,
                overlay_rect.y + overlay_rect.height - 24,
            };
            
            terminal->RenderFilledRect(v4(0.1f, 0.1f, 0.1f, 1.f),
                                       v4(overlay_rect.x,
                                          command_input_pos.y - 8,
                                          overlay_rect.width,
                                          overlay_rect.y + overlay_rect.height - (command_input_pos.y - 8)));
            
            f32 target_caret_x_pos = command_input_pos.x +
                terminal->RenderText(v4(1.f, 1.f, 1.f, 1.f), command_input_pos, text_scale, terminal->current_command_input);
            
            terminal->caret_x_pos += (target_caret_x_pos - terminal->caret_x_pos) * terminal->delta_t * 32.f;
            if(AbsoluteValue(target_caret_x_pos - terminal->caret_x_pos) >= 0.2f)
            {
                platform->pump_events = 1;
            }
            
            terminal->RenderText(V4MultiplyF32(v4(1.f, 1.f, 1.f, 1.f), (Sin((f32)platform->current_time) + 1.3f)),
                                 v2(terminal->caret_x_pos, command_input_pos.y),
                                 text_scale,
                                 "|");
        }
    }
    
#if ROLE == ROLE_PROGRAMMER
    if(terminal->new_errors_or_warnings && !terminal->open)
    {
        terminal->RenderText(v4(1, 1, 0, 1), v2(16, (f32)platform->window_height - 28), 0.3f,
                             MakeCStringOnMemoryArena(terminal->frame_arena, "%i new programmer message%s",
                                                      terminal->new_errors_or_warnings,
                                                      terminal->new_errors_or_warnings > 1 ? "s" : ""));
    }
#endif
}