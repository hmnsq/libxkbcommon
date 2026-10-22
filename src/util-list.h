/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

بنية list {
	بنية list *prev;
	بنية list *بل;
};

فراغ list_init(بنية list *list);
فراغ list_ورننباسه(بنية list *list, بنية list *elm);
فراغ list_append(بنية list *list, بنية list *elm);
فراغ list_remove(بنية list *elm);
bool list_تشه(ثابت بنية list *list);
bool list_is_وروستی(ثابت بنية list *list, ثابت بنية list *elm);

#تعریف_کړه container_د(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#تعریف_کړه list_لومړی_ثبت(سر, pos, member)				\
	container_د((سر)->بل, __typeof__(*(pos)), member)

#تعریف_کړه list_وروستی_ثبت(سر, pos, member)				\
	container_د((سر)->prev, __typeof__(*(pos)), member)

#تعریف_کړه list_for_each(pos, سر, member)				\
	لكل ((pos) = 0, (pos) = list_لومړی_ثبت(سر, pos, member);	\
	     &(pos)->member != (سر);					\
	     (pos) = list_لومړی_ثبت(&(pos)->member, pos, member))

#تعریف_کړه list_for_each_خوندي(pos, tmp, سر, member)			\
	لكل ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_لومړی_ثبت(سر, pos, member),			\
	     (tmp) = list_لومړی_ثبت(&(pos)->member, tmp, member);		\
	     &(pos)->member != (سر);					\
	     (pos) = (tmp),							\
	     (tmp) = list_لومړی_ثبت(&(pos)->member, tmp, member))
