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
	list->पूर्व = list;
	list->अग्र = list;
}

शून्य
list_निवेशय(संरचना list *list, संरचना list *elm)
{
	प्रतिज्ञा((list->अग्र != रिक्त && list->पूर्व != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	प्रतिज्ञा(((elm->अग्र == रिक्त && elm->पूर्व == रिक्त) || list_रिक्तानि(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->पूर्व = list;
	elm->अग्र = list->अग्र;
	list->अग्र = elm;
	elm->अग्र->पूर्व = elm;
}

शून्य
list_append(संरचना list *list, संरचना list *elm)
{
	प्रतिज्ञा((list->अग्र != रिक्त && list->पूर्व != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");
	प्रतिज्ञा(((elm->अग्र == रिक्त && elm->पूर्व == रिक्त) || list_रिक्तानि(elm)) ||
	       !"elm->next|prev is not NULL, list node used twice?");

	elm->अग्र = list;
	elm->पूर्व = list->पूर्व;
	list->पूर्व = elm;
	elm->पूर्व->अग्र = elm;
}

शून्य
list_remove(संरचना list *elm)
{
	प्रतिज्ञा((elm->अग्र != रिक्त && elm->पूर्व != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	elm->पूर्व->अग्र = elm->अग्र;
	elm->अग्र->पूर्व = elm->पूर्व;
	elm->अग्र = रिक्त;
	elm->पूर्व = रिक्त;
}

bool
list_रिक्तानि(ध्रुव संरचना list *list)
{
	प्रतिज्ञा((list->अग्र != रिक्त && list->पूर्व != रिक्त) ||
	       !"list->next|prev is NULL, possibly missing list_init()");

	प्रत्यागम list->अग्र == list;
}

bool
list_is_चरमः(ध्रुव संरचना list *list, ध्रुव संरचना list *elm)
{
	प्रत्यागम elm->अग्र == list;
}
