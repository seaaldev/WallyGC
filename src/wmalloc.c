#include <stdio.h> // Needed for printf
#include <stddef.h> // Needed for size_t

#include "warena.h"

/*typedef struct _wgc_hdr_t hdr_t;

#define MASK_FREE   0x4
#define MASK_MARKED 0x7

struct _wgc_hdr_t {
  uintptr_t      next;
  size_t flagged_size;
};

static void wgc_hdr_ini(hdr_t* h, size_t size) {
  h->next         = 0;
  h->flagged_size = 0;
}

static hdr_t* wgc_hdr_new(arena_t* arena, size_t size) {
  ALIGNMENT(sizeof(*h + size));
}

// static arena_t* _wgc_mstate
*/
extern void* wgc_malloc(size_t size) {
  printf("Allocated a block the size of %ld"
        " congrats!\n", size);
  return 0;
}

extern void wgc_free(void *ptr) {
  printf("Freed the block at address %p"
         " congrats!\n", ptr);

  return;
}

