#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stdio.h>

struct list_node {
  size_t value;
  struct list_node *next;
};

struct linked_list {
  struct list_node *head;
};

void insert_at_head(struct linked_list *list, size_t value);
void insert_at_tail(struct linked_list *list, size_t value);
size_t remove_from_head(struct linked_list *list);
size_t remove_from_tail(struct linked_list *list);

void dump_list(FILE *fp, struct linked_list list);

void free_list(struct linked_list list);

#endif // LINKED_LIST_H_
