void TsUIResetStacks(void);
void TsUIPushX(// @AddToCurrent 
f32 value);
void TsUIPopX(void);
void TsUIPushY(// @AddToCurrent 
f32 value);
void TsUIPopY(void);
void TsUIPushTextColor(v4 value);
void TsUIPopTextColor(void);
void TsUIPushTextScale(f32 value);
void TsUIPopTextScale(void);
void TsUIPushGroupMode(b32 is_column, f32 same_line_offset, b32 same_line, TsUIWidget *last_widget);
void TsUIPopGroupMode(void);
void TsUIPushStyleFlags(i32 flags);
void TsUIPopStyleFlags(void);
void TsUIPushActiveDropdown(TsUIWidget *value);
void TsUIPopActiveDropdown(void);
void TsUIPushClip(v4 value);
void TsUIPopClip(void);
