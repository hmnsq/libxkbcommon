/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stddef.h>
#অন্তর্ভাৱ <stdbool.h>

#অন্তর্ভাৱ "util-list.h"

শূন্য
list_init(সংরচনা list *list)
{
	list->prev = list;
	list->next = list;
}

শূন্য
list_ঢোকাও(সংরচনা list *list, সংরচনা list *elm)
{
	প্রতিজ্ঞা((list->next != রিক্ত && list->prev != রিক্ত) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	প্রতিজ্ঞা(((elm->next == রিক্ত && elm->prev == রিক্ত) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

শূন্য
list_append(সংরচনা list *list, সংরচনা list *elm)
{
	প্রতিজ্ঞা((list->next != রিক্ত && list->prev != রিক্ত) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	প্রতিজ্ঞা(((elm->next == রিক্ত && elm->prev == রিক্ত) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

শূন্য
list_remove(সংরচনা list *elm)
{
	প্রতিজ্ঞা((elm->next != রিক্ত && elm->prev != রিক্ত) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = রিক্ত;
	elm->prev = রিক্ত;
}

bool
list_empty(ধ্রুৱ সংরচনা list *list)
{
	প্রতিজ্ঞা((list->next != রিক্ত && list->prev != রিক্ত) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ঘূৰাই list->next == list;
}

bool
list_is_last(ধ্রুৱ সংরচনা list *list, ধ্রুৱ সংরচনা list *elm)
{
	ঘূৰাই elm->next == list;
}
