/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

typedef struct LinuxAppCode
{
    TsApplicationPermanentLoadCallback  *PermanentLoad;
    TsApplicationHotLoadCallback        *HotLoad;
    TsApplicationHotUnloadCallback      *HotUnload;
    TsApplicationUpdateCallback         *Update;
    void *dll;
}
LinuxAppCode;

internal b32
LinuxAppCodeLoad(LinuxAppCode *app_code)
{
    b32 result = 1;
    
    app_code->dll = dlopen(global_app_dll_path, RTLD_NOW);
    
    if(!app_code->dll)
    {
        result = 0;
        goto end;
    }
    
    app_code->PermanentLoad    = (void *)dlsym(app_code->dll, "PermanentLoad");
    app_code->HotLoad          = (void *)dlsym(app_code->dll, "HotLoad");
    app_code->HotUnload        = (void *)dlsym(app_code->dll, "HotUnload");
    app_code->Update           = (void *)dlsym(app_code->dll, "Update");
    
    if(!app_code->PermanentLoad || !app_code->HotLoad || !app_code->HotUnload || !app_code->Update)
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

internal void
LinuxAppCodeUnload(LinuxAppCode *app_code)
{
    if(app_code->dll)
    {
        dlclose(app_code->dll);
    }
    app_code->dll = 0;
    app_code->PermanentLoad = TsApplicationPermanentLoadStub;
    app_code->HotLoad = TsApplicationHotLoadStub;
    app_code->HotUnload = TsApplicationHotUnloadStub;
    app_code->Update = TsApplicationUpdateStub;
}

internal void
LinuxAppCodeUpdate(LinuxAppCode *app_code)
{}
