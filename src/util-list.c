/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stddef.h>
#ضمن <stdbool.h>

#ضمن "util-list.h"

فراغ
list_init(بنية list *list)
{
	list->prev = list;
	list->next = list;
}

فراغ
list_سيسيڤ(بنية list *list, بنية list *elm)
{
	أكد((list->next != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->next == عدم && elm->prev == عدم) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

فراغ
list_append(بنية list *list, بنية list *elm)
{
	أكد((list->next != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->next == عدم && elm->prev == عدم) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

فراغ
list_remove(بنية list *elm)
{
	أكد((elm->next != عدم && elm->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = عدم;
	elm->prev = عدم;
}

bool
list_empty(ثابت بنية list *list)
{
	أكد((list->next != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	أرجع list->next == list;
}

bool
list_is_last(ثابت بنية list *list, ثابت بنية list *elm)
{
	أرجع elm->next == list;
}
