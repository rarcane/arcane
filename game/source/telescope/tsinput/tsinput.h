/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifndef TSINPUT_H_INCLUDED
#define TSINPUT_H_INCLUDED

typedef enum TsInputControlMode TsInputControlMode;
enum TsInputControlMode
{
    TSINPUT_CONTROL_MODE_keyboard,
    TSINPUT_CONTROL_MODE_gamepad,
    TSINPUT_CONTROL_MODE_MAX
};

typedef struct TsInputControl TsInputControl;
struct TsInputControl
{
    b32 down;
    b32 pressed;
    f32 value;
    i32 control_mode;
};

typedef struct TsInputBinding TsInputBinding;
struct TsInputBinding
{
    i32 key_binding;
    i32 mouse_binding;
    i32 gamepad_button_binding;
    i32 gamepad_joystick_binding;
};

typedef enum TsInputMouseMovement TsInputMouseMovement;
enum TsInputMouseMovement
{
    TSINPUT_MOUSE_MOVEMENT_null,
    TSINPUT_MOUSE_MOVEMENT_x_positive,
    TSINPUT_MOUSE_MOVEMENT_x_negative,
    TSINPUT_MOUSE_MOVEMENT_y_positive,
    TSINPUT_MOUSE_MOVEMENT_y_negative,
};

typedef enum TsInputGamepadMovement TsInputGamepadMovement;
enum TsInputGamepadMovement
{
    TSINPUT_GAMEPAD_JOYSTICK_null,
    TSINPUT_GAMEPAD_JOYSTICK_left_x_negative,
    TSINPUT_GAMEPAD_JOYSTICK_left_x_positive,
    TSINPUT_GAMEPAD_JOYSTICK_left_y_negative,
    TSINPUT_GAMEPAD_JOYSTICK_left_y_positive,
    TSINPUT_GAMEPAD_JOYSTICK_right_x_negative,
    TSINPUT_GAMEPAD_JOYSTICK_right_x_positive,
    TSINPUT_GAMEPAD_JOYSTICK_right_y_negative,
    TSINPUT_GAMEPAD_JOYSTICK_right_y_positive,
};

typedef struct TsInput TsInput;
struct TsInput
{
    int control_count;
    TsInputControl *controls;
    TsInputControl *last_frame_controls;
    TsInputBinding *bindings;
    TsInputBinding *default_bindings;
    TsInputControlMode active_control_mode;
};

TsInput *global_tsinput = 0;

void TsInputInit(void);
void TsInputSetControls(int control_count, TsInputBinding *bindings, MemoryArena *arena);
void TsInputResetBindings(void);
void TsInputSetBinding(int binding_index, int key_binding, int mouse_binding, int gamepad_button_binding, int gamepad_joystick_binding);
void TsInputSetKeyBinding(int binding_index, int key_binding);
void TsInputSetGamepadButtonBinding(int binding_index, int gamepad_button);
TsInputBinding *TsInputGetBinding(int binding_index);
void TsInputLoad(void);
TsInputControlMode TsInputGetActiveControlMode(void);
TsInputControl *TsInputGetControl(int index);
TsInputControl *TsInputGetLastFrameControl(int index);

#endif // TSINPUT_H_INCLUDED