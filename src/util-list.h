/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

கட்டமைப்பு list {
	கட்டமைப்பு list *prev;
	கட்டமைப்பு list *next;
};

வெறுமை list_init(கட்டமைப்பு list *list);
வெறுமை list_செருகு(கட்டமைப்பு list *list, கட்டமைப்பு list *elm);
வெறுமை list_append(கட்டமைப்பு list *list, கட்டமைப்பு list *elm);
வெறுமை list_remove(கட்டமைப்பு list *elm);
bool list_empty(மாறிலி கட்டமைப்பு list *list);
bool list_is_last(மாறிலி கட்டமைப்பு list *list, மாறிலி கட்டமைப்பு list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((எழுத்து *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*(pos)), member)

#define list_last_entry(head, pos, member)				\
	container_of((head)->prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head, member)				\
	க்கு ((pos) = 0, (pos) = list_first_entry(head, pos, member);	\
	     &(pos)->member != (head);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	க்கு ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(head, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (head);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
