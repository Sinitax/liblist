#include "list.h"

void
list_init(struct list *list)
{
	LIST_ABORT_ON_ARGS(!list);

	list->head.prev = NULL;
	list->head.next = &list->tail;
	list->tail.prev = &list->head;
	list->tail.next = NULL;
}

void
list_clear(struct list *list)
{
	LIST_ABORT_ON_ARGS(!list);

	while (!list_empty(list))
		list_pop_back(list);
}

void
list_free_items(struct list *list,
	list_link_free_func free_item, size_t offset)
{
	struct list_link *item;

	LIST_ABORT_ON_ARGS(!list || !free_item);

	while (!list_empty(list)) {
		item = list_link_pop(list->head.next);
		free_item(((void *) item) - offset);
	}
}

size_t
list_len(const struct list *list)
{
	const struct list_link *link;
	size_t len;

	LIST_ABORT_ON_ARGS(!list);

	len = 0;
	for (LIST_ITER(list, link))
		len += 1;

	return len;
}

ssize_t
list_index(const struct list *list, const struct list_link *target)
{
	struct list_link *link;
	ssize_t index;

	LIST_ABORT_ON_ARGS(!list || !target);

	index = 0;
	for (LIST_ITER(list, link)) {
		if (link == target)
			return index;
		index++;
	}

	return -1;
}

void
list_insert_sorted(struct list *list, struct list_link *insert, bool reverse,
	bool (*in_order)(struct list_link *a, struct list_link *b))
{
	struct list_link *link;

	LIST_ABORT_ON_ARGS(!list || !insert || !in_order);

	for (LIST_ITER(list, link)) {
		if (in_order(insert, link) == !reverse) {
			list_link_prepend(link, insert);
			return;
		}
	}

	list_push_back(list, insert);
}

void
list_insertion_sort(struct list *list, bool reverse,
	bool (*in_order)(struct list_link *a, struct list_link *b))
{
	struct list_link *link, *cmp, *next;

	LIST_ABORT_ON_ARGS(!list || !in_order);

	link = list->head.next;
	while (LIST_INNER(link)) {
		next = link->next;
		cmp = link->prev;
		while (LIST_INNER(cmp)) {
			if (in_order(cmp, link) == !reverse)
				break;
			cmp = cmp->prev;
		}
		if (cmp != link->prev)
			list_link_append(cmp, list_link_pop(link));
		link = next;
	}
}

struct list_link *
list_at(struct list *list, size_t n)
{
	struct list_link *link;

	LIST_ABORT_ON_ARGS(!list);

	link = list_iter_fwd(list->head.next, n);

	return LIST_INNER(link) ? link : NULL;
}

struct list_link *
list_at_back(struct list *list, size_t n)
{
	struct list_link *link;

	LIST_ABORT_ON_ARGS(!list);

	link = list_iter_bwd(&list->tail, n);

	return LIST_INNER(link) ? link : NULL;
}

struct list_link *
list_pop_front(struct list *list)
{
	LIST_ABORT_ON_ARGS(!list);

	if (list_empty(list)) return NULL;

	return list_link_pop(list->head.next);
}

struct list_link *
list_pop_back(struct list *list)
{
	LIST_ABORT_ON_ARGS(!list);

	if (list_empty(list)) return NULL;

	return list_link_pop(list->tail.prev);
}

struct list_link *
list_link_pop(struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!link);

	if (link->prev)
		link->prev->next = link->next;
	if (link->next)
		link->next->prev = link->prev;
	*link = LIST_LINK_INIT;

	return link;
}

struct list_link *
list_iter_fwd(struct list_link *link, size_t n)
{
	size_t i;

	LIST_ABORT_ON_ARGS(!link);

	for (i = 0; i < n; i++) {
		if (!link) return NULL;
		link = link->next;
	}

	return link;
}

struct list_link *
list_iter_bwd(struct list_link *link, size_t n)
{
	size_t i;

	LIST_ABORT_ON_ARGS(!link);

	for (i = 0; i < n; i++) {
		if (!link) return NULL;
		link = link->prev;
	}

	return link;
}

void
list_link_prepend(struct list_link *cur, struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!cur || !link);

	link->prev = cur->prev;
	link->next = cur;

	if (link->prev)
		link->prev->next = link;
	if (link->next)
		link->next->prev = link;
}

void
list_link_append(struct list_link *cur, struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!cur || !link);

	link->prev = cur;
	link->next = cur->next;

	if (link->prev)
		link->prev->next = link;
	if (link->next)
		link->next->prev = link;
}
