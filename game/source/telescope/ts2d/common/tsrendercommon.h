#if defined(TS2D)

#define TsRenderPrefixLower(name) ts2d##name
#define TsRenderPrefixUpper(name) TS2D_##name
#define TsRenderPrefix(name) Ts2d##name
#define TsRenderStruct() global_ts2d

#elif defined(TS3D)

#define TsRenderPrefixLower(name) ts3d##name
#define TsRenderPrefixUpper(name) TS3D_##name
#define TsRenderPrefix(name) Ts3d##name
#define TsRenderStruct() global_ts3d

#endif