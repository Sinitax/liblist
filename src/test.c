#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct arg {
	const char *str;

	struct link link;
};

int
test_sort(struct link *link1, struct link *link2)
{
	struct arg *arg1, *arg2;

	arg1 = LINK_UPCAST(link1, struct arg, link);
	arg2 = LINK_UPCAST(link2, struct arg, link);

	return strcmp(arg1->str, arg2->str);
}

int
main(int argc, const char **argv)
{
	struct link *iter;
	struct list list;
	struct arg *item;
	const char **arg;

	list_init(&list);

	for (arg = argv; *arg; arg++) {
		item = malloc(sizeof(struct arg));
		if (!item) return 0;
		item->str = *arg;
		item->link = LINK_EMPTY;
		list_push_back(&list, &item->link);
	}

	list_sort(&list, test_sort);

	for (LIST_ITER(&list, iter)) {
		item = LINK_UPCAST(iter, struct arg, link);
		printf("%s\n", item->str);
	}
}
