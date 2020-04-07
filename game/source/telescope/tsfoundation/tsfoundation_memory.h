/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#define MemoryCopy memcpy
#define MemoryMove memmove

internal void
MemorySet(void *mem, u8 val, u64 size)
{
	for (u32 i = 0; i < size; ++i)
	{
		((u8 *)mem)[i] = val;
	}
}

internal b32
MemoryMatch(void *a, void *b, u32 size)
{
	b32 match = 0;
	if (a && b)
	{
		match = 1;
		u8 *a_byte = a;
		u8 *b_byte = b;
		for (u32 i = 0; i < size; ++i)
		{
			if (a_byte[i] != b_byte[i])
			{
				match = 0;
				break;
			}
		}
	}
	return match;
}

TS_DOC_DESC("Linear allocator state for a fix-sized buffer.")
typedef struct MemoryArena MemoryArena;
struct MemoryArena
{
	void *memory TS_DOC_MEMBER("A pointer to the fix-sized buffer to allocate from.");
	u32 memory_size TS_DOC_MEMBER("The size of the fix-sized buffer to allocate from.");
	u32 memory_alloc_pos TS_DOC_MEMBER("The position from which the next allocation will occur.");
	u32 memory_left TS_DOC_MEMBER("The number of bytes un-allocated in the buffer.");
	u32 minimum_alignment TS_DOC_MEMBER("The alignment guaranteed to be maintained by allocations.");
};

TS_DOC_DESC("Initializes a MemoryArena for allocations to some specified alignment.")
internal MemoryArena
MemoryArenaInitAligned(void *memory TS_DOC_PARAM("The buffer to allocate from."),
					   u32 memory_size TS_DOC_PARAM("The size of the buffer to allocate from."),
					   u32 alignment TS_DOC_PARAM("The minimum alignment to use for allocations."))
{
	MemoryArena arena = {0};
	arena.memory = memory;
	arena.memory_size = memory_size;
	arena.memory_left = memory_size;
	arena.minimum_alignment = alignment;
	return arena;
}

TS_DOC_DESC("Initializes a MemoryArena for allocations to a default alignment. Do not rely on the alignment of this.")
internal MemoryArena
MemoryArenaInit(void *memory TS_DOC_PARAM("The buffer to allocate from."),
				u32 memory_size TS_DOC_PARAM("The size of the buffer to allocate from.."))
{
	MemoryArena arena = MemoryArenaInitAligned(memory, memory_size, 8);
	return arena;
}

TS_DOC_DESC("Performs an allocation from the arena, to some specified alignment. Returns 0 on failure.")
internal void *
MemoryArenaAllocateAligned(MemoryArena *arena TS_DOC_PARAM("The arena to allocate from."),
						   u32 size TS_DOC_PARAM("The size to try to allocate."),
						   u32 alignment TS_DOC_PARAM("The required alignment of the allocation. If less than the memory arena's stored "
													  "minimum alignment, the allocation will fall back to the arena's minimumn alignment."))
{
	void *memory = 0;

	if (alignment < arena->minimum_alignment)
	{
		alignment = arena->minimum_alignment;
	}

	if (arena->memory_alloc_pos + size <= arena->memory_size)
	{
		memory = (void *)((char *)arena->memory + arena->memory_alloc_pos);
		u32 bytes_off_alignment = (uintptr_t)memory % (uintptr_t)alignment;
		if (bytes_off_alignment != 0)
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

TS_DOC_DESC("Performs an allocation from the arena, to some specified alignment, and zero-initializes the memory. Returns 0 on failure.")
internal void *
MemoryArenaAllocateAndZeroAligned(MemoryArena *arena TS_DOC_PARAM("The arena to allocate from."),
								  u32 size TS_DOC_PARAM("The size to try to allocate."),
								  u32 alignment TS_DOC_PARAM("The required alignment of the allocation. If less than the memory arena's stored "
															 "minimum alignment, the allocation will fall back to the arena's minimumn alignment."))
{
	void *memory = MemoryArenaAllocateAligned(arena, size, alignment);
	if (memory)
	{
		MemorySet(memory, 0, size);
	}
	return memory;
}

TS_DOC_DESC("Performs an allocation on the passed arena to the minimum arena alignment. Returns 0 on failure.")
internal void *
MemoryArenaAllocate(MemoryArena *arena TS_DOC_PARAM("The arena to allocate from."),
					u32 size TS_DOC_PARAM("The size to try to allocate."))
{
	return MemoryArenaAllocateAligned(arena, size, 1);
}

TS_DOC_DESC("Performs an allocation on the passed arena to the minimum arena alignment, and zeroes the allocated memory. Returns 0 on failure.")
internal void *
MemoryArenaAllocateAndZero(MemoryArena *arena TS_DOC_PARAM("The arena to allocate from."),
						   u32 size TS_DOC_PARAM("The size to try and allocate."))
{
	return MemoryArenaAllocateAndZeroAligned(arena, size, 1);
}

TS_DOC_DESC("Removes some number of bytes from the end of the portion of the arena buffer that is considered allocated, "
			"allowing the section of the buffer to be allocated from again.")
internal void
MemoryArenaFreeBytes(MemoryArena *arena TS_DOC_PARAM("The arena to free from."),
					 u32 size TS_DOC_PARAM("The number of bytes to free."))
{
	if (size > arena->memory_alloc_pos)
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

TS_DOC_DESC("Resets an arena so that it is considered empty. Do not hold on to any allocations made on the arena after this call has been made.")
internal void
MemoryArenaClear(MemoryArena *arena TS_DOC_PARAM("The arena to clear."))
{
	arena->memory_alloc_pos = 0;
	arena->memory_left = arena->memory_size;
}

TS_DOC_DESC("Clears the fix-sized buffer associated with the passed MemoryArena to all 0's.")
internal void
MemoryArenaZero(MemoryArena *arena TS_DOC_PARAM("The arena to zero."))
{
	MemorySet(arena->memory, 0, arena->memory_size);
}

TS_DOC_DESC("Copies the passed null-terminated C-string on the memory arena, and returns a pointer to the copy. Returns 0 on failure.")
internal char *
MemoryArenaAllocateCStringCopy(MemoryArena *arena TS_DOC_PARAM("The arena to allocate from."),
							   char *str TS_DOC_PARAM("The null-terminated C-string to copy."))
{
	u32 str_length = CalculateCStringLength(str);
	char *str_copy = (char *)MemoryArenaAllocate(arena, str_length + 1);
	if (str_copy)
	{
		MemoryCopy(str_copy, str, str_length);
		str_copy[str_length] = 0;
	}
	return str_copy;
}

TS_DOC_DESC("Allocates bytes off of a MemoryArena, and copies memory into them, effectively "
			"writing into the arena buffer.")
internal b32
MemoryArenaWrite(MemoryArena *arena TS_DOC_PARAM("The arena to write into."),
				 void *source TS_DOC_PARAM("The buffer to copy into the arena buffer."),
				 u32 size TS_DOC_PARAM("The size to write."))
{
	b32 success = 0;
	if (arena->memory_left >= size)
	{
		void *destination = MemoryArenaAllocate(arena, size);
		MemoryCopy(destination, source, size);
		success = 1;
	}
	return success;
}

TS_DOC_DESC("Allocates bytes off of a MemoryArena, and copies memory from them, effectively "
			"reading from the arena buffer.")
internal b32
MemoryArenaRead(MemoryArena *arena TS_DOC_PARAM("The arena to read from."),
				void *destination TS_DOC_PARAM("The buffer to read into."),
				u32 size TS_DOC_PARAM("The number of bytes to read."))
{
	b32 success = 0;
	if (arena->memory_left >= size)
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

TS_DOC_DESC("A pool-allocator for fix-sized chunks out of a fix-sized buffer.")
typedef struct MemoryPool MemoryPool;
struct MemoryPool
{
	void *memory TS_DOC_MEMBER("The buffer from which allocations are made.");
	u32 memory_size TS_DOC_MEMBER("The size of the buffer from which allocations are made..");
	u32 chunk_size TS_DOC_MEMBER("The size of an individual allocation from the pool/");
	u32 free_list_length TS_DOC_MEMBER("The number of free allocations left in the pool.");
	MemoryPoolChunk *free_list_head TS_DOC_MEMBER("The first free allocation remaining.");
};

TS_DOC_DESC("Initializes a MemoryPool for allocations.")
internal MemoryPool
MemoryPoolInit(void *memory TS_DOC_PARAM("The buffer to allocate from."),
			   u32 memory_size TS_DOC_PARAM("The size of the buffer to allocate from."),
			   u32 chunk_size TS_DOC_PARAM("The size of an individual allocation chunk."))
{
	MemoryPool pool = {0};
	pool.memory = memory;
	pool.memory_size = memory_size;
	if (chunk_size < sizeof(MemoryPoolChunk))
	{
		chunk_size = sizeof(MemoryPoolChunk);
	}
	pool.chunk_size = chunk_size;
	u32 number_of_chunks = memory_size / chunk_size;
	for (u32 i = 0; i < number_of_chunks; ++i)
	{
		MemoryPoolChunk *chunk = (void *)((char *)memory + chunk_size * i);

		if (i < number_of_chunks - 1)
		{
			chunk->next = (void *)((char *)memory + chunk_size * (i + 1));
		}
		else
		{
			chunk->next = 0;
		}
	}
	pool.free_list_head = memory;
	return pool;
}

TS_DOC_DESC("Allocates a fix-sized chunk from a MemoryPool. The fixed size is determined "
			"by the initialization of the MemoryPool (see MemoryPoolInit).")
internal void *
MemoryPoolAllocate(MemoryPool *pool TS_DOC_PARAM("The arena to allocate from."))
{
	void *memory = 0;
	if (pool->free_list_head)
	{
		memory = pool->free_list_head;
		pool->free_list_head = pool->free_list_head->next;
	}
	return memory;
}

TS_DOC_DESC("Release a previously-allocated fix-sized chunk of memory back into a MemoryPool "
			"to allow it to be allocated again.")
internal void
MemoryPoolFree(MemoryPool *pool TS_DOC_PARAM("The pool to which the memory belongs."),
			   void *memory TS_DOC_PARAM("The memory to free."))
{
	if ((u64)memory >= (u64)pool->memory && (u64)memory < (u64)pool->memory + pool->memory_size)
	{
		MemoryPoolChunk *old_free_list_head = pool->free_list_head;
		pool->free_list_head = memory;
		pool->free_list_head->next = old_free_list_head;
	}
}

TS_DOC_PARAM("Re-initialize the passed memory pool to allow all chunks to be re-allocated.")
internal void
MemoryPoolClear(MemoryPool *pool TS_DOC_PARAM("The MemoryPool to clear."))
{
	*pool = MemoryPoolInit(pool->memory, pool->memory_size, pool->chunk_size);
}