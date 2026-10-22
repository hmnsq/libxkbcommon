/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل <assert.h>
#شامل <stdalign.h>
#شامل <stddef.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_keymap_serialize_config_v1 {
    uint32_t سائز;
    uint32_t flags;
    uint32_t format;
    xkb_layout_نقاب_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_keymap_serialize_config, سائز, flags);
assert_no_padding(ساخت xkb_keymap_serialize_config, flags, format);
assert_no_padding(ساخت xkb_keymap_serialize_config, format, layouts);
assert_no_padding(ساخت xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_keymap_serialize_config) ==
              حجم(ساخت xkb_keymap_serialize_config_v1), "");
assert_same_میدان(ساخت xkb_keymap_serialize_config, _v1, سائز);
assert_same_میدان(ساخت xkb_keymap_serialize_config, _v1, flags);
assert_same_میدان(ساخت xkb_keymap_serialize_config, _v1, format);
assert_same_میدان(ساخت xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_keymap_serialize_config) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_keymap_serialize_نتیجہ_v1 {
    uint32_t سائز;
    xkb_layout_نقاب_t layouts;
    حرف *serialized;
    سائز_t لمبائی;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_keymap_serialize_نتیجہ, سائز, layouts);
assert_no_padding(ساخت xkb_keymap_serialize_نتیجہ, layouts, serialized);
assert_no_padding(ساخت xkb_keymap_serialize_نتیجہ, serialized, لمبائی);
assert_no_padding(ساخت xkb_keymap_serialize_نتیجہ, لمبائی);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_keymap_serialize_نتیجہ) ==
              حجم(ساخت xkb_keymap_serialize_نتیجہ_v1), "");
assert_same_میدان(ساخت xkb_keymap_serialize_نتیجہ, _v1, سائز);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_میدان(ساخت xkb_keymap_serialize_نتیجہ, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_میدان(ساخت xkb_keymap_serialize_نتیجہ, _v1, لمبائی);
assert_same_میدان(ساخت xkb_keymap_serialize_نتیجہ, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_keymap_serialize_نتیجہ) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_keymap_کلید_iterator_config_v1 {
    uint32_t سائز;
    uint32_t flags;
    xkb_keycode_t آغاز;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_keymap_کلید_iterator_config, سائز, flags);
assert_no_padding(ساخت xkb_keymap_کلید_iterator_config, flags, آغاز);
assert_no_padding(ساخت xkb_keymap_کلید_iterator_config, آغاز);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_keymap_کلید_iterator_config) ==
              حجم(ساخت xkb_keymap_کلید_iterator_config_v1), "");
assert_same_میدان(ساخت xkb_keymap_کلید_iterator_config, _v1, سائز);
assert_same_میدان(ساخت xkb_keymap_کلید_iterator_config, _v1, flags);
assert_same_میدان(ساخت xkb_keymap_کلید_iterator_config, _v1, آغاز);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_keymap_کلید_iterator_config) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#تعریف_کرو xkb_versioned_struct_سائز_v1(x) _Generic(        \
    (x),                                                 \
    ثابت ساخت xkb_keymap_serialize_config *:          \
        حجم(ساخت xkb_keymap_serialize_config_v1),   \
    ثابت ساخت xkb_keymap_serialize_نتیجہ *:          \
        حجم(ساخت xkb_keymap_serialize_نتیجہ_v1),   \
    ثابت ساخت xkb_keymap_کلید_iterator_config *:      \
        حجم(ساخت xkb_keymap_کلید_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#تعریف_کرو xkb_versioned_struct_سائز_min(x) _Generic(       \
    (x),                                                 \
    ثابت ساخت xkb_keymap_serialize_config *:          \
        حجم(ساخت xkb_keymap_serialize_config_v1),   \
    ثابت ساخت xkb_keymap_serialize_نتیجہ *:          \
        حجم(ساخت xkb_keymap_serialize_نتیجہ_v1),   \
    ثابت ساخت xkb_keymap_کلید_iterator_config *:      \
        حجم(ساخت xkb_keymap_کلید_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#تعریف_کرو xkb_versioned_struct_reserved_انحراف(x) _Generic(\
    (x),                                                 \
    ثابت ساخت xkb_keymap_serialize_config *:          \
        حجم(ساخت xkb_keymap_serialize_config),      \
    ثابت ساخت xkb_keymap_serialize_نتیجہ *:          \
        حجم(ساخت xkb_keymap_serialize_نتیجہ),      \
    ثابت ساخت xkb_keymap_کلید_iterator_config *:      \
        حجم(ساخت xkb_keymap_کلید_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_keymap_serialize_config *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_serialize_config *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_keymap_serialize_نتیجہ *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_serialize_نتیجہ *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_keymap_کلید_iterator_config *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_کلید_iterator_config *)تہی)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_serialize_config *)تہی)) <=
    حجم(ثابت ساخت xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_serialize_نتیجہ *)تہی)) <=
    حجم(ثابت ساخت xkb_keymap_serialize_نتیجہ),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_keymap_کلید_iterator_config *)تہی)) <=
    حجم(ثابت ساخت xkb_keymap_کلید_iterator_config),
    ""
);

#تعریف_کرو xkb_check_keymap_abi(x) xkb_check_versioned_struct_سائز_2( \
    xkb_versioned_struct_سائز_v1(x),                                               \
    xkb_versioned_struct_سائز_min(x),                                              \
    xkb_versioned_struct_reserved_انحراف(x),                                       \
    (x)                                                                            \
)
