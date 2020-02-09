internal void RenderPixelObjects()
{
	for (int i = 0; i < core->world_data->pixel_cluster_entity_count; i++)
	{
		PixelClusterEntity *pixel_cluster = &core->world_data->pixel_cluster_entities[i];
		if (pixel_cluster->parent_generic_entity)
		{
			v2 render_pos = v2view(pixel_cluster->position_comp->position);
			v2 render_size = v2zoom(v2(MAX_PIXEL_CLUSTER_LENGTH, MAX_PIXEL_CLUSTER_LENGTH));

			Ts2dPushTexture(core->renderer,
							&pixel_cluster->texture,
							v4(0.0f, 0.0f, MAX_PIXEL_CLUSTER_LENGTH, MAX_PIXEL_CLUSTER_LENGTH),
							v4(render_pos.x, render_pos.y, render_size.x, render_size.y));
		}
	}

	for (int i = 0; i < core->world_data->floating_pixel_entity_count; i++)
	{
		FloatingPixelEntity *pixel = &core->world_data->floating_pixel_entities[i];
		if (pixel->parent_generic_entity)
		{
			v2 render_pos = v2view(pixel->position_comp->position);

			Ts2dPushFilledRect(core->renderer,
							   pixel->colour,
							   v4(render_pos.x, render_pos.y, core->camera_zoom, core->camera_zoom));
		}
	}
}

internal void UpdatePixelClusterTexture(PixelClusterEntity *pixel_cluster)
{
	unsigned char pixel_data[MAX_PIXEL_CLUSTER_LENGTH * MAX_PIXEL_CLUSTER_LENGTH * 4];
	unsigned char *pixel_buffer = &pixel_data[0];

	for (int j = 0; j < MAX_PIXEL_CLUSTER_LENGTH * MAX_PIXEL_CLUSTER_LENGTH; j++)
	{
		Pixel *pixel = &pixel_cluster->pixels[j];

		*pixel_buffer = (unsigned char)(ClampF32(pixel->colour.r, 0.0f, 1.0f) * 255.0f);
		pixel_buffer++;
		*pixel_buffer = (unsigned char)(ClampF32(pixel->colour.g, 0.0f, 1.0f) * 255.0f);
		pixel_buffer++;
		*pixel_buffer = (unsigned char)(ClampF32(pixel->colour.b, 0.0f, 1.0f) * 255.0f);
		pixel_buffer++;
		*pixel_buffer = (unsigned char)(ClampF32(pixel->colour.a, 0.0f, 1.0f) * 255.0f);
		pixel_buffer++;
	}

	pixel_cluster->texture = Ts2dTextureInit(core->renderer,
											 TS2D_TEXTURE_FORMAT_R8G8B8A8,
											 MAX_PIXEL_CLUSTER_LENGTH,
											 MAX_PIXEL_CLUSTER_LENGTH,
											 pixel_data);
}

internal void LoadPixelClusterFromPNG(PixelClusterEntity *cluster, char *path)
{
	Ts2dTexture texture = {0};
	void *png_data = 0;
	u32 png_data_len = 0;
	char png_path[256] = {0};
	snprintf(png_path, sizeof(png_path), "%sres/%s.png", platform->executable_folder_absolute_path, path);
	platform->LoadEntireFile(png_path, &png_data, &png_data_len, 0);

	R_DEV_ASSERT(png_data && png_data_len, "Invalid path when loading pixel cluster (%s)", png_path);

	int texture_width = 0;
	int texture_height = 0;
	int channels = 0;
	void *texture_data = stbi_load_from_memory(png_data, png_data_len, &texture_width, &texture_height, &channels, 4);

	R_DEV_ASSERT(texture_width == MAX_PIXEL_CLUSTER_LENGTH && texture_height == MAX_PIXEL_CLUSTER_LENGTH,
				 "Image must be 64x64 for now.");

	unsigned char *texture_buffer = &((unsigned char *)(texture_data))[0];
	for (int i = 0; i < texture_width * texture_height; i++)
	{
		Pixel *pixel = &cluster->pixels[i];

		pixel->colour.r = *texture_buffer / 255.0f;
		texture_buffer++;
		pixel->colour.g = *texture_buffer / 255.0f;
		texture_buffer++;
		pixel->colour.b = *texture_buffer / 255.0f;
		texture_buffer++;
		pixel->colour.a = *texture_buffer / 255.0f;
		texture_buffer++;
	}

	platform->FreeFileMemory(png_data);
	stbi_image_free(texture_data);
}

internal void UpdatePixels()
{
	// Velocity calculation
	for (int i = 0; i < core->world_data->floating_pixel_entity_count; i++)
	{
		FloatingPixelEntity *pixel = &core->world_data->floating_pixel_entities[i];
		if (pixel->parent_generic_entity)
		{
			if (pixel->flags & PIXEL_FLAGS_apply_gravity)
				pixel->velocity.y = 50.0f;
		}
	}

	// Collision calculation
	for (int i = 0; i < core->world_data->floating_pixel_entity_count; i++)
	{
		FloatingPixelEntity *pixel = &core->world_data->floating_pixel_entities[i];
		if (pixel->parent_generic_entity)
		{
			if (pixel->velocity.x != 0.0f || pixel->velocity.y != 0.0f)
			{
				v2 new_pixel_pos = v2(pixel->position_comp->position.x + pixel->velocity.x * core->world_delta_t, pixel->position_comp->position.y + pixel->velocity.y * core->world_delta_t);
				v2 pixel_delta = V2SubtractV2(new_pixel_pos, pixel->position_comp->position);

				b8 ceil_pixel_x = pixel_delta.x >= 0;
				b8 ceil_pixel_y = pixel_delta.y >= 0;

				i32 x_pixel_steps = (i32)fabsf(ceil_pixel_x ? ceilf(pixel_delta.x) : floorf(pixel_delta.x));
				i32 y_pixel_steps = (i32)fabsf(ceil_pixel_y ? ceilf(pixel_delta.y) : floorf(pixel_delta.y));
				b8 x_is_largest = x_pixel_steps >= y_pixel_steps;

				if (x_pixel_steps == 0 && y_pixel_steps == 0)
				{
					pixel->position_comp->position = new_pixel_pos;
				}
				else
				{
					// Test against other pixels
					b32 does_collide = 0;
					for (int k = 0; k < core->world_data->floating_pixel_entity_count; k++)
					{
						FloatingPixelEntity *against_pixel = &core->world_data->floating_pixel_entities[k];
						if (against_pixel->parent_generic_entity && against_pixel != pixel)
						{
							for (int j = 1; j <= (x_is_largest ? x_pixel_steps : y_pixel_steps); j++)
							{
								f32 step_alpha = (x_is_largest ? (f32)j / (f32)x_pixel_steps : (f32)j / (f32)y_pixel_steps);

								v2 pixel_projection = {pixel->position_comp->position.x + step_alpha * pixel_delta.x,
													   pixel->position_comp->position.y + step_alpha * pixel_delta.y};

								// TODO: Need to ensure the against pixel's collision is resolved.
								v2 new_against_pixel_pos = v2(against_pixel->position_comp->position.x + against_pixel->velocity.x * core->world_delta_t, against_pixel->position_comp->position.y + against_pixel->velocity.y * core->world_delta_t);
								v2 against_pixel_delta = V2SubtractV2(new_against_pixel_pos, against_pixel->position_comp->position);

								b8 ceil_against_pixel_x = against_pixel_delta.x >= 0;
								b8 ceil_against_pixel_y = against_pixel_delta.y >= 0;

								v2 against_pixel_projection = {against_pixel->position_comp->position.x + step_alpha * against_pixel_delta.x,
															   against_pixel->position_comp->position.y + step_alpha * against_pixel_delta.y};

								if ((ceil_pixel_x ? ceilf(pixel_projection.x) : floorf(pixel_projection.x)) ==
										(ceil_against_pixel_x ? ceilf(against_pixel_projection.x) : floorf(against_pixel_projection.x)) &&
									(ceil_pixel_y ? ceilf(pixel_projection.y) : floorf(pixel_projection.y)) ==
										(ceil_against_pixel_y ? ceilf(against_pixel_projection.y) : floorf(against_pixel_projection.y)))
								{
									f32 pixel_delta_magnitude = SquareRoot(pixel_delta.x * pixel_delta.x + pixel_delta.y * pixel_delta.y);

									does_collide = 1;
									pixel->position_comp->position = v2(against_pixel_projection.x - GetSign(pixel_delta.x),
																		against_pixel_projection.y - GetSign(pixel_delta.y));
									break;
								}
							}

							if (does_collide)
								break;
						}
					}

					if (does_collide)
					{
						// Temp workaround
						pixel->velocity.y = 0.0f;
						pixel->flags = 0;
					}
					else
					{
						pixel->position_comp->position = new_pixel_pos;
					}
				}
			}
		}
	}
}