// TODO(randy): Integrate this with data desk to make it more streamlined. Rn when creating a new entity preset  you have to go to 3 different places.
// This'll require some sort of function body feature for data desk. Or maybe I can just pass it as a parameter?

EntityPresetCallback(TerrainSegment)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	EntitySetProperty(entity, ENTITY_PROPERTY_terrain_segment);
	EntitySetProperty(entity, ENTITY_PROPERTY_do_not_serialise);
	char *name = "terrain segment";
	strcpy(entity->debug_name, name);
	
	LineSegments lineseg = {0};
	entity->physics.shape.line_segments = lineseg;
	entity->physics.shape_type = C2_SHAPE_TYPE_line_segments;
	
	entity->physics.material.static_friction = 0.7f;
	entity->physics.material.dynamic_friction = 0.7f;
	entity->physics.mass_data.mass = 0.0f;
	entity->physics.mass_data.inv_mass = 0.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_ground;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_character | PHYSICS_BODY_TYPE_FLAGS_item;
}

EntityPresetCallback(Tree)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_tree);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_TREE;
	entity->tree_type = TREE_TYPE_pine;
	
	UpdateEntityWithDefaults(entity);
}

EntityPresetCallback(Yucca)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_yucca];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_SHRUB;
	entity->sprite_data.sprite = SPRITE_yucca;
}

EntityPresetCallback(BG1Hill)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG1_HILL;
	entity->sprite_data.sprite = SPRITE_bg1_hills_v2;
}

EntityPresetCallback(BG1Tree)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG1_TREE;
	entity->sprite_data.sprite = SPRITE_bg1_pine_tree_v1;
}

EntityPresetCallback(BG1Shrubs)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG1_SHRUBS;
	entity->sprite_data.sprite = SPRITE_bg1_shrub_v1;
}

EntityPresetCallback(BG1Sapling)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG1_SAPLING;
	entity->sprite_data.sprite = SPRITE_bg1_sapling_v1;
}

EntityPresetCallback(BG2Hill)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG2_HILL;
	entity->sprite_data.sprite = SPRITE_bg2_hills_v1;
}

EntityPresetCallback(BG2Tree)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG2_TREE;
	entity->sprite_data.sprite = SPRITE_bg2_pine_tree_v1;
}

EntityPresetCallback(BG2Shrubs)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG2_SHRUBS;
	entity->sprite_data.sprite = SPRITE_bg2_shrub_v1;
}

EntityPresetCallback(BG3Hill)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg1_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG3_HILL;
	entity->sprite_data.sprite = SPRITE_bg3_hills_v1;
}

EntityPresetCallback(BG3Trees)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG3_TREES;
	entity->sprite_data.sprite = SPRITE_bg3_pine_tree_v1;
}

EntityPresetCallback(BG3Shrubs)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG3_SHRUBS;
	entity->sprite_data.sprite = SPRITE_bg3_shrub_v1;
}

EntityPresetCallback(MidMountains)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_MID_MOUNTAINS;
	entity->sprite_data.sprite = SPRITE_mid_mountains;
}

EntityPresetCallback(FarMountains)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_FAR_MOUNTAINS;
	entity->sprite_data.sprite = SPRITE_far_mountains;
}

EntityPresetCallback(TextNote)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_text_note);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_text_note];
	strcpy(entity->debug_name, preset_data->print_name);
}

EntityPresetCallback(Item)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_item);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	EntitySetProperty(entity, ENTITY_PROPERTY_interactable);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_item];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_ITEMS;
	entity->item.type = ITEM_TYPE_flint;
	entity->item.stack_size = 1;
	
	UpdateEntityWithDefaults(entity);
	
	c2Circle circle = {
		.p = c2V(0.0f, -10.0f),
		.r = 10.0f,
	};
	entity->physics.shape.circle = circle;
	entity->physics.shape_type = C2_SHAPE_TYPE_circle;
	entity->physics.mass_data.mass = 10.0f;
	entity->physics.mass_data.inv_mass = 1.0f / 10.0f;
	entity->physics.material.restitution = 0.1f;
	entity->physics.material.static_friction = 0.1f;
	entity->physics.material.dynamic_friction = 0.1f;
	entity->physics.gravity_multiplier = 1.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_item;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_ground | PHYSICS_BODY_TYPE_FLAGS_station;
	
	entity->priority = 2.0f;
}

EntityPresetCallback(Blueprint)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	EntitySetProperty(entity, ENTITY_PROPERTY_blueprint);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_text_note];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.tint = v4(0.25f, 0.25f, 0.5f, 0.5f);
	entity->sprite_data.render_layer = LAYER_STRUCTURE;
	
	UpdateEntityWithDefaults(entity);
}