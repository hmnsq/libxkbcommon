/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "util-list.h"

શૂન્ય
list_init(સંરચના list *list)
{
	list->prev = list;
	list->next = list;
}

શૂન્ય
list_ઉમેરો(સંરચના list *list, સંરચના list *elm)
{
	પ્રતિજ્ઞા((list->next != ખાલી && list->prev != ખાલી) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	પ્રતિજ્ઞા(((elm->next == ખાલી && elm->prev == ખાલી) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

શૂન્ય
list_append(સંરચના list *list, સંરચના list *elm)
{
	પ્રતિજ્ઞા((list->next != ખાલી && list->prev != ખાલી) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	પ્રતિજ્ઞા(((elm->next == ખાલી && elm->prev == ખાલી) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

શૂન્ય
list_remove(સંરચના list *elm)
{
	પ્રતિજ્ઞા((elm->next != ખાલી && elm->prev != ખાલી) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = ખાલી;
	elm->prev = ખાલી;
}

bool
list_empty(અચલ સંરચના list *list)
{
	પ્રતિજ્ઞા((list->next != ખાલી && list->prev != ખાલી) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	પાછા list->next == list;
}

bool
list_is_last(અચલ સંરચના list *list, અચલ સંરચના list *elm)
{
	પાછા elm->next == list;
}
