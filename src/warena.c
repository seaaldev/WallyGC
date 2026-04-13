#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "wvm.h"
#include "wdebug.h"
#include "warena.h"


static void wgc_arena_ini(arena_t* arena);

static arena_t* wgc_arena_new(size_t page_count);

static void
wgc_arena_expand(arena_t* arena, size_t page_count);

static void*
wgc_arena_alloc(arena_t* arena, size_t bytes);

static arena_t* wgc_arena_new();


#define ALIGNMENT(a) \
(assert((a) % sizeof(uintptr_t) == 0))

struct arena_t {
  size_t      size;
  size_t allocated;
};

static void wgc_arena_ini(arena_t* arena) {
  arena->size      =    0; // Arena byte length
  arena->allocated =    0;

  return;
}

// Should at somepoint return 0 on success and 1 on failure
// or do something with errno that's for later

static arena_t* wgc_arena_new(size_t ini_pages) {
  size_t size = PAGE_SIZE * ini_pages;
  arena_t* arena = (arena_t*)wgc_virtual_map(0, size);

  wgc_arena_ini(arena);
  arena->size = size;
  arena->allocated = sizeof(*arena); 

  return arena;
}

static void wgc_arena_destroy(arena_t* arena) { 
  wgc_virtual_unmap((void*)arena, arena->size);
}

static void*
wgc_arena_alloc(arena_t* arena, size_t bytes) {
  if (arena ==  0) {
    handle_error("wgc_arena_alloc");
  }

  if ((arena->size - arena->allocated) < bytes ) {
    wgc_arena_expand(arena, (arena->size % PAGE_SIZE) + 1);
  }

  arena->allocated += bytes;
  return  (void*)((uintptr_t)arena + arena->size);
}

static void 
wgc_arena_expand(arena_t* arena, size_t page_count) {
  if (arena == 0 || page_count == 0) {
    handle_error("wgc_arena_expand");
  }
  /* NOTE:
   * We assume that pages mapped aligned
   * all the time. This will be fixed 
   * later!
   */
  size_t size = PAGE_SIZE * page_count;
  uintptr_t end_ptr = (uintptr_t)arena + arena->size;
  wgc_virtual_map((void*)end_ptr, size);

  arena->size += size;

}


static int 
wgc_arena_contains(arena_t* arena, uintptr_t ptr) {
  uintptr_t arena_end = (uintptr_t) arena + arena->size;

  return (ptr <= arena_end && ptr >= (uintptr_t) arena);
}

extern alloc_t* new_allocator(size_t ini_pages) {
  arena_t* arena = wgc_arena_new(ini_pages);
  
  alloc_t* alloc = wgc_arena_alloc(arena, sizeof(*alloc));
  

  alloc->alloc_ctx   = arena;
  alloc->destroy     = wgc_arena_destroy;
  alloc->alloc       = wgc_arena_alloc;
  alloc->expand      = wgc_arena_expand;
  alloc->is_heap_ptr = wgc_arena_contains;

  return alloc;
}
