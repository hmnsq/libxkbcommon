/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "util-list.h"

शून्य
list_init(संरचना list *list)
{
	list->prev = list;
	list->next = list;
}

शून्य
list_घुसाउ(संरचना list *list, संरचना list *elm)
{
	प्रतिज्ञा((list->next != रिक्त && list->prev != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	प्रतिज्ञा(((elm->next == रिक्त && elm->prev == रिक्त) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

शून्य
list_append(संरचना list *list, संरचना list *elm)
{
	प्रतिज्ञा((list->next != रिक्त && list->prev != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	प्रतिज्ञा(((elm->next == रिक्त && elm->prev == रिक्त) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->next = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->next = elm;
}

शून्य
list_remove(संरचना list *elm)
{
	प्रतिज्ञा((elm->next != रिक्त && elm->prev != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = रिक्त;
	elm->prev = रिक्त;
}

bool
list_empty(अचर संरचना list *list)
{
	प्रतिज्ञा((list->next != रिक्त && list->prev != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	फर्काऊ list->next == list;
}

bool
list_is_last(अचर संरचना list *list, अचर संरचना list *elm)
{
	फर्काऊ elm->next == list;
}
