/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#include "stdint.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define MAX_PATH 4096

void _TsAssetsLoadDirectoryItems(char *path, int *item_count_ptr, char ***items_ptr)
{
	int item_count = 0;
	int item_capacity = 0;
	char **items = 0;

	char task_memory[Kilobytes(32)];
	MemoryArena arena_ = MemoryArenaInit(task_memory, sizeof(task_memory));
	MemoryArena *arena = &arena_;

	typedef struct DirectorySearchTask DirectorySearchTask;
	struct DirectorySearchTask
	{
		char *sub_search_pattern;
		DirectorySearchTask *next;
	};
	DirectorySearchTask *first_search_task = 0;
	DirectorySearchTask **target_search_task = &first_search_task;

#define QueueTask(path)                                                                \
	{                                                                                  \
		DirectorySearchTask *new_task = MemoryArenaAllocate(arena, sizeof(*new_task)); \
		if (new_task != 0)                                                             \
		{                                                                              \
			new_task->sub_search_pattern = path;                                       \
			new_task->next = 0;                                                        \
			*target_search_task = new_task;                                            \
			target_search_task = &(*target_search_task)->next;                         \
		}                                                                              \
		else                                                                           \
		{                                                                              \
			LogWarning("[Assets] Directory search task space exhausted.");             \
		}                                                                              \
	}

	QueueTask("");

	for (DirectorySearchTask *task = first_search_task; task; task = task->next)
	{
		// NOTE(rjf): Load file list
		{
			int32_t error = 0;
			char search_pattern[MAX_PATH] = {0};
			char find_file_path[MAX_PATH] = {0};
			snprintf(search_pattern, MAX_PATH, "%s\\%s\\%s*", path, path, task->sub_search_pattern);

			struct dirent *pEntry;
			do
			{
				char full_path[MAX_PATH];
				strcpy(full_path, path);
				strcat(full_path, "/");
				strcat(full_path, pEntry->d_name);

				FILE *fp;
				struct stat path_stat;
				stat(full_path, &path_stat);

				if (S_ISREG(path_stat.st_mode) == 1)
				{
					if (!CStringMatchCaseSensitive(pEntry->d_name, ".") &&
						!CStringMatchCaseSensitive(pEntry->d_name, ".."))
					{
						char directory_name[MAX_PATH] = {0};
						TsAssetsMemoryCopy(directory_name, pEntry->d_name, sizeof(directory_name));
						QueueTask(MakeCStringOnMemoryArena(arena, "%s%s\\", task->sub_search_pattern, directory_name));
					}
				}
				else
				{
					// NOTE(rjf): Make room for new item if we need to.
					{
						if (item_count + 1 >= item_capacity)
						{
							if (item_capacity == 0)
							{
								item_capacity = 64;
							}
							else
							{
								item_capacity <<= 1;
							}
							char **new_items = TsAssetsHeapAllocate(item_capacity * sizeof(char *));
							TsAssetsMemoryCopy(new_items, items, sizeof(char *) * item_count);
							if (items)
							{
								TsAssetsHeapFree(items);
							}
							items = new_items;
						}
					}

					// NOTE(rjf): Allocate new item.
					char *new_item = 0;
					{
						TsAssetsMemoryCopy(find_file_path, pEntry->d_name, sizeof(find_file_path));
						unsigned int needed_bytes = CalculateCStringLength(find_file_path) + CalculateCStringLength(task->sub_search_pattern) + 1;
						new_item = TsAssetsHeapAllocate(needed_bytes + 1);
						snprintf(new_item, needed_bytes, "%s%s", task->sub_search_pattern, find_file_path);
						new_item[needed_bytes] = 0;
					}

					// NOTE(rjf): Remove extension from path.
					{
						char *last_period = new_item;
						for (int i = 0; new_item[i]; ++i)
						{
							if (new_item[i] == '.')
							{
								last_period = new_item + i;
							}
						}

						if (last_period != new_item || new_item[0] == '.')
						{
							*last_period = 0;
						}
					}

					// NOTE(rjf): Replace backslashes with forward slashes.
					{
						for (int i = 0; new_item[i]; ++i)
						{
							if (new_item[i] == '\\')
							{
								new_item[i] = '/';
							}
						}
					}

					// NOTE(rjf): Add item.
					{
						items[item_count++] = new_item;
					}
				}
			} while (pEntry = readdir(path));
		}
	}

#undef QueueTask

	*item_count_ptr = item_count;
	*items_ptr = items;
}

void _TsAssetsFreeDirectoryItems(char **items)
{
	TsAssetsHeapFree(items);
}

void _TsAssetsInitWatchDirectories(void)
{
	// TODO: IMPLEMENT THIS
	// _TsAssetsInitWatchDirectoryNames(assets);
	// for (int i = 0; i < TSASSETS_TYPE_MAX; ++i)
	// {
	// 	TsAssetsWatchDirectory *watch_directory = assets->asset_directories + i;
	// 	char path[MAX_PATH] = {0};
	// 	snprintf(path, sizeof(path), "%s\\%s\\", assets->assets_root_path, watch_directory->path);
	// 	watch_directory->directory_handle = CreateFileA(path, FILE_LIST_DIRECTORY,
	// 													FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
	// 													0,
	// 													OPEN_EXISTING,
	// 													FILE_FLAG_OVERLAPPED | FILE_FLAG_BACKUP_SEMANTICS,
	// 													0);

	// 	if (watch_directory->directory_handle == INVALID_HANDLE_VALUE)
	// 	{
	// 		goto fail;
	// 	}
	// 	else
	// 	{
	// 		watch_directory->overlapped_io_event = CreateEventA(0, FALSE, FALSE, 0);
	// 		watch_directory->overlapped_data.hEvent = watch_directory->overlapped_io_event;

	// 		if (ReadDirectoryChangesW(watch_directory->directory_handle,
	// 								  &watch_directory->file_notify_information,
	// 								  sizeof(watch_directory->file_notify_information),
	// 								  FALSE,
	// 								  FILE_NOTIFY_CHANGE_FILE_NAME |
	// 									  FILE_NOTIFY_CHANGE_LAST_WRITE |
	// 									  FILE_NOTIFY_CHANGE_CREATION,
	// 								  &watch_directory->bytes_returned,
	// 								  &watch_directory->overlapped_data,
	// 								  0))
	// 		{
	// 			// NOTE(rjf): Success
	// 		}
	// 		else
	// 		{
	// 			goto fail;
	// 		}
	// 	}

	// 	goto end;
	// fail:;
	// 	{
	// 		// TODO(rjf): Error.
	// 	}
	// end:;
	// }
}

void _TsAssetsCheckAssetDirectories(void)
{
	// TODO: IMPLEMENT THIS
	// for (int i = 0; i < TSASSETS_TYPE_MAX; ++i)
	// {
	// 	TsAssetsWatchDirectory *watch_directory = assets->asset_directories + i;
	// 	if (GetOverlappedResult(watch_directory->directory_handle,
	// 							&watch_directory->overlapped_data,
	// 							&watch_directory->bytes_returned,
	// 							FALSE))
	// 	{
	// 		_TsAssetsRefreshDirectory(assets, (TsAssetsType)i);

	// 		char modified_asset_file[MAX_PATH] = {0};
	// 		wcstombs(modified_asset_file, watch_directory->file_notify_information.FileName, sizeof(modified_asset_file));

	// 		if (ReadDirectoryChangesW(watch_directory->directory_handle,
	// 								  &watch_directory->file_notify_information,
	// 								  sizeof(watch_directory->file_notify_information),
	// 								  FALSE,
	// 								  FILE_NOTIFY_CHANGE_FILE_NAME |
	// 									  FILE_NOTIFY_CHANGE_LAST_WRITE |
	// 									  FILE_NOTIFY_CHANGE_CREATION,
	// 								  &watch_directory->bytes_returned,
	// 								  &watch_directory->overlapped_data,
	// 								  0))
	// 		{
	// 			// NOTE(rjf): Success
	// 		}
	// 		else
	// 		{
	// 			// TODO(rjf): Error.
	// 		}
	// 	}
	// }
}
