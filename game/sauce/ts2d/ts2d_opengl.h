/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

// TODO(rjf): We should super break this out so we don't need to remember
// it's here when porting.
#if TS2D_WIN32
#include "tsfoundation/tsfoundation_windows.h"
#include <gl/gl.h>
#include "ext/wglext.h"
#elif TS2D_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <time.h>
#include <GL/glx.h>
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
#else
#error "TS2D: OpenGL includes for platform not provided."
#endif
#include "ext/glext.h"

#define TS2D_ADDITIVE_BLEND  (1<<0)

struct Ts2dTexture
{
    GLuint id;
    int width;
    int height;
    Ts2dTextureFormat format;
    Ts2dTextureFlags flags;
};

struct Ts2dFont
{
    Ts2dTexture texture;
    i16 size;
    i16 line_height;
    u32 glyph_count;
    Ts2dFontGlyph *glyphs;
    u32 glyph_lower_bound_character;
};

struct Ts2dMaterial
{
    v4 backing_color;
    Ts2dTexture *albedo_texture;
};

struct Ts2dSubModel
{
    GLuint vao;
    Ts2dVertexDataFormat vertex_format;
    int vertex_count;
    GLuint vertex_buffer;
    int index_count;
    GLuint index_buffer;
    Ts2dMaterial *material;
    BoundingBox model_space_bounding_box;
};

struct Ts2dModel
{
    int sub_model_count;
    Ts2dSubModel *sub_models;
    BoundingBox model_space_bounding_box;
};

typedef struct Ts2dOpenGLShaderInput Ts2dOpenGLShaderInput;
struct Ts2dOpenGLShaderInput
{
    int index;
    char *name;
};

typedef struct Ts2dOpenGLShaderOutput Ts2dOpenGLShaderOutput;
struct Ts2dOpenGLShaderOutput
{
    int index;
    char *name;
};

#include "generated/generated_shaders.h"

#define TS2D_OPENGL_FBO_COLOR_OUT_0      (1<<0)
#define TS2D_OPENGL_FBO_COLOR_OUT_1      (1<<1)
#define TS2D_OPENGL_FBO_COLOR_OUT_2      (1<<2)
#define TS2D_OPENGL_FBO_COLOR_OUT_3      (1<<3)
#define TS2D_OPENGL_FBO_DEPTH_OUT        (1<<4)
#define TS2D_OPENGL_FBO_COLOR_OUT_0_16F  (1<<5)
#define TS2D_OPENGL_FBO_COLOR_OUT_1_16F  (1<<6)
#define TS2D_OPENGL_FBO_COLOR_OUT_2_16F  (1<<7)
#define TS2D_OPENGL_FBO_COLOR_OUT_3_16F  (1<<8)
#define TS2D_OPENGL_FBO_COLOR_OUT_0_32F  (1<<9)
#define TS2D_OPENGL_FBO_COLOR_OUT_1_32F  (1<<10)
#define TS2D_OPENGL_FBO_COLOR_OUT_2_32F  (1<<11)
#define TS2D_OPENGL_FBO_COLOR_OUT_3_32F  (1<<12)

typedef struct Ts2dOpenGLFBO Ts2dOpenGLFBO;
struct Ts2dOpenGLFBO
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

typedef enum Ts2dRequestType Ts2dRequestType;
enum Ts2dRequestType
{
    TS2D_REQUEST_null,
    
    TS2D_REQUEST_set_clip,
    TS2D_REQUEST_begin_world,
    TS2D_REQUEST_begin_background,
    TS2D_REQUEST_end_world,
    TS2D_REQUEST_end_background,
    TS2D_REQUEST_line,
    TS2D_REQUEST_rect,
    TS2D_REQUEST_filled_rect,
    TS2D_REQUEST_texture,
    TS2D_REQUEST_world_tile,
    TS2D_REQUEST_text,
    TS2D_REQUEST_blur_rectangle,
    TS2D_REQUEST_reflective_rect,
    TS2D_REQUEST_model,
    TS2D_REQUEST_MAX
};

typedef struct Ts2dRequest Ts2dRequest;
struct Ts2dRequest
{
    Ts2dRequestType type;
    unsigned int instance_data_offset;
    unsigned int instance_data_size;
    i32 flags;
    void *data;
    Ts2dRequest *next;
    
#if TS2D_DEBUG
    char *file;
    int line;
#endif
};

typedef struct Ts2d Ts2d;
struct Ts2d
{
    TS2D_COMMON_DATA;
    
    // NOTE(rjf): Frame Data
    struct
    {
        f32 render_width;
        f32 render_height;
        f32 delta_t;
        int flags;
        f32 shadow_opacity;
        v2 shadow_vector;
        v2 camera_pos;
        f32 camera_rotation;
        f32 camera_zoom;
        f32 brightness;
        f32 grayscale;
    };
    
    // NOTE(rjf): All-purpose VAO
    struct
    {
        GLuint all_purpose_vao;
    };
    
#define Ts2dInstanceType(name, size_per_instance, max) \
struct                                                 \
{                                                      \
GLuint vao;                                        \
GLuint instance_buffer;                            \
unsigned int instance_data_alloc_pos;              \
unsigned int instance_data_max;                    \
unsigned int instance_data_stride;                 \
GLubyte *instance_data;                            \
}                                                      \
name;
#include "ts2d_opengl_instance_type_list.inc"
    
    // NOTE(rjf): Request data
    struct
    {
        char *request_memory;
        unsigned int request_memory_max;
        unsigned int request_memory_alloc_position;
    };
    
    // NOTE(rjf): Light data
    struct
    {
        v2 position;
        v3 color;
        f32 radius;
        f32 intensity;
    }
    lights[TS2D_MAX_LIGHT_COUNT];
    u32 light_count;
    
    // NOTE(rjf): Clipping stack
    struct
    {
        u32 clip_stack_size;
        u32 clip_stack_max;
#define TS2D_CLIP_STACK_MAX 16
        v4 clip_stack[TS2D_CLIP_STACK_MAX];
        v4 current_clip;
    };
    
    // NOTE(rjf): FBO data
    struct
    {
        Ts2dOpenGLFBO *active_fbo;
        Ts2dOpenGLFBO pre_lighting_fbo_8u;
        Ts2dOpenGLFBO post_lighting_fbo_16f;
        Ts2dOpenGLFBO pre_composite_fbo_8u;
        Ts2dOpenGLFBO world_tile_fbo_8u;
        Ts2dOpenGLFBO world_reflection_fbo_8u;
        Ts2dOpenGLFBO screen_size_scratch_fbo_1_8u;
        Ts2dOpenGLFBO screen_size_scratch_fbo_2_8u;
        Ts2dOpenGLFBO screen_size_scratch_fbo_3_8u;
        Ts2dOpenGLFBO half_screen_size_scratch_fbo_1_8u;
        Ts2dOpenGLFBO half_screen_size_scratch_fbo_2_8u;
        
        Ts2dOpenGLFBO model_sprite_fbo;
    };
    
    // NOTE(rjf): Shaders
    GLuint *shaders;
    
    // NOTE(rjf): Platform-specific data
#if TS2D_WIN32
    HDC win32_device_context;
    HGLRC win32_opengl_context;
#else
#error "TS2D [OpenGL]: Platform-specific data not implemented for platform."
#endif
    void (*SwapBuffers)(void);
    void *(*LoadOpenGLProcedure)(char *name);
};

#define GLProc(type, name) PFNGL##type##PROC gl##name;
#include "ts2d_opengl_procedure_list.inc"

internal void
Ts2dLoadAllOpenGLProcedures(Ts2d *renderer)
{
#define GLProc(type, name) gl##name = renderer->LoadOpenGLProcedure("gl" #name);
#include "ts2d_opengl_procedure_list.inc"
}