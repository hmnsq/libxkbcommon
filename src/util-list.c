/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "util-list.h"

ଶୂନ୍ୟ
list_init(ସଂରଚନା list *list)
{
	list->prev = list;
	list->next = list;
}

ଶୂନ୍ୟ
list_ଭର୍ତ୍ତି(ସଂରଚନା list *list, ସଂରଚନା list *elm)
{
	ପ୍ରତିଜ୍ଞା((list->next != ଖାଲି && list->prev != ଖାଲି) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ପ୍ରତିଜ୍ଞା(((elm->next == ଖାଲି && elm->prev == ଖାଲି) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

ଶୂନ୍ୟ
list_append(ସଂରଚନା list *list, ସଂରଚନା list *elm)
{
	ପ୍ରତିଜ୍ଞା((list->next != ଖାଲି && list->prev != ଖାଲି) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ପ୍ରତିଜ୍ଞା(((elm->next == ଖାଲି && elm->prev == ଖାଲି) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

ଶୂନ୍ୟ
list_remove(ସଂରଚନା list *elm)
{
	ପ୍ରତିଜ୍ଞା((elm->next != ଖାଲି && elm->prev != ଖାଲି) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = ଖାଲି;
	elm->prev = ଖାଲି;
}

bool
list_empty(ଅଚଳ ସଂରଚନା list *list)
{
	ପ୍ରତିଜ୍ଞା((list->next != ଖାଲି && list->prev != ଖାଲି) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	ଫେରନ୍ତୁ list->next == list;
}

bool
list_is_last(ଅଚଳ ସଂରଚନା list *list, ଅଚଳ ସଂରଚନା list *elm)
{
	ଫେରନ୍ତୁ elm->next == list;
}
