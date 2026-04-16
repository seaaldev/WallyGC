#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "./../src/warena.h"
#include "./../src/wvm.h"
#include "./../src/wdebug.h"

static int test_alloc_creation() {
  return (new_allocator(1) != 0) ? 1 : 0;
}

static int test_null_alloc_creation() {
  return (new_allocator(0) == 0) ? 1 : 0;
}

static int test_alloc_destroy() {
  alloc_t* allocator = new_allocator(1);
  
  return (allocator->destroy(allocator->alloc_ctx) != 0 ? 1 : 0);
}

static int test_null_alloc_destroy() {
  alloc_t* allocator = new_allocator(1);

  return (allocator->destroy(0) == 0 ? 1 : 0);
}

static int test_allocation() {
  alloc_t* allocator = new_allocator(4);

  size_t used_mem = allocator->alloc_ctx->allocated;
  size_t aval_mem = allocator->alloc_ctx->size - used_mem;
  
  char* teststr = allocator->alloc(allocator->alloc_ctx, aval_mem);
  memset(teststr, 101, aval_mem - 1);
  teststr[aval_mem] = '\0';
  
  allocator->destroy(allocator->alloc_ctx);

  return 1;
}

static int test_null_allocation() {
  alloc_t* allocator = new_allocator(4);

  return (allocator->alloc(allocator->alloc_ctx, 0) == 0 ? 1 : 0);
}

static int test_multi_allocator() {
  size_t page_num  = 12;
  size_t alctr_num =  8;
  
  for (size_t i = 0; i < alctr_num; i++) {
    alloc_t* allocator = new_allocator(page_num);

    size_t used_mem = allocator->alloc_ctx->allocated;
    size_t aval_mem = allocator->alloc_ctx->size - used_mem;

    char* teststr = allocator->alloc(allocator->alloc_ctx, aval_mem);
    
    memset(teststr, 101, aval_mem - 1);
    teststr[aval_mem] = '\0';

    allocator->destroy(allocator->alloc_ctx);
  }
  
  return 1;
}

static int test_isheap_start() {
  alloc_t* allocator = new_allocator(1);
  
  int result = allocator->is_heap_ptr(allocator->alloc_ctx,
                                 (uintptr_t) allocator->alloc_ctx);

  allocator->destroy(allocator->alloc_ctx);

  return result;
}

static int test_isheap_middle() {
  alloc_t* allocator = new_allocator(1);

  uintptr_t alloc_mid = (uintptr_t) allocator + 
                        (allocator->alloc_ctx->size / 2);
  
  int result = allocator->is_heap_ptr(allocator->alloc_ctx, alloc_mid);

  allocator->destroy(allocator->alloc_ctx);

  return result;
}

static int test_isheap_end() {

  alloc_t* allocator = new_allocator(1);

  uintptr_t alloc_end = (uintptr_t) allocator + allocator->alloc_ctx->size - 1;
    
  int result = allocator->is_heap_ptr(allocator->alloc_ctx, alloc_end);

  allocator->destroy(allocator->alloc_ctx);

  return result;
}


static int test_is_not_heap_high() {
  alloc_t* allocator = new_allocator(1);

  uintptr_t high_ptr = (uintptr_t) allocator->alloc_ctx +
                      (allocator->alloc_ctx->size + 1);

  int result = !(allocator->is_heap_ptr(allocator->alloc_ctx, high_ptr));

  allocator->destroy(allocator->alloc_ctx);

  return result;
}

static int test_is_not_heap_low() {
  alloc_t* allocator = new_allocator(1);

  uintptr_t low_ptr = (uintptr_t) allocator->alloc_ctx - 1;

  int result = !(allocator->is_heap_ptr(allocator->alloc_ctx, low_ptr));

  allocator->destroy(allocator->alloc_ctx);

  return result;
}

extern void warena_api_test_runner() {
  char test_success = 1;

  test_success =      test_alloc_creation();
  test_success = test_null_alloc_creation();
  test_success =       test_alloc_destroy();
  test_success =  test_null_alloc_destroy();
  test_success =          test_allocation();
  test_success =     test_null_allocation();
  test_success =     test_multi_allocator();
  test_success =        test_isheap_start();
  test_success =       test_isheap_middle();
  test_success =          test_isheap_end();
  test_success =     test_is_not_heap_low();
  test_success =    test_is_not_heap_high();

  char* test_msg = (test_success) ? "PASS" : "FAIL";
  printf("[%s] %s \n", test_msg, __FILE__);
}
