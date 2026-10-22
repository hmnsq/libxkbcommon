/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "util-list.h"

ശൂന്യം
list_init(ഘടന list *list)
{
	list->prev = list;
	list->next = list;
}

ശൂന്യം
list_തിരുകുക(ഘടന list *list, ഘടന list *elm)
{
	പ്രതിജ്ഞാ((list->next != ശൂന്യ && list->prev != ശൂന്യ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	പ്രതിജ്ഞാ(((elm->next == ശൂന്യ && elm->prev == ശൂന്യ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

ശൂന്യം
list_append(ഘടന list *list, ഘടന list *elm)
{
	പ്രതിജ്ഞാ((list->next != ശൂന്യ && list->prev != ശൂന്യ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	പ്രതിജ്ഞാ(((elm->next == ശൂന്യ && elm->prev == ശൂന്യ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

ശൂന്യം
list_remove(ഘടന list *elm)
{
	പ്രതിജ്ഞാ((elm->next != ശൂന്യ && elm->prev != ശൂന്യ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = ശൂന്യ;
	elm->prev = ശൂന്യ;
}

bool
list_empty(സ്ഥിരം ഘടന list *list)
{
	പ്രതിജ്ഞാ((list->next != ശൂന്യ && list->prev != ശൂന്യ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	തിരികെ list->next == list;
}

bool
list_is_last(സ്ഥിരം ഘടന list *list, സ്ഥിരം ഘടന list *elm)
{
	തിരികെ elm->next == list;
}
