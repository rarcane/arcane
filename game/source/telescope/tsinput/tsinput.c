/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

internal TsInputBinding
TsInputBindingInit(i32 key, i32 mouse, i32 gamepad_button, i32 gamepad_joystick)
{
    TsInputBinding binding = { key, mouse, gamepad_button, gamepad_joystick };
    return binding;
}

void
TsInputResetBindings(void)
{
    MemoryCopy(global_tsinput->bindings, global_tsinput->default_bindings, global_tsinput->control_count * sizeof(TsInputBinding));
}

void
TsInputInit(void)
{
}

void
TsInputSetControls(int control_count, TsInputBinding *bindings, MemoryArena *arena)
{
    global_tsinput->control_count = control_count;
    global_tsinput->controls = MemoryArenaAllocateAndZero(arena, control_count * sizeof(TsInputControl));
    global_tsinput->last_frame_controls = MemoryArenaAllocateAndZero(arena, control_count * sizeof(TsInputControl));
    global_tsinput->bindings = MemoryArenaAllocateAndZero(arena, control_count * sizeof(TsInputBinding));
    global_tsinput->default_bindings = MemoryArenaAllocateAndZero(arena, control_count * sizeof(TsInputBinding));
    HardAssert(global_tsinput->controls != 0);
    HardAssert(global_tsinput->bindings != 0);
    HardAssert(global_tsinput->default_bindings != 0);
    MemoryCopy(global_tsinput->bindings, bindings, sizeof(TsInputBinding)*control_count);
    MemoryCopy(global_tsinput->default_bindings, bindings, sizeof(TsInputBinding)*control_count);
}

void
TsInputSetKeyBinding(int binding_index, int key_binding)
{
    global_tsinput->bindings[binding_index].key_binding = key_binding;
}

void
TsInputSetGamepadButtonBinding(int binding_index, int gamepad_button_binding)
{
    global_tsinput->bindings[binding_index].gamepad_button_binding = gamepad_button_binding;
}

void
TsInputSetBinding(int binding_index, int key_binding, int mouse_binding, int gamepad_button_binding, int gamepad_joystick_binding)
{
    TsInputSetKeyBinding(binding_index, key_binding);
    TsInputSetGamepadButtonBinding(binding_index, gamepad_button_binding);
}

void
TsInputLoad(void)
{
    MemoryCopy(global_tsinput->last_frame_controls, global_tsinput->controls, sizeof(global_tsinput->controls[0])*global_tsinput->control_count);
    
    for(int i = 0; i < global_tsinput->control_count; ++i)
    {
        global_tsinput->controls[i].down = 0;
        global_tsinput->controls[i].pressed = 0;
        global_tsinput->controls[i].control_mode = -1;
        
        if(global_tsinput->bindings[i].key_binding)
        {
            global_tsinput->controls[i].down |= platform->key_down[global_tsinput->bindings[i].key_binding];
            global_tsinput->controls[i].pressed |= platform->key_pressed[global_tsinput->bindings[i].key_binding];
            
            if(global_tsinput->controls[i].down || global_tsinput->controls[i].pressed)
            {
                global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_keyboard;
            }
        }
        
        if(global_tsinput->bindings[i].gamepad_button_binding)
        {
            b32 down = (platform->gamepads[0].button_states[global_tsinput->bindings[i].gamepad_button_binding]);
            b32 pressed = (down && !(platform->last_frame_gamepads[0].button_states[global_tsinput->bindings[i].gamepad_button_binding]));
            global_tsinput->controls[i].down |= down;
            global_tsinput->controls[i].pressed |= pressed;
            
            if(down || pressed)
            {
                global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
            }
        }
        
        if(global_tsinput->bindings[i].gamepad_joystick_binding)
        {
            if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_x_negative)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_1.x;
                if(global_tsinput->controls[i].value > 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value < -0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_x_positive)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_1.x;
                if(global_tsinput->controls[i].value < 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value > 0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_x_negative)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_2.x;
                if(global_tsinput->controls[i].value > 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value < -0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_x_positive)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_2.x;
                if(global_tsinput->controls[i].value < 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value > 0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_y_negative)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_1.y;
                if(global_tsinput->controls[i].value > 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value < -0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_y_positive)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_1.y;
                if(global_tsinput->controls[i].value < 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value > 0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_y_negative)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_2.y;
                if(global_tsinput->controls[i].value > 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value < -0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(global_tsinput->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_y_positive)
            {
                global_tsinput->controls[i].value = platform->gamepads[0].joystick_2.y;
                if(global_tsinput->controls[i].value < 0)
                {
                    global_tsinput->controls[i].value = 0;
                }
                else if(global_tsinput->controls[i].value > 0.1f)
                {
                    global_tsinput->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
        }
    }
    
    // NOTE(rjf): Calculate active control mode.
    {
        i32 control_mode_control_counts[TSINPUT_CONTROL_MODE_MAX] = {0};
        
        for(int i = 0; i < global_tsinput->control_count; ++i)
        {
            if(global_tsinput->controls[i].control_mode >= 0 &&
               global_tsinput->controls[i].control_mode < TSINPUT_CONTROL_MODE_MAX)
            {
                control_mode_control_counts[global_tsinput->controls[i].control_mode] += 1;
            }
        }
        
        TsInputControlMode control_mode = global_tsinput->active_control_mode;
        i32 control_mode_count = 0;
        
        for(int i = 0; i < TSINPUT_CONTROL_MODE_MAX; ++i)
        {
            if(control_mode_control_counts[i] > control_mode_count)
            {
                control_mode = (TsInputControlMode)i;
                control_mode_count = control_mode_control_counts[i];
            }
        }
        
        global_tsinput->active_control_mode = control_mode;
    }
    
}

TsInputControlMode
TsInputGetActiveControlMode(void)
{
    return global_tsinput->active_control_mode;
}

TsInputControl *
TsInputGetControl(int index)
{
    return global_tsinput->controls + index;
}

TsInputBinding *
TsInputGetBinding(int binding_index)
{
    return global_tsinput->bindings + binding_index;
}

TsInputControl *
TsInputGetLastFrameControl(int index)
{
    return global_tsinput->last_frame_controls + index;
}
