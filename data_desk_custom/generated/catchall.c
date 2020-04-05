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

static char *GetCellPropertiesTypeName(CellPropertiesType type)
{
switch(type)
{
case CELL_PROPERTIES_TYPE_solid:
return "Solid";
break;
case CELL_PROPERTIES_TYPE_fluid:
return "Fluid";
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
case CELL_MATERIAL_TYPE_dirt:
return "Dirt";
break;
case CELL_MATERIAL_TYPE_sand:
return "Sand";
break;
case CELL_MATERIAL_TYPE_water:
return "Water";
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

internal void PrintComponentDataUI(void *component_data, ComponentType type)
{
    switch (type)
    {
    case COMPONENT_INVALID :
    case COMPONENT_MAX :
        R_BREAK("Invalid component.")
        break;

    case COMPONENT_position :
    {
        PositionComponent *component = (PositionComponent*)component_data;
        char title[100];
        sprintf(title, "Position #%i", component->component_id);
        if (TsUICollapsable(title))        {
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "position: %f, %f", component->position.x, component->position.y); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_sprite :
    {
        SpriteComponent *component = (SpriteComponent*)component_data;
        char title[100];
        sprintf(title, "Sprite #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'sprite_data'
            component->is_flipped = TsUIToggler("is_flipped", component->is_flipped);
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, component->is_background_sprite ? "is_background_sprite: true" : "is_background_sprite: false"); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_animation :
    {
        AnimationComponent *component = (AnimationComponent*)component_data;
        char title[100];
        sprintf(title, "Animation #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'flags'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "current_frame: %i", component->current_frame); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "interval_mult: %f", component->interval_mult); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "frame_start_time: %f", component->frame_start_time); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_physics_body :
    {
        PhysicsBodyComponent *component = (PhysicsBodyComponent*)component_data;
        char title[100];
        sprintf(title, "PhysicsBody #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'shape'
            // TODO: Don't know how to generate UI print for variable 'shape_type'
            // TODO: Don't know how to generate UI print for variable 'material'
            // TODO: Don't know how to generate UI print for variable 'mass_data'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "velocity: %f, %f", component->velocity.x, component->velocity.y); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "force: %f, %f", component->force.x, component->force.y); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "gravity_multiplier: %f", component->gravity_multiplier); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_movement :
    {
        MovementComponent *component = (MovementComponent*)component_data;
        char title[100];
        sprintf(title, "Movement #%i", component->component_id);
        if (TsUICollapsable(title))        {
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "axis_x: %f", component->axis_x); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "move_speed: %f", component->move_speed); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "move_speed_mult: %f", component->move_speed_mult); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_arc_entity :
    {
        ArcEntityComponent *component = (ArcEntityComponent*)component_data;
        char title[100];
        sprintf(title, "ArcEntity #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'entity_type'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "current_general_state: %s", component->current_general_state); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'current_animation_state'

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_item :
    {
        ItemComponent *component = (ItemComponent*)component_data;
        char title[100];
        sprintf(title, "Item #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'item_type'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "stack_size: %i", component->stack_size); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_trigger :
    {
        TriggerComponent *component = (TriggerComponent*)component_data;
        char title[100];
        sprintf(title, "Trigger #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'enter_trigger_callback'
            // TODO: Don't know how to generate UI print for variable 'exit_trigger_callback'
            // TODO: Don't know how to generate UI print for variable 'previous_overlaps'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "previous_overlaps_count: %i", component->previous_overlaps_count); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, component->trigger_against ? "trigger_against: true" : "trigger_against: false"); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_storage :
    {
        StorageComponent *component = (StorageComponent*)component_data;
        char title[100];
        sprintf(title, "Storage #%i", component->component_id);
        if (TsUICollapsable(title))        {
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "storage_size: %i", component->storage_size); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'items'

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_parallax :
    {
        ParallaxComponent *component = (ParallaxComponent*)component_data;
        char title[100];
        sprintf(title, "Parallax #%i", component->component_id);
        if (TsUICollapsable(title))        {
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "parallax_amount: %f, %f", component->parallax_amount.x, component->parallax_amount.y); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "desired_position: %f, %f", component->desired_position.x, component->desired_position.y); TsUILabel(label); }
            TsUIPopWidth();

            TsUICollapsableEnd();
        }
        break;
    }

    case COMPONENT_particle_emitter :
    {
        ParticleEmitterComponent *component = (ParticleEmitterComponent*)component_data;
        char title[100];
        sprintf(title, "ParticleEmitter #%i", component->component_id);
        if (TsUICollapsable(title))        {
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "life_time: %f", component->life_time); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "start_time: %f", component->start_time); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'flags'
            // TODO: Don't know how to generate UI print for variable 'particles'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "particle_count: %i", component->particle_count); TsUILabel(label); }
            TsUIPopWidth();
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "free_particle_index: %i", component->free_particle_index); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'begin_callback'
            // TODO: Don't know how to generate UI print for variable 'finish_callback'

            TsUICollapsableEnd();
        }
        break;
    }

    }
}

internal PositionComponent *AddPositionComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.position_free_component_id == core->world_data->entity_components.position_component_count)
    {
        component_id = core->world_data->entity_components.position_component_count;
        core->world_data->entity_components.position_component_count++;
        core->world_data->entity_components.position_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.position_free_component_id;
    }

    core->world_data->entity_components.position_components[component_id] = GetDefaultPositionComponent();
    entity->components[COMPONENT_position] = &core->world_data->entity_components.position_components[component_id];
    core->world_data->entity_components.position_components[component_id].parent_entity = entity;
    core->world_data->entity_components.position_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.position_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.position_components[i].parent_entity)
        {
            core->world_data->entity_components.position_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.position_components[component_id];
}

internal void RemovePositionComponent(Entity *entity)
{
    PositionComponent *component = entity->components[COMPONENT_position];
    R_DEV_ASSERT(component, "Entity does not a PositionComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    PositionComponent empty_comp = {0};
    core->world_data->entity_components.position_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_position] = 0;

    if (deleted_component_id < core->world_data->entity_components.position_free_component_id)
        core->world_data->entity_components.position_free_component_id = deleted_component_id;
}

internal SpriteComponent *AddSpriteComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.sprite_free_component_id == core->world_data->entity_components.sprite_component_count)
    {
        component_id = core->world_data->entity_components.sprite_component_count;
        core->world_data->entity_components.sprite_component_count++;
        core->world_data->entity_components.sprite_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.sprite_free_component_id;
    }

    core->world_data->entity_components.sprite_components[component_id] = GetDefaultSpriteComponent();
    entity->components[COMPONENT_sprite] = &core->world_data->entity_components.sprite_components[component_id];
    core->world_data->entity_components.sprite_components[component_id].parent_entity = entity;
    core->world_data->entity_components.sprite_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.sprite_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.sprite_components[i].parent_entity)
        {
            core->world_data->entity_components.sprite_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.sprite_components[component_id];
}

internal void RemoveSpriteComponent(Entity *entity)
{
    SpriteComponent *component = entity->components[COMPONENT_sprite];
    R_DEV_ASSERT(component, "Entity does not a SpriteComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    SpriteComponent empty_comp = {0};
    core->world_data->entity_components.sprite_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_sprite] = 0;

    if (deleted_component_id < core->world_data->entity_components.sprite_free_component_id)
        core->world_data->entity_components.sprite_free_component_id = deleted_component_id;
}

internal AnimationComponent *AddAnimationComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.animation_free_component_id == core->world_data->entity_components.animation_component_count)
    {
        component_id = core->world_data->entity_components.animation_component_count;
        core->world_data->entity_components.animation_component_count++;
        core->world_data->entity_components.animation_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.animation_free_component_id;
    }

    core->world_data->entity_components.animation_components[component_id] = GetDefaultAnimationComponent();
    entity->components[COMPONENT_animation] = &core->world_data->entity_components.animation_components[component_id];
    core->world_data->entity_components.animation_components[component_id].parent_entity = entity;
    core->world_data->entity_components.animation_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.animation_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.animation_components[i].parent_entity)
        {
            core->world_data->entity_components.animation_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.animation_components[component_id];
}

internal void RemoveAnimationComponent(Entity *entity)
{
    AnimationComponent *component = entity->components[COMPONENT_animation];
    R_DEV_ASSERT(component, "Entity does not a AnimationComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    AnimationComponent empty_comp = {0};
    core->world_data->entity_components.animation_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_animation] = 0;

    if (deleted_component_id < core->world_data->entity_components.animation_free_component_id)
        core->world_data->entity_components.animation_free_component_id = deleted_component_id;
}

internal PhysicsBodyComponent *AddPhysicsBodyComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.physics_body_free_component_id == core->world_data->entity_components.physics_body_component_count)
    {
        component_id = core->world_data->entity_components.physics_body_component_count;
        core->world_data->entity_components.physics_body_component_count++;
        core->world_data->entity_components.physics_body_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.physics_body_free_component_id;
    }

    core->world_data->entity_components.physics_body_components[component_id] = GetDefaultPhysicsBodyComponent();
    entity->components[COMPONENT_physics_body] = &core->world_data->entity_components.physics_body_components[component_id];
    core->world_data->entity_components.physics_body_components[component_id].parent_entity = entity;
    core->world_data->entity_components.physics_body_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.physics_body_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.physics_body_components[i].parent_entity)
        {
            core->world_data->entity_components.physics_body_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.physics_body_components[component_id];
}

internal void RemovePhysicsBodyComponent(Entity *entity)
{
    PhysicsBodyComponent *component = entity->components[COMPONENT_physics_body];
    R_DEV_ASSERT(component, "Entity does not a PhysicsBodyComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    PhysicsBodyComponent empty_comp = {0};
    core->world_data->entity_components.physics_body_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_physics_body] = 0;

    if (deleted_component_id < core->world_data->entity_components.physics_body_free_component_id)
        core->world_data->entity_components.physics_body_free_component_id = deleted_component_id;
}

internal MovementComponent *AddMovementComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.movement_free_component_id == core->world_data->entity_components.movement_component_count)
    {
        component_id = core->world_data->entity_components.movement_component_count;
        core->world_data->entity_components.movement_component_count++;
        core->world_data->entity_components.movement_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.movement_free_component_id;
    }

    core->world_data->entity_components.movement_components[component_id] = GetDefaultMovementComponent();
    entity->components[COMPONENT_movement] = &core->world_data->entity_components.movement_components[component_id];
    core->world_data->entity_components.movement_components[component_id].parent_entity = entity;
    core->world_data->entity_components.movement_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.movement_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.movement_components[i].parent_entity)
        {
            core->world_data->entity_components.movement_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.movement_components[component_id];
}

internal void RemoveMovementComponent(Entity *entity)
{
    MovementComponent *component = entity->components[COMPONENT_movement];
    R_DEV_ASSERT(component, "Entity does not a MovementComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    MovementComponent empty_comp = {0};
    core->world_data->entity_components.movement_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_movement] = 0;

    if (deleted_component_id < core->world_data->entity_components.movement_free_component_id)
        core->world_data->entity_components.movement_free_component_id = deleted_component_id;
}

internal ArcEntityComponent *AddArcEntityComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.arc_entity_free_component_id == core->world_data->entity_components.arc_entity_component_count)
    {
        component_id = core->world_data->entity_components.arc_entity_component_count;
        core->world_data->entity_components.arc_entity_component_count++;
        core->world_data->entity_components.arc_entity_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.arc_entity_free_component_id;
    }

    core->world_data->entity_components.arc_entity_components[component_id] = GetDefaultArcEntityComponent();
    entity->components[COMPONENT_arc_entity] = &core->world_data->entity_components.arc_entity_components[component_id];
    core->world_data->entity_components.arc_entity_components[component_id].parent_entity = entity;
    core->world_data->entity_components.arc_entity_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.arc_entity_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.arc_entity_components[i].parent_entity)
        {
            core->world_data->entity_components.arc_entity_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.arc_entity_components[component_id];
}

internal void RemoveArcEntityComponent(Entity *entity)
{
    ArcEntityComponent *component = entity->components[COMPONENT_arc_entity];
    R_DEV_ASSERT(component, "Entity does not a ArcEntityComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ArcEntityComponent empty_comp = {0};
    core->world_data->entity_components.arc_entity_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_arc_entity] = 0;

    if (deleted_component_id < core->world_data->entity_components.arc_entity_free_component_id)
        core->world_data->entity_components.arc_entity_free_component_id = deleted_component_id;
}

internal ItemComponent *AddItemComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.item_free_component_id == core->world_data->entity_components.item_component_count)
    {
        component_id = core->world_data->entity_components.item_component_count;
        core->world_data->entity_components.item_component_count++;
        core->world_data->entity_components.item_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.item_free_component_id;
    }

    core->world_data->entity_components.item_components[component_id] = GetDefaultItemComponent();
    entity->components[COMPONENT_item] = &core->world_data->entity_components.item_components[component_id];
    core->world_data->entity_components.item_components[component_id].parent_entity = entity;
    core->world_data->entity_components.item_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.item_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.item_components[i].parent_entity)
        {
            core->world_data->entity_components.item_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.item_components[component_id];
}

internal void RemoveItemComponent(Entity *entity)
{
    ItemComponent *component = entity->components[COMPONENT_item];
    R_DEV_ASSERT(component, "Entity does not a ItemComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ItemComponent empty_comp = {0};
    core->world_data->entity_components.item_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_item] = 0;

    if (deleted_component_id < core->world_data->entity_components.item_free_component_id)
        core->world_data->entity_components.item_free_component_id = deleted_component_id;
}

internal TriggerComponent *AddTriggerComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.trigger_free_component_id == core->world_data->entity_components.trigger_component_count)
    {
        component_id = core->world_data->entity_components.trigger_component_count;
        core->world_data->entity_components.trigger_component_count++;
        core->world_data->entity_components.trigger_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.trigger_free_component_id;
    }

    core->world_data->entity_components.trigger_components[component_id] = GetDefaultTriggerComponent();
    entity->components[COMPONENT_trigger] = &core->world_data->entity_components.trigger_components[component_id];
    core->world_data->entity_components.trigger_components[component_id].parent_entity = entity;
    core->world_data->entity_components.trigger_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.trigger_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.trigger_components[i].parent_entity)
        {
            core->world_data->entity_components.trigger_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.trigger_components[component_id];
}

internal void RemoveTriggerComponent(Entity *entity)
{
    TriggerComponent *component = entity->components[COMPONENT_trigger];
    R_DEV_ASSERT(component, "Entity does not a TriggerComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    TriggerComponent empty_comp = {0};
    core->world_data->entity_components.trigger_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_trigger] = 0;

    if (deleted_component_id < core->world_data->entity_components.trigger_free_component_id)
        core->world_data->entity_components.trigger_free_component_id = deleted_component_id;
}

internal StorageComponent *AddStorageComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.storage_free_component_id == core->world_data->entity_components.storage_component_count)
    {
        component_id = core->world_data->entity_components.storage_component_count;
        core->world_data->entity_components.storage_component_count++;
        core->world_data->entity_components.storage_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.storage_free_component_id;
    }

    core->world_data->entity_components.storage_components[component_id] = GetDefaultStorageComponent();
    entity->components[COMPONENT_storage] = &core->world_data->entity_components.storage_components[component_id];
    core->world_data->entity_components.storage_components[component_id].parent_entity = entity;
    core->world_data->entity_components.storage_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.storage_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.storage_components[i].parent_entity)
        {
            core->world_data->entity_components.storage_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.storage_components[component_id];
}

internal void RemoveStorageComponent(Entity *entity)
{
    StorageComponent *component = entity->components[COMPONENT_storage];
    R_DEV_ASSERT(component, "Entity does not a StorageComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    StorageComponent empty_comp = {0};
    core->world_data->entity_components.storage_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_storage] = 0;

    if (deleted_component_id < core->world_data->entity_components.storage_free_component_id)
        core->world_data->entity_components.storage_free_component_id = deleted_component_id;
}

internal ParallaxComponent *AddParallaxComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.parallax_free_component_id == core->world_data->entity_components.parallax_component_count)
    {
        component_id = core->world_data->entity_components.parallax_component_count;
        core->world_data->entity_components.parallax_component_count++;
        core->world_data->entity_components.parallax_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.parallax_free_component_id;
    }

    core->world_data->entity_components.parallax_components[component_id] = GetDefaultParallaxComponent();
    entity->components[COMPONENT_parallax] = &core->world_data->entity_components.parallax_components[component_id];
    core->world_data->entity_components.parallax_components[component_id].parent_entity = entity;
    core->world_data->entity_components.parallax_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.parallax_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.parallax_components[i].parent_entity)
        {
            core->world_data->entity_components.parallax_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.parallax_components[component_id];
}

internal void RemoveParallaxComponent(Entity *entity)
{
    ParallaxComponent *component = entity->components[COMPONENT_parallax];
    R_DEV_ASSERT(component, "Entity does not a ParallaxComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ParallaxComponent empty_comp = {0};
    core->world_data->entity_components.parallax_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_parallax] = 0;

    if (deleted_component_id < core->world_data->entity_components.parallax_free_component_id)
        core->world_data->entity_components.parallax_free_component_id = deleted_component_id;
}

internal ParticleEmitterComponent *AddParticleEmitterComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.particle_emitter_free_component_id == core->world_data->entity_components.particle_emitter_component_count)
    {
        component_id = core->world_data->entity_components.particle_emitter_component_count;
        core->world_data->entity_components.particle_emitter_component_count++;
        core->world_data->entity_components.particle_emitter_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.particle_emitter_free_component_id;
    }

    core->world_data->entity_components.particle_emitter_components[component_id] = GetDefaultParticleEmitterComponent();
    entity->components[COMPONENT_particle_emitter] = &core->world_data->entity_components.particle_emitter_components[component_id];
    core->world_data->entity_components.particle_emitter_components[component_id].parent_entity = entity;
    core->world_data->entity_components.particle_emitter_components[component_id].component_id = component_id;

    for (int i = 0; i < core->world_data->entity_components.particle_emitter_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.particle_emitter_components[i].parent_entity)
        {
            core->world_data->entity_components.particle_emitter_free_component_id = i;
            break;
        }
    }

    return &core->world_data->entity_components.particle_emitter_components[component_id];
}

internal void RemoveParticleEmitterComponent(Entity *entity)
{
    ParticleEmitterComponent *component = entity->components[COMPONENT_particle_emitter];
    R_DEV_ASSERT(component, "Entity does not a ParticleEmitterComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ParticleEmitterComponent empty_comp = {0};
    core->world_data->entity_components.particle_emitter_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_particle_emitter] = 0;

    if (deleted_component_id < core->world_data->entity_components.particle_emitter_free_component_id)
        core->world_data->entity_components.particle_emitter_free_component_id = deleted_component_id;
}

internal void DeleteEntity(Entity *entity)
{
    PositionComponent *position_component = entity->components[1];
    if (position_component)
        RemovePositionComponent(entity);
    SpriteComponent *sprite_component = entity->components[2];
    if (sprite_component)
        RemoveSpriteComponent(entity);
    AnimationComponent *animation_component = entity->components[3];
    if (animation_component)
        RemoveAnimationComponent(entity);
    PhysicsBodyComponent *physics_body_component = entity->components[4];
    if (physics_body_component)
        RemovePhysicsBodyComponent(entity);
    MovementComponent *movement_component = entity->components[5];
    if (movement_component)
        RemoveMovementComponent(entity);
    ArcEntityComponent *arc_entity_component = entity->components[6];
    if (arc_entity_component)
        RemoveArcEntityComponent(entity);
    ItemComponent *item_component = entity->components[7];
    if (item_component)
        RemoveItemComponent(entity);
    TriggerComponent *trigger_component = entity->components[8];
    if (trigger_component)
        RemoveTriggerComponent(entity);
    StorageComponent *storage_component = entity->components[9];
    if (storage_component)
        RemoveStorageComponent(entity);
    ParallaxComponent *parallax_component = entity->components[10];
    if (parallax_component)
        RemoveParallaxComponent(entity);
    ParticleEmitterComponent *particle_emitter_component = entity->components[11];
    if (particle_emitter_component)
        RemoveParticleEmitterComponent(entity);

    i32 deleted_entity_id = entity->entity_id;
    Entity empty_entity = {0};
    *entity = empty_entity;
    if (deleted_entity_id < core->world_data->free_entity_id)
        core->world_data->free_entity_id = deleted_entity_id;
}

static CharacterEntity *InitialiseCharacterEntity()
{
    Entity *generic_entity = NewEntity("Character", ENTITY_TYPE_character, GENERALISED_ENTITY_TYPE_character);
    CharacterEntity *unique_entity = &core->world_data->character_entity;
    generic_entity->unique_entity = unique_entity;
    unique_entity->parent_generic_entity = generic_entity;

    unique_entity->position_comp = AddPositionComponent(generic_entity);
    unique_entity->sprite_comp = AddSpriteComponent(generic_entity);
    unique_entity->animation_comp = AddAnimationComponent(generic_entity);
    unique_entity->physics_body_comp = AddPhysicsBodyComponent(generic_entity);
    unique_entity->movement_comp = AddMovementComponent(generic_entity);
    unique_entity->arc_entity_comp = AddArcEntityComponent(generic_entity);

    return unique_entity;
}

static CloudEntity *NewCloudEntity()
{
    R_DEV_ASSERT(core->world_data->free_cloud_entity_index + 1 < MAX_CLOUD_ENTITIES, "Maximum amount of Cloud entites reached");

    i32 new_unique_id = core->world_data->free_cloud_entity_index;
    if (core->world_data->free_cloud_entity_index == core->world_data->cloud_entity_count)
    {
        core->world_data->cloud_entity_count++;
        core->world_data->free_cloud_entity_index++;
    }
    core->world_data->cloud_entities[new_unique_id].unique_entity_id = new_unique_id;

    Entity *generic_entity = NewEntity("Cloud", ENTITY_TYPE_cloud, GENERALISED_ENTITY_TYPE_scenic);
    CloudEntity *unique_entity = &core->world_data->cloud_entities[new_unique_id];
    generic_entity->unique_entity = unique_entity;
    unique_entity->parent_generic_entity = generic_entity;
    unique_entity->unique_entity_id = new_unique_id;

    unique_entity->position_comp = AddPositionComponent(generic_entity);
    unique_entity->sprite_comp = AddSpriteComponent(generic_entity);
    unique_entity->parallax_comp = AddParallaxComponent(generic_entity);

    return unique_entity;
}

static GroundSegmentEntity *NewGroundSegmentEntity()
{
    R_DEV_ASSERT(core->world_data->free_ground_segment_entity_index + 1 < MAX_GROUND_SEGMENT_ENTITIES, "Maximum amount of GroundSegment entites reached");

    i32 new_unique_id = core->world_data->free_ground_segment_entity_index;
    if (core->world_data->free_ground_segment_entity_index == core->world_data->ground_segment_entity_count)
    {
        core->world_data->ground_segment_entity_count++;
        core->world_data->free_ground_segment_entity_index++;
    }
    core->world_data->ground_segment_entities[new_unique_id].unique_entity_id = new_unique_id;

    Entity *generic_entity = NewEntity("GroundSegment", ENTITY_TYPE_ground_segment, GENERALISED_ENTITY_TYPE_ground);
    GroundSegmentEntity *unique_entity = &core->world_data->ground_segment_entities[new_unique_id];
    generic_entity->unique_entity = unique_entity;
    unique_entity->parent_generic_entity = generic_entity;
    unique_entity->unique_entity_id = new_unique_id;

    unique_entity->position_comp = AddPositionComponent(generic_entity);
    unique_entity->physics_body_comp = AddPhysicsBodyComponent(generic_entity);

    return unique_entity;
}

static void PrintEntityDataUI(Entity *entity)
{
    switch(entity->type)
    {
    case ENTITY_TYPE_character :
    {
        CharacterEntity *unique_entity = entity->unique_entity;
        break;
    }
    case ENTITY_TYPE_cloud :
    {
        CloudEntity *unique_entity = entity->unique_entity;
        break;
    }
    case ENTITY_TYPE_ground_segment :
    {
        GroundSegmentEntity *unique_entity = entity->unique_entity;
        break;
    }
    }

    for (int i = 1; i < COMPONENT_MAX; i++)
    {
        if (entity->components[i])
        {
            PrintComponentDataUI(entity->components[i], i);
        }
    }
}
