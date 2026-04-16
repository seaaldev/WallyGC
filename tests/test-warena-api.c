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

/* 
 * Im silly so I cannot think of a way to
 * meassure an allocations success other
 * than it not segfaulting
 */
static int test_allocation() {
  alloc_t* allocator = new_allocator(4);

  size_t used_mem = allocator->alloc_ctx->allocated;
  size_t aval_mem = allocator->alloc_ctx->size - used_mem;
  
  char* test = allocator->alloc(allocator->alloc_ctx, aval_mem);
  memset(test, 101, aval_mem - 1);
  long_string[aval_mem] = '\0';
  
  allocator->destroy(allocator->alloc_ctx);

  return 1;
}

/*
 * Same idea here! No SEGFAULT No STRESS
 */ 

static int test_multi_allocator() {
  size_t page_num  = 12;
  size_t alctr_num =  8;
  
  for (size_t i = 0; i < alctr_num; i++) {
    alloc_t* allocator = new_allocator(page_num);

    size_t used_mem = allocator->alloc_ctx->allocated;
    size_t aval_mem = allocator->alloc_ctx->size - used_mem;

    char* teststr = allocator->alloc(allocator->alloc_ctx, aval_mem);
    
    memset(teststr, 101, aval_mem - 1);
    test[aval_mem] = '\0';

    allocator->destroy(allocator->alloc_ctx);
  }
  
  return 1;
}

extern void warena_api_test_runner() {
  char test_success = 1;

  test_success =  test_alloc_creation();
  test_success =      test_allocation();
  test_success = test_multi_allocator();

  char* test_msg = (test_success) ? "PASS" : "FAIL";
  printf("[%s] %s \n", test_msg, __FILE__);
}
