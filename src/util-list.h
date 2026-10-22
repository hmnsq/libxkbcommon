/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ಸಂರಚನಾ list {
	ಸಂರಚನಾ list *prev;
	ಸಂರಚನಾ list *next;
};

ಶೂನ್ಯ list_init(ಸಂರಚನಾ list *list);
ಶೂನ್ಯ list_ಸೇರಿಸು(ಸಂರಚನಾ list *list, ಸಂರಚನಾ list *elm);
ಶೂನ್ಯ list_append(ಸಂರಚನಾ list *list, ಸಂರಚನಾ list *elm);
ಶೂನ್ಯ list_remove(ಸಂರಚನಾ list *elm);
bool list_empty(ಧ್ರುವ ಸಂರಚನಾ list *list);
bool list_is_last(ಧ್ರುವ ಸಂರಚನಾ list *list, ಧ್ರುವ ಸಂರಚನಾ list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((ವರ್ಣ *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	ಪ್ರತಿ ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	ಪ್ರತಿ ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
