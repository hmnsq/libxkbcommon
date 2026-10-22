/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

संरचना list {
	संरचना list *prev;
	संरचना list *next;
};

शून्य list_init(संरचना list *list);
शून्य list_घुसाउ(संरचना list *list, संरचना list *elm);
शून्य list_append(संरचना list *list, संरचना list *elm);
शून्य list_remove(संरचना list *elm);
bool list_empty(अचर संरचना list *list);
bool list_is_last(अचर संरचना list *list, अचर संरचना list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((वर्ण *)(ptr) -				\
		 दुरत्व(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	लागि ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	लागि ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
