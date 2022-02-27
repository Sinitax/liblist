#include "list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ASSERT(x) assert((x), __FILE__, __LINE__, #x)

static void assert(bool cond, const char *file,
	size_t line, const char *condstr);

static struct link *link_iter_fwd(struct link *link, size_t n);
static struct link *link_iter_bwd(struct link *link, size_t n);

void
assert(bool cond, const char *file, size_t line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "CLIST: Assertion failed at %s:%li (%s)\n",
		file, line, condstr);

	exit(1);
}

struct link *
link_iter_fwd(struct link *link, size_t n)
{
	size_t i;

	ASSERT(link != NULL);

	for (i = 0; i < n; i++) {
		if (!link) return NULL;
		link = link->next;
	}

	return link;
}

struct link *
link_iter_bwd(struct link *link, size_t n)
{
	size_t i;

	ASSERT(link != NULL);

	for (i = 0; i < n; i++) {
		if (!link) return NULL;
		link = link->prev;
	}

	return link;
}


void
list_init(struct list *list)
{
	ASSERT(list != NULL);

	list->head.prev = NULL;
	list->head.next = &list->tail;
	list->tail.prev = &list->head;
	list->tail.next = NULL;
}

void
list_free(struct list *list, void (*free_item)(void *), int offset)
{
	struct link *item;

	ASSERT(list != NULL && free_item != NULL);

	while (!list_empty(list)) {
		item = link_pop(list->head.next);
		free_item(((void *) item) - offset);
	}
}

void
list_clear(struct list *list)
{
	ASSERT(list != NULL);

	while (!list_empty(list))
		list_pop_back(list);
}

bool
list_empty(struct list *list)
{
	ASSERT(list != NULL);

	return list->head.next == &list->tail;
}

size_t
list_len(struct list *list)
{
	struct link *link;
	size_t len;

	ASSERT(list != NULL);

	len = 0;
	for (LIST_ITER(list, link))
		len += 1;

	return len;
}

void
list_insert_sorted(struct list *list, struct link *insert,
	int (*compare)(struct link *a, struct link *b))
{
	struct link *link;

	ASSERT(list != NULL && link != NULL && compare != NULL);

	for (LIST_ITER(list, link)) {
		if (compare(insert, link) > 0) {
			link_append(link, insert);
			break;
		}
	}
}

void
list_sort(struct list *list, int (*compare)(struct link *a, struct link *b))
{
	struct link *link, *cmp, *next;

	ASSERT(list != NULL && compare != NULL);

	/* Simple Insertion Sort */
	/* cmp(a,b) -> (a-b) */

	link = list->head.next;
	while (LIST_INNER(link)) {
		next = link->next;
		cmp = link->prev;
		while (LIST_INNER(cmp)) {
			if (compare(cmp, link) < 0) {
				link_append(cmp, link_pop(link));
				break;
			}
			cmp = cmp->prev;
		}
		link = next;
	}
}

int
list_index(struct list *list, struct link *target)
{
	struct link *link;
	int index;

	index = 0;
	for (LIST_ITER(list, link)) {
		if (link == target)
			return index;
		index++;
	}

	return -1;
}

struct link *
list_at(struct list *list, int n)
{
	struct link * link;

	ASSERT(list != NULL);

	if (n >= 0)
		link = link_iter_fwd(list->head.next, n);
	else
		link = link_iter_bwd(&list->tail, -n);

	return LIST_INNER(link) ? link : NULL;
}

struct link *
list_front(struct list *list)
{
	return list_at(list, 0);
}

struct link *
list_back(struct list *list)
{
	return list_at(list, -1);
}

void
list_push_front(struct list *list, struct link *link)
{
	link_append(&list->head, link);
}

void
list_push_back(struct list *list, struct link *link)
{
	link_prepend(&list->tail, link);
}

struct link *
list_pop_front(struct list *list)
{
	ASSERT(list != NULL);

	if (list_empty(list))
		return NULL;

	return link_pop(list->head.next);
}

struct link *
list_pop_back(struct list *list)
{
	ASSERT(list != NULL);

	if (list_empty(list))
		return NULL;

	return link_pop(list->tail.prev);
}

void
link_prepend(struct link *cur, struct link *link)
{
	ASSERT(cur != NULL && link != NULL);

	link->prev = cur->prev;
	link->next = cur;

	if (link->prev)
		link->prev->next = link;
	if (link->next)
		link->next->prev = link;
}

void
link_append(struct link *cur, struct link *link)
{
	ASSERT(cur != NULL && link != NULL);

	link->prev = cur;
	link->next = cur->next;

	if (link->prev)
		link->prev->next = link;
	if (link->next)
		link->next->prev = link;
}

bool
link_inuse(struct link *link)
{
	ASSERT(link != NULL);

	return link->prev != NULL && link->next != NULL;
}

struct link *
link_iter(struct link *link, int n)
{
	if (n >= 0)
		return link_iter_fwd(link, n);
	else
		return link_iter_bwd(link, -n);
}

struct link *
link_pop(struct link *link)
{
	ASSERT(link != NULL);

	if (link->prev)
		link->prev->next = link->next;
	if (link->next)
		link->next->prev = link->prev;
	*link = LINK_EMPTY;

	return link;
}
