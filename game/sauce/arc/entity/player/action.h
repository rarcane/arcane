typedef enum ActionType ActionType;
typedef struct Action Action;

internal void InitialiseActions();

enum ActionType
{
#define Action(action_name, action_print_name, default_keybind) ACTION_##action_name,
#include "actions.inc"
#undef Action
	ACTION_MAX,
};

struct Action
{
	i32 bound_key;
	char *name;
};
global Action action_data[ACTION_MAX] = {0};