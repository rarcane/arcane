void _TsAssetsInitTypeSizes(TsAssets *assets)
{
#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) {\
        assets->asset_types[TSASSETS_TYPE_##name].size_per_asset = sizeof(name);\
        assets->asset_types[TSASSETS_TYPE_##name].size_per_load_info = sizeof(load_info_name);\
    }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType
}

void _TsAssetsInitTypeMaxCounts(TsAssets *assets)
{
#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) assets->asset_types[TSASSETS_TYPE_##name].max_count = max;
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType
}

void _TsAssetsInitTypeCallbacks(TsAssets *assets)
{
#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) {\
        assets->asset_types[TSASSETS_TYPE_##name].PreLoad  = pre_load_callback;\
        assets->asset_types[TSASSETS_TYPE_##name].Load     = load_callback;\
        assets->asset_types[TSASSETS_TYPE_##name].PostLoad  = post_load_callback;\
        assets->asset_types[TSASSETS_TYPE_##name].CleanUp  = clean_up_callback;\
        assets->asset_types[TSASSETS_TYPE_##name].IsLoaded = is_loaded_callback;\
    }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType
}

void _TsAssetsInitWatchDirectoryNames(TsAssets *assets)
{
#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) {\
        assets->asset_types[TSASSETS_TYPE_##name].asset_folder_name = directory;\
        assets->asset_directories[TSASSETS_TYPE_##name].path = directory;\
    }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType
}

void _TsAssetsRefreshDirectory(TsAssets *assets, TsAssetsType type_id);

#if TSASSETS_WIN32
#include "tsassets_win32.c"
#endif

#define TsAssetsHashAssetName CStringCRC32

void
TsAssetsInit(TsAssets *assets, char *assets_root_path, MemoryArena *arena)
{
    
    assets->assets_root_path = assets_root_path;
    assets->active_priority = TSASSETS_PRIORITY_LOW;
    
    for(int i = 0; i < TSASSETS_PRIORITY_MAX; ++i)
    {
        assets->number_of_assets_queued_for_update[i] = 0;
        assets->number_of_assets_complete[i] = 0;
    }
    
    _TsAssetsInitTypeSizes(assets);
    _TsAssetsInitTypeMaxCounts(assets);
    _TsAssetsInitTypeCallbacks(assets);
    
    for(int i = 0; i < TSASSETS_TYPE_MAX; ++i)
    {
        TsAssetsTypeData *type = assets->asset_types + i;
        type->folder_items = 0;
        type->folder_item_count = 0;
        type->count = 0;
        type->update_index_count = 0;
        type->name_to_index_keys         = MemoryArenaAllocateAndZero(arena, sizeof(type->name_to_index_keys[0])*type->max_count);
        type->table                      = MemoryArenaAllocateAndZeroAligned(arena, type->size_per_asset*type->max_count, 64);
        type->load_info_table            = MemoryArenaAllocateAndZeroAligned(arena, type->size_per_load_info*type->max_count, 64);
        type->load_asset_info_table      = MemoryArenaAllocateAndZeroAligned(arena, sizeof(type->load_asset_info_table[0])*type->max_count, 64);
        type->asset_load_job_indices     = MemoryArenaAllocateAndZero(arena, sizeof(type->asset_load_job_indices[0])*type->max_count);
        type->requests                   = MemoryArenaAllocateAndZero(arena, sizeof(type->requests[0])*type->max_count);
        type->update_indices             = MemoryArenaAllocateAndZero(arena, sizeof(type->update_indices[0])*type->max_count);
        type->queued_for_update_status   = MemoryArenaAllocateAndZero(arena, sizeof(type->queued_for_update_status[0])*type->max_count);
        type->linear_iteration_list      = MemoryArenaAllocateAndZero(arena, sizeof(void *) * type->max_count);
        type->linear_iteration_name_list = MemoryArenaAllocateAndZero(arena, sizeof(char *) * type->max_count);
        type->asset_priorities           = MemoryArenaAllocateAndZero(arena, sizeof(type->asset_priorities[0]) * type->max_count);
        
        type->name_memory_size = type->max_count * TSASSETS_MAX_ASSET_PATH_LENGTH;
        type->name_memory = MemoryArenaAllocate(arena, type->name_memory_size);
        type->name_memory_swap = MemoryArenaAllocate(arena, type->name_memory_size);
        
        for(u32 j = 0; j < type->max_count; ++j)
        {
            type->asset_load_job_indices[j] = -1;
        }
    }
    
    _TsAssetsInitWatchDirectories(assets);
    
    for(int i = 0; i < TSASSETS_TYPE_MAX; ++i)
    {
        _TsAssetsRefreshDirectory(assets, (TsAssetsType)i);
    }
    
}

void
TsAssetsCleanUp(TsAssets *assets)
{
}

int
TsAssetsMatchAssetNames(char *a, char *b)
{
    int result = 1;
    for(int i = 0;; ++i)
    {
        if(a[i] != b[i])
        {
            result = 0;
            break;
        }
        else if(!a[i])
        {
            break;
        }
    }
    return result;
}

int
TsAssetsLoadAssetWorkerThread(void *job_data)
{
    int success = 1;
    TsAssetsLoadAssetInfo *load_info = job_data;
    load_info->Load(load_info->assets, load_info->source_file, load_info->asset_load_info);
    return success;
}

void
TsAssetsUpdate(TsAssets *assets)
{
    for(int type_id = 0; type_id < TSASSETS_TYPE_MAX; ++type_id)
    {
        TsAssetsTypeData *type = assets->asset_types + type_id;
        if(type->update_index_count)
        {
            platform->pump_events = 1;
        }
        for(int i = 0; i < (int)type->update_index_count; ++i)
        {
            uint32_t asset_index = type->update_indices[i];
            void *asset = (char *)type->table + type->size_per_asset * asset_index;
            void *asset_load_info = (char *)type->load_info_table + type->size_per_load_info * asset_index;
            u8 asset_priority = type->asset_priorities[asset_index];
            
            b32 need_removal = 0;
            
            switch(type->queued_for_update_status[asset_index])
            {
                case TSASSETS_ASSET_UPDATE_STATUS_FLAGGED:
                case TSASSETS_ASSET_UPDATE_STATUS_FLAGGED_RELOAD:
                {
                    if(type->requests[asset_index] > 0)
                    {
                        if(type->queued_for_update_status[asset_index] == TSASSETS_ASSET_UPDATE_STATUS_FLAGGED_RELOAD ||
                           !type->IsLoaded(asset))
                        {
                            ++assets->number_of_assets_queued_for_update[asset_priority];
                            
                            if(type->IsLoaded(asset))
                            {
                                type->CleanUp(assets, asset);
                            }
                            
                            char *filename = type->name_to_index_keys[asset_index].name;
                            
                            TsAssetsPriority last_priority = assets->active_priority;
                            TsAssetsSetRequestPriority(assets, asset_priority);
                            if(type->PreLoad)
                            {
                                type->PreLoad(assets, filename, asset);
                            }
                            TsAssetsSetRequestPriority(assets, last_priority);
                            
                            TsAssetsLoadAssetInfo *load_asset_info = type->load_asset_info_table + asset_index;
                            load_asset_info->assets = assets;
                            load_asset_info->asset_load_info = asset_load_info;
                            load_asset_info->Load = type->Load;
                            snprintf(load_asset_info->source_file, sizeof(load_asset_info->source_file), "%s%s/%s",
                                     assets->assets_root_path, type->asset_folder_name, filename);
                            i32 job_index = platform->QueueJob(load_asset_info, TsAssetsLoadAssetWorkerThread, 0);
                            type->asset_load_job_indices[asset_index] = job_index;
                            type->queued_for_update_status[asset_index] = TSASSETS_ASSET_UPDATE_STATUS_LOAD_QUEUED;
                        }
                        else
                        {
                            need_removal = 1;
                        }
                    }
                    else
                    {
                        if(!assets->clean_up_disabled && type->IsLoaded(asset))
                        {
                            type->CleanUp(assets, asset);
                            need_removal = 1;
                        }
                    }
                    
                    break;
                }
                
                case TSASSETS_ASSET_UPDATE_STATUS_LOAD_QUEUED:
                {
                    i32 job_index = type->asset_load_job_indices[asset_index];
                    if(platform->WaitForJob(job_index, 0))
                    {
                        --assets->number_of_assets_queued_for_update[asset_priority];
                        ++assets->number_of_assets_complete[asset_priority];
                        TsAssetsPriority last_priority = assets->active_priority;
                        TsAssetsSetRequestPriority(assets, asset_priority);
                        type->PostLoad(assets, asset_load_info, asset);
                        TsAssetsSetRequestPriority(assets, last_priority);
                        type->asset_load_job_indices[asset_index] = -1;
                        need_removal = 1;
                        platform->pump_events = 1;
                    }
                    break;
                }
                
                default:
                {
                    need_removal = 1;
                    break;
                }
                
            }
            
            if(need_removal)
            {
                type->queued_for_update_status[asset_index] = TSASSETS_ASSET_UPDATE_STATUS_DONE;
                if(i != type->update_index_count - 1)
                {
                    type->update_indices[i] = type->update_indices[type->update_index_count - 1];
                }
                --type->update_index_count;
                --i;
            }
        }
    }
    _TsAssetsCheckAssetDirectories(assets);
}

TsAssetsType
TsAssetsGetAssetType(TsAssets *assets, void *asset)
{
    TsAssetsType type = TSASSETS_TYPE_invalid;
    if(asset)
    {
        for(int i = 0; i < TSASSETS_TYPE_MAX; ++i)
        {
            uint32_t asset_index = (uint32_t)((uintptr_t)asset - (uintptr_t)assets->asset_types[i].table) / assets->asset_types[i].size_per_asset;
            if(asset_index >= 0 && asset_index < assets->asset_types[i].max_count)
            {
                type = i;
                break;
            }
        }
    }
    return type;
}

void
TsAssetsSetRequestPriority(TsAssets *assets, TsAssetsPriority priority)
{
    assets->active_priority = priority;
}

u32
TsAssetsGetAssetsToLoadForPriority(TsAssets *assets, TsAssetsPriority priority)
{
    return assets->number_of_assets_queued_for_update[priority];
}

u32
TsAssetsGetAssetsToLoad(TsAssets *assets)
{
    u32 assets_to_load = 0;
    for(int i = 0; i < TSASSETS_PRIORITY_MAX; ++i)
    {
        assets_to_load += assets->number_of_assets_queued_for_update[i];
    }
    return assets_to_load;
}

void *
TsAssetsGetAssetByName(TsAssets *assets, TsAssetsType type_id, char *name)
{
    void *asset = 0;
    if(type_id >= 0 && type_id < TSASSETS_TYPE_MAX)
    {
        TsAssetsTypeData *type = assets->asset_types + type_id;
        uint32_t hash = TsAssetsHashAssetName(name) % type->max_count;
        uint32_t original_hash = hash;
        
        for(;;)
        {
            if(type->name_to_index_keys[hash].name || type->name_to_index_keys[hash].deleted)
            {
                if(!type->name_to_index_keys[hash].deleted &&
                   TsAssetsMatchAssetNames(type->name_to_index_keys[hash].name, name))
                {
                    uint32_t asset_index = hash;
                    asset = (char *)type->table + asset_index*type->size_per_asset;
                    break;
                }
                else
                {
                    if(++hash >= type->max_count)
                    {
                        hash = 0;
                    }
                    if(hash == original_hash)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    return asset;
}

void *
TsAssetsRequestAsset(TsAssets *assets, TsAssetsType type_id, void *asset)
{
    if(asset)
    {
        TsAssetsTypeData *type = assets->asset_types + type_id;
        uint32_t asset_index = (uint32_t)((uintptr_t)asset - (uintptr_t)type->table) / type->size_per_asset;
        if(asset_index >= 0 && asset_index < type->max_count)
        {
            if(++type->requests[asset_index] == 1)
            {
                platform->pump_events = 1;
                type->asset_priorities[asset_index] = assets->active_priority;
                if(type->queued_for_update_status[asset_index] == TSASSETS_ASSET_UPDATE_STATUS_DONE)
                {
                    if(type->update_index_count < type->max_count)
                    {
                        type->queued_for_update_status[asset_index] = TSASSETS_ASSET_UPDATE_STATUS_FLAGGED;
                        type->update_indices[type->update_index_count++] = asset_index;
                    }
                }
            }
        }
        else
        {
            asset = 0;
        }
    }
    return asset;
}

void *
TsAssetsRequestAssetByName(TsAssets *assets, TsAssetsType type_id, char *name)
{
    void *asset = TsAssetsGetAssetByName(assets, type_id, name);
    TsAssetsRequestAsset(assets, type_id, asset);
    return asset;
}

void
TsAssetsReleaseAsset(TsAssets *assets, TsAssetsType type_id, void *asset)
{
    if(asset)
    {
        TsAssetsTypeData *type = assets->asset_types + type_id;
        uint32_t asset_index = (uint32_t)((uintptr_t)asset - (uintptr_t)type->table) / type->size_per_asset;
        if(asset_index >= 0 && asset_index < type->max_count)
        {
            if(--type->requests[asset_index] < 1)
            {
                if(type->queued_for_update_status[asset_index] == TSASSETS_ASSET_UPDATE_STATUS_DONE)
                {
                    if(type->update_index_count < type->max_count)
                    {
                        type->queued_for_update_status[asset_index] = TSASSETS_ASSET_UPDATE_STATUS_FLAGGED;
                        type->update_indices[type->update_index_count++] = asset_index;
                    }
                }
            }
        }
    }
}

void
TsAssetsForceAssetTypeReload(TsAssets *assets, TsAssetsType type_id)
{
    TsAssetsTypeData *type = assets->asset_types + type_id;
    platform->pump_events = 1;
    for(unsigned int i = 0; i < type->max_count; ++i)
    {
        if(type->name_to_index_keys[i].name && !type->name_to_index_keys[i].deleted)
        {
            if(type->requests[i] > 0 && type->update_index_count < type->max_count)
            {
                type->queued_for_update_status[i] = TSASSETS_ASSET_UPDATE_STATUS_FLAGGED_RELOAD;
                type->update_indices[type->update_index_count++] = i;
            }
        }
    }
}

void
TsAssetsEnableAssetCleanUp(TsAssets *assets)
{
    assets->clean_up_disabled = 0;
}

void
TsAssetsDisableAssetCleanUp(TsAssets *assets)
{
    assets->clean_up_disabled = 1;
}

void
_TsAssetsRefreshDirectory(TsAssets *assets, TsAssetsType type_id)
{
    TsAssetsTypeData *type = assets->asset_types + type_id;
    int item_count = 0;
    char **items = 0;
    _TsAssetsLoadDirectoryItems(assets, type->asset_folder_name, &item_count, &items);
    
    // NOTE(rjf): Swap name memory with swap memory.
    {
        char *swap = type->name_memory_swap;
        type->name_memory_swap = type->name_memory;
        type->name_memory = swap;
    }
    
    // NOTE(rjf): Remove deleted and unused items.
    {
        int new_item_count = item_count;
        char **new_items = items;
        int old_item_count = type->folder_item_count;
        char **old_items = type->folder_items;
        
        for(int i = 0; i < old_item_count; ++i)
        {
            b32 found = 0;
            for(int j = 0; j < new_item_count; ++j)
            {
                if(TsAssetsMatchAssetNames(old_items[i], new_items[j]))
                {
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                // NOTE(rjf): Asset at i was deleted.
                void *asset = TsAssetsGetAssetByName(assets, type_id, old_items[i]);
                if(asset)
                {
                    u32 asset_index = (u32)((uintptr_t)asset - (uintptr_t)type->table) / type->size_per_asset;
                    type->name_to_index_keys[asset_index].deleted = 1;
                    --type->count;
                }
            }
        }
    }
    
    // NOTE(rjf): Add items to asset table.
    {
        uint32_t name_memory_alloc_position = 0;
        
        for(int i = 0; i < item_count; ++i)
        {
            if(type->count < type->max_count)
            {
                uint32_t hash = TsAssetsHashAssetName(items[i]) % type->max_count;
                uint32_t original_hash = hash;
                uint32_t slot_index = 0;
                uint32_t slot_found = 0;
                b32 asset_already_existed = 0;
                
                for(;;)
                {
                    if(type->name_to_index_keys[hash].name || type->name_to_index_keys[hash].deleted)
                    {
                        if(!type->name_to_index_keys[hash].deleted &&
                           TsAssetsMatchAssetNames(type->name_to_index_keys[hash].name, items[i]))
                        {
                            slot_found = 1;
                            slot_index = hash;
                            asset_already_existed = 1;
                            break;
                        }
                        else
                        {
                            if(type->name_to_index_keys[hash].deleted)
                            {
                                slot_found = 1;
                                slot_index = hash;
                            }
                            
                            if(++hash >= type->max_count)
                            {
                                hash = 0;
                            }
                            if(hash == original_hash)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        slot_index = hash;
                        slot_found = 1;
                        break;
                    }
                }
                
                if(slot_found)
                {
                    uint32_t asset_index = slot_index;
                    
                    char *name = items[i];
                    int name_length = 0;
                    for(; name[name_length]; ++name_length);
                    
                    if(name_memory_alloc_position + name_length >= type->name_memory_size)
                    {
                        // TODO(rjf): Proper error reporting/assertion
                        name_length = type->name_memory_size - name_memory_alloc_position;
                    }
                    
                    type->name_to_index_keys[hash].name = type->name_memory + name_memory_alloc_position;
                    TsAssetsMemoryCopy(type->name_to_index_keys[hash].name, name, name_length+1);
                    type->name_to_index_keys[hash].name[name_length] = 0;
                    name_memory_alloc_position += name_length + 1;
                    type->linear_iteration_list[i] = (char *)type->table + asset_index*type->size_per_asset;
                    type->linear_iteration_name_list[i] = type->name_to_index_keys[asset_index].name;
                    
                    // NOTE(rjf): Reload the asset if it has nonzero requests. In the future, it might
                    // be a good idea to try to extract which assets need to be reloaded from Win32,
                    // but that would suck right now so I don't want to do that.
                    if(type->requests[asset_index] > 0 &&
                       type->queued_for_update_status[asset_index] == TSASSETS_ASSET_UPDATE_STATUS_DONE)
                    {
                        type->queued_for_update_status[asset_index] = TSASSETS_ASSET_UPDATE_STATUS_FLAGGED_RELOAD;
                        if(type->update_index_count < type->max_count)
                        {
                            type->update_indices[type->update_index_count++] = asset_index;
                        }
                    }
                    
                    if(i == 0)
                    {
                        type->default_asset = (char *)type->table + type->size_per_asset*asset_index;
                    }
                    
                    if(!asset_already_existed)
                    {
                        ++type->count;
                    }
                }
            }
        }
    }
    
    if(type->folder_items)
    {
        _TsAssetsFreeDirectoryItems(type->folder_items);
    }
    type->folder_item_count = item_count;
    type->folder_items = items;
}