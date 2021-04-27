EntityPresetCallback(TerrainSegment)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	EntitySetProperty(entity, ENTITY_PROPERTY_terrain_segment);
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
	EntitySetProperty(entity, ENTITY_PROPERTY_map_entity);
	EntitySetProperty(entity, ENTITY_PROPERTY_positional);
	EntitySetProperty(entity, ENTITY_PROPERTY_tree);
	EntitySetProperty(entity, ENTITY_PROPERTY_sprite);
	
	char *name = "Tree";
	strcpy(entity->debug_name, name);
	
	entity->sprite_data.render_layer = LAYER_TREE;
	entity->tree_type = TREE_TYPE_pine;
	
	UpdateEntitySprite(entity);
}