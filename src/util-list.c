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
	list->بعدی = list;
}

فراغ
list_درج(بنية list *list, بنية list *elm)
{
	أكد((list->بعدی != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->بعدی == عدم && elm->prev == عدم) || list_خالی(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->بعدی = list->بعدی;
	list->بعدی = elm;
	elm->بعدی->prev = elm;
}

فراغ
list_append(بنية list *list, بنية list *elm)
{
	أكد((list->بعدی != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->بعدی == عدم && elm->prev == عدم) || list_خالی(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->بعدی = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->بعدی = elm;
}

فراغ
list_remove(بنية list *elm)
{
	أكد((elm->بعدی != عدم && elm->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->بعدی = elm->بعدی;
	elm->بعدی->prev = elm->prev;
	elm->بعدی = عدم;
	elm->prev = عدم;
}

bool
list_خالی(ثابت بنية list *list)
{
	أكد((list->بعدی != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	أرجع list->بعدی == list;
}

bool
list_is_آخرین(ثابت بنية list *list, ثابت بنية list *elm)
{
	أرجع elm->بعدی == list;
}
