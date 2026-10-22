/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>
#समावेश <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

काष्ठा list {
	काष्ठा list *पूर्व;
	काष्ठा list *अग्र;
};

व्योम list_init(काष्ठा list *list);
व्योम list_जोड़ो(काष्ठा list *list, काष्ठा list *elm);
व्योम list_append(काष्ठा list *list, काष्ठा list *elm);
व्योम list_remove(काष्ठा list *elm);
bool list_empty(स्थिर काष्ठा list *list);
bool list_is_last(स्थिर काष्ठा list *list, स्थिर काष्ठा list *elm);

#घोषणा container_of(ptr, type, member)					\
	(__typeof__(type) *)((अक्षर *)(ptr) -				\
		 दुरत्व(__typeof__(type), member))

#घोषणा list_first_entry(शीर्ष, pos, member)				\
	container_of((शीर्ष)->अग्र, __typeof__(*(pos)), member)

#घोषणा list_last_entry(शीर्ष, pos, member)				\
	container_of((शीर्ष)->पूर्व, __typeof__(*(pos)), member)

#घोषणा list_for_each(pos, शीर्ष, member)				\
	क्रम ((pos) = 0, (pos) = list_first_entry(शीर्ष, pos, member);	\
	     &(pos)->member != (शीर्ष);					\
	     (pos) = list_first_entry(&(pos)->member, pos, member))

#घोषणा list_for_each_safe(pos, tmp, शीर्ष, member)			\
	क्रम ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_first_entry(शीर्ष, pos, member),			\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member);		\
	     &(pos)->member != (शीर्ष);					\
	     (pos) = (tmp),							\
	     (tmp) = list_first_entry(&(pos)->member, tmp, member))
