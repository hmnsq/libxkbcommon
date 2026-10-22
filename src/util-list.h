/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

संरचना list {
	संरचना list *पूर्व;
	संरचना list *अग्र;
};

शून्य list_init(संरचना list *list);
शून्य list_निवेशय(संरचना list *list, संरचना list *elm);
शून्य list_append(संरचना list *list, संरचना list *elm);
शून्य list_remove(संरचना list *elm);
bool list_रिक्तानि(ध्रुव संरचना list *list);
bool list_is_चरमः(ध्रुव संरचना list *list, ध्रुव संरचना list *elm);

#परिभाषय container_सम्बन्धि(ptr, type, member)					\
	(__typeof__(type) *)((वर्ण *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#परिभाषय list_first_प्रविष्टिः(शीर्ष, pos, member)				\
	container_सम्बन्धि((शीर्ष)->अग्र, __typeof__(*(pos)), member)

#परिभाषय list_चरमः_प्रविष्टिः(शीर्ष, pos, member)				\
	container_सम्बन्धि((शीर्ष)->पूर्व, __typeof__(*(pos)), member)

#परिभाषय list_for_each(pos, शीर्ष, member)				\
	प्रति ((pos) = 0, (pos) = list_first_प्रविष्टिः(शीर्ष, pos, member);	\
	     &(pos)->member != (शीर्ष);					\
	     (pos) = list_first_प्रविष्टिः(&(pos)->member, pos, member))

#परिभाषय list_for_each_safe(pos, tmp, शीर्ष, member)			\
	प्रति ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_प्रविष्टिः(शीर्ष, pos, member),			\
	     (tmp) = list_first_प्रविष्टिः(&(pos)->member, tmp, member);		\
	     &(pos)->member != (शीर्ष);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_प्रविष्टिः(&(pos)->member, tmp, member))
