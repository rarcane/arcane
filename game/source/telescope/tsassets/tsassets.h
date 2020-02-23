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

// TODO(rjf): BREAK THIS INTO PLATFORM LAYER!!! I don't want platform
// shit in the modules, it makes it hard to port later
#if TSASSETS_WIN32
#define WIN32_LEAN_AND_MEAN
#include "tsfoundation/tsfoundation_windows.h"
#undef DeleteFile
#include "tsassets_win32.h"
#endif

#ifndef TSASSETS_ASSET_ROOT_PATH
#define TSASSETS_ASSET_ROOT_PATH "data"
#endif

#ifndef TSASSETS_ASSET_TYPE_FILE
#define TSASSETS_ASSET_TYPE_FILE "tsassets_type_list.inc"
#endif

typedef struct TsAssets TsAssets;
global TsAssets *global_tsassets = 0;

typedef struct TsAssetTableKey TsAssetTableKey;
struct TsAssetTableKey
{
    char *name;
    uint8_t deleted;
};

typedef void   TsAssetsMainThread_PreLoadCallback   (char *source_file, void *asset);
typedef void   TsAssetsLoadThread_LoadCallback      (char *source_file, void *asset_load_thread_info);
typedef void   TsAssetsMainThread_PostLoadCallback  (void *asset_load_thread_info, void *asset);
typedef void   TsAssetsMainThread_CleanUpAssetCallback(void *asset);
typedef int    TsAssetsIsAssetLoadedCallback(void *asset);

typedef struct TsAssetsLoadAssetInfo TsAssetsLoadAssetInfo;
struct TsAssetsLoadAssetInfo
{
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

typedef struct TsAssetType TsAssetType;
struct TsAssetType
{
    char *name;
    char *load_info_name;
    char *asset_folder_name;
    u32 size_of_asset_type;
    u32 size_of_asset_load_info_type;
    u32 max;
    TsAssetsMainThread_PreLoadCallback      *PreLoad;
    TsAssetsLoadThread_LoadCallback         *Load;
    TsAssetsMainThread_PostLoadCallback     *PostLoad;
    TsAssetsMainThread_CleanUpAssetCallback *CleanUp;
    TsAssetsIsAssetLoadedCallback           *IsLoaded;
};

#define TSASSETS_MAX_ASSET_PATH_LENGTH 128
typedef struct TsAssetsTypeData TsAssetsTypeData;
struct TsAssetsTypeData
{
    char *name;
    char *load_info_name;
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

#define TSASSETS_TYPE_INVALID (-1)

typedef struct TsAssets TsAssets;
struct TsAssets
{
    u32 number_of_assets_queued_for_update[TSASSETS_PRIORITY_MAX];
    u32 number_of_assets_complete[TSASSETS_PRIORITY_MAX];
    TsAssetsPriority active_priority;
    int asset_type_count;
    TsAssetsTypeData *asset_types;
    char *assets_root_path;
    TsAssetsWatchDirectory *asset_directories;
    b32 clean_up_disabled;
};

void TsAssetsInit(MemoryArena *arena);
void TsAssetsSetAssetRootPath(char *asset_root_path);
void TsAssetsSetAssetTypes(int asset_type_count, TsAssetType *asset_types, MemoryArena *arena);
void TsAssetsCleanUp(void);
void TsAssetsUpdate(void);
int TsAssetsGetAssetTypeFromName(char *name);
int TsAssetsGetAssetType(void *asset);
void *TsAssetsGetAssetByName(int type, char *name);
void TsAssetsSetRequestPriority(TsAssetsPriority priority);
u32 TsAssetsGetAssetsToLoadForPriority(TsAssetsPriority priority);
u32 TsAssetsGetAssetsToLoad(void);
void *TsAssetsRequestAsset(void *asset);
void *TsAssetsRequestAssetByName(int type, char *name);
void TsAssetsReleaseAsset(void *asset);
char *TsAssetsGetAssetName(void *asset);
int TsAssetsGetAssetList(int asset_type, void ***list);
int TsAssetsGetAssetNameList(int asset_type, char ***list);
int TsAssetsGetAssetIndex(void *asset);
char *TsAssetsGetAssetName(void *asset);
void *TsAssetsGetDefaultAsset(int asset_type);
void TsAssetsSetDefaultAsset(int asset_type, void *asset);
void TsAssetsForceAssetTypeReload(int type);
void TsAssetsEnableAssetCleanUp(void);
void TsAssetsDisableAssetCleanUp(void);

#endif // TSASSETS_H_INCLUDED