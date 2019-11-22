#ifndef TS2D_H_INCLUDED
#define TS2D_H_INCLUDED

#if TS2D_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/

#define TS2D_FLIP_HORIZONTAL      (1<<1)
#define TS2D_FLIP_VERTICAL        (1<<2)
#define TS2D_TEXT_ALIGN_CENTER_X  (1<<3)
#define TS2D_TEXT_ALIGN_RIGHT_X   (1<<4)
#define TS2D_TEXT_ALIGN_CENTER_Y  (1<<5)

typedef struct Ts2d                 Ts2d;
typedef struct Ts2dBeginFrameInfo   Ts2dBeginFrameInfo;
typedef struct Ts2dWorldInfo        Ts2dWorldInfo;
typedef struct Ts2dTexture          Ts2dTexture;
typedef enum   Ts2dTextureFormat    Ts2dTextureFormat;
typedef enum   Ts2dTextureFlags     Ts2dTextureFlags;
typedef enum   Ts2dVertexDataFormat Ts2dVertexDataFormat;
typedef struct Ts2dSubModel         Ts2dSubModel;
typedef struct Ts2dModel            Ts2dModel;
typedef struct Ts2dSkeleton         Ts2dSkeleton;
typedef struct Ts2dFont             Ts2dFont;
typedef struct Ts2dFontGlyph        Ts2dFontGlyph;

#if TS2D_DEBUG
#define TS2D_DEBUG_EXTRA_PARAMS , char *file, int line
#define TS2D_DEBUG_EXTRA_ARGS   , file, line
#else
#define TS2D_DEBUG_EXTRA_PARAMS
#define TS2D_DEBUG_EXTRA_ARGS
#endif

void          Ts2dInit                                    (Ts2d *renderer, MemoryArena *arena);
void          Ts2dCleanUp                                 (Ts2d *renderer);
void          Ts2dBeginFrame                              (Ts2d *renderer, Ts2dBeginFrameInfo *info);
void          Ts2dEndFrame                                (Ts2d *renderer);
void          Ts2dSwapBuffers                             (Ts2d *renderer);
Ts2dTexture   Ts2dTextureInit                             (Ts2d *renderer, Ts2dTextureFormat format, int width, int height, void *data);
Ts2dTexture   Ts2dTextureInitFlags                        (Ts2d *renderer, Ts2dTextureFlags flags, Ts2dTextureFormat format, int width, int height, void *data);
void          Ts2dTextureCleanUp                          (Ts2d *renderer, Ts2dTexture *texture);
void          Ts2dTextureSetFlags                         (Ts2dTexture *texture, Ts2dTextureFlags flags);
b32           Ts2dTextureIsValid                          (Ts2dTexture *texture);

Ts2dSubModel  Ts2dSubModelInit                            (Ts2d *renderer, Ts2dVertexDataFormat format, int vertex_count, f32 *vertex_data, int index_count, i32 *index_data, Ts2dTexture *albedo, Ts2dTexture *normals);
Ts2dModel     Ts2dModelInit                               (Ts2d *renderer, int sub_model_count, Ts2dSubModel *sub_models);
void          Ts2dSubModelCleanUp                         (Ts2d *renderer, Ts2dSubModel *sub_model);
void          Ts2dModelCleanUp                            (Ts2d *renderer, Ts2dModel *model);
void          Ts2dPushModelWithSkeleton                   (Ts2d *renderer, float *transform, Ts2dModel *model, Ts2dSkeleton *skeleton);

Ts2dFont      Ts2dFontInit                                (Ts2d *renderer, Ts2dTextureFormat format, int texture_width, int texture_height, void *texture_data, int size, int line_height, u32 glyph_count, Ts2dFontGlyph *glyphs, u32 glyph_lower_bound_character);
void          Ts2dFontCleanUp                             (Ts2d *renderer, Ts2dFont *font);
f32           Ts2dFontGetLineHeight                       (Ts2dFont *font);
f32           Ts2dFontGetTextWidthN                       (Ts2dFont *font, char *text, u32 n);
f32           Ts2dFontGetTextWidth                        (Ts2dFont *font, char *text);
void          Ts2dSetDefaultFont                          (Ts2d *renderer, Ts2dFont *font);
Ts2dFont     *Ts2dGetDefaultFont                          (Ts2d *renderer);
void          _Ts2dPushWorldBegin                         (Ts2d *renderer, Ts2dWorldInfo *world_info TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushBackgroundBegin                    (Ts2d *renderer TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushWorldEnd                           (Ts2d *renderer TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushBackgroundEnd                      (Ts2d *renderer TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushClip                               (Ts2d *renderer, v4 clip TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushClipThatIsConstrainedByCurrent     (Ts2d *renderer, v4 clip TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPopClip                                (Ts2d *renderer TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushRectangleBlur                      (Ts2d *renderer, v4 rect, f32 blur_magnitude TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushLine                               (Ts2d *renderer, v4 color, v2 p1, v2 p2 TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushRect                               (Ts2d *renderer, v4 color, v4 rect TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushFilledRect                         (Ts2d *renderer, v4 color, v4 rect TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushFilledVertexColoredRect            (Ts2d *renderer, v4 color00, v4 color01, v4 color10, v4 color11, v4 rect TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushTintedTextureWithFlags             (Ts2d *renderer, Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushTintedTexture                      (Ts2d *renderer, Ts2dTexture *texture, v4 source, v4 destination, v4 tint TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushTextureWithFlags                   (Ts2d *renderer, Ts2dTexture *texture, i32 flags, v4 source, v4 destination TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushTexture                            (Ts2d *renderer, Ts2dTexture *texture, v4 source, v4 destination TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushWorldTile                          (Ts2d *renderer, Ts2dTexture *texture, iv2 source, iv2 position TS2D_DEBUG_EXTRA_PARAMS);
f32           _Ts2dPushTextWithBoldnessAndSoftnessN       (Ts2d *renderer, Ts2dFont *font, i32 flags, v4 color, v2 position, f32 font_scale, f32 boldness, f32 softness, char *text, u32 n TS2D_DEBUG_EXTRA_PARAMS);
f32           _Ts2dPushTextN                              (Ts2d *renderer, Ts2dFont *font, i32 flags, v4 color, v2 position, f32 font_scale, char *text, u32 n TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushPointLight                         (Ts2d *renderer, v2 position, v3 color, f32 radius, f32 intensity TS2D_DEBUG_EXTRA_PARAMS);
void          _Ts2dPushReflectiveRect                     (Ts2d *renderer, v4 rect, v4 color, f32 distortion, f32 distortion_time_factor TS2D_DEBUG_EXTRA_PARAMS);

#if TS2D_DEBUG
#define Ts2dPushWorldBegin(...)                     _Ts2dPushWorldBegin(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushBackgroundBegin(...)                _Ts2dPushBackgroundBegin(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushWorldEnd(...)                       _Ts2dPushWorldEnd(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushBackgroundEnd(...)                  _Ts2dPushBackgroundEnd(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushClip(...)                           _Ts2dPushClip(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushClipThatIsConstrainedByCurrent(...) _Ts2dPushClipThatIsConstrainedByCurrent(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPopClip(...)                            _Ts2dPopClip(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushRectangleBlur(...)                  _Ts2dPushRectangleBlur(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushLine(...)                           _Ts2dPushLine(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushRect(...)                           _Ts2dPushRect(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushFilledRect(...)                     _Ts2dPushFilledRect(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushFilledVertexColoredRect(...)        _Ts2dPushFilledVertexColoredRect(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTintedTextureWithFlags(...)         _Ts2dPushTintedTextureWithFlags(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTintedTexture(...)                  _Ts2dPushTintedTexture(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTextureWithFlags(...)               _Ts2dPushTextureWithFlags(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTexture(...)                        _Ts2dPushTexture(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushWorldTile(...)                      _Ts2dPushWorldTile(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTextWithBoldnessAndSoftnessN(...)   _Ts2dPushTextWithBoldnessAndSoftnessN(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTextN(...)                          _Ts2dPushTextN(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushTextWithBoldnessAndSoftness(...)    _Ts2dPushTextWithBoldnessAndSoftness(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushText(...)                           _Ts2dPushText(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushPointLight(...)                     _Ts2dPushPointLight(__VA_ARGS__, __FILE__, __LINE__)
#define Ts2dPushReflectiveRect(...)                 _Ts2dPushReflectiveRect(__VA_ARGS__, __FILE__, __LINE__)
#else
#define Ts2dPushWorldBegin(...)                     _Ts2dPushWorldBegin(__VA_ARGS__)
#define Ts2dPushBackgroundBegin(...)                _Ts2dPushBackgroundBegin(__VA_ARGS__)
#define Ts2dPushWorldEnd(...)                       _Ts2dPushWorldEnd(__VA_ARGS__)
#define Ts2dPushBackgroundEnd(...)                  _Ts2dPushBackgroundEnd(__VA_ARGS__)
#define Ts2dPushClip(...)                           _Ts2dPushClip(__VA_ARGS__)
#define Ts2dPushClipThatIsConstrainedByCurrent(...) _Ts2dPushClipThatIsConstrainedByCurrent(__VA_ARGS__)
#define Ts2dPopClip(...)                            _Ts2dPopClip(__VA_ARGS__)
#define Ts2dPushRectangleBlur(...)                  _Ts2dPushRectangleBlur(__VA_ARGS__)
#define Ts2dPushLine(...)                           _Ts2dPushLine(__VA_ARGS__)
#define Ts2dPushRect(...)                           _Ts2dPushRect(__VA_ARGS__)
#define Ts2dPushFilledRect(...)                     _Ts2dPushFilledRect(__VA_ARGS__)
#define Ts2dPushFilledVertexColoredRect(...)        _Ts2dPushFilledVertexColoredRect(__VA_ARGS__)
#define Ts2dPushTintedTextureWithFlags(...)         _Ts2dPushTintedTextureWithFlags(__VA_ARGS__)
#define Ts2dPushTintedTexture(...)                  _Ts2dPushTintedTexture(__VA_ARGS__)
#define Ts2dPushTextureWithFlags(...)               _Ts2dPushTextureWithFlags(__VA_ARGS__)
#define Ts2dPushTexture(...)                        _Ts2dPushTexture(__VA_ARGS__)
#define Ts2dPushWorldTile(...)                      _Ts2dPushWorldTile(__VA_ARGS__)
#define Ts2dPushTextWithBoldnessAndSoftnessN(...)   _Ts2dPushTextWithBoldnessAndSoftnessN(__VA_ARGS__)
#define Ts2dPushTextN(...)                          _Ts2dPushTextN(__VA_ARGS__)
#define Ts2dPushTextWithBoldnessAndSoftness(...)    _Ts2dPushTextWithBoldnessAndSoftness(__VA_ARGS__)
#define Ts2dPushText(...)                           _Ts2dPushText(__VA_ARGS__)
#define Ts2dPushPointLight(...)                     _Ts2dPushPointLight(__VA_ARGS__)
#define Ts2dPushReflectiveRect(...)                 _Ts2dPushReflectiveRect(__VA_ARGS__)
#endif

#if TS2D_WIN32
void Ts2dInitWin32(Ts2d *renderer);
#endif

#define TS2D_BLUR_SHADOWS      (1<<0)
#define TS2D_BLUR_BACKGROUNDS  (1<<1)
#define TS2D_BLOOM             (1<<2)

struct Ts2dBeginFrameInfo
{
    f32 render_width;
    f32 render_height;
    f32 delta_t;
    int flags;
};

struct Ts2dWorldInfo
{
    int flags;
    f32 shadow_opacity;
    v2 shadow_vector;
    v2 camera_pos;
    f32 camera_rotation;
    f32 camera_zoom;
    f32 brightness;
    f32 grayscale;
};

enum Ts2dTextureFormat
{
    TS2D_TEXTURE_FORMAT_R8,
    TS2D_TEXTURE_FORMAT_R8G8B8,
    TS2D_TEXTURE_FORMAT_R8G8B8A8,
    TS2D_TEXTURE_FORMAT_R32G32B32,
    TS2D_TEXTURE_FORMAT_R32G32B32A32,
    TS2D_TEXTURE_FORMAT_D16,
    TS2D_TEXTURE_FORMAT_D24,
    TS2D_TEXTURE_FORMAT_D32,
};

enum Ts2dTextureFlags
{
    TS2D_TEXTURE_FLAG_NOTHING = 0,
};

enum Ts2dVertexDataFormat
{
    TS2D_VERTEX_DATA_FORMAT_POSITION_UV_NORMAL_INTERLEAVED, // PPP,UV,NNN,PPP,UV,NNN ...
    TS2D_VERTEX_DATA_FORMAT_POSITION_UV_NORMAL_SPLIT,       // PPP,PPP,PPP,PPP ... UV,UV,UV,UV ... NNN,NNN,NNN,NNN
};

struct Ts2dFontGlyph
{
    i16 x;
    i16 y;
    i16 width;
    i16 height;
    i16 x_offset;
    i16 y_offset;
    i16 x_advance;
};

#ifndef TS2D_MAX_REQUEST_MEMORY
#define TS2D_MAX_REQUEST_MEMORY 65536
#endif

#ifndef TS2D_MAX_LINES
#define TS2D_MAX_LINES 65536
#endif

#ifndef TS2D_MAX_RECTS
#define TS2D_MAX_RECTS 65536
#endif

#ifndef TS2D_MAX_FILLED_RECTS
#define TS2D_MAX_FILLED_RECTS 65536
#endif

#ifndef TS2D_MAX_TEXTURES
#define TS2D_MAX_TEXTURES 65536
#endif

#ifndef TS2D_MAX_WORLD_TILES
#define TS2D_MAX_WORLD_TILES 65536
#endif

#ifndef TS2D_MAX_TEXT_CHARS
#define TS2D_MAX_TEXT_CHARS 65536
#endif

#ifndef TS2D_MAX_LIGHT_COUNT
#define TS2D_MAX_LIGHT_COUNT 1024
#endif

#ifndef TS2D_MAX_REFLECTIVE_RECTS
#define TS2D_MAX_REFLECTIVE_RECTS 1024
#endif

#ifndef TS2D_MEMCPY
#include <string.h>
#define TS2D_MEMCPY memcpy
#define Ts2dMemoryCopy TS2D_MEMCPY
#endif

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/

#define TS2D_ONE_OVER_SQUARE_ROOT_OF_TWO_PIf 0.3989422804f
#define TS2D_EULERS_NUMBERf 2.7182818284590452353602874713527f

#define TS2D_COMMON_DATA            \
struct                              \
{                                   \
    Ts2dRequest active_request;     \
    Ts2dRequest *first_request;     \
    Ts2dRequest *last_request;      \
    f32         current_time;       \
    Ts2dFont    *default_font;      \
}

#if TS2D_BACKEND == TS2D_OPENGL
#include "ts2d_opengl.h"
#else
#error "TS2D: Renderer backend not implemented."
#endif

#endif // TS2D_H_INCLUDED