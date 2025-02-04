#pragma once

#include <stddef.h>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT sizeof(void *)
#endif

struct Arena {
    unsigned char *buf;
    size_t buf_len;
    size_t curr_offset;
    size_t prev_offset;
};
typedef struct Arena Arena;

void arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length);
void arena_free_all(Arena *a);

void *arena_alloc(Arena *a, size_t size);
void *arena_alloc_align(Arena *a, size_t size, size_t align);

void *arena_resize(Arena *a, void *old_memory, size_t old_size,
                   size_t new_size);
void *arena_resize_align(Arena *a, void *old_memory, size_t old_size,
                         size_t new_size, size_t align);

void mem_init(void *backing_buffer, size_t backing_buffer_length);
void mem_reset(void);
void *mem_alloc(size_t size);
void *mem_alloc_align(size_t size, size_t align);
void *mem_resize(void *old_memory, size_t old_size, size_t new_size);
void *mem_resize_align(void *old_memory, size_t old_size, size_t new_size,
                 size_t align);

void temp_init(void *backing_buffer, size_t backing_buffer_length);
void temp_reset(void);
void *temp_alloc(size_t size);
void *temp_alloc_align(size_t size, size_t align);
