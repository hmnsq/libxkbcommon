/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

بنية list {
	بنية list *prev;
	بنية list *ايندڙ;
};

فراغ list_init(بنية list *list);
فراغ list_وجهو(بنية list *list, بنية list *elm);
فراغ list_append(بنية list *list, بنية list *elm);
فراغ list_remove(بنية list *elm);
bool list_خالي_جايون(ثابت بنية list *list);
bool list_is_آخري(ثابت بنية list *list, ثابت بنية list *elm);

#وصف_ڪريو container_جو(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#وصف_ڪريو list_پهريون_داخلا(سر, pos, member)				\
	container_جو((سر)->ايندڙ, __typeof__(*(pos)), member)

#وصف_ڪريو list_آخري_داخلا(سر, pos, member)				\
	container_جو((سر)->prev, __typeof__(*(pos)), member)

#وصف_ڪريو list_for_each(pos, سر, member)				\
	لكل ((pos) = 0, (pos) = list_پهريون_داخلا(سر, pos, member);	\
	     &(pos)->member != (سر);					\
	     (pos) = list_پهريون_داخلا(&(pos)->member, pos, member))

#وصف_ڪريو list_for_each_محفوظ(pos, tmp, سر, member)			\
	لكل ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_پهريون_داخلا(سر, pos, member),			\
	     (tmp) = list_پهريون_داخلا(&(pos)->member, tmp, member);		\
	     &(pos)->member != (سر);					\
	     (pos) = (tmp),							\
	     (tmp) = list_پهريون_داخلا(&(pos)->member, tmp, member))
