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