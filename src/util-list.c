/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stddef.h>
#شامل <stdbool.h>

#شامل "util-list.h"

تهی
list_init(ساختار list *list)
{
	list->prev = list;
	list->next = list;
}

تهی
list_درج(ساختار list *list, ساختار list *elm)
{
	أكد((list->next != پوچ && list->prev != پوچ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->next == پوچ && elm->prev == پوچ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

تهی
list_append(ساختار list *list, ساختار list *elm)
{
	أكد((list->next != پوچ && list->prev != پوچ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->next == پوچ && elm->prev == پوچ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

تهی
list_remove(ساختار list *elm)
{
	أكد((elm->next != پوچ && elm->prev != پوچ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = پوچ;
	elm->prev = پوچ;
}

bool
list_empty(ثابت ساختار list *list)
{
	أكد((list->next != پوچ && list->prev != پوچ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	بازگشت list->next == list;
}

bool
list_is_last(ثابت ساختار list *list, ثابت ساختار list *elm)
{
	بازگشت elm->next == list;
}
