void TsUIResetStacks(void)
{
global_tsui->x_stack_size = 0;
global_tsui->y_stack_size = 0;
global_tsui->width_stack_size = 0;
global_tsui->height_stack_size = 0;
global_tsui->text_color_stack_size = 0;
global_tsui->text_scale_stack_size = 0;
global_tsui->group_mode_stack_size = 0;
global_tsui->input_grid_state_stack_size = 0;
global_tsui->style_flags_stack_size = 0;
global_tsui->active_dropdown_stack_size = 0;
global_tsui->clip_stack_size = 0;
}

void TsUIPushX(// @AddToCurrent 
f32 value)
{
HardAssert(global_tsui->x_stack_size < ArrayCount(global_tsui->x_stack));
global_tsui->x_stack[global_tsui->x_stack_size].value = global_tsui->current_state.x.value;
global_tsui->current_state.x.value += value;
++global_tsui->x_stack_size;
}

void TsUIPopX(void)
{
HardAssert(global_tsui->x_stack_size > 0);
--global_tsui->x_stack_size;
global_tsui->current_state.x.value = global_tsui->x_stack[global_tsui->x_stack_size].value;
}

void TsUIPushY(// @AddToCurrent 
f32 value)
{
HardAssert(global_tsui->y_stack_size < ArrayCount(global_tsui->y_stack));
global_tsui->y_stack[global_tsui->y_stack_size].value = global_tsui->current_state.y.value;
global_tsui->current_state.y.value += value;
++global_tsui->y_stack_size;
}

void TsUIPopY(void)
{
HardAssert(global_tsui->y_stack_size > 0);
--global_tsui->y_stack_size;
global_tsui->current_state.y.value = global_tsui->y_stack[global_tsui->y_stack_size].value;
}

void TsUIPushTextColor(v4 value)
{
HardAssert(global_tsui->text_color_stack_size < ArrayCount(global_tsui->text_color_stack));
global_tsui->text_color_stack[global_tsui->text_color_stack_size].value = global_tsui->current_state.text_color.value;
global_tsui->current_state.text_color.value = value;
++global_tsui->text_color_stack_size;
}

void TsUIPopTextColor(void)
{
HardAssert(global_tsui->text_color_stack_size > 0);
--global_tsui->text_color_stack_size;
global_tsui->current_state.text_color.value = global_tsui->text_color_stack[global_tsui->text_color_stack_size].value;
}

void TsUIPushTextScale(f32 value)
{
HardAssert(global_tsui->text_scale_stack_size < ArrayCount(global_tsui->text_scale_stack));
global_tsui->text_scale_stack[global_tsui->text_scale_stack_size].value = global_tsui->current_state.text_scale.value;
global_tsui->current_state.text_scale.value = value;
++global_tsui->text_scale_stack_size;
}

void TsUIPopTextScale(void)
{
HardAssert(global_tsui->text_scale_stack_size > 0);
--global_tsui->text_scale_stack_size;
global_tsui->current_state.text_scale.value = global_tsui->text_scale_stack[global_tsui->text_scale_stack_size].value;
}

void TsUIPushGroupMode(b32 is_column, f32 same_line_offset, b32 same_line, TsUIWidget *last_widget)
{
HardAssert(global_tsui->group_mode_stack_size < ArrayCount(global_tsui->group_mode_stack));
global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].is_column = global_tsui->current_state.group_mode.is_column;
global_tsui->current_state.group_mode.is_column = is_column;
global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].same_line_offset = global_tsui->current_state.group_mode.same_line_offset;
global_tsui->current_state.group_mode.same_line_offset = same_line_offset;
global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].same_line = global_tsui->current_state.group_mode.same_line;
global_tsui->current_state.group_mode.same_line = same_line;
global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].last_widget = global_tsui->current_state.group_mode.last_widget;
global_tsui->current_state.group_mode.last_widget = last_widget;
++global_tsui->group_mode_stack_size;
}

void TsUIPopGroupMode(void)
{
HardAssert(global_tsui->group_mode_stack_size > 0);
--global_tsui->group_mode_stack_size;
global_tsui->current_state.group_mode.is_column = global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].is_column;
global_tsui->current_state.group_mode.same_line_offset = global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].same_line_offset;
global_tsui->current_state.group_mode.same_line = global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].same_line;
global_tsui->current_state.group_mode.last_widget = global_tsui->group_mode_stack[global_tsui->group_mode_stack_size].last_widget;
}

void TsUIPushStyleFlags(i32 flags)
{
HardAssert(global_tsui->style_flags_stack_size < ArrayCount(global_tsui->style_flags_stack));
global_tsui->style_flags_stack[global_tsui->style_flags_stack_size].flags = global_tsui->current_state.style_flags.flags;
global_tsui->current_state.style_flags.flags = flags;
++global_tsui->style_flags_stack_size;
}

void TsUIPopStyleFlags(void)
{
HardAssert(global_tsui->style_flags_stack_size > 0);
--global_tsui->style_flags_stack_size;
global_tsui->current_state.style_flags.flags = global_tsui->style_flags_stack[global_tsui->style_flags_stack_size].flags;
}

void TsUIPushActiveDropdown(TsUIWidget *value)
{
HardAssert(global_tsui->active_dropdown_stack_size < ArrayCount(global_tsui->active_dropdown_stack));
global_tsui->active_dropdown_stack[global_tsui->active_dropdown_stack_size].value = global_tsui->current_state.active_dropdown.value;
global_tsui->current_state.active_dropdown.value = value;
++global_tsui->active_dropdown_stack_size;
}

void TsUIPopActiveDropdown(void)
{
HardAssert(global_tsui->active_dropdown_stack_size > 0);
--global_tsui->active_dropdown_stack_size;
global_tsui->current_state.active_dropdown.value = global_tsui->active_dropdown_stack[global_tsui->active_dropdown_stack_size].value;
}

void TsUIPushClip(v4 value)
{
HardAssert(global_tsui->clip_stack_size < ArrayCount(global_tsui->clip_stack));
global_tsui->clip_stack[global_tsui->clip_stack_size].value = global_tsui->current_state.clip.value;
global_tsui->current_state.clip.value = value;
++global_tsui->clip_stack_size;
}

void TsUIPopClip(void)
{
HardAssert(global_tsui->clip_stack_size > 0);
--global_tsui->clip_stack_size;
global_tsui->current_state.clip.value = global_tsui->clip_stack[global_tsui->clip_stack_size].value;
}

