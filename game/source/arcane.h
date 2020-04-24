#define DEVELOPER_TOOLS
#define DEVELOPER_ENVIRONMENT

typedef void (*TriggerCallback)(OverlappedColliderInfo);

typedef struct Entity Entity;
typedef struct OverlappedColliderInfo
{
	Entity *overlapped_entity;
	// direction or some shit idk?
} OverlappedColliderInfo;

internal void InitialiseRunData();