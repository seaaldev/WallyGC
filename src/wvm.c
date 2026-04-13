#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <sys/mman.h>

#include "wdebug.h"

extern void* 
wgc_virtual_map(void* addr, size_t length) {
  void* map_ptr = mmap(addr, length, PROT_READ, MAP_ANONYMOUS, -1, 0);

  if (map_ptr == MAP_FAILED) {
    handle_error("mmap");
  }

  return map_ptr;
}

extern int
wgc_virtual_unmap(void* addr, size_t length) {
  return (munmap(addr, length));
}
