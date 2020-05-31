internal void DrawGameUI();
internal void DrawEditorUI();
// NOTE(randy): Switches the editor to a new state, providing new state-specific debug flags. If the state is already set, nothing will happen.
internal void SwitchEditorState(EditorState editor_state);

typedef struct InventoryIconCanvasData
{
	Item *item;
	b8 is_hovered;
	i32 slot;
} InventoryIconCanvasData;

typedef struct GrabbedIconCanvasData
{
	StaticSpriteData *static_sprite;
} GrabbedIconCanvasData;