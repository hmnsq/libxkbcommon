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

خالی
list_init(ساخت list *list)
{
	list->پچھلا = list;
	list->اگلا = list;
}

خالی
list_داخل_کرو(ساخت list *list, ساخت list *elm)
{
	أكد((list->اگلا != تہی && list->پچھلا != تہی) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->اگلا == تہی && elm->پچھلا == تہی) || list_خالی_خانے(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->پچھلا = list;
	elm->اگلا = list->اگلا;
	list->اگلا = elm;
	elm->اگلا->پچھلا = elm;
}

خالی
list_append(ساخت list *list, ساخت list *elm)
{
	أكد((list->اگلا != تہی && list->پچھلا != تہی) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	أكد(((elm->اگلا == تہی && elm->پچھلا == تہی) || list_خالی_خانے(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->اگلا = list;
	elm->پچھلا = list->پچھلا;
	list->پچھلا = elm;
	elm->پچھلا->اگلا = elm;
}

خالی
list_remove(ساخت list *elm)
{
	أكد((elm->اگلا != تہی && elm->پچھلا != تہی) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->پچھلا->اگلا = elm->اگلا;
	elm->اگلا->پچھلا = elm->پچھلا;
	elm->اگلا = تہی;
	elm->پچھلا = تہی;
}

bool
list_خالی_خانے(ثابت ساخت list *list)
{
	أكد((list->اگلا != تہی && list->پچھلا != تہی) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	واپس list->اگلا == list;
}

bool
list_is_آخری(ثابت ساخت list *list, ثابت ساخت list *elm)
{
	واپس elm->اگلا == list;
}
