#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "wvm.h"
#include "wlist.h"
#include "wdebug.h"
#include "warena.h"
#include "wcontainer_of.h"

static 
arena_t* wgc_arena_new(size_t page_count);

static 
void wgc_arena_destroy(arena_t* arena);

static 
void* wgc_arena_alloc(arena_t* arena, size_t bytes);

static 
bool wgc_arena_contains(arena_t* arena, uintptr_t ptr);


/*
 * PLACEHOLDERS FOR FUTURE TUNING FEATURE
 */

#define TUNE_ALIGNMENT 8

static 
arena_t* wgc_arena_new(size_t page_count) 
{
  if (page_count == 0) {
    return 0;
  }

  size_t size = PAGE_SIZE * page_count;
  arena_t* arena = (arena_t*) wgc_virtual_map(0, size);
  
  arena->size      = size;
  arena->allocated = sizeof(*arena) + sizeof(arena->head);
  
  arena->head = (struct list_head)LIST_HEAD_INIT(arena->head);

  return arena;
}

static
void wgc_arena_destroy(arena_t *arena) 
{ 
  list_del(&arena->head);
  wgc_virtual_unmap((void *)arena, arena->size);
}

static 
void* wgc_arena_alloc(arena_t* arena, size_t bytes) 
{
  uintptr_t ret_ptr = (uintptr_t)arena + arena->allocated;
  arena->allocated += bytes;
  return  (void*) ret_ptr;
}

static 
bool wgc_arena_contains(arena_t* arena, uintptr_t ptr)
{
  uintptr_t arena_end = (uintptr_t)arena + (arena->size - 1);

  return (ptr <= arena_end && ptr >= (uintptr_t) arena);
}

static
void wgc_destroy_allocator(alloc_t* allocator)
{
  arena_t *alloc_ctx = allocator->alloc_ctx;
  struct list_head *adv = alloc_ctx->head.prev;
  arena_t *adv_arena = 0;

  do {
    adv_arena = container_of(adv, arena_t, head);
    adv = adv->prev;

    wgc_arena_destroy(adv_arena);
  } while (adv_arena != alloc_ctx);
}

static 
void wgc_alloc_expand(alloc_t *allocator, size_t page_count) 
{
  arena_t *old_arena = allocator->alloc_ctx;

  if (!old_arena || !page_count) {
    return;
  }
  
  arena_t *new = wgc_arena_new(page_count);
  list_add_tail(&new->head, &old_arena->head);
  allocator->alloc_ctx = new;
}

#define ALIGN(number, align_to) \
  (((number) & ~(align_to)) + align_to)

static
void *wgc_alloc(alloc_t *allocator, size_t bytes)
{
  arena_t *ctx = allocator->alloc_ctx;

  size_t alloc_bytes = bytes;
  if (bytes % TUNE_ALIGNMENT != 0)
    alloc_bytes = ALIGN(bytes, TUNE_ALIGNMENT);
  
  size_t remains = ctx->size - ctx->allocated;
  if (remains < alloc_bytes) {
    size_t expand_pages = alloc_bytes / PAGE_SIZE + 1;
    wgc_alloc_expand(allocator, expand_pages);
  }

  return wgc_arena_alloc(ctx, alloc_bytes);
}

static
arena_t *wgc_is_alloc_ptr(alloc_t *allocator, uintptr_t ptr)
{
  arena_t *alloc_ctx = allocator->alloc_ctx;

  struct list_head *adv = &alloc_ctx->head;
  arena_t *adv_arena = 0;

  do {
    adv_arena = container_of(adv, arena_t, head);
    adv = adv->prev;

    if (wgc_arena_contains(adv_arena, ptr))
      return adv_arena;

  } while (adv_arena != alloc_ctx);

  return 0;
}

alloc_t *new_allocator(size_t ini_pages)
{
  if (ini_pages == 0) {
    return 0;
  }

  arena_t *arena = wgc_arena_new(ini_pages);
  
  alloc_t* alloc = wgc_arena_alloc(arena, sizeof(*alloc));
  

  alloc->alloc_ctx    = arena;
  alloc->is_alloc_ptr = wgc_is_alloc_ptr;
  alloc->destroy      = wgc_destroy_allocator;
  alloc->alloc        = wgc_alloc;

  return alloc;
}
