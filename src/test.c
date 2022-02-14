#include "list.h"

#include <stdlib.h>
#include <stdio.h>

struct arg {
	const char *str;

	struct link link;
};

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

	for (LIST_ITER(&list, iter)) {
		item = LINK_UPCAST(iter, struct arg, link);
		printf("%s\n", item->str);
	}
}
