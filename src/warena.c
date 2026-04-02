#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "wordsize.h"


/* Compiletime Constasts */

#define INITIAL_HDR_COUNT 1024

#define INITIAL_CORE (WORDSIZE * INITIAL_HDR_COUNT) 

/* ##### Internal Interface ##### */

typedef struct _wgc_arena_t arena;

static arena _wgc_mstate; 

typedef struct _wgc_hdr_t hdr;


/* Internal Routine Declartions */

static hdr* _wgc_heap_ini();

static void _wgc_malloc_ini();


/* ############################### */


struct _wgc_hdr_t {

  size_t  heap_index;
  size_t     memsize;
  size_t   magic_num;

  struct {
    bool   is_marked;
    bool    is_mmapd;
    bool is_prv_used;
  };
};


#define MORECORE(a) (sbrk(a)) 

struct _wgc_arena_t {
  
  size_t   core_size;
  size_t  core_alloc;
  size_t   page_size;
  size_t  page_alloc;

  bin*       fastbin;

  size_t   hdr_count;
  hdr**         heap;
};

static hdr* _wgc_heap_ini() {

  /* Initialize the initial hdr pointers     */
  /* on the processes attached memory (sbrk) */
  hdr* heap_start = MORECORE(INITIAL_CORE_SIZE);
  
  hdr** hdr_ref = (hdr**) heap_start
  for (int i = 0; i < INITIAL_HDR_COUNT; i++) {
    hdr_ref[i] = 0;
  }
  
  // TODO: Create the shared mmap page

  return hdr_ref;
}

static void _wgc_malloc_ini() {

  _wgc_mstate.core_size = INITIAL_CORE_SIZE;
  _wgc_mstate.core_alloc = 0;

  _wgc_mstate.page_size = INITIAL_MMAP_SIZE;
  _wgc_mstate.page_alloc = 0;

  _wgc_mstate.fastbin = 0; 

  _wgc_mstate.hdr_count = 0;
  _wgc_mstate.heap = _wgc_heap_ini();

}


