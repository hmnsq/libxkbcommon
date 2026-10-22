/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

بنية list {
	بنية list *prev;
	بنية list *اگلا;
};

فراغ list_init(بنية list *list);
فراغ list_داخل_کرو(بنية list *list, بنية list *elm);
فراغ list_append(بنية list *list, بنية list *elm);
فراغ list_remove(بنية list *elm);
bool list_خالی_تھاواں(ثابت بنية list *list);
bool list_is_آخری(ثابت بنية list *list, ثابت بنية list *elm);

#تعریف_کرو container_دا(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#تعریف_کرو list_پہلا_اندراج(سر, pos, member)				\
	container_دا((سر)->اگلا, __typeof__(*(pos)), member)

#تعریف_کرو list_آخری_اندراج(سر, pos, member)				\
	container_دا((سر)->prev, __typeof__(*(pos)), member)

#تعریف_کرو list_for_each(pos, سر, member)				\
	لكل ((pos) = 0, (pos) = list_پہلا_اندراج(سر, pos, member);	\
	     &(pos)->member != (سر);					\
	     (pos) = list_پہلا_اندراج(&(pos)->member, pos, member))

#تعریف_کرو list_for_each_محفوظ(pos, tmp, سر, member)			\
	لكل ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_پہلا_اندراج(سر, pos, member),			\
	     (tmp) = list_پہلا_اندراج(&(pos)->member, tmp, member);		\
	     &(pos)->member != (سر);					\
	     (pos) = (tmp),							\
	     (tmp) = list_پہلا_اندراج(&(pos)->member, tmp, member))
