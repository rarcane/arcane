typedef void (*EntityPresetCallback)(Entity *);
#define EntityPresetCallback(name) internal void name##EntityPresetCallback(Entity *entity)

EntityPresetCallback(Tree);
EntityPresetCallback(BG1Tree);
EntityPresetCallback(BG2Tree);