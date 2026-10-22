/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "util-list.h"

শূন্য
list_init(কাঠামো list *list)
{
	list->পূর্ব = list;
	list->পরবর্তী = list;
}

শূন্য
list_ঢোকাও(কাঠামো list *list, কাঠামো list *elm)
{
	প্রতিজ্ঞা((list->পরবর্তী != ফাঁকা && list->পূর্ব != ফাঁকা) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	প্রতিজ্ঞা(((elm->পরবর্তী == ফাঁকা && elm->পূর্ব == ফাঁকা) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->পূর্ব = list;
	elm->পরবর্তী = list->পরবর্তী;
	list->পরবর্তী = elm;
	elm->পরবর্তী->পূর্ব = elm;
}

শূন্য
list_append(কাঠামো list *list, কাঠামো list *elm)
{
	প্রতিজ্ঞা((list->পরবর্তী != ফাঁকা && list->পূর্ব != ফাঁকা) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	প্রতিজ্ঞা(((elm->পরবর্তী == ফাঁকা && elm->পূর্ব == ফাঁকা) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->পরবর্তী = list;
	elm->পূর্ব = list->পূর্ব;
	list->পূর্ব = elm;
	elm->পূর্ব->পরবর্তী = elm;
}

শূন্য
list_remove(কাঠামো list *elm)
{
	প্রতিজ্ঞা((elm->পরবর্তী != ফাঁকা && elm->পূর্ব != ফাঁকা) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->পূর্ব->পরবর্তী = elm->পরবর্তী;
	elm->পরবর্তী->পূর্ব = elm->পূর্ব;
	elm->পরবর্তী = ফাঁকা;
	elm->পূর্ব = ফাঁকা;
}

bool
list_empty(ধ্রুবক কাঠামো list *list)
{
	প্রতিজ্ঞা((list->পরবর্তী != ফাঁকা && list->পূর্ব != ফাঁকা) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ফেরত list->পরবর্তী == list;
}

bool
list_is_last(ধ্রুবক কাঠামো list *list, ধ্রুবক কাঠামো list *elm)
{
	ফেরত elm->পরবর্তী == list;
}
