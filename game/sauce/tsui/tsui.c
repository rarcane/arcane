/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

TsUIID
TsUIIDInit(i32 primary, i32 secondary)
{
    TsUIID id = {primary, secondary};
    return id;
}

TsUIID
TsUIIDNull(void)
{
    return TsUIIDInit(-1, -1);
}

TsUIID
TsUIIDNonInteractable(void)
{
    return TsUIIDInit(-2, -2);
}

b32
TsUIIDEqual(TsUIID id1, TsUIID id2)
{
    return (id1.primary == id2.primary && id1.secondary == id2.secondary);
}

b32
TsUIIDIsNull(TsUIID id)
{
    return TsUIIDEqual(id, TsUIIDNull());
}

TsUIID
TsUIGenerateID(TsUI *ui, char *text)
{
    i32 primary_id = CStringCRC32(text) % TSUI_WIDGET_MAX;
    i32 secondary_id = ui->widget_id_counters[primary_id]++;
    return TsUIIDInit(primary_id, secondary_id);
}

v4
TsUIGetWindowScrollbarRectangle(TsUIWindow *window)
{
    v4 scrollbar_rect = {0};
    {
        scrollbar_rect.x = window->rect.x + window->rect.width - 16;
        scrollbar_rect.y = window->rect.y + ((window->flags & TSUI_WINDOW_FLAG_NO_TITLE_BAR) ? 0.f : 31.f);
        scrollbar_rect.width = 16;
        scrollbar_rect.height = window->rect.height - (scrollbar_rect.y - window->rect.y);
    }
    return scrollbar_rect;
}

v4
TsUIGetWindowScrollbarHandleRectangle(TsUIWindow *window)
{
    v4 scrollbar_rect = TsUIGetWindowScrollbarRectangle(window);
    v4 handle_rect = {0};
    {
        handle_rect.x = scrollbar_rect.x;
        handle_rect.height = window->rect.height * (window->rect.height / (window->target_view_max.y + window->rect.height));
        handle_rect.y = scrollbar_rect.y + (scrollbar_rect.height - handle_rect.height) * window->target_view_offset.y / window->target_view_max.y;
        handle_rect.width = scrollbar_rect.width;
    }
    return handle_rect;
}

typedef struct TsUINote TsUINote;
struct TsUINote
{
    i32 note;
    v4 rect;
};

void
TsUIGetNotePickerKeys(v4 widget_rect,
                      TsUINote *white_notes, u32 white_note_count,
                      TsUINote *black_notes, u32 black_note_count)
{
    v4 white_key_rect =
    {
        widget_rect.x,
        widget_rect.y,
        widget_rect.width / white_note_count,
        widget_rect.height,
    };
    
    i32 note_val = 0;
    
    for(u32 i = 0; i < white_note_count; ++i)
    {
        white_notes[i].note = note_val;
        if(note_val % 12 == 4 || note_val % 12 == 11)
        {
            ++note_val;
        }
        else
        {
            note_val += 2;
        }
        white_notes[i].rect = white_key_rect;
        white_notes[i].rect.x += i * white_key_rect.width;
    }
    
    v4 black_key_rect = {
        widget_rect.x + white_key_rect.width - white_key_rect.width / 4,
        widget_rect.y,
        white_key_rect.width / 2,
        widget_rect.height * (1.15f / 2.f)
    };
    
    note_val = 1;
    
    for(u32 i = 0; i < black_note_count; ++i)
    {
        black_notes[i].note = note_val;
        black_notes[i].rect = black_key_rect;
        black_key_rect.x += white_key_rect.width;
        if(note_val % 12 == 3 || note_val % 12 == 10)
        {
            black_key_rect.x += white_key_rect.width;
            ++note_val;
        }
        note_val += 2;
    }
}

v4
TsUIGetColorPickerHueSelectorRect(v4 widget_rect)
{
    v4 rect =
    {
        widget_rect.x + widget_rect.width - widget_rect.width / 8,
        widget_rect.y,
        widget_rect.width / 8,
        widget_rect.height - widget_rect.height / 8
    };
    return rect;
}

v4
TsUIGetColorPickerSaturationValueSelectorRect(v4 widget_rect)
{
    v4 rect =
    {
        widget_rect.x,
        widget_rect.y,
        widget_rect.width - widget_rect.width / 8,
        widget_rect.height - widget_rect.height / 8
    };
    return rect;
}

v4
TsUIGetColorPickerExtraInfoRect(v4 widget_rect)
{
    v4 rect =
    {
        widget_rect.x,
        widget_rect.y + widget_rect.height - widget_rect.height / 8,
        widget_rect.width,
        widget_rect.height / 8,
    };
    return rect;
}

// NOTE(rjf): Default Ts2d renderer calls.
#ifdef TS2D_H_INCLUDED
f32
TsUIGetTextWidthDefaultTs2d(TsUI *ui, i32 style_flags, f32 text_scale, char *text, void *user_data)
{
    Ts2d *renderer = user_data;
    Ts2dFont *font = Ts2dGetDefaultFont(renderer);
    return Ts2dFontGetTextWidth(font, text) * text_scale;
}

f32
TsUIGetTextHeightDefaultTs2d(TsUI *ui, i32 style_flags, f32 text_scale, char *text, void *user_data)
{
    Ts2d *renderer = user_data;
    Ts2dFont *font = Ts2dGetDefaultFont(renderer);
    return font->line_height * text_scale;
}

void
TsUIRenderWidgetDefaultTs2d(TsUI *ui, i32 style_flags, TsUIWidget *widget, v4 clip, void *user_data, b32 widget_is_hot, b32 widget_is_active)
{
    Ts2d *renderer = user_data;
    
    Ts2dPushClipThatIsConstrainedByCurrent(renderer, clip);
    Ts2dPushClipThatIsConstrainedByCurrent(renderer, widget->clip);
    
    f32 widget_x = widget->rect.x;
    f32 widget_y = widget->rect.y;
    f32 widget_w = widget->rect.width;
    f32 widget_h = widget->rect.height;
    char *widget_text = widget->text.data;
    
    Ts2dFont *font = Ts2dGetDefaultFont(renderer);
    
    switch(widget->type)
    {
        case TSUI_WIDGET_button:
        case TSUI_WIDGET_toggler:
        {
            Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.8f), widget->rect);
            if(widget_is_hot)
            {
                Ts2dPushFilledRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f),
                                   v4(widget_x, widget_y, widget_w, widget_h));
            }
            if(widget_is_active)
            {
                Ts2dPushFilledRect(renderer, v4(0.4f, 0.4f, 0.4f, 0.4f),
                                   v4(widget_x, widget_y, widget_w, widget_h));
            }
            Ts2dPushRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f), widget->rect);
            
            Ts2dPushText(renderer,
                         font,
                         TS2D_TEXT_ALIGN_CENTER_Y,
                         v4(1, 1, 1, 1),
                         v2(widget_x + 16, widget_y + widget_h / 2 + 4),
                         widget->text_scale,
                         widget_text);
            
            if(widget->type == TSUI_WIDGET_toggler)
            {
                
                f32 check_size = widget->rect.height - 2 * widget->rect.height / 4;
                
                if(widget->toggler.toggled)
                {
                    Ts2dPushFilledRect(renderer, v4(0.7f, 0.55f, 0.2f, 0.7f),
                                       v4(widget->rect.x + widget->rect.width - check_size - check_size/2,
                                          widget->rect.y + widget->rect.height/2 - check_size/2,
                                          check_size, check_size));
                }
                
                Ts2dPushRect(renderer, v4(0.7f, 0.7f, 0.7f, 0.7f),
                             v4(widget->rect.x + widget->rect.width - check_size - check_size/2,
                                widget->rect.y + widget->rect.height/2 - check_size/2,
                                check_size, check_size));
            }
            
            break;
        }
        
        case TSUI_WIDGET_slider:
        case TSUI_WIDGET_int_slider:
        {
            char *slider_text = "";
            f32 fill_percentage = 0.f;
            
            if(widget->type == TSUI_WIDGET_slider)
            {
                slider_text = MakeCStringOnMemoryArena(ui->widget_arena, "%s (%.2f)",
                                                       widget->text.data,
                                                       widget->slider.value);
                fill_percentage = widget->slider.percentage;
            }
            else if(widget->type == TSUI_WIDGET_int_slider)
            {
                slider_text = MakeCStringOnMemoryArena(ui->widget_arena, "%s (%i)",
                                                       widget->text.data,
                                                       widget->int_slider.value);
                fill_percentage = (f32)(widget->int_slider.value - widget->int_slider.low) / (widget->int_slider.high - widget->int_slider.low);
            }
            
            Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.8f), widget->rect);
            if(widget_is_hot)
            {
                Ts2dPushFilledRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f),
                                   v4(widget_x, widget_y, widget_w, widget_h));
            }
            Ts2dPushRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f), widget->rect);
            
            Ts2dPushFilledRect(renderer,
                               v4(0.7f, 0.55f, 0.2f, 0.7f),
                               v4(widget_x, widget_y,
                                  widget_w * fill_percentage, widget_h));
            
            Ts2dPushText(renderer,
                         font,
                         TS2D_TEXT_ALIGN_CENTER_Y,
                         v4(1, 1, 1, 1),
                         v2(widget_x + 16,
                            widget_y + widget_h / 2 + 4),
                         widget->text_scale,
                         slider_text);
            
            break;
        }
        
        case TSUI_WIDGET_line_edit:
        {
            char *widget_edit_text = widget->line_edit.edit_text.data;
            
            Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.8f), widget->rect);
            if(widget_is_hot)
            {
                Ts2dPushFilledRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f),
                                   v4(widget_x, widget_y, widget_w, widget_h));
            }
            Ts2dPushRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f), widget->rect);
            
            Ts2dPushClipThatIsConstrainedByCurrent(renderer, widget->rect);
            if(widget->line_edit.edit_text.data[0] || widget_is_active)
            {
                f32 text_width = Ts2dFontGetTextWidth(font, widget_edit_text) * 0.26f;
                
                f32 target_view_offset = 0.f;
                if(text_width > widget->rect.width - 32)
                {
                    target_view_offset = text_width - (widget->rect.width - 32);
                }
                widget->line_edit.view_offset += ui->delta_t * 24.f * (target_view_offset - widget->line_edit.view_offset);
                
                Ts2dPushText(renderer,
                             font,
                             TS2D_TEXT_ALIGN_CENTER_Y,
                             v4(1, 1, 1, 1),
                             v2(widget_x + 16 - widget->line_edit.view_offset,
                                widget_y + widget_h / 2 + 4),
                             widget->text_scale,
                             widget_edit_text);
                
                if(widget_is_active)
                {
                    ui->caret_x_offset += ui->delta_t * 16.f * (text_width - ui->caret_x_offset);
                    if(AbsoluteValue(text_width - ui->caret_x_offset) >= 0.1f)
                    {
                        platform->pump_events = 1;
                    }
                }
                
                Ts2dPushText(renderer,
                             font,
                             TS2D_TEXT_ALIGN_CENTER_Y,
                             V4MultiplyF32(v4(1, 1, 1, 1), widget->active_transition),
                             v2(widget_x + 16 + ui->caret_x_offset - widget->line_edit.view_offset,
                                widget_y + widget_h / 2 + 4),
                             widget->text_scale,
                             "|");
            }
            else
            {
                Ts2dPushText(renderer,
                             font,
                             TS2D_TEXT_ALIGN_CENTER_Y,
                             v4(0.5f, 0.5f, 0.5f, 0.5f),
                             v2(widget_x + 16,
                                widget_y + widget_h / 2 + 4),
                             widget->text_scale,
                             widget_text);
            }
            
            Ts2dPopClip(renderer);
            
            break;
        }
        
        case TSUI_WIDGET_window_close_button:
        {
            Ts2dPushClip(renderer, v4(0, 0, ui->render_width, ui->render_height));
            Ts2dPushText(renderer, font,
                         TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
                         v4(0.5f + 0.5f * widget_is_hot,
                            0.5f - 0.3f * widget_is_hot,
                            0.5f - 0.3f * widget_is_hot,
                            0.5f + 0.5f * widget_is_hot),
                         v2(widget_x + widget_w/2,
                            widget_y + widget_h/2 + 2.5f),
                         widget->text_scale + 0.03f * widget_is_hot - 0.08f * widget_is_active,
                         widget_text);
            Ts2dPopClip(renderer);
            break;
        }
        
#if 0
        case TSUI_WIDGET_image_button:
        {
            Ts2dTexture *texture = widget->image_button.texture;
            v4 source = widget->image_button.source;
            v4 destination = widget->rect;
            Ts2dPushTintedTexture(renderer, texture, source, destination,
                                  v4u(0.6f + widget->hot_transition * 0.4f));
            break;
        }
#endif
        
        case TSUI_WIDGET_color_picker:
        {
            v4 widget_rect = {widget_x, widget_y, widget_w, widget_h};
            v4 h_selector_rect = TsUIGetColorPickerHueSelectorRect(widget_rect);
            v4 sv_selector_rect = TsUIGetColorPickerSaturationValueSelectorRect(widget_rect);
            
            // NOTE(rjf): Render Saturation/Value Quad
            {
                v3 full_sv_rgb = HSVToRGB(v3(widget->color_picker.hsv.x, 1, 1));
                
                f32 r = full_sv_rgb.r;
                f32 g = full_sv_rgb.g;
                f32 b = full_sv_rgb.b;
                
                v4 color00 = {1, 1, 1, 1};
                v4 color01 = {0, 0, 0, 1};
                v4 color10 = {r, g, b, 1};
                v4 color11 = {0, 0, 0, 1};
                
                Ts2dPushFilledVertexColoredRect(renderer, color00, color01, color10, color11, sv_selector_rect);
            }
            
            // NOTE(rjf): Render Hue Selection
            {
                v2 hue_ranges[] =
                {
                    {0.f   , 60.f  },
                    {60.f  , 120.f },
                    {120.f , 180.f },
                    {180.f , 240.f },
                    {240.f , 300.f },
                    {300.f , 360.f },
                };
                
                for(u32 i = 0; i < ArrayCount(hue_ranges); ++i)
                {
                    hue_ranges[i].x /= 360.f;
                    hue_ranges[i].y /= 360.f;
                }
                
                f32 hue_range_rectangle_height = h_selector_rect.height / ArrayCount(hue_ranges);
                
                for(u32 i = 0; i < ArrayCount(hue_ranges); ++i)
                {
                    v3 top_rgb = HSVToRGB(v3(hue_ranges[i].x, 1, 1));
                    v3 bottom_rgb = HSVToRGB(v3(hue_ranges[i].y, 1, 1));
                    
                    v4 color00 = {top_rgb.x, top_rgb.y, top_rgb.z, 1};
                    v4 color01 = {bottom_rgb.x, bottom_rgb.y, bottom_rgb.z, 1};
                    v4 color10 = {top_rgb.x, top_rgb.y, top_rgb.z, 1};
                    v4 color11 = {bottom_rgb.x, bottom_rgb.y, bottom_rgb.z, 1};
                    
                    Ts2dPushFilledVertexColoredRect(renderer, color00, color01, color10, color11,
                                                    v4(h_selector_rect.x,
                                                       h_selector_rect.y + hue_range_rectangle_height * i,
                                                       h_selector_rect.width,
                                                       hue_range_rectangle_height));
                }
            }
            
            // NOTE(rjf): Render color information
            {
                v4 extra_info_rect = TsUIGetColorPickerExtraInfoRect(widget_rect);
                
                Ts2dPushFilledRect(renderer,
                                   v4(1, 1, 1, 1),
                                   v4(extra_info_rect.x + 1,
                                      extra_info_rect.y + 1,
                                      extra_info_rect.height - 2,
                                      extra_info_rect.height - 2));
                
                Ts2dPushFilledRect(renderer, v4(widget->color_picker.rgb.r,
                                                widget->color_picker.rgb.g,
                                                widget->color_picker.rgb.b,
                                                1),
                                   v4(extra_info_rect.x + 2, extra_info_rect.y + 2,
                                      extra_info_rect.height - 4, extra_info_rect.height - 4));
                
                char r_text[16] = {0};
                char g_text[16] = {0};
                char b_text[16] = {0};
                
                snprintf(r_text, sizeof(r_text), "R: %i", (int)((u8)(widget->color_picker.rgb.r * 255.f)));
                snprintf(g_text, sizeof(g_text), "G: %i", (int)((u8)(widget->color_picker.rgb.g * 255.f)));
                snprintf(b_text, sizeof(b_text), "B: %i", (int)((u8)(widget->color_picker.rgb.b * 255.f)));
                
                Ts2dPushText(renderer, font, TS2D_TEXT_ALIGN_CENTER_Y, v4(1, 1, 1, 1),
                             v2(extra_info_rect.x + extra_info_rect.height + 16,
                                extra_info_rect.y + extra_info_rect.height / 2 + 4),
                             0.25f,
                             r_text);
                
                Ts2dPushText(renderer, font, TS2D_TEXT_ALIGN_CENTER_Y, v4(1, 1, 1, 1),
                             v2(extra_info_rect.x + extra_info_rect.height + 80,
                                extra_info_rect.y + extra_info_rect.height / 2 + 4),
                             0.25f,
                             g_text);
                
                Ts2dPushText(renderer, font, TS2D_TEXT_ALIGN_CENTER_Y, v4(1, 1, 1, 1),
                             v2(extra_info_rect.x + extra_info_rect.height + 144,
                                extra_info_rect.y + extra_info_rect.height / 2 + 4),
                             0.25f,
                             b_text);
            }
            
            Ts2dPushFilledRect(renderer,
                               v4(1, 1, 1, 1),
                               v4(widget_x + widget->color_picker.hsv.y * sv_selector_rect.width - 2,
                                  widget_y + (1 - widget->color_picker.hsv.z) * sv_selector_rect.height - 2,
                                  4, 4));
            
            Ts2dPushFilledRect(renderer,
                               v4(1, 1, 1, 1),
                               v4(h_selector_rect.x,
                                  h_selector_rect.y + widget->color_picker.hsv.x * h_selector_rect.height,
                                  h_selector_rect.width, 2));
            
            break;
        }
        
        case TSUI_WIDGET_note_picker:
        {
            v4 widget_rect = {widget_x, widget_y, widget_w, widget_h};
            TsUINote white_keys[8];
            TsUINote black_keys[5];
            
            TsUIGetNotePickerKeys(widget_rect,
                                  white_keys,
                                  ArrayCount(white_keys),
                                  black_keys,
                                  ArrayCount(black_keys));
            
            for(u32 j = 0; j < ArrayCount(white_keys); ++j)
            {
                v4 top_note_color;
                v4 bottom_note_color = {0.8f, 0.8f, 0.8f, 1.f};
                
                if(widget->note_picker.selected_note == white_keys[j].note)
                {
                    top_note_color = v4(1, 0, 0, 1);
                }
                else
                {
                    top_note_color = v4(1, 1, 1, 1);
                }
                
                Ts2dPushFilledVertexColoredRect(renderer, top_note_color, bottom_note_color, top_note_color, bottom_note_color, white_keys[j].rect);
            }
            
            for(u32 j = 0; j < ArrayCount(white_keys); ++j)
            {
                Ts2dPushRect(renderer,
                             v4(0, 0, 0, 1),
                             white_keys[j].rect);
            }
            
            for(u32 j = 0; j < ArrayCount(black_keys); ++j)
            {
                v4 top_note_color;
                v4 bottom_note_color = {0.f, 0.f, 0.f, 1.f};
                
                if(widget->note_picker.selected_note == black_keys[j].note)
                {
                    top_note_color = v4(1, 0.3f, 0.3f, 1);
                }
                else
                {
                    top_note_color = v4(0.3f, 0.3f, 0.3f, 1);
                }
                
                Ts2dPushFilledVertexColoredRect(renderer, top_note_color, bottom_note_color, top_note_color, bottom_note_color,
                                                black_keys[j].rect);
            }
            
            break;
        }
        
        case TSUI_WIDGET_tile_select:
        {
            v4 widget_rect = widget->rect;
            v4 selection_rect =
            {
                widget_rect.x + widget->tile_select.selection_x0 * 16.f,
                widget_rect.y + widget->tile_select.selection_y0 * 16.f,
                (widget->tile_select.selection_x1 - widget->tile_select.selection_x0) * 16.f,
                (widget->tile_select.selection_y1 - widget->tile_select.selection_y0) * 16.f
            };
            
            Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.8f), widget_rect);
            Ts2dPushTexture(renderer, widget->tile_select.tilemap_texture,
                            widget->tile_select.tilemap_source, widget_rect);
            Ts2dPushFilledRect(renderer, v4(1, 1, 1, 0.4f), selection_rect);
            
            break;
        }
        
        case TSUI_WIDGET_label:
        {
            Ts2dPushText(renderer,
                         font,
                         TS2D_TEXT_ALIGN_CENTER_Y,
                         v4(0.6f, 0.6f, 0.6f, 0.6f),
                         v2(widget_x + 4,
                            widget_y + widget_h / 2 + 4),
                         0.26f,
                         widget_text);
            break;
        }
        
        case TSUI_WIDGET_title:
        {
            Ts2dPushTextWithBoldnessAndSoftness(renderer,
                                                font,
                                                TS2D_TEXT_ALIGN_CENTER_Y,
                                                v4(1, 1, 1, 1),
                                                v2(widget_x,
                                                   widget_y + widget_h / 2 + 4),
                                                0.30f,
                                                0.7f, 0.2f,
                                                widget_text);
            break;
        }
        
        case TSUI_WIDGET_collapsable:
        {
            Ts2dPushText(renderer, font,
                         TS2D_TEXT_ALIGN_CENTER_Y,
                         v4u(0.6f + 0.3f * !!widget_is_hot - 0.3f * !!widget_is_active),
                         v2(widget_x + 4,
                            widget_y + widget_h / 2 + 4),
                         0.26f,
                         widget_text);
            
            f32 check_size = widget->rect.height - 2 * widget->rect.height / 4;
            
            v4 check_rect =
            {
                widget->rect.x + widget->rect.width - check_size - check_size/2,
                widget->rect.y + widget->rect.height/2 - check_size/2,
                check_size,
                check_size,
            };
            
            Ts2dPushText(renderer, font, TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
                         v4u(1), V2AddV2(V4Center(check_rect), v2(1, 2)), 0.26f, widget->collapsable.open ? "-" : "+");
            
            Ts2dPushRect(renderer, v4(0.7f, 0.7f, 0.7f, 0.7f), check_rect);
            
            break;
        }
        
        case TSUI_WIDGET_canvas:
        {
            widget->canvas.Render(widget->text.data, widget->rect, v2(ui->cursor_x - widget->rect.x, ui->cursor_y - widget->rect.y),
                                  widget->canvas.render_user_data);
            break;
        }
        
        default: break;
    }
    
    Ts2dPopClip(renderer);
    Ts2dPopClip(renderer);
}

void
TsUIRenderWindowDefaultTs2d(TsUI *ui, i32 style_flags, TsUIWindow *window, void *user_data)
{
    Ts2d *renderer = user_data;
    
    Ts2dFont *font = Ts2dGetDefaultFont(renderer);
    
    Ts2dPushRectangleBlur(renderer, window->rect, 0.5f);
    Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.8f), window->rect);
    
    if(!(window->flags & TSUI_WINDOW_FLAG_NO_TITLE_BAR))
    {
        Ts2dPushFilledRect(renderer, v4(0.2f, 0.2f, 0.2f, 0.4f),
                           v4(window->rect.x, window->rect.y, window->rect.width, 30));
        Ts2dPushText(renderer, font, TS2D_TEXT_ALIGN_CENTER_Y, v4(1, 1, 1, 1),
                     v2(window->rect.x + 10, window->rect.y + 20), 0.30f, window->title);
    }
    
    if(window->target_view_max.y > 0 && !(window->flags & TSUI_WINDOW_FLAG_NO_SCROLL))
    {
        v4 scrollbar_rect = TsUIGetWindowScrollbarRectangle(window);
        v4 scrollbar_handle_rect = TsUIGetWindowScrollbarHandleRectangle(window);
        Ts2dPushFilledRect(renderer, v4(0, 0, 0, 0.6f), scrollbar_rect);
        Ts2dPushFilledRect(renderer, v4(0.4f, 0.4f, 0.4f, 0.6f), scrollbar_handle_rect);
    }
    
    Ts2dPushRect(renderer, v4(0.6f, 0.6f, 0.6f, 0.8f), window->rect);
}
#endif

void
TsUIUpdateAndRenderWidget(TsUI *ui, i32 style_flags, TsUIWidget *widget, v4 clip)
{
    if(widget->id.primary >= 0 && widget->id.primary < TSUI_WIDGET_MAX)
    {
        ui->widget_id_counters[widget->id.primary] = 0;
    }
    
    b32 widget_is_hot = TsUIIDEqual(widget->id, ui->hot);
    b32 widget_is_active = TsUIIDEqual(widget->id, ui->active);
    
    if(widget_is_hot)
    {
        widget->hot_transition += (1.f - widget->hot_transition) * ui->delta_t * 16.f;
        if(AbsoluteValue(1.f - widget->hot_transition) >= 0.01f)
        {
            platform->pump_events = 1;
        }
    }
    else
    {
        widget->hot_transition += (0.f - widget->hot_transition) * ui->delta_t * 16.f;
        if(AbsoluteValue(0.f - widget->hot_transition) >= 0.01f)
        {
            platform->pump_events = 1;
        }
    }
    
    if(widget_is_active)
    {
        widget->active_transition += (1.f - widget->active_transition) * ui->delta_t * 16.f;
        if(AbsoluteValue(1.f - widget->active_transition) >= 0.01f)
        {
            platform->pump_events = 1;
        }
    }
    else
    {
        widget->active_transition += (0.f - widget->active_transition) * ui->delta_t * 16.f;
        if(AbsoluteValue(0.f - widget->active_transition) >= 0.01f)
        {
            platform->pump_events = 1;
        }
    }
    
    if(ui->RenderWidget)
    {
        ui->RenderWidget(ui, style_flags, widget, clip, ui->render_widget_user_data, widget_is_hot, widget_is_active);
    }
}

void
TsUIUpdateAndRenderWindow(TsUI *ui, TsUIWindow *window)
{
    if(ui->RenderWindow)
    {
        ui->RenderWindow(ui, window->style_flags, window, ui->render_window_user_data);
    }
}

void
TsUIWidgetInit(TsUI *ui, TsUIWidget *widget, TsUIWidgetType type, TsUIID id, i32 style_flags,
               v4 rect, char *text, TsUIWindow *parent_window)
{
    widget->type = type;
    if(!TsUIIDEqual(widget->id, id))
    {
        widget->hot_transition = 0.f;
        widget->active_transition = 0.f;
    }
    widget->id = id;
    widget->rect = rect;
    if(text)
    {
        widget->text = MakeStringOnMemoryArena(ui->widget_arena, "%s", text);
    }
    widget->parent_window = parent_window;
    widget->clip = ui->current_state.clip.value;
    widget->text_color = ui->current_state.text_color.value;
    widget->text_scale = ui->current_state.text_scale.value;
    widget->style_flags = style_flags | ui->current_state.style_flags.flags;
    
    if(ui->input_grid_group_active && !TsUIIDEqual(id, TsUIIDNonInteractable()))
    {
        b32 widget_is_in_active_input_group = ui->input_grid_selected_group_active;
        
        if(!widget_is_in_active_input_group)
        {
            if(TsUIIDIsNull(ui->input_grid_active_group_first_widget_id))
            {
                ui->input_grid_active_group_first_widget_id = id;
            }
            widget_is_in_active_input_group |= !TsUIIDIsNull(ui->input_grid_active_group_first_widget_id) && TsUIIDEqual(id, ui->input_grid_active_group_first_widget_id);
            ui->input_grid_selected_group_active = widget_is_in_active_input_group;
        }
        
        if(widget_is_in_active_input_group)
        {
            iv2 input_grid_position = ui->current_state.input_grid_state.position;
            
            if(!ui->input_grid_selected_cell_valid)
            {
                ui->input_grid_selected_cell_valid = 1;
                ui->input_grid_selected_cell = ui->current_state.input_grid_state.position;
            }
            
            switch(ui->current_state.input_grid_state.direction)
            {
                case TSUI_DIRECTION_LEFT:  { ui->current_state.input_grid_state.position.x -= 1; break; }
                case TSUI_DIRECTION_RIGHT: { ui->current_state.input_grid_state.position.x += 1; break; }
                case TSUI_DIRECTION_UP:    { ui->current_state.input_grid_state.position.y -= 1; break; }
                case TSUI_DIRECTION_DOWN:  { ui->current_state.input_grid_state.position.y += 1; break; }
                default: break;
            }
            ui->input_grid[input_grid_position.x][input_grid_position.y] = id;
        }
    }
}

void
TsUIInit(TsUI *ui)
{
    ui->hot = ui->active = TsUIIDNull();
    MemorySet(ui->windows, 0, sizeof(ui->windows));
    ui->input_grid_selected_cell_valid = 0;
    ui->input_grid_active_group_first_widget_id = TsUIIDNull();
    ui->input_grid_group_active = 0;
    ui->input_grid_selected_group_active = 0;
}

void
TsUIBeginFrame(TsUI *ui, TsUIFrameData *frame)
{
    // NOTE(rjf): Set last cursor position.
    {
        ui->last_cursor_x = ui->cursor_x;
        ui->last_cursor_y = ui->cursor_y;
    }
    
    // NOTE(rjf): Load frame data.
    {
        ui->render_width              = frame->render_width;
        ui->render_height             = frame->render_height;
        ui->cursor_x                  = frame->cursor_x;
        ui->cursor_y                  = frame->cursor_y;
        ui->cursor_scroll_x           = frame->cursor_scroll_x;
        ui->cursor_scroll_y           = frame->cursor_scroll_y;
        ui->cursor_left_down          = frame->cursor_left_down;
        ui->cursor_right_down         = frame->cursor_right_down;
        ui->cursor_left_pressed       = frame->cursor_left_pressed;
        ui->cursor_right_pressed      = frame->cursor_right_pressed;
        ui->up_pressed                = frame->up_pressed;
        ui->left_pressed              = frame->left_pressed;
        ui->down_pressed              = frame->down_pressed;
        ui->right_pressed             = frame->right_pressed;
        ui->up_hold                   = frame->up_hold;
        ui->left_hold                 = frame->left_hold;
        ui->down_hold                 = frame->down_hold;
        ui->right_hold                = frame->right_hold;
        ui->enter_pressed             = frame->enter_pressed;
        ui->tab_pressed               = frame->tab_pressed;
        ui->escape_pressed            = frame->escape_pressed;
        ui->delta_t                   = frame->delta_t;
        ui->widget_arena              = frame->widget_arena;
        ui->GetTextWidth              = frame->GetTextWidth;
        ui->get_text_width_user_data  = frame->get_text_width_user_data;
        ui->GetTextHeight             = frame->GetTextHeight;
        ui->get_text_height_user_data = frame->get_text_height_user_data;
        ui->RenderWidget              = frame->RenderWidget;
        ui->render_widget_user_data   = frame->render_widget_user_data;
        ui->RenderWindow              = frame->RenderWindow;
        ui->render_window_user_data   = frame->render_window_user_data;
        ui->ClickSound                = frame->ClickSound;
        ui->click_sound_user_data     = frame->click_sound_user_data;
        ui->HoverSound                = frame->HoverSound;
        ui->hover_sound_user_data     = frame->hover_sound_user_data;
    }
    
    // NOTE(rjf): Default to default rendering callbacks if none are provided.
    {
        if(!ui->GetTextWidth)
        {
#ifdef TS2D_H_INCLUDED
            ui->GetTextWidth = TsUIGetTextWidthDefaultTs2d;
            ui->get_text_width_user_data = platform->renderer;
#endif
        }
        
        if(!ui->GetTextHeight)
        {
#ifdef TS2D_H_INCLUDED
            ui->GetTextHeight = TsUIGetTextHeightDefaultTs2d;
            ui->get_text_height_user_data = platform->renderer;
#endif
        }
        
        if(!ui->RenderWidget)
        {
#ifdef TS2D_H_INCLUDED
            ui->RenderWidget = TsUIRenderWidgetDefaultTs2d;
            ui->render_widget_user_data = platform->renderer;
#endif
        }
        
        if(!ui->RenderWindow)
        {
#ifdef TS2D_H_INCLUDED
            ui->RenderWindow = TsUIRenderWindowDefaultTs2d;
            ui->render_window_user_data = platform->renderer;
#endif
        }
    }
    
    // NOTE(rjf): Reset widget state.
    {
        ui->last_frame_widget_count = ui->widget_count;
        ui->widget_count = 0;
    }
    
    // NOTE(rjf): Reset auto-layout state.
    {
        ui->current_state.x.value = 0;
        ui->current_state.y.value = 0;
        ui->current_state.width.value = 256;
        ui->current_state.height.value = 48;
        ui->current_state.text_color.value = v4(1, 1, 1, 1);
        ui->current_state.text_scale.value = 0.26f;
        ui->current_state.group_mode.is_column = 0;
        ui->current_state.clip.value = v4(0, 0, ui->render_width, ui->render_height);
        ui->current_state.input_grid_state.position = iv2(TSUI_INPUT_GRID_MAX/3, TSUI_INPUT_GRID_MAX/3);
        ui->current_state.input_grid_state.direction = TSUI_DIRECTION_DOWN;
    }
    
    // NOTE(rjf): Reset stack sizes.
    {
        TsUIResetStacks(ui);
    }
    
    // NOTE(rjf): Reset window data.
    {
        ui->allowed_window = 0;
        ui->active_window = 0;
    }
    
    // NOTE(rjf): Update windows (if not stale, make stale. If stale, delete).
    {
        if(ui->scrolling_window)
        {
            ui->hot = TsUIIDNonInteractable();
            TsPlatformCaptureMousePosition();
            TsPlatformCaptureMouseButtons();
        }
        
        for(i32 i = 0; i < (i32)ui->window_count; ++i)
        {
            TsUIWindow *window = ui->windows + ui->window_order[i];
            
            if(window->stale)
            {
                window->active = 0;
                window->deleted = 1;
                MemoryMove(ui->window_order+i, ui->window_order+i+1, sizeof(ui->window_order[0])*(ui->window_count - i - 1));
                --ui->window_count;
                --i;
            }
            else
            {
                window->stale = 1;
                
                if(window->active && (V4HasPoint(window->rect, v2(ui->cursor_x, ui->cursor_y)) ||
                                      ui->dragging_window == window) && ui->allowed_window == 0)
                {
                    ui->allowed_window = window;
                    
                    if(ui->cursor_left_pressed && i != 0 && !(window->flags & TSUI_WINDOW_FLAG_NO_BRING_TO_TOP))
                    {
                        u32 window_index = ui->window_order[i];
                        MemoryMove(ui->window_order+1, ui->window_order, sizeof(ui->window_order[0])*i);
                        ui->window_order[0] = window_index;
                    }
                }
            }
        }
    }
    
    // NOTE(rjf): Check if we should be in mouse-mode or discrete mode.
    {
        if(ui->mouse_mode)
        {
            if(ui->left_pressed || ui->right_pressed || ui->up_pressed || ui->down_pressed ||
               ui->tab_pressed)
            {
                ui->mouse_mode = 0;
                ui->input_grid_selected_cell_valid = 0;
            }
            else
            {
                
                if(platform->mouse_position_captured)
                {
                    ui->hot = TsUIIDNonInteractable();
                }
                else
                {
                    // NOTE(rjf): Update the hot ID. If the cursor has moved outside of the
                    // hot rectangle, then we need to reset the hot ID to null.
                    if(!V4HasPoint(ui->hot_rect, v2(ui->cursor_x, ui->cursor_y)))
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                
            }
        }
        else
        {
            if(AbsoluteValue(ui->cursor_x - ui->last_cursor_x) > 0.01f ||
               AbsoluteValue(ui->cursor_y - ui->last_cursor_y) > 0.01f)
            {
                ui->mouse_mode = 1;
            }
        }
    }
    
    // NOTE(rjf): Reset input grid.
    {
        for(int i = 0; i < TSUI_INPUT_GRID_MAX; ++i)
        {
            for(int j = 0; j < TSUI_INPUT_GRID_MAX; ++j)
            {
                ui->input_grid[i][j] = TsUIIDNull();
            }
        }
    }
}

void
TsUIEndFrame(TsUI *ui)
{
    // NOTE(rjf): Update non-mouse navigation input.
    if(!ui->mouse_mode)
    {
        b32 selected_has_changed = 0;
        
        if(ui->left_pressed)
        {
            if(ui->input_grid_selected_cell.x > 0 &&
               !TsUIIDIsNull(ui->input_grid[ui->input_grid_selected_cell.x-1][ui->input_grid_selected_cell.y]))
            {
                ui->input_grid_selected_cell.x -= 1;
                selected_has_changed = 1;
            }
        }
        else if(ui->right_pressed)
        {
            if(ui->input_grid_selected_cell.x < TSUI_INPUT_GRID_MAX-1 &&
               !TsUIIDIsNull(ui->input_grid[ui->input_grid_selected_cell.x+1][ui->input_grid_selected_cell.y]))
            {
                ui->input_grid_selected_cell.x += 1;
                selected_has_changed = 1;
            }
        }
        else if(ui->up_pressed)
        {
            if(ui->input_grid_selected_cell.y > 0 &&
               !TsUIIDIsNull(ui->input_grid[ui->input_grid_selected_cell.x][ui->input_grid_selected_cell.y-1]))
            {
                ui->input_grid_selected_cell.y -= 1;
                selected_has_changed = 1;
            }
        }
        else if(ui->down_pressed)
        {
            if(ui->input_grid_selected_cell.y < TSUI_INPUT_GRID_MAX-1 &&
               !TsUIIDIsNull(ui->input_grid[ui->input_grid_selected_cell.x][ui->input_grid_selected_cell.y+1]))
            {
                ui->input_grid_selected_cell.y += 1;
                selected_has_changed = 1;
            }
        }
        
        if(selected_has_changed)
        {
            if(ui->HoverSound)
            {
                ui->HoverSound(ui, 0, ui->hover_sound_user_data);
            }
        }
        
        ui->hot = ui->input_grid[ui->input_grid_selected_cell.x][ui->input_grid_selected_cell.y];
        
        if(TsUIIDEqual(ui->hot, TsUIIDNull()))
        {
            ui->input_grid_selected_cell_valid = 0;
            ui->input_grid_active_group_first_widget_id = TsUIIDNull();
        }
    }
    
    b32 found_hot = 0;
    b32 found_active = 0;
    b32 found_active_group_first_widget_id = 0;
    
    v4 default_clip =
    {
        0, 0, ui->render_width, ui->render_height,
    };
    
    for(i32 i = ui->widget_count - 1; i >= 0; --i)
    {
        if(TsUIIDEqual(ui->widgets[i].id, ui->hot) &&
           (ui->allowed_window == ui->widgets[i].parent_window ||
            ui->widgets[i].parent_window == TSUI_WINDOW_TOP))
        {
            found_hot = 1;
        }
        
        if(TsUIIDEqual(ui->widgets[i].id, ui->active))
        {
            found_active = 1;
        }
        
        if(TsUIIDEqual(ui->widgets[i].id, ui->input_grid_active_group_first_widget_id))
        {
            found_active_group_first_widget_id = 1;
        }
        
        if(ui->widgets[i].parent_window == 0)
        {
            TsUIUpdateAndRenderWidget(ui, ui->widgets[i].style_flags, ui->widgets+i, default_clip);
        }
    }
    
    for(i32 j = ui->window_count-1; j >= 0; --j)
    {
        TsUIWindow *window = ui->windows + ui->window_order[j];
        
        if(window->active)
        {
            TsUIUpdateAndRenderWindow(ui, window);
            
            v4 clip = {0};
            
            if(!(window->flags & TSUI_WINDOW_FLAG_NO_TITLE_BAR))
            {
                clip = v4(window->rect.x,
                          window->rect.y + 31,
                          window->rect.width,
                          window->rect.height - 31);
            }
            else
            {
                clip = window->rect;
            }
            
            for(i32 i = (i32)window->widget_index_end - 1; i >= (i32)window->widget_index_start; --i)
            {
                TsUIUpdateAndRenderWidget(ui, ui->widgets[i].style_flags, ui->widgets+i, clip);
            }
        }
    }
    
    for(i32 i = ui->widget_count - 1; i >= 0; --i)
    {
        if(ui->widgets[i].parent_window == TSUI_WINDOW_TOP)
        {
            TsUIUpdateAndRenderWidget(ui, ui->widgets[i].style_flags, ui->widgets+i, default_clip);
        }
    }
    
    if(!found_hot)
    {
        ui->hot = TsUIIDNull();
    }
    if(!found_active)
    {
        ui->active = TsUIIDNull();
    }
    if(!found_active_group_first_widget_id)
    {
        ui->input_grid_selected_cell_valid = 0;
        ui->input_grid_active_group_first_widget_id = TsUIIDNull();
    }
}

void
TsUIBeginInputGroup(TsUI *ui)
{
    ui->input_grid_group_active = 1;
}

void
TsUIEndInputGroup(TsUI *ui)
{
    ui->input_grid_group_active = 0;
    ui->input_grid_selected_group_active = 0;
}

#include "generated/generated_stack_procedures.c"

void
_TsUIPushWidth(TsUI *ui, f32 width, b32 calculate_with_text)
{
    HardAssert(ui->width_stack_size < TSUI_STACK_MAX);
    ui->width_stack[ui->width_stack_size].calculate_with_text =
        ui->current_state.width.calculate_with_text;
    ui->width_stack[ui->width_stack_size++].value = ui->current_state.width.value;
    ui->current_state.width.value = width;
    ui->current_state.width.calculate_with_text = calculate_with_text;
}

void
_TsUIPushHeight(TsUI *ui, f32 height, b32 calculate_with_text)
{
    HardAssert(ui->height_stack_size < TSUI_STACK_MAX);
    ui->height_stack[ui->height_stack_size].calculate_with_text =
        ui->current_state.height.calculate_with_text;
    ui->height_stack[ui->height_stack_size++].value = ui->current_state.height.value;
    ui->current_state.height.value = height;
    ui->current_state.height.calculate_with_text = calculate_with_text;
}

void
TsUIPopWidth(TsUI *ui)
{
    HardAssert(ui->width_stack_size > 0);
    --ui->width_stack_size;
    ui->current_state.width = ui->width_stack[ui->width_stack_size];
}

void
TsUIPopHeight(TsUI *ui)
{
    HardAssert(ui->height_stack_size > 0);
    --ui->height_stack_size;
    ui->current_state.height = ui->height_stack[ui->height_stack_size];
}

void
TsUIPushWidth(TsUI *ui, f32 width)
{
    _TsUIPushWidth(ui, width, 0);
}

void
TsUIPushHeight(TsUI *ui, f32 height)
{
    _TsUIPushHeight(ui, height, 0);
}

void
TsUIPushAutoWidth(TsUI *ui)
{
    _TsUIPushWidth(ui, 0, 1);
}

void
TsUIPushAutoHeight(TsUI *ui)
{
    _TsUIPushHeight(ui, 0, 1);
}

void
TsUIPushPosition(TsUI *ui, v2 pos)
{
    TsUIPushX(ui, pos.x);
    TsUIPushY(ui, pos.y);
}

void
TsUIPopPosition(TsUI *ui)
{
    TsUIPopX(ui);
    TsUIPopY(ui);
}

void
TsUIPushSize(TsUI *ui, v2 size)
{
    TsUIPushWidth(ui, size.x);
    TsUIPushHeight(ui, size.y);
}

void
TsUIPopSize(TsUI *ui)
{
    TsUIPopWidth(ui);
    TsUIPopHeight(ui);
}

void
TsUIPushColumn(TsUI *ui, v2 position, v2 size)
{
    TsUIPushPosition(ui, position);
    TsUIPushSize(ui, size);
    TsUIPushGroupMode(ui, 1, 0, 0, 0);
}

void
TsUIPushCenteredColumn(TsUI *ui, v2 size, u32 number_of_widgets)
{
    v2 position =
    {
        ui->render_width/2 - size.x/2,
        ui->render_height/2 - size.y*number_of_widgets/2,
    };
    
    TsUIPushPosition(ui, position);
    TsUIPushSize(ui, size);
    TsUIPushGroupMode(ui, 1, 0, 0, 0);
}

void
TsUIPopColumn(TsUI *ui)
{
    f32 column_x_upper_bound = ui->current_state.x.value + ui->current_state.width.value;
    f32 column_y_upper_bound = ui->current_state.y.value;
    
    TsUIPopPosition(ui);
    TsUIPopSize(ui);
    TsUIPopGroupMode(ui);
    
    if(ui->group_mode_stack_size > 0)
    {
        v4 column_rect = {0};
        {
            column_rect.x = ui->current_state.x.value;
            column_rect.y = ui->current_state.y.value;
            column_rect.width = column_x_upper_bound - column_rect.x;
            column_rect.height = column_y_upper_bound - column_rect.y;
        }
        
        if(ui->current_state.group_mode.is_column)
        {
            ui->current_state.y.value += column_rect.height;
        }
        else
        {
            ui->current_state.x.value += column_rect.width;
        }
    }
}

void
TsUIPushRow(TsUI *ui, v2 position, v2 size)
{
    TsUIPushPosition(ui, position);
    TsUIPushSize(ui, size);
    TsUIPushGroupMode(ui, 0, 0, 0, 0);
}

void
TsUIPushAutoRow(TsUI *ui, v2 position, f32 height)
{
    TsUIPushPosition(ui, position);
    TsUIPushAutoWidth(ui);
    TsUIPushHeight(ui, height);
    TsUIPushGroupMode(ui, 0, 0, 0, 0);
}

void
TsUIPopRow(TsUI *ui)
{
    f32 row_x_upper_bound = ui->current_state.x.value;
    f32 row_y_upper_bound = ui->current_state.y.value + ui->current_state.height.value;
    
    TsUIPopPosition(ui);
    TsUIPopSize(ui);
    TsUIPopGroupMode(ui);
    
    if(ui->group_mode_stack_size > 0 && ui->current_state.group_mode.is_column)
    {
        v4 row_rect = {0};
        {
            row_rect.x = ui->current_state.x.value;
            row_rect.y = ui->current_state.y.value;
            row_rect.width = row_x_upper_bound - row_rect.x;
            row_rect.height = row_y_upper_bound - row_rect.y;
        }
        
        if(ui->current_state.group_mode.is_column)
        {
            ui->current_state.x.value += row_rect.width;
        }
        else
        {
            ui->current_state.y.value += row_rect.height;
        }
    }
}

void
TsUIPushInputGridDirection(TsUI *ui, i32 direction)
{
    HardAssert(ui->input_grid_state_stack_size < TSUI_STACK_MAX);
    ui->input_grid_state_stack[ui->input_grid_state_stack_size].direction = ui->current_state.input_grid_state.direction;
    ui->input_grid_state_stack[ui->input_grid_state_stack_size++].position = ui->current_state.input_grid_state.position;
    ui->current_state.input_grid_state.direction = direction;
    
    switch(direction)
    {
        case TSUI_DIRECTION_LEFT:
        {
            ui->current_state.input_grid_state.position.x -= 1;
            break;
        }
        case TSUI_DIRECTION_RIGHT:
        {
            ui->current_state.input_grid_state.position.x += 1;
            break;
        }
        case TSUI_DIRECTION_UP:
        {
            ui->current_state.input_grid_state.position.y -= 1;
            break;
        }
        case TSUI_DIRECTION_DOWN:
        {
            ui->current_state.input_grid_state.position.y += 1;
            break;
        }
        default: break;
    }
}

void
TsUIPopInputGridDirection(TsUI *ui)
{
    if(ui->input_grid_state_stack_size)
    {
        u32 i = --ui->input_grid_state_stack_size;
        ui->current_state.input_grid_state.position = ui->input_grid_state_stack[i].position;
        ui->current_state.input_grid_state.direction = ui->input_grid_state_stack[i].direction;
    }
}

void
TsUISameLine(TsUI *ui)
{
    ui->current_state.group_mode.same_line = 1;
}

void
TsUIUpdateAndGetCurrentState(TsUI *ui, i32 style_flags, char *text, v4 *out_rect,
                             v4 *out_text_color, v4 padding)
{
    v2 position_delta = {0};
    
    f32 same_line_addition = 0.f;
    
    if(ui->current_state.group_mode.last_widget)
    {
        TsUIWidget *last_widget = ui->current_state.group_mode.last_widget;
        
        if(ui->current_state.group_mode.is_column)
        {
            if(ui->current_state.group_mode.same_line)
            {
                same_line_addition = last_widget->rect.width;
                ui->current_state.group_mode.same_line_offset += same_line_addition;
                ui->current_state.group_mode.same_line = 0;
                position_delta.x = ui->current_state.group_mode.same_line_offset;
                position_delta.y = 0;
            }
            else
            {
                position_delta.x = 0;
                position_delta.y = last_widget->rect.height;
                ui->current_state.group_mode.same_line_offset = 0;
            }
        }
        else
        {
            position_delta.x = last_widget->rect.width;
            position_delta.y = 0;
        }
    }
    
    ui->current_state.group_mode.last_widget = ui->widgets + ui->widget_count;
    
    ui->current_state.x.value += position_delta.x;
    ui->current_state.y.value += position_delta.y;
    
    out_rect->x = ui->current_state.x.value;
    out_rect->y = ui->current_state.y.value;
    
    ui->current_state.x.value -= ui->current_state.group_mode.same_line_offset;
    
    if(text && ui->current_state.width.calculate_with_text)
    {
        ui->current_state.width.value =
            ui->GetTextWidth(ui, style_flags, ui->current_state.text_scale.value,
                             text, ui->get_text_width_user_data) + 30 + padding.z;
    }
    
    if(text && ui->current_state.height.calculate_with_text)
    {
        ui->current_state.height.value =
            ui->GetTextHeight(ui, style_flags, ui->current_state.text_scale.value, text,
                              ui->get_text_height_user_data) + 30;
    }
    
    out_rect->width = ui->current_state.width.value;
    out_rect->height = ui->current_state.height.value;
    
    TsUIWindow *window = ui->active_window;
    if(window && window != TSUI_WINDOW_TOP)
    {
        f32 window_relative_position = (ui->current_state.y.value - window->rect.height + 64) - (window->rect.y - window->view_offset.y);
        if(window_relative_position > window->target_view_max.y)
        {
            window->target_view_max.y = window_relative_position;
        }
    }
    
    *out_text_color = ui->current_state.text_color.value;
}

#if 0
void
TsUIGetAndUpdateAutoLayoutState(TsUI *ui, i32 style_flags, char *text, v4 *rect, v4 *text_color, v4 padding)
{
    rect->x = ui->current_state.x.value;
    rect->y = ui->current_state.y.value;
    
    if(text && ui->current_state.width.calculate_with_text)
    {
        rect->width = ui->GetTextWidth(ui, style_flags, ui->current_state.text_scale.value,
                                       text, ui->get_text_width_user_data) + 30 + padding.z;
        ui->current_state.width.value = rect->width;
    }
    else
    {
        rect->z = ui->current_state.width.value;
    }
    
    if(text && ui->current_state.height.calculate_with_text)
    {
        rect->height = ui->GetTextHeight(ui, style_flags, ui->current_state.text_scale.value, text,
                                         ui->get_text_height_user_data) + 30;
        ui->current_state.height.value = rect->height;
    }
    else
    {
        rect->w = ui->current_state.height.value;
    }
    
    *text_color = ui->current_state.text_color.value;
    
    if(ui->current_state.group_mode.is_column)
    {
        ui->current_state.y.value += rect->height;
    }
    else
    {
        ui->current_state.x.value += rect->width;
    }
    
    TsUIWindow *window = ui->active_window;
    if(window && window != TSUI_WINDOW_TOP)
    {
        f32 window_relative_position = (rect->y - window->rect.height + 64) - (window->rect.y - window->view_offset.y);
        if(window_relative_position > window->target_view_max.y)
        {
            window->target_view_max.y = window_relative_position;
        }
    }
}
#endif

void
TsUIWidgetUpdateNonInteractable(TsUI *ui, v4 rect)
{
    b32 cursor_over = V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y));
    if(TsUIIDIsNull(ui->hot) && cursor_over)
    {
        ui->hot = TsUIIDNonInteractable();
        TsPlatformCaptureMousePosition();
    }
}

b32
_TsUIUpdateClickableWidget(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text)
{
    b32 result = 0;
    
    if(ui->allowed_window == ui->active_window || ui->active_window == TSUI_WINDOW_TOP)
    {
        b32 keyboard_mode = !ui->mouse_mode;
        
        if(keyboard_mode)
        {
            if(TsUIIDEqual(id, ui->hot))
            {
                if(ui->enter_pressed)
                {
                    result = 1;
                }
            }
        }
        else
        {
            b32 cursor_over = ui->cursor_x >= rect.x && ui->cursor_y >= rect.y && ui->cursor_x < rect.x + rect.width && ui->cursor_y < rect.y + rect.height &&
                V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
            
            if(TsUIIDEqual(ui->active, id))
            {
                if(!ui->cursor_left_down)
                {
                    result = cursor_over;
                    ui->active = TsUIIDNull();
                    if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                TsPlatformCaptureMousePosition();
                TsPlatformCaptureMouseButtons();
            }
            else
            {
                if(TsUIIDEqual(ui->hot, id))
                {
                    if(ui->cursor_left_down)
                    {
                        ui->active = id;
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                    }
                    else if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                    else
                    {
                        TsPlatformCaptureMousePosition();
                    }
                }
                else
                {
                    if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                    {
                        if(ui->HoverSound)
                        {
                            ui->HoverSound(ui, style_flags, ui->hover_sound_user_data);
                        }
                        
                        ui->hot = id;
                        ui->hot_rect = rect;
                        ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                        TsPlatformCaptureMousePosition();
                    }
                }
            }
            
        }
        
        if(result)
        {
            if(ui->ClickSound)
            {
                ui->ClickSound(ui, style_flags, ui->click_sound_user_data);
            }
        }
    }
    
    return result;
}

TsUIWidget *
TsUIGetNextWidget(TsUI *ui, TsUIID expected_id)
{
    TsUIWidget *widget = ui->widgets + ui->widget_count;
    
    if(!TsUIIDEqual(widget->id, expected_id))
    {
        b32 found_in_last_frame = 0;
        
        for(u32 i = ui->widget_count+1; i < ui->last_frame_widget_count; ++i)
        {
            if(TsUIIDEqual(ui->widgets[i].id, expected_id))
            {
                found_in_last_frame = 1;
                TsUIWidget swap = *widget;
                *widget = ui->widgets[i];
                ui->widgets[i] = swap;
                break;
            }
        }
        
        if(found_in_last_frame == 0)
        {
            int widgets_to_move = (int)ui->last_frame_widget_count - (int)ui->widget_count;
            if(widgets_to_move > 0)
            {
                MemoryMove(widget + 1, widget, widgets_to_move * sizeof(TsUIWidget));
            }
            
            ++ui->last_frame_widget_count;
        }
    }
    
    ++ui->widget_count;
    return widget;
}

b32
_TsUITextButton(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text)
{
    b32 result = _TsUIUpdateClickableWidget(ui, id, style_flags, rect, text);
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_button;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    return result;
}

b32
TsUIStyledButton(TsUI *ui, i32 style_flags, char *text)
{
    v4 rect = {0};
    v4 color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUITextButton(ui, id, style_flags, rect, text);
}

#if 0
b32
_TsUIImageButton(TsUI *ui, TsUIID id, v4 rect, char *text, b32 editor, Ts2dTexture *texture, v4 source)
{
    b32 result = _TsUIUpdateClickableWidget(ui, id, rect, text, editor);
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = editor ? TSUI_WIDGET_dev_image_button : TSUI_WIDGET_button;
    TsUIWidgetInit(ui, widget, widget_type, id, rect, text, ui->active_window);
    widget->image_button.texture = texture;
    widget->image_button.source = source;
    return result;
}
#endif

b32
_TsUIToggler(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text, b32 value)
{
    b32 clicked = _TsUIUpdateClickableWidget(ui, id, style_flags, rect, text);
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_toggler;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    if(clicked)
    {
        value = !value;
    }
    widget->toggler.toggled = value;
    return value;
}

b32
TsUIStyledToggler(TsUI *ui, i32 style_flags, char *text, b32 value)
{
    v4 rect = {0};
    v4 color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4(0, 0, 20, 0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUIToggler(ui, id, style_flags, rect, text, value);
}

f32
_TsUISlider(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text, f32 value, f32 low, f32 high)
{
    b32 result = 0;
    b32 keyboard_mode = !ui->mouse_mode;
    b32 cursor_over =
        V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
        V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
    
    if(keyboard_mode)
    {
        if(TsUIIDEqual(id, ui->hot))
        {
            if(ui->right_hold)
            {
                value += (high - low) * ui->delta_t * 0.5f;
            }
            else if(ui->left_hold)
            {
                value -= (high - low) * ui->delta_t * 0.5f;
            }
        }
    }
    else
    {
        if(TsUIIDEqual(ui->active, id))
        {
            if(ui->cursor_left_down)
            {
                value = (high - low) * ((ui->cursor_x - rect.x) / rect.width) + low;
            }
            else
            {
                ui->active = TsUIIDNull();
                if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
        }
        else if(ui->allowed_window == ui->active_window || ui->active_window == TSUI_WINDOW_TOP)
        {
            if(!TsUIIDEqual(ui->active, id))
            {
                if(TsUIIDEqual(ui->hot, id))
                {
                    if(ui->cursor_left_down)
                    {
                        ui->active = id;
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                    }
                    else if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                else
                {
                    if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                    {
                        ui->hot = id;
                        ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                        ui->hot_rect = rect;
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                    }
                }
            }
        }
    }
    
    if(value < low)
    {
        value = low;
    }
    
    if(value > high)
    {
        value = high;
    }
    
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_slider;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    widget->slider.value = value;
    widget->slider.percentage = (value - low) / (high - low);
    widget->slider.low = low;
    widget->slider.high = high;
    
    return value;
}

f32
TsUIStyledSlider(TsUI *ui, i32 style_flags, char *text, f32 value, f32 low, f32 high)
{
    v4 rect;
    v4 text_color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &text_color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUISlider(ui, id, style_flags, rect, text, value, low, high);
}

i32
_TsUIIntSlider(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text, i32 value, i32 low, i32 high)
{
    b32 result = 0;
    b32 keyboard_mode = !ui->mouse_mode;
    b32 cursor_over =
        V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
        V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
    
    if(keyboard_mode)
    {
        if(TsUIIDEqual(id, ui->hot))
        {
            if(ui->right_hold)
            {
                value += 1;
            }
            else if(ui->left_hold)
            {
                value -= 1;
            }
        }
    }
    else
    {
        if(TsUIIDEqual(ui->active, id))
        {
            
            if(ui->cursor_left_down)
            {
                value = (i32)((high - low) * ((ui->cursor_x - rect.x) / rect.width) + low);
            }
            else
            {
                ui->active = TsUIIDNull();
                if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
        }
        else if(ui->allowed_window == ui->active_window || ui->active_window == TSUI_WINDOW_TOP)
        {
            if(!TsUIIDEqual(ui->active, id))
            {
                if(TsUIIDEqual(ui->hot, id))
                {
                    if(ui->cursor_left_down)
                    {
                        ui->active = id;
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                    }
                    else if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                else
                {
                    if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                    {
                        ui->hot = id;
                        ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                        ui->hot_rect = rect;
                        TsPlatformCaptureMousePosition();
                    }
                }
            }
        }
    }
    
    if(value < low)
    {
        value = low;
    }
    
    if(value > high)
    {
        value = high;
    }
    
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_int_slider;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    widget->int_slider.value = value;
    widget->int_slider.low = low;
    widget->int_slider.high = high;
    
    return value;
}

i32
TsUIStyledIntSlider(TsUI *ui, i32 style_flags, char *text, i32 value, i32 low, i32 high)
{
    v4 rect;
    v4 text_color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &text_color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUIIntSlider(ui, id, style_flags, rect, text, value, low, high);
}

char *
_TsUILineEdit(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text,
              char *edit_text, u32 edit_text_max)
{
    b32 result = 0;
    
    if(ui->allowed_window == ui->active_window || ui->active_window == TSUI_WINDOW_TOP)
    {
        
        b32 keyboard_mode = !ui->mouse_mode;
        
        if(keyboard_mode)
        {
            if(TsUIIDEqual(id, ui->hot))
            {
                ui->active = id;
            }
        }
        else
        {
            b32 cursor_over =
                V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
                V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
            
            if(TsUIIDEqual(ui->active, id))
            {
                if(ui->cursor_left_down)
                {
                    if(cursor_over)
                    {
                        // TODO(rjf): Text selection?
                    }
                    else
                    {
                        ui->active = TsUIIDNull();
                    }
                }
            }
            else
            {
                if(TsUIIDEqual(ui->hot, id))
                {
                    if(ui->cursor_left_down && cursor_over)
                    {
                        ui->active = id;
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                    }
                    else if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                else
                {
                    if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                    {
                        ui->hot = id;
                        ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                        ui->hot_rect = rect;
                        TsPlatformCaptureMousePosition();
                    }
                }
            }
        }
    }
    
    if(TsUIIDEqual(ui->active, id))
    {
        if(platform->keyboard_captured)
        {
            ui->active = TsUIIDNull();
        }
        else
        {
            platform->target_text = edit_text;
            platform->target_text_max_characters = edit_text_max;
            platform->target_text_edit_pos = CalculateCStringLength(edit_text);
            TsPlatformCaptureKeyboard();
        }
    }
    
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_line_edit;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    widget->line_edit.edit_text = MakeStringOnMemoryArena(ui->widget_arena, "%s", edit_text);
    
    return edit_text;
}

char *
TsUIStyledLineEdit(TsUI *ui, i32 style_flags, char *text, char *edit_text, u32 edit_text_max)
{
    v4 rect;
    v4 text_color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &text_color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUILineEdit(ui, id, style_flags, rect, text, edit_text, edit_text_max);
}

void
TsUIStyledTitle(TsUI *ui, i32 style_flags, char *text)
{
    v4 rect;
    v4 color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIIDNonInteractable();
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_title;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
}

void
TsUIStyledLabel(TsUI *ui, i32 style_flags, char *text)
{
    v4 rect;
    v4 color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIIDNonInteractable();
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_label;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
}

b32
_TsUICollapsable(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text)
{
    b32 clicked = _TsUIUpdateClickableWidget(ui, id, style_flags, rect, text);
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_collapsable;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    
    if(clicked)
    {
        widget->collapsable.open = !widget->collapsable.open;
        widget->collapsable.open_transition = 0;
    }
    
    b32 is_open = widget->collapsable.open;
    
    if(is_open)
    {
        TsUIPushColumn(ui, v2(TSUI_COLLAPSABLE_BUMP_SIZE, rect.height*widget->collapsable.open_transition),
                       v2(ui->current_state.width.value - TSUI_COLLAPSABLE_BUMP_SIZE,
                          ui->current_state.height.value*widget->collapsable.open_transition));
        widget->collapsable.open_transition += ((f32)(!!widget->collapsable.open) - widget->collapsable.open_transition) * ui->delta_t * TSUI_DEFAULT_INTERPOLATION_RATE;
    }
    
    return is_open;
}

b32
TsUIStyledCollapsable(TsUI *ui, i32 style_flags, char *text)
{
    v4 rect = {0};
    v4 color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    b32 result = _TsUICollapsable(ui, id, style_flags, rect, text);
    return result;
}

void
TsUICollapsableEnd(TsUI *ui)
{
    TsUIPopColumn(ui);
}

void
TsUIStyledDivider(TsUI *ui, i32 style_flags)
{
    v4 rect;
    v4 color;
    
    if(ui->current_state.group_mode.is_column)
    {
        TsUIPushHeight(ui, ui->current_state.height.value/2);
    }
    else
    {
        TsUIPushHeight(ui, ui->current_state.width.value/16);
    }
    
    TsUIUpdateAndGetCurrentState(ui, style_flags, "", &rect, &color, v4u(0));
    TsUIID id = TsUIIDNonInteractable();
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_divider;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, 0, ui->active_window);
    
    if(ui->current_state.group_mode.is_column)
    {
        TsUIPopHeight(ui);
    }
    else
    {
        TsUIPopWidth(ui);
    }
    
}

b32
TsUIStyledWindowCloseButton(TsUI *ui, i32 style_flags)
{
    v4 rect = {0};
    v4 color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, "x", &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, "Close");
    b32 clicked = _TsUITextButton(ui, id, style_flags, rect, "x");
    ui->widgets[ui->widget_count-1].type = TSUI_WIDGET_window_close_button;
    return clicked;
}

void
TsUIStyledWindowBegin(TsUI *ui, i32 style_flags, char *title, v4 rect, i32 flags, b32 *open_value_ptr)
{
    HardAssert(ui->window_count < TSUI_WINDOW_MAX);
    TsUIWindow *window = 0;
    b32 first_frame_of_windows_existence = 0;
    
    u32 title_hash = CStringCRC32(title);
    u32 hash_value = title_hash % TSUI_WINDOW_MAX;
    u32 original_hash_value = hash_value;
    
    TsUIWindow *open_slot = 0;
    
    for(;;)
    {
        if(ui->windows[hash_value].active && !ui->windows[hash_value].deleted)
        {
            if(CStringMatchCaseSensitive(ui->windows[hash_value].title, title))
            {
                open_slot = ui->windows + hash_value;
                first_frame_of_windows_existence = 0;
                break;
            }
        }
        else
        {
            open_slot = ui->windows + hash_value;
            first_frame_of_windows_existence = 1;
            if(!ui->windows[hash_value].deleted)
            {
                break;
            }
        }
        
        ++hash_value;
        if(hash_value >= TSUI_WINDOW_MAX)
        {
            hash_value = 0;
        }
        if(hash_value == original_hash_value)
        {
            break;
        }
    }
    
    window = open_slot;
    
    if(window)
    {
        CopyCStringToFixedSizeBuffer(window->title, sizeof(window->title), title);
        window->title_hash = title_hash;
        if(first_frame_of_windows_existence)
        {
            ++ui->window_count;
            window->rect = rect;
            window->view_offset = v2(0, 0);
            window->target_view_offset = v2(0, 0);
            
            MemoryMove(ui->window_order+1, ui->window_order, sizeof(ui->window_order[0])*(ui->window_count-1));
            ui->window_order[0] = (u32)(window - ui->windows);
        }
        else
        {
            if(flags & TSUI_WINDOW_FLAG_FIXED_POSITION)
            {
                window->rect.x = rect.x;
                window->rect.y = rect.y;
            }
            window->rect.width = rect.width;
            window->rect.height = rect.height;
        }
        
        window->flags = flags;
        window->widget_index_start = ui->widget_count;
        window->active = 1;
        window->stale = 0;
        window->deleted = 0;
        window->open_value_ptr = open_value_ptr;
        window->style_flags = style_flags;
        
        if(window->flags & TSUI_WINDOW_FLAG_NO_SCROLL)
        {
            window->view_offset = v2(0, 0);
            window->target_view_offset = v2(0, 0);
        }
        else
        {
            window->view_offset.x += (window->target_view_offset.x - window->view_offset.x) * ui->delta_t * 16.f;
            window->view_offset.y += (window->target_view_offset.y - window->view_offset.y) * ui->delta_t * 16.f;
        }
        
        if(window == ui->allowed_window || ui->scrolling_window == window)
        {
            window->target_view_offset.y -= 1.f * ui->cursor_scroll_y;
            TsPlatformCaptureMousePosition();
            
            if(window->target_view_max.y > 0)
            {
                v2 cursor_position = { ui->cursor_x, ui->cursor_y };
                v4 scrollbar_rect = TsUIGetWindowScrollbarRectangle(window);
                v4 scrollbar_handle_rect = TsUIGetWindowScrollbarHandleRectangle(window);
                
                if((V4HasPoint(scrollbar_handle_rect, cursor_position) && !ui->scrolling_window) || ui->scrolling_window == window)
                {
                    if(ui->scrolling_window == window)
                    {
                        if(ui->cursor_left_down)
                        {
                            window->target_view_offset.y += ui->cursor_y - (scrollbar_handle_rect.y + ui->scroll_bar_grab_offset);
                        }
                        else
                        {
                            ui->scrolling_window = 0;
                        }
                    }
                    else
                    {
                        if(ui->cursor_left_pressed)
                        {
                            ui->scrolling_window = window;
                            ui->scroll_bar_grab_offset = ui->cursor_y - scrollbar_handle_rect.y;
                        }
                    }
                }
                else
                {
                    if(V4HasPoint(scrollbar_rect, cursor_position))
                    {
                        
                    }
                }
            }
            
            
            if(!(window->flags & TSUI_WINDOW_FLAG_FIXED_POSITION) && TsUIIDIsNull(ui->hot))
            {
                if(ui->dragging_window == window)
                {
                    if(ui->cursor_left_down)
                    {
                        TsPlatformCaptureMousePosition();
                        TsPlatformCaptureMouseButtons();
                        
                        window->rect.x += ui->cursor_x - ui->last_cursor_x;
                        window->rect.y += ui->cursor_y - ui->last_cursor_y;
                        
                        for(u32 i = 0; i < ui->window_count; ++i)
                        {
                            if(ui->windows + ui->window_order[i] == window)
                            {
                                u32 window_index = ui->window_order[i];
                                MemoryMove(ui->window_order+1, ui->window_order, sizeof(ui->window_order[0])*i);
                                ui->window_order[0] = window_index;
                                break;
                            }
                        }
                    }
                    else
                    {
                        ui->dragging_window = 0;
                    }
                }
                else if(!ui->dragging_window)
                {
                    if(V4HasPoint(v4(window->rect.x, window->rect.y,
                                     window->rect.width, 30), v2(ui->cursor_x, ui->cursor_y)))
                    {
                        if(TsUIIDEqual(ui->hot, TsUIIDNull()) && ui->cursor_left_pressed)
                        {
                            ui->dragging_window = window;
                            TsPlatformCaptureMousePosition();
                            TsPlatformCaptureMouseButtons();
                        }
                    }
                }
            }
        }
    }
    else
    {
        HardAssert("No window slots available" == 0);
    }
    
    window->target_view_offset.x = ClampF32(window->target_view_offset.x, 0, window->target_view_max.x);
    window->target_view_offset.y = ClampF32(window->target_view_offset.y, 0, window->target_view_max.y);
    
    window->target_view_max.x = 0;
    window->target_view_max.y = 0;
    
    
    ui->active_window = window;
    
    if(window->open_value_ptr)
    {
        TsUIPushGroupMode(ui, 0, 0, 0, 0);
        TsUIPushPosition(ui, v2(window->rect.x + window->rect.width - 30,
                                window->rect.y));
        TsUIPushSize(ui, v2(30, 30));
        if(TsUIStyledWindowCloseButton(ui, style_flags))
        {
            *window->open_value_ptr = 0;
        }
        TsUIPopSize(ui);
        TsUIPopPosition(ui);
        TsUIPopGroupMode(ui);
    }
    
    if(!(flags & TSUI_WINDOW_FLAG_NO_TITLE_BAR))
    {
        TsUIPushPosition(ui, v2(window->rect.x - window->view_offset.x, window->rect.y + 30 - window->view_offset.y));
        TsUIPushClip(ui, v4(window->rect.x, window->rect.y + 31, window->rect.width, window->rect.height - 31));
    }
    else
    {
        TsUIPushPosition(ui, v2(window->rect.x - window->view_offset.x, window->rect.y - window->view_offset.y));
        TsUIPushClip(ui, window->rect);
    }
}

void
TsUIWindowEnd(TsUI *ui)
{
    SoftAssert(ui->active_window != 0);
    ui->active_window->widget_index_end = ui->widget_count;
    
    if(TsUIIDEqual(ui->hot, TsUIIDNull()))
    {
        v2 cursor_position = v2(ui->cursor_x, ui->cursor_y);
        TsUIWindow *window = ui->active_window;
        if(V4HasPoint(window->rect, cursor_position))
        {
            TsPlatformCaptureMousePosition();
            TsPlatformCaptureMouseButtons();
        }
    }
    
    ui->active_window = 0;
    
    TsUIPopPosition(ui);
    TsUIPopClip(ui);
}

void
TsUIStyledCanvas(TsUI *ui, i32 style_flags, char *text,
                 TsUICanvasUpdateCallback *Update, void *update_user_data,
                 TsUICanvasRenderCallback *Render, void *render_user_data)
{
    v4 rect;
    v4 color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, 0, &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    TsUIWidgetType widget_type = TSUI_WIDGET_canvas;
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    widget->canvas.Update = Update;
    widget->canvas.update_user_data = update_user_data;
    widget->canvas.Render = Render;
    widget->canvas.render_user_data = render_user_data;
    
    if(TsUIIDEqual(ui->hot, TsUIIDNull()))
    {
        if(V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)))
        {
            ui->hot = id;
        }
    }
    else if(TsUIIDEqual(ui->hot, id))
    {
        if(!V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)))
        {
            ui->hot = ui->active = TsUIIDNull();
        }
    }
    
    widget->canvas.Update(text, rect, v2(ui->cursor_x - rect.x, ui->cursor_y - rect.y), update_user_data);
}

b32
TsUIStyledDropdown(TsUI *ui, i32 style_flags, char *text)
{
    v4 rect = {0};
    v4 color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    b32 clicked = _TsUITextButton(ui, id, style_flags, rect, text);
    b32 dropdown_open = 0;
    i32 open_dropdown_index = -1;
    
    TsUIWidget *widget = ui->widgets + ui->widget_count - 1;
    
    for(u32 i = 0; i < ui->open_dropdown_stack_size; ++i)
    {
        if(TsUIIDEqual(ui->open_dropdown_stack[i].widget_id, id))
        {
            dropdown_open = 1;
            open_dropdown_index = (i32)i;
            break;
        }
    }
    
    if(clicked)
    {
        if(dropdown_open)
        {
            dropdown_open = 0;
            ui->open_dropdown_stack_size = (u32)open_dropdown_index;
        }
        else
        {
            dropdown_open = 1;
            
            if(ui->current_state.active_dropdown.value)
            {
                
                for(u32 i = 0; i < ui->open_dropdown_stack_size; ++i)
                {
                    if(TsUIIDEqual(ui->open_dropdown_stack[i].widget_id, ui->current_state.active_dropdown.value->id))
                    {
                        ui->open_dropdown_stack_size = i+1;
                    }
                }
                
            }
            else
            {
                ui->open_dropdown_stack_size = 0;
            }
            
            open_dropdown_index = (i32)ui->open_dropdown_stack_size;
            ui->open_dropdown_stack[ui->open_dropdown_stack_size].widget_id = id;
            ui->open_dropdown_stack[ui->open_dropdown_stack_size++].open_transition = 0.f;
        }
    }
    
    ui->active_dropdown_stack[ui->active_dropdown_stack_size++].value = ui->current_state.active_dropdown.value;
    ui->current_state.active_dropdown.value = widget;
    
    if(ui->current_state.group_mode.is_column)
    {
        TsUIPushPosition(ui, v2(rect.width, 0));
    }
    else
    {
        TsUIPushPosition(ui, v2(0, rect.height));
    }
    TsUIPushGroupMode(ui, 1, 0, 0, 0);
    
    if(dropdown_open && ui->open_dropdown_stack_size)
    {
        ui->open_dropdown_stack[open_dropdown_index].open_transition +=
            (1 - ui->open_dropdown_stack[open_dropdown_index].open_transition) * ui->delta_t * TSUI_DEFAULT_INTERPOLATION_RATE;
    }
    
    f32 transition = 0.f;
    if(ui->open_dropdown_stack_size)
    {
        transition = ui->open_dropdown_stack[open_dropdown_index].open_transition;
    }
    TsUIPushSize(ui, v2(rect.width, rect.height*transition));
    
    return dropdown_open;
}

void
TsUICloseCurrentDropdownTree(TsUI *ui)
{
    ui->open_dropdown_stack_size = 0;
}

void
TsUIDropdownEnd(TsUI *ui)
{
    TsUIPopGroupMode(ui);
    TsUIPopPosition(ui);
    TsUIPopSize(ui);
    
    if(ui->active_dropdown_stack_size > 0)
    {
        ui->current_state.active_dropdown.value = ui->active_dropdown_stack[--ui->active_dropdown_stack_size].value;
    }
    else
    {
        ui->current_state.active_dropdown.value = 0;
    }
}

v3
_TsUIColorPicker(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text, v3 color)
{
    v3 color_hsv = RGBToHSV(color);
    
    b32 keyboard_mode = !ui->mouse_mode;
    b32 changing = 0;
    
    if(ui->active_window == ui->allowed_window || ui->active_window == TSUI_WINDOW_TOP)
    {
        if(keyboard_mode)
        {
            if(TsUIIDEqual(id, ui->hot))
            {
                changing = 1;
            }
        }
        else
        {
            b32 cursor_over =
                V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
                V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
            
            if(TsUIIDEqual(ui->active, id))
            {
                if(ui->cursor_left_down)
                {
                    changing = 1;
                }
                else
                {
                    ui->active = TsUIIDNull();
                    if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
            }
            else
            {
                if(TsUIIDEqual(ui->hot, id))
                {
                    if(ui->cursor_left_down)
                    {
                        ui->active = id;
                    }
                    else if(!cursor_over)
                    {
                        ui->hot = TsUIIDNull();
                    }
                }
                else
                {
                    if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                    {
                        ui->hot = id;
                        ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                        ui->hot_rect = rect;
                    }
                }
            }
        }
    }
    
    v4 h_selector_rect = TsUIGetColorPickerHueSelectorRect(rect);
    v4 sv_selector_rect = TsUIGetColorPickerSaturationValueSelectorRect(rect);
    
    TsUIWidgetType widget_type = TSUI_WIDGET_color_picker;
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    
    if(TsUIIDEqual(widget->id, id))
    {
        if(changing)
        {
            if(!widget->color_picker.was_selecting)
            {
                if(keyboard_mode)
                {
                    // TODO(rjf): Keyboard controls
                }
                else
                {
                    if(V4HasPoint(h_selector_rect, v2(ui->cursor_x, ui->cursor_y)))
                    {
                        widget->color_picker.selecting_hue = 1;
                    }
                    else if(V4HasPoint(sv_selector_rect, v2(ui->cursor_x, ui->cursor_y)))
                    {
                        widget->color_picker.selecting_hue = 0;
                    }
                }
            }
            
            widget->color_picker.was_selecting = 1;
            
            if(widget->color_picker.was_selecting)
            {
                if(keyboard_mode)
                {
                    // TODO(rjf): Keyboard controls
                }
                else
                {
                    if(widget->color_picker.selecting_hue)
                    {
                        color_hsv.x = (ui->cursor_y - h_selector_rect.y) / h_selector_rect.height;
                    }
                    else
                    {
                        color_hsv.y = (ui->cursor_x - sv_selector_rect.x) / sv_selector_rect.width;
                        color_hsv.z = 1 - (ui->cursor_y - sv_selector_rect.y) / sv_selector_rect.height;
                    }
                }
            }
            
            color_hsv.x = ClampF32(color_hsv.x, 0.001f, 0.999f);
            color_hsv.y = ClampF32(color_hsv.y, 0.001f, 0.999f);
            color_hsv.z = ClampF32(color_hsv.z, 0.001f, 0.999f);
        }
        else
        {
            widget->color_picker.selecting_hue = 0;
            widget->color_picker.was_selecting = 0;
        }
        
        color = HSVToRGB(color_hsv);
        
        widget->color_picker.rgb = color;
        widget->color_picker.hsv = color_hsv;
    }
    else
    {
        widget->color_picker.rgb = color;
        widget->color_picker.hsv = color_hsv;
        widget->color_picker.selecting_hue = 0;
        widget->color_picker.was_selecting = 0;
    }
    
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    
    return color;
}

v3
TsUIStyledColorPicker(TsUI *ui, i32 style_flags, char *text, v3 color)
{
    TsUIDivider(ui);
    TsUIPushSize(ui, v2(ui->current_state.width.value, ui->current_state.width.value));
    v4 rect = {0};
    v4 text_color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &text_color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    TsUIPopSize(ui);
    TsUIDivider(ui);
    return _TsUIColorPicker(ui, id, style_flags, rect, text, color);
}

i32
_TsUINotePicker(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text, i32 note)
{
    b32 keyboard_mode = !ui->mouse_mode;
    
    b32 fired = 0;
    
    if(keyboard_mode)
    {
        // TODO(rjf): Keyboard controls
    }
    else
    {
        b32 cursor_over =
            V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
            V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
        
        if(TsUIIDEqual(ui->active, id))
        {
            if(!ui->cursor_left_down)
            {
                fired = cursor_over;
                ui->active = TsUIIDNull();
                if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
        }
        else
        {
            if(TsUIIDEqual(ui->hot, id))
            {
                if(ui->cursor_left_down)
                {
                    ui->active = id;
                }
                else if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
            else
            {
                if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                {
                    ui->hot = id;
                    ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                    ui->hot_rect = rect;
                }
            }
        }
    }
    
    if(fired)
    {
        TsUINote white_keys[8];
        TsUINote black_keys[5];
        
        TsUIGetNotePickerKeys(rect,
                              white_keys,
                              ArrayCount(white_keys),
                              black_keys,
                              ArrayCount(black_keys));
        
        v2 cursor_position =
        {
            ui->cursor_x,
            ui->cursor_y,
        };
        
        i32 new_note = -1;
        
        for(u32 i = 0; i < ArrayCount(black_keys); ++i)
        {
            if(V4HasPoint(black_keys[i].rect, cursor_position))
            {
                new_note = black_keys[i].note;
            }
        }
        
        if(new_note < 0)
        {
            for(u32 i = 0; i < ArrayCount(white_keys); ++i)
            {
                if(V4HasPoint(white_keys[i].rect, cursor_position))
                {
                    new_note = white_keys[i].note;
                }
            }
        }
        
        if(new_note == note)
        {
            note = -1;
        }
        else
        {
            note = new_note;
        }
    }
    
    TsUIWidgetType widget_type = TSUI_WIDGET_note_picker;
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    
    widget->note_picker.selected_note = note;
    
    if(fired)
    {
        if(ui->ClickSound)
        {
            ui->ClickSound(ui, style_flags, ui->click_sound_user_data);
        }
    }
    
    return note;
}

i32
TsUIStyledNotePicker(TsUI *ui, i32 style_flags, char *text, i32 note)
{
    TsUIDivider(ui);
    TsUIPushSize(ui, v2(ui->current_state.width.value, ui->current_state.width.value*(2.f/3.f)));
    v4 rect = {0};
    v4 text_color = {0};
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &text_color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    TsUIPopSize(ui);
    note = _TsUINotePicker(ui, id, style_flags, rect, text, note);
    TsUIDivider(ui);
    return note;
}

b32
_TsUITileSelect(TsUI *ui, TsUIID id, i32 style_flags, v4 rect, char *text,
                Ts2dTexture *texture, v4 tilemap_source,
                i32 *tile_select_x0, i32 *tile_select_y0,
                i32 *tile_select_x1, i32 *tile_select_y1,
                b32 selection_from_widget)
{
    
    b32 result = 0;
    
    b32 keyboard_mode = !ui->mouse_mode;
    b32 changing = 0;
    
    if(keyboard_mode)
    {
        if(TsUIIDEqual(id, ui->hot))
        {
            changing = 1;
        }
    }
    else
    {
        b32 cursor_over =
            V4HasPoint(rect, v2(ui->cursor_x, ui->cursor_y)) &&
            V4HasPoint(ui->current_state.clip.value, v2(ui->cursor_x, ui->cursor_y));
        
        if(TsUIIDEqual(ui->active, id))
        {
            if(ui->cursor_left_down)
            {
                changing = 1;
            }
            else
            {
                ui->active = TsUIIDNull();
                if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
        }
        else
        {
            if(TsUIIDEqual(ui->hot, id))
            {
                if(ui->cursor_left_down)
                {
                    ui->active = id;
                }
                else if(!cursor_over)
                {
                    ui->hot = TsUIIDNull();
                }
            }
            else
            {
                if((TsUIIDIsNull(ui->hot) || !ui->hot_is_on_top) && cursor_over && !ui->cursor_left_down)
                {
                    ui->hot = id;
                    ui->hot_is_on_top = ui->active_window == TSUI_WINDOW_TOP;
                    ui->hot_rect = rect;
                }
            }
        }
    }
    
    TsUIWidgetType widget_type = TSUI_WIDGET_tile_select;
    HardAssert(ui->widget_count < TSUI_WIDGET_MAX);
    TsUIWidget *widget = TsUIGetNextWidget(ui, id);
    
    if(TsUIIDEqual(widget->id, id))
    {
        if(changing)
        {
            if(!widget->tile_select.was_selecting)
            {
                // NOTE(rjf): This assumes the tile art is 16x16, so maybe
                //            this'll change? I mean, probably not, but maybe?
                //            Anyways, this is just a reminder for the rare case
                //            where it'll need to change.
                *tile_select_x0 = (i32)(ui->cursor_x - rect.x) / 16;
                *tile_select_y0 = (i32)(ui->cursor_y - rect.y) / 16;
                *tile_select_x1 = *tile_select_x0;
                *tile_select_y1 = *tile_select_y0;
            }
            widget->tile_select.was_selecting = 1;
            if(widget->tile_select.was_selecting)
            {
                if(keyboard_mode)
                {
                    // TODO(rjf): Keyboard controls
                }
                else
                {
                    i32 new_tile_select_x1 = (i32)(ui->cursor_x - rect.x) / 16;
                    i32 new_tile_select_y1 = (i32)(ui->cursor_y - rect.y) / 16;
                    *tile_select_x1 = new_tile_select_x1;
                    *tile_select_y1 = new_tile_select_y1;
                }
            }
            
            *tile_select_x0 = ClampI32(*tile_select_x0, 0, (i32)tilemap_source.width / 16);
            *tile_select_x1 = ClampI32(*tile_select_x1, 0, (i32)tilemap_source.width / 16);
            *tile_select_y0 = ClampI32(*tile_select_y0, 0, (i32)tilemap_source.height / 16);
            *tile_select_y1 = ClampI32(*tile_select_y1, 0, (i32)tilemap_source.height / 16);
            
            if(*tile_select_x0 > *tile_select_x1)
            {
                int swap = *tile_select_x1;
                *tile_select_x1 = *tile_select_x0;
                *tile_select_x0 = swap;
            }
            else if(*tile_select_x0 == *tile_select_x1)
            {
                *tile_select_x1 = *tile_select_x0 + 1;
            }
            
            if(*tile_select_y0 > *tile_select_y1)
            {
                int swap = *tile_select_y1;
                *tile_select_y1 = *tile_select_y0;
                *tile_select_y0 = swap;
            }
            else if(*tile_select_y0 == *tile_select_y1)
            {
                *tile_select_y1 = *tile_select_y0 + 1;
            }
            
            widget->tile_select.tilemap_texture = texture;
            widget->tile_select.tilemap_source = tilemap_source;
            
            widget->tile_select.selection_x0 = *tile_select_x0;
            widget->tile_select.selection_x1 = *tile_select_x1;
            widget->tile_select.selection_y0 = *tile_select_y0;
            widget->tile_select.selection_y1 = *tile_select_y1;
        }
        else
        {
            widget->tile_select.was_selecting = 0;
            if(!selection_from_widget)
            {
                widget->tile_select.selection_x0 = 0;
                widget->tile_select.selection_x1 = 0;
                widget->tile_select.selection_y0 = 0;
                widget->tile_select.selection_y1 = 0;
            }
        }
    }
    else
    {
        widget->tile_select.was_selecting = 0;
        widget->tile_select.tilemap_texture = texture;
        widget->tile_select.tilemap_source = tilemap_source;
        
        if(!selection_from_widget)
        {
            widget->tile_select.selection_x0 = 0;
            widget->tile_select.selection_x1 = 0;
            widget->tile_select.selection_y0 = 0;
            widget->tile_select.selection_y1 = 0;
        }
        else
        {
            widget->tile_select.selection_x0 = *tile_select_x0;
            widget->tile_select.selection_x1 = *tile_select_x1;
            widget->tile_select.selection_y0 = *tile_select_y0;
            widget->tile_select.selection_y1 = *tile_select_y1;
        }
    }
    
    TsUIWidgetInit(ui, widget, widget_type, id, style_flags, rect, text, ui->active_window);
    
    result = changing;
    return result;
}

b32
TsUIStyledTileSelect(TsUI *ui, i32 style_flags, char *text,
                     Ts2dTexture *texture,
                     v4 tilemap_source,
                     i32 *tile_select_x0, i32 *tile_select_y0,
                     i32 *tile_select_x1, i32 *tile_select_y1,
                     b32 selection_from_widget)
{
    v4 rect;
    v4 color;
    TsUIUpdateAndGetCurrentState(ui, style_flags, text, &rect, &color, v4u(0));
    TsUIID id = TsUIGenerateID(ui, text);
    return _TsUITileSelect(ui, id, style_flags, rect, text, texture, tilemap_source,
                           tile_select_x0, tile_select_y0,
                           tile_select_x1, tile_select_y1,
                           selection_from_widget);
}

void
TsUIBeginAlwaysOnTop(TsUI *ui)
{
    ui->active_window = TSUI_WINDOW_TOP;
}

void
TsUIEndAlwaysOnTop(TsUI *ui)
{
    ui->active_window = 0;
}