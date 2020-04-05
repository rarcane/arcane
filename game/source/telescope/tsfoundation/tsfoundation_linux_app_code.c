#include "tsfoundation.h"
#include "dlfcn.h"

typedef struct LinuxAppCode
{
	TsApplicationPermanentLoadCallback *PermanentLoad;
	TsApplicationHotLoadCallback *HotLoad;
	TsApplicationHotUnloadCallback *HotUnload;
	TsApplicationUpdateCallback *Update;
	void *so
} LinuxAppCode;

internal b32
LinuxAppCodeLoad(LinuxAppCode *app_code)
{
	b32 result = 1;
	char buf[4096];
	printf(getcwd(buf, sizeof(buf)));
#if 1 // || BUILD_RELEASE
	app_code->so = dlopen("./arcane.so", RTLD_NOW);

#else
	// CopyFile(global_app_dll_path, global_temp_app_dll_path, FALSE);
	// app_code->dll = LoadLibraryA(global_temp_app_dll_path);
	app_code->so = dlopen(global_app_so_path);
#endif

	// IMPLEMENT THIS
	// app_code->last_dll_write_time = Win32GetLastWriteTime(global_app_dll_path);
	if (app_code->so == 0)
	{
		printf("Failed to load so: %s\n", dlerror());
		result = 0;
		goto end;
	}
	else
	{
		fprintf(stdout, "\nLoaded successfully!\n");
	}

	app_code->PermanentLoad = dlsym(app_code->so, "PermanentLoad");
	app_code->HotLoad = dlsym(app_code->so, "HotLoad");
	app_code->HotUnload = dlsym(app_code->so, "HotUnload");
	app_code->Update = dlsym(app_code->so, "Update");

	if (!app_code->PermanentLoad || !app_code->HotLoad || !app_code->HotUnload || !app_code->Update)
	{
		app_code->PermanentLoad = TsApplicationPermanentLoadStub;
		app_code->HotLoad = TsApplicationHotLoadStub;
		app_code->HotUnload = TsApplicationHotUnloadStub;
		app_code->Update = TsApplicationUpdateStub;
		result = 0;
		goto end;
	}

end:;
	return result;
}
