/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stddef.h>
#समावेश <stdbool.h>

#समावेश "util-list.h"

व्योम
list_init(काष्ठा list *list)
{
	list->पूर्व = list;
	list->अग्र = list;
}

व्योम
list_जोड़ो(काष्ठा list *list, काष्ठा list *elm)
{
	निश्चित((list->अग्र != शून्य && list->पूर्व != शून्य) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	निश्चित(((elm->अग्र == शून्य && elm->पूर्व == शून्य) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->पूर्व = list;
	elm->अग्र = list->अग्र;
	list->अग्र = elm;
	elm->अग्र->पूर्व = elm;
}

व्योम
list_append(काष्ठा list *list, काष्ठा list *elm)
{
	निश्चित((list->अग्र != शून्य && list->पूर्व != शून्य) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	निश्चित(((elm->अग्र == शून्य && elm->पूर्व == शून्य) || list_empty(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->अग्र = list;
	elm->पूर्व = list->पूर्व;
	list->पूर्व = elm;
	elm->पूर्व->अग्र = elm;
}

व्योम
list_remove(काष्ठा list *elm)
{
	निश्चित((elm->अग्र != शून्य && elm->पूर्व != शून्य) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->पूर्व->अग्र = elm->अग्र;
	elm->अग्र->पूर्व = elm->पूर्व;
	elm->अग्र = शून्य;
	elm->पूर्व = शून्य;
}

bool
list_empty(स्थिर काष्ठा list *list)
{
	निश्चित((list->अग्र != शून्य && list->पूर्व != शून्य) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	वापस list->अग्र == list;
}

bool
list_is_last(स्थिर काष्ठा list *list, स्थिर काष्ठा list *elm)
{
	वापस elm->अग्र == list;
}
