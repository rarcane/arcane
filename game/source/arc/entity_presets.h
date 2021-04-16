typedef void (*EntityPresetCallback)(Entity *);
#define EntityPresetCallback(name) internal void name##EntityPresetCallback(Entity *entity)

EntityPresetCallback(TerrainSegment);