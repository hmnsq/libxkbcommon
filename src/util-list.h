/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

بنية list {
	بنية list *prev;
	بنية list *بعدی;
};

فراغ list_init(بنية list *list);
فراغ list_درج(بنية list *list, بنية list *elm);
فراغ list_append(بنية list *list, بنية list *elm);
فراغ list_remove(بنية list *elm);
bool list_خالی(ثابت بنية list *list);
bool list_is_آخرین(ثابت بنية list *list, ثابت بنية list *elm);

#تعریف_کن container_از(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#تعریف_کن list_اولی_مدخل(سر, pos, member)				\
	container_از((سر)->بعدی, __typeof__(*(pos)), member)

#تعریف_کن list_آخرین_مدخل(سر, pos, member)				\
	container_از((سر)->prev, __typeof__(*(pos)), member)

#تعریف_کن list_for_each(pos, سر, member)				\
	لكل ((pos) = 0, (pos) = list_اولی_مدخل(سر, pos, member);	\
	     &(pos)->member != (سر);					\
	     (pos) = list_اولی_مدخل(&(pos)->member, pos, member))

#تعریف_کن list_for_each_امن(pos, tmp, سر, member)			\
	لكل ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_اولی_مدخل(سر, pos, member),			\
	     (tmp) = list_اولی_مدخل(&(pos)->member, tmp, member);		\
	     &(pos)->member != (سر);					\
	     (pos) = (tmp),							\
	     (tmp) = list_اولی_مدخل(&(pos)->member, tmp, member))
