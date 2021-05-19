internal v2 ClampV2(v2 v, v2 min, v2 max)
{
	return v2(ClampF32(v.x, min.x, max.x), ClampF32(v.y, min.y, max.y));
}

internal f32 sigmoid(f32 x)
{
	f32 exp_value = (f32)exp(-x);
	return  1 / (1 + exp_value);
}

internal f32 EaseOut(f32 x)
{
	return 1 - (1 - x) * (1 - x);
}

// Smooths a linear 0.0 -> 1.0
internal f32 Fade(f32 alpha)
{
	return 6 * powf(alpha, 5) - 15 * powf(alpha, 4) + 10 * powf(alpha, 3);
}

internal b8 IsV2InRect(v2 position, v4 rect)
{
	return position.x > rect.x && position.x < rect.x + rect.z &&
		position.y > rect.y && position.y < rect.y + rect.w;
}

internal f32 LerpF32(f32 alpha, f32 a, f32 b)
{
	return a + (b - a) * alpha;
}

internal void v2Normalise(v2 *v)
{
	f32 magnitude = PythagSolve(v->x, v->y);
    
	if (magnitude > 0)
	{
		v->x = v->x / magnitude;
		v->y = v->y / magnitude;
	}
}

internal void v2Realise(v2 *v)
{
	if (isnan(v->x) || !isfinite(v->x))
		v->x = 0.0f;
	if (isnan(v->y) || !isfinite(v->y))
		v->y = 0.0f;
}

internal f32 PythagSolve(f32 a, f32 b)
{
	f32 x = a * a + b * b;
	//Based on Quake's fast inverse square root
	u32 i = *(u32*)&x;//Making bits an int-type, so that we can bit hack
	i = 0x1fc00000 + (i >> 1);//Little sus, but it works
	f32 y = *(f32*)&i;//Back to float
	
	y = (y * y + x) / (2 * y);//Runs a couple iterations of the Newton-Raphson method
	y = (y * y + x) / (2 * y);//Keep adding more to increase accuracy
	return y;
}

internal f32 Square(f32 a)
{
	return a * a;
}

internal i32 GetSign(f32 x)
{
	return (x > 0.0f) - (x < 0.0f);
}

// Floors to the 1st decimal place.
internal f32 RoundFirst(f32 x)
{
	x *= 10.0f;
	x = roundf(x);
	x /= 10.0f;
	return x;
}

internal b8 EqualF32(f32 a, f32 b, f32 leeway)
{
	return (fabsf(a - b) <= leeway);
}

internal b8 EqualV2(v2 a, v2 b, f32 leeway)
{
	return (EqualF32(a.x, b.x, leeway) && EqualF32(a.y, b.y, leeway));
}

internal void ShufflePerlinNoise()
{
	for (int i = 0; i < PERLIN_NOISE_LENGTH; i++)
	{
		for (int j = 0; j < PERLIN_NOISE_LENGTH; j++)
		{
			core->random_field[j][i].x = (RandomI32(0, 1) ? -1.0f : 1.0f);
			core->random_field[i][j].y = (RandomI32(0, 1) ? -1.0f : 1.0f);
		}
	}
}

internal f32 GetPerlinNoise(f32 x_pos, f32 y_pos)
{
	if (x_pos >= 0)
		x_pos = fmodf(x_pos, PERLIN_NOISE_LENGTH);
	else
	{
		if (fmodf(fabsf(x_pos), PERLIN_NOISE_LENGTH) == 0)
			x_pos = 0;
		else
			x_pos = PERLIN_NOISE_LENGTH - fmodf(fabsf(x_pos), PERLIN_NOISE_LENGTH);
	}
    
	if (y_pos >= 0)
		y_pos = fmodf(y_pos, PERLIN_NOISE_LENGTH);
	else
	{
		if (fmodf(fabsf(y_pos), PERLIN_NOISE_LENGTH) == 0)
			y_pos = 0;
		else
			y_pos = PERLIN_NOISE_LENGTH - fmodf(fabsf(y_pos), PERLIN_NOISE_LENGTH);
	}
    
	i32 x_min = (i32)floorf(x_pos);
	i32 x_max = (i32)floorf(x_pos) + 1;
    
	i32 y_min = (i32)floorf(y_pos);
	i32 y_max = (i32)floorf(y_pos) + 1;
    
	v2 g0 = core->random_field[y_min][x_min];
	v2 g1 = core->random_field[y_min][x_min + 1];
	v2 g2 = core->random_field[y_min + 1][x_min];
	v2 g3 = core->random_field[y_min + 1][x_min + 1];
    
	v2 d0 = {x_pos - x_min, y_pos - y_min};
	v2 d1 = {x_pos - x_max, y_pos - y_min};
	v2 d2 = {x_pos - x_min, y_pos - y_max};
	v2 d3 = {x_pos - x_max, y_pos - y_max};
    
	f32 p0 = g0.x * d0.x + g0.y * d0.y;
	f32 p1 = g1.x * d1.x + g1.y * d1.y;
	f32 p2 = g2.x * d2.x + g2.y * d2.y;
	f32 p3 = g3.x * d3.x + g3.y * d3.y;
    
	f32 x_fade = Fade(x_pos - x_min);
	f32 y_fade = Fade(y_pos - y_min);
    
	f32 lerp1 = p0 + x_fade * (p1 - p0);
	f32 lerp2 = p2 + x_fade * (p3 - p2);
	f32 lerp3 = lerp1 + y_fade * (lerp2 - lerp1);
    
	return lerp3;
}

internal c2AABB v2AddAABB(v2 a, c2AABB aabb)
{
	c2AABB new_aabb = {.min = c2V(a.x + aabb.min.x, a.y + aabb.min.y),
        .max = c2V(a.x + aabb.max.x, a.y + aabb.max.y)};
	return new_aabb;
}

internal void AddPositionOffsetToShape(c2Shape *shape, c2ShapeType shape_type, v2 position)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb :
		{
			shape->aabb.min.x += position.x;
			shape->aabb.min.y += position.y;
			shape->aabb.max.x += position.x;
			shape->aabb.max.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_capsule :
		{
			shape->capsule.a.x += position.x;
			shape->capsule.a.y += position.y;
			shape->capsule.b.x += position.x;
			shape->capsule.b.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_poly :
		{
			// TODO(randy): Test this out to ensure it's working.
			for (int i = 0; i < shape->poly.count; i++)
			{
				shape->poly.verts[i].x += position.x;
				shape->poly.verts[i].y += position.y;
			}
			c2MakePoly(&shape->poly);
		} break;
		
		case C2_SHAPE_TYPE_line :
		{
			shape->line.p1 = V2AddV2(shape->line.p1, position);
			shape->line.p2 = V2AddV2(shape->line.p2, position);
		} break;
		
		case C2_SHAPE_TYPE_circle :
		{
			shape->circle.p.x += position.x;
			shape->circle.p.y += position.y;
		} break;
		
		case C2_SHAPE_TYPE_line_segments :
		{
			for (i32 i = 0; i < shape->line_segments.count; i++)
			{
				shape->line_segments.vertices[i] = V2AddV2(shape->line_segments.vertices[i], position);
			}
		} break;
		
		default :
		Assert(0);
		break;
	}
}

internal b8 IsV2OverlappingShape(v2 pos, c2Shape shape, c2ShapeType shape_type)
{
	switch (shape_type)
	{
		case C2_SHAPE_TYPE_aabb:
		{
			return (pos.x >= shape.aabb.min.x && pos.x < shape.aabb.max.x &&
					pos.y >= shape.aabb.min.y && pos.y < shape.aabb.max.y);
		} break;
		
		case C2_SHAPE_TYPE_line:
		{
			f32 thicc = 1.0f;
			
			return (pos.x >= shape.line.p1.x && pos.x <= shape.line.p2.x &&
					pos.y >= (shape.line.p1.y + shape.line.p2.y) / 2.0f - thicc && pos.y <= (shape.line.p1.y + shape.line.p2.y) / 2.0f + thicc);
		} break;
		
		case C2_SHAPE_TYPE_line_segments :
		{
			return (pos.x >= shape.line_segments.vertices[0].x && pos.x < shape.line_segments.vertices[shape.line_segments.count - 1].x &&
					pos.y >= shape.line_segments.vertices[shape.line_segments.count - 1].y && pos.y < shape.line_segments.vertices[0].y);
		} break;
		
		default:
		return 0;
	}
	
	return 0;
}

internal c2Shape GetEntityShapeInWorldspace(Entity *entity)
{
	c2Shape shape = entity->physics.shape;
	AddPositionOffsetToShape(&shape, entity->physics.shape_type, entity->position);
	return shape;
}

internal void CapsuleToWorldSpace(c2Capsule *capsule, v2 world_space)
{
	capsule->a.x += world_space.x;
	capsule->a.y += world_space.y;
    
	capsule->b.x += world_space.x;
	capsule->b.y += world_space.y;
}

internal void LineToWorldSpace(c2Ray *line, v2 world_space)
{
	line->p.x += world_space.x;
	line->p.y += world_space.y;
}

internal c2Poly v2AddPoly(v2 a, c2Poly poly)
{
	for (int i = 0; i < poly.count; i++)
	{
		poly.verts[i].x += a.x;
		poly.verts[i].y += a.y;
	}
    
	return poly;
}

// DrawShape
internal void PushDebugShape(c2Shape shape, c2ShapeType type, v2 position, v4 colour)
{
	switch (type)
	{
        case C2_SHAPE_TYPE_aabb :
        {
			v2 p0 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.min.y));
			v2 p1 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.min.y));
			v2 p2 = V2AddV2(position, v2(shape.aabb.max.x, shape.aabb.max.y));
			v2 p3 = V2AddV2(position, v2(shape.aabb.min.x, shape.aabb.max.y));
            
			ArcPushLine(colour, v2view(p0), v2view(p1), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p1), v2view(p2), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p2), v2view(p3), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p3), v2view(p0), LAYER_FRONT_UI);
        } break;
        
        case C2_SHAPE_TYPE_poly :
        {
            for (int i = 0; i < shape.poly.count; i++)
            {
                int secondPoint = (i == shape.poly.count - 1 ? 0 : i + 1);
                
				v2 p1 = V2AddV2(position,
								v2(shape.poly.verts[i].x,
								   shape.poly.verts[i].y));
				v2 p2 = V2AddV2(position,
								v2(shape.poly.verts[secondPoint].x,
								   shape.poly.verts[secondPoint].y));
                
				ArcPushLine(colour, v2view(p1), v2view(p2), LAYER_FRONT_UI);
            }
        } break;
        
        case C2_SHAPE_TYPE_line :
        {
			ArcPushLine(colour,
						v2view(V2AddV2(position, shape.line.p1)),
						v2view(V2AddV2(position, shape.line.p2)),
						LAYER_FRONT_UI);
        } break;
		
		case C2_SHAPE_TYPE_circle :
		{
			position = V2AddV2(position, v2(shape.circle.p.x, shape.circle.p.y));
			
			v2 p0 = V2AddV2(position, v2(shape.circle.r, shape.circle.r));
			v2 p1 = V2AddV2(position, v2(shape.circle.r, -shape.circle.r));
			v2 p2 = V2AddV2(position, v2(-shape.circle.r, -shape.circle.r));
			v2 p3 = V2AddV2(position, v2(-shape.circle.r, shape.circle.r));
            
			ArcPushLine(colour, v2view(p0), v2view(p1), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p1), v2view(p2), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p2), v2view(p3), LAYER_FRONT_UI);
			ArcPushLine(colour, v2view(p3), v2view(p0), LAYER_FRONT_UI);
		} break;
		
		case C2_SHAPE_TYPE_line_segments :
        {
			for (i32 i = 0; i < shape.line_segments.count - 1; i++)
			{
				ArcPushLine(colour,
							v2view(V2AddV2(position, shape.line_segments.vertices[i])),
							v2view(V2AddV2(position, shape.line_segments.vertices[i + 1])),
							LAYER_FRONT_UI);
			}
		} break;
	}
}

internal v2 GetMousePositionInWorldSpace()
{
	return v2(platform->mouse_x / core->camera_zoom + core->camera_position.x - GetZeroWorldPosition().x, platform->mouse_y / core->camera_zoom + core->camera_position.y - GetZeroWorldPosition().y);
}

internal v2 GetMousePosition()
{
	return v2(platform->mouse_x, platform->mouse_y);
}

internal v4 GetCameraRegionRect()
{
	v2 top_left = {core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 top_right = {core->camera_position.x + core->render_w / (2.0f * core->camera_zoom), core->camera_position.y - core->render_h / (2.0f * core->camera_zoom)};
	v2 bottom_left = {core->camera_position.x - core->render_w / (2.0f * core->camera_zoom), core->camera_position.y + core->render_h / (2.0f * core->camera_zoom)};
    
	// TODO(randy): simplify
    
	return v4(top_left.x, top_left.y, top_right.x - top_left.x, bottom_left.y - top_left.y);
}

internal c2AABB GetCameraRegionAABB()
{
	v4 rect = GetCameraRegionRect();
	c2AABB aabb = {0};
	aabb.min = c2V(rect.x, rect.y);
	aabb.max = c2V(rect.x + rect.width, rect.y + rect.height);
	return aabb;
}

internal void GetChunkPositionsInRect(iv2 *positions, i32 *chunk_count, v4 rect, i32 buffer)
{
	*chunk_count = 0;
    
	i32 width = WorldSpaceToChunkIndex(rect.x + rect.width) - WorldSpaceToChunkIndex(rect.x);
	i32 height = WorldSpaceToChunkIndex(rect.y + rect.height) - WorldSpaceToChunkIndex(rect.y);
    
	for (int y = -buffer; y <= height + buffer; y++)
	{
		for (int x = -buffer; x <= width + buffer; x++)
		{
			iv2 chunk = {WorldSpaceToChunkIndex(rect.x) + x, WorldSpaceToChunkIndex(rect.y) + y};
			positions[(*chunk_count)++] = chunk;
			
			if (*chunk_count + 1 == MAX_WORLD_CHUNKS)
			{
				return;
			}
		}
	}
}

internal v2 GetEntityParallaxAmount(Entity *entity)
{
	Assert(entity);
	if (entity->sprite_data.render_layer < 28)
	{
		LogWarning("Entity %s at layer %i is not the the parallax range", entity->debug_name, entity->sprite_data.render_layer);
		return v2(0.0f, 0.0f);
	}
	
	f32 parallax = (f32)(entity->sprite_data.render_layer - 28) / 100.0f;
	return PARALLAX_FORMULA(parallax);
}

internal v2 ParallaxPosition(v2 position, v2 parallax)
{
	return v2(position.x + core->camera_position.x * parallax.x,
			  position.y + core->camera_position.y * parallax.y);
}

internal iv2 GetChunkPosFromEntity(Entity *entity)
{
	Assert(entity);
	if (EntityHasProperty(entity, ENTITY_PROPERTY_positional))
	{
		if (EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
		{
			v2 entity_pos = ParallaxPosition(entity->position, GetEntityParallaxAmount(entity));
			iv2 pos = iv2(WorldSpaceToChunkIndex(entity_pos.x),
						  WorldSpaceToChunkIndex(entity_pos.y));
			return pos;
		}
		else
		{
			iv2 pos = iv2(WorldSpaceToChunkIndex(entity->position.x),
						  WorldSpaceToChunkIndex(entity->position.y));
			return pos;
		}
	}
	else
	{
		LogWarning("Entity %s isn't positional, can't retrieve chunk", entity->debug_name);
		return iv2(INT_MAX, INT_MAX);
	}
}

internal void GetChunksFromEntity(Entity *entity, iv2 *chunks)
{
	Assert(entity);
	
	for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
	{
		chunks[i] = iv2(INT_MAX, INT_MAX);
	}
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_positional))
	{
		if (EntityHasProperty(entity, ENTITY_PROPERTY_physical))
		{
			// TODO(randy):
		}
		else if (EntityHasProperty(entity, ENTITY_PROPERTY_sprite))
		{
			v4 rect;
			if (EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
			{
				rect = GetEntityVisibleParallaxRect(entity);
			}
			else
			{
				rect = GetEntityRectViaSprite(entity);
			}
			i32 count;
			GetChunkPositionsInRect(chunks, &count, rect, 0);
		}
	}
	else
	{
		LogWarning("Can't get chunks from a non-positional entity");
	}
}

internal Entity *GetClosestEntityWithProperty(EntityProperty property, f32 max_distance_from_player)
{
	// NOTE(randy): This will eventually prioritise player direction and whatnot to fine tune selection
	Entity *character = GetCharacterEntity();
	
	Entity *closest_entity = 0;
	f32 closest_mag = 0.0f;
	for (Entity *entity = 0; IncrementEntityWithProperty(&entity, property);)
	{
		if (closest_entity)
		{
			f32 entity_mag = fabsf(PythagSolve(character->position.x - entity->position.x,
											   character->position.y - entity->position.y));
			if (entity_mag < closest_mag)
			{
				closest_entity = entity;
				closest_mag = entity_mag;
			}
		}
		else
		{
			closest_entity = entity;
			closest_mag = fabsf(PythagSolve(character->position.x - entity->position.x,
											character->position.y - entity->position.y));
		}
	}
	
	return (closest_mag <= max_distance_from_player ? closest_entity : 0);
}

internal c2AABB RectToAABB(v4 rect)
{
	c2AABB aabb = {0};
	aabb.min = c2V(rect.x, rect.y);
	aabb.max = c2V(rect.x + rect.width, rect.y + rect.height);
	return aabb;
}

internal v4 GetEntityRectViaSprite(Entity *entity)
{
	v4 rect = {0};
	if (!EntityHasProperty(entity, ENTITY_PROPERTY_positional))
		return rect;
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_sprite))
	{
		SpriteData *sprite = &global_sprite_data[entity->sprite_data.sprite];
		
		v2 size = v2(sprite->source.width,
					 sprite->source.height);
		
		v2 pos = v2(size.x / -2.0f, -size.y);
		pos = V2AddV2(pos, sprite->offset);
		
		if (EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
		{
			v2 parallax = GetEntityParallaxAmount(entity);
			pos = V2AddV2(pos, ParallaxPosition(entity->position, parallax));
		}
		else
		{
			pos = V2AddV2(pos, entity->position);
		}
		
		rect = v4(pos.x, pos.y, size.width, size.height);
	}
	
	return rect;
}

internal b8 IsAABBOnScreen(c2AABB aabb)
{
	c2Shape shape = {0};
	shape.aabb = aabb;
	c2Shape screen = {0};
	screen.aabb = GetCameraRegionAABB();
	c2Manifold manifold = {0};
	GenerateCollisionManifold(shape, C2_SHAPE_TYPE_aabb, screen, C2_SHAPE_TYPE_aabb, &manifold);
	
	return (manifold.count > 0);
}

internal v4 GetEntityVisibleParallaxRect(Entity *entity)
{
	if (!EntityHasProperty(entity, ENTITY_PROPERTY_sprite) || !EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
	{
		LogWarning("Unable to extract parallax bounds from entity");
		return v4u(0.0f);
	}
	
	v2 parallax = GetEntityParallaxAmount(entity);
	v2 max_camera_view = {1280.0f, 720.0f}; // NOTE(randy): This is in world-space (2.0 zoom on a 2k screen)
	
	SpriteData *sprite = &global_sprite_data[entity->sprite_data.sprite];
	
	v2 size = v2(sprite->source.width,
				 sprite->source.height);
	
	// NOTE(randy): This is fucked lmao
	
	f32 left_x = (2.0f * parallax.x + max_camera_view.x + (size.x / 2.0f + sprite->offset.x) * 2.0f) / (-2.0f * parallax.x + 2.0f);
	left_x -= max_camera_view.x / 2.0f;
	
	f32 right_x = (2.0f * parallax.x + max_camera_view.x + (size.x / 2.0f + sprite->offset.x) * 2.0f) / (-2.0f * parallax.x + 2.0f);
	right_x -= max_camera_view.x / 2.0f;
	right_x *= -1.0f;
	
	f32 top_y = (2.0f * parallax.y + max_camera_view.y + (size.y + sprite->offset.y) * 2.0f) / (-2.0f * parallax.y + 2.0f);
	top_y -= max_camera_view.y / 2.0f;
	top_y *= -1.0f;
	
	f32 bottom_y = (2.0f * parallax.y + max_camera_view.y + sprite->offset.y * 2.0f) / (-2.0f * parallax.y + 2.0f);
	bottom_y -= max_camera_view.y / 2.0f;
	
	v4 bounds = v4(left_x + entity->position.x,
				   top_y + entity->position.y,
				   right_x - left_x,
				   bottom_y - top_y);
	
	return bounds;
}

internal b8 IsPositionOverlappingEntity(Entity *entity, v2 pos)
{
	if (!EntityHasProperty(entity, ENTITY_PROPERTY_positional))
		return 0;
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_sprite))
	{
		c2AABB aabb = RectToAABB(GetEntityRectViaSprite(entity));
		c2Shape shape = {0};
		shape.aabb = aabb;
		return IsV2OverlappingShape(pos, shape, C2_SHAPE_TYPE_aabb);
	}
	else if (EntityHasProperty(entity, ENTITY_PROPERTY_physical))
	{
		return IsV2OverlappingShape(pos, GetEntityShapeInWorldspace(entity), entity->physics.shape_type);
	}
	else if (EntityHasProperty(entity, ENTITY_PROPERTY_text_note))
	{
		// TODO(randy): Make it the actual text size
		f32 temp_size = 20.0f;
		v4 rect = v4(entity->position.x - temp_size / 2.0f,
					 entity->position.y - temp_size / 2.0f,
					 temp_size,
					 temp_size);
		
		c2AABB aabb = RectToAABB(rect);
		c2Shape shape = {0};
		shape.aabb = aabb;
		return IsV2OverlappingShape(pos, shape, C2_SHAPE_TYPE_aabb);
	}
	
	return 0;
}

internal void WriteToFile(FILE *file, void *data, size_t size_bytes)
{
	if (fwrite(data, size_bytes, 1, file) != 1)
	{
		Assert(ferror(file) == 0); // NOTE(randy): Ensure this error is caught.
		Assert(0);
	}
}

internal void WriteElementsToFile(FILE *file, void *data, size_t size_bytes, size_t size_count)
{
	if (fwrite(data, size_bytes, size_count, file) != 1)
	{
		Assert(ferror(file) == 0); // NOTE(randy): Ensure this error is caught.
		Assert(0);
	}
}

internal void ReadFromFile(FILE *file, void *data, size_t size_bytes)
{
	if (fread(data, size_bytes, 1, file) != 1)
	{
		Assert(feof(file) == 0);
		Assert(ferror(file) == 0);
	}
}

internal void ReadElementsFromFile(FILE *file, void *data, size_t size_bytes, size_t size_count)
{
	if (fread(data, size_bytes, size_count, file) != 1)
	{
		Assert(feof(file) == 0);
		Assert(ferror(file) == 0);
	}
}

internal RunData *GetRunData()
{
	return core->run_data;
}

internal WorldData *GetWorldData()
{
	return &core->run_data->world_data;
}

internal CharacterData *GetCharacterData()
{
	return &core->run_data->world_data.character_data;
}

internal EditorData *GetEditorData()
{
	return core->editor_data;
}

internal i32 EntitySelectedIndex(Entity *entity)
{
	Assert(entity);
	
	for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
	{
		if (GetEditorData()->selected_entities[i] == -1)
			return -1;
		
		Entity *selected_entity = &GetRunData()->entities[GetEditorData()->selected_entities[i]];
		if (entity == selected_entity)
			return i;
	}
	
	return -1;
}

internal void SelectNewEntity(Entity *entity)
{
	Assert(entity);
	i32 slot = -1;
	for (i32 i = 0; i < MAX_SELECTED_ENTITIES; i++)
	{
		if (GetEditorData()->selected_entities[i] == -1)
		{
			slot = i;
			break;
		}
	}
	
	if (slot == -1)
		return;
	
	GetEditorData()->selected_entities[slot] = (i32)(entity - GetRunData()->entities);
}

internal v2 GetEntityPosition(Entity *entity)
{
	Assert(entity && EntityHasProperty(entity, ENTITY_PROPERTY_positional));
	
	if (EntityHasProperty(entity, ENTITY_PROPERTY_parallaxable))
	{
		return ParallaxPosition(entity->position, GetEntityParallaxAmount(entity));
	}
	
	return entity->position;
}

internal b8 IsEntityInViewRange(Entity *entity)
{
	return (entity->sprite_data.render_layer >= GetEditorData()->view_min &&
			entity->sprite_data.render_layer <= GetEditorData()->view_max);
}