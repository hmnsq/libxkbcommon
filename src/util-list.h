/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

నిర్మాణం list {
	నిర్మాణం list *prev;
	నిర్మాణం list *తదుపరి;
};

శూన్య list_init(నిర్మాణం list *list);
శూన్య list_చొప్పించు(నిర్మాణం list *list, నిర్మాణం list *elm);
శూన్య list_append(నిర్మాణం list *list, నిర్మాణం list *elm);
శూన్య list_remove(నిర్మాణం list *elm);
bool list_ఖాళీలు(స్థిరరాశి నిర్మాణం list *list);
bool list_is_చివరి(స్థిరరాశి నిర్మాణం list *list, స్థిరరాశి నిర్మాణం list *elm);

#నిర్వచించు container_యొక్క(ptr, type, member)					\
	(__typeof__(type) *)((అక్షరం *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#నిర్వచించు list_మొదటి_నమోదు(తల, pos, member)				\
	container_యొక్క((తల)->తదుపరి, __typeof__(*(pos)), member)

#నిర్వచించు list_చివరి_నమోదు(తల, pos, member)				\
	container_యొక్క((తల)->prev, __typeof__(*(pos)), member)

#నిర్వచించు list_for_each(pos, తల, member)				\
	కొరకు ((pos) = 0, (pos) = list_మొదటి_నమోదు(తల, pos, member);	\
	     &(pos)->member != (తల);					\
	     (pos) = list_మొదటి_నమోదు(&(pos)->member, pos, member))

#నిర్వచించు list_for_each_సురక్షిత(pos, tmp, తల, member)			\
	కొరకు ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_మొదటి_నమోదు(తల, pos, member),			\
	     (tmp) = list_మొదటి_నమోదు(&(pos)->member, tmp, member);		\
	     &(pos)->member != (తల);					\
	     (pos) = (tmp),							\
	     (tmp) = list_మొదటి_నమోదు(&(pos)->member, tmp, member))
