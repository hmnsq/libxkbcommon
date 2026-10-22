/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

સંરચના list {
	સંરચના list *prev;
	સંરચના list *next;
};

શૂન્ય list_init(સંરચના list *list);
શૂન્ય list_ઉમેરો(સંરચના list *list, સંરચના list *elm);
શૂન્ય list_append(સંરચના list *list, સંરચના list *elm);
શૂન્ય list_remove(સંરચના list *elm);
bool list_empty(અચલ સંરચના list *list);
bool list_is_last(અચલ સંરચના list *list, અચલ સંરચના list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((અક્ષર *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	માટે ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	માટે ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
