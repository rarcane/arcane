static char *GetEntityTypeName(EntityType type)
{
switch(type)
{
case ENTITY_TYPE_undefined:
return "Undefined";
break;
case ENTITY_TYPE_character:
return "Character";
break;
case ENTITY_TYPE_monster:
return "Monster";
break;
case ENTITY_TYPE_animal:
return "Animal";
break;
case ENTITY_TYPE_item:
return "Item";
break;
case ENTITY_TYPE_storage:
return "Storage";
break;
case ENTITY_TYPE_resource:
return "Resource";
break;
case ENTITY_TYPE_scenic:
return "Scenic";
break;
case ENTITY_TYPE_ground:
return "Ground";
break;
default:
return "INVALID";
break;
}
}

internal void AddPositionComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->position_free_component_id == core->component_set->position_component_count)
    {
        component_id = core->component_set->position_component_count;
        core->component_set->position_component_count++;
        core->component_set->position_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->position_free_component_id;
        for (int i = 1; i < core->component_set->position_component_count + 1; i++)
        {
            if (core->component_set->position_components[i].component_id == 0)
            {
                core->component_set->position_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->position_components[component_id] = *((PositionComponent*)component_data);
    entity->components[COMPONENT_position] = &core->component_set->position_components[component_id];
    core->component_set->position_components[component_id].entity_id = entity->entity_id;
    core->component_set->position_components[component_id].component_id = component_id;
}

internal void RemovePositionComponent(Entity *entity)
{
    PositionComponent *component = entity->components[COMPONENT_position];
    R_DEV_ASSERT(component, "Entity does not a PositionComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    PositionComponent empty_comp = {0};
    core->component_set->position_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_position] = 0;

    if (deleted_component_id < core->component_set->position_free_component_id)
        core->component_set->position_free_component_id = deleted_component_id;
}

internal void AddSpriteComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->sprite_free_component_id == core->component_set->sprite_component_count)
    {
        component_id = core->component_set->sprite_component_count;
        core->component_set->sprite_component_count++;
        core->component_set->sprite_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->sprite_free_component_id;
        for (int i = 1; i < core->component_set->sprite_component_count + 1; i++)
        {
            if (core->component_set->sprite_components[i].component_id == 0)
            {
                core->component_set->sprite_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->sprite_components[component_id] = *((SpriteComponent*)component_data);
    entity->components[COMPONENT_sprite] = &core->component_set->sprite_components[component_id];
    core->component_set->sprite_components[component_id].entity_id = entity->entity_id;
    core->component_set->sprite_components[component_id].component_id = component_id;
}

internal void RemoveSpriteComponent(Entity *entity)
{
    SpriteComponent *component = entity->components[COMPONENT_sprite];
    R_DEV_ASSERT(component, "Entity does not a SpriteComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    SpriteComponent empty_comp = {0};
    core->component_set->sprite_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_sprite] = 0;

    if (deleted_component_id < core->component_set->sprite_free_component_id)
        core->component_set->sprite_free_component_id = deleted_component_id;
}

internal void AddSubSpriteComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->sub_sprite_free_component_id == core->component_set->sub_sprite_component_count)
    {
        component_id = core->component_set->sub_sprite_component_count;
        core->component_set->sub_sprite_component_count++;
        core->component_set->sub_sprite_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->sub_sprite_free_component_id;
        for (int i = 1; i < core->component_set->sub_sprite_component_count + 1; i++)
        {
            if (core->component_set->sub_sprite_components[i].component_id == 0)
            {
                core->component_set->sub_sprite_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->sub_sprite_components[component_id] = *((SubSpriteComponent*)component_data);
    entity->components[COMPONENT_sub_sprite] = &core->component_set->sub_sprite_components[component_id];
    core->component_set->sub_sprite_components[component_id].entity_id = entity->entity_id;
    core->component_set->sub_sprite_components[component_id].component_id = component_id;
}

internal void RemoveSubSpriteComponent(Entity *entity)
{
    SubSpriteComponent *component = entity->components[COMPONENT_sub_sprite];
    R_DEV_ASSERT(component, "Entity does not a SubSpriteComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    SubSpriteComponent empty_comp = {0};
    core->component_set->sub_sprite_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_sub_sprite] = 0;

    if (deleted_component_id < core->component_set->sub_sprite_free_component_id)
        core->component_set->sub_sprite_free_component_id = deleted_component_id;
}

internal void AddAnimationComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->animation_free_component_id == core->component_set->animation_component_count)
    {
        component_id = core->component_set->animation_component_count;
        core->component_set->animation_component_count++;
        core->component_set->animation_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->animation_free_component_id;
        for (int i = 1; i < core->component_set->animation_component_count + 1; i++)
        {
            if (core->component_set->animation_components[i].component_id == 0)
            {
                core->component_set->animation_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->animation_components[component_id] = *((AnimationComponent*)component_data);
    entity->components[COMPONENT_animation] = &core->component_set->animation_components[component_id];
    core->component_set->animation_components[component_id].entity_id = entity->entity_id;
    core->component_set->animation_components[component_id].component_id = component_id;
}

internal void RemoveAnimationComponent(Entity *entity)
{
    AnimationComponent *component = entity->components[COMPONENT_animation];
    R_DEV_ASSERT(component, "Entity does not a AnimationComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    AnimationComponent empty_comp = {0};
    core->component_set->animation_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_animation] = 0;

    if (deleted_component_id < core->component_set->animation_free_component_id)
        core->component_set->animation_free_component_id = deleted_component_id;
}

internal void AddColliderComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->collider_free_component_id == core->component_set->collider_component_count)
    {
        component_id = core->component_set->collider_component_count;
        core->component_set->collider_component_count++;
        core->component_set->collider_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->collider_free_component_id;
        for (int i = 1; i < core->component_set->collider_component_count + 1; i++)
        {
            if (core->component_set->collider_components[i].component_id == 0)
            {
                core->component_set->collider_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->collider_components[component_id] = *((ColliderComponent*)component_data);
    entity->components[COMPONENT_collider] = &core->component_set->collider_components[component_id];
    core->component_set->collider_components[component_id].entity_id = entity->entity_id;
    core->component_set->collider_components[component_id].component_id = component_id;
}

internal void RemoveColliderComponent(Entity *entity)
{
    ColliderComponent *component = entity->components[COMPONENT_collider];
    R_DEV_ASSERT(component, "Entity does not a ColliderComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ColliderComponent empty_comp = {0};
    core->component_set->collider_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_collider] = 0;

    if (deleted_component_id < core->component_set->collider_free_component_id)
        core->component_set->collider_free_component_id = deleted_component_id;
}

internal void AddVelocityComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->velocity_free_component_id == core->component_set->velocity_component_count)
    {
        component_id = core->component_set->velocity_component_count;
        core->component_set->velocity_component_count++;
        core->component_set->velocity_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->velocity_free_component_id;
        for (int i = 1; i < core->component_set->velocity_component_count + 1; i++)
        {
            if (core->component_set->velocity_components[i].component_id == 0)
            {
                core->component_set->velocity_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->velocity_components[component_id] = *((VelocityComponent*)component_data);
    entity->components[COMPONENT_velocity] = &core->component_set->velocity_components[component_id];
    core->component_set->velocity_components[component_id].entity_id = entity->entity_id;
    core->component_set->velocity_components[component_id].component_id = component_id;
}

internal void RemoveVelocityComponent(Entity *entity)
{
    VelocityComponent *component = entity->components[COMPONENT_velocity];
    R_DEV_ASSERT(component, "Entity does not a VelocityComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    VelocityComponent empty_comp = {0};
    core->component_set->velocity_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_velocity] = 0;

    if (deleted_component_id < core->component_set->velocity_free_component_id)
        core->component_set->velocity_free_component_id = deleted_component_id;
}

internal void AddPhysicsComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->physics_free_component_id == core->component_set->physics_component_count)
    {
        component_id = core->component_set->physics_component_count;
        core->component_set->physics_component_count++;
        core->component_set->physics_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->physics_free_component_id;
        for (int i = 1; i < core->component_set->physics_component_count + 1; i++)
        {
            if (core->component_set->physics_components[i].component_id == 0)
            {
                core->component_set->physics_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->physics_components[component_id] = *((PhysicsComponent*)component_data);
    entity->components[COMPONENT_physics] = &core->component_set->physics_components[component_id];
    core->component_set->physics_components[component_id].entity_id = entity->entity_id;
    core->component_set->physics_components[component_id].component_id = component_id;
}

internal void RemovePhysicsComponent(Entity *entity)
{
    PhysicsComponent *component = entity->components[COMPONENT_physics];
    R_DEV_ASSERT(component, "Entity does not a PhysicsComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    PhysicsComponent empty_comp = {0};
    core->component_set->physics_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_physics] = 0;

    if (deleted_component_id < core->component_set->physics_free_component_id)
        core->component_set->physics_free_component_id = deleted_component_id;
}

internal void AddMovementComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->movement_free_component_id == core->component_set->movement_component_count)
    {
        component_id = core->component_set->movement_component_count;
        core->component_set->movement_component_count++;
        core->component_set->movement_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->movement_free_component_id;
        for (int i = 1; i < core->component_set->movement_component_count + 1; i++)
        {
            if (core->component_set->movement_components[i].component_id == 0)
            {
                core->component_set->movement_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->movement_components[component_id] = *((MovementComponent*)component_data);
    entity->components[COMPONENT_movement] = &core->component_set->movement_components[component_id];
    core->component_set->movement_components[component_id].entity_id = entity->entity_id;
    core->component_set->movement_components[component_id].component_id = component_id;
}

internal void RemoveMovementComponent(Entity *entity)
{
    MovementComponent *component = entity->components[COMPONENT_movement];
    R_DEV_ASSERT(component, "Entity does not a MovementComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    MovementComponent empty_comp = {0};
    core->component_set->movement_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_movement] = 0;

    if (deleted_component_id < core->component_set->movement_free_component_id)
        core->component_set->movement_free_component_id = deleted_component_id;
}

internal void AddArcEntityComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->arc_entity_free_component_id == core->component_set->arc_entity_component_count)
    {
        component_id = core->component_set->arc_entity_component_count;
        core->component_set->arc_entity_component_count++;
        core->component_set->arc_entity_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->arc_entity_free_component_id;
        for (int i = 1; i < core->component_set->arc_entity_component_count + 1; i++)
        {
            if (core->component_set->arc_entity_components[i].component_id == 0)
            {
                core->component_set->arc_entity_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->arc_entity_components[component_id] = *((ArcEntityComponent*)component_data);
    entity->components[COMPONENT_arc_entity] = &core->component_set->arc_entity_components[component_id];
    core->component_set->arc_entity_components[component_id].entity_id = entity->entity_id;
    core->component_set->arc_entity_components[component_id].component_id = component_id;
}

internal void RemoveArcEntityComponent(Entity *entity)
{
    ArcEntityComponent *component = entity->components[COMPONENT_arc_entity];
    R_DEV_ASSERT(component, "Entity does not a ArcEntityComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ArcEntityComponent empty_comp = {0};
    core->component_set->arc_entity_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_arc_entity] = 0;

    if (deleted_component_id < core->component_set->arc_entity_free_component_id)
        core->component_set->arc_entity_free_component_id = deleted_component_id;
}

internal void AddItemComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->item_free_component_id == core->component_set->item_component_count)
    {
        component_id = core->component_set->item_component_count;
        core->component_set->item_component_count++;
        core->component_set->item_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->item_free_component_id;
        for (int i = 1; i < core->component_set->item_component_count + 1; i++)
        {
            if (core->component_set->item_components[i].component_id == 0)
            {
                core->component_set->item_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->item_components[component_id] = *((ItemComponent*)component_data);
    entity->components[COMPONENT_item] = &core->component_set->item_components[component_id];
    core->component_set->item_components[component_id].entity_id = entity->entity_id;
    core->component_set->item_components[component_id].component_id = component_id;
}

internal void RemoveItemComponent(Entity *entity)
{
    ItemComponent *component = entity->components[COMPONENT_item];
    R_DEV_ASSERT(component, "Entity does not a ItemComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ItemComponent empty_comp = {0};
    core->component_set->item_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_item] = 0;

    if (deleted_component_id < core->component_set->item_free_component_id)
        core->component_set->item_free_component_id = deleted_component_id;
}

internal void AddTriggerComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->trigger_free_component_id == core->component_set->trigger_component_count)
    {
        component_id = core->component_set->trigger_component_count;
        core->component_set->trigger_component_count++;
        core->component_set->trigger_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->trigger_free_component_id;
        for (int i = 1; i < core->component_set->trigger_component_count + 1; i++)
        {
            if (core->component_set->trigger_components[i].component_id == 0)
            {
                core->component_set->trigger_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->trigger_components[component_id] = *((TriggerComponent*)component_data);
    entity->components[COMPONENT_trigger] = &core->component_set->trigger_components[component_id];
    core->component_set->trigger_components[component_id].entity_id = entity->entity_id;
    core->component_set->trigger_components[component_id].component_id = component_id;
}

internal void RemoveTriggerComponent(Entity *entity)
{
    TriggerComponent *component = entity->components[COMPONENT_trigger];
    R_DEV_ASSERT(component, "Entity does not a TriggerComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    TriggerComponent empty_comp = {0};
    core->component_set->trigger_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_trigger] = 0;

    if (deleted_component_id < core->component_set->trigger_free_component_id)
        core->component_set->trigger_free_component_id = deleted_component_id;
}

internal void AddStorageComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->storage_free_component_id == core->component_set->storage_component_count)
    {
        component_id = core->component_set->storage_component_count;
        core->component_set->storage_component_count++;
        core->component_set->storage_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->storage_free_component_id;
        for (int i = 1; i < core->component_set->storage_component_count + 1; i++)
        {
            if (core->component_set->storage_components[i].component_id == 0)
            {
                core->component_set->storage_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->storage_components[component_id] = *((StorageComponent*)component_data);
    entity->components[COMPONENT_storage] = &core->component_set->storage_components[component_id];
    core->component_set->storage_components[component_id].entity_id = entity->entity_id;
    core->component_set->storage_components[component_id].component_id = component_id;
}

internal void RemoveStorageComponent(Entity *entity)
{
    StorageComponent *component = entity->components[COMPONENT_storage];
    R_DEV_ASSERT(component, "Entity does not a StorageComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    StorageComponent empty_comp = {0};
    core->component_set->storage_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_storage] = 0;

    if (deleted_component_id < core->component_set->storage_free_component_id)
        core->component_set->storage_free_component_id = deleted_component_id;
}

internal void AddParallaxComponent(Entity *entity, void *component_data)
{
    i32 component_id;
    if (core->component_set->parallax_free_component_id == core->component_set->parallax_component_count)
    {
        component_id = core->component_set->parallax_component_count;
        core->component_set->parallax_component_count++;
        core->component_set->parallax_free_component_id = component_id + 1;
    }
    else
    {
        component_id = core->component_set->parallax_free_component_id;
        for (int i = 1; i < core->component_set->parallax_component_count + 1; i++)
        {
            if (core->component_set->parallax_components[i].component_id == 0)
            {
                core->component_set->parallax_free_component_id = i;
                break;
            }
        }
    }

    core->component_set->parallax_components[component_id] = *((ParallaxComponent*)component_data);
    entity->components[COMPONENT_parallax] = &core->component_set->parallax_components[component_id];
    core->component_set->parallax_components[component_id].entity_id = entity->entity_id;
    core->component_set->parallax_components[component_id].component_id = component_id;
}

internal void RemoveParallaxComponent(Entity *entity)
{
    ParallaxComponent *component = entity->components[COMPONENT_parallax];
    R_DEV_ASSERT(component, "Entity does not a ParallaxComponent attached, so it can't remove it.");

    i32 deleted_component_id = component->component_id;
    ParallaxComponent empty_comp = {0};
    core->component_set->parallax_components[deleted_component_id] = empty_comp;
    entity->components[COMPONENT_parallax] = 0;

    if (deleted_component_id < core->component_set->parallax_free_component_id)
        core->component_set->parallax_free_component_id = deleted_component_id;
}

internal void DeleteEntity(Entity *entity)
{
PositionComponent *position_component = entity->components[1];
if (position_component)
RemovePositionComponent(entity);
SpriteComponent *sprite_component = entity->components[2];
if (sprite_component)
RemoveSpriteComponent(entity);
SubSpriteComponent *sub_sprite_component = entity->components[3];
if (sub_sprite_component)
RemoveSubSpriteComponent(entity);
AnimationComponent *animation_component = entity->components[4];
if (animation_component)
RemoveAnimationComponent(entity);
ColliderComponent *collider_component = entity->components[5];
if (collider_component)
RemoveColliderComponent(entity);
VelocityComponent *velocity_component = entity->components[6];
if (velocity_component)
RemoveVelocityComponent(entity);
PhysicsComponent *physics_component = entity->components[7];
if (physics_component)
RemovePhysicsComponent(entity);
MovementComponent *movement_component = entity->components[8];
if (movement_component)
RemoveMovementComponent(entity);
ArcEntityComponent *arc_entity_component = entity->components[9];
if (arc_entity_component)
RemoveArcEntityComponent(entity);
ItemComponent *item_component = entity->components[10];
if (item_component)
RemoveItemComponent(entity);
TriggerComponent *trigger_component = entity->components[11];
if (trigger_component)
RemoveTriggerComponent(entity);
StorageComponent *storage_component = entity->components[12];
if (storage_component)
RemoveStorageComponent(entity);
ParallaxComponent *parallax_component = entity->components[13];
if (parallax_component)
RemoveParallaxComponent(entity);

i32 deleted_entity_id = entity->entity_id;
Entity empty_entity = {0};
*entity = empty_entity;
if (deleted_entity_id < core->entity_set->free_entity_id)
core->entity_set->free_entity_id = deleted_entity_id;
}
