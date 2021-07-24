
TsRenderPrefix(Texture)
TsRenderPrefix(TextureInit)(TsRenderPrefix(TextureFormat) format, int width, int height, void *data)
{
    return TsRenderPrefix(TextureInitFlags)((TsRenderPrefix(TextureFlags))0, format, width, height, data);
}

TsRenderPrefix(Texture)
TsRenderPrefix(TextureInitFlags)(TsRenderPrefix(TextureFlags) flags, TsRenderPrefix(TextureFormat) format,
                                 int width, int height, void *data)
{
    TsRenderPrefix(Texture) texture = {0};
    texture.flags = flags;
    texture.format = format;
    texture.width = width;
    texture.height = height;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    if(format == TsRenderPrefixUpper(TEXTURE_FORMAT_R8G8B8A8))
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

void
TsRenderPrefix(TextureCleanUp)(TsRenderPrefix(Texture) *texture)
{
    if(texture && texture->id)
    {
        glDeleteTextures(1, &texture->id);
        texture->id = 0;
    }
}

b32
TsRenderPrefix(TextureIsValid)(TsRenderPrefix(Texture) *texture)
{
    return texture->id != 0;
}

void
TsRenderPrefix(TextureSetFlags)(TsRenderPrefix(Texture) *texture, TsRenderPrefix(TextureFlags) flags)
{
    texture->flags = flags;
}

TsRenderPrefix(Font)
TsRenderPrefix(FontInit)(TsRenderPrefix(TextureFormat) format, int texture_width, int texture_height,
                         void *texture_data, int size, int line_height, u32 glyph_count,
                         TsRenderPrefix(FontGlyph) *glyphs, u32 glyph_lower_bound_character)
{
    TsRenderPrefix(Font) font = {0};
    font.texture = TsRenderPrefix(TextureInitFlags)(0, format, texture_width, texture_height, texture_data);
    font.size = (i16)size;
    font.line_height = (i16)line_height;
    font.glyph_count = glyph_count;
    font.glyphs = platform->HeapAlloc(sizeof(TsRenderPrefix(FontGlyph))*glyph_count);
    MemoryCopy(font.glyphs, glyphs, sizeof(TsRenderPrefix(FontGlyph))*glyph_count);
    font.glyph_lower_bound_character = glyph_lower_bound_character;
    return font;
}

void
TsRenderPrefix(FontCleanUp)(TsRenderPrefix(Font) *font)
{
    if(font)
    {
        TsRenderPrefix(TextureCleanUp)(&font->texture);
        platform->HeapFree(font->glyphs);
    }
}

f32
TsRenderPrefix(FontGetLineHeight)(TsRenderPrefix(Font) *font)
{
    return font ? (f32)font->line_height : 0;
}

f32
TsRenderPrefix(FontGetTextWidthN)(TsRenderPrefix(Font) *font, char *text, u32 n)
{
    f32 text_width = 0.f;
    
    if(font == 0)
    {
        font = TsRenderPrefix(GetDefaultFont)();
    }
    
    if(font)
    {
        for(u32 i = 0; text[i] && i < n; ++i)
        {
            if((u32)text[i] >= font->glyph_lower_bound_character && (u32)text[i] < font->glyph_lower_bound_character+font->glyph_count)
            {
                text_width += (f32)font->glyphs[text[i] - font->glyph_lower_bound_character].x_advance;
            }
        }
    }
    return text_width;
}

f32
TsRenderPrefix(FontGetTextWidth)(TsRenderPrefix(Font) *font, char *text)
{
    return TsRenderPrefix(FontGetTextWidthN)(font, text, (u32)-1);
}

TsRenderPrefix(Material)
TsRenderPrefix(MaterialInit)(TsRenderPrefix(Texture) *albedo)
{
    TsRenderPrefix(Material) material = {0};
    material.albedo_texture = albedo;
    return material;
}

TsRenderPrefix(Material)
TsRenderPrefix(MaterialInitSimple)(v3 color)
{
    TsRenderPrefix(Material) material = {0};
    material.backing_color = v4(color.r, color.g, color.b, 1);
    return material;
}

void
TsRenderPrefix(MaterialCleanUp)(TsRenderPrefix(Material) *material)
{
    if(material && material->albedo_texture)
    {
        TsRenderPrefix(TextureCleanUp)(material->albedo_texture);
    }
}

TsRenderPrefix(SubModel)
TsRenderPrefix(SubModelInitSimple)(TsRenderPrefix(VertexDataFormat) format, int vertex_count, f32 *vertex_data,
                                   TsRenderPrefix(Material) *material)
{
    return TsRenderPrefix(SubModelInit)(format, vertex_count, vertex_data, 0, 0, material);
}

TsRenderPrefix(SubModel)
TsRenderPrefix(SubModelInit)(TsRenderPrefix(VertexDataFormat) format, int vertex_count,
                             f32 *vertex_data, int index_count, i32 *index_data,
                             TsRenderPrefix(Material) *material)
{
    TsRenderPrefix(SubModel) sub_model = {0};
    sub_model.vertex_format = format;
    glGenVertexArrays(1, &sub_model.vao);
    glBindVertexArray(sub_model.vao);
    {
        int bytes_per_vertex = TsRenderPrefix(GetBytesPerVertexWithFormat)(format);
		
        sub_model.vertex_count = vertex_count;
        glGenBuffers(1, &sub_model.vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, sub_model.vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * bytes_per_vertex, vertex_data, GL_STATIC_DRAW);
        
        if(index_data)
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
    if(format & TsRenderPrefixUpper(VERTEX_POSITION))
    {
        int floats_per_vertex = TsRenderPrefix(GetFloatsPerVertexWithFormat)(format);
        for(int i = 0; i < vertex_count; ++i)
        {
            v3 vertex =
            {
                vertex_data[i*floats_per_vertex + 0],
                vertex_data[i*floats_per_vertex + 1],
                vertex_data[i*floats_per_vertex + 2],
            };
            
            if(i == 0 || vertex.x < sub_model.model_space_bounding_box.min.x)
            {
                sub_model.model_space_bounding_box.min.x = vertex.x;
            }
            
            if(i == 0 || vertex.y < sub_model.model_space_bounding_box.min.y)
            {
                sub_model.model_space_bounding_box.min.y = vertex.y;
            }
            
            if(i == 0 || vertex.z < sub_model.model_space_bounding_box.min.z)
            {
                sub_model.model_space_bounding_box.min.z = vertex.z;
            }
            
            if(i == 0 || vertex.x > sub_model.model_space_bounding_box.max.x)
            {
                sub_model.model_space_bounding_box.max.x = vertex.x;
            }
            
            if(i == 0 || vertex.y > sub_model.model_space_bounding_box.max.y)
            {
                sub_model.model_space_bounding_box.max.y = vertex.y;
            }
            
            if(i == 0 || vertex.z > sub_model.model_space_bounding_box.max.z)
            {
                sub_model.model_space_bounding_box.max.z = vertex.z;
            }
            
        }
    }
    
    return sub_model;
}

TsRenderPrefix(SubModel)
TsRenderPrefix(SubModelInitFromTSM)(TSM *tsm)
{
	TsRenderPrefix(SubModel) sub_model = {0};
    sub_model.vertex_format = TS2D_VERTEX_POSITION | TS2D_VERTEX_UV | TS2D_VERTEX_NORMAL;// | TS2D_VERTEX_BONES;
    glGenVertexArrays(1, &sub_model.vao);
    glBindVertexArray(sub_model.vao);
    {
        int bytes_per_vertex = TsRenderPrefix(GetBytesPerVertexWithFormat)(sub_model.vertex_format);
		
		sub_model.vertex_count = tsm->vert_count;
		glGenBuffers(1, &sub_model.vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, sub_model.vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sub_model.vertex_count * bytes_per_vertex, tsm->vertices, GL_STATIC_DRAW);
		
		sub_model.index_count = tsm->index_count;
		glGenBuffers(1, &sub_model.index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_model.index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tsm->indices), tsm->indices, GL_STATIC_DRAW);
		
		//sub_model.material = material;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // NOTE(rjf): Calculate bounding box of this sub-model.
    // TODO(rjf): Is this a problem?
	if(sub_model.vertex_format & TsRenderPrefixUpper(VERTEX_POSITION))
	{
		int floats_per_vertex = TsRenderPrefix(GetFloatsPerVertexWithFormat)(sub_model.vertex_format);
		for(int i = 0; i < tsm->vert_count; ++i)
		{
			v3 vertex = tsm->vertices[i].position;
			
			if(i == 0 || vertex.x < sub_model.model_space_bounding_box.min.x)
			{
				sub_model.model_space_bounding_box.min.x = vertex.x;
			}
			
			if(i == 0 || vertex.y < sub_model.model_space_bounding_box.min.y)
			{
				sub_model.model_space_bounding_box.min.y = vertex.y;
			}
			
			if(i == 0 || vertex.z < sub_model.model_space_bounding_box.min.z)
			{
				sub_model.model_space_bounding_box.min.z = vertex.z;
			}
			
			if(i == 0 || vertex.x > sub_model.model_space_bounding_box.max.x)
			{
				sub_model.model_space_bounding_box.max.x = vertex.x;
			}
			
			if(i == 0 || vertex.y > sub_model.model_space_bounding_box.max.y)
			{
				sub_model.model_space_bounding_box.max.y = vertex.y;
			}
			
			if(i == 0 || vertex.z > sub_model.model_space_bounding_box.max.z)
			{
				sub_model.model_space_bounding_box.max.z = vertex.z;
			}
			
		}
	}
	
	return sub_model;
}

TsRenderPrefix(Model)
TsRenderPrefix(ModelInit)(int sub_model_count, TsRenderPrefix(SubModel) *sub_models,
                          int skeleton_count, TsRenderPrefix(Skeleton) *skeletons)
{
    TsRenderPrefix(Model) model = {0};
    model.sub_model_count = sub_model_count;
    model.sub_models = platform->HeapAlloc(sizeof(*sub_models) * sub_model_count);
    model.skeleton_count = skeleton_count;
    if(skeleton_count > 0)
    {
        model.skeletons = platform->HeapAlloc(sizeof(TsRenderPrefix(Skeleton)) * skeleton_count);
    }
    
    HardAssert(model.sub_models != 0);
    if(model.sub_models)
    {
        MemoryCopy(model.sub_models, sub_models, sizeof(*sub_models) * sub_model_count);
        
        // NOTE(rjf): Calculate bounding box.
        // TODO(rjf): Is this a problem?
        {
            v3 min = {0};
            v3 max = {0};
            
            for(int i = 0; i < sub_model_count; ++i)
            {
                
                if(i == 0 || min.x < sub_models[i].model_space_bounding_box.min.x)
                {
                    min.x = sub_models[i].model_space_bounding_box.min.x;
                }
                
                if(i == 0 || min.y < sub_models[i].model_space_bounding_box.min.y)
                {
                    min.y = sub_models[i].model_space_bounding_box.min.y;
                }
                
                if(i == 0 || min.z < sub_models[i].model_space_bounding_box.min.z)
                {
                    min.z = sub_models[i].model_space_bounding_box.min.z;
                }
                
                if(i == 0 || max.x > sub_models[i].model_space_bounding_box.max.x)
                {
                    max.x = sub_models[i].model_space_bounding_box.max.x;
                }
                
                if(i == 0 || max.y > sub_models[i].model_space_bounding_box.max.y)
                {
                    max.y = sub_models[i].model_space_bounding_box.max.y;
                }
                
                if(i == 0 || max.z > sub_models[i].model_space_bounding_box.max.z)
                {
                    max.z = sub_models[i].model_space_bounding_box.max.z;
                }
                
            }
            
            model.model_space_bounding_box.min = min;
            model.model_space_bounding_box.max = max;
        }
    }
    
    if(model.skeletons)
    {
        MemoryCopy(model.skeletons, skeletons, sizeof(*skeletons) * skeleton_count);
    }
    
    return model;
}

void
TsRenderPrefix(SubModelCleanUp)(TsRenderPrefix(SubModel) *sub_model)
{
    if(sub_model->vao)
    {
        glDeleteVertexArrays(1, &sub_model->vao);
        sub_model->vao = 0;
    }
    
    if(sub_model->vertex_buffer)
    {
        glDeleteBuffers(1, &sub_model->vertex_buffer);
        sub_model->vertex_buffer = 0;
    }
    
    if(sub_model->index_buffer)
    {
        glDeleteBuffers(1, &sub_model->index_buffer);
        sub_model->index_buffer = 0;
    }
    
    TsRenderPrefix(MaterialCleanUp)(sub_model->material);
}

void
TsRenderPrefix(ModelCleanUp)(TsRenderPrefix(Model) *model)
{
    for(int i = 0; i < model->sub_model_count; ++i)
    {
        TsRenderPrefix(SubModelCleanUp)(&model->sub_models[i]);
    }
    
    model->sub_model_count = 0;
    if(model->sub_models)
    {
        platform->HeapFree(model->sub_models);
        model->sub_models = 0;
    }
    
    model->skeleton_count = 0;
    if(model->skeletons)
    {
        platform->HeapFree(model->skeletons);
        model->skeletons = 0;
    }
}

static GLuint
TsOpenGLInitShaderFromData(char *shader_name,
                           TsOpenGLShaderInput *inputs, int input_count,
                           TsOpenGLShaderOutput *outputs, int output_count,
                           char *vertex, char *fragment)
{
    Log("[TsOpenGL] Building shader \"%s\".", shader_name);
    
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
            if(info_log_length > 1)
            {
                char vertex_shader_error[1024] = {0};
                if(info_log_length > sizeof(vertex_shader_error)-1)
                {
                    info_log_length = sizeof(vertex_shader_error)-1;
                }
                glGetShaderInfoLog(vertex_shader, info_log_length, 0, vertex_shader_error);
                LogError("[TsOpenGL] %s", vertex_shader_error);
            }
            else
            {
                Log("[TsOpenGL] Vertex shader compiled successfully.");
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
            if(info_log_length > 1)
            {
                char fragment_shader_error[1024] = {0};
                if(info_log_length > sizeof(fragment_shader_error)-1)
                {
                    info_log_length = sizeof(fragment_shader_error)-1;
                }
                glGetShaderInfoLog(fragment_shader, info_log_length, 0, fragment_shader_error);
                LogError("[TsOpenGL] %s", fragment_shader_error);
            }
            else
            {
                Log("[TsOpenGL] Fragment shader compiled successfully.");
            }
        }
    }
    
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    
    // NOTE(rjf): Specify shader inputs
    {
        for(int i = 0; i < input_count; ++i)
        {
            glBindAttribLocation(program, inputs[i].index, inputs[i].name);
        }
    }
    
    // NOTE(rjf): Specify shader outputs
    {
        for(int i = 0; i < output_count; ++i)
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

TsOpenGLFBO
TsOpenGLFBOInit(unsigned int width, unsigned int height, i32 flags)
{
    TsOpenGLFBO fbo = {0};
    fbo.flags = flags;
    fbo.width = width;
    fbo.height = height;
    glGenFramebuffers(1, &fbo.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
    {
        GLenum colors[4] = {0};
        unsigned int color_count = 0;
        
        for(int i = 0; i < sizeof(fbo.color_textures)/sizeof(fbo.color_textures[0]); ++i)
        {
            if(flags & (TS_OPENGL_FBO_COLOR_OUT_0 << i))
            {
                glGenTextures(1, fbo.color_textures+i);
                glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
                colors[color_count++] = GL_COLOR_ATTACHMENT0+i;
            }
            else if(flags & (TS_OPENGL_FBO_COLOR_OUT_0_16F << i))
            {
                glGenTextures(1, fbo.color_textures+i);
                glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
                colors[color_count++] = GL_COLOR_ATTACHMENT0+i;
            }
            else if(flags & (TS_OPENGL_FBO_COLOR_OUT_0_32F << i))
            {
                glGenTextures(1, fbo.color_textures+i);
                glBindTexture(GL_TEXTURE_2D, fbo.color_textures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, fbo.color_textures[i], 0);
                colors[color_count++] = GL_COLOR_ATTACHMENT0+i;
            }
        }
        
        if(flags & TS_OPENGL_FBO_DEPTH_OUT)
        {
            glGenTextures(1, &fbo.depth_texture);
            glBindTexture(GL_TEXTURE_2D, fbo.depth_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo.depth_texture, 0);
        }
        
        if(color_count)
        {
            glDrawBuffers(color_count, colors);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo;
}

void
TsOpenGLFBOCleanUp(TsOpenGLFBO *fbo)
{
    glDeleteFramebuffers(1, &fbo->fbo);
    for(unsigned int i = 0; i < ArrayCount(fbo->color_textures); ++i)
    {
        if(fbo->color_textures[i])
        {
            glDeleteTextures(1, &fbo->color_textures[i]);
        }
    }
    if(fbo->depth_texture)
    {
        glDeleteTextures(1, &fbo->depth_texture);
    }
    fbo->fbo = 0;
}

void
TsOpenGLFBOForceSize(TsOpenGLFBO *fbo, unsigned int w, unsigned int h, i32 flags)
{
    if(fbo->w != w || fbo->h != h)
    {
        TsOpenGLFBOCleanUp(fbo);
        *fbo = TsOpenGLFBOInit(w, h, flags);
    }
}

void
TsOpenGLFBOBind(TsOpenGLFBO *fbo)
{
    if(fbo != TsRenderStruct()->active_fbo)
    {
        if(fbo)
        {
            glViewport(0, 0, (GLsizei)fbo->w, (GLsizei)fbo->h);
            glScissor(0, 0, (GLsizei)fbo->w, (GLsizei)fbo->h);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            
#if defined(TS2D)
            glViewport(0, 0, (GLsizei)TsRenderStruct()->render_width, (GLsizei)TsRenderStruct()->render_height);
            glScissor(0, 0, (GLsizei)TsRenderStruct()->render_width, (GLsizei)TsRenderStruct()->render_height);
#elif defined(TS3D)
            glViewport((GLsizei)global_ts3d->viewport.x,
                       (GLsizei)global_ts3d->viewport.y,
                       (GLsizei)global_ts3d->viewport.width,
                       (GLsizei)global_ts3d->viewport.height);
            glScissor((GLsizei)global_ts3d->viewport.x,
                      (GLsizei)global_ts3d->viewport.y,
                      (GLsizei)global_ts3d->viewport.width,
                      (GLsizei)global_ts3d->viewport.height);
#endif
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        TsRenderStruct()->active_fbo = fbo;
    }
}

void
TsOpenGLFBOClear(TsOpenGLFBO *fbo)
{
    TsOpenGLFBO *last_fbo = TsRenderStruct()->active_fbo;
    TsOpenGLFBOBind(fbo);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    if(fbo->depth_texture)
    {
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    TsOpenGLFBOBind(last_fbo);
}

void
_TsOpenGLFBORender(GLuint shader, TsOpenGLFBO *fbo, v4 source, v4 dest)
{
    glBindVertexArray(TsRenderStruct()->all_purpose_vao);
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
            dest.width  / source.width,
            dest.height / source.height,
        };
        
#if defined(TS2D)
        dest.x      = +(2 * dest.x / TsRenderStruct()->render_width  - 1 );
        dest.y      = -(2 * dest.y / TsRenderStruct()->render_height - 1 );
        dest.width  = +(2 * dest.width / TsRenderStruct()->render_width  );
        dest.height = -(2 * dest.height / TsRenderStruct()->render_height);
#elif defined(TS3D)
        dest.x      = +(2 * dest.x / TsRenderStruct()->viewport.width  - 1 );
        dest.y      = -(2 * dest.y / TsRenderStruct()->viewport.height - 1 );
        dest.width  = +(2 * dest.width / TsRenderStruct()->viewport.width  );
        dest.height = -(2 * dest.height / TsRenderStruct()->viewport.height);
#endif
        
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

void
TsOpenGLFBORender(TsOpenGLFBO *fbo, v4 source, v4 dest)
{
    _TsOpenGLFBORender(TsRenderStruct()->shaders[TS_OPENGL_SHADER_fbo], fbo, source, dest);
}

void
TsOpenGLBlurredFBORender(GLuint texture, f32 width, f32 height,
                         b32 vertical, f32 standard_deviation, i32 radius, v4 clip)
{
    GLuint shader = TsRenderStruct()->shaders[TS_OPENGL_SHADER_gaussian_blur];
    glBindVertexArray(TsRenderStruct()->all_purpose_vao);
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
        
        for(i32 i = kernel_lower_bound;
            i >= 0 &&
            i <= kernel_upper_bound &&
            i <= kernel_size;
            ++i)
        {
            
            f32 x = (f32)(kernel_midpoint - i);
            
            vec4_buffer_because_glsl_sucks[vec4_counter_because_glsl_sucks++] =
                (TsRenderPrefixUpper(ONE_OVER_SQUARE_ROOT_OF_TWO_PIf) / standard_deviation) *
                powf(TsRenderPrefixUpper(EULERS_NUMBERf), -(x * x) / (2 * standard_deviation * standard_deviation));
            
            char *kernel_uniform_str = 0;
            i32 kernel_vec4_index_because_glsl_sucks = i / 4;
            
            if(kernel_vec4_index_because_glsl_sucks < 10)
            {
                kernel_uniform_str = kernel_uniform_str_1_digit_index_because_glsl_sucks;
                kernel_uniform_str[7] = '0' + kernel_vec4_index_because_glsl_sucks;
            }
            else if(kernel_vec4_index_because_glsl_sucks < 100)
            {
                kernel_uniform_str = kernel_uniform_str_2_digit_index_because_glsl_sucks;
                snprintf(kernel_uniform_str + 7, 4, "%i]", kernel_vec4_index_because_glsl_sucks);
            }
            
            if(kernel_uniform_str)
            {
                if(vec4_counter_because_glsl_sucks >= 4)
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
_TsOpenGLSetInstancedFloatAttributeData(int index, int stride, int size, int offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,
                          (void *)(sizeof(f32)*offset));
    glVertexAttribDivisor(index, 1);
}
