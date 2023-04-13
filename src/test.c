#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct arg {
	const char *str;

	struct list_link link;
};

bool
test_sort(const struct list_link *link1, const struct list_link *link2)
{
	const struct arg *arg1, *arg2;

	arg1 = LIST_UPCAST(link1, struct arg, link);
	arg2 = LIST_UPCAST(link2, struct arg, link);

	return strcmp(arg1->str, arg2->str) <= 0;
}

int
main(int argc, const char **argv)
{
	struct list_link *iter;
	struct list list;
	struct arg *item;
	const char **arg;

	list_init(&list);

	if (argc < 3) {
		fprintf(stderr, "Usage: test REVERSE [STR]..\n");
		return 1;
	}

	for (arg = argv + 2; *arg; arg++) {
		item = malloc(sizeof(struct arg));
		if (!item) return 0;
		item->str = *arg;
		item->link = LIST_LINK_INIT;
		list_insert_back(&list, &item->link);
	}

	list_insertion_sort(&list, atoi(argv[1]), test_sort);

	for (LIST_ITER(&list, iter)) {
		item = LIST_UPCAST(iter, struct arg, link);
		printf("%s\n", item->str);
	}

	list_free_items(&list, free, LIST_OFFSET(struct arg, link));
}
