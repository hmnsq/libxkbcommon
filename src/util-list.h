/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ساخت list {
	ساخت list *پچھلا;
	ساخت list *اگلا;
};

خالی list_init(ساخت list *list);
خالی list_داخل_کرو(ساخت list *list, ساخت list *elm);
خالی list_append(ساخت list *list, ساخت list *elm);
خالی list_remove(ساخت list *elm);
bool list_خالی_خانے(ثابت ساخت list *list);
bool list_is_آخری(ثابت ساخت list *list, ثابت ساخت list *elm);

#تعریف_کرو container_کا(ptr, type, member)					\
	(__typeof__(type) *)((حرف *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#تعریف_کرو list_پہلا_اندراج(سر, pos, member)				\
	container_کا((سر)->اگلا, __typeof__(*(pos)), member)

#تعریف_کرو list_آخری_اندراج(سر, pos, member)				\
	container_کا((سر)->پچھلا, __typeof__(*(pos)), member)

#تعریف_کرو list_for_each(pos, سر, member)				\
	برائے ((pos) = 0, (pos) = list_پہلا_اندراج(سر, pos, member);	\
	     &(pos)->member != (سر);					\
	     (pos) = list_پہلا_اندراج(&(pos)->member, pos, member))

#تعریف_کرو list_for_each_محفوظ(pos, tmp, سر, member)			\
	برائے ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_پہلا_اندراج(سر, pos, member),			\
	     (tmp) = list_پہلا_اندراج(&(pos)->member, tmp, member);		\
	     &(pos)->member != (سر);					\
	     (pos) = (tmp),							\
	     (tmp) = list_پہلا_اندراج(&(pos)->member, tmp, member))
