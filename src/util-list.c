/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "util-list.h"

வெறுமை
list_init(கட்டமைப்பு list *list)
{
	list->prev = list;
	list->next = list;
}

வெறுமை
list_செருகு(கட்டமைப்பு list *list, கட்டமைப்பு list *elm)
{
	ப்ரதிஜ்ஞா((list->next != வெற்று && list->prev != வெற்று) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ப்ரதிஜ்ஞா(((elm->next == வெற்று && elm->prev == வெற்று) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

வெறுமை
list_append(கட்டமைப்பு list *list, கட்டமைப்பு list *elm)
{
	ப்ரதிஜ்ஞா((list->next != வெற்று && list->prev != வெற்று) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ப்ரதிஜ்ஞா(((elm->next == வெற்று && elm->prev == வெற்று) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

வெறுமை
list_remove(கட்டமைப்பு list *elm)
{
	ப்ரதிஜ்ஞா((elm->next != வெற்று && elm->prev != வெற்று) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = வெற்று;
	elm->prev = வெற்று;
}

bool
list_empty(மாறிலி கட்டமைப்பு list *list)
{
	ப்ரதிஜ்ஞா((list->next != வெற்று && list->prev != வெற்று) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	திரும்பு list->next == list;
}

bool
list_is_last(மாறிலி கட்டமைப்பு list *list, மாறிலி கட்டமைப்பு list *elm)
{
	திரும்பு elm->next == list;
}
