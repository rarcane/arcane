void TsUIResetStacks(TsUI *ui)
{
ui->x_stack_size = 0;
ui->y_stack_size = 0;
ui->width_stack_size = 0;
ui->height_stack_size = 0;
ui->text_color_stack_size = 0;
ui->text_scale_stack_size = 0;
ui->group_mode_stack_size = 0;
ui->input_grid_state_stack_size = 0;
ui->style_flags_stack_size = 0;
ui->active_dropdown_stack_size = 0;
ui->clip_stack_size = 0;
}

void TsUIPushX(TsUI *ui, // @AddToCurrent 
f32 value)
{
HardAssert(ui->x_stack_size < ArrayCount(ui->x_stack));
ui->x_stack[ui->x_stack_size].value = ui->current_state.x.value;
ui->current_state.x.value += value;
++ui->x_stack_size;
}

void TsUIPopX(TsUI *ui)
{
HardAssert(ui->x_stack_size > 0);
--ui->x_stack_size;
ui->current_state.x.value = ui->x_stack[ui->x_stack_size].value;
}

void TsUIPushY(TsUI *ui, // @AddToCurrent 
f32 value)
{
HardAssert(ui->y_stack_size < ArrayCount(ui->y_stack));
ui->y_stack[ui->y_stack_size].value = ui->current_state.y.value;
ui->current_state.y.value += value;
++ui->y_stack_size;
}

void TsUIPopY(TsUI *ui)
{
HardAssert(ui->y_stack_size > 0);
--ui->y_stack_size;
ui->current_state.y.value = ui->y_stack[ui->y_stack_size].value;
}

void TsUIPushTextColor(TsUI *ui, v4 value)
{
HardAssert(ui->text_color_stack_size < ArrayCount(ui->text_color_stack));
ui->text_color_stack[ui->text_color_stack_size].value = ui->current_state.text_color.value;
ui->current_state.text_color.value = value;
++ui->text_color_stack_size;
}

void TsUIPopTextColor(TsUI *ui)
{
HardAssert(ui->text_color_stack_size > 0);
--ui->text_color_stack_size;
ui->current_state.text_color.value = ui->text_color_stack[ui->text_color_stack_size].value;
}

void TsUIPushTextScale(TsUI *ui, f32 value)
{
HardAssert(ui->text_scale_stack_size < ArrayCount(ui->text_scale_stack));
ui->text_scale_stack[ui->text_scale_stack_size].value = ui->current_state.text_scale.value;
ui->current_state.text_scale.value = value;
++ui->text_scale_stack_size;
}

void TsUIPopTextScale(TsUI *ui)
{
HardAssert(ui->text_scale_stack_size > 0);
--ui->text_scale_stack_size;
ui->current_state.text_scale.value = ui->text_scale_stack[ui->text_scale_stack_size].value;
}

void TsUIPushGroupMode(TsUI *ui, b32 is_column, f32 same_line_offset, b32 same_line, TsUIWidget *last_widget)
{
HardAssert(ui->group_mode_stack_size < ArrayCount(ui->group_mode_stack));
ui->group_mode_stack[ui->group_mode_stack_size].is_column = ui->current_state.group_mode.is_column;
ui->current_state.group_mode.is_column = is_column;
ui->group_mode_stack[ui->group_mode_stack_size].same_line_offset = ui->current_state.group_mode.same_line_offset;
ui->current_state.group_mode.same_line_offset = same_line_offset;
ui->group_mode_stack[ui->group_mode_stack_size].same_line = ui->current_state.group_mode.same_line;
ui->current_state.group_mode.same_line = same_line;
ui->group_mode_stack[ui->group_mode_stack_size].last_widget = ui->current_state.group_mode.last_widget;
ui->current_state.group_mode.last_widget = last_widget;
++ui->group_mode_stack_size;
}

void TsUIPopGroupMode(TsUI *ui)
{
HardAssert(ui->group_mode_stack_size > 0);
--ui->group_mode_stack_size;
ui->current_state.group_mode.is_column = ui->group_mode_stack[ui->group_mode_stack_size].is_column;
ui->current_state.group_mode.same_line_offset = ui->group_mode_stack[ui->group_mode_stack_size].same_line_offset;
ui->current_state.group_mode.same_line = ui->group_mode_stack[ui->group_mode_stack_size].same_line;
ui->current_state.group_mode.last_widget = ui->group_mode_stack[ui->group_mode_stack_size].last_widget;
}

void TsUIPushStyleFlags(TsUI *ui, i32 flags)
{
HardAssert(ui->style_flags_stack_size < ArrayCount(ui->style_flags_stack));
ui->style_flags_stack[ui->style_flags_stack_size].flags = ui->current_state.style_flags.flags;
ui->current_state.style_flags.flags = flags;
++ui->style_flags_stack_size;
}

void TsUIPopStyleFlags(TsUI *ui)
{
HardAssert(ui->style_flags_stack_size > 0);
--ui->style_flags_stack_size;
ui->current_state.style_flags.flags = ui->style_flags_stack[ui->style_flags_stack_size].flags;
}

void TsUIPushActiveDropdown(TsUI *ui, TsUIWidget *value)
{
HardAssert(ui->active_dropdown_stack_size < ArrayCount(ui->active_dropdown_stack));
ui->active_dropdown_stack[ui->active_dropdown_stack_size].value = ui->current_state.active_dropdown.value;
ui->current_state.active_dropdown.value = value;
++ui->active_dropdown_stack_size;
}

void TsUIPopActiveDropdown(TsUI *ui)
{
HardAssert(ui->active_dropdown_stack_size > 0);
--ui->active_dropdown_stack_size;
ui->current_state.active_dropdown.value = ui->active_dropdown_stack[ui->active_dropdown_stack_size].value;
}

void TsUIPushClip(TsUI *ui, v4 value)
{
HardAssert(ui->clip_stack_size < ArrayCount(ui->clip_stack));
ui->clip_stack[ui->clip_stack_size].value = ui->current_state.clip.value;
ui->current_state.clip.value = value;
++ui->clip_stack_size;
}

void TsUIPopClip(TsUI *ui)
{
HardAssert(ui->clip_stack_size > 0);
--ui->clip_stack_size;
ui->current_state.clip.value = ui->clip_stack[ui->clip_stack_size].value;
}

