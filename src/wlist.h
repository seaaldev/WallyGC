#ifndef WLIST_H
#define WLIST_H

#include <stdbool.h>


/* ------------------------ PUBLIC INTERACE ------------------------ */

struct list_head {
  struct list_head* next;
  struct list_head* prev;
};

// Initialize a struct list_head to point to itself
#define LIST_HEAD_INIT(name) { &(name), &(name) }

// Definition of a &struct list_head with initialization values 
#define LIST_HEAD(name) \
  struct list_head name = LIST_HEAD_INIT

// Insert a new entry after the specified head
static inline
void list_add(struct list_head* new, struct list_head* head);

// Initialize & insert a new entry before the specified head
static inline
void list_add_tail(struct list_head* new, struct list_head* head);

// Delete entry from list
static inline
void list_del(struct list_head* entry);

// Move an existing entry after the specified head
static inline
void list_move(struct list_head* entry, struct list_head* head);

// Move an existing entry to before the specified head
static inline 
void list_move_tail(struct list_head* entry, struct list_head* head);


/* 
 * ------------------------------------------------------------------ 
 * INTERNAL ROUTINES BELOW:
 * ------------------------------------------------------------------ 
 */


#define POISON 0xDEADDEADDEADDEAD

static inline
bool wgc_list_link_valid(struct list_head* new,
                         struct list_head* next,
                         struct list_head* prev)
{
  return (next->prev == next && prev->next == prev && new != next && new !=prev);
}

static inline
void wgc_list_link(struct list_head* new,
                   struct list_head* next,
                   struct list_head* prev)
{
  if (!wgc_list_link_valid(new, next, prev))
    return;
  

  next->prev       = new;
  new->next        = next;
  prev->next       = new;
  new->prev        = prev;
}

static inline
bool wgc_list_del_entry_valid(struct list_head* entry)
{
  struct list_head* next = entry->next;
  struct list_head* prev = entry->prev;

  return (prev->next == entry && next->prev == entry);
}

static inline
void wgc_list_del_link(struct list_head* next, struct list_head* prev)
{
  next->prev = prev;
  prev->next = next;
}

static inline
void wgc_list_del_entry(struct list_head* entry) 
{
  if (!wgc_list_del_entry_valid(entry))
    return;

  wgc_list_del_link(entry->next, entry->prev);
  entry->next = POISON;
  entry->prev = POISON;
}

static inline
void wgc_list_move(struct list_head* entry,
                   struct list_head* next,
                   struct list_head* prev)
{
  wgc_list_del_link(entry->next, entry->prev);
  wgc_list_link(entry, next, prev); 
}

/* 
 * ------------------------------------------------------------------ 
 * PUBLIC ROUTINES BELOW:
 * ------------------------------------------------------------------ 
 */

static inline
void list_add(struct list_head* new,
              struct list_head* head)
{
  wgc_list_link(new, head->next, head);
}

static inline
void list_add_tail(struct list_head* new,
                   struct list_head* head)
{
  wgc_list_link(new, head, head->prev);
}

static inline
void list_del(struct list_head* entry) 
{
  wgc_list_del_entry
}

static inline
void list_move(struct list_head* entry,
               struct list_head* head)
{
  wgc_list_move(entry, head->next, head);
}

static inline 
void list_move_tail(struct list_head* entry,
                    struct list_head* head) 
{
  wgc_list_move(entry, head, head->prev);
}

#endif // ! WLIST_H
