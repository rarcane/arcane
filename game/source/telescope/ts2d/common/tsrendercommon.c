
TsRenderPrefix(Texture)
TsRenderPrefix(TextureInitFromPNGData)(void *png_data, u32 png_data_size)
{
    TsRenderPrefix(Texture) texture = {0};
    int texture_width = 0;
    int texture_height = 0;
    int channels = 0;
    void *texture_data = stbi_load_from_memory(png_data, png_data_size, &texture_width, &texture_height, &channels, 4);
    texture = TsRenderPrefix(TextureInit)(TsRenderPrefixUpper(TEXTURE_FORMAT_R8G8B8A8), texture_width, texture_height, texture_data);
    stbi_image_free(texture_data);
    return texture;
}

TsRenderPrefix(Texture)
TsRenderPrefix(TextureLoad)(char *path)
{
    TsRenderPrefix(Texture) texture = {0};
    void *png_data = 0;
    u32 png_data_len = 0;
    platform->LoadEntireFile(path, &png_data, &png_data_len, 0);
    if(png_data && png_data_len)
    {
        texture = TsRenderPrefix(TextureInitFromPNGData)(png_data, png_data_len);
        platform->FreeFileMemory(png_data);
    }
    return texture;
}

TsRenderPrefix(Font)
TsRenderPrefix(FontInitFromPNGAndFNTData)(void *png_data, u32 png_data_size, char *font_data)
{
    TsRenderPrefix(Font) font = {0};
    
    TsRenderPrefix(FontGlyph) glyphs[256];
    u32 glyph_count = ArrayCount(glyphs);
    u32 glyph_lower_bound_character = 0;
    
    int texture_width = 0;
    int texture_height = 0;
    void *texture_data = 0;
    texture_data = stbi_load_from_memory(png_data, png_data_size, &texture_width, &texture_height, 0, STBI_rgb_alpha);
    
    enum
    {
        READ_NONE = 0,
        READ_NAME,
        READ_VALUE,
        MAX_READ
    };
    char *data = font_data;
    char name[32] = {0};
    char value[32] = {0};
    i32 read_state = 0;
    b32 commented = 0;
    u32 read_pos = 0;
    TsRenderPrefix(FontGlyph) *active_glyph = 0;
    
    int size = 0;
    int line_height = 0;
    
    for(u32 i = 0; data[i];)
    {
        if(data[i] == '#')
        {
            commented = 1;
        }
        if(commented)
        {
            if(data[i] == '\n')
            {
                commented = 0;
            }
            i++;
        }
        else
        {
            switch(read_state)
            {
                case READ_NONE:
                {
                    if(CharIsAlpha(data[i]))
                    {
                        read_state = READ_NAME;
                        read_pos = i;
                    }
                    else
                    {
                        i++;
                    }
                    break;
                }
                case READ_NAME:
                {
                    if(data[i] == '=')
                    {
                        i16 name_len =
                            (i16)(i + 1 - read_pos < 32 ? i + 1 - read_pos : 31);
                        strncpy(name, data + read_pos, name_len);
                        name[name_len - 1] = '\0';
                        read_state = READ_VALUE;
                        read_pos = ++i;
                    }
                    else
                    {
                        i++;
                    }
                    break;
                }
                case READ_VALUE:
                {
                    if(CharIsSpace((int)data[i]) || data[i+1] == 0)
                    {
                        i16 value_len = (i16)(i + 1 - read_pos < 32 ? i + 1 - read_pos : 31);
                        strncpy(value, data + read_pos, value_len);
                        value[value_len - 1] = '\0';
                        if(CStringMatchCaseSensitive(name, "char id"))
                        {
                            u32 char_index = (u32)GetFirstI32FromCString(value);
                            if(char_index >= glyph_lower_bound_character && char_index < glyph_lower_bound_character + glyph_count)
                            {
                                active_glyph = glyphs + char_index - glyph_lower_bound_character;
                            }
                        }
                        else if(CStringMatchCaseSensitive(name, "size"))
                        {
                            size = GetFirstI32FromCString(value);
                        }
                        else if(CStringMatchCaseSensitive(name, "lineHeight"))
                        {
                            line_height = GetFirstI32FromCString(value);
                        }
                        else if(active_glyph)
                        {
                            if(CStringMatchCaseSensitive(name, "x"))
                            {
                                active_glyph->x = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "y"))
                            {
                                active_glyph->y = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "width"))
                            {
                                active_glyph->width = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "height"))
                            {
                                active_glyph->height = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "xoffset"))
                            {
                                active_glyph->x_offset = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "yoffset"))
                            {
                                active_glyph->y_offset = (i16)GetFirstI32FromCString(value);
                            }
                            else if(CStringMatchCaseSensitive(name, "xadvance"))
                            {
                                active_glyph->x_advance = (i16)GetFirstI32FromCString(value) - 16;
                            }
                        }
                        
                        read_state = READ_NONE;
                    }
                    else
                    {
                        i++;
                    }
                    break;
                }
                default:
                {
                    i++;
                    break;
                }
            }
        }
    }
    
    font = TsRenderPrefix(FontInit)(TsRenderPrefixUpper(TEXTURE_FORMAT_R8G8B8A8),
                                    texture_width, texture_height, texture_data,
                                    size, line_height, glyph_count,
                                    glyphs, glyph_lower_bound_character);
    
    return font;
}

TsRenderPrefix(Font)
TsRenderPrefix(FontLoad)(char *png_path, char *fnt_path)
{
    TsRenderPrefix(Font) font = {0};
    
    void *png_data = 0;
    u32 png_data_len = 0;
    
    platform->LoadEntireFile(png_path, &png_data, &png_data_len, 0);
    char *font_data = platform->LoadEntireFileAndNullTerminate(fnt_path);
    
    if(png_data && font_data)
    {
        font = TsRenderPrefix(FontInitFromPNGAndFNTData)(png_data, png_data_len, font_data);
    }
    
    if(png_data)
    {
        platform->FreeFileMemory(png_data);
    }
    
    if(font_data)
    {
        platform->FreeFileMemory(font_data);
    }
    
    return font;
}


TsRenderPrefix(Model)
TsRenderPrefix(ModelInitFromTSMData)(void *data, u32 data_size)
{
    TsRenderPrefix(Model) model = {0};
    
    if(data)
    {
        MemoryArena read_ = MemoryArenaInitAligned(data, data_size, 1);
        MemoryArena *read = &read_;
        
        int sub_model_count = 0;
        TsRenderPrefix(SubModel) *sub_models = 0;
        
        int skeleton_count = 0;
        TsRenderPrefix(Skeleton) *skeletons = 0;
        
        // NOTE(rjf): Read header.
        {
            char expected_model_header[8] =
            {
                't', 's', 'm', 'f', 'm', 't', '2', '7',
            };
            char model_header[8];
            
            if(!MemoryArenaRead(read, model_header, sizeof(model_header)))
            {
                goto end_read;
            }
            
            if(MemoryMatch(expected_model_header, model_header, sizeof(model_header)))
            {
            }
            else
            {
                goto end_read;
            }
        }
        
        // NOTE(rjf): Read model metadata.
        {
            i32 number_of_sub_models = 0;
            if(!MemoryArenaRead(read, &number_of_sub_models, sizeof(number_of_sub_models)))
            {
                goto end_read;
            }
            sub_model_count = number_of_sub_models;
            sub_models = platform->HeapAlloc(sub_model_count * sizeof(*sub_models));
            
            i32 number_of_skeletons = 0;
            if(!MemoryArenaRead(read, &number_of_skeletons, sizeof(number_of_skeletons)))
            {
                goto end_read;
            }
            skeleton_count = number_of_skeletons;
            skeletons = platform->HeapAlloc(skeleton_count * sizeof(*skeletons));
        }
        
        // NOTE(rjf): Read sub-models.
        for(int i = 0; i < sub_model_count; ++i)
        {
            TsRenderPrefix(SubModel) *sub_model = sub_models + i;
            
            // NOTE(rjf): Read sub-model format flags.
            TsRenderPrefix(VertexDataFormat) format = 0;
            HardAssert(MemoryArenaRead(read, &format, sizeof(format)));
            
            // NOTE(rjf): Read vertices.
            u32 vertex_count = 0;
            HardAssert(MemoryArenaRead(read, &vertex_count, sizeof(vertex_count)));
            f32 *vertices = MemoryArenaAllocate(read, vertex_count * TsRenderPrefix(GetBytesPerVertexWithFormat)(format));
            
            // NOTE(rjf): Read indices.
            u32 index_count = 0;
            HardAssert(MemoryArenaRead(read, &index_count, sizeof(index_count)));
            i32 *indices = 0;
            if(index_count != 0)
            {
                indices = MemoryArenaAllocate(read, index_count * sizeof(i32));
            }
            
            // NOTE(rjf): Read material.
            TsRenderPrefix(Material) *material = 0;
            {
#define TS_MODEL_FILE_MATERIAL_REFERENCE_NONE 0
                u32 material_reference_type = 0;
                HardAssert(MemoryArenaRead(read, &material_reference_type, sizeof(material_reference_type)));
                
                switch(material_reference_type)
                {
                    default:
                    case TS_MODEL_FILE_MATERIAL_REFERENCE_NONE:
                    {
                        break;
                    }
                }
            }
            
            // NOTE(rjf): Initialize sub-model.
            if(indices)
            {
                *sub_model = TsRenderPrefix(SubModelInit)(format, (int)vertex_count, vertices, (int)index_count,
                                                          indices, material);
            }
            else
            {
                *sub_model = TsRenderPrefix(SubModelInitSimple)(format, (int)vertex_count, vertices,
                                                                material);
            }
        }
        
        // NOTE(rjf): Read skeletons.
        for(int i = 0; i < skeleton_count; ++i)
        {
            TsRenderPrefix(Skeleton) *skeleton = skeletons + i;
            
            i32 bone_count = 0;
            HardAssert(MemoryArenaRead(read, &bone_count, sizeof(bone_count)));
            
            for(int j = 0; j < bone_count; ++j)
            {
                i32 parent_index = -1;
                HardAssert(MemoryArenaRead(read, &parent_index, sizeof(parent_index)));
                skeleton->bones[j].parent_index = parent_index;
                HardAssert(MemoryArenaRead(read, &skeleton->bones[j].transform,
                                           sizeof(skeleton->bones[j].transform)));
            }
        }
        
        model = TsRenderPrefix(ModelInit)(sub_model_count, sub_models, skeleton_count, skeletons);
        
        end_read:;
    }
    
    return model;
}

TsRenderPrefix(Model)
TsRenderPrefix(ModelInitFromOBJData)(char *data)
{
    TsRenderPrefix(Model) model = {0};
    
    OBJParseInfo info = {0};
    {
        info.obj_data           = data;
        info.parse_memory_size  = platform->scratch_arena.memory_left;
        info.parse_memory       = MemoryArenaAllocate(&platform->scratch_arena, 0);
    }
    ParsedOBJ obj = ParseOBJ(&info);
    
    if(obj.renderable_count != 0)
    {
        TsRenderPrefix(SubModel) *sub_models = platform->HeapAlloc(obj.renderable_count *
                                                                   sizeof(*sub_models));
        
        for(unsigned int i = 0; i < obj.renderable_count; ++i)
        {
            ParsedOBJRenderable *renderable = obj.renderables + i;
            sub_models[i] = TsRenderPrefix(SubModelInit)(TsRenderPrefixUpper(VERTEX_POSITION) |
                                                         TsRenderPrefixUpper(VERTEX_UV) |
                                                         TsRenderPrefixUpper(VERTEX_NORMAL) |
                                                         TsRenderPrefixUpper(VERTEX_INTERLEAVED),
                                                         renderable->vertex_count,
                                                         renderable->vertices,
                                                         renderable->index_count,
                                                         renderable->indices,
                                                         0);
        }
        
        model = TsRenderPrefix(ModelInit)(obj.renderable_count, sub_models, 0, 0);
    }
    
    return model;
}

TsRenderPrefix(Model)
TsRenderPrefix(ModelLoad)(char *path)
{
    TsRenderPrefix(Model) model = {0};
    void *data = 0;
    u32 data_len = 0;
    
    if(CStringEndsWithSuffix(path, ".tsm"))
    {
        platform->LoadEntireFile(path, &data, &data_len, 0);
        if(data && data_len)
        {
            model = TsRenderPrefix(ModelInitFromTSMData)(data, data_len);
            platform->FreeFileMemory(data);
        }
    }
    else if(CStringEndsWithSuffix(path, ".obj"))
    {
        char *data = platform->LoadEntireFileAndNullTerminate(path);
        if(data)
        {
            model = TsRenderPrefix(ModelInitFromOBJData)(data);
            platform->FreeFileMemory(data);
        }
    }
    
    return model;
}
