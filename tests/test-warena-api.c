#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "./../src/warena.h"
#include "./../src/wvm.h"
#include "./../src/wdebug.h"

static 
bool test_alloc_destroy(void) 
{
  alloc_t *allocator = new_allocator(1);
  allocator->destroy(allocator);

  return true;
}

static 
bool test_alloc_creation(void) 
{
  alloc_t *allocator = new_allocator(1);
  bool ret = (allocator != 0);
  allocator->destroy(allocator);

  return ret;
}

static 
bool test_null_alloc_creation(void) 
{
  alloc_t *allocator = new_allocator(1);
  bool ret = (allocator != 0);
  allocator->destroy(allocator);
  
  return ret;
}

static 
bool test_allocation(void) 
{
  alloc_t *allocator = new_allocator(4);

  size_t used_mem = allocator->alloc_ctx->allocated;
  size_t aval_mem = allocator->alloc_ctx->size - used_mem;
  
  char *teststr = allocator->alloc(allocator, aval_mem);
  memset(teststr, 101, aval_mem - 2);
  teststr[aval_mem - 1] = '\0';
  
  allocator->destroy(allocator);

  return true;
}

static 
bool test_many_allocators(void) 
{
  size_t page_num  = 12;
  size_t alctr_num = 48;
  
  for (size_t i = 0; i < alctr_num; i++) {
    alloc_t *allocator = new_allocator(page_num);

    size_t used_mem = allocator->alloc_ctx->allocated;
    size_t aval_mem = allocator->alloc_ctx->size - used_mem;

    char *teststr = allocator->alloc(allocator, aval_mem);
    
    memset(teststr, 101, aval_mem - 2);
    teststr[aval_mem - 1] = '\0';

    allocator->destroy(allocator);
  }

  return true;
}

static 
bool test_alloc_expand(void) 
{
  bool ret = true;

  alloc_t *allocator = new_allocator(1);
  arena_t *check_ctx  = allocator->alloc_ctx;

  allocator->alloc(allocator, PAGE_SIZE * 3);
  ret &= (allocator->alloc_ctx != check_ctx);
  check_ctx = allocator->alloc_ctx; 

  allocator->alloc(allocator, PAGE_SIZE * 6);
  ret &= (allocator->alloc_ctx != check_ctx);
  check_ctx = allocator->alloc_ctx; 

  allocator->alloc(allocator, PAGE_SIZE * 12);
  ret &= (allocator->alloc_ctx != check_ctx);
  check_ctx = allocator->alloc_ctx; 

  allocator->destroy(allocator);

  return ret;
}

static 
bool test_isheap_start(alloc_t *allocator)
{
  uintptr_t arena_start = (uintptr_t)allocator->alloc_ctx;
  
  bool ret = allocator->is_alloc_ptr(allocator, arena_start);

  return ret;
}

static 
bool test_isheap_middle(alloc_t *allocator) 
{
  uintptr_t arena_mid = 
    (uintptr_t)allocator + 
    (allocator->alloc_ctx->size / 2);
  
  bool ret = allocator->is_alloc_ptr(allocator, arena_mid);

  return ret;
}

static 
bool test_isheap_end(alloc_t *allocator) 
{
  uintptr_t arena_end = (uintptr_t)allocator->alloc_ctx +
                        allocator->alloc_ctx->size - 1;
  
  bool ret = allocator->is_alloc_ptr(allocator, arena_end);

  return ret;
}


static 
bool test_is_not_heap_high(alloc_t *allocator) 
{
  uintptr_t high_ptr = UINTPTR_MAX; 
  
  bool ret = allocator->is_alloc_ptr(allocator, high_ptr);

  return !ret;
}

static 
bool test_is_not_heap_low(alloc_t *allocator)
{
  uintptr_t low_ptr = 0;
  
  bool ret = allocator->is_alloc_ptr(allocator, low_ptr);

  return !ret;
}

static 
bool test_multi_arena_is_heap(alloc_t *allocator) 
{
  uintptr_t arena1 = (uintptr_t) allocator->alloc(allocator, PAGE_SIZE * 1);
  uintptr_t arena2 = (uintptr_t) allocator->alloc(allocator, PAGE_SIZE * 3);
  uintptr_t arena3 = (uintptr_t) allocator->alloc(allocator, PAGE_SIZE * 7);
  
  bool ret = 1;
  
  ret &= (allocator->is_alloc_ptr(allocator, arena1));
  ret &= (allocator->is_alloc_ptr(allocator, arena2));
  ret &= (allocator->is_alloc_ptr(allocator, arena3));

  return ret;
}

int main(void) 
{
  char test_success = 1;
  
  test_success &=       test_alloc_destroy();
  test_success &=      test_alloc_creation();
  test_success &= test_null_alloc_creation();
  test_success &=          test_allocation();
  test_success &=     test_many_allocators();
  test_success &=        test_alloc_expand();
  
  alloc_t* heap_range = new_allocator(1);

  test_success &=        test_isheap_start(heap_range);
  test_success &=       test_isheap_middle(heap_range);
  test_success &=          test_isheap_end(heap_range);
  test_success &=     test_is_not_heap_low(heap_range);
  test_success &=    test_is_not_heap_high(heap_range);
  test_success &= test_multi_arena_is_heap(heap_range);

  heap_range->destroy(heap_range);

  char* test_msg = (test_success) ? "PASS" : "FAIL";
  printf("[%s] %s \n", test_msg, __FILE__);
}
