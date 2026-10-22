/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

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
    uint32_t ناپ;
    uint32_t flags;
    uint32_t format;
    xkb_layout_نقاب_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_config, ناپ, flags);
assert_no_padding(بنية xkb_keymap_serialize_config, flags, format);
assert_no_padding(بنية xkb_keymap_serialize_config, format, layouts);
assert_no_padding(بنية xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_config) ==
              حجم(بنية xkb_keymap_serialize_config_v1), "");
assert_same_میدان(بنية xkb_keymap_serialize_config, _v1, ناپ);
assert_same_میدان(بنية xkb_keymap_serialize_config, _v1, flags);
assert_same_میدان(بنية xkb_keymap_serialize_config, _v1, format);
assert_same_میدان(بنية xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_config) * 30 <=
              (ناپ_t)XKB_ABI_MAX_ناپ, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_serialize_نتیجہ_v1 {
    uint32_t ناپ;
    xkb_layout_نقاب_t layouts;
    حرف *serialized;
    ناپ_t لمائی;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_serialize_نتیجہ, ناپ, layouts);
assert_no_padding(بنية xkb_keymap_serialize_نتیجہ, layouts, serialized);
assert_no_padding(بنية xkb_keymap_serialize_نتیجہ, serialized, لمائی);
assert_no_padding(بنية xkb_keymap_serialize_نتیجہ, لمائی);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_serialize_نتیجہ) ==
              حجم(بنية xkb_keymap_serialize_نتیجہ_v1), "");
assert_same_میدان(بنية xkb_keymap_serialize_نتیجہ, _v1, ناپ);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_میدان(بنية xkb_keymap_serialize_نتیجہ, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_میدان(بنية xkb_keymap_serialize_نتیجہ, _v1, لمائی);
assert_same_میدان(بنية xkb_keymap_serialize_نتیجہ, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_serialize_نتیجہ) * 30 <=
              (ناپ_t)XKB_ABI_MAX_ناپ, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_keymap_کلید_iterator_config_v1 {
    uint32_t ناپ;
    uint32_t flags;
    xkb_keycode_t شروع;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_keymap_کلید_iterator_config, ناپ, flags);
assert_no_padding(بنية xkb_keymap_کلید_iterator_config, flags, شروع);
assert_no_padding(بنية xkb_keymap_کلید_iterator_config, شروع);

/* Current version is 1 */
static_assert(حجم(بنية xkb_keymap_کلید_iterator_config) ==
              حجم(بنية xkb_keymap_کلید_iterator_config_v1), "");
assert_same_میدان(بنية xkb_keymap_کلید_iterator_config, _v1, ناپ);
assert_same_میدان(بنية xkb_keymap_کلید_iterator_config, _v1, flags);
assert_same_میدان(بنية xkb_keymap_کلید_iterator_config, _v1, شروع);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_keymap_کلید_iterator_config) * 30 <=
              (ناپ_t)XKB_ABI_MAX_ناپ, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#تعریف_کرو xkb_versioned_struct_ناپ_v1(x) _Generic(        \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_نتیجہ *:          \
        حجم(بنية xkb_keymap_serialize_نتیجہ_v1),   \
    ثابت بنية xkb_keymap_کلید_iterator_config *:      \
        حجم(بنية xkb_keymap_کلید_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#تعریف_کرو xkb_versioned_struct_ناپ_min(x) _Generic(       \
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config_v1),   \
    ثابت بنية xkb_keymap_serialize_نتیجہ *:          \
        حجم(بنية xkb_keymap_serialize_نتیجہ_v1),   \
    ثابت بنية xkb_keymap_کلید_iterator_config *:      \
        حجم(بنية xkb_keymap_کلید_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#تعریف_کرو xkb_versioned_struct_reserved_وتھ(x) _Generic(\
    (x),                                                 \
    ثابت بنية xkb_keymap_serialize_config *:          \
        حجم(بنية xkb_keymap_serialize_config),      \
    ثابت بنية xkb_keymap_serialize_نتیجہ *:          \
        حجم(بنية xkb_keymap_serialize_نتیجہ),      \
    ثابت بنية xkb_keymap_کلید_iterator_config *:      \
        حجم(بنية xkb_keymap_کلید_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_ناپ_v1(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_ناپ_v1(((ثابت بنية xkb_keymap_serialize_نتیجہ *)عدم)) <=
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_serialize_نتیجہ *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_ناپ_v1(((ثابت بنية xkb_keymap_کلید_iterator_config *)عدم)) <=
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_کلید_iterator_config *)عدم)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_serialize_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_serialize_نتیجہ *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_serialize_نتیجہ),
    ""
);
static_assert(
    xkb_versioned_struct_ناپ_min(((ثابت بنية xkb_keymap_کلید_iterator_config *)عدم)) <=
    حجم(ثابت بنية xkb_keymap_کلید_iterator_config),
    ""
);

#تعریف_کرو xkb_check_keymap_abi(x) xkb_check_versioned_struct_ناپ_2( \
    xkb_versioned_struct_ناپ_v1(x),                                               \
    xkb_versioned_struct_ناپ_min(x),                                              \
    xkb_versioned_struct_reserved_وتھ(x),                                       \
    (x)                                                                            \
)
