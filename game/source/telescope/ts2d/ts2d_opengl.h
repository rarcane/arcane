/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#include <gl/gl.h>
#include "common/ext/wglext.h"
#include "common/ext/glext.h"

#define TS2D_ADDITIVE_BLEND  (1<<0)

#include "common/tsopenglcommon.h"
#include "generated/generated_shaders.h"

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
    TS2D_REQUEST_debug_skeleton,
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
};

#define MAX_GROUND_VERTICES 64

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
        i32 flags;
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
    
#define Ts2dInstanceType(name, size_per_instance, max)     \
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
        TsOpenGLFBO *active_fbo;
        TsOpenGLFBO pre_lighting_fbo_8u;
        TsOpenGLFBO post_lighting_fbo_16f;
        TsOpenGLFBO pre_composite_fbo_8u;
        TsOpenGLFBO world_tile_fbo_8u;
        TsOpenGLFBO world_reflection_fbo_8u;
        TsOpenGLFBO screen_size_scratch_fbo_1_8u;
        TsOpenGLFBO screen_size_scratch_fbo_2_8u;
        TsOpenGLFBO screen_size_scratch_fbo_3_8u;
        TsOpenGLFBO half_screen_size_scratch_fbo_1_8u;
        TsOpenGLFBO half_screen_size_scratch_fbo_2_8u;
        
        TsOpenGLFBO model_sprite_fbo;
    };
    
    // NOTE(rjf): Shaders
    GLuint *shaders;
	
	// NOTE(randy): Ground shader info
	v2 ground_vertices[MAX_GROUND_VERTICES];
	i32 ground_vertex_count;
	float ground_scale;
	float ground_vor_step;
	float ground_band_height;
};
