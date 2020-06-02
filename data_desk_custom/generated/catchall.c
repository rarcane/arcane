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
case EDITOR_STATE_chunk:
return "Chunk";
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
case STATIC_SPRITE_hills_1_v2:
return "Hills 1v2";
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
case STATIC_SPRITE_crafting_stump:
return "Crafting Stump";
break;
case STATIC_SPRITE_flint_sword_icon:
return "Flint Sword Icon";
break;
case STATIC_SPRITE_flint_sword_ground:
return "Flint Sword Ground";
break;
case STATIC_SPRITE_flint:
return "Flint";
break;
case STATIC_SPRITE_twig:
return "Twig";
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
case ITEM_TYPE_none:
return "None";
break;
case ITEM_TYPE_flint_sword:
return "Flint Sword";
break;
case ITEM_TYPE_flint:
return "Flint";
break;
case ITEM_TYPE_twig:
return "Twig";
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

static char *GetRecipeTypeName(RecipeType type)
{
switch(type)
{
case RECIPE_TYPE_flint_sword:
return "Flint Sword";
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

internal PlayerDataComponent *AddPlayerDataComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_player_data_component_id > 0);
    Assert(entity->component_ids[COMPONENT_player_data] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_player_data_component_id;

    PlayerDataComponent *comp = &core->run_data->entity_components.player_data_components[new_comp_id - 1];
    *comp = GetDefaultPlayerDataComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_player_data] = new_comp_id;

    if (core->run_data->entity_components.player_data_component_count == core->run_data->entity_components.free_player_data_component_id - 1)
    {
        core->run_data->entity_components.player_data_component_count++;
        core->run_data->entity_components.free_player_data_component_id++;
    }

    if (core->run_data->entity_components.player_data_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.player_data_component_count != core->run_data->entity_components.free_player_data_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.player_data_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.player_data_components[i].component_id)
                {
                    core->run_data->entity_components.free_player_data_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_player_data_component_id = 0;
    }

    return comp;
}

internal void RemovePlayerDataComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_player_data] != 0);
    PlayerDataComponent *comp = &core->run_data->entity_components.player_data_components[entity->component_ids[COMPONENT_player_data] - 1];

    if (comp->component_id < core->run_data->entity_components.free_player_data_component_id)
        core->run_data->entity_components.free_player_data_component_id = comp->component_id;

    PlayerDataComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_player_data] = 0;
}

internal PlayerDataComponent *GetPlayerDataComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_player_data]);
    PlayerDataComponent *comp = &core->run_data->entity_components.player_data_components[entity->component_ids[COMPONENT_player_data] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_player_data]);
    return comp;
}

internal InteractableComponent *AddInteractableComponent(Entity *entity)
{
    Assert(core->run_data->entity_components.free_interactable_component_id > 0);
    Assert(entity->component_ids[COMPONENT_interactable] == 0);
    i32 new_comp_id = core->run_data->entity_components.free_interactable_component_id;

    InteractableComponent *comp = &core->run_data->entity_components.interactable_components[new_comp_id - 1];
    *comp = GetDefaultInteractableComponent();
    comp->parent_entity_id = entity->entity_id;
    comp->component_id = new_comp_id;
    entity->component_ids[COMPONENT_interactable] = new_comp_id;

    if (core->run_data->entity_components.interactable_component_count == core->run_data->entity_components.free_interactable_component_id - 1)
    {
        core->run_data->entity_components.interactable_component_count++;
        core->run_data->entity_components.free_interactable_component_id++;
    }

    if (core->run_data->entity_components.interactable_component_count < MAX_ENTITIES)
    {
        if (core->run_data->entity_components.interactable_component_count != core->run_data->entity_components.free_interactable_component_id - 1)
        {
            b8 found = 0;
            for (i32 i = 0; i < core->run_data->entity_components.interactable_component_count + 1; i++)
            {
                if (!core->run_data->entity_components.interactable_components[i].component_id)
                {
                    core->run_data->entity_components.free_interactable_component_id = i + 1;
                    found = 1;
                    break;
                }
            }
            Assert(found);
        }
    }
    else
    {
        core->run_data->entity_components.free_interactable_component_id = 0;
    }

    return comp;
}

internal void RemoveInteractableComponent(Entity *entity)
{
    Assert(entity->component_ids[COMPONENT_interactable] != 0);
    InteractableComponent *comp = &core->run_data->entity_components.interactable_components[entity->component_ids[COMPONENT_interactable] - 1];

    if (comp->component_id < core->run_data->entity_components.free_interactable_component_id)
        core->run_data->entity_components.free_interactable_component_id = comp->component_id;

    InteractableComponent empty_comp = {0};
    *comp = empty_comp;
    entity->component_ids[COMPONENT_interactable] = 0;
}

internal InteractableComponent *GetInteractableComponentFromEntityID(i32 id)
{
    Entity *entity = GetEntityWithID(id);
    Assert(entity->component_ids[COMPONENT_interactable]);
    InteractableComponent *comp = &core->run_data->entity_components.interactable_components[entity->component_ids[COMPONENT_interactable] - 1];
    Assert(comp->parent_entity_id == entity->entity_id && comp->component_id == entity->component_ids[COMPONENT_interactable]);
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
    case COMPONENT_player_data:
    {
        if (entity->component_ids[COMPONENT_player_data])
            RemovePlayerDataComponent(entity);
        else
            Assert(0);
        break;
    }
    case COMPONENT_interactable:
    {
        if (entity->component_ids[COMPONENT_interactable])
            RemoveInteractableComponent(entity);
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
    core->run_data->entity_components.free_parallax_component_id = 1;
    core->run_data->entity_components.free_particle_emitter_component_id = 1;
    core->run_data->entity_components.free_player_data_component_id = 1;
    core->run_data->entity_components.free_interactable_component_id = 1;
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
        case COMPONENT_parallax:
        {
            WriteParallaxComponentToFile(file, &core->run_data->entity_components.parallax_components[comp_id - 1]);
        } break;
        case COMPONENT_particle_emitter:
        {
            WriteParticleEmitterComponentToFile(file, &core->run_data->entity_components.particle_emitter_components[comp_id - 1]);
        } break;
        case COMPONENT_player_data:
        {
            WritePlayerDataComponentToFile(file, &core->run_data->entity_components.player_data_components[comp_id - 1]);
        } break;
        case COMPONENT_interactable:
        {
            WriteInteractableComponentToFile(file, &core->run_data->entity_components.interactable_components[comp_id - 1]);
        } break;
    }
}
ReadComponentFromFile(FILE *file, Entity *entity, ComponentType type)
{
    switch (type)
    {
        case COMPONENT_position:
        {
            PositionComponent component = {0};
            ReadPositionComponentFromFile(file, &component);
            PositionComponent *new_comp = AddPositionComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_sprite:
        {
            SpriteComponent component = {0};
            ReadSpriteComponentFromFile(file, &component);
            SpriteComponent *new_comp = AddSpriteComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_animation:
        {
            AnimationComponent component = {0};
            ReadAnimationComponentFromFile(file, &component);
            AnimationComponent *new_comp = AddAnimationComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_physics_body:
        {
            PhysicsBodyComponent component = {0};
            ReadPhysicsBodyComponentFromFile(file, &component);
            PhysicsBodyComponent *new_comp = AddPhysicsBodyComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_movement:
        {
            MovementComponent component = {0};
            ReadMovementComponentFromFile(file, &component);
            MovementComponent *new_comp = AddMovementComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_arc_entity:
        {
            ArcEntityComponent component = {0};
            ReadArcEntityComponentFromFile(file, &component);
            ArcEntityComponent *new_comp = AddArcEntityComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_item:
        {
            ItemComponent component = {0};
            ReadItemComponentFromFile(file, &component);
            ItemComponent *new_comp = AddItemComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_trigger:
        {
            TriggerComponent component = {0};
            ReadTriggerComponentFromFile(file, &component);
            TriggerComponent *new_comp = AddTriggerComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_parallax:
        {
            ParallaxComponent component = {0};
            ReadParallaxComponentFromFile(file, &component);
            ParallaxComponent *new_comp = AddParallaxComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_particle_emitter:
        {
            ParticleEmitterComponent component = {0};
            ReadParticleEmitterComponentFromFile(file, &component);
            ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_player_data:
        {
            PlayerDataComponent component = {0};
            ReadPlayerDataComponentFromFile(file, &component);
            PlayerDataComponent *new_comp = AddPlayerDataComponent(entity);
            i32 new_comp_id = new_comp->component_id;
            *new_comp = component;
            new_comp->component_id = new_comp_id;
            new_comp->parent_entity_id = entity->entity_id;
        } break;
        case COMPONENT_interactable:
        {
            InteractableComponent component = {0};
            ReadInteractableComponentFromFile(file, &component);
            InteractableComponent *new_comp = AddInteractableComponent(entity);
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
        case COMPONENT_player_data:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                PlayerDataComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->player_data_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WritePlayerDataComponentToFile(file, comps[i].comp_data);
            }
        } break;
        case COMPONENT_interactable:
        {
            typedef struct ComponentSave {
                i32 entity_offset;
                InteractableComponent *comp_data;
            } ComponentSave;
            ComponentSave comps[MAX_ENTITIES];
            i32 comp_count = 0;

            for (i32 i = 0; i < id_count; i++)
            {
                if (entity_list[ids[i] - 1].component_ids[type])
                {
                    comps[comp_count].entity_offset = i;
                    comps[comp_count].comp_data = &component_set->interactable_components[entity_list[ids[i] - 1].component_ids[type] - 1];
                    comp_count++;
                }
            }

            WriteToFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
                WriteInteractableComponentToFile(file, comps[i].comp_data);
            }
        } break;
    }
}

SerialiseComponentsFromDataSet(FILE *file, Entity *entity_list, i32 entity_count, ComponentSet *component_set, i32 *ids, i32 id_count)
{
    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_position])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->position_components[entity->component_ids[COMPONENT_position] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WritePositionComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_sprite])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->sprite_components[entity->component_ids[COMPONENT_sprite] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteSpriteComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_animation])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->animation_components[entity->component_ids[COMPONENT_animation] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteAnimationComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_physics_body])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->physics_body_components[entity->component_ids[COMPONENT_physics_body] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WritePhysicsBodyComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_movement])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->movement_components[entity->component_ids[COMPONENT_movement] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteMovementComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_arc_entity])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->arc_entity_components[entity->component_ids[COMPONENT_arc_entity] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteArcEntityComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_item])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->item_components[entity->component_ids[COMPONENT_item] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteItemComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_trigger])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->trigger_components[entity->component_ids[COMPONENT_trigger] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteTriggerComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_parallax])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->parallax_components[entity->component_ids[COMPONENT_parallax] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteParallaxComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_particle_emitter])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->particle_emitter_components[entity->component_ids[COMPONENT_particle_emitter] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteParticleEmitterComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_player_data])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->player_data_components[entity->component_ids[COMPONENT_player_data] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WritePlayerDataComponentToFile(file, comps[i].comp_data);
        }
    }

    {
        ComponentSaveHelper comps[MAX_ENTITIES];
        i32 comp_count = 0;

        for (i32 i = 0; i < id_count; i++)
        {
            Entity *entity = &entity_list[ids[i] - 1];
            Assert(ids[i] - 1 < entity_count);
            if (entity->component_ids[COMPONENT_interactable])
            {
                comps[comp_count].entity_offset = i;
                comps[comp_count].comp_data = &component_set->interactable_components[entity->component_ids[COMPONENT_interactable] - 1];
                comp_count++;
            }
        }

        WriteToFile(file, &comp_count, sizeof(comp_count)); 
        for (i32 i = 0; i < comp_count; i++)
        {
            WriteToFile(file, &(comps[i].entity_offset), sizeof(i32));
            WriteInteractableComponentToFile(file, comps[i].comp_data);
        }
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                PositionComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                SpriteComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                AnimationComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                PhysicsBodyComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                MovementComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ArcEntityComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ItemComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                TriggerComponent component = {0};
                ReadTriggerComponentFromFile(file, &component);
                TriggerComponent *new_comp = AddTriggerComponent(entity);
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ParallaxComponent component = {0};
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
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                ParticleEmitterComponent component = {0};
                ReadParticleEmitterComponentFromFile(file, &component);
                ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_player_data:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                PlayerDataComponent component = {0};
                ReadPlayerDataComponentFromFile(file, &component);
                PlayerDataComponent *new_comp = AddPlayerDataComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
        case COMPONENT_interactable:
        {
            i32 comp_count = 0;
            ReadFromFile(file, &comp_count, sizeof(comp_count));
            for (i32 i = 0; i < comp_count; i++)
            {
                i32 entity_offset = 0;
                ReadFromFile(file, &entity_offset, sizeof(i32));
                Entity *entity = &core->run_data->entities[ids[entity_offset] - 1];
                Assert(entity->entity_id)
                InteractableComponent component = {0};
                ReadInteractableComponentFromFile(file, &component);
                InteractableComponent *new_comp = AddInteractableComponent(entity);
                i32 new_comp_id = new_comp->component_id;
                *new_comp = component;
                new_comp->component_id = new_comp_id;
                new_comp->parent_entity_id = entity->entity_id;
            }
        } break;
    }
}

DeserialiseComponentsToLoadData(FILE *file, ComponentSet *component_set, EntitySave *entity_list, i32 *ids, i32 id_count)
{
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            PositionComponent component_data;
            ReadPositionComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->position_components[component_set->position_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            SpriteComponent component_data;
            ReadSpriteComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->sprite_components[component_set->sprite_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            AnimationComponent component_data;
            ReadAnimationComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->animation_components[component_set->animation_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            PhysicsBodyComponent component_data;
            ReadPhysicsBodyComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->physics_body_components[component_set->physics_body_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            MovementComponent component_data;
            ReadMovementComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->movement_components[component_set->movement_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            ArcEntityComponent component_data;
            ReadArcEntityComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->arc_entity_components[component_set->arc_entity_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            ItemComponent component_data;
            ReadItemComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->item_components[component_set->item_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            TriggerComponent component_data;
            ReadTriggerComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->trigger_components[component_set->trigger_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            ParallaxComponent component_data;
            ReadParallaxComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->parallax_components[component_set->parallax_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            ParticleEmitterComponent component_data;
            ReadParticleEmitterComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->particle_emitter_components[component_set->particle_emitter_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            PlayerDataComponent component_data;
            ReadPlayerDataComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->player_data_components[component_set->player_data_component_count++] = component_data;
        }
    }
    {
        i32 component_count;
        ReadFromFile(file, &component_count, sizeof(i32));
        for (i32 i = 0; i < component_count; i++)
        {
            i32 entity_offset;
            ReadFromFile(file, &entity_offset, sizeof(i32));
            EntitySave *entity_save = &entity_list[ids[entity_offset] - 1];
            InteractableComponent component_data;
            ReadInteractableComponentFromFile(file, &component_data);
            component_data.parent_entity_id = ids[entity_offset];
            component_set->interactable_components[component_set->interactable_component_count++] = component_data;
        }
    }
}

DeserialiseComponentsFromMap(i32 *entity_id_map, i32 entity_count)
{
    for (i32 i = 0; i < core->run_data->loaded_entity_components.position_component_count; i++)
    {
        PositionComponent *saved_comp = &core->run_data->loaded_entity_components.position_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
        TriggerComponent *new_comp = AddTriggerComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.parallax_component_count; i++)
    {
        ParallaxComponent *saved_comp = &core->run_data->loaded_entity_components.parallax_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        Assert(entity->entity_id);
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
        Assert(entity->entity_id);
        ParticleEmitterComponent *new_comp = AddParticleEmitterComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.player_data_component_count; i++)
    {
        PlayerDataComponent *saved_comp = &core->run_data->loaded_entity_components.player_data_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        Assert(entity->entity_id);
        PlayerDataComponent *new_comp = AddPlayerDataComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
    for (i32 i = 0; i < core->run_data->loaded_entity_components.interactable_component_count; i++)
    {
        InteractableComponent *saved_comp = &core->run_data->loaded_entity_components.interactable_components[i];
        Entity *entity = &core->run_data->entities[entity_id_map[saved_comp->parent_entity_id - 1] - 1];
        Assert(entity->entity_id);
        InteractableComponent *new_comp = AddInteractableComponent(entity);
        i32 new_comp_id = new_comp->component_id;
        *new_comp = *saved_comp;
        new_comp->component_id = new_comp_id;
        new_comp->parent_entity_id = entity->entity_id;
    }
}

internal void ResetComponentSet(ComponentSet *comp_set)
{
    comp_set->position_component_count = 0;
    comp_set->sprite_component_count = 0;
    comp_set->animation_component_count = 0;
    comp_set->physics_body_component_count = 0;
    comp_set->movement_component_count = 0;
    comp_set->arc_entity_component_count = 0;
    comp_set->item_component_count = 0;
    comp_set->trigger_component_count = 0;
    comp_set->parallax_component_count = 0;
    comp_set->particle_emitter_component_count = 0;
    comp_set->player_data_component_count = 0;
    comp_set->interactable_component_count = 0;
}
static void WritePositionComponentToFile(FILE *file, PositionComponent *data)
{
    WriteToFile(file, &data->position, sizeof(data->position));

}

static void ReadPositionComponentFromFile(FILE *file, PositionComponent *data)
{
    ReadFromFile(file, &data->position, sizeof(data->position));

}

static void WriteSpriteComponentToFile(FILE *file, SpriteComponent *data)
{
    WriteToFile(file, &data->sprite_data, sizeof(data->sprite_data));

    WriteToFile(file, &data->is_flipped, sizeof(data->is_flipped));

    WriteToFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

}

static void ReadSpriteComponentFromFile(FILE *file, SpriteComponent *data)
{
    ReadFromFile(file, &data->sprite_data, sizeof(data->sprite_data));

    ReadFromFile(file, &data->is_flipped, sizeof(data->is_flipped));

    ReadFromFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

}

static void WriteAnimationComponentToFile(FILE *file, AnimationComponent *data)
{
    WriteToFile(file, &data->flags, sizeof(data->flags));

    WriteToFile(file, &data->current_frame, sizeof(data->current_frame));

    WriteToFile(file, &data->interval_mult, sizeof(data->interval_mult));

    WriteToFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

}

static void ReadAnimationComponentFromFile(FILE *file, AnimationComponent *data)
{
    ReadFromFile(file, &data->flags, sizeof(data->flags));

    ReadFromFile(file, &data->current_frame, sizeof(data->current_frame));

    ReadFromFile(file, &data->interval_mult, sizeof(data->interval_mult));

    ReadFromFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

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

    WriteToFile(file, &data->type, sizeof(data->type));

    WriteToFile(file, &data->collide_against, sizeof(data->collide_against));

}

static void ReadPhysicsBodyComponentFromFile(FILE *file, PhysicsBodyComponent *data)
{
    ReadFromFile(file, &data->shape, sizeof(data->shape));

    ReadFromFile(file, &data->shape_type, sizeof(data->shape_type));

    ReadFromFile(file, &data->material, sizeof(data->material));

    ReadFromFile(file, &data->mass_data, sizeof(data->mass_data));

    ReadFromFile(file, &data->velocity, sizeof(data->velocity));

    ReadFromFile(file, &data->force, sizeof(data->force));

    ReadFromFile(file, &data->gravity_multiplier, sizeof(data->gravity_multiplier));

    ReadFromFile(file, &data->type, sizeof(data->type));

    ReadFromFile(file, &data->collide_against, sizeof(data->collide_against));

}

static void WriteMovementComponentToFile(FILE *file, MovementComponent *data)
{
    WriteToFile(file, &data->axis_x, sizeof(data->axis_x));

    WriteToFile(file, &data->move_speed, sizeof(data->move_speed));

    WriteToFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

}

static void ReadMovementComponentFromFile(FILE *file, MovementComponent *data)
{
    ReadFromFile(file, &data->axis_x, sizeof(data->axis_x));

    ReadFromFile(file, &data->move_speed, sizeof(data->move_speed));

    ReadFromFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

}

static void WriteArcEntityComponentToFile(FILE *file, ArcEntityComponent *data)
{
    WriteToFile(file, &data->entity_type, sizeof(data->entity_type));

            i32 ptr = 0;
            WriteToFile(file, &ptr, sizeof(i32));
            // TODO(randy)

    WriteToFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

}

static void ReadArcEntityComponentFromFile(FILE *file, ArcEntityComponent *data)
{
    ReadFromFile(file, &data->entity_type, sizeof(data->entity_type));

            i32 ptr = 0;
            ReadFromFile(file, &ptr, sizeof(i32));
            Assert(ptr == 0); // Not implemented yet

    ReadFromFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

}

static void WriteItemComponentToFile(FILE *file, ItemComponent *data)
{
    WriteToFile(file, &data->item, sizeof(data->item));

}

static void ReadItemComponentFromFile(FILE *file, ItemComponent *data)
{
    ReadFromFile(file, &data->item, sizeof(data->item));

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

static void ReadTriggerComponentFromFile(FILE *file, TriggerComponent *data)
{
    ReadFromFile(file, &data->enter_trigger_callback, sizeof(data->enter_trigger_callback));

    ReadFromFile(file, &data->exit_trigger_callback, sizeof(data->exit_trigger_callback));

    for (i32 i = 0; i < MAX_OVERLAPPING_COLLIDERS; i++)
    {
        ReadFromFile(file, &data->previous_overlaps[i], sizeof(OverlappedColliderInfo));
    }

    ReadFromFile(file, &data->previous_overlaps_count, sizeof(data->previous_overlaps_count));

    ReadFromFile(file, &data->trigger_against, sizeof(data->trigger_against));

}

static void WriteParallaxComponentToFile(FILE *file, ParallaxComponent *data)
{
    WriteToFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    WriteToFile(file, &data->desired_position, sizeof(data->desired_position));

}

static void ReadParallaxComponentFromFile(FILE *file, ParallaxComponent *data)
{
    ReadFromFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    ReadFromFile(file, &data->desired_position, sizeof(data->desired_position));

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

static void ReadParticleEmitterComponentFromFile(FILE *file, ParticleEmitterComponent *data)
{
    ReadFromFile(file, &data->life_time, sizeof(data->life_time));

    ReadFromFile(file, &data->start_time, sizeof(data->start_time));

    ReadFromFile(file, &data->flags, sizeof(data->flags));

    for (i32 i = 0; i < MAX_PARTICLE_AMOUNT; i++)
    {
        ReadFromFile(file, &data->particles[i], sizeof(Particle));
    }

    ReadFromFile(file, &data->particle_count, sizeof(data->particle_count));

    ReadFromFile(file, &data->free_particle_index, sizeof(data->free_particle_index));

    ReadFromFile(file, &data->begin_callback, sizeof(data->begin_callback));

    ReadFromFile(file, &data->finish_callback, sizeof(data->finish_callback));

}

static void WritePlayerDataComponentToFile(FILE *file, PlayerDataComponent *data)
{
    for (i32 i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        WriteToFile(file, &data->inventory[i], sizeof(Item));
    }

    WriteToFile(file, &data->inventory_size, sizeof(data->inventory_size));

    for (i32 i = 0; i < MAX_HOTBAR_SLOTS; i++)
    {
        WriteToFile(file, &data->hotbar[i], sizeof(Item));
    }

    WriteToFile(file, &data->hotbar_size, sizeof(data->hotbar_size));

    WriteToFile(file, &data->active_hotbar_slot, sizeof(data->active_hotbar_slot));

    WriteToFile(file, &data->grabbed_item, sizeof(data->grabbed_item));

    WriteToFile(file, &data->grabbed_item_offset, sizeof(data->grabbed_item_offset));

            i32 ptr = 0;
            WriteToFile(file, &ptr, sizeof(i32));
            // TODO(randy)

}

static void ReadPlayerDataComponentFromFile(FILE *file, PlayerDataComponent *data)
{
    for (i32 i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        ReadFromFile(file, &data->inventory[i], sizeof(Item));
    }

    ReadFromFile(file, &data->inventory_size, sizeof(data->inventory_size));

    for (i32 i = 0; i < MAX_HOTBAR_SLOTS; i++)
    {
        ReadFromFile(file, &data->hotbar[i], sizeof(Item));
    }

    ReadFromFile(file, &data->hotbar_size, sizeof(data->hotbar_size));

    ReadFromFile(file, &data->active_hotbar_slot, sizeof(data->active_hotbar_slot));

    ReadFromFile(file, &data->grabbed_item, sizeof(data->grabbed_item));

    ReadFromFile(file, &data->grabbed_item_offset, sizeof(data->grabbed_item_offset));

            i32 ptr = 0;
            ReadFromFile(file, &ptr, sizeof(i32));
            Assert(ptr == 0); // Not implemented yet

}

static void WriteInteractableComponentToFile(FILE *file, InteractableComponent *data)
{
    WriteToFile(file, &data->bounds, sizeof(data->bounds));

    WriteToFile(file, &data->bounds_type, sizeof(data->bounds_type));

    WriteToFile(file, &data->priority, sizeof(data->priority));

    WriteToFile(file, &data->interact_callback, sizeof(data->interact_callback));

}

static void ReadInteractableComponentFromFile(FILE *file, InteractableComponent *data)
{
    ReadFromFile(file, &data->bounds, sizeof(data->bounds));

    ReadFromFile(file, &data->bounds_type, sizeof(data->bounds_type));

    ReadFromFile(file, &data->priority, sizeof(data->priority));

    ReadFromFile(file, &data->interact_callback, sizeof(data->interact_callback));

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

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WritePlayerDataComponentToFile(file, &(data->player_data_components[i]));
    }

    WriteToFile(file, &data->player_data_component_count, sizeof(data->player_data_component_count));

    WriteToFile(file, &data->free_player_data_component_id, sizeof(data->free_player_data_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        WriteInteractableComponentToFile(file, &(data->interactable_components[i]));
    }

    WriteToFile(file, &data->interactable_component_count, sizeof(data->interactable_component_count));

    WriteToFile(file, &data->free_interactable_component_id, sizeof(data->free_interactable_component_id));

}

static void ReadComponentSetFromFile(FILE *file, ComponentSet *data)
{
    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadPositionComponentFromFile(file, &(data->position_components[i]));
    }

    ReadFromFile(file, &data->position_component_count, sizeof(data->position_component_count));

    ReadFromFile(file, &data->free_position_component_id, sizeof(data->free_position_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadSpriteComponentFromFile(file, &(data->sprite_components[i]));
    }

    ReadFromFile(file, &data->sprite_component_count, sizeof(data->sprite_component_count));

    ReadFromFile(file, &data->free_sprite_component_id, sizeof(data->free_sprite_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadAnimationComponentFromFile(file, &(data->animation_components[i]));
    }

    ReadFromFile(file, &data->animation_component_count, sizeof(data->animation_component_count));

    ReadFromFile(file, &data->free_animation_component_id, sizeof(data->free_animation_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadPhysicsBodyComponentFromFile(file, &(data->physics_body_components[i]));
    }

    ReadFromFile(file, &data->physics_body_component_count, sizeof(data->physics_body_component_count));

    ReadFromFile(file, &data->free_physics_body_component_id, sizeof(data->free_physics_body_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadMovementComponentFromFile(file, &(data->movement_components[i]));
    }

    ReadFromFile(file, &data->movement_component_count, sizeof(data->movement_component_count));

    ReadFromFile(file, &data->free_movement_component_id, sizeof(data->free_movement_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadArcEntityComponentFromFile(file, &(data->arc_entity_components[i]));
    }

    ReadFromFile(file, &data->arc_entity_component_count, sizeof(data->arc_entity_component_count));

    ReadFromFile(file, &data->free_arc_entity_component_id, sizeof(data->free_arc_entity_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadItemComponentFromFile(file, &(data->item_components[i]));
    }

    ReadFromFile(file, &data->item_component_count, sizeof(data->item_component_count));

    ReadFromFile(file, &data->free_item_component_id, sizeof(data->free_item_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadTriggerComponentFromFile(file, &(data->trigger_components[i]));
    }

    ReadFromFile(file, &data->trigger_component_count, sizeof(data->trigger_component_count));

    ReadFromFile(file, &data->free_trigger_component_id, sizeof(data->free_trigger_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadParallaxComponentFromFile(file, &(data->parallax_components[i]));
    }

    ReadFromFile(file, &data->parallax_component_count, sizeof(data->parallax_component_count));

    ReadFromFile(file, &data->free_parallax_component_id, sizeof(data->free_parallax_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadParticleEmitterComponentFromFile(file, &(data->particle_emitter_components[i]));
    }

    ReadFromFile(file, &data->particle_emitter_component_count, sizeof(data->particle_emitter_component_count));

    ReadFromFile(file, &data->free_particle_emitter_component_id, sizeof(data->free_particle_emitter_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadPlayerDataComponentFromFile(file, &(data->player_data_components[i]));
    }

    ReadFromFile(file, &data->player_data_component_count, sizeof(data->player_data_component_count));

    ReadFromFile(file, &data->free_player_data_component_id, sizeof(data->free_player_data_component_id));

    for (i32 i = 0; i < MAX_ENTITIES; i++)
    {
        ReadInteractableComponentFromFile(file, &(data->interactable_components[i]));
    }

    ReadFromFile(file, &data->interactable_component_count, sizeof(data->interactable_component_count));

    ReadFromFile(file, &data->free_interactable_component_id, sizeof(data->free_interactable_component_id));

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

static void ReadEntityFromFile(FILE *file, Entity *data)
{
    ReadFromFile(file, &data->entity_id, sizeof(data->entity_id));

    for (i32 i = 0; i < COMPONENT_MAX; i++)
    {
        ReadFromFile(file, &data->component_ids[i], sizeof(i32));
    }

    for (i32 i = 0; i < 20; i++)
    {
        ReadFromFile(file, &data->name[i], sizeof(char));
    }

    ReadFromFile(file, &data->flags, sizeof(data->flags));

    ReadFromFile(file, &data->generalised_type, sizeof(data->generalised_type));

}

static void WriteWorldSaveDataToFile(FILE *file, WorldSaveData *data)
{
    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

            i32 ptr = 0;
            WriteToFile(file, &ptr, sizeof(i32));
            // TODO(randy)

}

static void ReadWorldSaveDataFromFile(FILE *file, WorldSaveData *data)
{
    ReadFromFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

            i32 ptr = 0;
            ReadFromFile(file, &ptr, sizeof(i32));
            Assert(ptr == 0); // Not implemented yet

}

