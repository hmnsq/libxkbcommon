/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "util-list.h"

శూన్య
list_init(నిర్మాణం list *list)
{
	list->prev = list;
	list->తదుపరి = list;
}

శూన్య
list_చొప్పించు(నిర్మాణం list *list, నిర్మాణం list *elm)
{
	ప్రతిజ్ఞా((list->తదుపరి != ఖాళీ && list->prev != ఖాళీ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ప్రతిజ్ఞా(((elm->తదుపరి == ఖాళీ && elm->prev == ఖాళీ) || list_ఖాళీలు(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->prev = list;
	elm->తదుపరి = list->తదుపరి;
	list->తదుపరి = elm;
	elm->తదుపరి->prev = elm;
}

శూన్య
list_append(నిర్మాణం list *list, నిర్మాణం list *elm)
{
	ప్రతిజ్ఞా((list->తదుపరి != ఖాళీ && list->prev != ఖాళీ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	ప్రతిజ్ఞా(((elm->తదుపరి == ఖాళీ && elm->prev == ఖాళీ) || list_ఖాళీలు(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->తదుపరి = list;
	elm->prev = list->prev;
	list->prev = elm;
	elm->prev->తదుపరి = elm;
}

శూన్య
list_remove(నిర్మాణం list *elm)
{
	ప్రతిజ్ఞా((elm->తదుపరి != ఖాళీ && elm->prev != ఖాళీ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->prev->తదుపరి = elm->తదుపరి;
	elm->తదుపరి->prev = elm->prev;
	elm->తదుపరి = ఖాళీ;
	elm->prev = ఖాళీ;
}

bool
list_ఖాళీలు(స్థిరరాశి నిర్మాణం list *list)
{
	ప్రతిజ్ఞా((list->తదుపరి != ఖాళీ && list->prev != ఖాళీ) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	తిరిగి list->తదుపరి == list;
}

bool
list_is_చివరి(స్థిరరాశి నిర్మాణం list *list, స్థిరరాశి నిర్మాణం list *elm)
{
	తిరిగి elm->తదుపరి == list;
}
