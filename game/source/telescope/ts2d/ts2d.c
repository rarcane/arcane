/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#include "common/tsrendercommon.c"

void *
_Ts2dAllocateRequestMemory(unsigned int size)
{
    void *memory = 0;
    if(global_ts2d->request_memory_alloc_position + size <= global_ts2d->request_memory_max)
    {
        memory = global_ts2d->request_memory + global_ts2d->request_memory_alloc_position;
        global_ts2d->request_memory_alloc_position += size;
    }
    return memory;
}

void
Ts2dAddRequest(Ts2dRequest request)
{
    Ts2dRequest *target;
    if(global_ts2d->last_request)
    {
        global_ts2d->last_request->next = _Ts2dAllocateRequestMemory(sizeof(Ts2dRequest));
        target = global_ts2d->last_request->next;
    }
    else
    {
        global_ts2d->first_request = _Ts2dAllocateRequestMemory(sizeof(Ts2dRequest));
        target = global_ts2d->first_request;
    }
    request.next = 0;
    *target = request;
    global_ts2d->last_request = target;
}

void
Ts2dInternalFinishActiveRequest(void)
{
    if(global_ts2d->active_request.type != TS2D_REQUEST_null)
    {
        Ts2dAddRequest(global_ts2d->active_request);
        global_ts2d->active_request.type = TS2D_REQUEST_null;
    }
}

#if TS2D_BACKEND == TS2D_OPENGL
#include "ts2d_opengl.c"
#endif