/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

// THIS IS BAD PARKER. PARKER COMMNENTED THIS OUT BECAUSE OPENGL WAS BEING INCLUDED SOMEHOW AND CLOBBERING STUFFZ
#define GLProc(type, name) PFNGL##type##PROC gl##name;
#include "ts2d_opengl_procedure_list.inc"

internal void
Ts2dLoadAllOpenGLProcedures(void)
{
#define GLProc(type, name) gl##name = platform->LoadOpenGLProcedure("gl" #name);
#include "ts2d_opengl_procedure_list.inc"
}

Ts2dTexture
Ts2dTextureInit(Ts2dTextureFormat format, int width, int height, void *data)
{
	return Ts2dTextureInitFlags((Ts2dTextureFlags)0, format, width, height, data);
}

Ts2dTexture
Ts2dTextureInitFlags(Ts2dTextureFlags flags, Ts2dTextureFormat format, int width, int height, void *data)
{
	Ts2dTexture texture = {0};
	texture.flags = flags;
	texture.format = format;
	texture.width = width;
	texture.height = height;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	if (format == TS2D_TEXTURE_FORMAT_R8G8B8A8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		HardAssert("Not implemented" == 0);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void Ts2dTextureCleanUp(Ts2dTexture *texture)
{
	if (texture && texture->id)
	{
		glDeleteTextures(1, &texture->id);
		texture->id = 0;
	}
}

b32 Ts2dTextureIsValid(Ts2dTexture *texture)
{
	return texture->id != 0;
}

void Ts2dTextureSetFlags(Ts2dTexture *texture, Ts2dTextureFlags flags)
{
	texture->flags = flags;
}

Ts2dFont
Ts2dFontInit(Ts2dTextureFormat format, int texture_width, int texture_height,
			 void *texture_data, int size, int line_height, u32 glyph_count,
			 Ts2dFontGlyph *glyphs, u32 glyph_lower_bound_character)
{
	Ts2dFont font = {0};
	font.texture = Ts2dTextureInit(format, texture_width, texture_height, texture_data);
	font.size = (i16)size;
	font.line_height = (i16)line_height;
	font.glyph_count = glyph_count;
	font.glyphs = platform->HeapAlloc(sizeof(Ts2dFontGlyph) * glyph_count);
	MemoryCopy(font.glyphs, glyphs, sizeof(Ts2dFontGlyph) * glyph_count);
	font.glyph_lower_bound_character = glyph_lower_bound_character;
	return font;
}

void Ts2dFontCleanUp(Ts2dFont *font)
{
	if (font)
	{
		Ts2dTextureCleanUp(&font->texture);
		platform->HeapFree(font->glyphs);
	}
}

f32 Ts2dFontGetLineHeight(Ts2dFont *font)
{
	return font ? (f32)font->line_height : 0;
}

f32 Ts2dFontGetTextWidthN(Ts2dFont *font, char *text, u32 n)
{
	f32 text_width = 0.f;
	if (font)
	{
		for (u32 i = 0; text[i] && i < n; ++i)
		{
			if ((u32)text[i] >= font->glyph_lower_bound_character && (u32)text[i] < font->glyph_lower_bound_character + font->glyph_count)
			{
				text_width += (f32)font->glyphs[text[i] - font->glyph_lower_bound_character].x_advance;
			}
		}
	}
	return text_width;
}

f32 Ts2dFontGetTextWidth(Ts2dFont *font, char *text)
{
	return Ts2dFontGetTextWidthN(font, text, (u32)-1);
}

Ts2dMaterial
Ts2dMaterialInit(Ts2dTexture *albedo)
{
	Ts2dMaterial material = {0};
	material.albedo_texture = albedo;
	return material;
}

Ts2dMaterial
Ts2dMaterialInitSimple(v3 color)
{
	Ts2dMaterial material = {0};
	material.backing_color = v4(color.r, color.g, color.b, 1);
	return material;
}

void Ts2dMaterialCleanUp(Ts2dMaterial *material)
{
	if (material && material->albedo_texture)
	{
		Ts2dTextureCleanUp(material->albedo_texture);
	}
}

Ts2dSubModel
Ts2dSubModelInit(Ts2dVertexDataFormat format, int vertex_count, f32 *vertex_data, int index_count, i32 *index_data, Ts2dMaterial *material)
{
	Ts2dSubModel sub_model = {0};
	sub_model.vertex_format = format;
	glGenVertexArrays(1, &sub_model.vao);
	glBindVertexArray(sub_model.vao);
	{
		int bytes_per_vertex = Ts2dGetBytesPerVertexWithFormat(format);

		sub_model.vertex_count = vertex_count;
		glGenBuffers(1, &sub_model.vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, sub_model.vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * bytes_per_vertex, vertex_data, GL_STATIC_DRAW);

		if (index_data)
		{
			sub_model.index_count = index_count;
			glGenBuffers(1, &sub_model.index_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_model.index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(index_data[0]), index_data, GL_STATIC_DRAW);
		}

		sub_model.material = material;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// NOTE(rjf): Calculate bounding box of this sub-model.
	// TODO(rjf): Is this a problem?
	if (format & TS2D_VERTEX_POSITION)
	{
		int floats_per_vertex = Ts2dGetFloatsPerVertexWithFormat(format);
		for (int i = 0; i < vertex_count; ++i)
		{
			v3 vertex =
				{
					vertex_data[i * floats_per_vertex + 0],
					vertex_data[i * floats_per_vertex + 1],
					vertex_data[i * floats_per_vertex + 2],
				};

			if (i == 0 || vertex.x < sub_model.model_space_bounding_box.min.x)
			{
				sub_model.model_space_bounding_box.min.x = vertex.x;
			}

			if (i == 0 || vertex.y < sub_model.model_space_bounding_box.min.y)
			{
				sub_model.model_space_bounding_box.min.y = vertex.y;
			}

			if (i == 0 || vertex.z < sub_model.model_space_bounding_box.min.z)
			{
				sub_model.model_space_bounding_box.min.z = vertex.z;
			}

			if (i == 0 || vertex.x > sub_model.model_space_bounding_box.max.x)
			{
				sub_model.model_space_bounding_box.max.x = vertex.x;
			}

			if (i == 0 || vertex.y > sub_model.model_space_bounding_box.max.y)
			{
				sub_model.model_space_bounding_box.max.y = vertex.y;
			}

			if (i == 0 || vertex.z > sub_model.model_space_bounding_box.max.z)
			{
				sub_model.model_space_bounding_box.max.z = vertex.z;
			}
		}
	}

	return sub_model;
}

Ts2dSubModel
Ts2dSubModelInitSimple(Ts2dVertexDataFormat format, int vertex_count, f32 *vertex_data, Ts2dMaterial *material)
{
	return Ts2dSubModelInit(format, vertex_count, vertex_data, 0, 0, material);
}

Ts2dModel
Ts2dModelInit(int sub_model_count, Ts2dSubModel *sub_models, int skeleton_count, Ts2dSkeleton *skeletons)
{
	Ts2dModel model = {0};
	model.sub_model_count = sub_model_count;
	model.sub_models = platform->HeapAlloc(sizeof(*sub_models) * sub_model_count);
	model.skeleton_count = skeleton_count;
	if (skeleton_count > 0)
	{
		model.skeletons = platform->HeapAlloc(sizeof(Ts2dSkeleton) * skeleton_count);
	}

	HardAssert(model.sub_models != 0);
	if (model.sub_models)
	{
		MemoryCopy(model.sub_models, sub_models, sizeof(*sub_models) * sub_model_count);

		// NOTE(rjf): Calculate bounding box.
		// TODO(rjf): Is this a problem?
		{
			v3 min = {0};
			v3 max = {0};

			for (int i = 0; i < sub_model_count; ++i)
			{

				if (i == 0 || min.x < sub_models[i].model_space_bounding_box.min.x)
				{
					min.x = sub_models[i].model_space_bounding_box.min.x;
				}

				if (i == 0 || min.y < sub_models[i].model_space_bounding_box.min.y)
				{
					min.y = sub_models[i].model_space_bounding_box.min.y;
				}

				if (i == 0 || min.z < sub_models[i].model_space_bounding_box.min.z)
				{
					min.z = sub_models[i].model_space_bounding_box.min.z;
				}

				if (i == 0 || max.x > sub_models[i].model_space_bounding_box.max.x)
				{
					max.x = sub_models[i].model_space_bounding_box.max.x;
				}

				if (i == 0 || max.y > sub_models[i].model_space_bounding_box.max.y)
				{
					max.y = sub_models[i].model_space_bounding_box.max.y;
				}

				if (i == 0 || max.z > sub_models[i].model_space_bounding_box.max.z)
				{
					max.z = sub_models[i].model_space_bounding_box.max.z;
				}
			}

			model.model_space_bounding_box.min = min;
			model.model_space_bounding_box.max = max;
		}
	}

	if (model.skeletons)
	{
		MemoryCopy(model.skeletons, skeletons, sizeof(*skeletons) * skeleton_count);
	}

	return model;
}

void Ts2dSubModelCleanUp(Ts2dSubModel *sub_model)
{
	if (sub_model->vao)
	{
		glDeleteVertexArrays(1, &sub_model->vao);
		sub_model->vao = 0;
	}

	if (sub_model->vertex_buffer)
	{
		glDeleteBuffers(1, &sub_model->vertex_buffer);
		sub_model->vertex_buffer = 0;
	}

	if (sub_model->index_buffer)
	{
		glDeleteBuffers(1, &sub_model->index_buffer);
		sub_model->index_buffer = 0;
	}

	Ts2dMaterialCleanUp(sub_model->material);
}

void Ts2dModelCleanUp(Ts2dModel *model)
{
	for (int i = 0; i < model->sub_model_count; ++i)
	{
		Ts2dSubModelCleanUp(&model->sub_models[i]);
	}

	model->sub_model_count = 0;
	if (model->sub_models)
	{
		platform->HeapFree(model->sub_models);
		model->sub_models = 0;
	}

	model->skeleton_count = 0;
	if (model->skeletons)
	{
		platform->HeapFree(model->skeletons);
		model->skeletons = 0;
	}
}

typedef struct Ts2dBlurRequestData Ts2dBlurRequestData;
struct Ts2dBlurRequestData
{
	f32 blur_magnitude;
	v4 clip;
};

typedef struct Ts2dTextureRequestData Ts2dTextureRequestData;
struct Ts2dTextureRequestData
{
	Ts2dTexture *texture;
};

typedef struct Ts2dModelRequestData Ts2dModelRequestData;
struct Ts2dModelRequestData
{
	v2 position;
	v2 size;
	Ts2dModel *model;
	b32 transform_with_skeleton;
	Ts2dSkeleton skeleton;
	m3 transform;
	float pixel_scale;
};

typedef struct Ts2dSkeletonRequestData Ts2dSkeletonRequestData;
struct Ts2dSkeletonRequestData
{
	v2 position;
	v2 size;
	Ts2dSkeleton skeleton;
	m3 transform;
	float pixel_scale;
};

static GLuint
Ts2dInitOpenGLShaderFromData(char *shader_name,
							 Ts2dOpenGLShaderInput *inputs, int input_count,
							 Ts2dOpenGLShaderOutput *outputs, int output_count,
							 char *vertex, char *fragment)
{
	Log("[Ts2d] Building shader \"%s\".", shader_name);

	GLuint program = 0;
	GLuint vertex_shader = 0;
	GLuint fragment_shader = 0;

	int info_log_length = 0;
	GLint result = GL_FALSE;
	GLint code_length = 0;

	// NOTE(rjf): Compile and attach vertex shader
	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		code_length = (GLint)CalculateCStringLength(vertex);
		glShaderSource(vertex_shader, 1, &vertex, &code_length);
		glCompileShader(vertex_shader);

		// NOTE(rjf): Get vertex shader errors
		{
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 1)
			{
				char vertex_shader_error[1024] = {0};
				if (info_log_length > sizeof(vertex_shader_error) - 1)
				{
					info_log_length = sizeof(vertex_shader_error) - 1;
				}
				glGetShaderInfoLog(vertex_shader, info_log_length, 0, vertex_shader_error);
				LogError("[Ts2d] %s", vertex_shader_error);
			}
			else
			{
				Log("[Ts2d] Vertex shader compiled successfully.");
			}
		}
	}

	// NOTE(rjf): Compile and attach fragment shader
	{
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		code_length = (GLint)CalculateCStringLength(fragment);
		glShaderSource(fragment_shader, 1, &fragment, &code_length);
		glCompileShader(fragment_shader);

		// NOTE(rjf): Get fragment shader errors
		{
			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 1)
			{
				char fragment_shader_error[1024] = {0};
				if (info_log_length > sizeof(fragment_shader_error) - 1)
				{
					info_log_length = sizeof(fragment_shader_error) - 1;
				}
				glGetShaderInfoLog(fragment_shader, info_log_length, 0, fragment_shader_error);
				LogError("[Ts2d] %s", fragment_shader_error);
			}
			else
			{
				Log("[Ts2d] Fragment shader compiled successfully.");
			}
		}
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// NOTE(rjf): Specify shader inputs
	{
		for (int i = 0; i < input_count; ++i)
		{
			glBindAttribLocation(program, inputs[i].index, inputs[i].name);
		}
	}

	// NOTE(rjf): Specify shader outputs
	{
		for (int i = 0; i < output_count; ++i)
		{
			glBindFragDataLocation(program, outputs[i].index, outputs[i].name);
		}
	}

	// NOTE(rjf): Link shaders
	{
		glLinkProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	glValidateProgram(program);

	return program;
}

Ts2dOpenGLFBO
Ts2dOpenGLFBOInit(unsigned int width, unsigned int height, i32 flags)
{
	Ts2dOpenGLFBO fbo = {0};
	fbo.flags = flags;
	fbo.width = width;
	fbo.height = height;
	glGenFramebuffers(1, &fbo.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
	{
		GLenum colors[4] = {0};
		unsigned int color_count = 0;

		for (int i = 0; i < sizeof(fbo.color_textures) / sizeof(fbo.color_textures[0]); ++i)
		{
			if (flags & (TS2D_OPENGL_FBO_COLOR_OUT_0 << i))
			{
				glGenTextures(1, fbo.color_textures + i);
				glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
				colors[color_count++] = GL_COLOR_ATTACHMENT0 + i;
			}
			else if (flags & (TS2D_OPENGL_FBO_COLOR_OUT_0_16F << i))
			{
				glGenTextures(1, fbo.color_textures + i);
				glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
				colors[color_count++] = GL_COLOR_ATTACHMENT0 + i;
			}
			else if (flags & (TS2D_OPENGL_FBO_COLOR_OUT_0_32F << i))
			{
				glGenTextures(1, fbo.color_textures + i);
				glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
				colors[color_count++] = GL_COLOR_ATTACHMENT0 + i;
			}
		}

		if (flags & TS2D_OPENGL_FBO_DEPTH_OUT)
		{
			glGenTextures(1, &fbo.depth_texture);
			glBindTexture(GL_TEXTURE_2D, fbo.depth_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo.depth_texture, 0);
		}

		if (color_count)
		{
			glDrawBuffers(color_count, colors);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fbo;
};

void Ts2dOpenGLFBOCleanUp(Ts2dOpenGLFBO *fbo)
{
	glDeleteFramebuffers(1, &fbo->fbo);
	for (unsigned int i = 0; i < ArrayCount(fbo->color_textures); ++i)
	{
		if (fbo->color_textures[i])
		{
			glDeleteTextures(1, &fbo->color_textures[i]);
		}
	}
	if (fbo->depth_texture)
	{
		glDeleteTextures(1, &fbo->depth_texture);
	}
	fbo->fbo = 0;
}

void Ts2dOpenGLFBOForceSize(Ts2dOpenGLFBO *fbo, unsigned int w, unsigned int h, i32 flags)
{
	if (fbo->w != w || fbo->h != h)
	{
		Ts2dOpenGLFBOCleanUp(fbo);
		*fbo = Ts2dOpenGLFBOInit(w, h, flags);
	}
}

void Ts2dOpenGLFBOBind(Ts2dOpenGLFBO *fbo)
{
	if (fbo != global_ts2d->active_fbo)
	{
		if (fbo)
		{
			glViewport(0, 0, (GLsizei)fbo->w, (GLsizei)fbo->h);
			glScissor(0, 0, (GLsizei)fbo->w, (GLsizei)fbo->h);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glViewport(0, 0, (GLsizei)global_ts2d->render_width, (GLsizei)global_ts2d->render_height);
			glScissor(0, 0, (GLsizei)global_ts2d->render_width, (GLsizei)global_ts2d->render_height);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}
		global_ts2d->active_fbo = fbo;
	}
}

void Ts2dOpenGLFBOClear(Ts2dOpenGLFBO *fbo)
{
	Ts2dOpenGLFBO *last_fbo = global_ts2d->active_fbo;
	Ts2dOpenGLFBOBind(fbo);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	if (fbo->depth_texture)
	{
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	Ts2dOpenGLFBOBind(last_fbo);
}

void _Ts2dOpenGLFBORender(GLuint shader, Ts2dOpenGLFBO *fbo, v4 source, v4 dest)
{
	glBindVertexArray(global_ts2d->texture.vao);
	glUseProgram(shader);
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->color_textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		v2 scale =
			{
				dest.width / source.width,
				dest.height / source.height,
			};

		dest.x = +(2 * dest.x / global_ts2d->render_width - 1);
		dest.y = -(2 * dest.y / global_ts2d->render_height - 1);
		dest.width = +(2 * dest.width / global_ts2d->render_width);
		dest.height = -(2 * dest.height / global_ts2d->render_height);

		glUniform4f(glGetUniformLocation(shader, "destination"), dest.x, dest.y, dest.width, dest.height);
		glUniform2f(glGetUniformLocation(shader, "uv_offset"), source.x, source.height + source.y);
		glUniform2f(glGetUniformLocation(shader, "uv_range"), source.width, -source.height);
		glUniform2f(glGetUniformLocation(shader, "scale"), scale.x, scale.y);
		glUniform1f(glGetUniformLocation(shader, "opacity"), 1.f);
		glUniform1i(glGetUniformLocation(shader, "tex"), 0);
		glUniform2f(glGetUniformLocation(shader, "tex_resolution"), (f32)fbo->w, (f32)fbo->h);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glBindVertexArray(0);
}

void Ts2dOpenGLFBORender(Ts2dOpenGLFBO *fbo, v4 source, v4 dest)
{
	_Ts2dOpenGLFBORender(global_ts2d->shaders[TS2D_OPENGL_SHADER_fbo], fbo, source, dest);
}

void Ts2dOpenGLBlurredFBORender(GLuint texture, f32 width, f32 height,
								b32 vertical, f32 standard_deviation, i32 radius, v4 clip)
{
	GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_gaussian_blur];
	glBindVertexArray(global_ts2d->all_purpose_vao);
	glUseProgram(shader);
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glUniform1i(glGetUniformLocation(shader, "tex"), 0);
		glUniform2f(glGetUniformLocation(shader, "tex_resolution"), width - 1.f, height - 1.f);
		glUniform1i(glGetUniformLocation(shader, "radius"), radius);
		glUniform1i(glGetUniformLocation(shader, "vertical"), !!vertical);
		glUniform4f(glGetUniformLocation(shader, "clip"), clip.x, clip.y, clip.width, clip.height);

		local_persist const i32 kernel_size = 128;
		i32 kernel_midpoint = kernel_size / 2;
		i32 kernel_lower_bound = kernel_midpoint - radius;
		i32 kernel_upper_bound = kernel_midpoint + radius;

		f32 vec4_buffer_because_glsl_sucks[4] = {0};
		i32 vec4_counter_because_glsl_sucks = 0;

		char kernel_uniform_str_1_digit_index_because_glsl_sucks[32] = "kernel[x]";
		char kernel_uniform_str_2_digit_index_because_glsl_sucks[32] = "kernel[xx]";

		for (i32 i = kernel_lower_bound;
			 i >= 0 &&
			 i <= kernel_upper_bound &&
			 i <= kernel_size;
			 ++i)
		{

			f32 x = (f32)(kernel_midpoint - i);

			vec4_buffer_because_glsl_sucks[vec4_counter_because_glsl_sucks++] =
				(TS2D_ONE_OVER_SQUARE_ROOT_OF_TWO_PIf / standard_deviation) *
				powf(TS2D_EULERS_NUMBERf, -(x * x) / (2 * standard_deviation * standard_deviation));

			char *kernel_uniform_str = 0;
			i32 kernel_vec4_index_because_glsl_sucks = i / 4;

			if (kernel_vec4_index_because_glsl_sucks < 10)
			{
				kernel_uniform_str = kernel_uniform_str_1_digit_index_because_glsl_sucks;
				kernel_uniform_str[7] = '0' + kernel_vec4_index_because_glsl_sucks;
			}
			else if (kernel_vec4_index_because_glsl_sucks < 100)
			{
				kernel_uniform_str = kernel_uniform_str_2_digit_index_because_glsl_sucks;
				snprintf(kernel_uniform_str + 7, 4, "%i]", kernel_vec4_index_because_glsl_sucks);
			}

			if (kernel_uniform_str)
			{
				if (vec4_counter_because_glsl_sucks >= 4)
				{
					vec4_counter_because_glsl_sucks = 0;
					glUniform4f(glGetUniformLocation(shader, kernel_uniform_str),
								vec4_buffer_because_glsl_sucks[0],
								vec4_buffer_because_glsl_sucks[1],
								vec4_buffer_because_glsl_sucks[2],
								vec4_buffer_because_glsl_sucks[3]);
				}
			}
		}

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glBindVertexArray(0);
}

static void
_Ts2dSetOpenGLInstancedFloatAttributeData(int index, int stride, int size, int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,
						  (void *)(sizeof(f32) * offset));
	glVertexAttribDivisor(index, 1);
}

void Ts2dInit(MemoryArena *arena)
{
	Ts2dLoadAllOpenGLProcedures();

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &global_ts2d->all_purpose_vao);

	// NOTE(rjf): Initialize instance type data
	{
#define Ts2dInstanceType(name, size_per_instance, max)                                                     \
	{                                                                                                      \
		global_ts2d->name.instance_data_max = size_per_instance * max;                                     \
		global_ts2d->name.instance_data = MemoryArenaAllocate(arena, global_ts2d->name.instance_data_max); \
		global_ts2d->name.instance_data_stride = size_per_instance;                                        \
		glGenVertexArrays(1, &global_ts2d->name.vao);                                                      \
		glBindVertexArray(global_ts2d->name.vao);                                                          \
		{                                                                                                  \
			glGenBuffers(1, &global_ts2d->name.instance_buffer);                                           \
			glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->name.instance_buffer);                              \
			glBufferData(GL_ARRAY_BUFFER, global_ts2d->name.instance_data_max, 0, GL_DYNAMIC_DRAW);        \
		}                                                                                                  \
		glBindVertexArray(0);                                                                              \
	}
#include "ts2d_opengl_instance_type_list.inc"
	}

	// NOTE(rjf): Instance instanced float attribute data for each instance type.
	{
		// NOTE(rjf): Lines
		{
			glBindVertexArray(global_ts2d->line.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->line.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->line.instance_data_stride, 2, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->line.instance_data_stride, 2, 2);
	_Ts2dSetOpenGLInstancedFloatAttributeData(2, global_ts2d->line.instance_data_stride, 4, 4);
	glBindVertexArray(0);
}

// NOTE(rjf): Rects
{
	glBindVertexArray(global_ts2d->rect.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->rect.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->rect.instance_data_stride, 4, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->rect.instance_data_stride, 4, 4);
	glBindVertexArray(0);
}

// NOTE(rjf): Filled Rects
{
	glBindVertexArray(global_ts2d->filled_rect.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->filled_rect.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->filled_rect.instance_data_stride, 4, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->filled_rect.instance_data_stride, 4, 4);
	_Ts2dSetOpenGLInstancedFloatAttributeData(2, global_ts2d->filled_rect.instance_data_stride, 4, 8);
	_Ts2dSetOpenGLInstancedFloatAttributeData(3, global_ts2d->filled_rect.instance_data_stride, 4, 12);
	_Ts2dSetOpenGLInstancedFloatAttributeData(4, global_ts2d->filled_rect.instance_data_stride, 4, 16);
	glBindVertexArray(0);
}

// NOTE(rjf): Textures
{
	glBindVertexArray(global_ts2d->texture.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->texture.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->texture.instance_data_stride, 4, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->texture.instance_data_stride, 4, 4);
	_Ts2dSetOpenGLInstancedFloatAttributeData(2, global_ts2d->texture.instance_data_stride, 4, 8);
	glBindVertexArray(0);
}

// NOTE(rjf): World Tiles
{
	glBindVertexArray(global_ts2d->world_tile.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->world_tile.instance_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 2, GL_UNSIGNED_BYTE, global_ts2d->world_tile.instance_data_stride, 0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 2, GL_UNSIGNED_BYTE, global_ts2d->world_tile.instance_data_stride, (void *)(sizeof(u8) * 2));
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);
}

// NOTE(rjf): Text Chars
{
	glBindVertexArray(global_ts2d->text_char.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->text_char.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->text_char.instance_data_stride, 4, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->text_char.instance_data_stride, 4, 4);
	_Ts2dSetOpenGLInstancedFloatAttributeData(2, global_ts2d->text_char.instance_data_stride, 4, 8);
	_Ts2dSetOpenGLInstancedFloatAttributeData(3, global_ts2d->text_char.instance_data_stride, 4, 12);
	glBindVertexArray(0);
}

// NOTE(rjf): Reflective Rects
{
	glBindVertexArray(global_ts2d->reflective_rect.vao);
	glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->reflective_rect.instance_buffer);
	_Ts2dSetOpenGLInstancedFloatAttributeData(0, global_ts2d->reflective_rect.instance_data_stride, 4, 0);
	_Ts2dSetOpenGLInstancedFloatAttributeData(1, global_ts2d->reflective_rect.instance_data_stride, 4, 4);
	_Ts2dSetOpenGLInstancedFloatAttributeData(2, global_ts2d->reflective_rect.instance_data_stride, 4, 8);
	glBindVertexArray(0);
}
}

// NOTE(rjf): Load shaders
{
	global_ts2d->shaders = MemoryArenaAllocate(arena, sizeof(global_ts2d->shaders[0]) * TS2D_OPENGL_SHADER_MAX);
	for (unsigned int i = 0; i < TS2D_OPENGL_SHADER_MAX; ++i)
	{
		global_ts2d->shaders[i] = Ts2dInitOpenGLShaderFromData(
			_ts2d__global_opengl_shaders[i].name,
			_ts2d__global_opengl_shaders[i].inputs,
			_ts2d__global_opengl_shaders[i].input_count,
			_ts2d__global_opengl_shaders[i].outputs,
			_ts2d__global_opengl_shaders[i].output_count,
			_ts2d__global_opengl_shaders[i].vert,
			_ts2d__global_opengl_shaders[i].frag);
	}
}

// NOTE(rjf): Initialize request data
{
	global_ts2d->request_memory_max = TS2D_MAX_REQUEST_MEMORY;
	global_ts2d->request_memory = MemoryArenaAllocate(arena, global_ts2d->request_memory_max);
}

// NOTE(rjf): Initialize FBO data
{
	global_ts2d->active_fbo = 0;
	global_ts2d->pre_lighting_fbo_8u.w = 0;
	global_ts2d->post_lighting_fbo_16f.w = 0;
	global_ts2d->pre_composite_fbo_8u.w = 0;
	global_ts2d->world_tile_fbo_8u.w = 0;
	global_ts2d->world_reflection_fbo_8u.w = 0;
	global_ts2d->screen_size_scratch_fbo_1_8u.w = 0;
	global_ts2d->screen_size_scratch_fbo_2_8u.w = 0;
	global_ts2d->screen_size_scratch_fbo_3_8u.w = 0;
	global_ts2d->half_screen_size_scratch_fbo_1_8u.w = 0;
	global_ts2d->half_screen_size_scratch_fbo_2_8u.w = 0;
}

// NOTE(rjf): Initialize default font.
{
	global_ts2d->default_font = 0;
}

global_ts2d->current_time = 0.f;
}

void Ts2dBeginFrame(Ts2dBeginFrameInfo *begin_info)
{
	global_ts2d->current_clip = v4(0, 0, global_ts2d->render_width, global_ts2d->render_height);

	// NOTE(rjf): Load data from info struct
	{
		global_ts2d->render_width = begin_info->render_width;
		global_ts2d->render_height = begin_info->render_height;
		global_ts2d->delta_t = begin_info->delta_t;
		global_ts2d->flags = begin_info->flags;
	}

	global_ts2d->current_time += global_ts2d->delta_t;

	// NOTE(rjf): Initialize clip stack
	{
		global_ts2d->clip_stack_size = 0;
		global_ts2d->clip_stack_max = sizeof(global_ts2d->clip_stack) / sizeof(global_ts2d->clip_stack[0]);
	}

#define Ts2dInstanceType(name, size_per_instance, max) global_ts2d->name.instance_data_alloc_pos = 0;
#include "ts2d_opengl_instance_type_list.inc"
	global_ts2d->light_count = 0;

	global_ts2d->active_request.type = TS2D_REQUEST_null;
	global_ts2d->first_request = 0;
	global_ts2d->last_request = 0;
	global_ts2d->request_memory_alloc_position = 0;

	Ts2dOpenGLFBOForceSize(&global_ts2d->pre_lighting_fbo_8u,
						   (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->post_lighting_fbo_16f,
						   (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0_16F);
	Ts2dOpenGLFBOForceSize(&global_ts2d->pre_composite_fbo_8u,
						   (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->world_tile_fbo_8u, (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->world_reflection_fbo_8u, (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0 | TS2D_OPENGL_FBO_COLOR_OUT_1_16F);
	Ts2dOpenGLFBOForceSize(&global_ts2d->screen_size_scratch_fbo_1_8u, (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->screen_size_scratch_fbo_2_8u, (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->screen_size_scratch_fbo_3_8u, (unsigned int)begin_info->render_width, (unsigned int)begin_info->render_height,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->half_screen_size_scratch_fbo_1_8u, (unsigned int)begin_info->render_width / 2, (unsigned int)begin_info->render_height / 2,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->half_screen_size_scratch_fbo_2_8u, (unsigned int)begin_info->render_width / 2, (unsigned int)begin_info->render_height / 2,
						   TS2D_OPENGL_FBO_COLOR_OUT_0);
	Ts2dOpenGLFBOForceSize(&global_ts2d->model_sprite_fbo, 2048, 2048, TS2D_OPENGL_FBO_COLOR_OUT_0 | TS2D_OPENGL_FBO_DEPTH_OUT);
}

typedef struct Rect2DQuadTree Rect2DQuadTree;
struct Rect2DQuadTree
{
	i16 number_of_children_allocated;
};

internal v4
Rect2DQuadTreeAllocate(Rect2DQuadTree *tree, u32 tree_max_nodes, v2 tree_size, v2 size)
{
	v4 result = {0};

	Rect2DQuadTree *node = tree;
	v2 node_position = {0};
	v2 node_size = tree_size;

	Rect2DQuadTree *node_to_allocate = 0;
	v2 node_to_allocate_position = {0};
	v2 node_to_allocate_size = {0};

	for (; node && node->number_of_children_allocated < 4;)
	{
		u32 node_index = (u32)(node - tree);

		if (node->number_of_children_allocated == 0 &&
			node_size.x >= size.x && node_size.y >= size.y)
		{
			node_to_allocate = node;
			node_to_allocate_position = node_position;
			node_to_allocate_size = node_size;
		}

		u32 child_index[4] =
			{
				node_index * 4 + 1,
				node_index * 4 + 2,
				node_index * 4 + 3,
				node_index * 4 + 4,
			};

		Rect2DQuadTree *children[4] =
			{
				(child_index[0] < tree_max_nodes) ? (tree + child_index[0]) : 0,
				(child_index[1] < tree_max_nodes) ? (tree + child_index[1]) : 0,
				(child_index[2] < tree_max_nodes) ? (tree + child_index[2]) : 0,
				(child_index[3] < tree_max_nodes) ? (tree + child_index[3]) : 0,
			};

		v2 next_node_position = node_position;
		v2 next_node_size = {node_size.x / 2, node_size.y / 2};
		Rect2DQuadTree *next_node = 0;
		if (next_node_size.x >= size.x && next_node_size.y >= size.y)
		{
			for (int i = 0; i < ArrayCount(children); ++i)
			{
				if (children[i] &&
					((children[i]->number_of_children_allocated < 4 && next_node_size.x / 2 >= size.x && next_node_size.y / 2 >= size.y) ||
					 children[i]->number_of_children_allocated == 0))
				{
					next_node = children[i];
					next_node_position.x = next_node_position.x + next_node_size.x * FMod(i * node_size.x, next_node_size.x);
					next_node_position.y = next_node_position.y + next_node_size.y * (int)((i * node_size.x) / node_size.x);
					break;
				}
			}
		}

		if (next_node)
		{
			node = next_node;
			node_position = next_node_position;
			node_size = next_node_size;
		}
		else
		{
			break;
		}
	}

	if (node_to_allocate)
	{
		// NOTE(rjf): Allocate node.
		{
			++node_to_allocate->number_of_children_allocated;
			result.x = node_to_allocate_position.x;
			result.y = node_to_allocate_position.y;
			result.width = size.x;
			result.height = size.y;
		}

		// NOTE(rjf): Increment all parents information of child allocations.
		{
			u32 node_to_allocate_index = (u32)(node_to_allocate - tree);
			for (int i = (int)node_to_allocate_index / 4; i >= 0; i /= 4)
			{
				++tree[i].number_of_children_allocated;
				if (i == 0)
				{
					break;
				}
			}
		}
	}

	return result;
}

void Ts2dEndFrame(void)
{
	Ts2dInternalFinishActiveRequest();

	Rect2DQuadTree model_sprite_fbo_quad_tree[1025] = {0};

	Ts2dOpenGLFBOBind(0);
	glDisable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	glViewport(0, 0, (GLsizei)global_ts2d->render_width, (GLsizei)global_ts2d->render_height);
	glScissor(0, 0, (GLint)global_ts2d->render_width, (GLint)global_ts2d->render_height);

	Ts2dOpenGLFBOClear(&global_ts2d->pre_lighting_fbo_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->post_lighting_fbo_16f);
	Ts2dOpenGLFBOClear(&global_ts2d->pre_composite_fbo_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->screen_size_scratch_fbo_1_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->screen_size_scratch_fbo_2_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->screen_size_scratch_fbo_3_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_2_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->world_reflection_fbo_8u);
	Ts2dOpenGLFBOClear(&global_ts2d->model_sprite_fbo);

	v4 active_clip = {0, 0, global_ts2d->render_width, global_ts2d->render_height};

	Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);

	Ts2dOpenGLFBO *foreground_rendering_fbo = &global_ts2d->screen_size_scratch_fbo_1_8u;
	Ts2dOpenGLFBO *background_rendering_fbo = &global_ts2d->screen_size_scratch_fbo_2_8u;

	b32 world_rendering_enabled = 0;

	for (Ts2dRequest *request = global_ts2d->first_request; request; request = request->next)
	{
		switch (request->type)
		{
		case TS2D_REQUEST_set_clip:
		{
			v4 clip = *(v4 *)request->data;
			glScissor((GLint)clip.x, (GLint)(global_ts2d->render_height - clip.y - clip.height),
					  (GLint)clip.width, (GLint)clip.height);
			active_clip = clip;
			break;
		}

		case TS2D_REQUEST_begin_world:
		{
			Ts2dOpenGLFBOClear(foreground_rendering_fbo);
			Ts2dOpenGLFBOBind(foreground_rendering_fbo);
			world_rendering_enabled = 1;
			break;
		}

		case TS2D_REQUEST_end_world:
		{
			// NOTE(rjf): Generate shadow buffer from foreground.
			Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
			if (global_ts2d->shadow_opacity > 0.001f)
			{
				Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_shadow];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->all_purpose_vao);
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, foreground_rendering_fbo->color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "tex"), 0);
					glUniform2f(glGetUniformLocation(shader, "tex_resolution"), global_ts2d->render_width, global_ts2d->render_height);
					glUniform2f(glGetUniformLocation(shader, "light_vector"), global_ts2d->shadow_vector.x, global_ts2d->shadow_vector.y);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				glBindVertexArray(0);
				glUseProgram(0);
				Ts2dOpenGLFBOBind(0);

				// NOTE(rjf): Blur shadow buffer.
				if (global_ts2d->flags & TS2D_BLUR_SHADOWS)
				{
					f32 blur_stdev = 1.f;
					i32 blur_radius = 16;

					Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_2_8u);
					Ts2dOpenGLBlurredFBORender(global_ts2d->half_screen_size_scratch_fbo_1_8u.color_textures[0],
											   global_ts2d->render_width / 2, global_ts2d->render_height / 2,
											   0, blur_stdev, blur_radius, v4(0, 0, global_ts2d->render_width / 2, global_ts2d->render_height / 2));
					Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
					Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
					Ts2dOpenGLBlurredFBORender(global_ts2d->half_screen_size_scratch_fbo_2_8u.color_textures[0],
											   global_ts2d->render_width / 2, global_ts2d->render_height / 2,
											   1, blur_stdev, blur_radius, v4(0, 0, global_ts2d->render_width / 2, global_ts2d->render_height / 2));
					Ts2dOpenGLFBOBind(0);
				}
			}

			// NOTE(rjf): Composite background.
			{
				Ts2dOpenGLFBOBind(&global_ts2d->post_lighting_fbo_16f);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_background];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->all_purpose_vao);
				{
					glUniform1i(glGetUniformLocation(shader, "background_texture"), 0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, background_rendering_fbo->color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "tex"), 0);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				glBindVertexArray(0);
				glUseProgram(0);
			}

			// NOTE(rjf): World composite/lighting pass.
			{
				Ts2dOpenGLFBOBind(&global_ts2d->post_lighting_fbo_16f);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_world];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->all_purpose_vao);
				{
					glUniform1i(glGetUniformLocation(shader, "foreground_texture"), 1);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, foreground_rendering_fbo->color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glUniform1i(glGetUniformLocation(shader, "shadow_texture"), 2);
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, global_ts2d->half_screen_size_scratch_fbo_1_8u.color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glUniform1f(glGetUniformLocation(shader, "brightness"), global_ts2d->brightness);
					glUniform1f(glGetUniformLocation(shader, "shadow_opacity"), global_ts2d->shadow_opacity);

					// NOTE(rjf): Upload light information
					{
						v2 camera =
							{
								global_ts2d->camera_pos.x,
								global_ts2d->camera_pos.y,
							};

						v4 screen_rect =
							{
								camera.x,
								camera.y,
								global_ts2d->render_width,
								global_ts2d->render_height,
							};

						char light_pos_uniform_str_1_digit[32] = "lights[x].position";
						char light_pos_uniform_str_2_digit[32] = "lights[xx].position";
						char light_color_uniform_str_1_digit[32] = "lights[x].color";
						char light_color_uniform_str_2_digit[32] = "lights[xx].color";
						char light_radius_uniform_str_1_digit[32] = "lights[x].radius";
						char light_radius_uniform_str_2_digit[32] = "lights[xx].radius";
						char light_intensity_uniform_str_1_digit[32] = "lights[x].intensity";
						char light_intensity_uniform_str_2_digit[32] = "lights[xx].intensity";

						char *light_pos_uniform_str = 0;
						char *light_color_uniform_str = 0;
						char *light_radius_uniform_str = 0;
						char *light_intensity_uniform_str = 0;

						int number_of_lights_that_were_sent_to_shader = 0;

						for (u32 i = 0; i < global_ts2d->light_count && i < 16; ++i)
						{
							v4 light_rect =
								{
									global_ts2d->lights[i].position.x - global_ts2d->lights[i].radius,
									global_ts2d->lights[i].position.y - global_ts2d->lights[i].radius,
									global_ts2d->lights[i].radius * 2,
									global_ts2d->lights[i].radius * 2,
								};

							if (V4sIntersect(screen_rect, light_rect))
							{
								if (number_of_lights_that_were_sent_to_shader < 10)
								{
									light_pos_uniform_str = light_pos_uniform_str_1_digit;
									light_color_uniform_str = light_color_uniform_str_1_digit;
									light_radius_uniform_str = light_radius_uniform_str_1_digit;
									light_intensity_uniform_str = light_intensity_uniform_str_1_digit;

									light_pos_uniform_str[7] = '0' + (i32)number_of_lights_that_were_sent_to_shader;
									light_color_uniform_str[7] = '0' + (i32)number_of_lights_that_were_sent_to_shader;
									light_radius_uniform_str[7] = '0' + (i32)number_of_lights_that_were_sent_to_shader;
									light_intensity_uniform_str[7] = '0' + (i32)number_of_lights_that_were_sent_to_shader;
								}
								else if (number_of_lights_that_were_sent_to_shader < 20)
								{
									light_pos_uniform_str = light_pos_uniform_str_2_digit;
									light_color_uniform_str = light_color_uniform_str_2_digit;
									light_radius_uniform_str = light_radius_uniform_str_2_digit;
									light_intensity_uniform_str = light_intensity_uniform_str_2_digit;

									light_pos_uniform_str[7] = '1';
									light_color_uniform_str[7] = '1';
									light_radius_uniform_str[7] = '1';
									light_intensity_uniform_str[7] = '1';

									light_pos_uniform_str[8] = '0' + ((i32)number_of_lights_that_were_sent_to_shader - 10);
									light_color_uniform_str[8] = '0' + ((i32)number_of_lights_that_were_sent_to_shader - 10);
									light_radius_uniform_str[8] = '0' + ((i32)number_of_lights_that_were_sent_to_shader - 10);
									light_intensity_uniform_str[8] = '0' + ((i32)number_of_lights_that_were_sent_to_shader - 10);
								}
								else
								{
									AssertStatement("NOTE(rjf): We did not plan for this many lights to be sent to to the shader" == 0);
								}

								glUniform2f(glGetUniformLocation(shader, light_pos_uniform_str),
											global_ts2d->camera_zoom *
													(global_ts2d->lights[i].position.x - camera.x) -
												(global_ts2d->camera_zoom - 1) * (global_ts2d->render_width / 2),
											global_ts2d->camera_zoom *
													(global_ts2d->render_height - global_ts2d->lights[i].position.y + camera.y) -
												(global_ts2d->camera_zoom - 1) * (global_ts2d->render_height / 2));
								glUniform3f(glGetUniformLocation(shader, light_color_uniform_str),
											global_ts2d->lights[i].color.r,
											global_ts2d->lights[i].color.g,
											global_ts2d->lights[i].color.b);
								glUniform1f(glGetUniformLocation(shader, light_radius_uniform_str),
											global_ts2d->camera_zoom * global_ts2d->lights[i].radius);
								glUniform1f(glGetUniformLocation(shader, light_intensity_uniform_str),
											global_ts2d->lights[i].intensity);

								++number_of_lights_that_were_sent_to_shader;
							}
						}

						glUniform1i(glGetUniformLocation(shader, "light_count"),
									number_of_lights_that_were_sent_to_shader);
					}

					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				glBindVertexArray(0);
				glUseProgram(0);
				Ts2dOpenGLFBOBind(0);
			}

			// NOTE(rjf): Composite world to main framebuffer with tone-mapping and post-processing pass.
			{
				Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_post_process];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->all_purpose_vao);
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, global_ts2d->post_lighting_fbo_16f.color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "tex"), 0);
					glUniform1f(glGetUniformLocation(shader, "grayscale"), global_ts2d->grayscale);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				glBindVertexArray(0);
				glUseProgram(0);
				Ts2dOpenGLFBOBind(0);
			}

			// NOTE(rjf): Apply reflections.
			{
				Ts2dOpenGLFBOClear(&global_ts2d->screen_size_scratch_fbo_3_8u);
				Ts2dOpenGLFBOBind(&global_ts2d->screen_size_scratch_fbo_3_8u);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_reflection];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->all_purpose_vao);
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, global_ts2d->pre_composite_fbo_8u.color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "tex"), 0);

					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, global_ts2d->world_reflection_fbo_8u.color_textures[0]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "reflection_color_tex"), 1);

					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, global_ts2d->world_reflection_fbo_8u.color_textures[1]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "reflection_data_tex"), 2);

					glUniform2f(glGetUniformLocation(shader, "render_resolution"), global_ts2d->render_width,
								global_ts2d->render_height);

					glUniform1f(glGetUniformLocation(shader, "wave_position"), global_ts2d->current_time * 8.f);

					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				glBindVertexArray(0);
				glUseProgram(0);
				Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
				Ts2dOpenGLFBORender(&global_ts2d->screen_size_scratch_fbo_3_8u,
									v4(0, 0, global_ts2d->render_width, global_ts2d->render_height),
									v4(0, 0, global_ts2d->render_width, global_ts2d->render_height));
			}

			// NOTE(rjf): Apply bloom.
			if (global_ts2d->flags & TS2D_BLOOM)
			{
				Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_1_8u);

				// NOTE(rjf): Do bloom filter pass.
				{
					Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
					GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_bloom_filter];
					glUseProgram(shader);
					glBindVertexArray(global_ts2d->all_purpose_vao);
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, global_ts2d->post_lighting_fbo_16f.color_textures[0]);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glUniform1i(glGetUniformLocation(shader, "tex"), 0);

						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, foreground_rendering_fbo->color_textures[0]);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glUniform1i(glGetUniformLocation(shader, "foreground_tex"), 1);

						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					}
					glBindVertexArray(0);
					glUseProgram(0);
					Ts2dOpenGLFBOBind(0);
				}

				// NOTE(rjf): Blur bloom.
				{
					f32 blur_stdev = 3.f;
					i32 blur_radius = 32;

					Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_2_8u);
					Ts2dOpenGLBlurredFBORender(global_ts2d->half_screen_size_scratch_fbo_1_8u.color_textures[0],
											   global_ts2d->render_width / 2, global_ts2d->render_height / 2,
											   0, blur_stdev, blur_radius, v4(0, 0, global_ts2d->render_width / 2, global_ts2d->render_height / 2));
					Ts2dOpenGLFBOBind(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
					Ts2dOpenGLFBOClear(&global_ts2d->half_screen_size_scratch_fbo_1_8u);
					Ts2dOpenGLBlurredFBORender(global_ts2d->half_screen_size_scratch_fbo_2_8u.color_textures[0],
											   global_ts2d->render_width / 2, global_ts2d->render_height / 2,
											   1, blur_stdev, blur_radius, v4(0, 0, global_ts2d->render_width / 2, global_ts2d->render_height / 2));
					Ts2dOpenGLFBOBind(0);
				}

				// NOTE(rjf): Additively apply bloom to pre composite framebuffer.
				{
					Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);
					Ts2dOpenGLFBORender(&global_ts2d->half_screen_size_scratch_fbo_1_8u,
										v4(0, 0, global_ts2d->render_width / 2, global_ts2d->render_height / 2),
										v4(0, 0, global_ts2d->render_width, global_ts2d->render_height));
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				}
			}

			Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
			world_rendering_enabled = 0;

			break;
		}

		case TS2D_REQUEST_begin_background:
		{
			Ts2dOpenGLFBOBind(background_rendering_fbo);
			break;
		}

		case TS2D_REQUEST_end_background:
		{
			Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
			break;
		}

		case TS2D_REQUEST_line:
		{
			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->line.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->line.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_line];
			glUseProgram(shader);
			glBindVertexArray(global_ts2d->line.vao);
			{
				GLint first = 0;
				GLsizei count = 2;
				GLsizei instance_count = request->instance_data_size / global_ts2d->line.instance_data_stride;
				glDrawArraysInstanced(GL_LINES, first, count, instance_count);
			}
			glBindVertexArray(0);
			break;
		}

		case TS2D_REQUEST_rect:
		{
			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->rect.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->rect.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_rect];
			glUseProgram(shader);
			glBindVertexArray(global_ts2d->rect.vao);
			{
				GLint first = 0;
				GLsizei count = 4;
				GLsizei instance_count = request->instance_data_size / global_ts2d->rect.instance_data_stride;
				glDrawArraysInstanced(GL_LINE_LOOP, first, count, instance_count);
			}
			glBindVertexArray(0);
			break;
		}

		case TS2D_REQUEST_filled_rect:
		{
			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->filled_rect.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->filled_rect.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_filled_rect];
			glUseProgram(shader);
			glBindVertexArray(global_ts2d->filled_rect.vao);
			{
				GLint first = 0;
				GLsizei count = 4;
				GLsizei instance_count = request->instance_data_size / global_ts2d->filled_rect.instance_data_stride;
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
			}
			glBindVertexArray(0);
			break;
		}

		case TS2D_REQUEST_texture:
		{
			Ts2dTextureRequestData *texture_data = request->data;
			b32 additive = request->flags & TS2D_ADDITIVE_BLEND;
			Ts2dTexture *texture = texture_data->texture;

			if (texture)
			{
				if (additive)
				{
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);
				}

				// NOTE(rjf): Upload data
				{
					glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->texture.instance_buffer);
					glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
									global_ts2d->texture.instance_data + request->instance_data_offset);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_texture];
				glUseProgram(shader);
				glBindVertexArray(global_ts2d->texture.vao);
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture->id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glUniform1i(glGetUniformLocation(shader, "tex"), 0);
					glUniform2f(glGetUniformLocation(shader, "tex_resolution"), (f32)texture->width, (f32)texture->height);
					glUniform2f(glGetUniformLocation(shader, "render_resolution"),
								global_ts2d->render_width, global_ts2d->render_height);

					GLint first = 0;
					GLsizei count = 4;
					GLsizei instance_count = request->instance_data_size / global_ts2d->texture.instance_data_stride;
					glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
				}
				glBindVertexArray(0);
				if (additive)
				{
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				}

				if (world_rendering_enabled)
				{
					Ts2dOpenGLFBO *last_fbo = global_ts2d->active_fbo;
					Ts2dOpenGLFBOBind(&global_ts2d->world_reflection_fbo_8u);
					GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_texture_reflection];
					glUseProgram(shader);
					glBindVertexArray(global_ts2d->texture.vao);
					glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texture->id);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glUniform1i(glGetUniformLocation(shader, "tex"), 0);
						glUniform2f(glGetUniformLocation(shader, "tex_resolution"), (f32)texture->width, (f32)texture->height);
						glUniform2f(glGetUniformLocation(shader, "render_resolution"),
									global_ts2d->render_width, global_ts2d->render_height);

						GLint first = 0;
						GLsizei count = 4;
						GLsizei instance_count = request->instance_data_size / global_ts2d->texture.instance_data_stride;
						glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
					}
					glBindVertexArray(0);
					glBlendEquation(GL_FUNC_ADD);
					Ts2dOpenGLFBOBind(last_fbo);
				}
			}

			break;
		}

		case TS2D_REQUEST_world_tile:
		{
			Ts2dTexture *texture = ((Ts2dTextureRequestData *)request->data)->texture;

			Ts2dOpenGLFBOClear(&global_ts2d->world_tile_fbo_8u);
			Ts2dOpenGLFBO *last_fbo = global_ts2d->active_fbo;
			Ts2dOpenGLFBOBind(&global_ts2d->world_tile_fbo_8u);

			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->world_tile.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->world_tile.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_world_tile];
			glBindVertexArray(global_ts2d->world_tile.vao);
			glUseProgram(shader);
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glUniform1i(glGetUniformLocation(shader, "tex"), 0);
				glUniform2f(glGetUniformLocation(shader, "tex_resolution"),
							(f32)texture->width, (f32)texture->height);
				glUniform2f(glGetUniformLocation(shader, "render_resolution"), global_ts2d->render_width, global_ts2d->render_height);
				GLint first = 0;
				GLsizei count = 4;
				GLsizei instance_count = request->instance_data_size / global_ts2d->world_tile.instance_data_stride;
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
			}
			glBindVertexArray(0);

			Ts2dOpenGLFBOBind(last_fbo);

			v2 tile_texture_offset =
				{
					global_ts2d->camera_zoom * (-FMod(global_ts2d->camera_pos.x, 32.f) - 32.f),
					global_ts2d->camera_zoom * (-FMod(global_ts2d->camera_pos.y, 32.f) - 32.f),
				};

			v4 tile_fbo_source =
				{
					0,
					0,
					(f32)global_ts2d->world_tile_fbo_8u.w,
					(f32)global_ts2d->world_tile_fbo_8u.h,
				};

			v4 tile_fbo_destination =
				{
					tile_texture_offset.x -
						(global_ts2d->camera_zoom - 1) * global_ts2d->render_width / 2.f,
					tile_texture_offset.y +
						global_ts2d->camera_zoom * 16.f -
						(global_ts2d->camera_zoom - 1) * global_ts2d->render_height / 2.f,
					global_ts2d->camera_zoom * global_ts2d->world_tile_fbo_8u.w * 2.f,
					global_ts2d->camera_zoom * global_ts2d->world_tile_fbo_8u.h * 2.f};

			Ts2dOpenGLFBORender(&global_ts2d->world_tile_fbo_8u, tile_fbo_source, tile_fbo_destination);

			// NOTE(rjf): Write tiles into reflection buffer
			if (world_rendering_enabled)
			{
				Ts2dOpenGLFBO *last_fbo = global_ts2d->active_fbo;
				Ts2dOpenGLFBOBind(&global_ts2d->world_reflection_fbo_8u);
				glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
				_Ts2dOpenGLFBORender(global_ts2d->shaders[TS2D_OPENGL_SHADER_fbo_reflection],
									 &global_ts2d->world_tile_fbo_8u, tile_fbo_source, tile_fbo_destination);
				glBlendEquation(GL_FUNC_ADD);
				Ts2dOpenGLFBOBind(last_fbo);
			}
			break;
		}

		case TS2D_REQUEST_text:
		{
			Ts2dTextureRequestData *text_data = request->data;
			Ts2dTexture *texture = text_data->texture;
			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->text_char.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->text_char.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_text];
			glUseProgram(shader);
			glBindVertexArray(global_ts2d->text_char.vao);
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glUniform1i(glGetUniformLocation(shader, "tex"), 0);
				glUniform2f(glGetUniformLocation(shader, "tex_resolution"), (f32)texture->width, (f32)texture->height);
				GLint first = 0;
				GLsizei count = 4;
				GLsizei instance_count = request->instance_data_size / global_ts2d->text_char.instance_data_stride;
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
			}
			glBindVertexArray(0);
			break;
		}

		case TS2D_REQUEST_blur_rectangle:
		{
			Ts2dOpenGLFBO *last_active_fbo = global_ts2d->active_fbo;

			// NOTE(rjf): Blit current buffer to blur texture
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, global_ts2d->pre_composite_fbo_8u.fbo);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, global_ts2d->screen_size_scratch_fbo_1_8u.fbo);
				glBlitFramebuffer(0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
								  0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
								  GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}

			Ts2dBlurRequestData *blur = request->data;
			v4 clip = blur->clip;
			glScissor((GLint)clip.x, (GLint)(global_ts2d->render_height - clip.y - clip.height),
					  (GLint)clip.width, (GLint)clip.height);
			f32 blur_stdev = blur->blur_magnitude * 8.f;
			i32 blur_radius = (i32)(blur_stdev * 4.f);
			Ts2dOpenGLFBOBind(&global_ts2d->screen_size_scratch_fbo_1_8u);
			Ts2dOpenGLBlurredFBORender(global_ts2d->pre_composite_fbo_8u.color_textures[0],
									   (f32)global_ts2d->pre_composite_fbo_8u.width, (f32)global_ts2d->pre_composite_fbo_8u.height,
									   1, blur_stdev, blur_radius, clip);
			Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, global_ts2d->screen_size_scratch_fbo_1_8u.fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, global_ts2d->pre_composite_fbo_8u.fbo);
			glBlitFramebuffer(0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
							  0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
							  GL_COLOR_BUFFER_BIT, GL_NEAREST);
			Ts2dOpenGLFBOBind(&global_ts2d->screen_size_scratch_fbo_1_8u);
			Ts2dOpenGLBlurredFBORender(global_ts2d->pre_composite_fbo_8u.color_textures[0],
									   (f32)global_ts2d->pre_composite_fbo_8u.width, (f32)global_ts2d->pre_composite_fbo_8u.height,
									   0, blur_stdev, blur_radius, clip);
			Ts2dOpenGLFBOBind(&global_ts2d->pre_composite_fbo_8u);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, global_ts2d->screen_size_scratch_fbo_1_8u.fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, global_ts2d->pre_composite_fbo_8u.fbo);
			glBlitFramebuffer(0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
							  0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
							  GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glScissor((GLint)active_clip.x, (GLint)(global_ts2d->render_height - active_clip.y - active_clip.height),
					  (GLint)active_clip.width, (GLint)active_clip.height);

			Ts2dOpenGLFBOBind(last_active_fbo);

			break;
		}

		case TS2D_REQUEST_reflective_rect:
		{
			Ts2dOpenGLFBO *last_fbo = global_ts2d->active_fbo;
			Ts2dOpenGLFBOBind(&global_ts2d->world_reflection_fbo_8u);

			// NOTE(rjf): Upload data
			{
				glBindBuffer(GL_ARRAY_BUFFER, global_ts2d->reflective_rect.instance_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, request->instance_data_size,
								global_ts2d->reflective_rect.instance_data + request->instance_data_offset);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_reflective_rect];
			glUseProgram(shader);
			glBindVertexArray(global_ts2d->reflective_rect.vao);
			{
				GLint first = 0;
				GLsizei count = 4;
				GLsizei instance_count = request->instance_data_size / global_ts2d->reflective_rect.instance_data_stride;
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, first, count, instance_count);
			}
			glBindVertexArray(0);
			Ts2dOpenGLFBOBind(last_fbo);
			break;
		}

		case TS2D_REQUEST_model:
		{
			Ts2dModelRequestData *model_data = request->data;
			v2 position = model_data->position;
			v2 size = model_data->size;
			Ts2dModel *model = model_data->model;
			b32 transform_with_skeleton = model_data->transform_with_skeleton;
			Ts2dSkeleton *skeleton = &model_data->skeleton;
			m3 transform = model_data->transform;
			float pixel_scale = model_data->pixel_scale;

			m4 view = M4LookAt(v3(10, 0, 0), v3(0, 0, 0), v3(0, 1, 0));
			m4 projection = M4Orthographic(-3.f, 3.f, -3.f, 3.f, 0.1f, 1000.f);
			m4 view_projection = M4MultiplyM4(projection, view);

			Ts2dOpenGLFBO *fbo = &global_ts2d->model_sprite_fbo;
			v4 offscreen_texture_rect = Rect2DQuadTreeAllocate(model_sprite_fbo_quad_tree,
															   sizeof(model_sprite_fbo_quad_tree),
															   v2((f32)fbo->w, (f32)fbo->h),
															   v2(size.x / pixel_scale, size.y / pixel_scale));

			Ts2dOpenGLFBO *last_active_fbo = global_ts2d->active_fbo;
			Ts2dOpenGLFBOBind(fbo);
			glViewport((GLint)(offscreen_texture_rect.x),
					   (GLint)(offscreen_texture_rect.y),
					   (GLint)(offscreen_texture_rect.width),
					   (GLint)(offscreen_texture_rect.height));

			// NOTE(rjf): Render model to off-screen texture as sprite.
			{
				glEnable(GL_DEPTH_TEST);
				GLuint shader = global_ts2d->shaders[TS2D_OPENGL_SHADER_model_sprite];
				glUseProgram(shader);

				v3 origin_shift = V3MultiplyF32(BoundingBoxCenter(model->model_space_bounding_box), -1);
				glUniform3f(glGetUniformLocation(shader, "origin_shift"),
							origin_shift.x, origin_shift.y, origin_shift.z);
				glUniformMatrix3fv(glGetUniformLocation(shader, "model_transform"),
								   1, GL_FALSE, &transform.elements[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(shader, "view_projection"),
								   1, GL_FALSE, &view_projection.elements[0][0]);

				glUniform1i(glGetUniformLocation(shader, "transform_with_bones"),
							(int)transform_with_skeleton);

				if (transform_with_skeleton)
				{
					char bone_transform_uniform_str_1_digit_because_opengl_drivers_are_awful[32] =
						"bone_transform[x]";
					char bone_transform_uniform_str_2_digit_because_opengl_drivers_are_awful[32] =
						"bone_transform[xx]";

					// NOTE(rjf): Transform + upload bones.
					for (int i = 0; i < ArrayCount(skeleton->bones); ++i)
					{
						Ts2dSkeletonBone *bone = skeleton->bones + i;

						m4 transform;
						MemoryCopy(&transform.elements[0][0], &bone->transform[0][0], sizeof(transform.elements));
						for (Ts2dSkeletonBone *parent = bone; parent;
							 parent->parent_index >= 0
								 ? skeleton->bones + parent->parent_index
								 : 0)
						{
							m4 next_transform;
							MemoryCopy(&next_transform.elements[0][0], &parent->transform[0][0], sizeof(next_transform.elements));
							transform = M4MultiplyM4(next_transform, transform);
						}

						char *bone_transform_uniform_str = i < 10 ? bone_transform_uniform_str_1_digit_because_opengl_drivers_are_awful : bone_transform_uniform_str_2_digit_because_opengl_drivers_are_awful;

						if (i < 10)
						{
							bone_transform_uniform_str[15] = '0' + i;
						}
						else
						{
							int ten_num = i / 10;
							bone_transform_uniform_str[15] = '0' + ten_num;
							bone_transform_uniform_str[16] = '0' + (i - ten_num * 10);
						}

						glUniformMatrix4fv(glGetUniformLocation(shader, bone_transform_uniform_str),
										   1, GL_FALSE, &transform.elements[0][0]);
					}
				}

				v3 shadow_vector =
					{
						-global_ts2d->shadow_vector.x,
						0,
						global_ts2d->shadow_vector.y,
					};

				shadow_vector = V3Normalize(shadow_vector);

				glUniform3f(glGetUniformLocation(shader, "shadow_vector"),
							shadow_vector.x, shadow_vector.y, shadow_vector.z);

				for (int i = 0; i < model->sub_model_count; ++i)
				{
					Ts2dSubModel *sub_model = model->sub_models + i;
					glBindVertexArray(sub_model->vao);
					glBindBuffer(GL_ARRAY_BUFFER, sub_model->vertex_buffer);

					int bytes_per_vertex = Ts2dGetBytesPerVertexWithFormat(sub_model->vertex_format);
					int offset = 0;

					// NOTE(rjf): Position
					if (sub_model->vertex_format & TS2D_VERTEX_POSITION)
					{
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bytes_per_vertex, (void *)(sizeof(float) * offset));
						offset += 3;
					}

					// NOTE(rjf): UV
					if (sub_model->vertex_format & TS2D_VERTEX_UV)
					{
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, bytes_per_vertex, (void *)(sizeof(float) * offset));
						offset += 2;
					}

					// NOTE(rjf): Normal
					if (sub_model->vertex_format & TS2D_VERTEX_NORMAL)
					{
						glEnableVertexAttribArray(2);
						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, bytes_per_vertex, (void *)(sizeof(float) * offset));
						offset += 3;
					}

					// NOTE(rjf): Bone Indices and Weights
					if (sub_model->vertex_format & TS2D_VERTEX_BONES)
					{
						glEnableVertexAttribArray(3);
						glVertexAttribIPointer(3, 4, GL_SHORT, bytes_per_vertex, (void *)(sizeof(float) * offset));
						offset += 2;

						glEnableVertexAttribArray(4);
						glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, bytes_per_vertex, (void *)(sizeof(float) * offset));
						offset += 4;
					}

					if (sub_model->index_buffer)
					{
						GLsizei count = sub_model->index_count;
						glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
					}
					else
					{
						GLsizei count = sub_model->vertex_count;
						glDrawArrays(GL_TRIANGLES, 0, count);
					}
				}
				glBindVertexArray(0);
				glUseProgram(0);
				glDisable(GL_DEPTH_TEST);
			}

			Ts2dOpenGLFBOBind(last_active_fbo);

			// NOTE(rjf): Render off-screen sprite texture as sprite.
			{
				Ts2dOpenGLFBORender(fbo, offscreen_texture_rect, v4(position.x, position.y, size.x, size.y));

				if (world_rendering_enabled)
				{
					// TODO(rjf): Write to reflection buffer.
				}
			}

			break;
		}

		case TS2D_REQUEST_debug_skeleton:
		{
			break;
		}

		default:
			break;
		}
	}

	// NOTE(rjf): Blit main FBO to the default framebuffer.
	{
		glDisable(GL_SCISSOR_TEST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, global_ts2d->pre_composite_fbo_8u.fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
						  0, 0, (int)global_ts2d->render_width, (int)global_ts2d->render_height,
						  GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glEnable(GL_SCISSOR_TEST);
	}

	GLenum error = glGetError();
	if (error)
	{
		LogError("[Ts2d] OpenGL Error: %i", (int)error);
	}
}

void Ts2dSwapBuffers(void)
{
	platform->RefreshScreen();
}

void Ts2dSetDefaultFont(Ts2dFont *font)
{
	global_ts2d->default_font = font;
}

Ts2dFont *
Ts2dGetDefaultFont(void)
{
	return global_ts2d->default_font;
}

void _Ts2dSetClip(v4 clip)
{
	Ts2dInternalFinishActiveRequest();
	global_ts2d->current_clip = clip;
	Ts2dRequestType request_type = TS2D_REQUEST_set_clip;
	global_ts2d->active_request.type = request_type;
	global_ts2d->active_request.data = _Ts2dAllocateRequestMemory(sizeof(v4));

	*(v4 *)global_ts2d->active_request.data = clip;
}

void Ts2dPushClip(v4 clip)
{
	if (global_ts2d->clip_stack_size < global_ts2d->clip_stack_max)
	{
		global_ts2d->clip_stack[global_ts2d->clip_stack_size++] = global_ts2d->current_clip;
		_Ts2dSetClip(clip);
	}
}

void Ts2dPopClip(void)
{
	if (global_ts2d->clip_stack_size > 0)
	{
		global_ts2d->current_clip = global_ts2d->clip_stack[--global_ts2d->clip_stack_size];
		_Ts2dSetClip(global_ts2d->current_clip);
	}
}

void Ts2dPushClipThatIsConstrainedByCurrent(v4 clip)
{
	v4 parent_clip = global_ts2d->current_clip;
	v4 new_clip = v4(MaximumF32(clip.x, parent_clip.x),
					 MaximumF32(clip.y, parent_clip.y),
					 clip.width, clip.height);
	if (clip.x + clip.width > parent_clip.x + parent_clip.width)
	{
		new_clip.width = parent_clip.width;
	}
	if (clip.y + clip.height > parent_clip.y + parent_clip.height)
	{
		new_clip.height = parent_clip.height;
	}
	Ts2dPushClip(new_clip);
}

void Ts2dPushRectangleBlur(v4 rect, f32 blur_magnitude)
{
	Ts2dInternalFinishActiveRequest();
	Ts2dRequestType request_type = TS2D_REQUEST_blur_rectangle;
	global_ts2d->active_request.type = request_type;
	global_ts2d->active_request.data = _Ts2dAllocateRequestMemory(sizeof(Ts2dBlurRequestData));

	rect.y = global_ts2d->render_height - rect.y - rect.height;
	((Ts2dBlurRequestData *)global_ts2d->active_request.data)->clip = rect;
	((Ts2dBlurRequestData *)global_ts2d->active_request.data)->blur_magnitude = blur_magnitude;
}

void Ts2dPushLine(v4 color, v2 p1, v2 p2)
{
	HardAssert(global_ts2d->line.instance_data_alloc_pos + global_ts2d->line.instance_data_stride <= global_ts2d->line.instance_data_max);
	Ts2dRequestType request_type = TS2D_REQUEST_line;
	if (global_ts2d->active_request.type != request_type)
	{
		Ts2dInternalFinishActiveRequest();
		global_ts2d->active_request.type = request_type;
		global_ts2d->active_request.instance_data_offset = global_ts2d->line.instance_data_alloc_pos;
		global_ts2d->active_request.instance_data_size = global_ts2d->line.instance_data_stride;
		global_ts2d->active_request.flags = 0;
	}
	else
	{
		global_ts2d->active_request.instance_data_size += global_ts2d->line.instance_data_stride;
	}
	GLubyte *data = (global_ts2d->line.instance_data + global_ts2d->line.instance_data_alloc_pos);
	((f32 *)data)[0] = +(2 * p1.x / global_ts2d->render_width - 1);
	((f32 *)data)[1] = -(2 * p1.y / global_ts2d->render_height - 1);
	((f32 *)data)[2] = +(2 * p2.x / global_ts2d->render_width - 1);
	((f32 *)data)[3] = -(2 * p2.y / global_ts2d->render_height - 1);
	((f32 *)data)[4] = color.r;
	((f32 *)data)[5] = color.g;
	((f32 *)data)[6] = color.b;
	((f32 *)data)[7] = color.a;
	global_ts2d->line.instance_data_alloc_pos += global_ts2d->line.instance_data_stride;
}

void Ts2dPushRect(v4 color, v4 rect)
{
	HardAssert(global_ts2d->rect.instance_data_alloc_pos + global_ts2d->rect.instance_data_stride <= global_ts2d->rect.instance_data_max);
	Ts2dRequestType request_type = TS2D_REQUEST_rect;
	if (global_ts2d->active_request.type != request_type)
	{
		Ts2dInternalFinishActiveRequest();
		global_ts2d->active_request.type = request_type;
		global_ts2d->active_request.instance_data_offset = global_ts2d->rect.instance_data_alloc_pos;
		global_ts2d->active_request.instance_data_size = global_ts2d->rect.instance_data_stride;
		global_ts2d->active_request.flags = 0;
	}
	else
	{
		global_ts2d->active_request.instance_data_size += global_ts2d->rect.instance_data_stride;
	}
	GLubyte *data = (global_ts2d->rect.instance_data + global_ts2d->rect.instance_data_alloc_pos);
	((f32 *)data)[0] = +(2 * (rect.x) / global_ts2d->render_width - 1);
	((f32 *)data)[1] = -(2 * (rect.y) / global_ts2d->render_height - 1);
	((f32 *)data)[2] = +(2 * (rect.width) / global_ts2d->render_width);
	((f32 *)data)[3] = -(2 * (rect.height) / global_ts2d->render_height);
	((f32 *)data)[4] = color.r;
	((f32 *)data)[5] = color.g;
	((f32 *)data)[6] = color.b;
	((f32 *)data)[7] = color.a;
	global_ts2d->rect.instance_data_alloc_pos += global_ts2d->rect.instance_data_stride;
}

void Ts2dPushFilledVertexColoredRect(v4 color00, v4 color01, v4 color10, v4 color11, v4 rect)
{
	HardAssert(global_ts2d->filled_rect.instance_data_alloc_pos + global_ts2d->filled_rect.instance_data_stride <= global_ts2d->filled_rect.instance_data_max);
	Ts2dRequestType request_type = TS2D_REQUEST_filled_rect;
	if (global_ts2d->active_request.type != request_type)
	{
		Ts2dInternalFinishActiveRequest();
		global_ts2d->active_request.type = request_type;
		global_ts2d->active_request.instance_data_offset = global_ts2d->filled_rect.instance_data_alloc_pos;
		global_ts2d->active_request.instance_data_size = global_ts2d->filled_rect.instance_data_stride;
		global_ts2d->active_request.flags = 0;
	}
	else
	{
		global_ts2d->active_request.instance_data_size += global_ts2d->filled_rect.instance_data_stride;
	}
	GLubyte *data = (global_ts2d->filled_rect.instance_data + global_ts2d->filled_rect.instance_data_alloc_pos);
	((f32 *)data)[0] = +(2 * rect.x / global_ts2d->render_width - 1);
	((f32 *)data)[1] = -(2 * rect.y / global_ts2d->render_height - 1);
	((f32 *)data)[2] = +(2 * rect.width / global_ts2d->render_width);
	((f32 *)data)[3] = -(2 * rect.height / global_ts2d->render_height);
	((f32 *)data)[4] = color00.r;
	((f32 *)data)[5] = color00.g;
	((f32 *)data)[6] = color00.b;
	((f32 *)data)[7] = color00.a;
	((f32 *)data)[8] = color01.r;
	((f32 *)data)[9] = color01.g;
	((f32 *)data)[10] = color01.b;
	((f32 *)data)[11] = color01.a;
	((f32 *)data)[12] = color10.r;
	((f32 *)data)[13] = color10.g;
	((f32 *)data)[14] = color10.b;
	((f32 *)data)[15] = color10.a;
	((f32 *)data)[16] = color11.r;
	((f32 *)data)[17] = color11.g;
	((f32 *)data)[18] = color11.b;
	((f32 *)data)[19] = color11.a;
	global_ts2d->filled_rect.instance_data_alloc_pos += global_ts2d->filled_rect.instance_data_stride;
}

void Ts2dPushFilledRect(v4 color, v4 rect)
{
	Ts2dPushFilledVertexColoredRect(color, color, color, color, rect);
}

void Ts2dPushTintedTextureWithFlags(Ts2dTexture *texture, i32 flags, v4 source, v4 destination, v4 tint)
{
	if (texture && texture->id)
	{
		HardAssert(global_ts2d->texture.instance_data_alloc_pos + global_ts2d->texture.instance_data_stride <= global_ts2d->texture.instance_data_max);
		Ts2dRequestType request_type = TS2D_REQUEST_texture;

		if (global_ts2d->active_request.type != request_type ||
			global_ts2d->active_request.flags != flags ||
			((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture != texture)
		{
			Ts2dInternalFinishActiveRequest();
			global_ts2d->active_request.type = request_type;
			global_ts2d->active_request.instance_data_offset = global_ts2d->texture.instance_data_alloc_pos;
			global_ts2d->active_request.instance_data_size = global_ts2d->texture.instance_data_stride;
			global_ts2d->active_request.flags = flags;
			global_ts2d->active_request.data = _Ts2dAllocateRequestMemory(sizeof(Ts2dTextureRequestData));
			((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture = texture;
		}
		else
		{
			global_ts2d->active_request.instance_data_size += global_ts2d->texture.instance_data_stride;
		}

		if (flags & TS2D_FLIP_HORIZONTAL)
		{
			source.x += source.width;
			source.width *= -1;
		}

		if (flags & TS2D_FLIP_VERTICAL)
		{
			source.y += source.height;
			source.height *= -1;
		}

		v2 scale =
			{
				destination.width / source.width,
				destination.height / source.height,
			};

		GLubyte *data = (global_ts2d->texture.instance_data + global_ts2d->texture.instance_data_alloc_pos);
		((f32 *)data)[0] = source.x;
		((f32 *)data)[1] = source.y;
		((f32 *)data)[2] = source.width;
		((f32 *)data)[3] = source.height;
		((f32 *)data)[4] = +(2 * destination.x / global_ts2d->render_width - 1);
		((f32 *)data)[5] = -(2 * (destination.y + source.height * scale.y) / global_ts2d->render_height - 1);
		((f32 *)data)[6] = scale.x;
		((f32 *)data)[7] = scale.y;
		((f32 *)data)[8] = tint.r;
		((f32 *)data)[9] = tint.g;
		((f32 *)data)[10] = tint.b;
		((f32 *)data)[11] = tint.a;
		global_ts2d->texture.instance_data_alloc_pos += global_ts2d->texture.instance_data_stride;
	}
	else
	{
		Ts2dPushFilledRect(v4(1, 0, 0, 1), destination);
	}
}

void Ts2dPushTintedTexture(Ts2dTexture *texture, v4 source, v4 destination, v4 tint)
{
	Ts2dPushTintedTextureWithFlags(texture, 0, source, destination, tint);
}

void Ts2dPushTextureWithFlags(Ts2dTexture *texture, i32 flags, v4 source, v4 destination)
{
	v4 tint = {1, 1, 1, 1};
	Ts2dPushTintedTextureWithFlags(texture, flags, source, destination, tint);
}

void Ts2dPushTexture(Ts2dTexture *texture, v4 source, v4 destination)
{
	v4 tint = {1, 1, 1, 1};
	Ts2dPushTintedTextureWithFlags(texture, 0, source, destination, tint);
}

void Ts2dPushWorldTile(Ts2dTexture *texture, iv2 source, iv2 position)
{
	if (texture && texture->id)
	{
		HardAssert(global_ts2d->world_tile.instance_data_alloc_pos + global_ts2d->world_tile.instance_data_stride <= global_ts2d->world_tile.instance_data_max);
		Ts2dRequestType request_type = TS2D_REQUEST_world_tile;

		if (global_ts2d->active_request.type != request_type ||
			((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture != texture)
		{
			Ts2dInternalFinishActiveRequest();
			global_ts2d->active_request.type = request_type;
			global_ts2d->active_request.instance_data_offset = global_ts2d->world_tile.instance_data_alloc_pos;
			global_ts2d->active_request.instance_data_size = global_ts2d->world_tile.instance_data_stride;
			global_ts2d->active_request.flags = 0;
			global_ts2d->active_request.data = _Ts2dAllocateRequestMemory(sizeof(Ts2dTextureRequestData));
			((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture = texture;
		}
		else
		{
			global_ts2d->active_request.instance_data_size += global_ts2d->world_tile.instance_data_stride;
		}

		GLubyte *data = (global_ts2d->world_tile.instance_data + global_ts2d->world_tile.instance_data_alloc_pos);
		((u8 *)data)[0] = (u8)source.x;
		((u8 *)data)[1] = (u8)source.y;
		((u8 *)data)[2] = (u8)position.x;
		((u8 *)data)[3] = (u8)position.y;
		global_ts2d->world_tile.instance_data_alloc_pos += global_ts2d->world_tile.instance_data_stride;
	}
}

f32 Ts2dPushTextWithBoldnessAndSoftnessN(Ts2dFont *font, i32 flags, v4 color, v2 position,
										 f32 font_scale, f32 boldness, f32 softness, char *text, u32 n)
{
	f32 text_width = 0;

	if (font && font->texture.id)
	{
		v2 char_pos = position;

		if (flags & TS2D_TEXT_ALIGN_CENTER_X)
		{
			f32 text_width = Ts2dFontGetTextWidth(font, text);
			char_pos.x -= (text_width / 2) * font_scale;
		}
		else if (flags & TS2D_TEXT_ALIGN_RIGHT_X)
		{
			f32 text_width = Ts2dFontGetTextWidth(font, text);
			char_pos.x -= (text_width)*font_scale;
		}

		if (flags & TS2D_TEXT_ALIGN_CENTER_Y)
		{
			char_pos.y -= (font->line_height / 2) * font_scale;
		}

		for (u32 c = 0; text[c] && c < n; ++c)
		{
			Ts2dFontGlyph *glyph = 0;

			if ((u32)text[c] >= font->glyph_lower_bound_character && (u32)text[c] < font->glyph_lower_bound_character + font->glyph_count)
			{
				glyph = font->glyphs + (u32)text[c] - font->glyph_lower_bound_character;
			}

			if (glyph)
			{
				f32 x_advance = (glyph->x_advance) * font_scale;
				if (text[c] > 32)
				{
					HardAssert(global_ts2d->text_char.instance_data_alloc_pos + global_ts2d->text_char.instance_data_stride <= global_ts2d->text_char.instance_data_max);
					Ts2dRequestType request_type = TS2D_REQUEST_text;

					if (global_ts2d->active_request.type != request_type ||
						((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture != &font->texture)
					{
						Ts2dInternalFinishActiveRequest();
						global_ts2d->active_request.type = request_type;
						global_ts2d->active_request.instance_data_offset = global_ts2d->text_char.instance_data_alloc_pos;
						global_ts2d->active_request.instance_data_size = global_ts2d->text_char.instance_data_stride;
						global_ts2d->active_request.flags = 0;
						global_ts2d->active_request.data = _Ts2dAllocateRequestMemory(sizeof(Ts2dTextureRequestData));
						((Ts2dTextureRequestData *)global_ts2d->active_request.data)->texture = &font->texture;
					}
					else
					{
						global_ts2d->active_request.instance_data_size += global_ts2d->text_char.instance_data_stride;
					}

					f32 char_x = (f32)glyph->x;
					f32 char_y = (f32)glyph->y;
					f32 char_w = (f32)glyph->width;
					f32 char_h = (f32)glyph->height;
					f32 x_offset = glyph->x_offset * font_scale;
					f32 y_offset = glyph->y_offset * font_scale;

					v4 source =
						{
							char_x,
							char_y,
							char_w,
							char_h,
						};

					v4 destination =
						{
							char_pos.x + x_offset,
							char_pos.y + y_offset,
							char_w * font_scale,
							char_h * font_scale,
						};

					destination.x = +(2 * destination.x / global_ts2d->render_width - 1);
					destination.y = -(2 * destination.y / global_ts2d->render_height - 1);
					destination.width = +(2 * destination.width / global_ts2d->render_width);
					destination.height = -(2 * destination.height / global_ts2d->render_height);

					GLubyte *data = (global_ts2d->text_char.instance_data + global_ts2d->text_char.instance_data_alloc_pos);
					((f32 *)data)[0] = source.x;
					((f32 *)data)[1] = source.y;
					((f32 *)data)[2] = source.width;
					((f32 *)data)[3] = source.height;
					((f32 *)data)[4] = destination.x;
					((f32 *)data)[5] = destination.y;
					((f32 *)data)[6] = destination.width;
					((f32 *)data)[7] = destination.height;
					((f32 *)data)[8] = color.r;
					((f32 *)data)[9] = color.g;
					((f32 *)data)[10] = color.b;
					((f32 *)data)[11] = color.a;
					((f32 *)data)[12] = boldness;
					((f32 *)data)[13] = softness;
					((f32 *)data)[14] = 0;
					((f32 *)data)[15] = 0;

					global_ts2d->text_char.instance_data_alloc_pos += global_ts2d->text_char.instance_data_stride;
				}
				char_pos.x += x_advance;
				text_width += x_advance;
			}
		}
	}

	return text_width;
}

f32 Ts2dPushTextN(Ts2dFont *font, i32 flags, v4 color, v2 position, f32 font_scale, char *text, u32 n)
{
	f32 boldness = 0.62f + (0.12f * (1.f - font_scale));
	f32 softness = 0.10f + (0.22f * (1.f - font_scale));
	return Ts2dPushTextWithBoldnessAndSoftnessN(font, flags, color, position, font_scale, boldness,
												softness, text, n);
}

f32 Ts2dPushText(Ts2dFont *font, i32 flags, v4 color, v2 position, f32 font_scale, char *text)
{
	return Ts2dPushTextN(font, flags, color, position, font_scale, text, (u32)(-1));
}

f32 Ts2dPushTextWithBoldnessAndSoftness(Ts2dFont *font, i32 flags, v4 color, v2 position, f32 font_scale, f32 boldness, f32 softness, char *text)
{
	return Ts2dPushTextWithBoldnessAndSoftnessN(font, flags, color, position, font_scale, boldness,
												softness, text, (u32)(-1));
}

void Ts2dPushPointLight(v2 position, v3 color, f32 radius, f32 intensity)
{
	HardAssert(global_ts2d->light_count < TS2D_MAX_LIGHT_COUNT);
	u32 i = global_ts2d->light_count++;
	global_ts2d->lights[i].position = position;
	global_ts2d->lights[i].color = color;
	global_ts2d->lights[i].radius = radius;
	global_ts2d->lights[i].intensity = intensity;
}

void Ts2dPushWorldBegin(Ts2dWorldInfo *info)
{
	Ts2dInternalFinishActiveRequest();
	Ts2dRequestType request_type = TS2D_REQUEST_begin_world;
	global_ts2d->active_request.type = request_type;

	// NOTE(rjf): Load data from info struct.
	{
		global_ts2d->flags |= info->flags;
		global_ts2d->shadow_opacity = info->shadow_opacity;
		global_ts2d->shadow_vector = info->shadow_vector;
		global_ts2d->camera_pos = info->camera_pos;
		global_ts2d->camera_rotation = info->camera_rotation;
		global_ts2d->camera_zoom = info->camera_zoom;
		global_ts2d->brightness = info->brightness;
		global_ts2d->grayscale = info->grayscale;
	}
}

void Ts2dPushBackgroundBegin(void)
{
	Ts2dInternalFinishActiveRequest();
	Ts2dRequestType request_type = TS2D_REQUEST_begin_background;
	global_ts2d->active_request.type = request_type;
}

void Ts2dPushWorldEnd(void)
{
	Ts2dInternalFinishActiveRequest();
	Ts2dRequestType request_type = TS2D_REQUEST_end_world;
	global_ts2d->active_request.type = request_type;
}

void Ts2dPushBackgroundEnd(void)
{
	Ts2dInternalFinishActiveRequest();
	Ts2dRequestType request_type = TS2D_REQUEST_end_background;
	global_ts2d->active_request.type = request_type;
}

void Ts2dPushReflectiveRect(v4 rect, v4 color, f32 distortion, f32 distortion_time_factor)
{
	Ts2dPushFilledRect(color, rect);

	HardAssert(global_ts2d->reflective_rect.instance_data_alloc_pos + global_ts2d->reflective_rect.instance_data_stride <= global_ts2d->reflective_rect.instance_data_max);
	Ts2dRequestType request_type = TS2D_REQUEST_reflective_rect;
	if (global_ts2d->active_request.type != request_type)
	{
		Ts2dInternalFinishActiveRequest();
		global_ts2d->active_request.type = request_type;
		global_ts2d->active_request.instance_data_offset = global_ts2d->reflective_rect.instance_data_alloc_pos;
		global_ts2d->active_request.instance_data_size = global_ts2d->reflective_rect.instance_data_stride;
		global_ts2d->active_request.flags = 0;
	}
	else
	{
		global_ts2d->active_request.instance_data_size += global_ts2d->reflective_rect.instance_data_stride;
	}

	GLubyte *data = (global_ts2d->reflective_rect.instance_data + global_ts2d->reflective_rect.instance_data_alloc_pos);
	((f32 *)data)[0] = +(2 * rect.x / global_ts2d->render_width - 1);
	((f32 *)data)[1] = -(2 * rect.y / global_ts2d->render_height - 1);
	((f32 *)data)[2] = +(2 * rect.width / global_ts2d->render_width);
	((f32 *)data)[3] = -(2 * rect.height / global_ts2d->render_height);
	((f32 *)data)[4] = color.r;
	((f32 *)data)[5] = color.g;
	((f32 *)data)[6] = color.b;
	((f32 *)data)[7] = color.a;
	((f32 *)data)[8] = distortion;
	((f32 *)data)[9] = distortion_time_factor;
	((f32 *)data)[10] = 0;
	((f32 *)data)[11] = 0;
	global_ts2d->reflective_rect.instance_data_alloc_pos += global_ts2d->reflective_rect.instance_data_stride;
}

void Ts2dPushModel(Ts2dModel *model, v2 position, v2 size, m3 transform, float pixel_scale)
{
	Ts2dRequestType request_type = TS2D_REQUEST_model;
	Ts2dInternalFinishActiveRequest();
	global_ts2d->active_request.type = request_type;
	global_ts2d->active_request.instance_data_offset = 0;
	global_ts2d->active_request.instance_data_size = 0;
	global_ts2d->active_request.flags = 0;
	Ts2dModelRequestData *request_data = _Ts2dAllocateRequestMemory(sizeof(*request_data));
	request_data->position = position;
	request_data->size = size;
	request_data->model = model;
	request_data->transform_with_skeleton = 0;
	request_data->transform = transform;
	request_data->pixel_scale = pixel_scale;
	global_ts2d->active_request.data = request_data;
}

void Ts2dPushModelWithSkeleton(Ts2dModel *model, Ts2dSkeleton *skeleton, v2 position, v2 size, m3 transform, float pixel_scale)
{
	Ts2dRequestType request_type = TS2D_REQUEST_model;
	Ts2dInternalFinishActiveRequest();
	global_ts2d->active_request.type = request_type;
	global_ts2d->active_request.instance_data_offset = 0;
	global_ts2d->active_request.instance_data_size = 0;
	global_ts2d->active_request.flags = 0;
	Ts2dModelRequestData *request_data = _Ts2dAllocateRequestMemory(sizeof(*request_data));
	request_data->position = position;
	request_data->size = size;
	request_data->model = model;
	request_data->transform_with_skeleton = 1;
	MemoryCopy(&request_data->skeleton, skeleton, sizeof(*skeleton));
	request_data->transform = transform;
	request_data->pixel_scale = pixel_scale;
	global_ts2d->active_request.data = request_data;
}

void Ts2dPushDebugSkeleton(Ts2dSkeleton *skeleton, v2 position, v2 size, m3 transform, float pixel_scale)
{
	Ts2dRequestType request_type = TS2D_REQUEST_debug_skeleton;
	Ts2dInternalFinishActiveRequest();
	global_ts2d->active_request.type = request_type;
	global_ts2d->active_request.instance_data_offset = 0;
	global_ts2d->active_request.instance_data_size = 0;
	global_ts2d->active_request.flags = 0;
	Ts2dSkeletonRequestData *request_data = _Ts2dAllocateRequestMemory(sizeof(*request_data));
	request_data->position = position;
	request_data->size = size;
	MemoryCopy(&request_data->skeleton, skeleton, sizeof(*skeleton));
	request_data->transform = transform;
	request_data->pixel_scale = pixel_scale;
	global_ts2d->active_request.data = request_data;
}