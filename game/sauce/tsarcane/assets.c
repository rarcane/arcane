internal void
PreLoadTs2dTexture(TsAssets *assets, char *name, Ts2dTexture *texture)
{
    Log("[Assets] Preloading texture \"%s\" into %p.", name, texture);
}

internal void
PreLoadTs2dFont(TsAssets *assets, char *name, Ts2dFont *font)
{
    Log("[Assets] Preloading font \"%s\" into %p.", name, font);
}

internal void
LoadTs2dTexture(TsAssets *assets, char *name, Ts2dTextureLoadInfo *load_info)
{
    void *png_data = 0;
    u32 png_data_len = 0;
    char png_path[256] = {0};
    snprintf(png_path, sizeof(png_path), "%s.png", name);
    platform->LoadEntireFile(png_path, &png_data, &png_data_len, 0);
    if(png_data)
    {
        InitTs2dTextureLoadInfoFromPNGData(load_info, png_data, png_data_len);
        platform->FreeFileMemory(png_data);
    }
}

internal void
LoadTs2dFont(TsAssets *assets, char *name, Ts2dFontLoadInfo *load_info)
{
    void *png_data = 0;
    u32 png_data_len = 0;
    
    char png_path[256] = {0};
    char fnt_path[256] = {0};
    snprintf(png_path, sizeof(png_path), "%s.png", name);
    snprintf(fnt_path, sizeof(fnt_path), "%s.fnt", name);
    
    platform->LoadEntireFile(png_path, &png_data, &png_data_len, 0);
    char *font_data = platform->LoadEntireFileAndNullTerminate(fnt_path);
    
    if(png_data && font_data)
    {
        InitTs2dFontLoadInfoFromPNGAndFNTData(load_info, png_data, png_data_len, font_data);
    }
    
    if(png_data)
    {
        platform->FreeFileMemory(png_data);
    }
    
    if(font_data)
    {
        platform->FreeFileMemory(font_data);
    }
}

internal void
PostLoadTs2dTexture(TsAssets *assets, Ts2dTextureLoadInfo *texture_load_info, Ts2dTexture *texture)
{
    Log("[Assets] Postloading texture %p.", texture);
    texture->width = texture_load_info->width;
    texture->height = texture_load_info->height;
    *texture = Ts2dTextureInit(core->renderer, TS2D_TEXTURE_FORMAT_R8G8B8A8, texture->width,
                               texture->height, texture_load_info->texture_data);
    free(texture_load_info->texture_data);
}

internal void
PostLoadTs2dFont(TsAssets *assets, Ts2dFontLoadInfo *load_info, Ts2dFont *font)
{
    Log("[Assets] Postloading font %p.", font);
    *font = Ts2dFontInit(core->renderer, TS2D_TEXTURE_FORMAT_R8G8B8A8,
                         load_info->texture_load_info.width,
                         load_info->texture_load_info.height,
                         load_info->texture_load_info.texture_data,
                         load_info->size,
                         load_info->line_height,
                         load_info->glyph_count,
                         load_info->glyphs,
                         load_info->glyph_lower_bound_character);
}

internal void
CleanUpTs2dTexture(TsAssets *assets, Ts2dTexture *texture)
{
    if(texture->id)
    {
        Ts2dTextureCleanUp(core->renderer, texture);
    }
}

internal void
CleanUpTs2dFont(TsAssets *assets, Ts2dFont *font)
{
    if(font->texture.id)
    {
        CleanUpTs2dTexture(assets, &font->texture);
    }
}

internal int
IsTs2dTextureLoaded(Ts2dTexture *texture)
{
    return texture->id != 0;
}

internal int
IsTs2dFontLoaded(Ts2dFont *font)
{
    return IsTs2dTextureLoaded(&font->texture);
}

internal void
InitTs2dTextureLoadInfoFromPNGData(Ts2dTextureLoadInfo *texture_load_info, void *data, u32 len)
{
    int w, h;
    u8 *tex_data = stbi_load_from_memory((unsigned char *)data, (int)len, &w, &h, 0, STBI_rgb_alpha);
    texture_load_info->texture_data = tex_data;
    texture_load_info->width = (i16)w;
    texture_load_info->height = (i16)h;
}

internal void
InitTs2dFontLoadInfoFromPNGAndFNTData(Ts2dFontLoadInfo *font_load_info, void *png_data, u32 png_data_len, char *font_data)
{
    InitTs2dTextureLoadInfoFromPNGData(&font_load_info->texture_load_info, png_data, png_data_len);
    
    Ts2dFontGlyph *glyphs = font_load_info->glyphs;
    u32 glyph_count = ArrayCount(font_load_info->glyphs);
    u32 glyph_lower_bound_character = 32;
    
    int size = 0;
    int line_height = 0;
    
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
    Ts2dFontGlyph *active_glyph = 0;
    
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
    
    font_load_info->size = size;
    font_load_info->line_height = line_height;
    font_load_info->glyph_count = glyph_count;
    font_load_info->glyph_lower_bound_character = glyph_lower_bound_character;
}
