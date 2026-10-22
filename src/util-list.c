/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "util-list.h"

ಶೂನ್ಯ
list_init(ಸಂರಚನಾ list *list)
{
	list->prev = list;
	list->next = list;
}

ಶೂನ್ಯ
list_ಸೇರಿಸು(ಸಂರಚನಾ list *list, ಸಂರಚನಾ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((list->next != ರಿಕ್ತ && list->prev != ರಿಕ್ತ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ಪ್ರತಿಜ್ಞಾ(((elm->next == ರಿಕ್ತ && elm->prev == ರಿಕ್ತ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

ಶೂನ್ಯ
list_append(ಸಂರಚನಾ list *list, ಸಂರಚನಾ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((list->next != ರಿಕ್ತ && list->prev != ರಿಕ್ತ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ಪ್ರತಿಜ್ಞಾ(((elm->next == ರಿಕ್ತ && elm->prev == ರಿಕ್ತ) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

ಶೂನ್ಯ
list_remove(ಸಂರಚನಾ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((elm->next != ರಿಕ್ತ && elm->prev != ರಿಕ್ತ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = ರಿಕ್ತ;
	elm->prev = ರಿಕ್ತ;
}

bool
list_empty(ಧ್ರುವ ಸಂರಚನಾ list *list)
{
	ಪ್ರತಿಜ್ಞಾ((list->next != ರಿಕ್ತ && list->prev != ರಿಕ್ತ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ಪ್ರತ್ಯಾಗಮ list->next == list;
}

bool
list_is_last(ಧ್ರುವ ಸಂರಚನಾ list *list, ಧ್ರುವ ಸಂರಚನಾ list *elm)
{
	ಪ್ರತ್ಯಾಗಮ elm->next == list;
}
