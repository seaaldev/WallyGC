#ifndef WARENA_H
#define WARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef WLIST_H
struct list_head {
  struct list_head *next;
  struct list_head *prev;
};
#endif

typedef struct wgc_arena_t arena_t;

struct wgc_arena_t {
  size_t           size;
  size_t           allocated;
  struct list_head head;
};

typedef struct wgc_allocator_t alloc_t;

struct wgc_allocator_t {
  /*
   * Ignore alloc_ctx. It's
   * only used internally.
   */
  arena_t  *alloc_ctx;

  arena_t* (*is_alloc_ptr)(alloc_t*, uintptr_t); 
  void     (*destroy)(alloc_t*);                
  void*    (*alloc)(alloc_t*, size_t);

};

alloc_t *new_allocator(size_t ini_pages);

#endif
