/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "util-list.h"

ਖਾਲੀ
list_init(ਬਣਤਰ list *list)
{
	list->prev = list;
	list->ਅਗਲਾ = list;
}

ਖਾਲੀ
list_ਪਾਓ(ਬਣਤਰ list *list, ਬਣਤਰ list *elm)
{
	ਪ੍ਰਤਿਜ੍ਞਾ((list->ਅਗਲਾ != ਨਿਰਾ && list->prev != ਨਿਰਾ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ਪ੍ਰਤਿਜ੍ਞਾ(((elm->ਅਗਲਾ == ਨਿਰਾ && elm->prev == ਨਿਰਾ) || list_ਖਾਲੀ_ਥਾਵਾਂ(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->ਅਗਲਾ = list->ਅਗਲਾ;
	list->ਅਗਲਾ = elm;
	elm->ਅਗਲਾ->prev = elm;
}

ਖਾਲੀ
list_append(ਬਣਤਰ list *list, ਬਣਤਰ list *elm)
{
	ਪ੍ਰਤਿਜ੍ਞਾ((list->ਅਗਲਾ != ਨਿਰਾ && list->prev != ਨਿਰਾ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ਪ੍ਰਤਿਜ੍ਞਾ(((elm->ਅਗਲਾ == ਨਿਰਾ && elm->prev == ਨਿਰਾ) || list_ਖਾਲੀ_ਥਾਵਾਂ(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->ਅਗਲਾ = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->ਅਗਲਾ = elm;
}

ਖਾਲੀ
list_remove(ਬਣਤਰ list *elm)
{
	ਪ੍ਰਤਿਜ੍ਞਾ((elm->ਅਗਲਾ != ਨਿਰਾ && elm->prev != ਨਿਰਾ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->ਅਗਲਾ = elm->ਅਗਲਾ;
	elm->ਅਗਲਾ->prev = elm->prev;
	elm->ਅਗਲਾ = ਨਿਰਾ;
	elm->prev = ਨਿਰਾ;
}

bool
list_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ list *list)
{
	ਪ੍ਰਤਿਜ੍ਞਾ((list->ਅਗਲਾ != ਨਿਰਾ && list->prev != ਨਿਰਾ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ਵਾਪਸ list->ਅਗਲਾ == list;
}

bool
list_is_ਆਖਰੀ(ਅਚਲ ਬਣਤਰ list *list, ਅਚਲ ਬਣਤਰ list *elm)
{
	ਵਾਪਸ elm->ਅਗਲਾ == list;
}
