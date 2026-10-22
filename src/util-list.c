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
	list->ايندڙ = list;
}

فراغ
list_وجهو(بنية list *list, بنية list *elm)
{
	أكد((list->ايندڙ != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->ايندڙ == عدم && elm->prev == عدم) || list_خالي_جايون(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->ايندڙ = list->ايندڙ;
	list->ايندڙ = elm;
	elm->ايندڙ->prev = elm;
}

فراغ
list_append(بنية list *list, بنية list *elm)
{
	أكد((list->ايندڙ != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->ايندڙ == عدم && elm->prev == عدم) || list_خالي_جايون(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->ايندڙ = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->ايندڙ = elm;
}

فراغ
list_remove(بنية list *elm)
{
	أكد((elm->ايندڙ != عدم && elm->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->ايندڙ = elm->ايندڙ;
	elm->ايندڙ->prev = elm->prev;
	elm->ايندڙ = عدم;
	elm->prev = عدم;
}

bool
list_خالي_جايون(ثابت بنية list *list)
{
	أكد((list->ايندڙ != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	أرجع list->ايندڙ == list;
}

bool
list_is_آخري(ثابت بنية list *list, ثابت بنية list *elm)
{
	أرجع elm->ايندڙ == list;
}
