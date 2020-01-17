/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

#define MemoryCopy                 memcpy
#define MemoryMove                 memmove

internal void
MemorySet(void *mem, u8 val, u64 size)
{
    for(u32 i = 0; i < size; ++i)
    {
        ((u8 *)mem)[i] = val;
    }
}

typedef struct MemoryArena MemoryArena;
struct MemoryArena
{
    void *memory;
    u32 memory_size;
    u32 memory_alloc_pos;
    u32 memory_left;
    u32 minimum_alignment;
};

internal MemoryArena
MemoryArenaInitAligned(void *memory, u32 memory_size, u32 alignment)
{
    MemoryArena arena = {0};
    arena.memory = memory;
    arena.memory_size = memory_size;
    arena.memory_left = memory_size;
    arena.minimum_alignment = alignment;
    return arena;
}

internal MemoryArena
MemoryArenaInit(void *memory, u32 memory_size)
{
    MemoryArena arena = MemoryArenaInitAligned(memory, memory_size, 8);
    return arena;
}

internal void *
MemoryArenaAllocateAligned(MemoryArena *arena, u32 size, u32 alignment)
{
    void *memory = 0;
    
    if(alignment < arena->minimum_alignment)
    {
        alignment = arena->minimum_alignment;
    }
    
    if(arena->memory_alloc_pos + size <= arena->memory_size)
    {
        memory = (void *)((char *)arena->memory + arena->memory_alloc_pos);
        u32 bytes_off_alignment = (uintptr_t)memory % (uintptr_t)alignment;
        if(bytes_off_alignment != 0)
        {
            u32 padding = alignment - bytes_off_alignment;
            memory = (char *)memory + padding;
            size += padding;
        }
        arena->memory_alloc_pos += size;
        arena->memory_left -= size;
    }
    
    return memory;
}

internal void *
MemoryArenaAllocateAndZeroAligned(MemoryArena *arena, u32 size, u32 alignment)
{
    void *memory = MemoryArenaAllocateAligned(arena, size, alignment);
    if(memory)
    {
        MemorySet(memory, 0, size);
    }
    return memory;
}

internal void *
MemoryArenaAllocate(MemoryArena *arena, u32 size)
{
    return MemoryArenaAllocateAligned(arena, size, 1);
}

internal void *
MemoryArenaAllocateAndZero(MemoryArena *arena, u32 size)
{
    return MemoryArenaAllocateAndZeroAligned(arena, size, 1);
}

internal void
MemoryArenaFreeBytes(MemoryArena *arena, u32 size)
{
    if(size > arena->memory_alloc_pos)
    {
        arena->memory_alloc_pos = 0;
        arena->memory_left = arena->memory_size;
    }
    else
    {
        arena->memory_alloc_pos -= size;
        arena->memory_left += size;
    }
}

internal void
MemoryArenaClear(MemoryArena *arena)
{
    arena->memory_alloc_pos = 0;
    arena->memory_left = arena->memory_size;
}

internal void
MemoryArenaZero(MemoryArena *arena)
{
    MemorySet(arena->memory, 0, arena->memory_size);
}

internal char *
MemoryArenaAllocateCStringCopy(MemoryArena *arena, char *str)
{
    u32 str_length = CalculateCStringLength(str);
    char *str_copy = (char *)MemoryArenaAllocate(arena, str_length+1);
    MemoryCopy(str_copy, str, str_length);
    str_copy[str_length] = 0;
    return str_copy;
}

internal b32
MemoryArenaWrite(MemoryArena *arena, void *source, u32 size)
{
    b32 success = 0;
    if(arena->memory_left >= size)
    {
        void *destination = MemoryArenaAllocate(arena, size);
        MemoryCopy(destination, source, size);
        success = 1;
    }
    return success;
}

internal b32
MemoryArenaRead(MemoryArena *arena, void *destination, u32 size)
{
    b32 success = 0;
    if(arena->memory_left >= size)
    {
        void *source = MemoryArenaAllocate(arena, size);
        MemoryCopy(destination, source, size);
        success = 1;
    }
    return success;
}

typedef struct MemoryPoolChunk MemoryPoolChunk;
struct MemoryPoolChunk
{
    MemoryPoolChunk *next;
};

typedef struct MemoryPool MemoryPool;
struct MemoryPool
{
    void *memory;
    u32 memory_size;
    u32 chunk_size;
    u32 free_list_length;
    MemoryPoolChunk *free_list_head;
};

internal MemoryPool
MemoryPoolInit(void *memory, u32 memory_size, u32 chunk_size)
{
    MemoryPool pool = {0};
    pool.memory = memory;
    pool.memory_size = memory_size;
    if(chunk_size < sizeof(MemoryPoolChunk))
    {
        chunk_size = sizeof(MemoryPoolChunk);
    }
    pool.chunk_size = chunk_size;
    u32 number_of_chunks = memory_size / chunk_size;
    for(u32 i = 0; i < number_of_chunks; ++i)
    {
        MemoryPoolChunk *chunk = (void *)((char *)memory + chunk_size*i);
        
        if(i < number_of_chunks - 1)
        {
            chunk->next = (void *)((char *)memory + chunk_size*(i+1));
        }
        else
        {
            chunk->next = 0;
        }
    }
    pool.free_list_head = memory;
    return pool;
}

internal void *
MemoryPoolAllocate(MemoryPool *pool)
{
    void *memory = 0;
    if(pool->free_list_head)
    {
        memory = pool->free_list_head;
        pool->free_list_head = pool->free_list_head->next;
    }
    return memory;
}

internal void
MemoryPoolFree(MemoryPool *pool, void *memory)
{
    if((u64)memory >= (u64)pool->memory && (u64)memory < (u64)pool->memory + pool->memory_size)
    {
        MemoryPoolChunk *old_free_list_head = pool->free_list_head;
        pool->free_list_head = memory;
        pool->free_list_head->next = old_free_list_head;
    }
}

internal void
MemoryPoolClear(MemoryPool *pool)
{
    *pool = MemoryPoolInit(pool->memory, pool->memory_size, pool->chunk_size);
}