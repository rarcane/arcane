internal void InitialiseActions(){
#define Action(action_name, action_print_name, default_keybind)        \
	{                                                                  \
		action_data[ACTION_##action_name].bound_key = default_keybind; \
		action_data[ACTION_##action_name].name = action_print_name;    \
	}
#include "actions.inc"
#undef Action
}

internal Action *GetAction(ActionType action_enum)
{
	R_DEV_ASSERT(action_enum < ACTION_MAX, "Out of bounds action.");
	return &action_data[action_enum];
}

internal b32 IsActionDown(ActionType action_enum)
{
	Action *action = GetAction(action_enum);
	if (action->bound_key > -1)
		return platform->key_down[action->bound_key];
	else
		return 0;
}

internal b32 IsActionPressed(ActionType action_enum)
{
	Action *action = GetAction(action_enum);
	if (action->bound_key > -1)
		return platform->key_pressed[action->bound_key];
	else
		return 0;
}