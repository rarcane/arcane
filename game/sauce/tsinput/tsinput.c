internal TsInputBinding
TsInputBindingInit(i32 key, i32 mouse, i32 gamepad_button, i32 gamepad_joystick)
{
    TsInputBinding binding = { key, mouse, gamepad_button, gamepad_joystick };
    return binding;
}

void
TsInputResetBindings(TsInput *input)
{
    MemoryCopy(input->bindings, input->default_bindings, input->control_count * sizeof(TsInputBinding));
}

void
TsInputInit(TsInput *input, int binding_count, TsInputBinding *bindings, MemoryArena *arena)
{
    input->control_count = binding_count;
    input->controls = MemoryArenaAllocateAndZero(arena, binding_count * sizeof(TsInputControl));
    input->last_frame_controls = MemoryArenaAllocateAndZero(arena, binding_count * sizeof(TsInputControl));
    input->bindings = MemoryArenaAllocateAndZero(arena, binding_count * sizeof(TsInputBinding));
    input->default_bindings = MemoryArenaAllocateAndZero(arena, binding_count * sizeof(TsInputBinding));
    HardAssert(input->controls != 0);
    HardAssert(input->bindings != 0);
    HardAssert(input->default_bindings != 0);
    MemoryCopy(input->bindings, bindings, sizeof(TsInputBinding)*binding_count);
    MemoryCopy(input->default_bindings, bindings, sizeof(TsInputBinding)*binding_count);
}

void
TsInputLoad(TsInput *input)
{
    MemoryCopy(input->last_frame_controls, input->controls, sizeof(input->controls[0])*input->control_count);
    
    for(int i = 0; i < input->control_count; ++i)
    {
        input->controls[i].down = 0;
        input->controls[i].pressed = 0;
        input->controls[i].control_mode = -1;
        
        if(input->bindings[i].key_binding)
        {
            input->controls[i].down |= platform->key_down[input->bindings[i].key_binding];
            input->controls[i].pressed |= platform->key_pressed[input->bindings[i].key_binding];
            
            if(input->controls[i].down || input->controls[i].pressed)
            {
                input->controls[i].control_mode = TSINPUT_CONTROL_MODE_keyboard;
            }
        }
        
        if(input->bindings[i].gamepad_button_binding)
        {
            b32 down = (platform->gamepads[0].button_states[input->bindings[i].gamepad_button_binding]);
            b32 pressed = (down && !(platform->last_frame_gamepads[0].button_states[input->bindings[i].gamepad_button_binding]));
            input->controls[i].down |= down;
            input->controls[i].pressed |= pressed;
            
            if(down || pressed)
            {
                input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
            }
        }
        
        if(input->bindings[i].gamepad_joystick_binding)
        {
            if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_x_negative)
            {
                input->controls[i].value = platform->gamepads[0].joystick_1.x;
                if(input->controls[i].value > 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value < -0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_x_positive)
            {
                input->controls[i].value = platform->gamepads[0].joystick_1.x;
                if(input->controls[i].value < 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value > 0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_x_negative)
            {
                input->controls[i].value = platform->gamepads[0].joystick_2.x;
                if(input->controls[i].value > 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value < -0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_x_positive)
            {
                input->controls[i].value = platform->gamepads[0].joystick_2.x;
                if(input->controls[i].value < 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value > 0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_y_negative)
            {
                input->controls[i].value = platform->gamepads[0].joystick_1.y;
                if(input->controls[i].value > 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value < -0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_left_y_positive)
            {
                input->controls[i].value = platform->gamepads[0].joystick_1.y;
                if(input->controls[i].value < 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value > 0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_y_negative)
            {
                input->controls[i].value = platform->gamepads[0].joystick_2.y;
                if(input->controls[i].value > 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value < -0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
            else if(input->bindings[i].gamepad_joystick_binding == TSINPUT_GAMEPAD_JOYSTICK_right_y_positive)
            {
                input->controls[i].value = platform->gamepads[0].joystick_2.y;
                if(input->controls[i].value < 0)
                {
                    input->controls[i].value = 0;
                }
                else if(input->controls[i].value > 0.1f)
                {
                    input->controls[i].control_mode = TSINPUT_CONTROL_MODE_gamepad;
                }
            }
        }
    }
    
    // NOTE(rjf): Calculate active control mode.
    {
        i32 control_mode_control_counts[TSINPUT_CONTROL_MODE_MAX] = {0};
        
        for(int i = 0; i < input->control_count; ++i)
        {
            if(core->input->controls[i].control_mode >= 0 &&
               core->input->controls[i].control_mode < TSINPUT_CONTROL_MODE_MAX)
            {
                control_mode_control_counts[core->input->controls[i].control_mode] += 1;
            }
        }
        
        TsInputControlMode control_mode = input->active_control_mode;
        i32 control_mode_count = 0;
        
        for(int i = 0; i < TSINPUT_CONTROL_MODE_MAX; ++i)
        {
            if(control_mode_control_counts[i] > control_mode_count)
            {
                control_mode = (TsInputControlMode)i;
                control_mode_count = control_mode_control_counts[i];
            }
        }
        
        input->active_control_mode = control_mode;
    }
    
}

TsInputControlMode
TsInputGetActiveControlMode(TsInput *input)
{
    return input->active_control_mode;
}