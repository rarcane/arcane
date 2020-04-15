static char *GetGeneralisedEntityTypeName(GeneralisedEntityType type)
{
switch(type)
{
case GENERALISED_ENTITY_TYPE_undefined:
return "Undefined";
break;
case GENERALISED_ENTITY_TYPE_character:
return "Character";
break;
case GENERALISED_ENTITY_TYPE_monster:
return "Monster";
break;
case GENERALISED_ENTITY_TYPE_animal:
return "Animal";
break;
case GENERALISED_ENTITY_TYPE_item:
return "Item";
break;
case GENERALISED_ENTITY_TYPE_storage:
return "Storage";
break;
case GENERALISED_ENTITY_TYPE_resource:
return "Resource";
break;
case GENERALISED_ENTITY_TYPE_scenic:
return "Scenic";
break;
case GENERALISED_ENTITY_TYPE_ground:
return "Ground";
break;
case GENERALISED_ENTITY_TYPE_pixel_object:
return "Pixel Object";
break;
default:
return "INVALID";
break;
}
}

static char *GetEditorStateName(EditorState type)
{
switch(type)
{
case EDITOR_STATE_none:
return "None";
break;
case EDITOR_STATE_entity:
return "Entity";
break;
case EDITOR_STATE_terrain:
return "Terrain";
break;
case EDITOR_STATE_collision:
return "Collision";
break;
default:
return "INVALID";
break;
}
}

static char *GetStaticSpriteName(StaticSprite type)
{
switch(type)
{
case STATIC_SPRITE_INVALID:
return "INVALID";
break;
case STATIC_SPRITE_ground_arctic:
return "Ground Arctic";
break;
case STATIC_SPRITE_ground_desert:
return "Ground Desert";
break;
case STATIC_SPRITE_ground_plains:
return "Ground Plains";
break;
case STATIC_SPRITE_player:
return "Player";
break;
case STATIC_SPRITE_flint_sword_icon:
return "Flint Sword Icon";
break;
case STATIC_SPRITE_flint_sword_ground:
return "Flint Sword Ground";
break;
case STATIC_SPRITE_far_mountains:
return "Far Mountains";
break;
case STATIC_SPRITE_mid_mountains:
return "Mid Mountains";
break;
case STATIC_SPRITE_cloud_v1:
return "Cloud V1";
break;
case STATIC_SPRITE_cloud_v2:
return "Cloud V2";
break;
case STATIC_SPRITE_cloud_v3:
return "Cloud V3";
break;
case STATIC_SPRITE_cloud_v4:
return "Cloud V4";
break;
case STATIC_SPRITE_cloud_v5:
return "Cloud V5";
break;
case STATIC_SPRITE_cloud_v6:
return "Cloud V6";
break;
case STATIC_SPRITE_ground_forest_flat:
return "Ground Forest Flat";
break;
case STATIC_SPRITE_ground_forest_10_degree:
return "Ground Forest 10 Degree";
break;
case STATIC_SPRITE_ground_rocks_v1:
return "Ground Rocks V1";
break;
case STATIC_SPRITE_ground_rocks_v2:
return "Ground Rocks V2";
break;
case STATIC_SPRITE_pine_tree_v1:
return "Pine Tree V1";
break;
case STATIC_SPRITE_hills_1_v1:
return "Hills 1v1";
break;
case STATIC_SPRITE_bg1_shrub_v1:
return "Bg1 Shrub V1";
break;
case STATIC_SPRITE_bg1_shrub_v2:
return "Bg1 Shrub V2";
break;
case STATIC_SPRITE_bg1_shrub_v3:
return "Bg1 Shrub V3";
break;
case STATIC_SPRITE_bg1_sapling_v1:
return "Bg1 Sapling V1";
break;
case STATIC_SPRITE_bg1_sapling_v2:
return "Bg1 Sapling V2";
break;
case STATIC_SPRITE_bg1_sapling_v3:
return "Bg1 Sapling V3";
break;
case STATIC_SPRITE_bg1_pine_tree_v1:
return "Bg1 Pine Tree V1";
break;
case STATIC_SPRITE_bg1_pine_tree_v2:
return "Bg1 Pine Tree V2";
break;
case STATIC_SPRITE_bg2_hills_v1:
return "Bg2 Hills V1";
break;
case STATIC_SPRITE_bg2_hills_v2:
return "Bg2 Hills V2";
break;
case STATIC_SPRITE_bg2_shrub_v1:
return "Bg2 Shrub V1";
break;
case STATIC_SPRITE_bg2_shrub_v2:
return "Bg2 Shrub V2";
break;
case STATIC_SPRITE_bg2_shrub_v3:
return "Bg2 Shrub V3";
break;
case STATIC_SPRITE_bg2_pine_tree_v1:
return "Bg2 Pine Tree V1";
break;
case STATIC_SPRITE_bg2_pine_tree_v2:
return "Bg2 Pine Tree V2";
break;
case STATIC_SPRITE_bg3_hills_v1:
return "Bg3 Hills V1";
break;
case STATIC_SPRITE_bg3_shrub_v1:
return "Bg3 Shrub V1";
break;
case STATIC_SPRITE_bg3_shrub_v2:
return "Bg3 Shrub V2";
break;
case STATIC_SPRITE_bg3_shrub_v3:
return "Bg3 Shrub V3";
break;
case STATIC_SPRITE_bg3_shrub_v4:
return "Bg3 Shrub V4";
break;
case STATIC_SPRITE_bg3_pine_tree_v1:
return "Bg3 Pine Tree V1";
break;
case STATIC_SPRITE_bg3_pine_tree_v2:
return "Bg3 Pine Tree V2";
break;
case STATIC_SPRITE_bg3_pine_tree_v3:
return "Bg3 Pine Tree V3";
break;
case STATIC_SPRITE_bg3_pine_tree_v4:
return "Bg3 Pine Tree V4";
break;
case STATIC_SPRITE_bg3_pine_tree_v5:
return "Bg3 Pine Tree V5";
break;
case STATIC_SPRITE_bg3_pine_tree_v6:
return "Bg3 Pine Tree V6";
break;
case STATIC_SPRITE_bg3_pine_tree_v7:
return "Bg3 Pine Tree V7";
break;
case STATIC_SPRITE_bg3_pine_tree_v8:
return "Bg3 Pine Tree V8";
break;
case STATIC_SPRITE_y_axis_arrow_icon:
return "Yaxis Arrow Icon";
break;
case STATIC_SPRITE_x_axis_arrow_icon:
return "Xaxis Arrow Icon";
break;
case STATIC_SPRITE_circle_icon:
return "Circle Icon";
break;
default:
return "INVALID";
break;
}
}

static char *GetDynamicSpriteName(DynamicSprite type)
{
switch(type)
{
case DYNAMIC_SPRITE_INVALID:
return "INVALID";
break;
case DYNAMIC_SPRITE_player_idle:
return "Player Idle";
break;
case DYNAMIC_SPRITE_player_walking:
return "Player Walking";
break;
case DYNAMIC_SPRITE_player_sprinting:
return "Player Sprinting";
break;
case DYNAMIC_SPRITE_birch_tree1:
return "Birch Tree1";
break;
case DYNAMIC_SPRITE_birch_tree2:
return "Birch Tree2";
break;
case DYNAMIC_SPRITE_birch_tree3:
return "Birch Tree3";
break;
case DYNAMIC_SPRITE_birch_tree4:
return "Birch Tree4";
break;
default:
return "INVALID";
break;
}
}

static char *GetArcEntityAnimationStateName(ArcEntityAnimationState type)
{
switch(type)
{
case ARC_ENTITY_ANIMATION_STATE_player_idle:
return "Player Idle";
break;
case ARC_ENTITY_ANIMATION_STATE_player_walking:
return "Player Walking";
break;
case ARC_ENTITY_ANIMATION_STATE_player_sprinting:
return "Player Sprinting";
break;
default:
return "INVALID";
break;
}
}

static char *GetArcEntityTypeName(ArcEntityType type)
{
switch(type)
{
case ARC_ENTITY_TYPE_player:
return "Player";
break;
default:
return "INVALID";
break;
}
}

static char *GetItemTypeName(ItemType type)
{
switch(type)
{
case ITEM_TYPE_flint_sword:
return "Flint Sword";
break;
default:
return "INVALID";
break;
}
}

static char *Getc2ShapeTypeName(c2ShapeType type)
{
switch(type)
{
case C2_SHAPE_TYPE_aabb:
return "Aabb";
break;
case C2_SHAPE_TYPE_capsule:
return "Capsule";
break;
case C2_SHAPE_TYPE_circle:
return "Circle";
break;
case C2_SHAPE_TYPE_poly:
return "Poly";
break;
case C2_SHAPE_TYPE_line:
return "Line";
break;
default:
return "INVALID";
break;
}
}

internal PositionComponent *AddPositionComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_position_component_id > 0, "Max PositionComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_position] == 0, "Entity already has a PositionComponent");
    i32 new_comp_id = core->world_data->entity_components.free_position_component_id;

    PositionComponent *comp = &core->world_data->entity_components.position_components[new_comp_id - 1];
    *comp = GetDefaultPositionComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_position] = new_comp_id;

    if (core->world_data->entity_components.position_component_count == core->world_data->entity_components.free_position_component_id - 1)
    {
        core->world_data->entity_components.position_component_count++;
        core->world_data->entity_components.free_position_component_id++;
    }

    if (core->world_data->entity_components.position_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.position_component_count != core->world_data->entity_components.free_position_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.position_component_count; i++)
            {
                if (core->world_data->entity_components.position_components[i].component_id)
                {
                    core->world_data->entity_components.free_position_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_position_component_id = 0;
    }

    return comp;
}

internal void RemovePositionComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_position] != 0, "Entity does not have a PositionComponent");
    PositionComponent *comp = &core->world_data->entity_components.position_components[entity->component_ids[COMPONENT_position] - 1];

    if (comp->component_id < core->world_data->entity_components.free_position_component_id)
        core->world_data->entity_components.free_position_component_id = comp->component_id;

    PositionComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_position] = 0;
}

internal PositionComponent *GetPositionComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_position], "Entity doesn't have a PositionComponent");
    PositionComponent *comp = &core->world_data->entity_components.position_components[entity->component_ids[COMPONENT_position] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_position], "IDs are mismatched.");
    return comp;
}

internal SpriteComponent *AddSpriteComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_sprite_component_id > 0, "Max SpriteComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_sprite] == 0, "Entity already has a SpriteComponent");
    i32 new_comp_id = core->world_data->entity_components.free_sprite_component_id;

    SpriteComponent *comp = &core->world_data->entity_components.sprite_components[new_comp_id - 1];
    *comp = GetDefaultSpriteComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_sprite] = new_comp_id;

    if (core->world_data->entity_components.sprite_component_count == core->world_data->entity_components.free_sprite_component_id - 1)
    {
        core->world_data->entity_components.sprite_component_count++;
        core->world_data->entity_components.free_sprite_component_id++;
    }

    if (core->world_data->entity_components.sprite_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.sprite_component_count != core->world_data->entity_components.free_sprite_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.sprite_component_count; i++)
            {
                if (core->world_data->entity_components.sprite_components[i].component_id)
                {
                    core->world_data->entity_components.free_sprite_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_sprite_component_id = 0;
    }

    return comp;
}

internal void RemoveSpriteComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_sprite] != 0, "Entity does not have a SpriteComponent");
    SpriteComponent *comp = &core->world_data->entity_components.sprite_components[entity->component_ids[COMPONENT_sprite] - 1];

    if (comp->component_id < core->world_data->entity_components.free_sprite_component_id)
        core->world_data->entity_components.free_sprite_component_id = comp->component_id;

    SpriteComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_sprite] = 0;
}

internal SpriteComponent *GetSpriteComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_sprite], "Entity doesn't have a SpriteComponent");
    SpriteComponent *comp = &core->world_data->entity_components.sprite_components[entity->component_ids[COMPONENT_sprite] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_sprite], "IDs are mismatched.");
    return comp;
}

internal AnimationComponent *AddAnimationComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_animation_component_id > 0, "Max AnimationComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_animation] == 0, "Entity already has a AnimationComponent");
    i32 new_comp_id = core->world_data->entity_components.free_animation_component_id;

    AnimationComponent *comp = &core->world_data->entity_components.animation_components[new_comp_id - 1];
    *comp = GetDefaultAnimationComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_animation] = new_comp_id;

    if (core->world_data->entity_components.animation_component_count == core->world_data->entity_components.free_animation_component_id - 1)
    {
        core->world_data->entity_components.animation_component_count++;
        core->world_data->entity_components.free_animation_component_id++;
    }

    if (core->world_data->entity_components.animation_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.animation_component_count != core->world_data->entity_components.free_animation_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.animation_component_count; i++)
            {
                if (core->world_data->entity_components.animation_components[i].component_id)
                {
                    core->world_data->entity_components.free_animation_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_animation_component_id = 0;
    }

    return comp;
}

internal void RemoveAnimationComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_animation] != 0, "Entity does not have a AnimationComponent");
    AnimationComponent *comp = &core->world_data->entity_components.animation_components[entity->component_ids[COMPONENT_animation] - 1];

    if (comp->component_id < core->world_data->entity_components.free_animation_component_id)
        core->world_data->entity_components.free_animation_component_id = comp->component_id;

    AnimationComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_animation] = 0;
}

internal AnimationComponent *GetAnimationComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_animation], "Entity doesn't have a AnimationComponent");
    AnimationComponent *comp = &core->world_data->entity_components.animation_components[entity->component_ids[COMPONENT_animation] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_animation], "IDs are mismatched.");
    return comp;
}

internal PhysicsBodyComponent *AddPhysicsBodyComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_physics_body_component_id > 0, "Max PhysicsBodyComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_physics_body] == 0, "Entity already has a PhysicsBodyComponent");
    i32 new_comp_id = core->world_data->entity_components.free_physics_body_component_id;

    PhysicsBodyComponent *comp = &core->world_data->entity_components.physics_body_components[new_comp_id - 1];
    *comp = GetDefaultPhysicsBodyComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_physics_body] = new_comp_id;

    if (core->world_data->entity_components.physics_body_component_count == core->world_data->entity_components.free_physics_body_component_id - 1)
    {
        core->world_data->entity_components.physics_body_component_count++;
        core->world_data->entity_components.free_physics_body_component_id++;
    }

    if (core->world_data->entity_components.physics_body_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.physics_body_component_count != core->world_data->entity_components.free_physics_body_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.physics_body_component_count; i++)
            {
                if (core->world_data->entity_components.physics_body_components[i].component_id)
                {
                    core->world_data->entity_components.free_physics_body_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_physics_body_component_id = 0;
    }

    return comp;
}

internal void RemovePhysicsBodyComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_physics_body] != 0, "Entity does not have a PhysicsBodyComponent");
    PhysicsBodyComponent *comp = &core->world_data->entity_components.physics_body_components[entity->component_ids[COMPONENT_physics_body] - 1];

    if (comp->component_id < core->world_data->entity_components.free_physics_body_component_id)
        core->world_data->entity_components.free_physics_body_component_id = comp->component_id;

    PhysicsBodyComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_physics_body] = 0;
}

internal PhysicsBodyComponent *GetPhysicsBodyComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_physics_body], "Entity doesn't have a PhysicsBodyComponent");
    PhysicsBodyComponent *comp = &core->world_data->entity_components.physics_body_components[entity->component_ids[COMPONENT_physics_body] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_physics_body], "IDs are mismatched.");
    return comp;
}

internal MovementComponent *AddMovementComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_movement_component_id > 0, "Max MovementComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_movement] == 0, "Entity already has a MovementComponent");
    i32 new_comp_id = core->world_data->entity_components.free_movement_component_id;

    MovementComponent *comp = &core->world_data->entity_components.movement_components[new_comp_id - 1];
    *comp = GetDefaultMovementComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_movement] = new_comp_id;

    if (core->world_data->entity_components.movement_component_count == core->world_data->entity_components.free_movement_component_id - 1)
    {
        core->world_data->entity_components.movement_component_count++;
        core->world_data->entity_components.free_movement_component_id++;
    }

    if (core->world_data->entity_components.movement_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.movement_component_count != core->world_data->entity_components.free_movement_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.movement_component_count; i++)
            {
                if (core->world_data->entity_components.movement_components[i].component_id)
                {
                    core->world_data->entity_components.free_movement_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_movement_component_id = 0;
    }

    return comp;
}

internal void RemoveMovementComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_movement] != 0, "Entity does not have a MovementComponent");
    MovementComponent *comp = &core->world_data->entity_components.movement_components[entity->component_ids[COMPONENT_movement] - 1];

    if (comp->component_id < core->world_data->entity_components.free_movement_component_id)
        core->world_data->entity_components.free_movement_component_id = comp->component_id;

    MovementComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_movement] = 0;
}

internal MovementComponent *GetMovementComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_movement], "Entity doesn't have a MovementComponent");
    MovementComponent *comp = &core->world_data->entity_components.movement_components[entity->component_ids[COMPONENT_movement] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_movement], "IDs are mismatched.");
    return comp;
}

internal ArcEntityComponent *AddArcEntityComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_arc_entity_component_id > 0, "Max ArcEntityComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_arc_entity] == 0, "Entity already has a ArcEntityComponent");
    i32 new_comp_id = core->world_data->entity_components.free_arc_entity_component_id;

    ArcEntityComponent *comp = &core->world_data->entity_components.arc_entity_components[new_comp_id - 1];
    *comp = GetDefaultArcEntityComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_arc_entity] = new_comp_id;

    if (core->world_data->entity_components.arc_entity_component_count == core->world_data->entity_components.free_arc_entity_component_id - 1)
    {
        core->world_data->entity_components.arc_entity_component_count++;
        core->world_data->entity_components.free_arc_entity_component_id++;
    }

    if (core->world_data->entity_components.arc_entity_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.arc_entity_component_count != core->world_data->entity_components.free_arc_entity_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.arc_entity_component_count; i++)
            {
                if (core->world_data->entity_components.arc_entity_components[i].component_id)
                {
                    core->world_data->entity_components.free_arc_entity_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_arc_entity_component_id = 0;
    }

    return comp;
}

internal void RemoveArcEntityComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_arc_entity] != 0, "Entity does not have a ArcEntityComponent");
    ArcEntityComponent *comp = &core->world_data->entity_components.arc_entity_components[entity->component_ids[COMPONENT_arc_entity] - 1];

    if (comp->component_id < core->world_data->entity_components.free_arc_entity_component_id)
        core->world_data->entity_components.free_arc_entity_component_id = comp->component_id;

    ArcEntityComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_arc_entity] = 0;
}

internal ArcEntityComponent *GetArcEntityComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_arc_entity], "Entity doesn't have a ArcEntityComponent");
    ArcEntityComponent *comp = &core->world_data->entity_components.arc_entity_components[entity->component_ids[COMPONENT_arc_entity] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_arc_entity], "IDs are mismatched.");
    return comp;
}

internal ItemComponent *AddItemComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_item_component_id > 0, "Max ItemComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_item] == 0, "Entity already has a ItemComponent");
    i32 new_comp_id = core->world_data->entity_components.free_item_component_id;

    ItemComponent *comp = &core->world_data->entity_components.item_components[new_comp_id - 1];
    *comp = GetDefaultItemComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_item] = new_comp_id;

    if (core->world_data->entity_components.item_component_count == core->world_data->entity_components.free_item_component_id - 1)
    {
        core->world_data->entity_components.item_component_count++;
        core->world_data->entity_components.free_item_component_id++;
    }

    if (core->world_data->entity_components.item_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.item_component_count != core->world_data->entity_components.free_item_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.item_component_count; i++)
            {
                if (core->world_data->entity_components.item_components[i].component_id)
                {
                    core->world_data->entity_components.free_item_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_item_component_id = 0;
    }

    return comp;
}

internal void RemoveItemComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_item] != 0, "Entity does not have a ItemComponent");
    ItemComponent *comp = &core->world_data->entity_components.item_components[entity->component_ids[COMPONENT_item] - 1];

    if (comp->component_id < core->world_data->entity_components.free_item_component_id)
        core->world_data->entity_components.free_item_component_id = comp->component_id;

    ItemComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_item] = 0;
}

internal ItemComponent *GetItemComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_item], "Entity doesn't have a ItemComponent");
    ItemComponent *comp = &core->world_data->entity_components.item_components[entity->component_ids[COMPONENT_item] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_item], "IDs are mismatched.");
    return comp;
}

internal TriggerComponent *AddTriggerComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_trigger_component_id > 0, "Max TriggerComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_trigger] == 0, "Entity already has a TriggerComponent");
    i32 new_comp_id = core->world_data->entity_components.free_trigger_component_id;

    TriggerComponent *comp = &core->world_data->entity_components.trigger_components[new_comp_id - 1];
    *comp = GetDefaultTriggerComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_trigger] = new_comp_id;

    if (core->world_data->entity_components.trigger_component_count == core->world_data->entity_components.free_trigger_component_id - 1)
    {
        core->world_data->entity_components.trigger_component_count++;
        core->world_data->entity_components.free_trigger_component_id++;
    }

    if (core->world_data->entity_components.trigger_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.trigger_component_count != core->world_data->entity_components.free_trigger_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.trigger_component_count; i++)
            {
                if (core->world_data->entity_components.trigger_components[i].component_id)
                {
                    core->world_data->entity_components.free_trigger_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_trigger_component_id = 0;
    }

    return comp;
}

internal void RemoveTriggerComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_trigger] != 0, "Entity does not have a TriggerComponent");
    TriggerComponent *comp = &core->world_data->entity_components.trigger_components[entity->component_ids[COMPONENT_trigger] - 1];

    if (comp->component_id < core->world_data->entity_components.free_trigger_component_id)
        core->world_data->entity_components.free_trigger_component_id = comp->component_id;

    TriggerComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_trigger] = 0;
}

internal TriggerComponent *GetTriggerComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_trigger], "Entity doesn't have a TriggerComponent");
    TriggerComponent *comp = &core->world_data->entity_components.trigger_components[entity->component_ids[COMPONENT_trigger] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_trigger], "IDs are mismatched.");
    return comp;
}

internal StorageComponent *AddStorageComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_storage_component_id > 0, "Max StorageComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_storage] == 0, "Entity already has a StorageComponent");
    i32 new_comp_id = core->world_data->entity_components.free_storage_component_id;

    StorageComponent *comp = &core->world_data->entity_components.storage_components[new_comp_id - 1];
    *comp = GetDefaultStorageComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_storage] = new_comp_id;

    if (core->world_data->entity_components.storage_component_count == core->world_data->entity_components.free_storage_component_id - 1)
    {
        core->world_data->entity_components.storage_component_count++;
        core->world_data->entity_components.free_storage_component_id++;
    }

    if (core->world_data->entity_components.storage_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.storage_component_count != core->world_data->entity_components.free_storage_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.storage_component_count; i++)
            {
                if (core->world_data->entity_components.storage_components[i].component_id)
                {
                    core->world_data->entity_components.free_storage_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_storage_component_id = 0;
    }

    return comp;
}

internal void RemoveStorageComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_storage] != 0, "Entity does not have a StorageComponent");
    StorageComponent *comp = &core->world_data->entity_components.storage_components[entity->component_ids[COMPONENT_storage] - 1];

    if (comp->component_id < core->world_data->entity_components.free_storage_component_id)
        core->world_data->entity_components.free_storage_component_id = comp->component_id;

    StorageComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_storage] = 0;
}

internal StorageComponent *GetStorageComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_storage], "Entity doesn't have a StorageComponent");
    StorageComponent *comp = &core->world_data->entity_components.storage_components[entity->component_ids[COMPONENT_storage] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_storage], "IDs are mismatched.");
    return comp;
}

internal ParallaxComponent *AddParallaxComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_parallax_component_id > 0, "Max ParallaxComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_parallax] == 0, "Entity already has a ParallaxComponent");
    i32 new_comp_id = core->world_data->entity_components.free_parallax_component_id;

    ParallaxComponent *comp = &core->world_data->entity_components.parallax_components[new_comp_id - 1];
    *comp = GetDefaultParallaxComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_parallax] = new_comp_id;

    if (core->world_data->entity_components.parallax_component_count == core->world_data->entity_components.free_parallax_component_id - 1)
    {
        core->world_data->entity_components.parallax_component_count++;
        core->world_data->entity_components.free_parallax_component_id++;
    }

    if (core->world_data->entity_components.parallax_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.parallax_component_count != core->world_data->entity_components.free_parallax_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.parallax_component_count; i++)
            {
                if (core->world_data->entity_components.parallax_components[i].component_id)
                {
                    core->world_data->entity_components.free_parallax_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_parallax_component_id = 0;
    }

    return comp;
}

internal void RemoveParallaxComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_parallax] != 0, "Entity does not have a ParallaxComponent");
    ParallaxComponent *comp = &core->world_data->entity_components.parallax_components[entity->component_ids[COMPONENT_parallax] - 1];

    if (comp->component_id < core->world_data->entity_components.free_parallax_component_id)
        core->world_data->entity_components.free_parallax_component_id = comp->component_id;

    ParallaxComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_parallax] = 0;
}

internal ParallaxComponent *GetParallaxComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_parallax], "Entity doesn't have a ParallaxComponent");
    ParallaxComponent *comp = &core->world_data->entity_components.parallax_components[entity->component_ids[COMPONENT_parallax] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_parallax], "IDs are mismatched.");
    return comp;
}

internal ParticleEmitterComponent *AddParticleEmitterComponent(Entity *entity)
{
    R_DEV_ASSERT(core->world_data->entity_components.free_particle_emitter_component_id > 0, "Max ParticleEmitterComponent reached.");
    R_DEV_ASSERT(entity->component_ids[COMPONENT_particle_emitter] == 0, "Entity already has a ParticleEmitterComponent");
    i32 new_comp_id = core->world_data->entity_components.free_particle_emitter_component_id;

    ParticleEmitterComponent *comp = &core->world_data->entity_components.particle_emitter_components[new_comp_id - 1];
    *comp = GetDefaultParticleEmitterComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_particle_emitter] = new_comp_id;

    if (core->world_data->entity_components.particle_emitter_component_count == core->world_data->entity_components.free_particle_emitter_component_id - 1)
    {
        core->world_data->entity_components.particle_emitter_component_count++;
        core->world_data->entity_components.free_particle_emitter_component_id++;
    }

    if (core->world_data->entity_components.particle_emitter_component_count < MAX_ACTIVE_ENTITIES)
    {
        if (core->world_data->entity_components.particle_emitter_component_count != core->world_data->entity_components.free_particle_emitter_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->entity_components.particle_emitter_component_count; i++)
            {
                if (core->world_data->entity_components.particle_emitter_components[i].component_id)
                {
                    core->world_data->entity_components.free_particle_emitter_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    else
    {
        core->world_data->entity_components.free_particle_emitter_component_id = 0;
    }

    return comp;
}

internal void RemoveParticleEmitterComponent(Entity *entity)
{
    R_DEV_ASSERT(entity->component_ids[COMPONENT_particle_emitter] != 0, "Entity does not have a ParticleEmitterComponent");
    ParticleEmitterComponent *comp = &core->world_data->entity_components.particle_emitter_components[entity->component_ids[COMPONENT_particle_emitter] - 1];

    if (comp->component_id < core->world_data->entity_components.free_particle_emitter_component_id)
        core->world_data->entity_components.free_particle_emitter_component_id = comp->component_id;

    ParticleEmitterComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_particle_emitter] = 0;
}

internal ParticleEmitterComponent *GetParticleEmitterComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    R_DEV_ASSERT(entity->component_ids[COMPONENT_particle_emitter], "Entity doesn't have a ParticleEmitterComponent");
    ParticleEmitterComponent *comp = &core->world_data->entity_components.particle_emitter_components[entity->component_ids[COMPONENT_particle_emitter] - 1];
    R_DEV_ASSERT(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_particle_emitter], "IDs are mismatched.");
    return comp;
}

internal void RemoveComponent(Entity *entity, ComponentType type)
{
    switch (type)
    {
    case COMPONENT_position:
    {
        if (entity->component_ids[COMPONENT_position])
            RemovePositionComponent(entity);
        else
            R_BREAK("Entity doesn't have a PositionComponent to remove.");
        break;
    }
    case COMPONENT_sprite:
    {
        if (entity->component_ids[COMPONENT_sprite])
            RemoveSpriteComponent(entity);
        else
            R_BREAK("Entity doesn't have a SpriteComponent to remove.");
        break;
    }
    case COMPONENT_animation:
    {
        if (entity->component_ids[COMPONENT_animation])
            RemoveAnimationComponent(entity);
        else
            R_BREAK("Entity doesn't have a AnimationComponent to remove.");
        break;
    }
    case COMPONENT_physics_body:
    {
        if (entity->component_ids[COMPONENT_physics_body])
            RemovePhysicsBodyComponent(entity);
        else
            R_BREAK("Entity doesn't have a PhysicsBodyComponent to remove.");
        break;
    }
    case COMPONENT_movement:
    {
        if (entity->component_ids[COMPONENT_movement])
            RemoveMovementComponent(entity);
        else
            R_BREAK("Entity doesn't have a MovementComponent to remove.");
        break;
    }
    case COMPONENT_arc_entity:
    {
        if (entity->component_ids[COMPONENT_arc_entity])
            RemoveArcEntityComponent(entity);
        else
            R_BREAK("Entity doesn't have a ArcEntityComponent to remove.");
        break;
    }
    case COMPONENT_item:
    {
        if (entity->component_ids[COMPONENT_item])
            RemoveItemComponent(entity);
        else
            R_BREAK("Entity doesn't have a ItemComponent to remove.");
        break;
    }
    case COMPONENT_trigger:
    {
        if (entity->component_ids[COMPONENT_trigger])
            RemoveTriggerComponent(entity);
        else
            R_BREAK("Entity doesn't have a TriggerComponent to remove.");
        break;
    }
    case COMPONENT_storage:
    {
        if (entity->component_ids[COMPONENT_storage])
            RemoveStorageComponent(entity);
        else
            R_BREAK("Entity doesn't have a StorageComponent to remove.");
        break;
    }
    case COMPONENT_parallax:
    {
        if (entity->component_ids[COMPONENT_parallax])
            RemoveParallaxComponent(entity);
        else
            R_BREAK("Entity doesn't have a ParallaxComponent to remove.");
        break;
    }
    case COMPONENT_particle_emitter:
    {
        if (entity->component_ids[COMPONENT_particle_emitter])
            RemoveParticleEmitterComponent(entity);
        else
            R_BREAK("Entity doesn't have a ParticleEmitterComponent to remove.");
        break;
    }
    default:
        R_BREAK("Invalid component type.");
        break;
    }
}
internal void InitialiseComponents()
{
    core->world_data->entity_components.free_position_component_id = 1;
    core->world_data->entity_components.free_sprite_component_id = 1;
    core->world_data->entity_components.free_animation_component_id = 1;
    core->world_data->entity_components.free_physics_body_component_id = 1;
    core->world_data->entity_components.free_movement_component_id = 1;
    core->world_data->entity_components.free_arc_entity_component_id = 1;
    core->world_data->entity_components.free_item_component_id = 1;
    core->world_data->entity_components.free_trigger_component_id = 1;
    core->world_data->entity_components.free_storage_component_id = 1;
    core->world_data->entity_components.free_parallax_component_id = 1;
    core->world_data->entity_components.free_particle_emitter_component_id = 1;
}
internal void InitialiseUniqueEntities()
{
    core->world_data->free_cloud_entity_id = 1;
    core->world_data->free_ground_segment_entity_id = 1;
}
static CharacterEntity *InitialiseCharacterEntity()
{
    Entity *generic_entity = NewEntity("CharacterEntity", GENERALISED_ENTITY_TYPE_character);
    CharacterEntity *unique_entity = &core->world_data->character_entity;
    generic_entity->type = ENTITY_TYPE_character_entity;
    generic_entity->unique_entity_id = 1;
    unique_entity->entity_id = generic_entity->entity_id;

    AddPositionComponent(generic_entity);
    AddSpriteComponent(generic_entity);
    AddAnimationComponent(generic_entity);
    AddPhysicsBodyComponent(generic_entity);
    AddMovementComponent(generic_entity);
    AddArcEntityComponent(generic_entity);

    return unique_entity;
}

static CloudEntity *NewCloudEntity()
{
    R_DEV_ASSERT(core->world_data->free_cloud_entity_id > 0, "Maximum amount of CloudEntity entites reached");

    i32 new_unique_id = core->world_data->free_cloud_entity_id;

    Entity *generic_entity = NewEntity("CloudEntity", GENERALISED_ENTITY_TYPE_scenic);
    generic_entity->unique_entity_id = new_unique_id;
    generic_entity->type = ENTITY_TYPE_cloud_entity;
    CloudEntity *unique_entity = &core->world_data->cloud_entity_list[new_unique_id - 1];
    unique_entity->entity_id = generic_entity->entity_id;
    unique_entity->unique_entity_id = new_unique_id;

    AddPositionComponent(generic_entity);
    AddSpriteComponent(generic_entity);
    AddParallaxComponent(generic_entity);

    if (core->world_data->cloud_entity_count == core->world_data->free_cloud_entity_id - 1)
    {
        core->world_data->cloud_entity_count++;
        core->world_data->free_cloud_entity_id++;
    }

    if (core->world_data->cloud_entity_count < MAX_CLOUD_ENTITY_COUNT)
    {
        if (core->world_data->cloud_entity_count != core->world_data->free_cloud_entity_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->cloud_entity_count++; i++)
            {
                core->world_data->free_cloud_entity_id = i + 1;
                found = 1;
                break;
            }
            R_DEV_ASSERT(found, "Couldn't find a free index?");
        }
    }
    else
    {
        core->world_data->free_cloud_entity_id = 0;
    }

    return unique_entity;
}

static void DeleteCloudEntity(CloudEntity *entity)
{
    R_DEV_ASSERT(entity && entity->entity_id && entity->unique_entity_id, "Invalid entity");
    Entity *generic_entity = GetEntityWithID(entity->entity_id);
    R_DEV_ASSERT(generic_entity->unique_entity_id == entity->unique_entity_id, "Mismatched id.");
    generic_entity->unique_entity_id = 0;
    DeleteEntity(generic_entity);

    if (entity->unique_entity_id < core->world_data->free_cloud_entity_id);
        core->world_data->free_cloud_entity_id = entity->unique_entity_id;
    CloudEntity empty_entity = {0};
    *entity = empty_entity;
}

static GroundSegmentEntity *NewGroundSegmentEntity()
{
    R_DEV_ASSERT(core->world_data->free_ground_segment_entity_id > 0, "Maximum amount of GroundSegmentEntity entites reached");

    i32 new_unique_id = core->world_data->free_ground_segment_entity_id;

    Entity *generic_entity = NewEntity("GroundSegmentEntity", GENERALISED_ENTITY_TYPE_ground);
    generic_entity->unique_entity_id = new_unique_id;
    generic_entity->type = ENTITY_TYPE_ground_segment_entity;
    GroundSegmentEntity *unique_entity = &core->world_data->ground_segment_entity_list[new_unique_id - 1];
    unique_entity->entity_id = generic_entity->entity_id;
    unique_entity->unique_entity_id = new_unique_id;

    AddPositionComponent(generic_entity);
    AddPhysicsBodyComponent(generic_entity);

    if (core->world_data->ground_segment_entity_count == core->world_data->free_ground_segment_entity_id - 1)
    {
        core->world_data->ground_segment_entity_count++;
        core->world_data->free_ground_segment_entity_id++;
    }

    if (core->world_data->ground_segment_entity_count < MAX_GROUND_SEGMENT_ENTITY_COUNT)
    {
        if (core->world_data->ground_segment_entity_count != core->world_data->free_ground_segment_entity_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->world_data->ground_segment_entity_count++; i++)
            {
                core->world_data->free_ground_segment_entity_id = i + 1;
                found = 1;
                break;
            }
            R_DEV_ASSERT(found, "Couldn't find a free index?");
        }
    }
    else
    {
        core->world_data->free_ground_segment_entity_id = 0;
    }

    return unique_entity;
}

static void DeleteGroundSegmentEntity(GroundSegmentEntity *entity)
{
    R_DEV_ASSERT(entity && entity->entity_id && entity->unique_entity_id, "Invalid entity");
    Entity *generic_entity = GetEntityWithID(entity->entity_id);
    R_DEV_ASSERT(generic_entity->unique_entity_id == entity->unique_entity_id, "Mismatched id.");
    generic_entity->unique_entity_id = 0;
    DeleteEntity(generic_entity);

    if (entity->unique_entity_id < core->world_data->free_ground_segment_entity_id);
        core->world_data->free_ground_segment_entity_id = entity->unique_entity_id;
    GroundSegmentEntity empty_entity = {0};
    *entity = empty_entity;
}

static char *GetCellPropertiesTypeName(CellPropertiesType type)
{
switch(type)
{
case CELL_PROPERTIES_TYPE_air:
return "Air";
break;
case CELL_PROPERTIES_TYPE_liquid:
return "Liquid";
break;
case CELL_PROPERTIES_TYPE_solid:
return "Solid";
break;
default:
return "INVALID";
break;
}
}

static char *GetCellMaterialTypeName(CellMaterialType type)
{
switch(type)
{
case CELL_MATERIAL_TYPE_air:
return "Air";
break;
case CELL_MATERIAL_TYPE_water:
return "Water";
break;
case CELL_MATERIAL_TYPE_dirt:
return "Dirt";
break;
case CELL_MATERIAL_TYPE_sand:
return "Sand";
break;
default:
return "INVALID";
break;
}
}

static void WritePositionComponentToFile(FILE *file, PositionComponent *data)
{
    // 'parent_entity_id' in PositionComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in PositionComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'position' in PositionComponent
    WriteToFile(file, &data->position, sizeof(data->position));

}

static void FillPositionComponentPointersInFile(FILE *file, PositionComponent *data)
{
}

static void WriteSpriteComponentToFile(FILE *file, SpriteComponent *data)
{
    // 'parent_entity_id' in SpriteComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in SpriteComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'sprite_data' in SpriteComponent
    WriteToFile(file, &data->sprite_data, sizeof(data->sprite_data));

    // 'is_flipped' in SpriteComponent
    WriteToFile(file, &data->is_flipped, sizeof(data->is_flipped));

    // 'is_background_sprite' in SpriteComponent
    WriteToFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

}

static void FillSpriteComponentPointersInFile(FILE *file, SpriteComponent *data)
{
}

static void WriteAnimationComponentToFile(FILE *file, AnimationComponent *data)
{
    // 'parent_entity_id' in AnimationComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in AnimationComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'flags' in AnimationComponent
    WriteToFile(file, &data->flags, sizeof(data->flags));

    // 'current_frame' in AnimationComponent
    WriteToFile(file, &data->current_frame, sizeof(data->current_frame));

    // 'interval_mult' in AnimationComponent
    WriteToFile(file, &data->interval_mult, sizeof(data->interval_mult));

    // 'frame_start_time' in AnimationComponent
    WriteToFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

}

static void FillAnimationComponentPointersInFile(FILE *file, AnimationComponent *data)
{
}

static void WritePhysicsBodyComponentToFile(FILE *file, PhysicsBodyComponent *data)
{
    // 'parent_entity_id' in PhysicsBodyComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in PhysicsBodyComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'shape' in PhysicsBodyComponent
    WriteToFile(file, &data->shape, sizeof(data->shape));

    // 'shape_type' in PhysicsBodyComponent
    WriteToFile(file, &data->shape_type, sizeof(data->shape_type));

    // 'material' in PhysicsBodyComponent
    WriteToFile(file, &data->material, sizeof(data->material));

    // 'mass_data' in PhysicsBodyComponent
    WriteToFile(file, &data->mass_data, sizeof(data->mass_data));

    // 'velocity' in PhysicsBodyComponent
    WriteToFile(file, &data->velocity, sizeof(data->velocity));

    // 'force' in PhysicsBodyComponent
    WriteToFile(file, &data->force, sizeof(data->force));

    // 'gravity_multiplier' in PhysicsBodyComponent
    WriteToFile(file, &data->gravity_multiplier, sizeof(data->gravity_multiplier));

}

static void FillPhysicsBodyComponentPointersInFile(FILE *file, PhysicsBodyComponent *data)
{
}

static void WriteMovementComponentToFile(FILE *file, MovementComponent *data)
{
    // 'parent_entity_id' in MovementComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in MovementComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'axis_x' in MovementComponent
    WriteToFile(file, &data->axis_x, sizeof(data->axis_x));

    // 'move_speed' in MovementComponent
    WriteToFile(file, &data->move_speed, sizeof(data->move_speed));

    // 'move_speed_mult' in MovementComponent
    WriteToFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

}

static void FillMovementComponentPointersInFile(FILE *file, MovementComponent *data)
{
}

static void WriteArcEntityComponentToFile(FILE *file, ArcEntityComponent *data)
{
    // 'parent_entity_id' in ArcEntityComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in ArcEntityComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'entity_type' in ArcEntityComponent
    WriteToFile(file, &data->entity_type, sizeof(data->entity_type));

    // 'current_general_state' pointer in ArcEntityComponent
    {
        i32 pos = ftell(file);
        R_DEV_ASSERT(pos != -1, "Uh oh.");
        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, "Max pointers reached. Consider a better design?");
        SerialisationPointer ptr = {&data->current_general_state, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }

    // 'current_animation_state' in ArcEntityComponent
    WriteToFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

}

static void FillArcEntityComponentPointersInFile(FILE *file, ArcEntityComponent *data)
{
}

static void WriteItemComponentToFile(FILE *file, ItemComponent *data)
{
    // 'parent_entity_id' in ItemComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in ItemComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'item_type' in ItemComponent
    WriteToFile(file, &data->item_type, sizeof(data->item_type));

    // 'stack_size' in ItemComponent
    WriteToFile(file, &data->stack_size, sizeof(data->stack_size));

}

static void FillItemComponentPointersInFile(FILE *file, ItemComponent *data)
{
}

static void WriteTriggerComponentToFile(FILE *file, TriggerComponent *data)
{
    // 'parent_entity_id' in TriggerComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in TriggerComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'enter_trigger_callback' in TriggerComponent
    WriteToFile(file, &data->enter_trigger_callback, sizeof(data->enter_trigger_callback));

    // 'exit_trigger_callback' in TriggerComponent
    WriteToFile(file, &data->exit_trigger_callback, sizeof(data->exit_trigger_callback));

    for (i32 i = 0; i < MAX_OVERLAPPING_COLLIDERS; i++)
    {
        // 'previous_overlaps' array in TriggerComponent
        WriteToFile(file, &data->previous_overlaps[i], sizeof(OverlappedColliderInfo));
    }

    // 'previous_overlaps_count' in TriggerComponent
    WriteToFile(file, &data->previous_overlaps_count, sizeof(data->previous_overlaps_count));

    // 'trigger_against' in TriggerComponent
    WriteToFile(file, &data->trigger_against, sizeof(data->trigger_against));

}

static void FillTriggerComponentPointersInFile(FILE *file, TriggerComponent *data)
{
}

static void WriteStorageComponentToFile(FILE *file, StorageComponent *data)
{
    // 'parent_entity_id' in StorageComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in StorageComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'storage_size' in StorageComponent
    WriteToFile(file, &data->storage_size, sizeof(data->storage_size));

    for (i32 i = 0; i < MAX_STORAGE_SIZE; i++)
    {
        // 'items' pointer array in StorageComponent
        i32 pos = ftell(file);
        R_DEV_ASSERT(pos != -1, "Uh oh.");
        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, "Max pointers reached. Consider a better design?");
        SerialisationPointer ptr = {&(data->items[i]), pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }

}

static void FillStorageComponentPointersInFile(FILE *file, StorageComponent *data)
{
}

static void WriteParallaxComponentToFile(FILE *file, ParallaxComponent *data)
{
    // 'parent_entity_id' in ParallaxComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in ParallaxComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'parallax_amount' in ParallaxComponent
    WriteToFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    // 'desired_position' in ParallaxComponent
    WriteToFile(file, &data->desired_position, sizeof(data->desired_position));

}

static void FillParallaxComponentPointersInFile(FILE *file, ParallaxComponent *data)
{
}

static void WriteParticleEmitterComponentToFile(FILE *file, ParticleEmitterComponent *data)
{
    // 'parent_entity_id' in ParticleEmitterComponent
    WriteToFile(file, &data->parent_entity_id, sizeof(data->parent_entity_id));

    // 'component_id' in ParticleEmitterComponent
    WriteToFile(file, &data->component_id, sizeof(data->component_id));

    // 'life_time' in ParticleEmitterComponent
    WriteToFile(file, &data->life_time, sizeof(data->life_time));

    // 'start_time' in ParticleEmitterComponent
    WriteToFile(file, &data->start_time, sizeof(data->start_time));

    // 'flags' in ParticleEmitterComponent
    WriteToFile(file, &data->flags, sizeof(data->flags));

    for (i32 i = 0; i < MAX_PARTICLE_AMOUNT; i++)
    {
        // 'particles' array in ParticleEmitterComponent
        WriteToFile(file, &data->particles[i], sizeof(Particle));
    }

    // 'particle_count' in ParticleEmitterComponent
    WriteToFile(file, &data->particle_count, sizeof(data->particle_count));

    // 'free_particle_index' in ParticleEmitterComponent
    WriteToFile(file, &data->free_particle_index, sizeof(data->free_particle_index));

    // 'begin_callback' in ParticleEmitterComponent
    WriteToFile(file, &data->begin_callback, sizeof(data->begin_callback));

    // 'finish_callback' in ParticleEmitterComponent
    WriteToFile(file, &data->finish_callback, sizeof(data->finish_callback));

}

static void FillParticleEmitterComponentPointersInFile(FILE *file, ParticleEmitterComponent *data)
{
}

static void WriteComponentSetToFile(FILE *file, ComponentSet *data)
{
    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'position_components' array in ComponentSet
        WritePositionComponentToFile(file, &(data->position_components[i]));
    }

    // 'position_component_count' in ComponentSet
    WriteToFile(file, &data->position_component_count, sizeof(data->position_component_count));

    // 'free_position_component_id' in ComponentSet
    WriteToFile(file, &data->free_position_component_id, sizeof(data->free_position_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'sprite_components' array in ComponentSet
        WriteSpriteComponentToFile(file, &(data->sprite_components[i]));
    }

    // 'sprite_component_count' in ComponentSet
    WriteToFile(file, &data->sprite_component_count, sizeof(data->sprite_component_count));

    // 'free_sprite_component_id' in ComponentSet
    WriteToFile(file, &data->free_sprite_component_id, sizeof(data->free_sprite_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'animation_components' array in ComponentSet
        WriteAnimationComponentToFile(file, &(data->animation_components[i]));
    }

    // 'animation_component_count' in ComponentSet
    WriteToFile(file, &data->animation_component_count, sizeof(data->animation_component_count));

    // 'free_animation_component_id' in ComponentSet
    WriteToFile(file, &data->free_animation_component_id, sizeof(data->free_animation_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'physics_body_components' array in ComponentSet
        WritePhysicsBodyComponentToFile(file, &(data->physics_body_components[i]));
    }

    // 'physics_body_component_count' in ComponentSet
    WriteToFile(file, &data->physics_body_component_count, sizeof(data->physics_body_component_count));

    // 'free_physics_body_component_id' in ComponentSet
    WriteToFile(file, &data->free_physics_body_component_id, sizeof(data->free_physics_body_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'movement_components' array in ComponentSet
        WriteMovementComponentToFile(file, &(data->movement_components[i]));
    }

    // 'movement_component_count' in ComponentSet
    WriteToFile(file, &data->movement_component_count, sizeof(data->movement_component_count));

    // 'free_movement_component_id' in ComponentSet
    WriteToFile(file, &data->free_movement_component_id, sizeof(data->free_movement_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'arc_entity_components' array in ComponentSet
        WriteArcEntityComponentToFile(file, &(data->arc_entity_components[i]));
    }

    // 'arc_entity_component_count' in ComponentSet
    WriteToFile(file, &data->arc_entity_component_count, sizeof(data->arc_entity_component_count));

    // 'free_arc_entity_component_id' in ComponentSet
    WriteToFile(file, &data->free_arc_entity_component_id, sizeof(data->free_arc_entity_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'item_components' array in ComponentSet
        WriteItemComponentToFile(file, &(data->item_components[i]));
    }

    // 'item_component_count' in ComponentSet
    WriteToFile(file, &data->item_component_count, sizeof(data->item_component_count));

    // 'free_item_component_id' in ComponentSet
    WriteToFile(file, &data->free_item_component_id, sizeof(data->free_item_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'trigger_components' array in ComponentSet
        WriteTriggerComponentToFile(file, &(data->trigger_components[i]));
    }

    // 'trigger_component_count' in ComponentSet
    WriteToFile(file, &data->trigger_component_count, sizeof(data->trigger_component_count));

    // 'free_trigger_component_id' in ComponentSet
    WriteToFile(file, &data->free_trigger_component_id, sizeof(data->free_trigger_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'storage_components' array in ComponentSet
        WriteStorageComponentToFile(file, &(data->storage_components[i]));
    }

    // 'storage_component_count' in ComponentSet
    WriteToFile(file, &data->storage_component_count, sizeof(data->storage_component_count));

    // 'free_storage_component_id' in ComponentSet
    WriteToFile(file, &data->free_storage_component_id, sizeof(data->free_storage_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'parallax_components' array in ComponentSet
        WriteParallaxComponentToFile(file, &(data->parallax_components[i]));
    }

    // 'parallax_component_count' in ComponentSet
    WriteToFile(file, &data->parallax_component_count, sizeof(data->parallax_component_count));

    // 'free_parallax_component_id' in ComponentSet
    WriteToFile(file, &data->free_parallax_component_id, sizeof(data->free_parallax_component_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'particle_emitter_components' array in ComponentSet
        WriteParticleEmitterComponentToFile(file, &(data->particle_emitter_components[i]));
    }

    // 'particle_emitter_component_count' in ComponentSet
    WriteToFile(file, &data->particle_emitter_component_count, sizeof(data->particle_emitter_component_count));

    // 'free_particle_emitter_component_id' in ComponentSet
    WriteToFile(file, &data->free_particle_emitter_component_id, sizeof(data->free_particle_emitter_component_id));

}

static void FillComponentSetPointersInFile(FILE *file, ComponentSet *data)
{
}

static void WriteCharacterEntityToFile(FILE *file, CharacterEntity *data)
{
    // 'entity_id' in CharacterEntity
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));

}

static void FillCharacterEntityPointersInFile(FILE *file, CharacterEntity *data)
{
}

static void WriteCloudEntityToFile(FILE *file, CloudEntity *data)
{
    // 'entity_id' in CloudEntity
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));

    // 'unique_entity_id' in CloudEntity
    WriteToFile(file, &data->unique_entity_id, sizeof(data->unique_entity_id));

}

static void FillCloudEntityPointersInFile(FILE *file, CloudEntity *data)
{
}

static void WriteGroundSegmentEntityToFile(FILE *file, GroundSegmentEntity *data)
{
    // 'entity_id' in GroundSegmentEntity
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));

    // 'unique_entity_id' in GroundSegmentEntity
    WriteToFile(file, &data->unique_entity_id, sizeof(data->unique_entity_id));

}

static void FillGroundSegmentEntityPointersInFile(FILE *file, GroundSegmentEntity *data)
{
}

static void WriteEntityToFile(FILE *file, Entity *data)
{
    // 'entity_id' in Entity
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));

    for (i32 i = 0; i < COMPONENT_MAX; i++)
    {
        // 'component_ids' array in Entity
        WriteToFile(file, &data->component_ids[i], sizeof(i32));
    }

    for (i32 i = 0; i < 20; i++)
    {
        // 'name' array in Entity
        WriteToFile(file, &data->name[i], sizeof(char));
    }

    // 'unique_entity_id' in Entity
    WriteToFile(file, &data->unique_entity_id, sizeof(data->unique_entity_id));

    // 'type' in Entity
    WriteToFile(file, &data->type, sizeof(data->type));

    // 'flags' in Entity
    WriteToFile(file, &data->flags, sizeof(data->flags));

    // 'generalised_type' in Entity
    WriteToFile(file, &data->generalised_type, sizeof(data->generalised_type));

}

static void FillEntityPointersInFile(FILE *file, Entity *data)
{
}

static void WriteCellToFile(FILE *file, Cell *data)
{
    // 'dynamic_id' in Cell
    WriteToFile(file, &data->dynamic_id, sizeof(data->dynamic_id));

    // 'x_position' in Cell
    WriteToFile(file, &data->x_position, sizeof(data->x_position));

    // 'y_position' in Cell
    WriteToFile(file, &data->y_position, sizeof(data->y_position));

    // 'material_type' in Cell
    WriteToFile(file, &data->material_type, sizeof(data->material_type));

    // 'dynamic_properties' in Cell
    WriteToFile(file, &data->dynamic_properties, sizeof(data->dynamic_properties));

}

static void FillCellPointersInFile(FILE *file, Cell *data)
{
}

static void WriteChunkToFile(FILE *file, Chunk *data)
{
    // 'is_valid' in Chunk
    WriteToFile(file, &data->is_valid, sizeof(data->is_valid));

    // 'remain_loaded' in Chunk
    WriteToFile(file, &data->remain_loaded, sizeof(data->remain_loaded));

    for (i32 i = 0; i < MAX_ENTITIES_PER_CHUNK; i++)
    {
        // 'entity_ids' array in Chunk
        WriteToFile(file, &data->entity_ids[i], sizeof(i32));
    }

    // 'entity_count' in Chunk
    WriteToFile(file, &data->entity_count, sizeof(data->entity_count));

    // 'x_index' in Chunk
    WriteToFile(file, &data->x_index, sizeof(data->x_index));

    // 'y_index' in Chunk
    WriteToFile(file, &data->y_index, sizeof(data->y_index));

// - 2D Arary CHUNK_SIZE CHUNK_SIZE
    // 'texture' in Chunk
    WriteToFile(file, &data->texture, sizeof(data->texture));

}

static void FillChunkPointersInFile(FILE *file, Chunk *data)
{
}

static void WriteWorldDataToFile(FILE *file, WorldData *data)
{
    // 'elapsed_world_time' in WorldData
    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

    for (i32 i = 0; i < MAX_WORLD_CHUNKS; i++)
    {
        // 'active_chunks' array in WorldData
        WriteChunkToFile(file, &(data->active_chunks[i]));
    }

    // 'active_chunk_count' in WorldData
    WriteToFile(file, &data->active_chunk_count, sizeof(data->active_chunk_count));

    // 'character_entity' in WorldData
    WriteCharacterEntityToFile(file, &data->character_entity);

    for (i32 i = 0; i < MAX_CLOUD_ENTITY_COUNT; i++)
    {
        // 'cloud_entity_list' array in WorldData
        WriteCloudEntityToFile(file, &(data->cloud_entity_list[i]));
    }

    // 'cloud_entity_count' in WorldData
    WriteToFile(file, &data->cloud_entity_count, sizeof(data->cloud_entity_count));

    // 'free_cloud_entity_id' in WorldData
    WriteToFile(file, &data->free_cloud_entity_id, sizeof(data->free_cloud_entity_id));

    for (i32 i = 0; i < MAX_GROUND_SEGMENT_ENTITY_COUNT; i++)
    {
        // 'ground_segment_entity_list' array in WorldData
        WriteGroundSegmentEntityToFile(file, &(data->ground_segment_entity_list[i]));
    }

    // 'ground_segment_entity_count' in WorldData
    WriteToFile(file, &data->ground_segment_entity_count, sizeof(data->ground_segment_entity_count));

    // 'free_ground_segment_entity_id' in WorldData
    WriteToFile(file, &data->free_ground_segment_entity_id, sizeof(data->free_ground_segment_entity_id));

    for (i32 i = 0; i < MAX_ACTIVE_ENTITIES; i++)
    {
        // 'entities' array in WorldData
        WriteEntityToFile(file, &(data->entities[i]));
    }

    // 'entity_count' in WorldData
    WriteToFile(file, &data->entity_count, sizeof(data->entity_count));

    // 'free_entity_id' in WorldData
    WriteToFile(file, &data->free_entity_id, sizeof(data->free_entity_id));

    // 'entity_components' in WorldData
    WriteComponentSetToFile(file, &data->entity_components);

    // 'test_ptr' pointer in WorldData
    {
        i32 pos = ftell(file);
        R_DEV_ASSERT(pos != -1, "Uh oh.");
        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, "Max pointers reached. Consider a better design?");
        SerialisationPointer ptr = {&data->test_ptr, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }

    for (i32 i = 0; i < MAX_DYNAMIC_CELLS; i++)
    {
        // 'dynamic_cells' pointer array in WorldData
        i32 pos = ftell(file);
        R_DEV_ASSERT(pos != -1, "Uh oh.");
        R_DEV_ASSERT(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS, "Max pointers reached. Consider a better design?");
        SerialisationPointer ptr = {&(data->dynamic_cells[i]), pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }

    // 'dynamic_cell_count' in WorldData
    WriteToFile(file, &data->dynamic_cell_count, sizeof(data->dynamic_cell_count));

    // 'free_dynamic_cell_id' in WorldData
    WriteToFile(file, &data->free_dynamic_cell_id, sizeof(data->free_dynamic_cell_id));

}

static void FillWorldDataPointersInFile(FILE *file, WorldData *data)
{
}

