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
        sprintf(title, "PositionComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "SpriteComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "AnimationComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "PhysicsBodyComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "MovementComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "ArcEntityComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "ItemComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "TriggerComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "StorageComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "ParallaxComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
        sprintf(title, "ParticleEmitterComponent #%i", component->component_id);
        if (TsUICollapsable(title))        {
            // TODO: Don't know how to generate UI print for variable 'parent_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "component_id: %i", component->component_id); TsUILabel(label); }
            TsUIPopWidth();
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
    if (core->world_data->entity_components.position_component_free_id == core->world_data->entity_components.position_component_count)
    {
        component_id = core->world_data->entity_components.position_component_count;
        core->world_data->entity_components.position_component_count++;
        core->world_data->entity_components.position_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.position_component_free_id;
    }

    core->world_data->entity_components.position_components[component_id] = GetDefaultPositionComponent();
    entity->components[COMPONENT_position] = &core->world_data->entity_components.position_components[component_id];
    core->world_data->entity_components.position_components[component_id].parent_entity = entity;
    core->world_data->entity_components.position_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.position_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.position_components[i].parent_entity)
        {
            core->world_data->entity_components.position_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.position_component_free_id)
        core->world_data->entity_components.position_component_free_id = deleted_component_id;
}

internal SpriteComponent *AddSpriteComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.sprite_component_free_id == core->world_data->entity_components.sprite_component_count)
    {
        component_id = core->world_data->entity_components.sprite_component_count;
        core->world_data->entity_components.sprite_component_count++;
        core->world_data->entity_components.sprite_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.sprite_component_free_id;
    }

    core->world_data->entity_components.sprite_components[component_id] = GetDefaultSpriteComponent();
    entity->components[COMPONENT_sprite] = &core->world_data->entity_components.sprite_components[component_id];
    core->world_data->entity_components.sprite_components[component_id].parent_entity = entity;
    core->world_data->entity_components.sprite_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.sprite_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.sprite_components[i].parent_entity)
        {
            core->world_data->entity_components.sprite_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.sprite_component_free_id)
        core->world_data->entity_components.sprite_component_free_id = deleted_component_id;
}

internal AnimationComponent *AddAnimationComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.animation_component_free_id == core->world_data->entity_components.animation_component_count)
    {
        component_id = core->world_data->entity_components.animation_component_count;
        core->world_data->entity_components.animation_component_count++;
        core->world_data->entity_components.animation_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.animation_component_free_id;
    }

    core->world_data->entity_components.animation_components[component_id] = GetDefaultAnimationComponent();
    entity->components[COMPONENT_animation] = &core->world_data->entity_components.animation_components[component_id];
    core->world_data->entity_components.animation_components[component_id].parent_entity = entity;
    core->world_data->entity_components.animation_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.animation_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.animation_components[i].parent_entity)
        {
            core->world_data->entity_components.animation_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.animation_component_free_id)
        core->world_data->entity_components.animation_component_free_id = deleted_component_id;
}

internal PhysicsBodyComponent *AddPhysicsBodyComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.physics_body_component_free_id == core->world_data->entity_components.physics_body_component_count)
    {
        component_id = core->world_data->entity_components.physics_body_component_count;
        core->world_data->entity_components.physics_body_component_count++;
        core->world_data->entity_components.physics_body_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.physics_body_component_free_id;
    }

    core->world_data->entity_components.physics_body_components[component_id] = GetDefaultPhysicsBodyComponent();
    entity->components[COMPONENT_physics_body] = &core->world_data->entity_components.physics_body_components[component_id];
    core->world_data->entity_components.physics_body_components[component_id].parent_entity = entity;
    core->world_data->entity_components.physics_body_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.physics_body_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.physics_body_components[i].parent_entity)
        {
            core->world_data->entity_components.physics_body_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.physics_body_component_free_id)
        core->world_data->entity_components.physics_body_component_free_id = deleted_component_id;
}

internal MovementComponent *AddMovementComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.movement_component_free_id == core->world_data->entity_components.movement_component_count)
    {
        component_id = core->world_data->entity_components.movement_component_count;
        core->world_data->entity_components.movement_component_count++;
        core->world_data->entity_components.movement_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.movement_component_free_id;
    }

    core->world_data->entity_components.movement_components[component_id] = GetDefaultMovementComponent();
    entity->components[COMPONENT_movement] = &core->world_data->entity_components.movement_components[component_id];
    core->world_data->entity_components.movement_components[component_id].parent_entity = entity;
    core->world_data->entity_components.movement_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.movement_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.movement_components[i].parent_entity)
        {
            core->world_data->entity_components.movement_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.movement_component_free_id)
        core->world_data->entity_components.movement_component_free_id = deleted_component_id;
}

internal ArcEntityComponent *AddArcEntityComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.arc_entity_component_free_id == core->world_data->entity_components.arc_entity_component_count)
    {
        component_id = core->world_data->entity_components.arc_entity_component_count;
        core->world_data->entity_components.arc_entity_component_count++;
        core->world_data->entity_components.arc_entity_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.arc_entity_component_free_id;
    }

    core->world_data->entity_components.arc_entity_components[component_id] = GetDefaultArcEntityComponent();
    entity->components[COMPONENT_arc_entity] = &core->world_data->entity_components.arc_entity_components[component_id];
    core->world_data->entity_components.arc_entity_components[component_id].parent_entity = entity;
    core->world_data->entity_components.arc_entity_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.arc_entity_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.arc_entity_components[i].parent_entity)
        {
            core->world_data->entity_components.arc_entity_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.arc_entity_component_free_id)
        core->world_data->entity_components.arc_entity_component_free_id = deleted_component_id;
}

internal ItemComponent *AddItemComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.item_component_free_id == core->world_data->entity_components.item_component_count)
    {
        component_id = core->world_data->entity_components.item_component_count;
        core->world_data->entity_components.item_component_count++;
        core->world_data->entity_components.item_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.item_component_free_id;
    }

    core->world_data->entity_components.item_components[component_id] = GetDefaultItemComponent();
    entity->components[COMPONENT_item] = &core->world_data->entity_components.item_components[component_id];
    core->world_data->entity_components.item_components[component_id].parent_entity = entity;
    core->world_data->entity_components.item_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.item_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.item_components[i].parent_entity)
        {
            core->world_data->entity_components.item_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.item_component_free_id)
        core->world_data->entity_components.item_component_free_id = deleted_component_id;
}

internal TriggerComponent *AddTriggerComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.trigger_component_free_id == core->world_data->entity_components.trigger_component_count)
    {
        component_id = core->world_data->entity_components.trigger_component_count;
        core->world_data->entity_components.trigger_component_count++;
        core->world_data->entity_components.trigger_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.trigger_component_free_id;
    }

    core->world_data->entity_components.trigger_components[component_id] = GetDefaultTriggerComponent();
    entity->components[COMPONENT_trigger] = &core->world_data->entity_components.trigger_components[component_id];
    core->world_data->entity_components.trigger_components[component_id].parent_entity = entity;
    core->world_data->entity_components.trigger_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.trigger_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.trigger_components[i].parent_entity)
        {
            core->world_data->entity_components.trigger_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.trigger_component_free_id)
        core->world_data->entity_components.trigger_component_free_id = deleted_component_id;
}

internal StorageComponent *AddStorageComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.storage_component_free_id == core->world_data->entity_components.storage_component_count)
    {
        component_id = core->world_data->entity_components.storage_component_count;
        core->world_data->entity_components.storage_component_count++;
        core->world_data->entity_components.storage_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.storage_component_free_id;
    }

    core->world_data->entity_components.storage_components[component_id] = GetDefaultStorageComponent();
    entity->components[COMPONENT_storage] = &core->world_data->entity_components.storage_components[component_id];
    core->world_data->entity_components.storage_components[component_id].parent_entity = entity;
    core->world_data->entity_components.storage_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.storage_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.storage_components[i].parent_entity)
        {
            core->world_data->entity_components.storage_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.storage_component_free_id)
        core->world_data->entity_components.storage_component_free_id = deleted_component_id;
}

internal ParallaxComponent *AddParallaxComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.parallax_component_free_id == core->world_data->entity_components.parallax_component_count)
    {
        component_id = core->world_data->entity_components.parallax_component_count;
        core->world_data->entity_components.parallax_component_count++;
        core->world_data->entity_components.parallax_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.parallax_component_free_id;
    }

    core->world_data->entity_components.parallax_components[component_id] = GetDefaultParallaxComponent();
    entity->components[COMPONENT_parallax] = &core->world_data->entity_components.parallax_components[component_id];
    core->world_data->entity_components.parallax_components[component_id].parent_entity = entity;
    core->world_data->entity_components.parallax_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.parallax_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.parallax_components[i].parent_entity)
        {
            core->world_data->entity_components.parallax_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.parallax_component_free_id)
        core->world_data->entity_components.parallax_component_free_id = deleted_component_id;
}

internal ParticleEmitterComponent *AddParticleEmitterComponent(Entity *entity)
{
    i32 component_id;
    if (core->world_data->entity_components.particle_emitter_component_free_id == core->world_data->entity_components.particle_emitter_component_count)
    {
        component_id = core->world_data->entity_components.particle_emitter_component_count;
        core->world_data->entity_components.particle_emitter_component_count++;
        core->world_data->entity_components.particle_emitter_component_free_id = component_id + 1;
    }
    else
    {
        component_id = core->world_data->entity_components.particle_emitter_component_free_id;
    }

    core->world_data->entity_components.particle_emitter_components[component_id] = GetDefaultParticleEmitterComponent();
    entity->components[COMPONENT_particle_emitter] = &core->world_data->entity_components.particle_emitter_components[component_id];
    core->world_data->entity_components.particle_emitter_components[component_id].parent_entity = entity;
    core->world_data->entity_components.particle_emitter_components[component_id].component_id = component_id;

    for (i32 i = 0; i < core->world_data->entity_components.particle_emitter_component_count + 1; i++)
    {
        if (!core->world_data->entity_components.particle_emitter_components[i].parent_entity)
        {
            core->world_data->entity_components.particle_emitter_component_free_id = i;
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

    if (deleted_component_id < core->world_data->entity_components.particle_emitter_component_free_id)
        core->world_data->entity_components.particle_emitter_component_free_id = deleted_component_id;
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
    Entity *generic_entity = NewEntity("CharacterEntity", ENTITY_TYPE_character_entity, GENERALISED_ENTITY_TYPE_character);
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
    R_DEV_ASSERT(core->world_data->free_cloud_entity_index + 1 < MAX_CLOUD_ENTITY_COUNT, "Maximum amount of CloudEntity entites reached");

    i32 new_unique_id = core->world_data->free_cloud_entity_index;
    if (core->world_data->free_cloud_entity_index == core->world_data->cloud_entity_count)
    {
        core->world_data->cloud_entity_count++;
        core->world_data->free_cloud_entity_index++;
    }
    core->world_data->cloud_entity_list[new_unique_id].unique_entity_id = new_unique_id;

    Entity *generic_entity = NewEntity("CloudEntity", ENTITY_TYPE_cloud_entity, GENERALISED_ENTITY_TYPE_scenic);
    CloudEntity *unique_entity = &core->world_data->cloud_entity_list[new_unique_id];
    generic_entity->unique_entity = unique_entity;
    unique_entity->parent_generic_entity = generic_entity;
    unique_entity->unique_entity_id = new_unique_id;

    unique_entity->position_comp = AddPositionComponent(generic_entity);
    unique_entity->sprite_comp = AddSpriteComponent(generic_entity);
    unique_entity->parallax_comp = AddParallaxComponent(generic_entity);

    return unique_entity;
}

static void DeleteCloudEntity(CloudEntity *entity)
{
    DeleteEntity(entity->parent_generic_entity);
    if (entity->unique_entity_id < core->world_data->free_cloud_entity_index);
        core->world_data->free_cloud_entity_index = entity->unique_entity_id;
    CloudEntity empty_entity = {0};
    *entity = empty_entity;
}

static GroundSegmentEntity *NewGroundSegmentEntity()
{
    R_DEV_ASSERT(core->world_data->free_ground_segment_entity_index + 1 < MAX_GROUND_SEGMENT_ENTITY_COUNT, "Maximum amount of GroundSegmentEntity entites reached");

    i32 new_unique_id = core->world_data->free_ground_segment_entity_index;
    if (core->world_data->free_ground_segment_entity_index == core->world_data->ground_segment_entity_count)
    {
        core->world_data->ground_segment_entity_count++;
        core->world_data->free_ground_segment_entity_index++;
    }
    core->world_data->ground_segment_entity_list[new_unique_id].unique_entity_id = new_unique_id;

    Entity *generic_entity = NewEntity("GroundSegmentEntity", ENTITY_TYPE_ground_segment_entity, GENERALISED_ENTITY_TYPE_ground);
    GroundSegmentEntity *unique_entity = &core->world_data->ground_segment_entity_list[new_unique_id];
    generic_entity->unique_entity = unique_entity;
    unique_entity->parent_generic_entity = generic_entity;
    unique_entity->unique_entity_id = new_unique_id;

    unique_entity->position_comp = AddPositionComponent(generic_entity);
    unique_entity->physics_body_comp = AddPhysicsBodyComponent(generic_entity);

    return unique_entity;
}

static void DeleteGroundSegmentEntity(GroundSegmentEntity *entity)
{
    DeleteEntity(entity->parent_generic_entity);
    if (entity->unique_entity_id < core->world_data->free_ground_segment_entity_index);
        core->world_data->free_ground_segment_entity_index = entity->unique_entity_id;
    GroundSegmentEntity empty_entity = {0};
    *entity = empty_entity;
}

static void PrintEntityDataUI(Entity *entity)
{
    switch(entity->type)
    {
    case ENTITY_TYPE_character_entity :
    {
        CharacterEntity *unique_entity = entity->unique_entity;
            // TODO: Don't know how to generate UI print for variable 'parent_generic_entity'
            // TODO: Don't know how to generate UI print for variable 'position_comp'
            // TODO: Don't know how to generate UI print for variable 'sprite_comp'
            // TODO: Don't know how to generate UI print for variable 'animation_comp'
            // TODO: Don't know how to generate UI print for variable 'physics_body_comp'
            // TODO: Don't know how to generate UI print for variable 'movement_comp'
            // TODO: Don't know how to generate UI print for variable 'arc_entity_comp'
        break;
    }
    case ENTITY_TYPE_cloud_entity :
    {
        CloudEntity *unique_entity = entity->unique_entity;
            // TODO: Don't know how to generate UI print for variable 'parent_generic_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "unique_entity_id: %i", unique_entity->unique_entity_id); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'position_comp'
            // TODO: Don't know how to generate UI print for variable 'sprite_comp'
            // TODO: Don't know how to generate UI print for variable 'parallax_comp'
        break;
    }
    case ENTITY_TYPE_ground_segment_entity :
    {
        GroundSegmentEntity *unique_entity = entity->unique_entity;
            // TODO: Don't know how to generate UI print for variable 'parent_generic_entity'
            TsUIPushAutoWidth();
            { char label[100]; sprintf(label, "unique_entity_id: %i", unique_entity->unique_entity_id); TsUILabel(label); }
            TsUIPopWidth();
            // TODO: Don't know how to generate UI print for variable 'position_comp'
            // TODO: Don't know how to generate UI print for variable 'physics_body_comp'
        break;
    }
    }

    for (i32 i = 1; i < COMPONENT_MAX; i++)
    {
        if (entity->components[i])
        {
            PrintComponentDataUI(entity->components[i], i);
        }
    }
}
static void WritePositionComponentToFile(FILE *file, PositionComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->position, sizeof(data->position));
}

static void FillPositionComponentPointersInFile(FILE *file, PositionComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->position)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->position), SEEK_CUR);
}

static void WriteSpriteComponentToFile(FILE *file, SpriteComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->sprite_data, sizeof(data->sprite_data));
    WriteToFile(file, &data->is_flipped, sizeof(data->is_flipped));
    WriteToFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));
}

static void FillSpriteComponentPointersInFile(FILE *file, SpriteComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->sprite_data)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->sprite_data), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_flipped)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_flipped), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_background_sprite)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_background_sprite), SEEK_CUR);
}

static void WriteAnimationComponentToFile(FILE *file, AnimationComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->flags, sizeof(data->flags));
    WriteToFile(file, &data->current_frame, sizeof(data->current_frame));
    WriteToFile(file, &data->interval_mult, sizeof(data->interval_mult));
    WriteToFile(file, &data->frame_start_time, sizeof(data->frame_start_time));
}

static void FillAnimationComponentPointersInFile(FILE *file, AnimationComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->current_frame)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->current_frame), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->interval_mult)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->interval_mult), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->frame_start_time)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->frame_start_time), SEEK_CUR);
}

static void WritePhysicsBodyComponentToFile(FILE *file, PhysicsBodyComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
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
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->shape)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->shape), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->shape_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->shape_type), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->material)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->material), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->mass_data)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->mass_data), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->velocity)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->velocity), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->force)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->force), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->gravity_multiplier)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->gravity_multiplier), SEEK_CUR);
}

static void WriteMovementComponentToFile(FILE *file, MovementComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->axis_x, sizeof(data->axis_x));
    WriteToFile(file, &data->move_speed, sizeof(data->move_speed));
    WriteToFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));
}

static void FillMovementComponentPointersInFile(FILE *file, MovementComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->axis_x)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->axis_x), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->move_speed)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->move_speed), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->move_speed_mult)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->move_speed_mult), SEEK_CUR);
}

static void WriteArcEntityComponentToFile(FILE *file, ArcEntityComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->entity_type, sizeof(data->entity_type));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->current_general_state, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->current_animation_state, sizeof(data->current_animation_state));
}

static void FillArcEntityComponentPointersInFile(FILE *file, ArcEntityComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_type), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->current_animation_state)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->current_animation_state), SEEK_CUR);
}

static void WriteItemComponentToFile(FILE *file, ItemComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->item_type, sizeof(data->item_type));
    WriteToFile(file, &data->stack_size, sizeof(data->stack_size));
}

static void FillItemComponentPointersInFile(FILE *file, ItemComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->item_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->item_type), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->stack_size)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->stack_size), SEEK_CUR);
}

static void WriteTriggerComponentToFile(FILE *file, TriggerComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->enter_trigger_callback, sizeof(data->enter_trigger_callback));
    WriteToFile(file, &data->exit_trigger_callback, sizeof(data->exit_trigger_callback));
    WriteToFile(file, &data->previous_overlaps_count, sizeof(data->previous_overlaps_count));
    WriteToFile(file, &data->trigger_against, sizeof(data->trigger_against));
}

static void FillTriggerComponentPointersInFile(FILE *file, TriggerComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->enter_trigger_callback)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->enter_trigger_callback), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->exit_trigger_callback)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->exit_trigger_callback), SEEK_CUR);
// - Arary MAX_OVERLAPPING_COLLIDERS
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->previous_overlaps_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->previous_overlaps_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->trigger_against)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->trigger_against), SEEK_CUR);
}

static void WriteStorageComponentToFile(FILE *file, StorageComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->storage_size, sizeof(data->storage_size));
}

static void FillStorageComponentPointersInFile(FILE *file, StorageComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->storage_size)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->storage_size), SEEK_CUR);
// - Arary MAX_STORAGE_SIZE
}

static void WriteParallaxComponentToFile(FILE *file, ParallaxComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->parallax_amount, sizeof(data->parallax_amount));
    WriteToFile(file, &data->desired_position, sizeof(data->desired_position));
}

static void FillParallaxComponentPointersInFile(FILE *file, ParallaxComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->parallax_amount)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->parallax_amount), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->desired_position)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->desired_position), SEEK_CUR);
}

static void WriteParticleEmitterComponentToFile(FILE *file, ParticleEmitterComponent *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->component_id, sizeof(data->component_id));
    WriteToFile(file, &data->life_time, sizeof(data->life_time));
    WriteToFile(file, &data->start_time, sizeof(data->start_time));
    WriteToFile(file, &data->flags, sizeof(data->flags));
    WriteToFile(file, &data->particle_count, sizeof(data->particle_count));
    WriteToFile(file, &data->free_particle_index, sizeof(data->free_particle_index));
    WriteToFile(file, &data->begin_callback, sizeof(data->begin_callback));
    WriteToFile(file, &data->finish_callback, sizeof(data->finish_callback));
}

static void FillParticleEmitterComponentPointersInFile(FILE *file, ParticleEmitterComponent *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->component_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->component_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->life_time)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->life_time), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->start_time)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->start_time), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);
// - Arary MAX_PARTICLE_AMOUNT
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->particle_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->particle_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_particle_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_particle_index), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->begin_callback)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->begin_callback), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->finish_callback)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->finish_callback), SEEK_CUR);
}

static void WriteComponentSetToFile(FILE *file, ComponentSet *data)
{
    WriteToFile(file, &data->position_component_count, sizeof(data->position_component_count));
    WriteToFile(file, &data->position_component_free_id, sizeof(data->position_component_free_id));
    WriteToFile(file, &data->sprite_component_count, sizeof(data->sprite_component_count));
    WriteToFile(file, &data->sprite_component_free_id, sizeof(data->sprite_component_free_id));
    WriteToFile(file, &data->animation_component_count, sizeof(data->animation_component_count));
    WriteToFile(file, &data->animation_component_free_id, sizeof(data->animation_component_free_id));
    WriteToFile(file, &data->physics_body_component_count, sizeof(data->physics_body_component_count));
    WriteToFile(file, &data->physics_body_component_free_id, sizeof(data->physics_body_component_free_id));
    WriteToFile(file, &data->movement_component_count, sizeof(data->movement_component_count));
    WriteToFile(file, &data->movement_component_free_id, sizeof(data->movement_component_free_id));
    WriteToFile(file, &data->arc_entity_component_count, sizeof(data->arc_entity_component_count));
    WriteToFile(file, &data->arc_entity_component_free_id, sizeof(data->arc_entity_component_free_id));
    WriteToFile(file, &data->item_component_count, sizeof(data->item_component_count));
    WriteToFile(file, &data->item_component_free_id, sizeof(data->item_component_free_id));
    WriteToFile(file, &data->trigger_component_count, sizeof(data->trigger_component_count));
    WriteToFile(file, &data->trigger_component_free_id, sizeof(data->trigger_component_free_id));
    WriteToFile(file, &data->storage_component_count, sizeof(data->storage_component_count));
    WriteToFile(file, &data->storage_component_free_id, sizeof(data->storage_component_free_id));
    WriteToFile(file, &data->parallax_component_count, sizeof(data->parallax_component_count));
    WriteToFile(file, &data->parallax_component_free_id, sizeof(data->parallax_component_free_id));
    WriteToFile(file, &data->particle_emitter_component_count, sizeof(data->particle_emitter_component_count));
    WriteToFile(file, &data->particle_emitter_component_free_id, sizeof(data->particle_emitter_component_free_id));
}

static void FillComponentSetPointersInFile(FILE *file, ComponentSet *data)
{
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->position_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->position_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->position_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->position_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->sprite_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->sprite_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->sprite_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->sprite_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->animation_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->animation_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->animation_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->animation_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->physics_body_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->physics_body_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->physics_body_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->physics_body_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->movement_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->movement_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->movement_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->movement_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->arc_entity_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->arc_entity_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->arc_entity_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->arc_entity_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->item_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->item_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->item_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->item_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->trigger_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->trigger_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->trigger_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->trigger_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->storage_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->storage_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->storage_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->storage_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->parallax_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->parallax_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->parallax_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->parallax_component_free_id), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->particle_emitter_component_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->particle_emitter_component_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->particle_emitter_component_free_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->particle_emitter_component_free_id), SEEK_CUR);
}

static void WriteCharacterEntityToFile(FILE *file, CharacterEntity *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_generic_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->position_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->sprite_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->animation_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->physics_body_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->movement_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->arc_entity_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillCharacterEntityPointersInFile(FILE *file, CharacterEntity *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
}

static void WriteCloudEntityToFile(FILE *file, CloudEntity *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_generic_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->unique_entity_id, sizeof(data->unique_entity_id));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->position_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->sprite_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parallax_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillCloudEntityPointersInFile(FILE *file, CloudEntity *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->unique_entity_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->unique_entity_id), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
}

static void WriteGroundSegmentEntityToFile(FILE *file, GroundSegmentEntity *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_generic_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->unique_entity_id, sizeof(data->unique_entity_id));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->position_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->physics_body_comp, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillGroundSegmentEntityPointersInFile(FILE *file, GroundSegmentEntity *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->unique_entity_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->unique_entity_id), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
}

static void WriteEntityToFile(FILE *file, Entity *data)
{
    WriteToFile(file, &data->entity_id, sizeof(data->entity_id));
    WriteToFile(file, &data->generalised_type, sizeof(data->generalised_type));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->unique_entity, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->type, sizeof(data->type));
    WriteToFile(file, &data->flags, sizeof(data->flags));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->active_chunk, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillEntityPointersInFile(FILE *file, Entity *data)
{
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_id), SEEK_CUR);
// - Arary 20
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->generalised_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->generalised_type), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->type), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);
// - Arary COMPONENT_MAX
    fseek(file, sizeof(i32), SEEK_CUR);
}

static void WriteCellMaterialToFile(FILE *file, CellMaterial *data)
{
    WriteToFile(file, &data->id, sizeof(data->id));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_cell, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->material_type, sizeof(data->material_type));
    WriteToFile(file, &data->flags, sizeof(data->flags));
    WriteToFile(file, &data->mass, sizeof(data->mass));
    WriteToFile(file, &data->properties_type, sizeof(data->properties_type));
    WriteToFile(file, &data->properties, sizeof(data->properties));
    WriteToFile(file, &data->is_material_dynamic, sizeof(data->is_material_dynamic));
    WriteToFile(file, &data->has_been_updated, sizeof(data->has_been_updated));
    WriteToFile(file, &data->idle_start, sizeof(data->idle_start));
}

static void FillCellMaterialPointersInFile(FILE *file, CellMaterial *data)
{
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->id), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->material_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->material_type), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->flags)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->flags), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->mass)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->mass), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->properties_type)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->properties_type), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->properties)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->properties), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_material_dynamic)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_material_dynamic), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->has_been_updated)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->has_been_updated), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->idle_start)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->idle_start), SEEK_CUR);
}

static void WriteCellToFile(FILE *file, Cell *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_cell_chunk, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->x_index, sizeof(data->x_index));
    WriteToFile(file, &data->y_index, sizeof(data->y_index));
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->material, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillCellPointersInFile(FILE *file, Cell *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->x_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->x_index), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->y_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->y_index), SEEK_CUR);
    fseek(file, sizeof(i32), SEEK_CUR);
}

static void WriteCellChunkToFile(FILE *file, CellChunk *data)
{
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->parent_chunk, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
    WriteToFile(file, &data->x_index, sizeof(data->x_index));
    WriteToFile(file, &data->y_index, sizeof(data->y_index));
// - 2D Arary CELL_CHUNK_SIZE CELL_CHUNK_SIZE
    WriteToFile(file, &data->texture, sizeof(data->texture));
}

static void FillCellChunkPointersInFile(FILE *file, CellChunk *data)
{
    fseek(file, sizeof(i32), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->x_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->x_index), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->y_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->y_index), SEEK_CUR);
// - 2D Arary CELL_CHUNK_SIZE CELL_CHUNK_SIZE
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->texture)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->texture), SEEK_CUR);
}

static void WriteChunkDataToFile(FILE *file, ChunkData *data)
{
    WriteToFile(file, &data->is_valid, sizeof(data->is_valid));
    WriteToFile(file, &data->entity_count, sizeof(data->entity_count));
    WriteToFile(file, &data->x_index, sizeof(data->x_index));
    WriteToFile(file, &data->y_index, sizeof(data->y_index));
// - 2D Arary CELL_CHUNKS_IN_CHUNK CELL_CHUNKS_IN_CHUNK
    WriteToFile(file, &data->cell_material_count, sizeof(data->cell_material_count));
    WriteToFile(file, &data->free_cell_material_id, sizeof(data->free_cell_material_id));
    WriteToFile(file, &data->dynamic_cell_material_count, sizeof(data->dynamic_cell_material_count));
}

static void FillChunkDataPointersInFile(FILE *file, ChunkData *data)
{
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->is_valid)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->is_valid), SEEK_CUR);
// - Arary MAX_ENTITIES_PER_CHUNK
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->x_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->x_index), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->y_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->y_index), SEEK_CUR);
// - 2D Arary CELL_CHUNKS_IN_CHUNK CELL_CHUNKS_IN_CHUNK
// - Arary CHUNK_AREA
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->cell_material_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->cell_material_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_cell_material_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_cell_material_id), SEEK_CUR);
// - Arary MAX_DYNAMIC_CELLS
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->dynamic_cell_material_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->dynamic_cell_material_count), SEEK_CUR);
}

static void WriteWorldDataToFile(FILE *file, WorldData *data)
{
    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));
    WriteToFile(file, &data->active_chunk_count, sizeof(data->active_chunk_count));
    WriteChunkDataToFile(file, &data->floating_chunk);
    WriteCharacterEntityToFile(file, &data->character_entity);
    WriteToFile(file, &data->cloud_entity_count, sizeof(data->cloud_entity_count));
    WriteToFile(file, &data->free_cloud_entity_index, sizeof(data->free_cloud_entity_index));
    WriteToFile(file, &data->ground_segment_entity_count, sizeof(data->ground_segment_entity_count));
    WriteToFile(file, &data->free_ground_segment_entity_index, sizeof(data->free_ground_segment_entity_index));
    WriteToFile(file, &data->entity_count, sizeof(data->entity_count));
    WriteToFile(file, &data->free_entity_id, sizeof(data->free_entity_id));
    WriteComponentSetToFile(file, &data->entity_components);
    {
        i32 pos = ftell(file);
        SerialisationPointer ptr = {&data->test_ptr, pos};
        serialisation_pointers[serialisation_pointer_count++] = ptr;
        i32 empty = 255;
        WriteToFile(file, &empty, sizeof(i32));
    }
}

static void FillWorldDataPointersInFile(FILE *file, WorldData *data)
{
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->elapsed_world_time)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->elapsed_world_time), SEEK_CUR);
// - Arary MAX_WORLD_CHUNKS
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->active_chunk_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->active_chunk_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->floating_chunk)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    FillChunkDataPointersInFile(file, &data->floating_chunk);

    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->character_entity)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    FillCharacterEntityPointersInFile(file, &data->character_entity);

// - Arary MAX_CLOUD_ENTITY_COUNT
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->cloud_entity_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->cloud_entity_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_cloud_entity_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_cloud_entity_index), SEEK_CUR);
// - Arary MAX_GROUND_SEGMENT_ENTITY_COUNT
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->ground_segment_entity_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->ground_segment_entity_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_ground_segment_entity_index)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_ground_segment_entity_index), SEEK_CUR);
// - Arary MAX_ACTIVE_ENTITIES
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_count)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->entity_count), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->free_entity_id)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    fseek(file, sizeof(data->free_entity_id), SEEK_CUR);
    for (i32 i = 0; i < serialisation_pointer_count; i++)
    {
        SerialisationPointer *ptr = &serialisation_pointers[i];
        if (*ptr->pointer_address == &data->entity_components)
        {
            i32 current_pos = ftell(file);
            fseek(file, ptr->offset, SEEK_SET);
            WriteToFile(file, &current_pos, sizeof(i32));
            fseek(file, current_pos, SEEK_SET);
        }
    }
    FillComponentSetPointersInFile(file, &data->entity_components);

    fseek(file, sizeof(i32), SEEK_CUR);
}

