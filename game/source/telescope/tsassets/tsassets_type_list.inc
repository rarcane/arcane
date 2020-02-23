// Macro Format:
// TsAssetsAssetType(<type name>                (Not optional),
//                   <load info type name>      (Not optional),
//                   <max count>                (Not optional),
//                   <pre load function>        (Main Thread, 0 if n/a),
//                   <load function>            (Load Thread, not optional),
//                   <post load function>       (Main Thread, not optional),
//                   <clean up function>        (Main Thread, not optional),
//                   <asset is loaded function> (Not optional),
//                   <get asset key function>   (0 if n/a, and if you just want the filename to be the key),
//                   <asset directory name>     (Not optional))
//
// #define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory)

TsAssetsAssetType(Texture,
                  TextureLoadInfo,
                  4096,
                  0,
                  LoadTexture,
                  PostLoadTexture,
                  CleanUpTexture,
                  TextureIsLoaded,
                  "texture")

TsAssetsAssetType(Font,
                  FontLoadInfo,
                  4096,
                  0,
                  LoadFont,
                  PostLoadFont,
                  CleanUpFont,
                  FontIsLoaded,
                  "font")

TsAssetsAssetType(Sound,
                  SoundLoadInfo,
                  4096,
                  0,
                  LoadSound,
                  PostLoadSound,
                  CleanUpSound,
                  SoundIsLoaded,
                  "sound")

TsAssetsAssetType(AnimationSet,
                  AnimationSetLoadInfo,
                  4096,
                  PreLoadAnimationSet,
                  LoadAnimationSet,
                  PostLoadAnimationSet,
                  CleanUpAnimationSet,
                  AnimationSetIsLoaded,
                  "animation_set")