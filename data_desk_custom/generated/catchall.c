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
case STATIC_SPRITE_side_arrow:
return "Side Arrow";
break;
case STATIC_SPRITE_test_icon:
return "Test Icon";
break;
case STATIC_SPRITE_crafting_stump:
return "Crafting Stump";
break;
case STATIC_SPRITE_runic_enchanter:
return "Runic Enchanter";
break;
case STATIC_SPRITE_flint_sword_icon:
return "Flint Sword Icon";
break;
case STATIC_SPRITE_flint_sword_ground:
return "Flint Sword Ground";
break;
case STATIC_SPRITE_flint_axe:
return "Flint Axe";
break;
case STATIC_SPRITE_flint:
return "Flint";
break;
case STATIC_SPRITE_twig:
return "Twig";
break;
case STATIC_SPRITE_crafting_tool:
return "Crafting Tool";
break;
case STATIC_SPRITE_test_chestpiece:
return "Test Chestpiece";
break;
case STATIC_SPRITE_shia:
return "Shia";
break;
case STATIC_SPRITE_shia2:
return "Shia2";
break;
case STATIC_SPRITE_dummy:
return "Dummy";
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
case ENTITY_PROPERTY_no_delete:
return "No Delete";
break;
case ENTITY_PROPERTY_force_floating:
return "Force Floating";
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
    //Although char* current_general_state is marked @DoNotSerialise, it's just 8 extra bytes
    //Alternative is to write it in two chunks, one before the pointer, and one after
    WriteToFile(file, data, sizeof(Entity));
}

static void ReadEntityFromFile(FILE *file, Entity *data)
{
    ReadFromFile(file, data, sizeof(Entity));
    //NOTE(to randy): The only time you set this variable is in entity.c (line 24), and it's set to "Idle"
    //There's no other references to it, not entirely sure what it's for
    data->current_general_state = 0;
}

static void WriteCharacterDataToFile(FILE *file, CharacterData *data)
{
    //NOTE(to randy): This saves the Character data, including the Item* grabbed_item_origin_slot
    //If you don't want to save that, delete the second line
    WriteToFile(file, data, sizeof(CharacterData)); //Saves the pointer to the Item data
    WriteToFile(file, data->grabbed_item_origin_slot, sizeof(Item)); //Saves the item data, but at the end
}

static void ReadCharacterDataFromFile(FILE *file, CharacterData *data)
{
    //Below code is if you don't store Item* grabbed_item_origin_slot
    //ReadFromFile(file, data, sizeof(CharacterData));
    //data->grabbed_item_origin_slot = 0;
    ReadFromFile(file, data, sizeof(CharacterData));
    Item* grabbed_item_origin_slot = malloc(sizeof(Item)); //Not sure if this is what you want
    ReadFromFile(file, grabbed_item_origin_slot, sizeof(Item)); //Get actual item data
    data->grabbed_item_origin_slot = grabbed_item_origin_slot; //Give the Item* pointer to the CharacterData
}

static void WriteWorldSaveDataToFile(FILE *file, WorldSaveData *data)
{
    WriteToFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

}

static void ReadWorldSaveDataFromFile(FILE *file, WorldSaveData *data)
{
    ReadFromFile(file, &data->elapsed_world_time, sizeof(data->elapsed_world_time));

}

