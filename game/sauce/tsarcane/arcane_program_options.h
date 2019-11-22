// NOTE(rjf): Version
#define VERSION_MAJOR 0
#define VERSION_MINOR 0

// NOTE(rjf): Telescope Foundation Options
#define TS_APP_FILENAME "arcane"
#define TS_APP_WINDOW_TITLE "Arcane"
#define TS_APP_DEFAULT_WINDOW_WIDTH 1280
#define TS_APP_DEFAULT_WINDOW_HEIGHT 720
#define TS_APP_PERMANENT_STORAGE_SIZE Megabytes(64)
#define TS_APP_SCRATCH_STORAGE_SIZE Megabytes(64)
#define TS_PLATFORM_WIN32 1
#define TS_PLATFORM TS_PLATFORM_WIN32

// NOTE(rjf): Telescope 2D
#define TS2D_BACKEND TS2D_OPENGL
#define TS2D_DEBUG 1

// NOTE(rjf): Build-modes for team roles
#define ROLE_PROGRAMMER 1
#define ROLE_ARTIST 2

// NOTE(rjf): Telescope Assets
#define TSASSETS_ASSET_TYPE_FILE "tsarcane/arcane_asset_types.inc"