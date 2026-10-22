/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdalign.h>
#ضمن <stddef.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_serialize_config_v1 {
    uint32_t سائيز;
    uint32_t flags;
    uint32_t format;
    xkb_layout_نقاب_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_config, سائيز, flags);
assert_no_padding(بنية xkb_keymap_serialize_config, flags, format);
assert_no_padding(بنية xkb_keymap_serialize_config, format, layouts);
assert_no_padding(بنية xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_config) ==
              حجم(بنية xkb_keymap_serialize_config_v1), "");
assert_same_ميدان(بنية xkb_keymap_serialize_config, _v1, سائيز);
assert_same_ميدان(بنية xkb_keymap_serialize_config, _v1, flags);
assert_same_ميدان(بنية xkb_keymap_serialize_config, _v1, format);
assert_same_ميدان(بنية xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_config) * 30 <=
              (سائيز_t)XKB_ABI_MAX_سائيز, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_serialize_نتيجو_v1 {
    uint32_t سائيز;
    xkb_layout_نقاب_t layouts;
    حرف *serialized;
    سائيز_t ڊيگهائي;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_نتيجو, سائيز, layouts);
assert_no_padding(بنية xkb_keymap_serialize_نتيجو, layouts, serialized);
assert_no_padding(بنية xkb_keymap_serialize_نتيجو, serialized, ڊيگهائي);
assert_no_padding(بنية xkb_keymap_serialize_نتيجو, ڊيگهائي);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_نتيجو) ==
              حجم(بنية xkb_keymap_serialize_نتيجو_v1), "");
assert_same_ميدان(بنية xkb_keymap_serialize_نتيجو, _v1, سائيز);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ميدان(بنية xkb_keymap_serialize_نتيجو, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_ميدان(بنية xkb_keymap_serialize_نتيجو, _v1, ڊيگهائي);
assert_same_ميدان(بنية xkb_keymap_serialize_نتيجو, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_نتيجو) * 30 <=
              (سائيز_t)XKB_ABI_MAX_سائيز, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_ڪنجي_iterator_config_v1 {
    uint32_t سائيز;
    uint32_t flags;
    xkb_keycode_t شروعات;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_ڪنجي_iterator_config, سائيز, flags);
assert_no_padding(بنية xkb_keymap_ڪنجي_iterator_config, flags, شروعات);
assert_no_padding(بنية xkb_keymap_ڪنجي_iterator_config, شروعات);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_ڪنجي_iterator_config) ==
              حجم(بنية xkb_keymap_ڪنجي_iterator_config_v1), "");
assert_same_ميدان(بنية xkb_keymap_ڪنجي_iterator_config, _v1, سائيز);
assert_same_ميدان(بنية xkb_keymap_ڪنجي_iterator_config, _v1, flags);
assert_same_ميدان(بنية xkb_keymap_ڪنجي_iterator_config, _v1, شروعات);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_ڪنجي_iterator_config) * 30 <=
              (سائيز_t)XKB_ABI_MAX_سائيز, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#وصف_ڪريو xkb_versioned_struct_سائيز_v1(x) _Generic(        \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_نتيجو *:          \
        حجم(بنية xkb_keymap_serialize_نتيجو_v1),   \
    ثابت بنية xkb_keymap_ڪنجي_iterator_config *:      \
        حجم(بنية xkb_keymap_ڪنجي_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#وصف_ڪريو xkb_versioned_struct_سائيز_min(x) _Generic(       \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_نتيجو *:          \
        حجم(بنية xkb_keymap_serialize_نتيجو_v1),   \
    ثابت بنية xkb_keymap_ڪنجي_iterator_config *:      \
        حجم(بنية xkb_keymap_ڪنجي_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#وصف_ڪريو xkb_versioned_struct_reserved_وٿي(x) _Generic(\
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config),      \
    ثابت بنية xkb_keymap_serialize_نتيجو *:          \
        حجم(بنية xkb_keymap_serialize_نتيجو),      \
    ثابت بنية xkb_keymap_ڪنجي_iterator_config *:      \
        حجم(بنية xkb_keymap_ڪنجي_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_سائيز_v1(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_سائيز_v1(((ثابت بنية xkb_keymap_serialize_نتيجو *)عدم)) <=
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_serialize_نتيجو *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_سائيز_v1(((ثابت بنية xkb_keymap_ڪنجي_iterator_config *)عدم)) <=
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_ڪنجي_iterator_config *)عدم)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_serialize_نتيجو *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_نتيجو),
    ""
);
static_assert(
    xkb_versioned_struct_سائيز_min(((ثابت بنية xkb_keymap_ڪنجي_iterator_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_ڪنجي_iterator_config),
    ""
);

#وصف_ڪريو xkb_check_keymap_abi(x) xkb_check_versioned_struct_سائيز_2( \
    xkb_versioned_struct_سائيز_v1(x),                                               \
    xkb_versioned_struct_سائيز_min(x),                                              \
    xkb_versioned_struct_reserved_وٿي(x),                                       \
    (x)                                                                            \
)
