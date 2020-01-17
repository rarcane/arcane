// @UIStack 
typedef struct TsUIX TsUIX;
struct TsUIX
{
// @AddToCurrent 
f32 value;
};

// @UIStack 
typedef struct TsUIY TsUIY;
struct TsUIY
{
// @AddToCurrent 
f32 value;
};

// @NoProc @UIStack 
typedef struct TsUIWidth TsUIWidth;
struct TsUIWidth
{
f32 value;
b32 calculate_with_text;
};

// @NoProc @UIStack 
typedef struct TsUIHeight TsUIHeight;
struct TsUIHeight
{
f32 value;
b32 calculate_with_text;
};

// @UIStack 
typedef struct TsUITextColor TsUITextColor;
struct TsUITextColor
{
v4 value;
};

// @UIStack 
typedef struct TsUITextScale TsUITextScale;
struct TsUITextScale
{
f32 value;
};

// @UIStack 
typedef struct TsUIGroupMode TsUIGroupMode;
struct TsUIGroupMode
{
b32 is_column;
f32 same_line_offset;
b32 same_line;
TsUIWidget *last_widget;
};

// @NoZero @NoProc @UIStack 
typedef struct TsUIOpenDropdown TsUIOpenDropdown;
struct TsUIOpenDropdown
{
TsUIID widget_id;
f32 open_transition;
};

// @NoProc @UIStack 
typedef struct TsUIInputGridState TsUIInputGridState;
struct TsUIInputGridState
{
iv2 position;
i32 direction;
};

// @UIStack 
typedef struct TsUIStyleFlags TsUIStyleFlags;
struct TsUIStyleFlags
{
i32 flags;
};

// @UIStack 
typedef struct TsUIActiveDropdown TsUIActiveDropdown;
struct TsUIActiveDropdown
{
TsUIWidget *value;
};

// @UIStack 
typedef struct TsUIClip TsUIClip;
struct TsUIClip
{
v4 value;
};

typedef struct TsUIState TsUIState;
struct TsUIState
{
TsUIX x;
TsUIY y;
TsUIWidth width;
TsUIHeight height;
TsUITextColor text_color;
TsUITextScale text_scale;
TsUIGroupMode group_mode;
TsUIOpenDropdown open_dropdown;
TsUIInputGridState input_grid_state;
TsUIStyleFlags style_flags;
TsUIActiveDropdown active_dropdown;
TsUIClip clip;
};

