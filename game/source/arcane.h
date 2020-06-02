#define DEVELOPER_TOOLS
#define DEVELOPER_ENVIRONMENT

typedef struct Entity Entity;
typedef struct OverlappedColliderInfo
{
	Entity *overlapped_entity;
	// direction or some shit idk?
} OverlappedColliderInfo;

typedef void (*TriggerCallback)(OverlappedColliderInfo);
typedef void (*InteractCallback)(Entity *);

internal void InitialiseRunData();
internal void FreeRunData();