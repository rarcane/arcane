internal void DrawGameUI();
internal void DrawEditorUI();
// NOTE(randy): Switches the editor to a new state, providing new state-specific debug flags. If the state is already set, nothing will happen.
internal void SwitchEditorState(EditorState editor_state);

typedef struct InventoryIconCanvasData
{
	StorageComponent *storage_comp;
	ItemComponent *item_comp;
	i32 slot;
	b8 is_hovered;
} InventoryIconCanvasData;

typedef struct GrabbedIconCanvasData
{
	StaticSpriteData *static_sprite;
} GrabbedIconCanvasData;