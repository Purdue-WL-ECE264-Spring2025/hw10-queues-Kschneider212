#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (!node) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) {
        return;
    }
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) {
        return;
    }
    if (!list->head) {
        list->head = node;
        return;
    }
    struct list_node *cur = list->head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
}

size_t remove_from_head(struct linked_list *list) {
    if (!list->head) {
        return 0;
    }
    struct list_node *temp = list->head;
    size_t value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (!list->head) {
        return 0;
    }
    struct list_node *cur = list->head;
    struct list_node *prev = NULL;
    while (cur->next) {
        prev = cur;
        cur = cur->next;
    }
    size_t value = cur->value;
    if (prev) {
        prev->next = NULL;
    } else {
        list->head = NULL;
    }
    free(cur);
    return value;
}
void free_list(struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur) {
        struct list_node *temp = cur;
        cur = cur->next;
        free(temp);
    }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
