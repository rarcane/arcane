
TS_APP_PROC TS_APPLICATION_PERMANENT_LOAD(PermanentLoad)
{
    platform = platform_;
    
    // NOTE(rjf): Initialize modules.
    {
        
#ifdef TSDEVTERMINAL
        global_tsdevterminal = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_tsdevterminal), 64);
        HardAssert(global_tsdevterminal != 0);
        TsDevTerminalInit(&platform->permanent_arena);
#endif
        
#ifdef TS2D
        global_ts2d = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_ts2d), 64);
        HardAssert(global_ts2d != 0);
        Ts2dInit(&platform->permanent_arena);
#endif
        
#ifdef TS3D
        global_ts3d = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_ts3d), 64);
        HardAssert(global_ts3d != 0);
        Ts3dInit(&platform->permanent_arena);
#endif
        
#ifdef TSASSETS
        global_tsassets = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_tsassets), 64);
        HardAssert(global_tsassets != 0);
        TsAssetsInit(&platform->permanent_arena);
#endif
        
#ifdef TSINPUT
        global_tsinput = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_tsinput), 64);
        HardAssert(global_tsinput != 0);
        TsInputInit();
#endif
        
#ifdef TSUI
        global_tsui = MemoryArenaAllocateAndZeroAligned(&platform->permanent_arena, sizeof(*global_tsui), 64);
        HardAssert(global_tsui != 0);
        TsUIInit();
#endif
    }
    
    GameInit();
}

TS_APP_PROC TS_APPLICATION_HOT_LOAD(HotLoad)
{
    platform = platform_;
    GameHotLoad();
}

TS_APP_PROC TS_APPLICATION_HOT_UNLOAD(HotUnload)
{
    GameHotUnload();
}

TS_APP_PROC TS_APPLICATION_UPDATE(Update)
{
    
    // NOTE(rjf): Begin frame.
    {
        
        //~
#ifdef TS2D
        Ts2dBeginFrameInfo begin_frame_info = {0};
        {
            begin_frame_info.render_width  = (f32)platform->window_width;
            begin_frame_info.render_height = (f32)platform->window_height;
            begin_frame_info.delta_t       = 1.f / platform->target_frames_per_second;
            begin_frame_info.flags         = 0;
        }
        Ts2dBeginFrame(&begin_frame_info);
#endif
        
        //~
#ifdef TS3D
        Ts3dBeginFrameInfo begin_frame_info = {0};
        {
            begin_frame_info.delta_t       = 1.f / platform->target_frames_per_second;
            begin_frame_info.flags         = 0;
        }
        Ts3dBeginFrame(&begin_frame_info);
#endif
        
        //~
#ifdef TSDEVTERMINAL
        TsDevTerminalUpdate(1.f / platform->target_frames_per_second);
#endif
        
        //~
#ifdef TSINPUT
        TsInputLoad();
#endif
        
        //~
#ifdef TSUI
        TsUIFrameData ui_frame = {0};
        TsUILoadFrameDataFromPlatform(&ui_frame);
        TsUIBeginFrame(&ui_frame);
#endif
        
    }
    
    GameUpdate();
    
    // NOTE(rjf): End frame.
    {
        
        //~
#ifdef TSUI
        TsUIEndFrame();
#endif
        
        //~
#ifdef TSDEVTERMINAL
        TsDevTerminalRender();
#endif
        
        //~
#ifdef TS2D
        Ts2dEndFrame();
        Ts2dPresent();
#endif
        
        //~
#ifdef TS3D
        Ts3dEndFrame();
        Ts3dPresent();
#endif
        
        //~
#ifdef TSASSETS
        TsAssetsUpdate();
#endif
    }
    
}

#ifdef TS2D
#include "ts2d/ts2d.c"
#endif

#ifdef TS3D
#include "ts3d/ts3d.c"
#endif

#ifdef TSASSETS
#include "tsassets/tsassets.c"
#endif

#ifdef TSDEVTERMINAL
#include "tsdevterminal/tsdevterminal.c"
#endif

#ifdef TSINPUT
#include "tsinput/tsinput.c"
#endif

#ifdef TSSCRIPT
#include "tsscript/tsscript.c"
#endif

#ifdef TSUI
#include "tsui/tsui.c"
#endif
