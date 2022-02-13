#pragma once

#include <stdlib.h>

#define LINK_OFFSET(type, member) ((size_t) &((type *)0)->member)
#define LINK_UPCAST(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - LINK_OFFSET(type, member) ); })

#define LINK_EMPTY ((struct link) { 0 })

#define LIST_INNER(list, link) \
	(((link) != &(list)->head) && ((link) != &(list)->tail))

#define LIST_ITER(list, iter) (iter) = (list)->head.next; \
	(iter) != &(list)->tail; (iter) = (iter)->next

typedef void (*link_free_func)(void *p);

struct link {
	struct link *prev;
	struct link *next;
};

struct list {
	struct link head;
	struct link tail;
};

void list_init(struct list *list);
void list_free(struct list *list, void (*free_item)(void *), int offset);

int list_empty(struct list *list);
int list_len(struct list *list);

struct link *list_at(struct list *list, int n);
struct link *list_front(struct list *list);
struct link *list_back(struct list *list);

void list_push_front(struct list *list, struct link *link);
void list_push_back(struct list *list, struct link *link);
struct link *list_pop_front(struct list *list);
struct link *list_pop_back(struct list *list);

struct link *link_iter(struct link *link, int n);
struct link *link_pop(struct link *link);

void link_prepend(struct link *list, struct link *link);
void link_append(struct link *list, struct link *link);

