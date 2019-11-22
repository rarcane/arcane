void *
_Ts2dAllocateRequestMemory(Ts2d *renderer, unsigned int size)
{
    void *memory = 0;
    if(renderer->request_memory_alloc_position + size <= renderer->request_memory_max)
    {
        memory = renderer->request_memory + renderer->request_memory_alloc_position;
        renderer->request_memory_alloc_position += size;
    }
    return memory;
}

void
Ts2dAddRequest(Ts2d *renderer, Ts2dRequest request)
{
    Ts2dRequest *target;
    if(renderer->last_request)
    {
        renderer->last_request->next = _Ts2dAllocateRequestMemory(renderer, sizeof(Ts2dRequest));
        target = renderer->last_request->next;
    }
    else
    {
        renderer->first_request = _Ts2dAllocateRequestMemory(renderer, sizeof(Ts2dRequest));
        target = renderer->first_request;
    }
    request.next = 0;
    *target = request;
    renderer->last_request = target;
}

void
Ts2dInternalFinishActiveRequest(Ts2d *renderer)
{
    if(renderer->active_request.type != TS2D_REQUEST_null)
    {
        Ts2dAddRequest(renderer, renderer->active_request);
        renderer->active_request.type = TS2D_REQUEST_null;
    }
}

#if TS2D_BACKEND == TS2D_OPENGL
#include "ts2d_opengl.c"
#endif