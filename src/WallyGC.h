#ifndef WALLYGC_H
#define WALLYGC_H

#include <stddef.h>

extern void* wgc_malloc(size_t size);

extern void wgc_free(void *ptr);

#endif
