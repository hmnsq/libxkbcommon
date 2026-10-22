/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ساختار list {
	ساختار list *prev;
	ساختار list *next;
};

تهی list_init(ساختار list *list);
تهی list_درج(ساختار list *list, ساختار list *elm);
تهی list_append(ساختار list *list, ساختار list *elm);
تهی list_remove(ساختار list *elm);
bool list_empty(ثابت ساختار list *list);
bool list_is_last(ثابت ساختار list *list, ثابت ساختار list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((نویسه *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	برای ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	برای ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
