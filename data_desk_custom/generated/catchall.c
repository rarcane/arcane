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
    Assert(core->run_data->entity_components.free_position_component_id > 0);
    Assert(entity->component_ids[COMPONENT_position] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_position_component_id;

    PositionComponent *comp = &core->run_data->entity_components.position_components[new_comp_id - 1];
    *comp = GetDefaultPositionComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_position] = new_comp_id;

    if (core->run_data->entity_components.position_component_count == core->run_data->entity_components.free_position_component_id - 1)
    {
        core->run_data->entity_components.position_component_count++;
        core->run_data->entity_components.free_position_component_id++;
    }

    if (core->run_data->entity_components.position_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.position_component_count != core->run_data->entity_components.free_position_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.position_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.position_components[i].component_id)
                {
                    core->run_data->entity_components.free_position_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_position_component_id = 0;
    }

    return comp;
}

internal void RemovePositionComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_position] != 0);
    PositionComponent *comp = &core->run_data->entity_components.position_components[entity->component_ids[COMPONENT_position] - 1];

    if (comp->component_id < core->run_data->entity_components.free_position_component_id)
        core->run_data->entity_components.free_position_component_id = comp->component_id;

    PositionComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_position] = 0;
}

internal PositionComponent *GetPositionComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_position]);
    PositionComponent *comp = &core->run_data->entity_components.position_components[entity->component_ids[COMPONENT_position] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_position]);
    return comp;
}

internal SpriteComponent *AddSpriteComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_sprite_component_id > 0);
    Assert(entity->component_ids[COMPONENT_sprite] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_sprite_component_id;

    SpriteComponent *comp = &core->run_data->entity_components.sprite_components[new_comp_id - 1];
    *comp = GetDefaultSpriteComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_sprite] = new_comp_id;

    if (core->run_data->entity_components.sprite_component_count == core->run_data->entity_components.free_sprite_component_id - 1)
    {
        core->run_data->entity_components.sprite_component_count++;
        core->run_data->entity_components.free_sprite_component_id++;
    }

    if (core->run_data->entity_components.sprite_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.sprite_component_count != core->run_data->entity_components.free_sprite_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.sprite_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.sprite_components[i].component_id)
                {
                    core->run_data->entity_components.free_sprite_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_sprite_component_id = 0;
    }

    return comp;
}

internal void RemoveSpriteComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_sprite] != 0);
    SpriteComponent *comp = &core->run_data->entity_components.sprite_components[entity->component_ids[COMPONENT_sprite] - 1];

    if (comp->component_id < core->run_data->entity_components.free_sprite_component_id)
        core->run_data->entity_components.free_sprite_component_id = comp->component_id;

    SpriteComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_sprite] = 0;
}

internal SpriteComponent *GetSpriteComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_sprite]);
    SpriteComponent *comp = &core->run_data->entity_components.sprite_components[entity->component_ids[COMPONENT_sprite] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_sprite]);
    return comp;
}

internal AnimationComponent *AddAnimationComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_animation_component_id > 0);
    Assert(entity->component_ids[COMPONENT_animation] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_animation_component_id;

    AnimationComponent *comp = &core->run_data->entity_components.animation_components[new_comp_id - 1];
    *comp = GetDefaultAnimationComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_animation] = new_comp_id;

    if (core->run_data->entity_components.animation_component_count == core->run_data->entity_components.free_animation_component_id - 1)
    {
        core->run_data->entity_components.animation_component_count++;
        core->run_data->entity_components.free_animation_component_id++;
    }

    if (core->run_data->entity_components.animation_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.animation_component_count != core->run_data->entity_components.free_animation_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.animation_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.animation_components[i].component_id)
                {
                    core->run_data->entity_components.free_animation_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_animation_component_id = 0;
    }

    return comp;
}

internal void RemoveAnimationComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_animation] != 0);
    AnimationComponent *comp = &core->run_data->entity_components.animation_components[entity->component_ids[COMPONENT_animation] - 1];

    if (comp->component_id < core->run_data->entity_components.free_animation_component_id)
        core->run_data->entity_components.free_animation_component_id = comp->component_id;

    AnimationComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_animation] = 0;
}

internal AnimationComponent *GetAnimationComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_animation]);
    AnimationComponent *comp = &core->run_data->entity_components.animation_components[entity->component_ids[COMPONENT_animation] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_animation]);
    return comp;
}

internal PhysicsBodyComponent *AddPhysicsBodyComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_physics_body_component_id > 0);
    Assert(entity->component_ids[COMPONENT_physics_body] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_physics_body_component_id;

    PhysicsBodyComponent *comp = &core->run_data->entity_components.physics_body_components[new_comp_id - 1];
    *comp = GetDefaultPhysicsBodyComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_physics_body] = new_comp_id;

    if (core->run_data->entity_components.physics_body_component_count == core->run_data->entity_components.free_physics_body_component_id - 1)
    {
        core->run_data->entity_components.physics_body_component_count++;
        core->run_data->entity_components.free_physics_body_component_id++;
    }

    if (core->run_data->entity_components.physics_body_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.physics_body_component_count != core->run_data->entity_components.free_physics_body_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.physics_body_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.physics_body_components[i].component_id)
                {
                    core->run_data->entity_components.free_physics_body_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_physics_body_component_id = 0;
    }

    return comp;
}

internal void RemovePhysicsBodyComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_physics_body] != 0);
    PhysicsBodyComponent *comp = &core->run_data->entity_components.physics_body_components[entity->component_ids[COMPONENT_physics_body] - 1];

    if (comp->component_id < core->run_data->entity_components.free_physics_body_component_id)
        core->run_data->entity_components.free_physics_body_component_id = comp->component_id;

    PhysicsBodyComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_physics_body] = 0;
}

internal PhysicsBodyComponent *GetPhysicsBodyComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_physics_body]);
    PhysicsBodyComponent *comp = &core->run_data->entity_components.physics_body_components[entity->component_ids[COMPONENT_physics_body] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_physics_body]);
    return comp;
}

internal MovementComponent *AddMovementComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_movement_component_id > 0);
    Assert(entity->component_ids[COMPONENT_movement] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_movement_component_id;

    MovementComponent *comp = &core->run_data->entity_components.movement_components[new_comp_id - 1];
    *comp = GetDefaultMovementComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_movement] = new_comp_id;

    if (core->run_data->entity_components.movement_component_count == core->run_data->entity_components.free_movement_component_id - 1)
    {
        core->run_data->entity_components.movement_component_count++;
        core->run_data->entity_components.free_movement_component_id++;
    }

    if (core->run_data->entity_components.movement_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.movement_component_count != core->run_data->entity_components.free_movement_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.movement_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.movement_components[i].component_id)
                {
                    core->run_data->entity_components.free_movement_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_movement_component_id = 0;
    }

    return comp;
}

internal void RemoveMovementComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_movement] != 0);
    MovementComponent *comp = &core->run_data->entity_components.movement_components[entity->component_ids[COMPONENT_movement] - 1];

    if (comp->component_id < core->run_data->entity_components.free_movement_component_id)
        core->run_data->entity_components.free_movement_component_id = comp->component_id;

    MovementComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_movement] = 0;
}

internal MovementComponent *GetMovementComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_movement]);
    MovementComponent *comp = &core->run_data->entity_components.movement_components[entity->component_ids[COMPONENT_movement] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_movement]);
    return comp;
}

internal ArcEntityComponent *AddArcEntityComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_arc_entity_component_id > 0);
    Assert(entity->component_ids[COMPONENT_arc_entity] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_arc_entity_component_id;

    ArcEntityComponent *comp = &core->run_data->entity_components.arc_entity_components[new_comp_id - 1];
    *comp = GetDefaultArcEntityComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_arc_entity] = new_comp_id;

    if (core->run_data->entity_components.arc_entity_component_count == core->run_data->entity_components.free_arc_entity_component_id - 1)
    {
        core->run_data->entity_components.arc_entity_component_count++;
        core->run_data->entity_components.free_arc_entity_component_id++;
    }

    if (core->run_data->entity_components.arc_entity_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.arc_entity_component_count != core->run_data->entity_components.free_arc_entity_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.arc_entity_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.arc_entity_components[i].component_id)
                {
                    core->run_data->entity_components.free_arc_entity_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_arc_entity_component_id = 0;
    }

    return comp;
}

internal void RemoveArcEntityComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_arc_entity] != 0);
    ArcEntityComponent *comp = &core->run_data->entity_components.arc_entity_components[entity->component_ids[COMPONENT_arc_entity] - 1];

    if (comp->component_id < core->run_data->entity_components.free_arc_entity_component_id)
        core->run_data->entity_components.free_arc_entity_component_id = comp->component_id;

    ArcEntityComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_arc_entity] = 0;
}

internal ArcEntityComponent *GetArcEntityComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_arc_entity]);
    ArcEntityComponent *comp = &core->run_data->entity_components.arc_entity_components[entity->component_ids[COMPONENT_arc_entity] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_arc_entity]);
    return comp;
}

internal ItemComponent *AddItemComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_item_component_id > 0);
    Assert(entity->component_ids[COMPONENT_item] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_item_component_id;

    ItemComponent *comp = &core->run_data->entity_components.item_components[new_comp_id - 1];
    *comp = GetDefaultItemComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_item] = new_comp_id;

    if (core->run_data->entity_components.item_component_count == core->run_data->entity_components.free_item_component_id - 1)
    {
        core->run_data->entity_components.item_component_count++;
        core->run_data->entity_components.free_item_component_id++;
    }

    if (core->run_data->entity_components.item_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.item_component_count != core->run_data->entity_components.free_item_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.item_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.item_components[i].component_id)
                {
                    core->run_data->entity_components.free_item_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_item_component_id = 0;
    }

    return comp;
}

internal void RemoveItemComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_item] != 0);
    ItemComponent *comp = &core->run_data->entity_components.item_components[entity->component_ids[COMPONENT_item] - 1];

    if (comp->component_id < core->run_data->entity_components.free_item_component_id)
        core->run_data->entity_components.free_item_component_id = comp->component_id;

    ItemComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_item] = 0;
}

internal ItemComponent *GetItemComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_item]);
    ItemComponent *comp = &core->run_data->entity_components.item_components[entity->component_ids[COMPONENT_item] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_item]);
    return comp;
}

internal TriggerComponent *AddTriggerComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_trigger_component_id > 0);
    Assert(entity->component_ids[COMPONENT_trigger] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_trigger_component_id;

    TriggerComponent *comp = &core->run_data->entity_components.trigger_components[new_comp_id - 1];
    *comp = GetDefaultTriggerComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_trigger] = new_comp_id;

    if (core->run_data->entity_components.trigger_component_count == core->run_data->entity_components.free_trigger_component_id - 1)
    {
        core->run_data->entity_components.trigger_component_count++;
        core->run_data->entity_components.free_trigger_component_id++;
    }

    if (core->run_data->entity_components.trigger_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.trigger_component_count != core->run_data->entity_components.free_trigger_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.trigger_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.trigger_components[i].component_id)
                {
                    core->run_data->entity_components.free_trigger_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_trigger_component_id = 0;
    }

    return comp;
}

internal void RemoveTriggerComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_trigger] != 0);
    TriggerComponent *comp = &core->run_data->entity_components.trigger_components[entity->component_ids[COMPONENT_trigger] - 1];

    if (comp->component_id < core->run_data->entity_components.free_trigger_component_id)
        core->run_data->entity_components.free_trigger_component_id = comp->component_id;

    TriggerComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_trigger] = 0;
}

internal TriggerComponent *GetTriggerComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_trigger]);
    TriggerComponent *comp = &core->run_data->entity_components.trigger_components[entity->component_ids[COMPONENT_trigger] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_trigger]);
    return comp;
}

internal StorageComponent *AddStorageComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_storage_component_id > 0);
    Assert(entity->component_ids[COMPONENT_storage] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_storage_component_id;

    StorageComponent *comp = &core->run_data->entity_components.storage_components[new_comp_id - 1];
    *comp = GetDefaultStorageComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_storage] = new_comp_id;

    if (core->run_data->entity_components.storage_component_count == core->run_data->entity_components.free_storage_component_id - 1)
    {
        core->run_data->entity_components.storage_component_count++;
        core->run_data->entity_components.free_storage_component_id++;
    }

    if (core->run_data->entity_components.storage_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.storage_component_count != core->run_data->entity_components.free_storage_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.storage_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.storage_components[i].component_id)
                {
                    core->run_data->entity_components.free_storage_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_storage_component_id = 0;
    }

    return comp;
}

internal void RemoveStorageComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_storage] != 0);
    StorageComponent *comp = &core->run_data->entity_components.storage_components[entity->component_ids[COMPONENT_storage] - 1];

    if (comp->component_id < core->run_data->entity_components.free_storage_component_id)
        core->run_data->entity_components.free_storage_component_id = comp->component_id;

    StorageComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_storage] = 0;
}

internal StorageComponent *GetStorageComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_storage]);
    StorageComponent *comp = &core->run_data->entity_components.storage_components[entity->component_ids[COMPONENT_storage] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_storage]);
    return comp;
}

internal ParallaxComponent *AddParallaxComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_parallax_component_id > 0);
    Assert(entity->component_ids[COMPONENT_parallax] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_parallax_component_id;

    ParallaxComponent *comp = &core->run_data->entity_components.parallax_components[new_comp_id - 1];
    *comp = GetDefaultParallaxComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_parallax] = new_comp_id;

    if (core->run_data->entity_components.parallax_component_count == core->run_data->entity_components.free_parallax_component_id - 1)
    {
        core->run_data->entity_components.parallax_component_count++;
        core->run_data->entity_components.free_parallax_component_id++;
    }

    if (core->run_data->entity_components.parallax_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.parallax_component_count != core->run_data->entity_components.free_parallax_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.parallax_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.parallax_components[i].component_id)
                {
                    core->run_data->entity_components.free_parallax_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_parallax_component_id = 0;
    }

    return comp;
}

internal void RemoveParallaxComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_parallax] != 0);
    ParallaxComponent *comp = &core->run_data->entity_components.parallax_components[entity->component_ids[COMPONENT_parallax] - 1];

    if (comp->component_id < core->run_data->entity_components.free_parallax_component_id)
        core->run_data->entity_components.free_parallax_component_id = comp->component_id;

    ParallaxComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_parallax] = 0;
}

internal ParallaxComponent *GetParallaxComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_parallax]);
    ParallaxComponent *comp = &core->run_data->entity_components.parallax_components[entity->component_ids[COMPONENT_parallax] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_parallax]);
    return comp;
}

internal ParticleEmitterComponent *AddParticleEmitterComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_particle_emitter_component_id > 0);
    Assert(entity->component_ids[COMPONENT_particle_emitter] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_particle_emitter_component_id;

    ParticleEmitterComponent *comp = &core->run_data->entity_components.particle_emitter_components[new_comp_id - 1];
    *comp = GetDefaultParticleEmitterComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_particle_emitter] = new_comp_id;

    if (core->run_data->entity_components.particle_emitter_component_count == core->run_data->entity_components.free_particle_emitter_component_id - 1)
    {
        core->run_data->entity_components.particle_emitter_component_count++;
        core->run_data->entity_components.free_particle_emitter_component_id++;
    }

    if (core->run_data->entity_components.particle_emitter_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.particle_emitter_component_count != core->run_data->entity_components.free_particle_emitter_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.particle_emitter_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.particle_emitter_components[i].component_id)
                {
                    core->run_data->entity_components.free_particle_emitter_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_particle_emitter_component_id = 0;
    }

    return comp;
}

internal void RemoveParticleEmitterComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_particle_emitter] != 0);
    ParticleEmitterComponent *comp = &core->run_data->entity_components.particle_emitter_components[entity->component_ids[COMPONENT_particle_emitter] - 1];

    if (comp->component_id < core->run_data->entity_components.free_particle_emitter_component_id)
        core->run_data->entity_components.free_particle_emitter_component_id = comp->component_id;

    ParticleEmitterComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_particle_emitter] = 0;
}

internal ParticleEmitterComponent *GetParticleEmitterComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_particle_emitter]);
    ParticleEmitterComponent *comp = &core->run_data->entity_components.particle_emitter_components[entity->component_ids[COMPONENT_particle_emitter] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_particle_emitter]);
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
            Assert(0);
        break;
    }
    case COMPONENT_sprite:
    {
        if (entity->component_ids[COMPONENT_sprite])
            RemoveSpriteComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_animation:
    {
        if (entity->component_ids[COMPONENT_animation])
            RemoveAnimationComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_physics_body:
    {
        if (entity->component_ids[COMPONENT_physics_body])
            RemovePhysicsBodyComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_movement:
    {
        if (entity->component_ids[COMPONENT_movement])
            RemoveMovementComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_arc_entity:
    {
        if (entity->component_ids[COMPONENT_arc_entity])
            RemoveArcEntityComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_item:
    {
        if (entity->component_ids[COMPONENT_item])
            RemoveItemComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_trigger:
    {
        if (entity->component_ids[COMPONENT_trigger])
            RemoveTriggerComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_storage:
    {
        if (entity->component_ids[COMPONENT_storage])
            RemoveStorageComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_parallax:
    {
        if (entity->component_ids[COMPONENT_parallax])
            RemoveParallaxComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_particle_emitter:
    {
        if (entity->component_ids[COMPONENT_particle_emitter])
            RemoveParticleEmitterComponent(entity);
        else
            Assert(0);
        break;
    }
    default:
        Assert(0);
        break;
    }
}
internal void InitialiseComponents()
{
    core->run_data->entity_components.free_position_component_id = 1;
    core->run_data->entity_components.free_sprite_component_id = 1;
    core->run_data->entity_components.free_animation_component_id = 1;
    core->run_data->entity_components.free_physics_body_component_id = 1;
    core->run_data->entity_components.free_movement_component_id = 1;
    core->run_data->entity_components.free_arc_entity_component_id = 1;
    core->run_data->entity_components.free_item_component_id = 1;
    core->run_data->entity_components.free_trigger_component_id = 1;
    core->run_data->entity_components.free_storage_component_id = 1;
    core->run_data->entity_components.free_parallax_component_id = 1;
    core->run_data->entity_components.free_particle_emitter_component_id = 1;
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

WriteComponentToFile(FILE *file, i32 comp_id, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            WritePositionComponentToFile(file, &core->run_data->entity_components.position_components[comp_id - 1]);
        } break;
        case COMPONENT_sprite:
        {
            WriteSpriteComponentToFile(file, &core->run_data->entity_components.sprite_components[comp_id - 1]);
        } break;
        case COMPONENT_animation:
        {
            WriteAnimationComponentToFile(file, &core->run_data->entity_components.animation_components[comp_id - 1]);
        } break;
        case COMPONENT_physics_body:
        {
            WritePhysicsBodyComponentToFile(file, &core->run_data->entity_components.physics_body_components[comp_id - 1]);
        } break;
        case COMPONENT_movement:
        {
            WriteMovementComponentToFile(file, &core->run_data->entity_components.movement_components[comp_id - 1]);
        } break;
        case COMPONENT_arc_entity:
        {
            WriteArcEntityComponentToFile(file, &core->run_data->entity_components.arc_entity_components[comp_id - 1]);
        } break;
        case COMPONENT_item:
        {
            WriteItemComponentToFile(file, &core->run_data->entity_components.item_components[comp_id - 1]);
        } break;
        case COMPONENT_trigger:
        {
            WriteTriggerComponentToFile(file, &core->run_data->entity_components.trigger_components[comp_id - 1]);
        } break;
        case COMPONENT_storage:
        {
            WriteStorageComponentToFile(file, &core->run_data->entity_components.storage_components[comp_id - 1]);
        } break;
        case COMPONENT_parallax:
        {
            WriteParallaxComponentToFile(file, &core->run_data->entity_components.parallax_components[comp_id - 1]);
        } break;
        case COMPONENT_particle_emitter:
        {
            WriteParticleEmitterComponentToFile(file, &core->run_data->entity_components.particle_emitter_components[comp_id - 1]);
        } break;
    }
}
ReadComponentFromFile(FILE *file, Entity *entity, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            PositionComponent component;
            ReadPositionComponentFromFile(file, &component);
            PositionComponent *new_comp = AddPositionComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_sprite:
        {
            SpriteComponent component;
            ReadSpriteComponentFromFile(file, &component);
            SpriteComponent *new_comp = AddSpriteComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_animation:
        {
            AnimationComponent component;
            ReadAnimationComponentFromFile(file, &component);
            AnimationComponent *new_comp = AddAnimationComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_physics_body:
        {
            PhysicsBodyComponent component;
            ReadPhysicsBodyComponentFromFile(file, &component);
            PhysicsBodyComponent *new_comp = AddPhysicsBodyComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_movement:
        {
            MovementComponent component;
            ReadMovementComponentFromFile(file, &component);
            MovementComponent *new_comp = AddMovementComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_arc_entity:
        {
            ArcEntityComponent component;
            ReadArcEntityComponentFromFile(file, &component);
            ArcEntityComponent *new_comp = AddArcEntityComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_item:
        {
            ItemComponent component;
            ReadItemComponentFromFile(file, &component);
            ItemComponent *new_comp = AddItemComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_trigger:
        {
            TriggerComponent component;
            ReadTriggerComponentFromFile(file, &component);
            TriggerComponent *new_comp = AddTriggerComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_storage:
        {
            StorageComponent component;
            ReadStorageComponentFromFile(file, &component);
            StorageComponent *new_comp = AddStorageComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_parallax:
        {
            ParallaxComponent component;
            ReadParallaxComponentFromFile(file, &component);
            ParallaxComponent *new_comp = AddParallaxComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_particle_emitter:
        {
            ParticleEmitterComponent component;
            ReadParticleEmitterComponentFromFile(file, &component);
            ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
    }
}
SerialiseEntityComponentsFromIDList(FILE *file, Entity *entity_list, ComponentSet *component_set, i32 *ids, i32 id_count, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                PositionComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->position_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WritePositionComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_sprite:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                SpriteComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->sprite_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteSpriteComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_animation:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                AnimationComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->animation_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteAnimationComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_physics_body:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                PhysicsBodyComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->physics_body_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WritePhysicsBodyComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_movement:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                MovementComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->movement_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteMovementComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_arc_entity:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                ArcEntityComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->arc_entity_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteArcEntityComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_item:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                ItemComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->item_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteItemComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_trigger:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                TriggerComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->trigger_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteTriggerComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_storage:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                StorageComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->storage_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteStorageComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_parallax:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                ParallaxComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->parallax_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteParallaxComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_particle_emitter:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                ParticleEmitterComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->particle_emitter_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteParticleEmitterComponentToFile(file, comps[i].comp_data);
            }
        } break;
    }
}

DeserialiseEntityComponentsFromIDList(FILE *file, i32 *ids, i32 id_count, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                PositionComponent component;
                ReadPositionComponentFromFile(file, &component);
                PositionComponent *new_comp = AddPositionComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_sprite:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                SpriteComponent component;
                ReadSpriteComponentFromFile(file, &component);
                SpriteComponent *new_comp = AddSpriteComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_animation:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                AnimationComponent component;
                ReadAnimationComponentFromFile(file, &component);
                AnimationComponent *new_comp = AddAnimationComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_physics_body:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                PhysicsBodyComponent component;
                ReadPhysicsBodyComponentFromFile(file, &component);
                PhysicsBodyComponent *new_comp = AddPhysicsBodyComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_movement:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                MovementComponent component;
                ReadMovementComponentFromFile(file, &component);
                MovementComponent *new_comp = AddMovementComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_arc_entity:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ArcEntityComponent component;
                ReadArcEntityComponentFromFile(file, &component);
                ArcEntityComponent *new_comp = AddArcEntityComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_item:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ItemComponent component;
                ReadItemComponentFromFile(file, &component);
                ItemComponent *new_comp = AddItemComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_trigger:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                TriggerComponent component;
                ReadTriggerComponentFromFile(file, &component);
                TriggerComponent *new_comp = AddTriggerComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_storage:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                StorageComponent component;
                ReadStorageComponentFromFile(file, &component);
                StorageComponent *new_comp = AddStorageComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_parallax:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ParallaxComponent component;
                ReadParallaxComponentFromFile(file, &component);
                ParallaxComponent *new_comp = AddParallaxComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_particle_emitter:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ParticleEmitterComponent component;
                ReadParticleEmitterComponentFromFile(file, &component);
                ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
    }
}

DeserialiseComponentsToLoadData(FILE *file, ComponentSet *component_set, EntitySave *entity_list, i32 *ids, i32 id_count, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                PositionComponent component;
                ReadPositionComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->position_components[component_set->position_component_count++] = component;
            }
        } break;
        case COMPONENT_sprite:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                SpriteComponent component;
                ReadSpriteComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->sprite_components[component_set->sprite_component_count++] = component;
            }
        } break;
        case COMPONENT_animation:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                AnimationComponent component;
                ReadAnimationComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->animation_components[component_set->animation_component_count++] = component;
            }
        } break;
        case COMPONENT_physics_body:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                PhysicsBodyComponent component;
                ReadPhysicsBodyComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->physics_body_components[component_set->physics_body_component_count++] = component;
            }
        } break;
        case COMPONENT_movement:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                MovementComponent component;
                ReadMovementComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->movement_components[component_set->movement_component_count++] = component;
            }
        } break;
        case COMPONENT_arc_entity:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                ArcEntityComponent component;
                ReadArcEntityComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->arc_entity_components[component_set->arc_entity_component_count++] = component;
            }
        } break;
        case COMPONENT_item:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                ItemComponent component;
                ReadItemComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->item_components[component_set->item_component_count++] = component;
            }
        } break;
        case COMPONENT_trigger:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                TriggerComponent component;
                ReadTriggerComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->trigger_components[component_set->trigger_component_count++] = component;
            }
        } break;
        case COMPONENT_storage:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                StorageComponent component;
                ReadStorageComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->storage_components[component_set->storage_component_count++] = component;
            }
        } break;
        case COMPONENT_parallax:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                ParallaxComponent component;
                ReadParallaxComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->parallax_components[component_set->parallax_component_count++] = component;
            }
        } break;
        case COMPONENT_particle_emitter:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                EntitySave *entity = &entity_list[ids[entity_offset] - 1];
                ParticleEmitterComponent component;
                ReadParticleEmitterComponentFromFile(file, &component);
                component.parent_entity_id = ids[entity_offset];
                component_set->particle_emitter_components[component_set->particle_emitter_component_count++] = component;
            }
        } break;
    }
}

DeserialiseComponentsFromMap(i32 *entity_id_map, i32 entity_count)
{
    for (i32 i = 0; i < core->run_data->loaded_entity_components.position_component_count; i++)
    {
        PositionComponent *saved_comp = &core->run_data->loaded_entity_components.position_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        PositionComponent *new_comp = AddPositionComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.sprite_component_count; i++)
    {
        SpriteComponent *saved_comp = &core->run_data->loaded_entity_components.sprite_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        SpriteComponent *new_comp = AddSpriteComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.animation_component_count; i++)
    {
        AnimationComponent *saved_comp = &core->run_data->loaded_entity_components.animation_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        AnimationComponent *new_comp = AddAnimationComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.physics_body_component_count; i++)
    {
        PhysicsBodyComponent *saved_comp = &core->run_data->loaded_entity_components.physics_body_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        PhysicsBodyComponent *new_comp = AddPhysicsBodyComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.movement_component_count; i++)
    {
        MovementComponent *saved_comp = &core->run_data->loaded_entity_components.movement_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        MovementComponent *new_comp = AddMovementComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.arc_entity_component_count; i++)
    {
        ArcEntityComponent *saved_comp = &core->run_data->loaded_entity_components.arc_entity_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        ArcEntityComponent *new_comp = AddArcEntityComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.item_component_count; i++)
    {
        ItemComponent *saved_comp = &core->run_data->loaded_entity_components.item_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        ItemComponent *new_comp = AddItemComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.trigger_component_count; i++)
    {
        TriggerComponent *saved_comp = &core->run_data->loaded_entity_components.trigger_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        TriggerComponent *new_comp = AddTriggerComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.storage_component_count; i++)
    {
        StorageComponent *saved_comp = &core->run_data->loaded_entity_components.storage_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        StorageComponent *new_comp = AddStorageComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.parallax_component_count; i++)
    {
        ParallaxComponent *saved_comp = &core->run_data->loaded_entity_components.parallax_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        ParallaxComponent *new_comp = AddParallaxComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.particle_emitter_component_count; i++)
    {
        ParticleEmitterComponent *saved_comp = &core->run_data->loaded_entity_components.particle_emitter_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
}

static void WritePositionComponentToFile(FILE *file, PositionComponent *data)
{
    WriteToFile(file, &data->position, sizeof(data->position));

}

static void FillPositionComponentPointersInFile(FILE *file, PositionComponent *data)
{
    // 'position' in PositionComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->position)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->position), SEEK_CUR);

}

static void ReadPositionComponentFromFile(FILE *file, PositionComponent *data)
{
    // 'position' in PositionComponent
    ReadFromFile(file, &data->position, sizeof(data->position));

}

static void FillPositionComponentPointersFromFile(FILE *file, PositionComponent *data)
{
    // 'position' in PositionComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->position;
        }
    }
    fseek(file, sizeof(data->position), SEEK_CUR);

}

static void WriteSpriteComponentToFile(FILE *file, SpriteComponent *data)
{
    WriteToFile(file, &data->sprite_data, sizeof(data->sprite_data));

    WriteToFile(file, &data->is_flipped, sizeof(data->is_flipped));

    WriteToFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

}

static void FillSpriteComponentPointersInFile(FILE *file, SpriteComponent *data)
{
    // 'sprite_data' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->sprite_data)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->sprite_data), SEEK_CUR);

    // 'is_flipped' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_flipped)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_flipped), SEEK_CUR);

    // 'is_background_sprite' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_background_sprite)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_background_sprite), SEEK_CUR);

}

static void ReadSpriteComponentFromFile(FILE *file, SpriteComponent *data)
{
    // 'sprite_data' in SpriteComponent
    ReadFromFile(file, &data->sprite_data, sizeof(data->sprite_data));

    // 'is_flipped' in SpriteComponent
    ReadFromFile(file, &data->is_flipped, sizeof(data->is_flipped));

    // 'is_background_sprite' in SpriteComponent
    ReadFromFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

}

static void FillSpriteComponentPointersFromFile(FILE *file, SpriteComponent *data)
{
    // 'sprite_data' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->sprite_data;
        }
    }
    fseek(file, sizeof(data->sprite_data), SEEK_CUR);

    // 'is_flipped' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->is_flipped;
        }
    }
    fseek(file, sizeof(data->is_flipped), SEEK_CUR);

    // 'is_background_sprite' in SpriteComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->is_background_sprite;
        }
    }
    fseek(file, sizeof(data->is_background_sprite), SEEK_CUR);

}

static void WriteAnimationComponentToFile(FILE *file, AnimationComponent *data)
{
    WriteToFile(file, &data->flags, sizeof(data->flags));

    WriteToFile(file, &data->current_frame, sizeof(data->current_frame));

    WriteToFile(file, &data->interval_mult, sizeof(data->interval_mult));

    WriteToFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

}

static void FillAnimationComponentPointersInFile(FILE *file, AnimationComponent *data)
{
    // 'flags' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    // 'current_frame' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->current_frame)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->current_frame), SEEK_CUR);

    // 'interval_mult' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->interval_mult)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->interval_mult), SEEK_CUR);

    // 'frame_start_time' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->frame_start_time)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->frame_start_time), SEEK_CUR);

}

static void ReadAnimationComponentFromFile(FILE *file, AnimationComponent *data)
{
    // 'flags' in AnimationComponent
    ReadFromFile(file, &data->flags, sizeof(data->flags));

    // 'current_frame' in AnimationComponent
    ReadFromFile(file, &data->current_frame, sizeof(data->current_frame));

    // 'interval_mult' in AnimationComponent
    ReadFromFile(file, &data->interval_mult, sizeof(data->interval_mult));

    // 'frame_start_time' in AnimationComponent
    ReadFromFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

}

static void FillAnimationComponentPointersFromFile(FILE *file, AnimationComponent *data)
{
    // 'flags' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->flags;
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    // 'current_frame' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->current_frame;
        }
    }
    fseek(file, sizeof(data->current_frame), SEEK_CUR);

    // 'interval_mult' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->interval_mult;
        }
    }
    fseek(file, sizeof(data->interval_mult), SEEK_CUR);

    // 'frame_start_time' in AnimationComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->frame_start_time;
        }
    }
    fseek(file, sizeof(data->frame_start_time), SEEK_CUR);

}

static void WritePhysicsBodyComponentToFile(FILE *file, PhysicsBodyComponent *data)
{
    WriteToFile(file, &data->shape, sizeof(data->shape));

    WriteToFile(file, &data->shape_type, sizeof(data->shape_type));

    WriteToFile(file, &data->material, sizeof(data->material));

    WriteToFile(file, &data->mass_data, sizeof(data->mass_data));

    WriteToFile(file, &data->velocity, sizeof(data->velocity));

    WriteToFile(file, &data->force, sizeof(data->force));

    WriteToFile(file, &data->gravity_multiplier, sizeof(data->gravity_multiplier));

}

static void FillPhysicsBodyComponentPointersInFile(FILE *file, PhysicsBodyComponent *data)
{
    // 'shape' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->shape)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->shape), SEEK_CUR);

    // 'shape_type' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->shape_type)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->shape_type), SEEK_CUR);

    // 'material' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->material)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->material), SEEK_CUR);

    // 'mass_data' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->mass_data)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->mass_data), SEEK_CUR);

    // 'velocity' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->velocity)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->velocity), SEEK_CUR);

    // 'force' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->force)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->force), SEEK_CUR);

    // 'gravity_multiplier' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->gravity_multiplier)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->gravity_multiplier), SEEK_CUR);

}

static void ReadPhysicsBodyComponentFromFile(FILE *file, PhysicsBodyComponent *data)
{
    // 'shape' in PhysicsBodyComponent
    ReadFromFile(file, &data->shape, sizeof(data->shape));

    // 'shape_type' in PhysicsBodyComponent
    ReadFromFile(file, &data->shape_type, sizeof(data->shape_type));

    // 'material' in PhysicsBodyComponent
    ReadFromFile(file, &data->material, sizeof(data->material));

    // 'mass_data' in PhysicsBodyComponent
    ReadFromFile(file, &data->mass_data, sizeof(data->mass_data));

    // 'velocity' in PhysicsBodyComponent
    ReadFromFile(file, &data->velocity, sizeof(data->velocity));

    // 'force' in PhysicsBodyComponent
    ReadFromFile(file, &data->force, sizeof(data->force));

    // 'gravity_multiplier' in PhysicsBodyComponent
    ReadFromFile(file, &data->gravity_multiplier, sizeof(data->gravity_multiplier));

}

static void FillPhysicsBodyComponentPointersFromFile(FILE *file, PhysicsBodyComponent *data)
{
    // 'shape' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->shape;
        }
    }
    fseek(file, sizeof(data->shape), SEEK_CUR);

    // 'shape_type' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->shape_type;
        }
    }
    fseek(file, sizeof(data->shape_type), SEEK_CUR);

    // 'material' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->material;
        }
    }
    fseek(file, sizeof(data->material), SEEK_CUR);

    // 'mass_data' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->mass_data;
        }
    }
    fseek(file, sizeof(data->mass_data), SEEK_CUR);

    // 'velocity' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->velocity;
        }
    }
    fseek(file, sizeof(data->velocity), SEEK_CUR);

    // 'force' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->force;
        }
    }
    fseek(file, sizeof(data->force), SEEK_CUR);

    // 'gravity_multiplier' in PhysicsBodyComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->gravity_multiplier;
        }
    }
    fseek(file, sizeof(data->gravity_multiplier), SEEK_CUR);

}

static void WriteMovementComponentToFile(FILE *file, MovementComponent *data)
{
    WriteToFile(file, &data->axis_x, sizeof(data->axis_x));

    WriteToFile(file, &data->move_speed, sizeof(data->move_speed));

    WriteToFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

}

static void FillMovementComponentPointersInFile(FILE *file, MovementComponent *data)
{
    // 'axis_x' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->axis_x)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->axis_x), SEEK_CUR);

    // 'move_speed' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->move_speed)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->move_speed), SEEK_CUR);

    // 'move_speed_mult' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->move_speed_mult)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->move_speed_mult), SEEK_CUR);

}

static void ReadMovementComponentFromFile(FILE *file, MovementComponent *data)
{
    // 'axis_x' in MovementComponent
    ReadFromFile(file, &data->axis_x, sizeof(data->axis_x));

    // 'move_speed' in MovementComponent
    ReadFromFile(file, &data->move_speed, sizeof(data->move_speed));

    // 'move_speed_mult' in MovementComponent
    ReadFromFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

}

static void FillMovementComponentPointersFromFile(FILE *file, MovementComponent *data)
{
    // 'axis_x' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->axis_x;
        }
    }
    fseek(file, sizeof(data->axis_x), SEEK_CUR);

    // 'move_speed' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->move_speed;
        }
    }
    fseek(file, sizeof(data->move_speed), SEEK_CUR);

    // 'move_speed_mult' in MovementComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->move_speed_mult;
        }
    }
    fseek(file, sizeof(data->move_speed_mult), SEEK_CUR);

}

static void WriteArcEntityComponentToFile(FILE *file, ArcEntityComponent *data)
{
    WriteToFile(file, &data->entity_type, sizeof(data->entity_type));

    if (data->current_general_state)
    {
        i32 pos = ftell(file);
        Assert(pos != -1);
        Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);
        SerialisationPointer ptr = {&data->current_general_state, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }
    else
    {
        i32 null_ptr = 0;
        WriteToFile(file, &null_ptr, sizeof(i32));
    }
    WriteToFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

}

static void FillArcEntityComponentPointersInFile(FILE *file, ArcEntityComponent *data)
{
    // 'entity_type' in ArcEntityComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_type)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_type), SEEK_CUR);

    // 'current_general_state' pointer in ArcEntityComponent
    fseek(file, sizeof(i32), SEEK_CUR);

    // 'current_animation_state' in ArcEntityComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->current_animation_state)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->current_animation_state), SEEK_CUR);

}

static void ReadArcEntityComponentFromFile(FILE *file, ArcEntityComponent *data)
{
    // 'entity_type' in ArcEntityComponent
    ReadFromFile(file, &data->entity_type, sizeof(data->entity_type));

    // 'current_general_state' pointer in ArcEntityComponent
    {
        i32 pointer_offset;
        ReadFromFile(file, &pointer_offset, sizeof(i32));
        if (pointer_offset)
        {
            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);
            SerialisationPointer ptr = {&data->current_general_state, pointer_offset};
            serialisation_pointers[serialisation_pointer_count++] = ptr;
        }
        else
            data->current_general_state = 0;
    }
    // 'current_animation_state' in ArcEntityComponent
    ReadFromFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

}

static void FillArcEntityComponentPointersFromFile(FILE *file, ArcEntityComponent *data)
{
    // 'entity_type' in ArcEntityComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->entity_type;
        }
    }
    fseek(file, sizeof(data->entity_type), SEEK_CUR);

    // 'current_general_state' pointer in ArcEntityComponent
    fseek(file, sizeof(i32), SEEK_CUR);

    // 'current_animation_state' in ArcEntityComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->current_animation_state;
        }
    }
    fseek(file, sizeof(data->current_animation_state), SEEK_CUR);

}

static void WriteItemComponentToFile(FILE *file, ItemComponent *data)
{
    WriteToFile(file, &data->item_type, sizeof(data->item_type));

    WriteToFile(file, &data->stack_size, sizeof(data->stack_size));

}

static void FillItemComponentPointersInFile(FILE *file, ItemComponent *data)
{
    // 'item_type' in ItemComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->item_type)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->item_type), SEEK_CUR);

    // 'stack_size' in ItemComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->stack_size)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->stack_size), SEEK_CUR);

}

static void ReadItemComponentFromFile(FILE *file, ItemComponent *data)
{
    // 'item_type' in ItemComponent
    ReadFromFile(file, &data->item_type, sizeof(data->item_type));

    // 'stack_size' in ItemComponent
    ReadFromFile(file, &data->stack_size, sizeof(data->stack_size));

}

static void FillItemComponentPointersFromFile(FILE *file, ItemComponent *data)
{
    // 'item_type' in ItemComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->item_type;
        }
    }
    fseek(file, sizeof(data->item_type), SEEK_CUR);

    // 'stack_size' in ItemComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->stack_size;
        }
    }
    fseek(file, sizeof(data->stack_size), SEEK_CUR);

}

static void WriteTriggerComponentToFile(FILE *file, TriggerComponent *data)
{
    WriteToFile(file, &data->enter_trigger_callback, sizeof(data->enter_trigger_callback));

    WriteToFile(file, &data->exit_trigger_callback, sizeof(data->exit_trigger_callback));

    for (i32 i = 0; i < MAX_OVERLAPPING_COLLIDERS; i++)
    {
        WriteToFile(file, &data->previous_overlaps[i], sizeof(OverlappedColliderInfo));
    }

    WriteToFile(file, &data->previous_overlaps_count, sizeof(data->previous_overlaps_count));

    WriteToFile(file, &data->trigger_against, sizeof(data->trigger_against));

}

static void FillTriggerComponentPointersInFile(FILE *file, TriggerComponent *data)
{
    // 'enter_trigger_callback' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->enter_trigger_callback)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->enter_trigger_callback), SEEK_CUR);

    // 'exit_trigger_callback' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->exit_trigger_callback)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->exit_trigger_callback), SEEK_CUR);

    for (i32 i = 0; i < MAX_OVERLAPPING_COLLIDERS; i++)
    {
        // 'previous_overlaps' array in TriggerComponent
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->previous_overlaps[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        fseek(file, sizeof(OverlappedColliderInfo), SEEK_CUR);
    }

    // 'previous_overlaps_count' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->previous_overlaps_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->previous_overlaps_count), SEEK_CUR);

    // 'trigger_against' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->trigger_against)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->trigger_against), SEEK_CUR);

}

static void ReadTriggerComponentFromFile(FILE *file, TriggerComponent *data)
{
    // 'enter_trigger_callback' in TriggerComponent
    ReadFromFile(file, &data->enter_trigger_callback, sizeof(data->enter_trigger_callback));

    // 'exit_trigger_callback' in TriggerComponent
    ReadFromFile(file, &data->exit_trigger_callback, sizeof(data->exit_trigger_callback));

    // 'previous_overlaps_count' in TriggerComponent
    ReadFromFile(file, &data->previous_overlaps_count, sizeof(data->previous_overlaps_count));

    // 'trigger_against' in TriggerComponent
    ReadFromFile(file, &data->trigger_against, sizeof(data->trigger_against));

}

static void FillTriggerComponentPointersFromFile(FILE *file, TriggerComponent *data)
{
    // 'enter_trigger_callback' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->enter_trigger_callback;
        }
    }
    fseek(file, sizeof(data->enter_trigger_callback), SEEK_CUR);

    // 'exit_trigger_callback' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->exit_trigger_callback;
        }
    }
    fseek(file, sizeof(data->exit_trigger_callback), SEEK_CUR);

    // 'previous_overlaps_count' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->previous_overlaps_count;
        }
    }
    fseek(file, sizeof(data->previous_overlaps_count), SEEK_CUR);

    // 'trigger_against' in TriggerComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->trigger_against;
        }
    }
    fseek(file, sizeof(data->trigger_against), SEEK_CUR);

}

static void WriteStorageComponentToFile(FILE *file, StorageComponent *data)
{
    WriteToFile(file, &data->storage_size, sizeof(data->storage_size));

    for (i32 i = 0; i < MAX_STORAGE_SIZE; i++)
    {
        if (data->items[i])
        {
            i32 pos = ftell(file);
            Assert(pos != -1);
            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);
            SerialisationPointer ptr = {&(data->items[i]), pos};
            serialisation_pointers[serialisation_pointer_count++] = ptr;
            i32 empty = INT_MAX;
            WriteToFile(file, &empty, sizeof(i32));
        }
        else
        {
            i32 null_ptr = 0;
            WriteToFile(file, &null_ptr, sizeof(i32));
        }

    }

}

static void FillStorageComponentPointersInFile(FILE *file, StorageComponent *data)
{
    // 'storage_size' in StorageComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->storage_size)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->storage_size), SEEK_CUR);

    for (i32 i = 0; i < MAX_STORAGE_SIZE; i++)
    {
    // 'items' pointer array in StorageComponent
    fseek(file, sizeof(i32), SEEK_CUR);
    }

}

static void ReadStorageComponentFromFile(FILE *file, StorageComponent *data)
{
    // 'storage_size' in StorageComponent
    ReadFromFile(file, &data->storage_size, sizeof(data->storage_size));

}

static void FillStorageComponentPointersFromFile(FILE *file, StorageComponent *data)
{
    // 'storage_size' in StorageComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->storage_size;
        }
    }
    fseek(file, sizeof(data->storage_size), SEEK_CUR);

}

static void WriteParallaxComponentToFile(FILE *file, ParallaxComponent *data)
{
    WriteToFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    WriteToFile(file, &data->desired_position, sizeof(data->desired_position));

}

static void FillParallaxComponentPointersInFile(FILE *file, ParallaxComponent *data)
{
    // 'parallax_amount' in ParallaxComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->parallax_amount)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->parallax_amount), SEEK_CUR);

    // 'desired_position' in ParallaxComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->desired_position)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->desired_position), SEEK_CUR);

}

static void ReadParallaxComponentFromFile(FILE *file, ParallaxComponent *data)
{
    // 'parallax_amount' in ParallaxComponent
    ReadFromFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    // 'desired_position' in ParallaxComponent
    ReadFromFile(file, &data->desired_position, sizeof(data->desired_position));

}

static void FillParallaxComponentPointersFromFile(FILE *file, ParallaxComponent *data)
{
    // 'parallax_amount' in ParallaxComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->parallax_amount;
        }
    }
    fseek(file, sizeof(data->parallax_amount), SEEK_CUR);

    // 'desired_position' in ParallaxComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->desired_position;
        }
    }
    fseek(file, sizeof(data->desired_position), SEEK_CUR);

}

static void WriteParticleEmitterComponentToFile(FILE *file, ParticleEmitterComponent *data)
{
    WriteToFile(file, &data->life_time, sizeof(data->life_time));

    WriteToFile(file, &data->start_time, sizeof(data->start_time));

    WriteToFile(file, &data->flags, sizeof(data->flags));

    for (i32 i = 0; i < MAX_PARTICLE_AMOUNT; i++)
    {
        WriteToFile(file, &data->particles[i], sizeof(Particle));
    }

    WriteToFile(file, &data->particle_count, sizeof(data->particle_count));

    WriteToFile(file, &data->free_particle_index, sizeof(data->free_particle_index));

    WriteToFile(file, &data->begin_callback, sizeof(data->begin_callback));

    WriteToFile(file, &data->finish_callback, sizeof(data->finish_callback));

}

static void FillParticleEmitterComponentPointersInFile(FILE *file, ParticleEmitterComponent *data)
{
    // 'life_time' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->life_time)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->life_time), SEEK_CUR);

    // 'start_time' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->start_time)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->start_time), SEEK_CUR);

    // 'flags' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    for (i32 i = 0; i < MAX_PARTICLE_AMOUNT; i++)
    {
        // 'particles' array in ParticleEmitterComponent
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->particles[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        fseek(file, sizeof(Particle), SEEK_CUR);
    }

    // 'particle_count' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->particle_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->particle_count), SEEK_CUR);

    // 'free_particle_index' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_particle_index)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_particle_index), SEEK_CUR);

    // 'begin_callback' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->begin_callback)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->begin_callback), SEEK_CUR);

    // 'finish_callback' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->finish_callback)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->finish_callback), SEEK_CUR);

}

static void ReadParticleEmitterComponentFromFile(FILE *file, ParticleEmitterComponent *data)
{
    // 'life_time' in ParticleEmitterComponent
    ReadFromFile(file, &data->life_time, sizeof(data->life_time));

    // 'start_time' in ParticleEmitterComponent
    ReadFromFile(file, &data->start_time, sizeof(data->start_time));

    // 'flags' in ParticleEmitterComponent
    ReadFromFile(file, &data->flags, sizeof(data->flags));

    // 'particle_count' in ParticleEmitterComponent
    ReadFromFile(file, &data->particle_count, sizeof(data->particle_count));

    // 'free_particle_index' in ParticleEmitterComponent
    ReadFromFile(file, &data->free_particle_index, sizeof(data->free_particle_index));

    // 'begin_callback' in ParticleEmitterComponent
    ReadFromFile(file, &data->begin_callback, sizeof(data->begin_callback));

    // 'finish_callback' in ParticleEmitterComponent
    ReadFromFile(file, &data->finish_callback, sizeof(data->finish_callback));

}

static void FillParticleEmitterComponentPointersFromFile(FILE *file, ParticleEmitterComponent *data)
{
    // 'life_time' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->life_time;
        }
    }
    fseek(file, sizeof(data->life_time), SEEK_CUR);

    // 'start_time' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->start_time;
        }
    }
    fseek(file, sizeof(data->start_time), SEEK_CUR);

    // 'flags' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->flags;
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    // 'particle_count' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->particle_count;
        }
    }
    fseek(file, sizeof(data->particle_count), SEEK_CUR);

    // 'free_particle_index' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_particle_index;
        }
    }
    fseek(file, sizeof(data->free_particle_index), SEEK_CUR);

    // 'begin_callback' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->begin_callback;
        }
    }
    fseek(file, sizeof(data->begin_callback), SEEK_CUR);

    // 'finish_callback' in ParticleEmitterComponent
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->finish_callback;
        }
    }
    fseek(file, sizeof(data->finish_callback), SEEK_CUR);

}

static void WriteComponentSetToFile(FILE *file, ComponentSet *data)
{
    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WritePositionComponentToFile(file, &(data->position_components[i]));
    }

    WriteToFile(file, &data->position_component_count, sizeof(data->position_component_count));

    WriteToFile(file, &data->free_position_component_id, sizeof(data->free_position_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteSpriteComponentToFile(file, &(data->sprite_components[i]));
    }

    WriteToFile(file, &data->sprite_component_count, sizeof(data->sprite_component_count));

    WriteToFile(file, &data->free_sprite_component_id, sizeof(data->free_sprite_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteAnimationComponentToFile(file, &(data->animation_components[i]));
    }

    WriteToFile(file, &data->animation_component_count, sizeof(data->animation_component_count));

    WriteToFile(file, &data->free_animation_component_id, sizeof(data->free_animation_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WritePhysicsBodyComponentToFile(file, &(data->physics_body_components[i]));
    }

    WriteToFile(file, &data->physics_body_component_count, sizeof(data->physics_body_component_count));

    WriteToFile(file, &data->free_physics_body_component_id, sizeof(data->free_physics_body_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteMovementComponentToFile(file, &(data->movement_components[i]));
    }

    WriteToFile(file, &data->movement_component_count, sizeof(data->movement_component_count));

    WriteToFile(file, &data->free_movement_component_id, sizeof(data->free_movement_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteArcEntityComponentToFile(file, &(data->arc_entity_components[i]));
    }

    WriteToFile(file, &data->arc_entity_component_count, sizeof(data->arc_entity_component_count));

    WriteToFile(file, &data->free_arc_entity_component_id, sizeof(data->free_arc_entity_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteItemComponentToFile(file, &(data->item_components[i]));
    }

    WriteToFile(file, &data->item_component_count, sizeof(data->item_component_count));

    WriteToFile(file, &data->free_item_component_id, sizeof(data->free_item_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteTriggerComponentToFile(file, &(data->trigger_components[i]));
    }

    WriteToFile(file, &data->trigger_component_count, sizeof(data->trigger_component_count));

    WriteToFile(file, &data->free_trigger_component_id, sizeof(data->free_trigger_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteStorageComponentToFile(file, &(data->storage_components[i]));
    }

    WriteToFile(file, &data->storage_component_count, sizeof(data->storage_component_count));

    WriteToFile(file, &data->free_storage_component_id, sizeof(data->free_storage_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteParallaxComponentToFile(file, &(data->parallax_components[i]));
    }

    WriteToFile(file, &data->parallax_component_count, sizeof(data->parallax_component_count));

    WriteToFile(file, &data->free_parallax_component_id, sizeof(data->free_parallax_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteParticleEmitterComponentToFile(file, &(data->particle_emitter_components[i]));
    }

    WriteToFile(file, &data->particle_emitter_component_count, sizeof(data->particle_emitter_component_count));

    WriteToFile(file, &data->free_particle_emitter_component_id, sizeof(data->free_particle_emitter_component_id));

}

static void FillComponentSetPointersInFile(FILE *file, ComponentSet *data)
{
    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'position_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->position_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillPositionComponentPointersInFile(file, &(data->position_components[i]));
    }

    // 'position_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->position_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->position_component_count), SEEK_CUR);

    // 'free_position_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_position_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_position_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'sprite_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->sprite_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillSpriteComponentPointersInFile(file, &(data->sprite_components[i]));
    }

    // 'sprite_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->sprite_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->sprite_component_count), SEEK_CUR);

    // 'free_sprite_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_sprite_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_sprite_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'animation_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->animation_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillAnimationComponentPointersInFile(file, &(data->animation_components[i]));
    }

    // 'animation_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->animation_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->animation_component_count), SEEK_CUR);

    // 'free_animation_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_animation_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_animation_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'physics_body_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->physics_body_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillPhysicsBodyComponentPointersInFile(file, &(data->physics_body_components[i]));
    }

    // 'physics_body_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->physics_body_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->physics_body_component_count), SEEK_CUR);

    // 'free_physics_body_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_physics_body_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_physics_body_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'movement_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->movement_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillMovementComponentPointersInFile(file, &(data->movement_components[i]));
    }

    // 'movement_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->movement_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->movement_component_count), SEEK_CUR);

    // 'free_movement_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_movement_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_movement_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'arc_entity_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->arc_entity_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillArcEntityComponentPointersInFile(file, &(data->arc_entity_components[i]));
    }

    // 'arc_entity_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->arc_entity_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->arc_entity_component_count), SEEK_CUR);

    // 'free_arc_entity_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_arc_entity_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_arc_entity_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'item_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->item_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillItemComponentPointersInFile(file, &(data->item_components[i]));
    }

    // 'item_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->item_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->item_component_count), SEEK_CUR);

    // 'free_item_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_item_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_item_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'trigger_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->trigger_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillTriggerComponentPointersInFile(file, &(data->trigger_components[i]));
    }

    // 'trigger_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->trigger_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->trigger_component_count), SEEK_CUR);

    // 'free_trigger_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_trigger_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_trigger_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'storage_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->storage_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillStorageComponentPointersInFile(file, &(data->storage_components[i]));
    }

    // 'storage_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->storage_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->storage_component_count), SEEK_CUR);

    // 'free_storage_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_storage_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_storage_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'parallax_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->parallax_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillParallaxComponentPointersInFile(file, &(data->parallax_components[i]));
    }

    // 'parallax_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->parallax_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->parallax_component_count), SEEK_CUR);

    // 'free_parallax_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_parallax_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_parallax_component_id), SEEK_CUR);

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        // 'particle_emitter_components' array in ComponentSet
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->particle_emitter_components[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        FillParticleEmitterComponentPointersInFile(file, &(data->particle_emitter_components[i]));
    }

    // 'particle_emitter_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->particle_emitter_component_count)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->particle_emitter_component_count), SEEK_CUR);

    // 'free_particle_emitter_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_particle_emitter_component_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_particle_emitter_component_id), SEEK_CUR);

}

static void ReadComponentSetFromFile(FILE *file, ComponentSet *data)
{
    // 'position_component_count' in ComponentSet
    ReadFromFile(file, &data->position_component_count, sizeof(data->position_component_count));

    // 'free_position_component_id' in ComponentSet
    ReadFromFile(file, &data->free_position_component_id, sizeof(data->free_position_component_id));

    // 'sprite_component_count' in ComponentSet
    ReadFromFile(file, &data->sprite_component_count, sizeof(data->sprite_component_count));

    // 'free_sprite_component_id' in ComponentSet
    ReadFromFile(file, &data->free_sprite_component_id, sizeof(data->free_sprite_component_id));

    // 'animation_component_count' in ComponentSet
    ReadFromFile(file, &data->animation_component_count, sizeof(data->animation_component_count));

    // 'free_animation_component_id' in ComponentSet
    ReadFromFile(file, &data->free_animation_component_id, sizeof(data->free_animation_component_id));

    // 'physics_body_component_count' in ComponentSet
    ReadFromFile(file, &data->physics_body_component_count, sizeof(data->physics_body_component_count));

    // 'free_physics_body_component_id' in ComponentSet
    ReadFromFile(file, &data->free_physics_body_component_id, sizeof(data->free_physics_body_component_id));

    // 'movement_component_count' in ComponentSet
    ReadFromFile(file, &data->movement_component_count, sizeof(data->movement_component_count));

    // 'free_movement_component_id' in ComponentSet
    ReadFromFile(file, &data->free_movement_component_id, sizeof(data->free_movement_component_id));

    // 'arc_entity_component_count' in ComponentSet
    ReadFromFile(file, &data->arc_entity_component_count, sizeof(data->arc_entity_component_count));

    // 'free_arc_entity_component_id' in ComponentSet
    ReadFromFile(file, &data->free_arc_entity_component_id, sizeof(data->free_arc_entity_component_id));

    // 'item_component_count' in ComponentSet
    ReadFromFile(file, &data->item_component_count, sizeof(data->item_component_count));

    // 'free_item_component_id' in ComponentSet
    ReadFromFile(file, &data->free_item_component_id, sizeof(data->free_item_component_id));

    // 'trigger_component_count' in ComponentSet
    ReadFromFile(file, &data->trigger_component_count, sizeof(data->trigger_component_count));

    // 'free_trigger_component_id' in ComponentSet
    ReadFromFile(file, &data->free_trigger_component_id, sizeof(data->free_trigger_component_id));

    // 'storage_component_count' in ComponentSet
    ReadFromFile(file, &data->storage_component_count, sizeof(data->storage_component_count));

    // 'free_storage_component_id' in ComponentSet
    ReadFromFile(file, &data->free_storage_component_id, sizeof(data->free_storage_component_id));

    // 'parallax_component_count' in ComponentSet
    ReadFromFile(file, &data->parallax_component_count, sizeof(data->parallax_component_count));

    // 'free_parallax_component_id' in ComponentSet
    ReadFromFile(file, &data->free_parallax_component_id, sizeof(data->free_parallax_component_id));

    // 'particle_emitter_component_count' in ComponentSet
    ReadFromFile(file, &data->particle_emitter_component_count, sizeof(data->particle_emitter_component_count));

    // 'free_particle_emitter_component_id' in ComponentSet
    ReadFromFile(file, &data->free_particle_emitter_component_id, sizeof(data->free_particle_emitter_component_id));

}

static void FillComponentSetPointersFromFile(FILE *file, ComponentSet *data)
{
    // 'position_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->position_component_count;
        }
    }
    fseek(file, sizeof(data->position_component_count), SEEK_CUR);

    // 'free_position_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_position_component_id;
        }
    }
    fseek(file, sizeof(data->free_position_component_id), SEEK_CUR);

    // 'sprite_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->sprite_component_count;
        }
    }
    fseek(file, sizeof(data->sprite_component_count), SEEK_CUR);

    // 'free_sprite_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_sprite_component_id;
        }
    }
    fseek(file, sizeof(data->free_sprite_component_id), SEEK_CUR);

    // 'animation_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->animation_component_count;
        }
    }
    fseek(file, sizeof(data->animation_component_count), SEEK_CUR);

    // 'free_animation_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_animation_component_id;
        }
    }
    fseek(file, sizeof(data->free_animation_component_id), SEEK_CUR);

    // 'physics_body_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->physics_body_component_count;
        }
    }
    fseek(file, sizeof(data->physics_body_component_count), SEEK_CUR);

    // 'free_physics_body_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_physics_body_component_id;
        }
    }
    fseek(file, sizeof(data->free_physics_body_component_id), SEEK_CUR);

    // 'movement_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->movement_component_count;
        }
    }
    fseek(file, sizeof(data->movement_component_count), SEEK_CUR);

    // 'free_movement_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_movement_component_id;
        }
    }
    fseek(file, sizeof(data->free_movement_component_id), SEEK_CUR);

    // 'arc_entity_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->arc_entity_component_count;
        }
    }
    fseek(file, sizeof(data->arc_entity_component_count), SEEK_CUR);

    // 'free_arc_entity_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_arc_entity_component_id;
        }
    }
    fseek(file, sizeof(data->free_arc_entity_component_id), SEEK_CUR);

    // 'item_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->item_component_count;
        }
    }
    fseek(file, sizeof(data->item_component_count), SEEK_CUR);

    // 'free_item_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_item_component_id;
        }
    }
    fseek(file, sizeof(data->free_item_component_id), SEEK_CUR);

    // 'trigger_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->trigger_component_count;
        }
    }
    fseek(file, sizeof(data->trigger_component_count), SEEK_CUR);

    // 'free_trigger_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_trigger_component_id;
        }
    }
    fseek(file, sizeof(data->free_trigger_component_id), SEEK_CUR);

    // 'storage_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->storage_component_count;
        }
    }
    fseek(file, sizeof(data->storage_component_count), SEEK_CUR);

    // 'free_storage_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_storage_component_id;
        }
    }
    fseek(file, sizeof(data->free_storage_component_id), SEEK_CUR);

    // 'parallax_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->parallax_component_count;
        }
    }
    fseek(file, sizeof(data->parallax_component_count), SEEK_CUR);

    // 'free_parallax_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_parallax_component_id;
        }
    }
    fseek(file, sizeof(data->free_parallax_component_id), SEEK_CUR);

    // 'particle_emitter_component_count' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->particle_emitter_component_count;
        }
    }
    fseek(file, sizeof(data->particle_emitter_component_count), SEEK_CUR);

    // 'free_particle_emitter_component_id' in ComponentSet
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->free_particle_emitter_component_id;
        }
    }
    fseek(file, sizeof(data->free_particle_emitter_component_id), SEEK_CUR);

}

static void WriteEntityToFile(FILE *file, Entity *data)
{
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));

    for (i32 i = 0; i < COMPONENT_MAX; i++)
    {
        WriteToFile(file, &data->component_ids[i], sizeof(i32));
    }

    for (i32 i = 0; i < 20; i++)
    {
        WriteToFile(file, &data->name[i], sizeof(char));
    }

    WriteToFile(file, &data->flags, sizeof(data->flags));

    WriteToFile(file, &data->generalised_type, sizeof(data->generalised_type));

}

static void FillEntityPointersInFile(FILE *file, Entity *data)
{
    // 'entity_id' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_id)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_id), SEEK_CUR);

    for (i32 i = 0; i < COMPONENT_MAX; i++)
    {
        // 'component_ids' array in Entity
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->component_ids[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        fseek(file, sizeof(i32), SEEK_CUR);
    }

    for (i32 i = 0; i < 20; i++)
    {
        // 'name' array in Entity
        for (i32 j = 0; j < serialisation_pointer_count; j++)
        {
            SerialisationPointer *ptr = &serialisation_pointers[j];
            if (*ptr->pointer_address == &(data->name[i]))
            {
                i32 current_pos = ftell(file);
                Assert(current_pos != -1);
                fseek(file, ptr->offset, SEEK_SET);
                WriteToFile(file, &current_pos, sizeof(i32));
                fseek(file, current_pos, SEEK_SET);
            }
        }
        fseek(file, sizeof(char), SEEK_CUR);
    }

    // 'flags' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    // 'generalised_type' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->generalised_type)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->generalised_type), SEEK_CUR);

}

static void ReadEntityFromFile(FILE *file, Entity *data)
{
    // 'entity_id' in Entity
    ReadFromFile(file, &data->entity_id, sizeof(data->entity_id));

    // 'flags' in Entity
    ReadFromFile(file, &data->flags, sizeof(data->flags));

    // 'generalised_type' in Entity
    ReadFromFile(file, &data->generalised_type, sizeof(data->generalised_type));

}

static void FillEntityPointersFromFile(FILE *file, Entity *data)
{
    // 'entity_id' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->entity_id;
        }
    }
    fseek(file, sizeof(data->entity_id), SEEK_CUR);

    // 'flags' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->flags;
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);

    // 'generalised_type' in Entity
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->generalised_type;
        }
    }
    fseek(file, sizeof(data->generalised_type), SEEK_CUR);

}

static void WriteWorldSaveDataToFile(FILE *file, WorldSaveData *data)
{
    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

    if (data->test_ptr)
    {
        i32 pos = ftell(file);
        Assert(pos != -1);
        Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);
        SerialisationPointer ptr = {&data->test_ptr, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = INT_MAX;
        WriteToFile(file, &empty, sizeof(i32));
    }
    else
    {
        i32 null_ptr = 0;
        WriteToFile(file, &null_ptr, sizeof(i32));
    }
}

static void FillWorldSaveDataPointersInFile(FILE *file, WorldSaveData *data)
{
    // 'elapsed_world_time' in WorldSaveData
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->elapsed_world_time)
        {
            i32 current_pos = ftell(file);
            Assert(current_pos != -1);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->elapsed_world_time), SEEK_CUR);

    // 'test_ptr' pointer in WorldSaveData
    fseek(file, sizeof(i32), SEEK_CUR);

}

static void ReadWorldSaveDataFromFile(FILE *file, WorldSaveData *data)
{
    // 'elapsed_world_time' in WorldSaveData
    ReadFromFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

    // 'test_ptr' pointer in WorldSaveData
    {
        i32 pointer_offset;
        ReadFromFile(file, &pointer_offset, sizeof(i32));
        if (pointer_offset)
        {
            Assert(serialisation_pointer_count + 1 < MAX_SERIALISATION_POINTERS);
            SerialisationPointer ptr = {&data->test_ptr, pointer_offset};
            serialisation_pointers[serialisation_pointer_count++] = ptr;
        }
        else
            data->test_ptr = 0;
    }
}

static void FillWorldSaveDataPointersFromFile(FILE *file, WorldSaveData *data)
{
    // 'elapsed_world_time' in WorldSaveData
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        i32 current_pos = ftell(file);
        Assert(current_pos != -1);
        if (ptr->offset == current_pos)
        {
            *ptr->pointer_address = &data->elapsed_world_time;
        }
    }
    fseek(file, sizeof(data->elapsed_world_time), SEEK_CUR);

    // 'test_ptr' pointer in WorldSaveData
    fseek(file, sizeof(i32), SEEK_CUR);

}

