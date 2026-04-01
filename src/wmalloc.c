#include <stdio.h> // Needed for printf
#include <stddef.h> // Needed for size_t

void *wgc_malloc(size_t size) {
  printf("Allocated a block the size of %ld"
        " congrats!\n", size);

  return 0;
}

void wgc_free(void *ptr) {
  printf("Freed the block at address %p"
         " congrats!\n", ptr);

  return;
}
