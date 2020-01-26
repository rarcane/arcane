/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifndef TSUI_H_INCLUDED
#define TSUI_H_INCLUDED

#ifndef TSUI_WIDGET_MAX
#define TSUI_WIDGET_MAX 1024
#endif

#ifndef TSUI_WINDOW_MAX
#define TSUI_WINDOW_MAX 128
#endif

#ifndef TSUI_WIDGET_DEFAULT_WIDTH
#define TSUI_WIDGET_DEFAULT_WIDTH 250
#endif

#ifndef TSUI_WIDGET_DEFAULT_HEIGHT
#define TSUI_WIDGET_DEFAULT_HEIGHT 30
#endif

#ifndef TSUI_COLLAPSABLE_BUMP_SIZE
#define TSUI_COLLAPSABLE_BUMP_SIZE 30
#endif

#ifndef TSUI_STACK_MAX
#define TSUI_STACK_MAX 16
#endif

#ifndef TSUI_DEFAULT_INTERPOLATION_RATE
#define TSUI_DEFAULT_INTERPOLATION_RATE 20.f
#endif

#define TSUI_WINDOW_FLAG_FIXED_POSITION  (1<<0)
#define TSUI_WINDOW_FLAG_NO_TITLE_BAR    (1<<1)
#define TSUI_WINDOW_FLAG_NO_BRING_TO_TOP (1<<2)
#define TSUI_WINDOW_FLAG_NO_SCROLL       (1<<3)
#define TSUI_WINDOW_TOP ((TsUIWindow *)0xffffffffffffffff)

typedef struct TsUI TsUI;
typedef struct TsUIWidget TsUIWidget;
typedef struct TsUIWindow TsUIWindow;
typedef f32 TsUIGetTextWidthCallback(TsUI *ui, i32 style_flags, f32 text_scale, char *text, void *user_data);
typedef f32 TsUIGetTextHeightCallback(TsUI *ui, i32 style_flags, f32 text_scale, char *text, void *user_data);
typedef void TsUIRenderWidgetCallback(TsUI *ui, i32 style_flags, TsUIWidget *widget, v4 clip, void *user_data, b32 widget_is_hot, b32 widget_is_active);
typedef void TsUIRenderWindowCallback(TsUI *ui, i32 style_flags, TsUIWindow *window, void *user_data);
typedef void TsUIClickSoundCallback(TsUI *ui, i32 style_flags, void *user_data);
typedef void TsUIHoverSoundCallback(TsUI *ui, i32 style_flags, void *user_data);

#ifdef TS2D_H_INCLUDED
void TsUIRenderWidgetDefaultTs2d(TsUI *ui, i32 style_flags, TsUIWidget *widget, v4 clip, void *user_data, b32 widget_is_hot, b32 widget_is_active);
void TsUIRenderWindowDefaultTs2d(TsUI *ui, i32 style_flags, TsUIWindow *window, void *user_data);
#endif

typedef struct TsUIFrameData TsUIFrameData;
struct TsUIFrameData
{
    f32 render_width;
    f32 render_height;
    f32 cursor_x;
    f32 cursor_y;
    f32 cursor_scroll_x;
    f32 cursor_scroll_y;
    b32 cursor_left_down;
    b32 cursor_right_down;
    b32 cursor_left_pressed;
    b32 cursor_right_pressed;
    b32 up_pressed;
    b32 left_pressed;
    b32 down_pressed;
    b32 right_pressed;
    b32 up_hold;
    b32 left_hold;
    b32 down_hold;
    b32 right_hold;
    b32 enter_pressed;
    b32 tab_pressed;
    b32 escape_pressed;
    f32 delta_t;
    MemoryArena *widget_arena;
    TsUIGetTextWidthCallback *GetTextWidth;
    void *get_text_width_user_data;
    TsUIGetTextWidthCallback *GetTextHeight;
    void *get_text_height_user_data;
    TsUIRenderWidgetCallback *RenderWidget;
    void *render_widget_user_data;
    TsUIRenderWindowCallback *RenderWindow;
    void *render_window_user_data;
    TsUIClickSoundCallback *ClickSound;
    void *click_sound_user_data;
    TsUIHoverSoundCallback *HoverSound;
    void *hover_sound_user_data;
};

enum
{
    TSUI_DIRECTION_RIGHT  = 0,
    TSUI_DIRECTION_LEFT   = 1,
    TSUI_DIRECTION_DOWN   = 2,
    TSUI_DIRECTION_UP     = 3,
};

typedef struct TsUIID TsUIID;
struct TsUIID
{
    i32 primary;
    i32 secondary;
};

typedef enum TsUIWidgetType TsUIWidgetType;
enum TsUIWidgetType
{
    TSUI_WIDGET_button,
    TSUI_WIDGET_toggler,
    TSUI_WIDGET_slider,
    TSUI_WIDGET_int_slider,
    TSUI_WIDGET_line_edit,
    TSUI_WIDGET_title,
    TSUI_WIDGET_label,
    TSUI_WIDGET_collapsable,
    TSUI_WIDGET_divider,
    TSUI_WIDGET_canvas,
    TSUI_WIDGET_window_close_button,
    TSUI_WIDGET_color_picker,
    TSUI_WIDGET_note_picker,
    TSUI_WIDGET_tile_select,
};

typedef struct TsUIWindow TsUIWindow;
struct TsUIWindow
{
    char title[64];
    u32 title_hash;
    v4 rect;
    i32 flags;
    u32 widget_index_start;
    u32 widget_index_end;
    b32 active;
    b32 stale;
    b32 deleted;
    b32 *open_value_ptr;
    v2 view_offset;
    v2 target_view_offset;
    v2 target_view_max;
    i32 style_flags;
};

typedef void TsUICanvasUpdateCallback(char *name, v4 rect, v2 mouse, void *user_data);
typedef void TsUICanvasRenderCallback(char *name, v4 rect, v2 mouse, void *user_data);

typedef struct TsUICircleMenuChoice TsUICircleMenuChoice;
struct TsUICircleMenuChoice
{
    char *string;
    TsUICircleMenuChoice *next;
};

typedef struct TsUIWidget TsUIWidget;
struct TsUIWidget
{
    TsUIWidgetType type;
    TsUIID id;
    v4 rect;
    v4 clip;
    f32 hot_transition;
    f32 active_transition;
    String text;
    v4 text_color;
    f32 text_scale;
    TsUIWindow *parent_window;
    i32 style_flags;
    
    union
    {
        
        struct ImageButton
        {
            Ts2dTexture *texture;
            v4 source;
        }
        image_button;
        
        struct Toggler
        {
            b32 toggled;
            f32 toggle_transition;
        }
        toggler;
        
        struct Slider
        {
            f32 value;
            f32 percentage;
            f32 low;
            f32 high;
        }
        slider;
        
        struct IntSlider
        {
            i32 value;
            i32 low;
            i32 high;
        }
        int_slider;
        
        struct LineEdit
        {
            String edit_text;
            f32 view_offset;
        }
        line_edit;
        
        struct Collapsable
        {
            b32 open;
            f32 open_transition;
        }
        collapsable;
        
        struct TileSelect
        {
            b32 was_selecting;
            v4 tilemap_source;
            i16 selection_x0;
            i16 selection_x1;
            i16 selection_y0;
            i16 selection_y1;
            Ts2dTexture *tilemap_texture;
        }
        tile_select;
        
        struct ColorPicker
        {
            v3 rgb;
            v3 hsv;
            b32 selecting_hue;
            b32 was_selecting;
        }
        color_picker;
        
        struct NotePicker
        {
            i32 selected_note;
        }
        note_picker;
        
        struct Canvas
        {
            TsUICanvasUpdateCallback *Update;
            void *update_user_data;
            TsUICanvasRenderCallback *Render;
            void *render_user_data;
        }
        canvas;
        
        struct CircleMenu
        {
            u32 choice_count;
            TsUICircleMenuChoice *first_choice;
        }
        circle_menu;
    };
};

#include "generated/generated_stack_definitions.h"

typedef struct TsUI TsUI;
struct TsUI
{
    
    //~ NOTE(rjf): Frame data
    f32 render_width;
    f32 render_height;
    TsUIID hot;
    v4 hot_rect;
    b32 hot_is_on_top;
    TsUIID active;
    f32 last_cursor_x;
    f32 last_cursor_y;
    f32 cursor_x;
    f32 cursor_y;
    b32 cursor_left_down;
    b32 cursor_right_down;
    b32 cursor_left_pressed;
    b32 cursor_right_pressed;
    f32 cursor_scroll_x;
    f32 cursor_scroll_y;
    b32 up_pressed;
    b32 left_pressed;
    b32 down_pressed;
    b32 right_pressed;
    b32 up_hold;
    b32 left_hold;
    b32 down_hold;
    b32 right_hold;
    b32 enter_pressed;
    b32 tab_pressed;
    b32 escape_pressed;
    b32 mouse_mode;
    f32 delta_t;
    MemoryArena *widget_arena;
    
    f32 caret_x_offset;
    f32 scroll_bar_grab_offset;
    TsUIGetTextWidthCallback *GetTextWidth;
    void *get_text_width_user_data;
    TsUIGetTextWidthCallback *GetTextHeight;
    void *get_text_height_user_data;
    TsUIRenderWidgetCallback *RenderWidget;
    void *render_widget_user_data;
    TsUIRenderWindowCallback *RenderWindow;
    void *render_window_user_data;
    TsUIClickSoundCallback *ClickSound;
    void *click_sound_user_data;
    TsUIHoverSoundCallback *HoverSound;
    void *hover_sound_user_data;
    
    //~ NOTE(rjf): Widget data
    u32 widget_count;
    u32 last_frame_widget_count;
    TsUIWidget widgets[TSUI_WIDGET_MAX];
    u32 widget_id_counters[TSUI_WIDGET_MAX];
    
    //~ NOTE(rjf): Input grid
#define TSUI_INPUT_GRID_MAX 256
    TsUIID input_grid[TSUI_INPUT_GRID_MAX][TSUI_INPUT_GRID_MAX];
    iv2 input_grid_selected_cell;
    TsUIID input_grid_active_group_first_widget_id;
    b32 input_grid_selected_cell_valid;
    b32 input_grid_selected_group_active;
    b32 input_grid_group_active;
    
#if 0
    //~ NOTE(rjf): Auto-layout and coloring information
    struct
    {
        v2 position;
        v2 size;
        v2 last_position;
        v2 last_size;
        f32 same_line_x_offset;
        b32 calculate_width_with_text;
        b32 calculate_height_with_text;
        v4 text_color;
        f32 text_scale;
        v4 clip;
        b32 is_column;
        iv2 input_grid_active_position;
        i32 input_grid_active_direction;
        i32 style_flags;
        b32 same_line;
    }
    current_auto_layout_state;
#endif
    
    //~ NOTE(rjf): Declarations for state
#include "generated/generated_stack_declarations.h"
    
    //~ NOTE(rjf): Window data
    u32 window_count;
    TsUIWindow windows[TSUI_WINDOW_MAX];
    TsUIWindow *active_window;
    TsUIWindow *allowed_window;
    u32 window_order[TSUI_WINDOW_MAX];
    TsUIWindow *dragging_window;
    TsUIWindow *scrolling_window;
};

// NOTE(rjf): Main calls
void  TsUIInit                      (TsUI *ui);
void  TsUILoadFrameDataFromPlatform (TsUIFrameData *ui_frame);
void  TsUIBeginFrame                (TsUI *ui, TsUIFrameData *frame);
void  TsUIEndFrame                  (TsUI *ui);

// NOTE(rjf): Input group calls
void  TsUIBeginInputGroup  (TsUI *ui);
void  TsUIEndInputGroup    (TsUI *ui);

// NOTE(rjf): Auto-layout and design stack calls
void  TsUIPushX                  (TsUI *ui, f32 x);
void  TsUIPopX                   (TsUI *ui);
void  TsUIPushY                  (TsUI *ui, f32 y);
void  TsUIPopY                   (TsUI *ui);
void  TsUIPushWidth              (TsUI *ui, f32 width);
void  TsUIPushAutoWidth          (TsUI *ui);
void  TsUIPopWidth               (TsUI *ui);
void  TsUIPushHeight             (TsUI *ui, f32 height);
void  TsUIPushAutoHeight         (TsUI *ui);
void  TsUIPopHeight              (TsUI *ui);
void  TsUIPushTextColor          (TsUI *ui, v4 color);
void  TsUIPopTextColor           (TsUI *ui);
void  TsUIPushTextScale          (TsUI *ui, f32 scale);
void  TsUIPopTextScale           (TsUI *ui);
void  TsUIPushStyleFlags         (TsUI *ui, i32 style_flags);
void  TsUIPopStyleFlags          (TsUI *ui);
void  TsUIPushPosition           (TsUI *ui, v2 pos);
void  TsUIPopPosition            (TsUI *ui);
void  TsUIPushSize               (TsUI *ui, v2 size);
void  TsUIPopSize                (TsUI *ui);
void  TsUIPushColumn             (TsUI *ui, v2 position, v2 size);
void  TsUIPushCenteredColumn     (TsUI *ui, v2 size, u32 number_of_widgets);
void  TsUIPopColumn              (TsUI *ui);
void  TsUIPushRow                (TsUI *ui, v2 position, v2 size);
void  TsUIPushAutoRow            (TsUI *ui, v2 position, f32 height);
void  TsUIPopRow                 (TsUI *ui);
void  TsUIPushClip               (TsUI *ui, v4 clip);
void  TsUIPopClip                (TsUI *ui);
void  TsUIPushInputGridDirection (TsUI *ui, i32 direction);
void  TsUIPopInputGridDirection  (TsUI *ui);
void  TsUISameLine               (TsUI *ui);

// NOTE(rjf): Styled widget calls
b32   TsUIStyledButton                   (TsUI *ui, i32 style_flags, char *text);
b32   TsUIStyledToggler                  (TsUI *ui, i32 style_flags, char *text, b32 value);
f32   TsUIStyledSlider                   (TsUI *ui, i32 style_flags, char *text, f32 value, f32 low, f32 high);
i32   TsUIStyledIntSlider                (TsUI *ui, i32 style_flags, char *text, i32 value, i32 low, i32 high);
char *TsUIStyledLineEdit                 (TsUI *ui, i32 style_flags, char *text, char *edit_text, u32 edit_text_max);
void  TsUIStyledTitle                    (TsUI *ui, i32 style_flags, char *text);
void  TsUIStyledLabel                    (TsUI *ui, i32 style_flags, char *text);
b32   TsUIStyledCollapsable              (TsUI *ui, i32 style_flags, char *text);
void  TsUIStyledDivider                  (TsUI *ui, i32 style_flags);
void  TsUIStyledCanvas                   (TsUI *ui, i32 style_flags, char *text, TsUICanvasUpdateCallback *Update, void *update_user_data, TsUICanvasRenderCallback *Render, void *render_user_data);
b32   TsUIStyledDropdown                 (TsUI *ui, i32 style_flags, char *text);
b32   TsUIStyledWindowCloseButton        (TsUI *ui, i32 style_flags);
void  TsUIStyledWindowBegin              (TsUI *ui, i32 style_flags, char *title, v4 rect, i32 flags, ...);
void  TsUIWindowEnd                      (TsUI *ui);
v3    TsUIStyledColorPicker              (TsUI *ui, i32 style_flags, char *text, v3 color);
i32   TsUIStyledNotePicker               (TsUI *ui, i32 style_flags, char *text, i32 note);
b32   TsUIStyledTileSelect               (TsUI *ui, i32 style_flags, char *text, Ts2dTexture *texture, v4 tilemap_source, i32 *tile_select_x0, i32 *tile_select_y0, i32 *tile_select_x1, i32 *tile_select_y1, b32 selection_from_widget);

// NOTE(rjf): Ending widget calls
void  TsUICloseCurrentDropdownTree       (TsUI *ui);
void  TsUIDropdownEnd                    (TsUI *ui);
void  TsUICollapsableEnd                 (TsUI *ui);

// NOTE(rjf): Default style widget calls
b32   TsUIButton                   (TsUI *ui, char *text)                                      { return TsUIStyledButton   (ui, 0, text);                           }
b32   TsUIToggler                  (TsUI *ui, char *text, b32 value)                           { return TsUIStyledToggler  (ui, 0, text, value);                    }
f32   TsUISlider                   (TsUI *ui, char *text, f32 value, f32 low, f32 high)        { return TsUIStyledSlider   (ui, 0, text, value, low, high);         }
i32   TsUIIntSlider                (TsUI *ui, char *text, i32 value, i32 low, i32 high)        { return TsUIStyledIntSlider(ui, 0, text, value, low, high);         }
char *TsUILineEdit                 (TsUI *ui, char *text, char *edit_text, u32 edit_text_max)  { return TsUIStyledLineEdit (ui, 0, text, edit_text, edit_text_max); }
void  TsUITitle                    (TsUI *ui, char *text)                                      {        TsUIStyledTitle    (ui, 0, text);                           }
void  TsUILabel                    (TsUI *ui, char *text)                                      {        TsUIStyledLabel    (ui, 0, text);                           }
b32   TsUICollapsable              (TsUI *ui, char *text)                                      { return TsUIStyledCollapsable(ui, 0, text);                         }
void  TsUIDivider                  (TsUI *ui)                                                  {        TsUIStyledDivider  (ui, 0);                                 }
void  TsUICanvas                   (TsUI *ui, char *text, TsUICanvasUpdateCallback *Update, void *update_user_data, TsUICanvasRenderCallback *Render, void *render_user_data)
{
    TsUIStyledCanvas(ui, 0, text, Update, update_user_data, Render, render_user_data);
}
b32   TsUIDropdown                 (TsUI *ui, char *text)                                      { return TsUIStyledDropdown (ui, 0, text);                           }
b32   TsUIWindowCloseButton        (TsUI *ui)                                                  { return TsUIStyledWindowCloseButton(ui, 0);                         }
void  TsUIWindowBegin              (TsUI *ui, char *title, v4 rect, i32 flags, b32 *open_ptr)  {        TsUIStyledWindowBegin(ui, 0, title, rect, flags, open_ptr); }
v3    TsUIColorPicker              (TsUI *ui, char *text, v3 color)                            { return TsUIStyledColorPicker(ui, 0, text, color);                  }
i32   TsUINotePicker               (TsUI *ui, char *text, i32 note)                            { return TsUIStyledNotePicker(ui, 0, text, note);                    }
b32   TsUITileSelect               (TsUI *ui, char *text, Ts2dTexture *texture, v4 tilemap_source, i32 *tile_select_x0, i32 *tile_select_y0, i32 *tile_select_x1, i32 *tile_select_y1, b32 selection_from_widget)
{
    return TsUIStyledTileSelect(ui, 0, text, texture, tilemap_source, tile_select_x0, tile_select_y0, tile_select_x1, tile_select_y1, selection_from_widget);
}

#ifdef TSUI_STYLE_CALLS_FILE
#define TsUIStyleCall(name, style_flags) \
b32   TsUI##name##Button                   (TsUI *ui, char *text)                                      { return TsUIStyledButton   (ui, style_flags, text);                           }\
b32   TsUI##name##Toggler                  (TsUI *ui, char *text, b32 value)                           { return TsUIStyledToggler  (ui, style_flags, text, value);                    }\
f32   TsUI##name##Slider                   (TsUI *ui, char *text, f32 value, f32 low, f32 high)        { return TsUIStyledSlider   (ui, style_flags, text, value, low, high);         }\
i32   TsUI##name##IntSlider                (TsUI *ui, char *text, i32 value, i32 low, i32 high)        { return TsUIStyledIntSlider(ui, style_flags, text, value, low, high);         }\
char *TsUI##name##LineEdit                 (TsUI *ui, char *text, char *edit_text, u32 edit_text_max)  { return TsUIStyledLineEdit (ui, style_flags, text, edit_text, edit_text_max); }\
void  TsUI##name##Title                    (TsUI *ui, char *text)                                      { TsUIStyledTitle    (ui, style_flags, text);                           }\
void  TsUI##name##Label                    (TsUI *ui, char *text)                                      { TsUIStyledLabel    (ui, style_flags, text);                           }\
b32   TsUI##name##Collapsable              (TsUI *ui, char *text)                                      { return TsUIStyledCollapsable(ui, style_flags, text);                  }\
void  TsUI##name##Divider                  (TsUI *ui)                                                  { TsUIStyledDivider  (ui, style_flags);                                 }\
void  TsUI##name##Canvas                   (TsUI *ui, char *text, TsUICanvasUpdateCallback *Update, void *update_user_data, TsUICanvasRenderCallback *Render, void *render_user_data)\
{\
TsUIStyledCanvas(ui, style_flags, text, Update, update_user_data, Render, render_user_data);\
}\
b32   TsUI##name##Dropdown                 (TsUI *ui, char *text)                                      { return TsUIStyledDropdown (ui, style_flags, text);                           }\
b32   TsUI##name##WindowCloseButton        (TsUI *ui)                                                  { return TsUIStyledWindowCloseButton(ui, style_flags);                         }\
void  TsUI##name##WindowBegin              (TsUI *ui, char *title, v4 rect, i32 flags, b32 *open_ptr)  { TsUIStyledWindowBegin(ui, style_flags, title, rect, flags, open_ptr);              }\
v3    TsUI##name##ColorPicker              (TsUI *ui, char *text, v3 color)                            { return TsUIStyledColorPicker(ui, style_flags, text, color);                  }\
i32   TsUI##name##NotePicker               (TsUI *ui, char *text, i32 note)                            { return TsUIStyledNotePicker(ui, style_flags, text, note);                    }\
b32   TsUI##name##TileSelect               (TsUI *ui, char *text, Ts2dTexture *texture, v4 tilemap_source, i32 *tile_select_x0, i32 *tile_select_y0, i32 *tile_select_x1, i32 *tile_select_y1, b32 selection_from_widget)\
{\
return TsUIStyledTileSelect(ui, style_flags, text, texture, tilemap_source, tile_select_x0, tile_select_y0, tile_select_x1, tile_select_y1, selection_from_widget);\
}\

#include TSUI_STYLE_CALLS_FILE
#undef TsUIStyleCall
#endif

#include "generated/generated_stack_procedures.h"

#endif // TSUI_H_INCLUDED