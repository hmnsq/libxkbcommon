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
list_init(ರಚನೆ list *list)
{
	list->prev = list;
	list->ಮುಂದಿನ = list;
}

ಶೂನ್ಯ
list_ಸೇರಿಸು(ರಚನೆ list *list, ರಚನೆ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((list->ಮುಂದಿನ != ಖಾಲಿ && list->prev != ಖಾಲಿ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ಪ್ರತಿಜ್ಞಾ(((elm->ಮುಂದಿನ == ಖಾಲಿ && elm->prev == ಖಾಲಿ) || list_ಖಾಲಿಗಳು(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->ಮುಂದಿನ = list->ಮುಂದಿನ;
	list->ಮುಂದಿನ = elm;
	elm->ಮುಂದಿನ->prev = elm;
}

ಶೂನ್ಯ
list_append(ರಚನೆ list *list, ರಚನೆ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((list->ಮುಂದಿನ != ಖಾಲಿ && list->prev != ಖಾಲಿ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ಪ್ರತಿಜ್ಞಾ(((elm->ಮುಂದಿನ == ಖಾಲಿ && elm->prev == ಖಾಲಿ) || list_ಖಾಲಿಗಳು(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->ಮುಂದಿನ = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->ಮುಂದಿನ = elm;
}

ಶೂನ್ಯ
list_remove(ರಚನೆ list *elm)
{
	ಪ್ರತಿಜ್ಞಾ((elm->ಮುಂದಿನ != ಖಾಲಿ && elm->prev != ಖಾಲಿ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->ಮುಂದಿನ = elm->ಮುಂದಿನ;
	elm->ಮುಂದಿನ->prev = elm->prev;
	elm->ಮುಂದಿನ = ಖಾಲಿ;
	elm->prev = ಖಾಲಿ;
}

bool
list_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ list *list)
{
	ಪ್ರತಿಜ್ಞಾ((list->ಮುಂದಿನ != ಖಾಲಿ && list->prev != ಖಾಲಿ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ಹಿಂತಿರುಗಿ list->ಮುಂದಿನ == list;
}

bool
list_is_ಕೊನೆಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ list *list, ಸ್ಥಿರಾಂಕ ರಚನೆ list *elm)
{
	ಹಿಂತಿರುಗಿ elm->ಮುಂದಿನ == list;
}
