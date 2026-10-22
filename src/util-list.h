/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

কাঠামো list {
	কাঠামো list *পূর্ব;
	কাঠামো list *পরবর্তী;
};

শূন্য list_init(কাঠামো list *list);
শূন্য list_ঢোকাও(কাঠামো list *list, কাঠামো list *elm);
শূন্য list_append(কাঠামো list *list, কাঠামো list *elm);
শূন্য list_remove(কাঠামো list *elm);
bool list_empty(ধ্রুবক কাঠামো list *list);
bool list_is_last(ধ্রুবক কাঠামো list *list, ধ্রুবক কাঠামো list *elm);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((অক্ষর *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(শীর্ষ, pos, member)				\
	container_of((শীর্ষ)->পরবর্তী, __typeof__(*(pos)), member)

#define list_last_entry(শীর্ষ, pos, member)				\
	container_of((শীর্ষ)->পূর্ব, __typeof__(*(pos)), member)

#define list_for_each(pos, শীর্ষ, member)				\
	জন্য ((pos) = 0, (pos) = list_first_entry(শীর্ষ, pos, member);	\
	     &(pos)->member != (শীর্ষ);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#define list_for_each_safe(pos, tmp, শীর্ষ, member)			\
	জন্য ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(শীর্ষ, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (শীর্ষ);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
