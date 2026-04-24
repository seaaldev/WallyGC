#ifndef WCONTAINER_OF_H
#define WCONTAINER_OF_H

#include <stdint.h>

#define container_of(ptr, type, member) ({                  \
  uintptr_t _memptr = (uintptr_t) ptr;                      \
  ((type*)(_memptr - __builtin_offsetof(type, member))); }) \

#endif // WCONTAINER_OF_H
