/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ସଂରଚନା list {
	ସଂରଚନା list *prev;
	ସଂରଚନା list *next;
};

ଶୂନ୍ୟ list_init(ସଂରଚନା list *list);
ଶୂନ୍ୟ list_ଭର୍ତ୍ତି(ସଂରଚନା list *list, ସଂରଚନା list *elm);
ଶୂନ୍ୟ list_append(ସଂରଚନା list *list, ସଂରଚନା list *elm);
ଶୂନ୍ୟ list_remove(ସଂରଚନା list *elm);
bool list_empty(ଅଚଳ ସଂରଚନା list *list);
bool list_is_last(ଅଚଳ ସଂରଚନା list *list, ଅଚଳ ସଂରଚନା list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((ଅକ୍ଷର *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	ପାଇଁ ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	ପାଇଁ ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
