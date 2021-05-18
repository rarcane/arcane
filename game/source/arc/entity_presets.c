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
	
	char *name = "Tree";
	strcpy(entity->debug_name, name);
	
	entity->sprite_data.render_layer = LAYER_TREE;
	entity->tree_type = TREE_TYPE_pine;
	
	UpdateEntitySprite(entity);
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

EntityPresetCallback(BG2Tree)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_parallaxable);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	EntityPresetTypeData *preset_data = &global_entity_preset_type_data[ENTITY_PRESET_TYPE_bg2_tree];
	strcpy(entity->debug_name, preset_data->print_name);
	
	entity->sprite_data.render_layer = LAYER_BG2_TREE;
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
	
	UpdateEntitySprite(entity);
	
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