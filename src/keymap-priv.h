/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdalign.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_keymap_serialize_config_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t flags;
    uint32_t format;
    xkb_layout_ಮುಖವಾಡ_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_keymap_serialize_config, ಗಾತ್ರ_2, flags);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_config, flags, format);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_config, format, layouts);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config) ==
              ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_config, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_config, _v1, flags);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_config, _v1, format);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ_v1 {
    uint32_t ಗಾತ್ರ_2;
    xkb_layout_ಮುಖವಾಡ_t layouts;
    ಅಕ್ಷರ *serialized;
    ಗಾತ್ರ_t ಉದ್ದಳತೆ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, ಗಾತ್ರ_2, layouts);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, layouts, serialized);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, serialized, ಉದ್ದಳತೆ);
assert_no_padding(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, ಉದ್ದಳತೆ);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ) ==
              ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, _v1, ಗಾತ್ರ_2);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, _v1, ಉದ್ದಳತೆ);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t flags;
    xkb_keycode_t ಆರಂಭ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, ಗಾತ್ರ_2, flags);
assert_no_padding(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, flags, ಆರಂಭ);
assert_no_padding(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, ಆರಂಭ);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config) ==
              ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, _v1, flags);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config, _v1, ಆರಂಭ);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_ಗಾತ್ರ_v1(x) _Generic(        \
    (x),                                                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config_v1),   \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ_v1),   \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *:      \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_ಗಾತ್ರ_min(x) _Generic(       \
    (x),                                                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config_v1),   \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ_v1),   \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *:      \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_reserved_ಸ್ಥಾನಾಂತರ(x) _Generic(\
    (x),                                                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_config),      \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *:          \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ),      \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *:      \
        ಗಾತ್ರ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *)ಖಾಲಿ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config),
    ""
);

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_check_keymap_abi(x) xkb_check_versioned_struct_ಗಾತ್ರ_2( \
    xkb_versioned_struct_ಗಾತ್ರ_v1(x),                                               \
    xkb_versioned_struct_ಗಾತ್ರ_min(x),                                              \
    xkb_versioned_struct_reserved_ಸ್ಥಾನಾಂತರ(x),                                       \
    (x)                                                                            \
)
