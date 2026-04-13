#ifndef WVM_H
#define WVM_H

#include <stddef.h>
#include <stdint.h>


//TODO: Make pagesize
//system pagesize conf depedendant
#define PAGE_SIZE 4096

void* wgc_virtual_map(void* addr, size_t length);

extern void wgc_virtual_unmap(void* addr, size_t length);

#endif
