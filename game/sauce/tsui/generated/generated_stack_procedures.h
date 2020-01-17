void TsUIResetStacks(TsUI *ui);
void TsUIPushX(TsUI *ui, // @AddToCurrent 
f32 value);
void TsUIPopX(TsUI *ui);
void TsUIPushY(TsUI *ui, // @AddToCurrent 
f32 value);
void TsUIPopY(TsUI *ui);
void TsUIPushTextColor(TsUI *ui, v4 value);
void TsUIPopTextColor(TsUI *ui);
void TsUIPushTextScale(TsUI *ui, f32 value);
void TsUIPopTextScale(TsUI *ui);
void TsUIPushGroupMode(TsUI *ui, b32 is_column, f32 same_line_offset, b32 same_line, TsUIWidget *last_widget);
void TsUIPopGroupMode(TsUI *ui);
void TsUIPushStyleFlags(TsUI *ui, i32 flags);
void TsUIPopStyleFlags(TsUI *ui);
void TsUIPushActiveDropdown(TsUI *ui, TsUIWidget *value);
void TsUIPopActiveDropdown(TsUI *ui);
void TsUIPushClip(TsUI *ui, v4 value);
void TsUIPopClip(TsUI *ui);
