/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdalign.h>
#অন্তর্ভাব <stddef.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
কাঠামো xkb_keymap_serialize_config_v1 {
    uint32_t আকার_2;
    uint32_t flags;
    uint32_t format;
    xkb_layout_mask_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(কাঠামো xkb_keymap_serialize_config, আকার_2, flags);
assert_no_padding(কাঠামো xkb_keymap_serialize_config, flags, format);
assert_no_padding(কাঠামো xkb_keymap_serialize_config, format, layouts);
assert_no_padding(কাঠামো xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(আকার(কাঠামো xkb_keymap_serialize_config) ==
              আকার(কাঠামো xkb_keymap_serialize_config_v1), "");
assert_same_field(কাঠামো xkb_keymap_serialize_config, _v1, আকার_2);
assert_same_field(কাঠামো xkb_keymap_serialize_config, _v1, flags);
assert_same_field(কাঠামো xkb_keymap_serialize_config, _v1, format);
assert_same_field(কাঠামো xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(আকার(কাঠামো xkb_keymap_serialize_config) * 30 <=
              (আকার_t)XKB_ABI_MAX_আকার, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
কাঠামো xkb_keymap_serialize_ফলাফল_v1 {
    uint32_t আকার_2;
    xkb_layout_mask_t layouts;
    অক্ষর *serialized;
    আকার_t দৈর্ঘ্য;
};

/* Ensure there is no implicit padding */
assert_no_padding(কাঠামো xkb_keymap_serialize_ফলাফল, আকার_2, layouts);
assert_no_padding(কাঠামো xkb_keymap_serialize_ফলাফল, layouts, serialized);
assert_no_padding(কাঠামো xkb_keymap_serialize_ফলাফল, serialized, দৈর্ঘ্য);
assert_no_padding(কাঠামো xkb_keymap_serialize_ফলাফল, দৈর্ঘ্য);

/* Current version is 1 */
static_assert(আকার(কাঠামো xkb_keymap_serialize_ফলাফল) ==
              আকার(কাঠামো xkb_keymap_serialize_ফলাফল_v1), "");
assert_same_field(কাঠামো xkb_keymap_serialize_ফলাফল, _v1, আকার_2);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_field(কাঠামো xkb_keymap_serialize_ফলাফল, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_field(কাঠামো xkb_keymap_serialize_ফলাফল, _v1, দৈর্ঘ্য);
assert_same_field(কাঠামো xkb_keymap_serialize_ফলাফল, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(আকার(কাঠামো xkb_keymap_serialize_ফলাফল) * 30 <=
              (আকার_t)XKB_ABI_MAX_আকার, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
কাঠামো xkb_keymap_চাবি_iterator_config_v1 {
    uint32_t আকার_2;
    uint32_t flags;
    xkb_keycode_t start;
};

/* Ensure there is no implicit padding */
assert_no_padding(কাঠামো xkb_keymap_চাবি_iterator_config, আকার_2, flags);
assert_no_padding(কাঠামো xkb_keymap_চাবি_iterator_config, flags, start);
assert_no_padding(কাঠামো xkb_keymap_চাবি_iterator_config, start);

/* Current version is 1 */
static_assert(আকার(কাঠামো xkb_keymap_চাবি_iterator_config) ==
              আকার(কাঠামো xkb_keymap_চাবি_iterator_config_v1), "");
assert_same_field(কাঠামো xkb_keymap_চাবি_iterator_config, _v1, আকার_2);
assert_same_field(কাঠামো xkb_keymap_চাবি_iterator_config, _v1, flags);
assert_same_field(কাঠামো xkb_keymap_চাবি_iterator_config, _v1, start);

/* Ensure reasonable margin to the upper size limit */
static_assert(আকার(কাঠামো xkb_keymap_চাবি_iterator_config) * 30 <=
              (আকার_t)XKB_ABI_MAX_আকার, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#define xkb_versioned_struct_আকার_v1(x) _Generic(        \
    (x),                                                 \
    ধ্রুবক কাঠামো xkb_keymap_serialize_config *:          \
        আকার(কাঠামো xkb_keymap_serialize_config_v1),   \
    ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *:          \
        আকার(কাঠামো xkb_keymap_serialize_ফলাফল_v1),   \
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *:      \
        আকার(কাঠামো xkb_keymap_চাবি_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#define xkb_versioned_struct_আকার_min(x) _Generic(       \
    (x),                                                 \
    ধ্রুবক কাঠামো xkb_keymap_serialize_config *:          \
        আকার(কাঠামো xkb_keymap_serialize_config_v1),   \
    ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *:          \
        আকার(কাঠামো xkb_keymap_serialize_ফলাফল_v1),   \
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *:      \
        আকার(কাঠামো xkb_keymap_চাবি_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#define xkb_versioned_struct_reserved_offset(x) _Generic(\
    (x),                                                 \
    ধ্রুবক কাঠামো xkb_keymap_serialize_config *:          \
        আকার(কাঠামো xkb_keymap_serialize_config),      \
    ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *:          \
        আকার(কাঠামো xkb_keymap_serialize_ফলাফল),      \
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *:      \
        আকার(কাঠামো xkb_keymap_চাবি_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_আকার_v1(((ধ্রুবক কাঠামো xkb_keymap_serialize_config *)ফাঁকা)) <=
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_serialize_config *)ফাঁকা)),
    ""
);
static_assert(
    xkb_versioned_struct_আকার_v1(((ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *)ফাঁকা)) <=
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *)ফাঁকা)),
    ""
);
static_assert(
    xkb_versioned_struct_আকার_v1(((ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *)ফাঁকা)) <=
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *)ফাঁকা)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_serialize_config *)ফাঁকা)) <=
    আকার(ধ্রুবক কাঠামো xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল *)ফাঁকা)) <=
    আকার(ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল),
    ""
);
static_assert(
    xkb_versioned_struct_আকার_min(((ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config *)ফাঁকা)) <=
    আকার(ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config),
    ""
);

#define xkb_check_keymap_abi(x) xkb_check_versioned_struct_আকার_2( \
    xkb_versioned_struct_আকার_v1(x),                                               \
    xkb_versioned_struct_আকার_min(x),                                              \
    xkb_versioned_struct_reserved_offset(x),                                       \
    (x)                                                                            \
)
