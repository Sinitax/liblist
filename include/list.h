#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include <stddef.h>

#define LIST_LINK_INIT ((struct list_link) { 0 })

#define LIST_OFFSET(type, member) ((size_t) &((type *)0)->member)
#define LIST_UPCAST(ptr, type, member) \
	((type *)((char *)ptr - LIST_OFFSET(type, member)))

#define LIST_INNER(link) ((link) != NULL && \
	(link)->prev != NULL && (link)->next != NULL)

#define LIST_ITER(list, iter) (iter) = (list)->head.next; \
	(iter) != &(list)->tail; (iter) = (iter)->next
#define LIST_ITER_BWD(list, iter) (iter) = (list)->tail.prev; \
	(iter) != &(list)->head; (iter) = (iter)->prev

#ifdef LIBLIST_ASSERT_ARGS
#define LIST_ABORT_ON_ARGS(cond) do { if (cond) abort(); } while (0)
#else
#define LIST_ABORT_ON_ARGS(cond)
#endif

typedef void (*list_link_free_func)(void *p);

struct list_link {
	struct list_link *prev;
	struct list_link *next;
};

struct list {
	struct list_link head;
	struct list_link tail;
};

void list_init(struct list *list);
void list_clear(struct list *list);
void list_free_items(struct list *list,
	list_link_free_func free_item, size_t offset);

static inline struct list_link *list_front(const struct list *list);
static inline struct list_link *list_back(const struct list *list);

static inline bool list_empty(const struct list *list);
size_t list_len(const struct list *list);
ssize_t list_index(const struct list *list, const struct list_link *link);

struct list_link *list_at(struct list *list, size_t n);
struct list_link *list_at_back(struct list *list, size_t n);

void list_insert_sorted(struct list *list, struct list_link *link, bool reverse,
	bool (*in_order)(const struct list_link *a, const struct list_link *b));
void list_insertion_sort(struct list *list, bool reverse,
	bool (*in_order)(const struct list_link *a, const struct list_link *b));
static inline void list_insert_front(struct list *list, struct list_link *link);
static inline void list_insert_back(struct list *list, struct list_link *link);

struct list_link *list_pop_front(struct list *list);
struct list_link *list_pop_back(struct list *list);

static inline bool list_link_inuse(const struct list_link *link);
struct list_link *list_link_pop(struct list_link *link);

struct list_link *list_iter_fwd(struct list_link *link, size_t n);
struct list_link *list_iter_bwd(struct list_link *link, size_t n);

static inline void list_link_prepend(struct list_link *a, struct list_link *b);
static inline void list_link_append(struct list_link *a, struct list_link *b);

static inline void list_link_attach_next(struct list_link *a, struct list_link *b);
static inline void list_link_attach_prev(struct list_link *a, struct list_link *b);

static inline void list_link_detach_next(struct list_link *link);
static inline void list_link_detach_prev(struct list_link *link);

static inline struct list_link *
list_front(const struct list *list)
{
	struct list_link *link;

	LIST_ABORT_ON_ARGS(!list);

	link = list->head.next;

	return link != &list->tail ? link : NULL;
}

static inline struct list_link *
list_back(const struct list *list)
{
	struct list_link *link;

	LIST_ABORT_ON_ARGS(!list);

	link = list->tail.prev;

	return link != &list->head ? link : NULL;
}

static inline bool
list_empty(const struct list *list)
{
	LIST_ABORT_ON_ARGS(!list);

	return list->head.next == &list->tail;
}

static inline bool
list_link_inuse(const struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!link);

	return link->prev && link->next;
}

static inline void
list_insert_front(struct list *list, struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!list);

	list_link_append(&list->head, link);
}

static inline void
list_insert_back(struct list *list, struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!list);

	list_link_prepend(&list->tail, link);
}

/* attach b before a: b -> a */
static inline void
list_link_prepend(struct list_link *a, struct list_link *b)
{
	list_link_attach_prev(a, b);
}

/* attach b after a: a -> b */
static inline void
list_link_append(struct list_link *a, struct list_link *b)
{
	list_link_attach_next(a, b);
}

/* attach b before a: b -> a */
static inline void
list_link_attach_prev(struct list_link *a, struct list_link *b)
{
	LIST_ABORT_ON_ARGS(!a || !b);

	b->prev = a->prev;
	b->next = a;

	if (b->prev) b->prev->next = b;
	a->prev = b;
}

/* attach b after a: a -> b */
static inline void
list_link_attach_next(struct list_link *a, struct list_link *b)
{
	LIST_ABORT_ON_ARGS(!a || !b);

	b->next = a->next;
	b->prev = a;

	if (b->next) b->next->prev = b;
	a->next = b;
}

static inline void
list_link_detach_next(struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!link);

	if (link->next) link->next->prev = NULL;
	link->next = NULL;
}

static inline void
list_link_detach_prev(struct list_link *link)
{
	LIST_ABORT_ON_ARGS(!link);

	if (link->prev) link->prev->next = NULL;
	link->prev = NULL;
}
