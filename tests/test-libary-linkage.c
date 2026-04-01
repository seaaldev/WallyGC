#include "../src/WallyGC.h"

int main(void) {
  void *ptr = wgc_malloc(100);
  wgc_free(ptr);

  return 0;
}
