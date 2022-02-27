LIBLIST_1.0 {
	global:
		list_init;
		list_free;

		list_clear;

		list_empty;
		list_len;

		list_insert_sorted;
		list_sort;

		list_index;

		list_at;
		list_front;
		list_back;

		list_push_front;
		list_push_back;
		list_pop_front;
		list_pop_back;

		link_inuse;

		link_iter;
		link_pop;
		
		link_prepend;
		link_append;
	local: *;
};
