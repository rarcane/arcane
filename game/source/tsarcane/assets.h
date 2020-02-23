
enum
{
#define TsAssetsAssetType(name, load_info_name, max, pre_load, load, post_load, clean_up, is_loaded, folder) ASSET_TYPE_##name,
#include "arcane_asset_types.inc"
#undef TsAssetsAssetType
    ASSET_TYPE_MAX
};

typedef struct TsAssets              TsAssets;
typedef struct Ts2dTexture           Ts2dTexture;
typedef struct Ts2dFont              Ts2dFont;
typedef struct Ts2dTextureLoadInfo   Ts2dTextureLoadInfo;
typedef struct Ts2dFontLoadInfo      Ts2dFontLoadInfo;
internal void PreLoadTs2dTexture     (char *name, Ts2dTexture *asset);
internal void PreLoadTs2dFont        (char *name, Ts2dFont    *asset);
internal void LoadTs2dTexture        (char *name, Ts2dTextureLoadInfo *load_info);
internal void LoadTs2dFont           (char *name, Ts2dFontLoadInfo    *load_info);
internal void PostLoadTs2dTexture    (Ts2dTextureLoadInfo *load_info, Ts2dTexture *asset);
internal void PostLoadTs2dFont       (Ts2dFontLoadInfo    *load_info, Ts2dFont    *asset);
internal void CleanUpTs2dTexture     (Ts2dTexture  *asset);
internal void CleanUpTs2dFont        (Ts2dFont     *asset);
internal int  IsTs2dTextureLoaded    (Ts2dTexture *asset);
internal int  IsTs2dFontLoaded       (Ts2dFont *asset);

internal void InitTs2dTextureLoadInfoFromPNGData(Ts2dTextureLoadInfo *texture_load_info, void *data, u32 len);
internal void InitTs2dFontLoadInfoFromPNGAndFNTData(Ts2dFontLoadInfo *font_load_info, void *image, u32 image_len, char *font_data);

struct Ts2dTextureLoadInfo
{
    int width;
    int height;
    void *texture_data;
};

struct Ts2dFontLoadInfo
{
    Ts2dTextureLoadInfo texture_load_info;
    int size;
    int line_height;
    u32 glyph_count;
    u32 glyph_lower_bound_character;
    Ts2dFontGlyph glyphs[256];
};
