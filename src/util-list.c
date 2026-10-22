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
	list->بل = list;
}

فراغ
list_ورننباسه(بنية list *list, بنية list *elm)
{
	أكد((list->بل != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->بل == عدم && elm->prev == عدم) || list_تشه(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->بل = list->بل;
	list->بل = elm;
	elm->بل->prev = elm;
}

فراغ
list_append(بنية list *list, بنية list *elm)
{
	أكد((list->بل != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->بل == عدم && elm->prev == عدم) || list_تشه(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->بل = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->بل = elm;
}

فراغ
list_remove(بنية list *elm)
{
	أكد((elm->بل != عدم && elm->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->بل = elm->بل;
	elm->بل->prev = elm->prev;
	elm->بل = عدم;
	elm->prev = عدم;
}

bool
list_تشه(ثابت بنية list *list)
{
	أكد((list->بل != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	أرجع list->بل == list;
}

bool
list_is_وروستی(ثابت بنية list *list, ثابت بنية list *elm)
{
	أرجع elm->بل == list;
}
