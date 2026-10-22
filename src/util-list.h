/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

সংরচনা list {
	সংরচনা list *prev;
	সংরচনা list *next;
};

শূন্য list_init(সংরচনা list *list);
শূন্য list_ঢোকাও(সংরচনা list *list, সংরচনা list *elm);
শূন্য list_append(সংরচনা list *list, সংরচনা list *elm);
শূন্য list_remove(সংরচনা list *elm);
bool list_empty(ধ্রুৱ সংরচনা list *list);
bool list_is_last(ধ্রুৱ সংরচনা list *list, ধ্রুৱ সংরচনা list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((ৱর্ণ *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	বাবে ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	বাবে ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
