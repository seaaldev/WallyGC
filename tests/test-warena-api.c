#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "./../src/warena.h"
#include "./../src/wvm.h"
#include "./../src/wdebug.h"

static int test_alloc_creation() {
  alloc_t* mockalloc = new_allocator(4);

  return (mockalloc->alloc_ctx->size == (4 * PAGE_SIZE));
}

/* Im silly so I cannot think of a way to
 * meassure an allocations success other
 * than it not segfaulting
 */
static int test_allocation() {
  alloc_t* mockalloc = new_allocator(4);

  size_t size = mockalloc->alloc_ctx->size - sizeof(*mockalloc)
                - sizeof(arena_t);
  
  char* long_string = mockalloc->alloc(mockalloc->alloc_ctx, size);
  memset(long_string, 67, (3 * PAGE_SIZE) - 1);
  long_string[3*PAGE_SIZE] = '\0';

  return 1;
}

extern void warena_api_test_runner() {
  char** test_msgs;

  if (!test_alloc_creation()) {
    test_msgs[0] = "[TEST] Allocator Creation failed \n";
  }

  // This will never happen cause its just
  // going to segfault if it fails :D
  // Don't know why im adding it
  // but I am :D
  if (!test_allocation()) {
    test_msgs[1] = "[TEST] Allocation failed \n";
  }

  for (int i = 0; test_msgs[i] != 0; i++) {
    fprintf(stderr, "%d - %s - %s",
            __LINE__, __FILE__, test_msgs[i]);
  }
}
