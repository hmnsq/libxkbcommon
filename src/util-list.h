/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ഘടന list {
	ഘടന list *prev;
	ഘടന list *next;
};

ശൂന്യം list_init(ഘടന list *list);
ശൂന്യം list_തിരുകുക(ഘടന list *list, ഘടന list *elm);
ശൂന്യം list_append(ഘടന list *list, ഘടന list *elm);
ശൂന്യം list_remove(ഘടന list *elm);
bool list_empty(സ്ഥിരം ഘടന list *list);
bool list_is_last(സ്ഥിരം ഘടന list *list, സ്ഥിരം ഘടന list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((അക്ഷരം *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	വേണ്ടി ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	വേണ്ടി ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
