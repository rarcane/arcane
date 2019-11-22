internal void ArcaneUIRenderWidget(TsUI *ui, i32 style_flags, TsUIWidget *widget, v4 clip, void *user_data,
								   b32 widget_is_hot, b32 widget_is_active)
{
	if (style_flags & ARCANE_UI_STYLE_MENU)
	{
		Ts2dFont *font = Ts2dGetDefaultFont(core->renderer);
		f32 widget_x = widget->rect.x;
		f32 widget_y = widget->rect.y;
		f32 widget_w = widget->rect.width;
		f32 widget_h = widget->rect.height;
		char *widget_text = widget->text.data;

		switch (widget->type)
		{
		case TSUI_WIDGET_button:
		{
			/* f32 widget_text_scale = 0.5f +
										0.05f * widget->hot_transition -
										0.07f * widget->active_transition;
				Ts2dPushText(core->renderer, font, TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
							v4(1, 1, 1, 1),
							v2(widget_x + widget_w / 2, widget_y + widget_h / 2 + 4),
							widget_text_scale, widget_text); */

			//Ts2dPushFilledRect(core->renderer, v4(0, 0, 0, 0.8f), widget->rect);
			// if (widget_is_hot)
			// {
			// 	Ts2dPushFilledRect(core->renderer, v4(0.2f, 0.2f, 0.2f, 0.4f),
			// 					   v4(widget_x, widget_y, widget_w, widget_h));
			// }
			// if (widget_is_active)
			// {
			// 	Ts2dPushFilledRect(core->renderer, v4(0.4f, 0.4f, 0.4f, 0.4f),
			// 					   v4(widget_x, widget_y, widget_w, widget_h));
			// }
			Ts2dPushRect(core->renderer, v4(0.2f, 0.2f, 0.2f, 0.4f), widget->rect);

			f32 widget_text_scale = widget->text_scale +
									0.05f * widget->hot_transition -
									0.07f * widget->active_transition;

			Ts2dPushText(core->renderer,
						 font,
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1, 1, 1, 1),
						 v2(widget_x + widget_w / 2, widget_y + widget_h / 2 + 4),
						 widget_text_scale,
						 widget_text);

			break;
		}

		case TSUI_WIDGET_toggler:
		{
			f32 widget_text_scale = 0.5f +
									0.05f * widget->hot_transition -
									0.07f * widget->active_transition;

			f32 text_width =
				Ts2dPushText(core->renderer,
							 font,
							 TS2D_TEXT_ALIGN_CENTER_X |
								 TS2D_TEXT_ALIGN_CENTER_Y,
							 v4(1, 1, 1, 1),
							 v2(widget_x + widget_w / 2 - 16 * widget->toggler.toggle_transition,
								widget_y + widget_h / 2 + 4),
							 widget_text_scale,
							 widget_text);

			if (widget->toggler.toggled)
			{
				widget->toggler.toggle_transition += (1 - widget->toggler.toggle_transition) * 8.f * ui->delta_t;
			}
			else
			{
				widget->toggler.toggle_transition += (0 - widget->toggler.toggle_transition) * 8.f * ui->delta_t;
			}
			break;
		}

		case TSUI_WIDGET_slider:
		case TSUI_WIDGET_int_slider:
		{
			f32 widget_text_scale = 0.5f;
			f32 border_brightness = 0.6f + 0.4f * widget->hot_transition;
			f32 fill_brightness = 0.2f + 0.22f * widget->hot_transition;
			f32 fill_percentage = 0;
			char *slider_text = "";

			if (widget->type == TSUI_WIDGET_slider)
			{
				fill_percentage = widget->slider.percentage;
				slider_text = MakeCStringOnMemoryArena(ui->widget_arena, "%s (%.2f)", widget_text, widget->slider.value);
			}
			else if (widget->type == TSUI_WIDGET_int_slider)
			{
				fill_percentage = (f32)(widget->int_slider.value - widget->int_slider.low) / (widget->int_slider.high - widget->int_slider.low);
				slider_text = MakeCStringOnMemoryArena(ui->widget_arena, "%s (%i)", widget_text, widget->slider.value);
			}

			Ts2dPushFilledRect(core->renderer, v4(0.2f, 0.2f, 0.2f, 0.2f),
							   v4(widget_x, widget_y + 6, widget_w, widget_h - 13));

			Ts2dPushFilledRect(core->renderer,
							   v4(fill_brightness, fill_brightness, fill_brightness, 0.6f),
							   v4(widget_x, widget_y + 6, widget_w * fill_percentage,
								  widget_h - 13));

			Ts2dPushRect(core->renderer,
						 v4(border_brightness, border_brightness, border_brightness, 1),
						 v4(widget_x, widget_y + 6, widget_w, widget_h - 13));

			Ts2dPushText(core->renderer, font,
						 TS2D_TEXT_ALIGN_CENTER_X |
							 TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1, 1, 1, 1),
						 v2(widget_x + widget_w / 2,
							widget_y + widget_h / 2 + 4),
						 widget_text_scale,
						 slider_text);

			break;
		}

		case TSUI_WIDGET_title:
		{
			Ts2dPushText(core->renderer,
						 font,
						 TS2D_TEXT_ALIGN_CENTER_X |
							 TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1, 1, 1, 1),
						 v2(widget_x + widget_w / 2,
							widget_y + widget_h / 2 + 4),
						 0.8f,
						 widget_text);

			Ts2dPushLine(core->renderer,
						 v4(1, 1, 1, 1),
						 v2(widget_x, widget_y + widget_h),
						 v2(widget_x + widget_w, widget_y + widget_h));
			break;
		}

		default:
			TsUIRenderWidgetDefaultTs2d(ui, style_flags, widget, clip, core->renderer, widget_is_hot, widget_is_active);
			break;
		}
	}
	else if (style_flags & ARCANE_UI_STYLE_GAME)
	{
		Ts2dFont *font = Ts2dGetDefaultFont(core->renderer);
		f32 widget_x = widget->rect.x;
		f32 widget_y = widget->rect.y;
		f32 widget_w = widget->rect.width;
		f32 widget_h = widget->rect.height;
		char *widget_text = widget->text.data;

		switch (widget->type)
		{
		case TSUI_WIDGET_button:
		{
			Ts2dPushRect(core->renderer, v4(0.2f, 0.2f, 0.2f, 0.4f), widget->rect);

			f32 widget_text_scale = widget->text_scale +
									0.05f * widget->hot_transition -
									0.07f * widget->active_transition;

			Ts2dPushText(core->renderer,
						 font,
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1, 1, 1, 1),
						 v2(widget_x + widget_w / 2, widget_y + widget_h / 2 + 4),
						 widget_text_scale,
						 widget_text);
		}
		break;
		case TSUI_WIDGET_label:
		{
			Ts2dPushText(core->renderer,
						 font,
						 TS2D_TEXT_ALIGN_CENTER_X | TS2D_TEXT_ALIGN_CENTER_Y,
						 v4(1.0f, 1.0f, 1.0f, 1.0f),
						 v2(widget_x + widget_w / 2,
							widget_y + widget_h / 2 + 4),
						 widget->text_scale,
						 widget_text);
		}
		break;
		default:
			TsUIRenderWidgetDefaultTs2d(ui, style_flags, widget, clip, core->renderer, widget_is_hot, widget_is_active);
			break;
		}
	}
	else
	{
		TsUIRenderWidgetDefaultTs2d(ui, style_flags, widget, clip, core->renderer, widget_is_hot, widget_is_active);
	}
}

internal void ArcaneUIRenderWindow(TsUI *ui, i32 style_flags, TsUIWindow *window, void *user_data)
{
	TsUIRenderWindowDefaultTs2d(ui, style_flags, window, core->renderer);
}