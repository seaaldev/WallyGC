#ifndef WARENA_H
#define WARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct arena_t arena_t;

struct arena_t;

typedef struct wgc_allocator_t {
  arena_t* alloc_ctx;


  /* Always pass in alloc_ctx as the arena_t parameter */
  void     (*destroy)(arena_t*);                // unmaps the whole arena
  void*    (*alloc)(arena_t*, size_t);          // Allocates n(size_t) bytes 
  void     (*expand)(arena_t*, size_t);         // Creates more mappings n(size_t)
  int      (*is_heap_ptr)(arena_t*, uintptr_t); // Returns 1 if alloc contains
                                               // the ptr & 0 if not

} alloc_t;

extern alloc_t* new_allocator(size_t ini_pages);

#endif
