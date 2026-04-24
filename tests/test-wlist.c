#include "stdio.h"
#include "stdbool.h"

#include "./../src/wlist.h"


bool test_list_init(void)
{
  LIST_HEAD(list);
  
  struct list_head *next = list.next;
  struct list_head *prev = list.prev;

  return (next == &list && prev == &list);
}

bool test_list_add(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add(&second, &first);
  list_add(&third, &second);
  
  bool ret = 1;

  ret &= (first.next == &second &&
    first.prev == &third);

  ret &= (second.next == &third &&
    second.prev == &first);

   ret &= (third.next == &first &&
    third.prev == &second);

  return ret;
}

bool test_list_add_tail(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add_tail(&second, &first);
  list_add_tail(&third, &second);

  bool ret = 1;

  ret &= (first.next == &third &&
    first.prev == &second);

  ret &= (second.next == &first &&
    second.prev == &third);

  ret &= (third.next == &second &&
    third.prev == &first);

  return ret;
}

bool test_list_del(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add(&second, &first);
  list_add(&third, &second);

  list_del(&second);
  
  bool ret = 1;

  ret &= (first.next  == &third);
  ret &= (first.prev  == &third);
  ret &= (third.next  == &first);
  ret &= (third.prev  == &first);
  ret &= (second.next == (struct list_head *)POISON);
  ret &= (second.prev == (struct list_head *)POISON);

  return ret;
}

bool test_list_del_head(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add(&second, &first);
  list_add(&third, &second);

  list_del(&first);
  
  bool ret = 1;

  ret &= (second.next == &third);
  ret &= (second.prev == &third);
  ret &= (third.next  == &second);
  ret &= (third.prev  == &second);
  ret &= (first.next  == (struct list_head *)POISON);
  ret &= (first.prev  == (struct list_head *)POISON);

  return ret;
}

bool test_list_del_tail(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add(&second, &first);
  list_add(&third, &second);

  list_del(&third);
  
  bool ret = 1;

  ret &= (second.next == &first);
  ret &= (second.prev == &first);
  ret &= (first.next  == &second);
  ret &= (first.prev  == &second);
  ret &= (third.next  == (struct list_head *)POISON);
  ret &= (third.prev  == (struct list_head *)POISON);

  return ret;
}

bool test_list_move(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add(&second, &first);
  list_add(&third, &second);

  list_move(&first, &third);
  list_move(&second, &third);
  
  bool ret = 1;

  ret &= (first.next == &third &&
    first.prev == &second);

  ret &= (second.next == &first &&
    second.prev == &third);

  ret &= (third.next == &second &&
    third.prev == &first);

  return ret;
}

bool test_list_move_tail(void)
{
  LIST_HEAD(first);
  LIST_HEAD(second);
  LIST_HEAD(third);

  list_add_tail(&second, &first);
  list_add_tail(&third, &second);
  
  list_move_tail(&second, &third);
  list_move_tail(&first, &second);
  
  bool ret = 1;

  ret &= (first.next == &second &&
    first.prev == &third);

  ret &= (second.next == &third &&
    second.prev == &first);

  ret &= (third.next == &first &&
    third.prev == &second);

  return ret;
}


int main(void)
{
  bool test_success = 1;
  
  test_success &= test_list_init();
  test_success &= test_list_add();
  test_success &= test_list_add_tail();
  test_success &= test_list_del();
  test_success &= test_list_del_head();
  test_success &= test_list_del_tail();
  test_success &= test_list_move();
  test_success &= test_list_move_tail();
  
  char* test_msg = (test_success) ? "PASS" : "FAIL";
  printf("[%s] %s \n", test_msg, __FILE__);

  return 0;
}
