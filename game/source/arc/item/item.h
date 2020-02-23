typedef enum ItemType ItemType;
typedef struct Item Item;

internal void InitialiseItemData();

enum ItemType
{
#define Item(item_name,           \
			 item_print_name,     \
			 item_icon_sprite,    \
			 item_ground_sprite,  \
			 item_max_stack_size, \
			 item_flags)          \
	ITEM_##item_name,
#include "items.inc"
#undef Item
	ITEM_MAX
};

#define ITEM_FLAG_resource (1 << 0)
#define ITEM_FLAG_sword (1 << 1)
#define ITEM_FLAG_bouncy (1 << 2)
#define ITEM_FLAGS_HOTBARABLE (ITEM_FLAG_sword)
struct Item
{
	char print_name[20];
	SpriteType icon_sprite;
	SpriteType ground_sprite;
	i32 max_stack_size;
	b32 flags;
};
global Item item_data[ITEM_MAX] = {0};