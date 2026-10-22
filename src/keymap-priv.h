/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

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
    uint32_t اندازه;
    uint32_t flags;
    uint32_t format;
    xkb_layout_ماسک_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_config, اندازه, flags);
assert_no_padding(بنية xkb_keymap_serialize_config, flags, format);
assert_no_padding(بنية xkb_keymap_serialize_config, format, layouts);
assert_no_padding(بنية xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_config) ==
              حجم(بنية xkb_keymap_serialize_config_v1), "");
assert_same_ډګر(بنية xkb_keymap_serialize_config, _v1, اندازه);
assert_same_ډګر(بنية xkb_keymap_serialize_config, _v1, flags);
assert_same_ډګر(بنية xkb_keymap_serialize_config, _v1, format);
assert_same_ډګر(بنية xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_config) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_serialize_پایله_v1 {
    uint32_t اندازه;
    xkb_layout_ماسک_t layouts;
    حرف *serialized;
    اندازه_t اوږدتیا;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_پایله, اندازه, layouts);
assert_no_padding(بنية xkb_keymap_serialize_پایله, layouts, serialized);
assert_no_padding(بنية xkb_keymap_serialize_پایله, serialized, اوږدتیا);
assert_no_padding(بنية xkb_keymap_serialize_پایله, اوږدتیا);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_پایله) ==
              حجم(بنية xkb_keymap_serialize_پایله_v1), "");
assert_same_ډګر(بنية xkb_keymap_serialize_پایله, _v1, اندازه);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ډګر(بنية xkb_keymap_serialize_پایله, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_ډګر(بنية xkb_keymap_serialize_پایله, _v1, اوږدتیا);
assert_same_ډګر(بنية xkb_keymap_serialize_پایله, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_پایله) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_کلی_iterator_config_v1 {
    uint32_t اندازه;
    uint32_t flags;
    xkb_keycode_t پیل;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_کلی_iterator_config, اندازه, flags);
assert_no_padding(بنية xkb_keymap_کلی_iterator_config, flags, پیل);
assert_no_padding(بنية xkb_keymap_کلی_iterator_config, پیل);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_کلی_iterator_config) ==
              حجم(بنية xkb_keymap_کلی_iterator_config_v1), "");
assert_same_ډګر(بنية xkb_keymap_کلی_iterator_config, _v1, اندازه);
assert_same_ډګر(بنية xkb_keymap_کلی_iterator_config, _v1, flags);
assert_same_ډګر(بنية xkb_keymap_کلی_iterator_config, _v1, پیل);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_کلی_iterator_config) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#تعریف_کړه xkb_versioned_struct_اندازه_v1(x) _Generic(        \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_پایله *:          \
        حجم(بنية xkb_keymap_serialize_پایله_v1),   \
    ثابت بنية xkb_keymap_کلی_iterator_config *:      \
        حجم(بنية xkb_keymap_کلی_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#تعریف_کړه xkb_versioned_struct_اندازه_min(x) _Generic(       \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_پایله *:          \
        حجم(بنية xkb_keymap_serialize_پایله_v1),   \
    ثابت بنية xkb_keymap_کلی_iterator_config *:      \
        حجم(بنية xkb_keymap_کلی_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#تعریف_کړه xkb_versioned_struct_reserved_دننه_واټن(x) _Generic(\
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config),      \
    ثابت بنية xkb_keymap_serialize_پایله *:          \
        حجم(بنية xkb_keymap_serialize_پایله),      \
    ثابت بنية xkb_keymap_کلی_iterator_config *:      \
        حجم(بنية xkb_keymap_کلی_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_keymap_serialize_پایله *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_serialize_پایله *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_keymap_کلی_iterator_config *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_کلی_iterator_config *)عدم)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_serialize_پایله *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_پایله),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_keymap_کلی_iterator_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_کلی_iterator_config),
    ""
);

#تعریف_کړه xkb_check_keymap_abi(x) xkb_check_versioned_struct_اندازه_2( \
    xkb_versioned_struct_اندازه_v1(x),                                               \
    xkb_versioned_struct_اندازه_min(x),                                              \
    xkb_versioned_struct_reserved_دننه_واټن(x),                                       \
    (x)                                                                            \
)
