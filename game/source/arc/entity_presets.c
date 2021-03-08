EntityPresetCallback(GroundSegment)
{
	EntitySetProperty(entity, ENTITY_PROPERTY_physical);
	EntitySetProperty(entity, ENTITY_PROPERTY_ground_segment);
	EntitySetProperty(entity, ENTITY_PROPERTY_map_entity);
	char *name = "ground segment";
	strcpy(entity->debug_name, name);
	
	v4 camera_region = GetCameraRegionRect();
	entity->position = v2(-camera_region.x - camera_region.z / 2.0f,
						  -camera_region.y - camera_region.w / 2.0f);
	
	Line line = {0};
	line.p1.x = -10.0f;
	line.p2.x = 10.0f;
	entity->physics.shape.line = line;
	entity->physics.shape_type = C2_SHAPE_TYPE_line;
	
	entity->physics.material.static_friction = 0.7f;
	entity->physics.material.dynamic_friction = 0.7f;
	entity->physics.mass_data.mass = 0.0f;
	entity->physics.mass_data.inv_mass = 0.0f;
	entity->physics.type |= PHYSICS_BODY_TYPE_FLAGS_ground;
	entity->physics.collide_against |= PHYSICS_BODY_TYPE_FLAGS_character | PHYSICS_BODY_TYPE_FLAGS_item;
}