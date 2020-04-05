#include "tsfoundation.h"
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "unistd.h"

internal void
LinuxSaveToFile(char *path, void *data, u32 data_len)
{
	FILE *fp;

	fp = fopen(path, "w+");
	fputs(data, fp);
	fclose(fp);
}

internal void
LinuxAppendToFile(char *path, void *data, u32 data_len)
{
	FILE *fp;

	fp = fopen(path, "a");
	fputs(data, fp);
	fclose(fp);
}

// THIS IS NOT WORKING
// TODO: ADD NON EXISTENCE ERROR HANDLING
internal void
LinuxLoadEntireFile(char *path, void **data, u32 *data_len, b32 error_on_non_existence)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char *full_path[sizeof(path) + 2];
	strcpy(full_path, "..");
	strcat(full_path, path);

	// printf("%s\n", full_path);

	FILE *fp;
	fp = fopen(full_path, "r");
	if (fp == 0)
	{
		// printf("Failed to load file\n");
		// printf("ERROR: %s\n", strerror(errno));
		return 0;
	}
	else
	{
		printf("SUCCESS\n");
	}
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	rewind(fp);

	*data_len = fsize;

	char *read_data = malloc(fsize + 1);
	fread(read_data, 1, fsize, fp);
	fclose(fp);

	*data = read_data;
}

internal char *
LinuxLoadEntireFileAndNullTerminate(char *path)
{
	char *full_path[sizeof(path) + 1];
	strcpy(full_path, "..");
	strcat(full_path, path);

	FILE *fp;
	fp = fopen(full_path, "r");
	if (fp == 0)
	{
		// printf("Failed to load file\n");
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	u32 fsize = ftell(fp);
	rewind(fp);

	char *read_data = malloc(fsize + 1);
	fread(read_data, 1, fsize, fp);
	fclose(fp);

	read_data[fsize] = 0;

	return read_data;
}

internal void
LinuxFreeFileMemory(void *data)
{
	LinuxHeapFree(data);
}

internal void
LinuxDeleteFile(char *path)
{
	remove(path);
}

internal b32
LinuxMakeDirectory(char *path)
{
	if (mkdir(path) == 0)
	{
		return 1;
	}
	return 0;
}

internal b32
LinuxDoesFileExist(char *path)
{
	// This probably isn't the best way
	if (access(path, F_OK) != -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

internal b32
LinuxDoesDirectoryExist(char *path)
{
	return LinuxDoesFileExist(path);
}

internal b32
LinuxCopyFile(char *dest, char *source)
{
	FILE *source_fp;
	fseek(source_fp, 0, SEEK_END);
	u32 fsize = ftell(source_fp);
	rewind(source_fp);

	char *source_data = malloc(fsize + 1);
	fread(source_data, 1, fsize, source_fp);
	fclose(source_fp);

	FILE *dest_fp;
	dest_fp = fopen(dest, "w+");
	fputs(dest_fp, source_data);
	fclose(dest_fp);

	// Return 0 on error
	return 1;
}

internal TsPlatformDirectoryList
LinuxPlatformDirectoryListLoad(char *path, i32 flags)
{
}