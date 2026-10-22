/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

بنية list {
	بنية list *prev;
	بنية list *next;
};

فراغ list_init(بنية list *list);
فراغ list_داخل_کرو(بنية list *list, بنية list *elm);
فراغ list_append(بنية list *list, بنية list *elm);
فراغ list_remove(بنية list *elm);
bool list_empty(ثابت بنية list *list);
bool list_is_last(ثابت بنية list *list, ثابت بنية list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	لكل ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	لكل ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
