/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdalign.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_keymap_serialize_config_v1 {
    uint32_t ਮਾਪ;
    uint32_t flags;
    uint32_t format;
    xkb_layout_ਨਕਾਬ_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_config, ਮਾਪ, flags);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_config, flags, format);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_config, format, layouts);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_config, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_config, _v1, flags);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_config, _v1, format);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ_v1 {
    uint32_t ਮਾਪ;
    xkb_layout_ਨਕਾਬ_t layouts;
    ਅੱਖਰ *serialized;
    ਮਾਪ_t ਲੰਮਾਈ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, ਮਾਪ, layouts);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, layouts, serialized);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, serialized, ਲੰਮਾਈ);
assert_no_padding(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, ਲੰਮਾਈ);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, _v1, ਮਾਪ);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, _v1, ਲੰਮਾਈ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config_v1 {
    uint32_t ਮਾਪ;
    uint32_t flags;
    xkb_keycode_t ਸ਼ੁਰੂ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, ਮਾਪ, flags);
assert_no_padding(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, flags, ਸ਼ੁਰੂ);
assert_no_padding(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, ਸ਼ੁਰੂ);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, _v1, flags);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config, _v1, ਸ਼ੁਰੂ);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_ਮਾਪ_v1(x) _Generic(        \
    (x),                                                 \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config_v1),   \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ_v1),   \
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *:      \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_ਮਾਪ_min(x) _Generic(       \
    (x),                                                 \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config_v1),   \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ_v1),   \
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *:      \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_reserved_ਵਿੱਥ(x) _Generic(\
    (x),                                                 \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_config),      \
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *:          \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ),      \
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *:      \
        ਆਕਾਰ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *)ਨਿਰਾ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config),
    ""
);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_check_keymap_abi(x) xkb_check_versioned_struct_ਮਾਪ_2( \
    xkb_versioned_struct_ਮਾਪ_v1(x),                                               \
    xkb_versioned_struct_ਮਾਪ_min(x),                                              \
    xkb_versioned_struct_reserved_ਵਿੱਥ(x),                                       \
    (x)                                                                            \
)
