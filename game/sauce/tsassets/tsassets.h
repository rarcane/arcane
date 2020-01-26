/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifndef TSASSETS_H_INCLUDED
#define TSASSETS_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

typedef struct TsAssetsWatchDirectory TsAssetsWatchDirectory;

#if TSASSETS_WIN32
#define WIN32_LEAN_AND_MEAN
#include "tsfoundation/tsfoundation_windows.h"
#undef DeleteFile
#include "tsassets_win32.h"
#endif

#ifndef TSASSETS_ASSET_TYPE_FILE
#define TSASSETS_ASSET_TYPE_FILE "tsassets_type_list.inc"
#endif

#define TsAssetsMemorySet     memset
#define TsAssetsMemoryCopy    memcpy
#define TsAssetsHeapAllocate  malloc
#define TsAssetsHeapFree      free

typedef struct TsAssets TsAssets;

typedef struct TsAssetTableKey TsAssetTableKey;
struct TsAssetTableKey
{
    char *name;
    uint8_t deleted;
};

typedef void   TsAssetsMainThread_PreLoadCallback   (TsAssets *assets, char *source_file, void *asset);
typedef void   TsAssetsLoadThread_LoadCallback      (TsAssets *assets, char *source_file, void *asset_load_thread_info);
typedef void   TsAssetsMainThread_PostLoadCallback  (TsAssets *assets, void *asset_load_thread_info, void *asset);
typedef void   TsAssetsMainThread_CleanUpAssetCallback(TsAssets *assets, void *asset);
typedef int    TsAssetsIsAssetLoadedCallback(void *asset);

typedef struct TsAssetsLoadAssetInfo TsAssetsLoadAssetInfo;
struct TsAssetsLoadAssetInfo
{
    TsAssets *assets;
    void *asset_load_info;
    char source_file[256];
    TsAssetsLoadThread_LoadCallback *Load;
};

typedef enum TsAssetsPriority TsAssetsPriority;
enum TsAssetsPriority
{
    TSASSETS_PRIORITY_LOW,
    TSASSETS_PRIORITY_MEDIUM,
    TSASSETS_PRIORITY_HIGH,
    TSASSETS_PRIORITY_ULTRA,
    TSASSETS_PRIORITY_MAX
};

#define TSASSETS_MAX_ASSET_PATH_LENGTH 128
typedef struct TsAssetsTypeData TsAssetsTypeData;
struct TsAssetsTypeData
{
    char *asset_folder_name;
    int folder_item_count;
    char **folder_items;
    u32 count;
    u32 max_count;
    u32 size_per_asset;
    u32 size_per_load_info;
    char *name_memory;
    u32 name_memory_size;
    char *name_memory_swap;
    TsAssetTableKey *name_to_index_keys;
    void *table;
    void *default_asset;
    void *load_info_table;
    TsAssetsLoadAssetInfo *load_asset_info_table;
    i32 *asset_load_job_indices;
    u32 *requests;
    u32 update_index_count;
    u32 *update_indices;
    u8 *asset_priorities;
    
#define TSASSETS_ASSET_UPDATE_STATUS_DONE           0
#define TSASSETS_ASSET_UPDATE_STATUS_FLAGGED        1
#define TSASSETS_ASSET_UPDATE_STATUS_LOAD_QUEUED    2
#define TSASSETS_ASSET_UPDATE_STATUS_FLAGGED_RELOAD 3
    uint8_t *queued_for_update_status;
    void **linear_iteration_list;
    char **linear_iteration_name_list;
    
    TsAssetsMainThread_PreLoadCallback      *PreLoad;
    TsAssetsLoadThread_LoadCallback         *Load;
    TsAssetsMainThread_PostLoadCallback     *PostLoad;
    TsAssetsMainThread_CleanUpAssetCallback *CleanUp;
    TsAssetsIsAssetLoadedCallback           *IsLoaded;
    
};

typedef enum TsAssetsType TsAssetsType;
enum TsAssetsType
{
    TSASSETS_TYPE_invalid = -1,
#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) TSASSETS_TYPE_##name,
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType
    TSASSETS_TYPE_MAX
};

typedef struct TsAssets TsAssets;
struct TsAssets
{
    u32 number_of_assets_queued_for_update[TSASSETS_PRIORITY_MAX];
    u32 number_of_assets_complete[TSASSETS_PRIORITY_MAX];
    TsAssetsPriority active_priority;
    TsAssetsTypeData asset_types[TSASSETS_TYPE_MAX];
    char *assets_root_path;
    TsAssetsWatchDirectory asset_directories[TSASSETS_TYPE_MAX];
    b32 clean_up_disabled;
};

void TsAssetsInit(TsAssets *assets, char *assets_root_path, MemoryArena *arena);
void TsAssetsCleanUp(TsAssets *assets);
void TsAssetsUpdate(TsAssets *assets);
TsAssetsType TsAssetsGetAssetTypeFromName(TsAssets *assets, char *name);
TsAssetsType TsAssetsGetAssetType(TsAssets *assets, void *asset);
void *TsAssetsGetAssetByName(TsAssets *assets, TsAssetsType type, char *name);
void TsAssetsSetRequestPriority(TsAssets *assets, TsAssetsPriority priority);
u32 TsAssetsGetAssetsToLoadForPriority(TsAssets *assets, TsAssetsPriority priority);
u32 TsAssetsGetAssetsToLoad(TsAssets *assets);
void *TsAssetsRequestAsset(TsAssets *assets, TsAssetsType type, void *asset);
void *TsAssetsRequestAssetByName(TsAssets *assets, TsAssetsType type, char *name);
void TsAssetsReleaseAsset(TsAssets *assets, TsAssetsType type, void *asset);
void TsAssetsForceAssetTypeReload(TsAssets *assets, TsAssetsType type);
void TsAssetsEnableAssetCleanUp(TsAssets *assets);
void TsAssetsDisableAssetCleanUp(TsAssets *assets);

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) name *TsAssetsGetDefault##name##(TsAssets *assets) { return assets->asset_types[TSASSETS_TYPE_##name].default_asset; }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) name *TsAssetsGet##name##ByName(TsAssets *assets, char *name) { return TsAssetsGetAssetByName(assets, TSASSETS_TYPE_##name, name); }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) name *TsAssetsRequest##name##(TsAssets *assets, name *asset) { return TsAssetsRequestAsset(assets, TSASSETS_TYPE_##name, asset); }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) name *TsAssetsRequest##name##ByName(TsAssets *assets, char *name) { return TsAssetsRequestAssetByName(assets, TSASSETS_TYPE_##name, name); }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) void TsAssetsRelease##name##(TsAssets *assets, name *asset) { TsAssetsReleaseAsset(assets, TSASSETS_TYPE_##name, asset); }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) int TsAssetsGet##name##List(TsAssets *assets, name ***list) { *list = (name **)assets->asset_types[TSASSETS_TYPE_##name].linear_iteration_list; return (int)assets->asset_types[TSASSETS_TYPE_##name].count; }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) int TsAssetsGet##name##NameList(TsAssets *assets, char ***list) { *list = assets->asset_types[TSASSETS_TYPE_##name].linear_iteration_name_list; return (int)assets->asset_types[TSASSETS_TYPE_##name].count; }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name_, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) char *TsAssetsGet##name_##Name(TsAssets *assets, name_ *asset)\
{\
int asset_index = asset ? (int)((uintptr_t)asset - (uintptr_t)assets->asset_types[TSASSETS_TYPE_##name_].table) / sizeof(name_) : -1;\
return asset_index >= 0 ? (assets->asset_types[TSASSETS_TYPE_##name_].name_to_index_keys[asset_index].name) : "";\
}
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name_, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) int TsAssetsGet##name_##Index(TsAssets *assets, name_ *asset)\
{\
int asset_index = asset ? (int)((uintptr_t)asset - (uintptr_t)assets->asset_types[TSASSETS_TYPE_##name_].table) / sizeof(name_) : -1;\
return asset_index;\
}
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#define TsAssetsAssetType(name, load_info_name, max, pre_load_callback, load_callback, post_load_callback, clean_up_callback, is_loaded_callback, directory) void TsAssetsForce##name##Reload(TsAssets *assets) { TsAssetsForceAssetTypeReload(assets, TSASSETS_TYPE_##name); }
#include TSASSETS_ASSET_TYPE_FILE
#undef TsAssetsAssetType

#endif // TSASSETS_H_INCLUDED