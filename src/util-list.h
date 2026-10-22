/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ਬਣਤਰ list {
	ਬਣਤਰ list *prev;
	ਬਣਤਰ list *ਅਗਲਾ;
};

ਖਾਲੀ list_init(ਬਣਤਰ list *list);
ਖਾਲੀ list_ਪਾਓ(ਬਣਤਰ list *list, ਬਣਤਰ list *elm);
ਖਾਲੀ list_append(ਬਣਤਰ list *list, ਬਣਤਰ list *elm);
ਖਾਲੀ list_remove(ਬਣਤਰ list *elm);
bool list_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ list *list);
bool list_is_ਆਖਰੀ(ਅਚਲ ਬਣਤਰ list *list, ਅਚਲ ਬਣਤਰ list *elm);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ container_ਦਾ(ptr, type, member)					\
	(__typeof__(type) *)((ਅੱਖਰ *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(ਸਿਰ, pos, member)				\
	container_ਦਾ((ਸਿਰ)->ਅਗਲਾ, __typeof__(*(pos)), member)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ list_ਆਖਰੀ_ਇੰਦਰਾਜ(ਸਿਰ, pos, member)				\
	container_ਦਾ((ਸਿਰ)->prev, __typeof__(*(pos)), member)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ list_for_each(pos, ਸਿਰ, member)				\
	ਲਈ ((pos) = 0, (pos) = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(ਸਿਰ, pos, member);	\
	     &(pos)->member != (ਸਿਰ);					\
	     (pos) = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&(pos)->member, pos, member))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ list_for_each_ਸੁਰੱਖਿਅਤ(pos, tmp, ਸਿਰ, member)			\
	ਲਈ ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(ਸਿਰ, pos, member),			\
	     (tmp) = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&(pos)->member, tmp, member);		\
	     &(pos)->member != (ਸਿਰ);					\
	     (pos) = (tmp),							\
	     (tmp) = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&(pos)->member, tmp, member))
