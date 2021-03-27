
#if defined(TS2D)

typedef Ts2dTextureFormat       TsOpenGLTextureFormat;
typedef Ts2dTextureFlags        TsOpenGLTextureFlags;
typedef Ts2dFontGlyph           TsOpenGLFontGlyph;
typedef Ts2dTexture             TsOpenGLTexture;
typedef Ts2dFont                TsOpenGLFont;
typedef Ts2dSubModel            TsOpenGLSubModel;
typedef Ts2dModel               TsOpenGLModel;
typedef Ts2dMaterial            TsOpenGLMaterial;
typedef Ts2dVertexDataFormat    TsOpenGLVertexDataFormat;
typedef Ts2dSkeleton            TsOpenGLSkeleton;
#define TsOpenGLGetDefaultFont() Ts2dGetDefaultFont()

#elif defined(TS3D)

typedef Ts3dTextureFormat       TsOpenGLTextureFormat;
typedef Ts3dTextureFlags        TsOpenGLTextureFlags;
typedef Ts3dFontGlyph           TsOpenGLFontGlyph;
typedef Ts3dTexture             TsOpenGLTexture;
typedef Ts3dFont                TsOpenGLFont;
typedef Ts3dSubModel            TsOpenGLSubModel;
typedef Ts3dModel               TsOpenGLModel;
typedef Ts3dMaterial            TsOpenGLMaterial;
typedef Ts3dVertexDataFormat    TsOpenGLVertexDataFormat;
typedef Ts3dSkeleton            TsOpenGLSkeleton;
#define TsOpenGLGetDefaultFont() Ts3dGetDefaultFont()

#endif

struct TsRenderPrefix(Texture)
{
    GLuint id;
    int width;
    int height;
    TsRenderPrefix(TextureFormat) format;
    TsRenderPrefix(TextureFlags) flags;
};

struct TsRenderPrefix(Font)
{
    TsRenderPrefix(Texture) texture;
    i16 size;
    i16 line_height;
    u32 glyph_count;
    TsRenderPrefix(FontGlyph) *glyphs;
    u32 glyph_lower_bound_character;
};

struct TsRenderPrefix(Material)
{
    v4 backing_color;
    TsRenderPrefix(Texture) *albedo_texture;
};

struct TsRenderPrefix(SubModel)
{
    GLuint vao;
    TsRenderPrefix(VertexDataFormat) vertex_format;
    int vertex_count;
    GLuint vertex_buffer;
    int index_count;
    GLuint index_buffer;
    TsRenderPrefix(Material) *material;
    BoundingBox model_space_bounding_box;
};

struct TsRenderPrefix(Model)
{
    int sub_model_count;
    TsRenderPrefix(SubModel) *sub_models;
    BoundingBox model_space_bounding_box;
    int skeleton_count;
    TsRenderPrefix(Skeleton) *skeletons;
};

typedef struct TsOpenGLShaderInput TsOpenGLShaderInput;
struct TsOpenGLShaderInput
{
    int index;
    char *name;
};

typedef struct TsOpenGLShaderOutput TsOpenGLShaderOutput;
struct TsOpenGLShaderOutput
{
    int index;
    char *name;
};

#define TS_OPENGL_FBO_COLOR_OUT_0      (1<<0)
#define TS_OPENGL_FBO_COLOR_OUT_1      (1<<1)
#define TS_OPENGL_FBO_COLOR_OUT_2      (1<<2)
#define TS_OPENGL_FBO_COLOR_OUT_3      (1<<3)
#define TS_OPENGL_FBO_DEPTH_OUT        (1<<4)
#define TS_OPENGL_FBO_COLOR_OUT_0_16F  (1<<5)
#define TS_OPENGL_FBO_COLOR_OUT_1_16F  (1<<6)
#define TS_OPENGL_FBO_COLOR_OUT_2_16F  (1<<7)
#define TS_OPENGL_FBO_COLOR_OUT_3_16F  (1<<8)
#define TS_OPENGL_FBO_COLOR_OUT_0_32F  (1<<9)
#define TS_OPENGL_FBO_COLOR_OUT_1_32F  (1<<10)
#define TS_OPENGL_FBO_COLOR_OUT_2_32F  (1<<11)
#define TS_OPENGL_FBO_COLOR_OUT_3_32F  (1<<12)

typedef struct TsOpenGLFBO TsOpenGLFBO;
struct TsOpenGLFBO
{
    int flags;
    GLuint fbo;
    GLuint color_textures[4];
    GLuint depth_texture;
    
    union
    {
        struct
        {
            int w;
            int h;
        };
        struct
        {
            int width;
            int height;
        };
    };
};

#define GLProc(type, name) PFNGL##type##PROC gl##name;
#include "tsopenglcommon_procedure_list.inc"

internal void
TsOpenGLLoadAllOpenGLProcedures(void)
{
#define GLProc(type, name) gl##name = platform->LoadOpenGLProcedure("gl" #name);
#include "tsopenglcommon_procedure_list.inc"
}
