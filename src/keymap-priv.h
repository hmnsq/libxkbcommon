/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdalign.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ସଂରଚନା xkb_keymap_serialize_config_v1 {
    uint32_t size;
    uint32_t flags;
    uint32_t format;
    xkb_layout_mask_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_config, size, flags);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_config, flags, format);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_config, format, layouts);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_serialize_config) ==
              ଆକାର(ସଂରଚନା xkb_keymap_serialize_config_v1), "");
assert_same_field(ସଂରଚନା xkb_keymap_serialize_config, _v1, size);
assert_same_field(ସଂରଚନା xkb_keymap_serialize_config, _v1, flags);
assert_same_field(ସଂରଚନା xkb_keymap_serialize_config, _v1, format);
assert_same_field(ସଂରଚନା xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_serialize_config) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
ସଂରଚନା xkb_keymap_serialize_result_v1 {
    uint32_t size;
    xkb_layout_mask_t layouts;
    ଅକ୍ଷର *serialized;
    size_t length;
};

/* Ensure there is no implicit padding */
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_result, size, layouts);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_result, layouts, serialized);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_result, serialized, length);
assert_no_padding(ସଂରଚନା xkb_keymap_serialize_result, length);

/* Current version is 1 */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_serialize_result) ==
              ଆକାର(ସଂରଚନା xkb_keymap_serialize_result_v1), "");
assert_same_field(ସଂରଚନା xkb_keymap_serialize_result, _v1, size);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_field(ସଂରଚନା xkb_keymap_serialize_result, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_field(ସଂରଚନା xkb_keymap_serialize_result, _v1, length);
assert_same_field(ସଂରଚନା xkb_keymap_serialize_result, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_serialize_result) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ସଂରଚନା xkb_keymap_ଚାବି_iterator_config_v1 {
    uint32_t size;
    uint32_t flags;
    xkb_keycode_t start;
};

/* Ensure there is no implicit padding */
assert_no_padding(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, size, flags);
assert_no_padding(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, flags, start);
assert_no_padding(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, start);

/* Current version is 1 */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config) ==
              ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config_v1), "");
assert_same_field(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, _v1, size);
assert_same_field(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, _v1, flags);
assert_same_field(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config, _v1, start);

/* Ensure reasonable margin to the upper size limit */
static_assert(ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#define xkb_versioned_struct_size_v1(x) _Generic(        \
    (x),                                                 \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_config_v1),   \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_result_v1),   \
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *:      \
        ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#define xkb_versioned_struct_size_min(x) _Generic(       \
    (x),                                                 \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_config_v1),   \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_result_v1),   \
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *:      \
        ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#define xkb_versioned_struct_reserved_offset(x) _Generic(\
    (x),                                                 \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_config),      \
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *:          \
        ଆକାର(ସଂରଚନା xkb_keymap_serialize_result),      \
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *:      \
        ଆକାର(ସଂରଚନା xkb_keymap_ଚାବି_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_size_v1(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *)ଖାଲି)) <=
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *)ଖାଲି)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *)ଖାଲି)) <=
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *)ଖାଲି)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *)ଖାଲି)) <=
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *)ଖାଲି)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *)ଖାଲି)) <=
    ଆକାର(ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result *)ଖାଲି)) <=
    ଆକାର(ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config *)ଖାଲି)) <=
    ଆକାର(ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config),
    ""
);

#define xkb_check_keymap_abi(x) xkb_check_versioned_struct_size( \
    xkb_versioned_struct_size_v1(x),                                               \
    xkb_versioned_struct_size_min(x),                                              \
    xkb_versioned_struct_reserved_offset(x),                                       \
    (x)                                                                            \
)
