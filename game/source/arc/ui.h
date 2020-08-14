internal void DrawGameUI();
internal void DrawEditorUI();
internal void DrawCraftingUI();
// NOTE(randy): Switches the editor to a new state, providing new state-specific debug flags. If the state is already set, nothing will happen.
internal void SwitchEditorState(EditorState editor_state);

typedef struct InventoryIconCanvasData
{
	Item *item;
	b8 is_hovered;
	i32 slot;
	// b8 is_hotbar_slot;
} InventoryIconCanvasData;

typedef struct GrabbedIconCanvasData
{
	StaticSpriteData *static_sprite;
} GrabbedIconCanvasData;