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
typedef void (*EnterInteractableCallback)(Entity *);
typedef void (*ExitInteractableCallback)(Entity *);
typedef void (*StructureBuildCallback)(Entity *);
typedef void (*TimerCompleteCallback)();
typedef void (*SpellCastCallback)();

internal void InitialiseRunData();
internal void FreeRunData();

internal void Fireball()
{
	Log("fire");
}

internal void Yeet()
{
	Log("yeet");
}