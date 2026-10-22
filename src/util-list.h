/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2013-2015 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

ರಚನೆ list {
	ರಚನೆ list *prev;
	ರಚನೆ list *ಮುಂದಿನ;
};

ಶೂನ್ಯ list_init(ರಚನೆ list *list);
ಶೂನ್ಯ list_ಸೇರಿಸು(ರಚನೆ list *list, ರಚನೆ list *elm);
ಶೂನ್ಯ list_append(ರಚನೆ list *list, ರಚನೆ list *elm);
ಶೂನ್ಯ list_remove(ರಚನೆ list *elm);
bool list_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ list *list);
bool list_is_ಕೊನೆಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ list *list, ಸ್ಥಿರಾಂಕ ರಚನೆ list *elm);

#ವ್ಯಾಖ್ಯಾನಿಸು container_ದ(ptr, type, member)					\
	(__typeof__(type) *)((ಅಕ್ಷರ *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#ವ್ಯಾಖ್ಯಾನಿಸು list_ಮೊದಲ_ನಮೂದು(ತಲೆ, pos, member)				\
	container_ದ((ತಲೆ)->ಮುಂದಿನ, __typeof__(*(pos)), member)

#ವ್ಯಾಖ್ಯಾನಿಸು list_ಕೊನೆಯ_ನಮೂದು(ತಲೆ, pos, member)				\
	container_ದ((ತಲೆ)->prev, __typeof__(*(pos)), member)

#ವ್ಯಾಖ್ಯಾನಿಸು list_for_each(pos, ತಲೆ, member)				\
	ಗಾಗಿ ((pos) = 0, (pos) = list_ಮೊದಲ_ನಮೂದು(ತಲೆ, pos, member);	\
	     &(pos)->member != (ತಲೆ);					\
	     (pos) = list_ಮೊದಲ_ನಮೂದು(&(pos)->member, pos, member))

#ವ್ಯಾಖ್ಯಾನಿಸು list_for_each_ಸುರಕ್ಷಿತ(pos, tmp, ತಲೆ, member)			\
	ಗಾಗಿ ((pos) = 0, (tmp) = 0,						\
	     (pos) = list_ಮೊದಲ_ನಮೂದು(ತಲೆ, pos, member),			\
	     (tmp) = list_ಮೊದಲ_ನಮೂದು(&(pos)->member, tmp, member);		\
	     &(pos)->member != (ತಲೆ);					\
	     (pos) = (tmp),							\
	     (tmp) = list_ಮೊದಲ_ನಮೂದು(&(pos)->member, tmp, member))
