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
	list->اگلا = list;
}

فراغ
list_داخل_کرو(بنية list *list, بنية list *elm)
{
	أكد((list->اگلا != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->اگلا == عدم && elm->prev == عدم) || list_خالی_تھاواں(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->اگلا = list->اگلا;
	list->اگلا = elm;
	elm->اگلا->prev = elm;
}

فراغ
list_append(بنية list *list, بنية list *elm)
{
	أكد((list->اگلا != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->اگلا == عدم && elm->prev == عدم) || list_خالی_تھاواں(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->اگلا = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->اگلا = elm;
}

فراغ
list_remove(بنية list *elm)
{
	أكد((elm->اگلا != عدم && elm->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->اگلا = elm->اگلا;
	elm->اگلا->prev = elm->prev;
	elm->اگلا = عدم;
	elm->prev = عدم;
}

bool
list_خالی_تھاواں(ثابت بنية list *list)
{
	أكد((list->اگلا != عدم && list->prev != عدم) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	أرجع list->اگلا == list;
}

bool
list_is_آخری(ثابت بنية list *list, ثابت بنية list *elm)
{
	أرجع elm->اگلا == list;
}
