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
case GENERALISED_ENTITY_TYPE_structure:
return "Structure";
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
case EDITOR_STATE_general:
return "General";
break;
case EDITOR_STATE_terrain:
return "Terrain";
break;
default:
return "INVALID";
break;
}
}

static char *GetSpriteName(Sprite type)
{
switch(type)
{
case SPRITE_default:
return "Default";
break;
case SPRITE_ground_arctic:
return "Ground Arctic";
break;
case SPRITE_ground_desert:
return "Ground Desert";
break;
case SPRITE_ground_plains:
return "Ground Plains";
break;
case SPRITE_player:
return "Player";
break;
case SPRITE_far_mountains:
return "Far Mountains";
break;
case SPRITE_mid_mountains:
return "Mid Mountains";
break;
case SPRITE_cloud_v1:
return "Cloud V1";
break;
case SPRITE_cloud_v2:
return "Cloud V2";
break;
case SPRITE_cloud_v3:
return "Cloud V3";
break;
case SPRITE_cloud_v4:
return "Cloud V4";
break;
case SPRITE_cloud_v5:
return "Cloud V5";
break;
case SPRITE_cloud_v6:
return "Cloud V6";
break;
case SPRITE_ground_forest_flat:
return "Ground Forest Flat";
break;
case SPRITE_ground_forest_10_degree:
return "Ground Forest 10 Degree";
break;
case SPRITE_ground_rocks_v1:
return "Ground Rocks V1";
break;
case SPRITE_ground_rocks_v2:
return "Ground Rocks V2";
break;
case SPRITE_pine_tree_v1:
return "Pine Tree V1";
break;
case SPRITE_hills_1_v1:
return "Hills 1v1";
break;
case SPRITE_hills_1_v2:
return "Hills 1v2";
break;
case SPRITE_bg1_shrub_v1:
return "Bg1 Shrub V1";
break;
case SPRITE_bg1_shrub_v2:
return "Bg1 Shrub V2";
break;
case SPRITE_bg1_shrub_v3:
return "Bg1 Shrub V3";
break;
case SPRITE_bg1_sapling_v1:
return "Bg1 Sapling V1";
break;
case SPRITE_bg1_sapling_v2:
return "Bg1 Sapling V2";
break;
case SPRITE_bg1_sapling_v3:
return "Bg1 Sapling V3";
break;
case SPRITE_bg1_pine_tree_v1:
return "Bg1 Pine Tree V1";
break;
case SPRITE_bg1_pine_tree_v2:
return "Bg1 Pine Tree V2";
break;
case SPRITE_bg2_hills_v1:
return "Bg2 Hills V1";
break;
case SPRITE_bg2_hills_v2:
return "Bg2 Hills V2";
break;
case SPRITE_bg2_shrub_v1:
return "Bg2 Shrub V1";
break;
case SPRITE_bg2_shrub_v2:
return "Bg2 Shrub V2";
break;
case SPRITE_bg2_shrub_v3:
return "Bg2 Shrub V3";
break;
case SPRITE_bg2_pine_tree_v1:
return "Bg2 Pine Tree V1";
break;
case SPRITE_bg2_pine_tree_v2:
return "Bg2 Pine Tree V2";
break;
case SPRITE_bg3_hills_v1:
return "Bg3 Hills V1";
break;
case SPRITE_bg3_shrub_v1:
return "Bg3 Shrub V1";
break;
case SPRITE_bg3_shrub_v2:
return "Bg3 Shrub V2";
break;
case SPRITE_bg3_shrub_v3:
return "Bg3 Shrub V3";
break;
case SPRITE_bg3_shrub_v4:
return "Bg3 Shrub V4";
break;
case SPRITE_bg3_pine_tree_v1:
return "Bg3 Pine Tree V1";
break;
case SPRITE_bg3_pine_tree_v2:
return "Bg3 Pine Tree V2";
break;
case SPRITE_bg3_pine_tree_v3:
return "Bg3 Pine Tree V3";
break;
case SPRITE_bg3_pine_tree_v4:
return "Bg3 Pine Tree V4";
break;
case SPRITE_bg3_pine_tree_v5:
return "Bg3 Pine Tree V5";
break;
case SPRITE_bg3_pine_tree_v6:
return "Bg3 Pine Tree V6";
break;
case SPRITE_bg3_pine_tree_v7:
return "Bg3 Pine Tree V7";
break;
case SPRITE_bg3_pine_tree_v8:
return "Bg3 Pine Tree V8";
break;
case SPRITE_y_axis_arrow_icon:
return "Yaxis Arrow Icon";
break;
case SPRITE_x_axis_arrow_icon:
return "Xaxis Arrow Icon";
break;
case SPRITE_circle_icon:
return "Circle Icon";
break;
case SPRITE_side_arrow:
return "Side Arrow";
break;
case SPRITE_test_icon:
return "Test Icon";
break;
case SPRITE_crafting_stump:
return "Crafting Stump";
break;
case SPRITE_runic_enchanter:
return "Runic Enchanter";
break;
case SPRITE_flint_sword_icon:
return "Flint Sword Icon";
break;
case SPRITE_flint_sword_ground:
return "Flint Sword Ground";
break;
case SPRITE_flint_axe:
return "Flint Axe";
break;
case SPRITE_flint:
return "Flint";
break;
case SPRITE_twig:
return "Twig";
break;
case SPRITE_crafting_tool:
return "Crafting Tool";
break;
case SPRITE_test_chestpiece:
return "Test Chestpiece";
break;
case SPRITE_shia:
return "Shia";
break;
case SPRITE_shia2:
return "Shia2";
break;
case SPRITE_dummy:
return "Dummy";
break;
case SPRITE_default_dynamic:
return "Default Dynamic";
break;
case SPRITE_player_idle:
return "Player Idle";
break;
case SPRITE_player_walking:
return "Player Walking";
break;
case SPRITE_player_sprinting:
return "Player Sprinting";
break;
case SPRITE_birch_tree1:
return "Birch Tree1";
break;
case SPRITE_birch_tree2:
return "Birch Tree2";
break;
case SPRITE_birch_tree3:
return "Birch Tree3";
break;
case SPRITE_birch_tree4:
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

static char *GetEnchantmentTypeName(EnchantmentType type)
{
switch(type)
{
case ENCHANTMENT_TYPE_none:
return "None";
break;
case ENCHANTMENT_TYPE_test:
return "Test";
break;
case ENCHANTMENT_TYPE_yeet:
return "Yeet";
break;
case ENCHANTMENT_TYPE_uhhhh:
return "Uhhhh";
break;
default:
return "INVALID";
break;
}
}

static char *GetItemCategoryName(ItemCategory type)
{
switch(type)
{
case ITEM_CATEGORY_none:
return "None";
break;
case ITEM_CATEGORY_resource:
return "Resource";
break;
case ITEM_CATEGORY_lumber_axe:
return "Lumber Axe";
break;
case ITEM_CATEGORY_sword:
return "Sword";
break;
case ITEM_CATEGORY_helmet:
return "Helmet";
break;
case ITEM_CATEGORY_chestpiece:
return "Chestpiece";
break;
case ITEM_CATEGORY_gauntlet:
return "Gauntlet";
break;
case ITEM_CATEGORY_leggings:
return "Leggings";
break;
case ITEM_CATEGORY_boots:
return "Boots";
break;
case ITEM_CATEGORY_cloak:
return "Cloak";
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
case ITEM_TYPE_flint_axe:
return "Flint Axe";
break;
case ITEM_TYPE_flint:
return "Flint";
break;
case ITEM_TYPE_twig:
return "Twig";
break;
case ITEM_TYPE_crafting_tool:
return "Crafting Tool";
break;
case ITEM_TYPE_test_chestpiece:
return "Test Chestpiece";
break;
default:
return "INVALID";
break;
}
}

static char *GetElementalSkillTypeName(ElementalSkillType type)
{
switch(type)
{
case ELEMENTAL_SKILL_TYPE_none:
return "None";
break;
case ELEMENTAL_SKILL_TYPE_hand_flame:
return "Hand Flame";
break;
case ELEMENTAL_SKILL_TYPE_hand_flame_2:
return "Hand Flame 2";
break;
case ELEMENTAL_SKILL_TYPE_hand_flame_3:
return "Hand Flame 3";
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
case C2_SHAPE_TYPE_line_segments:
return "Line Segments";
break;
default:
return "INVALID";
break;
}
}

static char *GetCraftingRecipeTypeName(CraftingRecipeType type)
{
switch(type)
{
case CRAFTING_RECIPE_TYPE_none:
return "None";
break;
case CRAFTING_RECIPE_TYPE_flint_axe:
return "Flint Axe";
break;
case CRAFTING_RECIPE_TYPE_flint_sword:
return "Flint Sword";
break;
default:
return "INVALID";
break;
}
}

static char *GetStructureCategoryName(StructureCategory type)
{
switch(type)
{
case STRUCTURE_CATEGORY_none:
return "None";
break;
case STRUCTURE_CATEGORY_shia:
return "Shia";
break;
case STRUCTURE_CATEGORY_crafting:
return "Crafting";
break;
case STRUCTURE_CATEGORY_base:
return "Base";
break;
default:
return "INVALID";
break;
}
}

static char *GetStructureTypeName(StructureType type)
{
switch(type)
{
case STRUCTURE_TYPE_none:
return "None";
break;
case STRUCTURE_TYPE_shia:
return "Shia";
break;
case STRUCTURE_TYPE_crafting_stump:
return "Crafting Stump";
break;
case STRUCTURE_TYPE_shia2:
return "Shia2";
break;
case STRUCTURE_TYPE_base:
return "Base";
break;
default:
return "INVALID";
break;
}
}

static char *GetMagicTypeName(MagicType type)
{
switch(type)
{
case MAGIC_TYPE_conscientiousness:
return "Conscientiousness";
break;
case MAGIC_TYPE_elemental:
return "Elemental";
break;
case MAGIC_TYPE_big_spook:
return "Big Spook";
break;
default:
return "INVALID";
break;
}
}

static char *GetElementalAffinityName(ElementalAffinity type)
{
switch(type)
{
case ELEMENTAL_AFFINITY_earth:
return "Earth";
break;
case ELEMENTAL_AFFINITY_fire:
return "Fire";
break;
case ELEMENTAL_AFFINITY_water:
return "Water";
break;
case ELEMENTAL_AFFINITY_air:
return "Air";
break;
default:
return "INVALID";
break;
}
}

static char *GetSpellTypeName(SpellType type)
{
switch(type)
{
case SPELL_TYPE_none:
return "None";
break;
case SPELL_TYPE_fireball:
return "Fireball";
break;
case SPELL_TYPE_yeet:
return "Yeet";
break;
default:
return "INVALID";
break;
}
}

static char *GetTreeTypeName(TreeType type)
{
switch(type)
{
case TREE_TYPE_oak:
return "Oak";
break;
case TREE_TYPE_birch:
return "Birch";
break;
case TREE_TYPE_pine:
return "Pine";
break;
default:
return "INVALID";
break;
}
}

static char *GetEntityPropertyName(EntityProperty type)
{
switch(type)
{
case ENTITY_PROPERTY_is_allocated:
return "Is Allocated";
break;
case ENTITY_PROPERTY_is_character:
return "Is Character";
break;
case ENTITY_PROPERTY_map_entity:
return "Map Entity";
break;
case ENTITY_PROPERTY_positional:
return "Positional";
break;
case ENTITY_PROPERTY_interactable:
return "Interactable";
break;
case ENTITY_PROPERTY_interactable_left_click:
return "Interactable Left Click";
break;
case ENTITY_PROPERTY_lumber_axable:
return "Lumber Axable";
break;
case ENTITY_PROPERTY_enemy:
return "Enemy";
break;
case ENTITY_PROPERTY_enchanter:
return "Enchanter";
break;
case ENTITY_PROPERTY_elemental_skill_tree:
return "Elemental Skill Tree";
break;
case ENTITY_PROPERTY_item:
return "Item";
break;
case ENTITY_PROPERTY_sprite:
return "Sprite";
break;
case ENTITY_PROPERTY_flipbook:
return "Flipbook";
break;
case ENTITY_PROPERTY_parallaxable:
return "Parallaxable";
break;
case ENTITY_PROPERTY_physical:
return "Physical";
break;
case ENTITY_PROPERTY_blueprint:
return "Blueprint";
break;
case ENTITY_PROPERTY_queryable:
return "Queryable";
break;
case ENTITY_PROPERTY_terrain_segment:
return "Terrain Segment";
break;
case ENTITY_PROPERTY_tree:
return "Tree";
break;
default:
return "INVALID";
break;
}
}

static char *GetEntityPresetTypeName(EntityPresetType type)
{
switch(type)
{
case ENTITY_PRESET_TYPE_none:
return "None";
break;
case ENTITY_PRESET_TYPE_tree:
return "Tree";
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

static char *GetRenderableTypeName(RenderableType type)
{
switch(type)
{
case RENDERABLE_TYPE_texture:
return "Texture";
break;
case RENDERABLE_TYPE_text:
return "Text";
break;
case RENDERABLE_TYPE_filled_rect:
return "Filled Rect";
break;
case RENDERABLE_TYPE_line:
return "Line";
break;
default:
return "INVALID";
break;
}
}

static char *GetCharacterCombatStateName(CharacterCombatState type)
{
switch(type)
{
case CHARACTER_COMBAT_STATE_none:
return "None";
break;
case CHARACTER_COMBAT_STATE_light_charge:
return "Light Charge";
break;
case CHARACTER_COMBAT_STATE_heavy_charge:
return "Heavy Charge";
break;
case CHARACTER_COMBAT_STATE_light_attack:
return "Light Attack";
break;
case CHARACTER_COMBAT_STATE_heavy_attack:
return "Heavy Attack";
break;
default:
return "INVALID";
break;
}
}

static void WriteEntityToFile(FILE *file, Entity *data)
{
    i32 version = 0;
    WriteToFile(file, &version, sizeof(i32));
    WriteEntity_Version0ToFile(file, data);
}

static void ReadEntityFromFile(FILE *file, Entity *data)
{
    i32 actual_version = -1;
    ReadFromFile(file, &actual_version, sizeof(i32));
    if (actual_version == 0)
    {
        ReadEntity_Version0FromFile(file, data);
        return;
    }

    switch (actual_version)
    {
    }
}

static void WriteCharacterDataToFile(FILE *file, CharacterData *data)
{
    i32 version = 0;
    WriteToFile(file, &version, sizeof(i32));
    WriteCharacterData_Version0ToFile(file, data);
}

static void ReadCharacterDataFromFile(FILE *file, CharacterData *data)
{
    i32 actual_version = -1;
    ReadFromFile(file, &actual_version, sizeof(i32));
    if (actual_version == 0)
    {
        ReadCharacterData_Version0FromFile(file, data);
        return;
    }

    switch (actual_version)
    {
    }
}

static void WriteWorldDataToFile(FILE *file, WorldData *data)
{
    i32 version = 0;
    WriteToFile(file, &version, sizeof(i32));
    WriteWorldData_Version0ToFile(file, data);
}

static void ReadWorldDataFromFile(FILE *file, WorldData *data)
{
    i32 actual_version = -1;
    ReadFromFile(file, &actual_version, sizeof(i32));
    if (actual_version == 0)
    {
        ReadWorldData_Version0FromFile(file, data);
        return;
    }

    switch (actual_version)
    {
    }
}

static void WriteEntity_Version0ToFile(FILE *file, Entity *data)
{
    for (i32 i = 0; i < ENTITY_PROPERTY_SIZE; i++)
    {
        WriteToFile(file, &data->properties[i], sizeof(u64));
    }

    WriteToFile(file, &data->testint, sizeof(data->testint));

    for (i32 i = 0; i < 100; i++)
    {
        WriteToFile(file, &data->debug_name[i], sizeof(char));
    }

    WriteToFile(file, &data->position, sizeof(data->position));

    WriteToFile(file, &data->sprite_data, sizeof(data->sprite_data));

    WriteToFile(file, &data->is_flipped, sizeof(data->is_flipped));

    WriteToFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

    WriteToFile(file, &data->animation_flags, sizeof(data->animation_flags));

    WriteToFile(file, &data->current_frame, sizeof(data->current_frame));

    WriteToFile(file, &data->interval_mult, sizeof(data->interval_mult));

    WriteToFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

    WriteToFile(file, &data->physics, sizeof(data->physics));

    WriteToFile(file, &data->smooth_velocity, sizeof(data->smooth_velocity));

    WriteToFile(file, &data->axis_x, sizeof(data->axis_x));

    WriteToFile(file, &data->move_speed, sizeof(data->move_speed));

    WriteToFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

    WriteToFile(file, &data->entity_type, sizeof(data->entity_type));

    WriteToFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

    WriteToFile(file, &data->item, sizeof(data->item));

    for (i32 i = 0; i < MAX_ENCHANTMENTS; i++)
    {
        WriteToFile(file, &data->enchantments[i], sizeof(Enchantment));
    }

    WriteToFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    WriteToFile(file, &data->desired_position, sizeof(data->desired_position));

    WriteToFile(file, &data->priority, sizeof(data->priority));

    WriteToFile(file, &data->structure_type, sizeof(data->structure_type));

    for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
    {
        WriteToFile(file, &data->remaining_items_in_blueprint[i], sizeof(Item));
    }

    WriteToFile(file, &data->durability, sizeof(data->durability));

    WriteToFile(file, &data->tree_type, sizeof(data->tree_type));

}

static void ReadEntity_Version0FromFile(FILE *file, Entity *data)
{
    for (i32 i = 0; i < ENTITY_PROPERTY_SIZE; i++)
    {
        ReadFromFile(file, &data->properties[i], sizeof(u64));
    }

    ReadFromFile(file, &data->testint, sizeof(data->testint));

    for (i32 i = 0; i < 100; i++)
    {
        ReadFromFile(file, &data->debug_name[i], sizeof(char));
    }

    ReadFromFile(file, &data->position, sizeof(data->position));

    ReadFromFile(file, &data->sprite_data, sizeof(data->sprite_data));

    ReadFromFile(file, &data->is_flipped, sizeof(data->is_flipped));

    ReadFromFile(file, &data->is_background_sprite, sizeof(data->is_background_sprite));

    ReadFromFile(file, &data->animation_flags, sizeof(data->animation_flags));

    ReadFromFile(file, &data->current_frame, sizeof(data->current_frame));

    ReadFromFile(file, &data->interval_mult, sizeof(data->interval_mult));

    ReadFromFile(file, &data->frame_start_time, sizeof(data->frame_start_time));

    ReadFromFile(file, &data->physics, sizeof(data->physics));

    ReadFromFile(file, &data->smooth_velocity, sizeof(data->smooth_velocity));

    ReadFromFile(file, &data->axis_x, sizeof(data->axis_x));

    ReadFromFile(file, &data->move_speed, sizeof(data->move_speed));

    ReadFromFile(file, &data->move_speed_mult, sizeof(data->move_speed_mult));

    ReadFromFile(file, &data->entity_type, sizeof(data->entity_type));

    ReadFromFile(file, &data->current_animation_state, sizeof(data->current_animation_state));

    ReadFromFile(file, &data->item, sizeof(data->item));

    for (i32 i = 0; i < MAX_ENCHANTMENTS; i++)
    {
        ReadFromFile(file, &data->enchantments[i], sizeof(Enchantment));
    }

    ReadFromFile(file, &data->parallax_amount, sizeof(data->parallax_amount));

    ReadFromFile(file, &data->desired_position, sizeof(data->desired_position));

    ReadFromFile(file, &data->priority, sizeof(data->priority));

    ReadFromFile(file, &data->structure_type, sizeof(data->structure_type));

    for (i32 i = 0; i < MAX_ITEMS_IN_BLUEPRINT_RECIPE; i++)
    {
        ReadFromFile(file, &data->remaining_items_in_blueprint[i], sizeof(Item));
    }

    ReadFromFile(file, &data->durability, sizeof(data->durability));

    ReadFromFile(file, &data->tree_type, sizeof(data->tree_type));

}

static void WriteCharacterData_Version0ToFile(FILE *file, CharacterData *data)
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

    for (i32 i = 0; i < MAX_SPELL_SLOTS; i++)
    {
        WriteToFile(file, &data->freehand_spell_slots[i], sizeof(Spell));
    }

    WriteToFile(file, &data->freehand_spell_count, sizeof(data->freehand_spell_count));

    for (i32 i = 0; i < MAX_EQUIPMENT_SLOTS; i++)
    {
        WriteToFile(file, &data->equipment_slots[i], sizeof(Item));
    }

    for (i32 i = 0; i < ELEMENTAL_SKILL_TYPE_MAX; i++)
    {
        WriteToFile(file, &data->unlocked_elemental_skills[i], sizeof(b8));
    }

    for (i32 i = 0; i < ELEMENTAL_SKILL_TYPE_MAX; i++)
    {
        WriteToFile(file, &data->purchased_elemental_skills[i], sizeof(ElementalSkillType));
    }

    WriteToFile(file, &data->elemental_skill_points, sizeof(data->elemental_skill_points));

}

static void ReadCharacterData_Version0FromFile(FILE *file, CharacterData *data)
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

    for (i32 i = 0; i < MAX_SPELL_SLOTS; i++)
    {
        ReadFromFile(file, &data->freehand_spell_slots[i], sizeof(Spell));
    }

    ReadFromFile(file, &data->freehand_spell_count, sizeof(data->freehand_spell_count));

    for (i32 i = 0; i < MAX_EQUIPMENT_SLOTS; i++)
    {
        ReadFromFile(file, &data->equipment_slots[i], sizeof(Item));
    }

    for (i32 i = 0; i < ELEMENTAL_SKILL_TYPE_MAX; i++)
    {
        ReadFromFile(file, &data->unlocked_elemental_skills[i], sizeof(b8));
    }

    for (i32 i = 0; i < ELEMENTAL_SKILL_TYPE_MAX; i++)
    {
        ReadFromFile(file, &data->purchased_elemental_skills[i], sizeof(ElementalSkillType));
    }

    ReadFromFile(file, &data->elemental_skill_points, sizeof(data->elemental_skill_points));

}

static void WriteWorldData_Version0ToFile(FILE *file, WorldData *data)
{
    WriteCharacterDataToFile(file, &data->character_data);

    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

    WriteToFile(file, &data->character_chunk, sizeof(data->character_chunk));

}

static void ReadWorldData_Version0FromFile(FILE *file, WorldData *data)
{
    ReadCharacterDataFromFile(file, &data->character_data);

    ReadFromFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

    ReadFromFile(file, &data->character_chunk, sizeof(data->character_chunk));

}

