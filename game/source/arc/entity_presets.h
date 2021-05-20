typedef void (*EntityPresetCallback)(Entity *);
#define EntityPresetCallback(name) internal void name##EntityPresetCallback(Entity *entity)

EntityPresetCallback(Tree);

EntityPresetCallback(BG1Hill);
EntityPresetCallback(BG1Tree);
EntityPresetCallback(BG1Sapling);
EntityPresetCallback(BG1Shrubs);

EntityPresetCallback(BG2Hill);
EntityPresetCallback(BG2Tree);
EntityPresetCallback(BG2Shrubs);

EntityPresetCallback(BG3Hill);
EntityPresetCallback(BG3Trees);
EntityPresetCallback(BG3Shrubs);

EntityPresetCallback(MidMountains);
EntityPresetCallback(FarMountains);

EntityPresetCallback(TextNote);
EntityPresetCallback(Item);
EntityPresetCallback(Blueprint);