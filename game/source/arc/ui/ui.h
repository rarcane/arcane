internal void DrawGameUI();
internal void DrawEditorUI();

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