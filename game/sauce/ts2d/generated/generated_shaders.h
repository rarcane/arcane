typedef enum Ts2dOpenGLShaderType Ts2dOpenGLShaderType;
enum Ts2dOpenGLShaderType
{
TS2D_OPENGL_SHADER_background,
TS2D_OPENGL_SHADER_bloom_filter,
TS2D_OPENGL_SHADER_fbo,
TS2D_OPENGL_SHADER_fbo_reflection,
TS2D_OPENGL_SHADER_filled_rect,
TS2D_OPENGL_SHADER_gaussian_blur,
TS2D_OPENGL_SHADER_line,
TS2D_OPENGL_SHADER_model_sprite,
TS2D_OPENGL_SHADER_post_process,
TS2D_OPENGL_SHADER_rect,
TS2D_OPENGL_SHADER_reflection,
TS2D_OPENGL_SHADER_reflective_rect,
TS2D_OPENGL_SHADER_shadow,
TS2D_OPENGL_SHADER_text,
TS2D_OPENGL_SHADER_texture,
TS2D_OPENGL_SHADER_texture_reflection,
TS2D_OPENGL_SHADER_world,
TS2D_OPENGL_SHADER_world_tile,
TS2D_OPENGL_SHADER_MAX,
};

#define TS2D_OPENGL_SHADER_INPUT_MAX 16
#define TS2D_OPENGL_SHADER_OUTPUT_MAX 16
global struct
{
char *name;
Ts2dOpenGLShaderInput inputs[TS2D_OPENGL_SHADER_INPUT_MAX];
u32 input_count;
Ts2dOpenGLShaderOutput outputs[TS2D_OPENGL_SHADER_OUTPUT_MAX];
u32 output_count;
char *vert;
char *frag;
}
_ts2d__global_opengl_shaders[] = {
{
"Background",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position.xy = screen_position.xy * 2 - 1;\n"
"    frag_uv = vert_position;\n"
"    gl_Position = screen_position;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"\n"
"void main()\n"
"{\n"
"    color = texture(tex, frag_uv);\n"
"    color.xyz /= color.a;\n"
"    if(color.a <= 0)\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"\n"
"",
},
{
"Bloom Filter",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    \n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position.xy = screen_position.xy * 2 - 1;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform sampler2D foreground_tex;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 foreground_color = texture(foreground_tex, frag_uv);\n"
"    color = texture(tex, frag_uv);\n"
"    float brightness = (0.299*color.r + 0.587*color.g + 0.114*color.b);\n"
"    if(foreground_color.a < 0.1)\n"
"    {\n"
"        brightness -= 0.4;\n"
"    }\n"
"    color *= brightness * 1.4;\n"
"}\n"
"",
},
{
"FBO",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"\n"
"out vec2 frag_uv;\n"
"out vec2 frag_position;\n"
"uniform vec4 destination;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    \n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    frag_position = screen_position.xy;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"\n"
"",
"#version 330 core\n"
"\n"
"\n"
"in vec2 frag_uv;\n"
"in vec2 frag_position;\n"
"out vec4 color;\n"
"uniform vec2 uv_offset;\n"
"uniform vec2 uv_range;\n"
"uniform vec4 destination;\n"
"uniform vec2 scale;\n"
"uniform float opacity;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 pixel = (uv_offset + (frag_uv * uv_range));\n"
"    vec2 sample_uv = floor(pixel) + vec2(0.5, 0.5);\n"
"    \n"
"    sample_uv.x += 1.0 - clamp((1.0 - fract(pixel.x)) * scale.x, 0.0, 1.0);\n"
"    sample_uv.y += 1.0 - clamp((1.0 - fract(pixel.y)) * scale.y, 0.0, 1.0);\n"
"    \n"
"    color = texture(tex, sample_uv / tex_resolution);\n"
"    color.xyz /= color.a;\n"
"    if(color.a > 0)\n"
"    {\n"
"        color *= opacity;\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"\n"
"",
},
{
"Reflection FBO",
{
0},
0,
{
{ 1, "reflection", },
{ 0, "color", },
},
2,
"#version 330 core\n"
"\n"
"\n"
"out vec2 frag_uv;\n"
"out vec2 frag_position;\n"
"uniform vec4 destination;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    \n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    frag_position = screen_position.xy;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"\n"
"",
"#version 330 core\n"
"\n"
"\n"
"in vec2 frag_uv;\n"
"in vec2 frag_position;\n"
"out vec4 color;\n"
"out vec4 reflection;\n"
"uniform vec2 uv_offset;\n"
"uniform vec2 uv_range;\n"
"uniform vec4 destination;\n"
"uniform vec2 scale;\n"
"uniform float opacity;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 pixel = (uv_offset + (frag_uv * uv_range));\n"
"    vec2 sample_uv = floor(pixel) + vec2(0.5, 0.5);\n"
"    \n"
"    sample_uv.x += 1.0 - clamp((1.0 - fract(pixel.x)) * scale.x, 0.0, 1.0);\n"
"    sample_uv.y += 1.0 - clamp((1.0 - fract(pixel.y)) * scale.y, 0.0, 1.0);\n"
"    \n"
"    color = texture(tex, sample_uv / tex_resolution);\n"
"    color.xyz /= color.a;\n"
"    if(color.a > 0)\n"
"    {\n"
"        color *= opacity;\n"
"        reflection = vec4(0, 0, 0, color.a);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"\n"
"",
},
{
"Filled Rect Batch",
{
{ 4, "vert_color11", },
{ 3, "vert_color10", },
{ 2, "vert_color01", },
{ 1, "vert_color00", },
{ 0, "vert_rect_data", },
},
5,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"\n"
"in vec4 vert_rect_data;\n"
"in vec4 vert_color00;\n"
"in vec4 vert_color01;\n"
"in vec4 vert_color10;\n"
"in vec4 vert_color11;\n"
"out vec4 rect_color;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    vec4 destination = vert_rect_data;\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    gl_Position = screen_position;\n"
"    \n"
"    vec4 colors[] = vec4[](vert_color00, vert_color01, vert_color10, vert_color11);\n"
"    \n"
"    rect_color = colors[gl_VertexID];\n"
"}\n"
"\n"
"",
"#version 330 core\n"
"\n"
"\n"
"in vec4 rect_color;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = rect_color;\n"
"}\n"
"\n"
"",
},
{
"Gaussian Blur",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position = 2 * screen_position - 1;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position;\n"
"}\n"
"\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"uniform int radius;\n"
"uniform vec4 kernel[32];\n"
"uniform int vertical;\n"
"uniform vec4 clip;\n"
"\n"
"void main()\n"
"{\n"
"    color = vec4(0, 0, 0, 0);\n"
"    \n"
"    if(gl_FragCoord.x >= clip.x && gl_FragCoord.x <= clip.x + clip.z &&\n"
"       gl_FragCoord.y >= clip.y && gl_FragCoord.y <= clip.y + clip.w)\n"
"    {\n"
"        int first_kernel_index = (16 - radius/4);\n"
"        \n"
"        for(int i = 0; i < 2*radius/4; ++i)\n"
"        {\n"
"            if(vertical != 0)\n"
"            {\n"
"                color += texture(tex, frag_uv + vec2(0, -radius + i*4 + 0) / tex_resolution) * kernel[first_kernel_index + i].x;\n"
"                color += texture(tex, frag_uv + vec2(0, -radius + i*4 + 1) / tex_resolution) * kernel[first_kernel_index + i].y;\n"
"                color += texture(tex, frag_uv + vec2(0, -radius + i*4 + 2) / tex_resolution) * kernel[first_kernel_index + i].z;\n"
"                color += texture(tex, frag_uv + vec2(0, -radius + i*4 + 3) / tex_resolution) * kernel[first_kernel_index + i].w;\n"
"            }\n"
"            else\n"
"            {\n"
"                color += texture(tex, frag_uv + vec2(-radius + i*4 + 0, 0) / tex_resolution) * kernel[first_kernel_index + i].x;\n"
"                color += texture(tex, frag_uv + vec2(-radius + i*4 + 1, 0) / tex_resolution) * kernel[first_kernel_index + i].y;\n"
"                color += texture(tex, frag_uv + vec2(-radius + i*4 + 2, 0) / tex_resolution) * kernel[first_kernel_index + i].z;\n"
"                color += texture(tex, frag_uv + vec2(-radius + i*4 + 3, 0) / tex_resolution) * kernel[first_kernel_index + i].w;\n"
"            }\n"
"        }\n"
"    }\n"
"}\n"
"\n"
"",
},
{
"Line Batch",
{
{ 2, "vert_color_data", },
{ 1, "vert_2", },
{ 0, "vert_1", },
},
3,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"in vec2 vert_1;\n"
"in vec2 vert_2;\n"
"in vec4 vert_color;\n"
"out vec4 frag_color_data;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](vert_1, vert_2);\n"
"    frag_color_data = vert_color;\n"
"    vec4 screen_position = vec4(vertices[gl_VertexID], 0, 1);\n"
"    gl_Position = screen_position;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec4 frag_color_data;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = frag_color_data;\n"
"}\n"
"",
},
{
"Model Sprite",
{
{ 2, "vert_normal", },
{ 1, "vert_uv", },
{ 0, "vert_position", },
},
3,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"\n"
"in vec3 vert_position;\n"
"in vec2 vert_uv;\n"
"in vec3 vert_normal;\n"
"\n"
"uniform mat3 model_transform;\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    vec3 vertex_position = model_transform * vert_position;\n"
"    vec4 world_space_position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1);\n"
"    vec4 clip_space_position = view_projection * world_space_position;\n"
"    gl_Position = clip_space_position;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = vec4(1, 0, 0, 1);\n"
"}\n"
"",
},
{
"Post Process",
{
{ 0, "vert_position", },
},
1,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position.xy = screen_position.xy * 2 - 1;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform float grayscale;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 frag_color = texture(tex, frag_uv);\n"
"    frag_color.xyz /= frag_color.a;\n"
"    \n"
"    float brightness = (0.299*frag_color.r + 0.587*frag_color.g + 0.114*frag_color.b);\n"
"    frag_color.r += (brightness - frag_color.r) * grayscale;\n"
"    frag_color.g += (brightness - frag_color.g) * grayscale;\n"
"    frag_color.b += (brightness - frag_color.b) * grayscale;\n"
"    \n"
"    if(frag_color.a > 0)\n"
"    {\n"
"        // const float gamma = 1.8;\n"
"        // vec3 mapped = frag_color.rgb / (frag_color.rgb + vec3(1.0));\n"
"        // mapped = pow(mapped, vec3(1.0 / gamma));\n"
"        // frag_color = vec4(mapped, 1.0);\n"
"        \n"
"        color = frag_color;\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"Rectangle Batch",
{
{ 1, "vert_color_data", },
{ 0, "vert_rect_data", },
},
2,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"in vec4 vert_rect_data;\n"
"in vec4 vert_color_data;\n"
"out vec4 frag_color_data;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 1),\n"
"        vec2(1, 0)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    vec4 destination = vert_rect_data;\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    gl_Position = screen_position;\n"
"    frag_color_data = vert_color_data;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec4 frag_color_data;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = frag_color_data;\n"
"}\n"
"",
},
{
"Reflection Pass",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"void main()\n"
"{\n"
"    vec4 vertices[] = vec4[](vec4(0, 0, 0, 1),\n"
"                             vec4(0, 1, 0, 1),\n"
"                             vec4(1, 0, 0, 1),\n"
"                             vec4(1, 1, 0, 1));\n"
"    frag_uv = vertices[gl_VertexID].xy;\n"
"    gl_Position = vertices[gl_VertexID] * 2 - 1;\n"
"}\n"
"\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform sampler2D reflection_color_tex;\n"
"uniform sampler2D reflection_data_tex;\n"
"uniform vec2 render_resolution;\n"
"uniform float wave_position;\n"
"void main()\n"
"{\n"
"    vec4 reflection_color = texture(reflection_color_tex, frag_uv);\n"
"    float reflection_amount = reflection_color.a;\n"
"    if(reflection_amount > 0.01)\n"
"    {\n"
"        vec4 reflection_data = texture(reflection_data_tex, frag_uv);\n"
"        float distortion_amount = reflection_data.y;\n"
"        float distortion_by_time_factor = reflection_data.z;\n"
"        vec4 reflection = vec4(0, 0, 0, 0);\n"
"        \n"
"        float kernel[33] = float[](0.000035, 0.000091, 0.000225, 0.00052,\n"
"                                   0.001131, 0.002312, 0.004442, 0.008019,\n"
"                                   0.013604, 0.021686, 0.032486, 0.04573,\n"
"                                   0.060495, 0.075201, 0.087848, 0.096435,\n"
"                                   0.09948, 0.096435, 0.087848, 0.075201,\n"
"                                   0.060495, 0.04573, 0.032486, 0.021686,\n"
"                                   0.013604, 0.008019, 0.004442, 0.002312,\n"
"                                   0.001131, 0.00052, 0.000225, 0.000091,\n"
"                                   0.000035);\n"
"        \n"
"        float adjusted_wave_position = wave_position * distortion_by_time_factor;\n"
"        vec2 sample_offset;\n"
"        for(int i = 0; i < 33; ++i)\n"
"        {\n"
"            sample_offset.y = (float(i) - 16.5) / render_resolution.y;\n"
"            sample_offset.x = 6.f * sin(adjusted_wave_position + float(i)*0.5) / render_resolution.x;\n"
"            reflection += texture(tex, vec2(frag_uv.x + sample_offset.x, frag_uv.y + reflection_data.x * 2 + sample_offset.y)) * kernel[i];\n"
"        }\n"
"        \n"
"        float t_reflection = reflection_data.x * render_resolution.y / 512.0;\n"
"        color = (1 - t_reflection) * reflection + t_reflection * reflection_color;\n"
"        color *= reflection_amount;\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"\n"
"",
},
{
"Reflective Rectangle Batch",
{
{ 2, "vert_reflection_data", },
{ 1, "vert_color_data", },
{ 0, "vert_rect_data", },
},
3,
{
{ 1, "reflection", },
{ 0, "color", },
},
2,
"#version 330 core\n"
"\n"
"in vec4 vert_rect_data;\n"
"in vec4 vert_color_data;\n"
"in vec4 vert_reflection_data;\n"
"out float frag_clip_space_y;\n"
"out float frag_rect_top_clip_space_y;\n"
"out vec4 frag_color_data;\n"
"out vec4 frag_reflection_data;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    \n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    vec4 destination = vert_rect_data;\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    gl_Position = screen_position;\n"
"    frag_clip_space_y = -gl_Position.y;\n"
"    frag_rect_top_clip_space_y = -destination.y;\n"
"    frag_color_data = vert_color_data;\n"
"    frag_reflection_data = vert_reflection_data;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in float frag_clip_space_y;\n"
"in float frag_rect_top_clip_space_y;\n"
"in vec4 frag_color_data;\n"
"in vec4 frag_reflection_data;\n"
"out vec4 color;\n"
"out vec4 reflection;\n"
"void main()\n"
"{\n"
"    float distance_from_water_top = frag_clip_space_y - frag_rect_top_clip_space_y;\n"
"    color = frag_color_data;\n"
"    reflection.x = distance_from_water_top / 2;\n"
"    reflection.y = frag_reflection_data.x;\n"
"    reflection.z = frag_reflection_data.y;\n"
"    reflection.w = 1;\n"
"}\n"
"",
},
{
"Shadow",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"out vec2 frag_position;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    screen_position = screen_position * 2 - 1;\n"
"    frag_position = screen_position.xy;\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"in vec2 frag_position;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"uniform vec2 light_vector;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 pixel = frag_uv * tex_resolution;\n"
"    vec2 sample_uv = pixel;\n"
"    vec4 frag_color = texture(tex, frag_uv);\n"
"    \n"
"    float shadow_value = 0;\n"
"    if(frag_color.a > 0)\n"
"    {\n"
"        for(int i = 0; i < 16; ++i)\n"
"        {\n"
"            vec2 sample_pos = (sample_uv + light_vector*(float(i) / 16.0));\n"
"            vec4 lookup_color = texture(tex, sample_pos / tex_resolution);\n"
"            float distance = length(sample_pos - sample_uv);\n"
"            shadow_value += 1.0 * (1 - float(i) / 16.0)*frag_color.a*(lookup_color.a/28.0);\n"
"        }\n"
"    }\n"
"    \n"
"    if(shadow_value > 0)\n"
"    {\n"
"        color = vec4(shadow_value, 0, 0, shadow_value);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"Text",
{
{ 3, "vert_style_data", },
{ 2, "vert_color_data", },
{ 1, "vert_dest_data", },
{ 0, "vert_source_data", },
},
4,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"in vec4 vert_source_data;\n"
"in vec4 vert_dest_data;\n"
"in vec4 vert_color_data;\n"
"in vec4 vert_style_data;\n"
"out vec2 frag_uv;\n"
"out vec4 frag_color_data;\n"
"out vec4 frag_style_data;\n"
"uniform vec2 tex_resolution;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1));\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    \n"
"    vec4 source = vert_source_data;\n"
"    vec4 dest = vert_dest_data;\n"
"    vec4 color = vert_color_data;\n"
"    vec4 style = vert_style_data;\n"
"    \n"
"    screen_position.xy *= dest.zw;\n"
"    screen_position.xy += dest.xy;\n"
"    frag_color_data = color;\n"
"    frag_style_data = style;\n"
"    gl_Position = screen_position;\n"
"	frag_uv = vert_position;\n"
"    frag_uv *= source.zw / tex_resolution;\n"
"    frag_uv += source.xy / tex_resolution;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"in vec4 frag_color_data;\n"
"in vec4 frag_style_data;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"void main()\n"
"{\n"
"    float boldness = frag_style_data.x;\n"
"    float softness = frag_style_data.y;\n"
"    vec4 text_color = frag_color_data;\n"
"    float distance = texture(tex, frag_uv).a;\n"
"    float smooth_step = smoothstep(1.0 - boldness, (1.0 - boldness) + softness, distance);\n"
"    color = text_color * smooth_step;\n"
"    color.xyz /= color.a;\n"
"    if(color.a < 0.02)\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"Texture",
{
{ 2, "vert_tint", },
{ 1, "vert_dest", },
{ 0, "vert_source", },
},
3,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"in vec4 vert_source;\n"
"in vec4 vert_dest;\n"
"in vec4 vert_tint;\n"
"out vec2 frag_uv;\n"
"out vec4 frag_source;\n"
"out vec2 frag_scale;\n"
"out vec4 frag_tint;\n"
"uniform vec2 render_resolution;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    vec4 destination;\n"
"    destination.zw = vert_source.zw / render_resolution;\n"
"    vec2 scale = 2 * vert_dest.zw;\n"
"    destination.zw *= scale;\n"
"    destination.xy = vert_dest.xy;\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    gl_Position  = screen_position;\n"
"    frag_uv      = vert_position.xy;\n"
"    frag_uv.y    = 1 - frag_uv.y;\n"
"    frag_source  = vert_source;\n"
"    frag_scale   = vert_dest.zw;\n"
"    frag_tint    = vert_tint;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"in vec4 frag_source;\n"
"in vec2 frag_scale;\n"
"in vec4 frag_tint;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"void main()\n"
"{\n"
"    vec2 uv_offset = frag_source.xy;\n"
"    vec2 uv_range = frag_source.zw;\n"
"    vec4 tint = frag_tint;\n"
"    vec2 scale = frag_scale;\n"
"    \n"
"    vec2 pixel = (uv_offset + (frag_uv * uv_range));\n"
"    vec2 sample_uv = floor(pixel) + vec2(0.5, 0.5);\n"
"    \n"
"    sample_uv.x += 1.0 - clamp((1.0 - fract(pixel.x)) * abs(scale.x), 0.0, 1.0);\n"
"    sample_uv.y += 1.0 - clamp((1.0 - fract(pixel.y)) * abs(scale.y), 0.0, 1.0);\n"
"    \n"
"    color = texture(tex, sample_uv / tex_resolution);\n"
"    \n"
"    if(color.a > 0)\n"
"    {\n"
"        color *= tint;\n"
"        color.xyz /= color.a;\n"
"        color.xyz = clamp(color.xyz, 0, 1);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"Texture (Reflection)",
{
{ 2, "vert_opacity", },
{ 1, "vert_dest", },
{ 0, "vert_source", },
},
3,
{
{ 1, "reflection", },
{ 0, "color", },
},
2,
"#version 330 core\n"
"\n"
"in vec4 vert_source;\n"
"in vec4 vert_dest;\n"
"in float vert_opacity;\n"
"out vec2 frag_uv;\n"
"out vec4 frag_source;\n"
"out vec2 frag_scale;\n"
"out float frag_opacity;\n"
"uniform vec2 render_resolution;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    vec4 destination;\n"
"    destination.zw = vert_source.zw / render_resolution;\n"
"    vec2 scale = 2 * vert_dest.zw;\n"
"    destination.zw *= scale;\n"
"    destination.xy = vert_dest.xy;\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    gl_Position  = screen_position;\n"
"    frag_uv      = vert_position.xy;\n"
"    frag_uv.y    = 1 - frag_uv.y;\n"
"    frag_source  = vert_source;\n"
"    frag_scale   = vert_dest.zw;\n"
"    frag_opacity = vert_opacity;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"in vec4 frag_source;\n"
"in vec2 frag_scale;\n"
"in float frag_opacity;\n"
"out vec4 color;\n"
"out vec4 reflection;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"void main()\n"
"{\n"
"    vec2 uv_offset = frag_source.xy;\n"
"    vec2 uv_range = frag_source.zw;\n"
"    float opacity = frag_opacity;\n"
"    vec2 scale = frag_scale;\n"
"    \n"
"    vec2 pixel = (uv_offset + (frag_uv * uv_range));\n"
"    vec2 sample_uv = floor(pixel) + vec2(0.5, 0.5);\n"
"    \n"
"    sample_uv.x += 1.0 - clamp((1.0 - fract(pixel.x)) * abs(scale.x), 0.0, 1.0);\n"
"    sample_uv.y += 1.0 - clamp((1.0 - fract(pixel.y)) * abs(scale.y), 0.0, 1.0);\n"
"    \n"
"    color = texture(tex, sample_uv / tex_resolution);\n"
"    \n"
"    if(color.a > 0)\n"
"    {\n"
"        color *= opacity;\n"
"        reflection = vec4(0, 0, 0, color.a);\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"World",
{
0},
0,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"out vec2 frag_uv;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] = vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position * 2 - 1, 0, 1);\n"
"    gl_Position = screen_position;\n"
"    frag_uv = vert_position.xy;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"out vec4 color;\n"
"uniform sampler2D foreground_texture;\n"
"uniform sampler2D shadow_texture;\n"
"uniform float brightness;\n"
"uniform float shadow_opacity;\n"
"\n"
"struct Light\n"
"{\n"
"    vec2 position;\n"
"    vec3 color;\n"
"    float radius;\n"
"    float intensity;\n"
"};\n"
"uniform Light lights[16];\n"
"uniform int light_count;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 foreground = texture(foreground_texture, frag_uv);\n"
"    float shadow_value = texture(shadow_texture, frag_uv).a * shadow_opacity;\n"
"    vec2 frag_position = gl_FragCoord.xy;\n"
"    \n"
"    color = foreground;\n"
"    color.xyz /= color.a;\n"
"    vec3 brightness_factor =\n"
"        vec3(0.02 + 0.98*clamp(brightness - shadow_value*2, 0.2, 1)\n"
"             ,   0.03 + 0.97*clamp(brightness - shadow_value*2, 0.2, 1)\n"
"             ,   0.08 + 0.92*clamp(brightness - shadow_value*2, 0.2, 1));\n"
"    vec3 diffuse_factor = brightness_factor;\n"
"    vec3 light_color_sum = vec3(0, 0, 0);\n"
"    \n"
"    // NOTE(rjf): Calculate lighting factor\n"
"    {\n"
"        for(int i = 0; i < light_count && i < 16; ++i)\n"
"        {\n"
"            vec2 diff = frag_position - lights[i].position;\n"
"            float distance2 = (diff.x*diff.x + diff.y*diff.y);\n"
"            float rad2 = lights[i].radius*lights[i].radius;\n"
"            if(distance2 < rad2)\n"
"            {\n"
"                light_color_sum += (lights[i].color * lights[i].intensity * (1 - (distance2) / rad2));\n"
"            }\n"
"        }\n"
"    }\n"
"    \n"
"    if(color.a > 0)\n"
"    {\n"
"        if(light_color_sum.r < diffuse_factor.r) light_color_sum.r = diffuse_factor.r;\n"
"        if(light_color_sum.g < diffuse_factor.g) light_color_sum.g = diffuse_factor.g;\n"
"        if(light_color_sum.b < diffuse_factor.b) light_color_sum.b = diffuse_factor.b;\n"
"        \n"
"        float brightness = (0.299*color.r + 0.587*color.g + 0.114*color.b);\n"
"        float shadow_factor = 0.3 - shadow_value;\n"
"        \n"
"        color.rgb *= light_color_sum * clamp(brightness, 0.75, 1.0);\n"
"        color.r += shadow_opacity * (1    - color.r) * shadow_factor;\n"
"        color.g += shadow_opacity * (0.95 - color.g) * shadow_factor;\n"
"        color.b += shadow_opacity * (0.92 - color.b) * shadow_factor;\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
{
"World Tile Batch",
{
{ 2, "vert_dest", },
{ 1, "vert_source", },
},
2,
{
{ 0, "color", },
},
1,
"#version 330 core\n"
"\n"
"in uvec2 vert_source;\n"
"in uvec2 vert_dest;\n"
"out vec2 frag_uv;\n"
"out vec2 frag_source;\n"
"uniform vec2 render_resolution;\n"
"void main()\n"
"{\n"
"    vec2 vertices[] =\n"
"        vec2[](\n"
"        vec2(0, 0),\n"
"        vec2(0, 1),\n"
"        vec2(1, 0),\n"
"        vec2(1, 1)\n"
"        );\n"
"    vec2 vert_position = vertices[gl_VertexID];\n"
"    vec4 screen_position = vec4(vert_position, 0, 1);\n"
"    frag_uv = vert_position.xy;\n"
"    frag_source = 16.0 * (vec2(float(vert_source.x), float(vert_source.y)) + frag_uv);\n"
"    vec2 dest_position = vec2(16.0 * float(vert_dest.x), 16.0 * float(vert_dest.y));\n"
"    vec4 destination = vec4(dest_position.x, dest_position.y, 16.0, 16.0);\n"
"    screen_position.xy *= destination.zw;\n"
"    screen_position.xy += destination.xy;\n"
"    screen_position.xy = 2 * screen_position.xy / render_resolution - 1;\n"
"    screen_position.y *= -1;\n"
"    gl_Position = screen_position;\n"
"}\n"
"",
"#version 330 core\n"
"\n"
"in vec2 frag_uv;\n"
"in vec2 frag_source;\n"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"uniform vec2 tex_resolution;\n"
"void main()\n"
"{\n"
"    vec2 sample_uv = frag_source;\n"
"    color = texture(tex, sample_uv / tex_resolution);\n"
"    if(color.a > 0)\n"
"    {\n"
"        color.xyz /= color.a;\n"
"    }\n"
"    else\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n"
"",
},
};

