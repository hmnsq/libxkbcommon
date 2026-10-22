/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdalign.h>
#समावेश <stddef.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
काष्ठा xkb_keymap_serialize_config_v1 {
    uint32_t आकार;
    uint32_t flags;
    uint32_t format;
    xkb_layout_mask_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(काष्ठा xkb_keymap_serialize_config, आकार, flags);
assert_no_padding(काष्ठा xkb_keymap_serialize_config, flags, format);
assert_no_padding(काष्ठा xkb_keymap_serialize_config, format, layouts);
assert_no_padding(काष्ठा xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(माप(काष्ठा xkb_keymap_serialize_config) ==
              माप(काष्ठा xkb_keymap_serialize_config_v1), "");
assert_same_field(काष्ठा xkb_keymap_serialize_config, _v1, आकार);
assert_same_field(काष्ठा xkb_keymap_serialize_config, _v1, flags);
assert_same_field(काष्ठा xkb_keymap_serialize_config, _v1, format);
assert_same_field(काष्ठा xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(माप(काष्ठा xkb_keymap_serialize_config) * 30 <=
              (माप_प्रकार)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
काष्ठा xkb_keymap_serialize_परिणाम_v1 {
    uint32_t आकार;
    xkb_layout_mask_t layouts;
    अक्षर *serialized;
    माप_प्रकार लंबाई;
};

/* Ensure there is no implicit padding */
assert_no_padding(काष्ठा xkb_keymap_serialize_परिणाम, आकार, layouts);
assert_no_padding(काष्ठा xkb_keymap_serialize_परिणाम, layouts, serialized);
assert_no_padding(काष्ठा xkb_keymap_serialize_परिणाम, serialized, लंबाई);
assert_no_padding(काष्ठा xkb_keymap_serialize_परिणाम, लंबाई);

/* Current version is 1 */
static_assert(माप(काष्ठा xkb_keymap_serialize_परिणाम) ==
              माप(काष्ठा xkb_keymap_serialize_परिणाम_v1), "");
assert_same_field(काष्ठा xkb_keymap_serialize_परिणाम, _v1, आकार);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_field(काष्ठा xkb_keymap_serialize_परिणाम, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_field(काष्ठा xkb_keymap_serialize_परिणाम, _v1, लंबाई);
assert_same_field(काष्ठा xkb_keymap_serialize_परिणाम, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(माप(काष्ठा xkb_keymap_serialize_परिणाम) * 30 <=
              (माप_प्रकार)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
काष्ठा xkb_keymap_कुंजी_iterator_config_v1 {
    uint32_t आकार;
    uint32_t flags;
    xkb_keycode_t start;
};

/* Ensure there is no implicit padding */
assert_no_padding(काष्ठा xkb_keymap_कुंजी_iterator_config, आकार, flags);
assert_no_padding(काष्ठा xkb_keymap_कुंजी_iterator_config, flags, start);
assert_no_padding(काष्ठा xkb_keymap_कुंजी_iterator_config, start);

/* Current version is 1 */
static_assert(माप(काष्ठा xkb_keymap_कुंजी_iterator_config) ==
              माप(काष्ठा xkb_keymap_कुंजी_iterator_config_v1), "");
assert_same_field(काष्ठा xkb_keymap_कुंजी_iterator_config, _v1, आकार);
assert_same_field(काष्ठा xkb_keymap_कुंजी_iterator_config, _v1, flags);
assert_same_field(काष्ठा xkb_keymap_कुंजी_iterator_config, _v1, start);

/* Ensure reasonable margin to the upper size limit */
static_assert(माप(काष्ठा xkb_keymap_कुंजी_iterator_config) * 30 <=
              (माप_प्रकार)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#घोषणा xkb_versioned_struct_आकार_v1(x) _Generic(        \
    (x),                                                 \
    स्थिर काष्ठा xkb_keymap_serialize_config *:          \
        माप(काष्ठा xkb_keymap_serialize_config_v1),   \
    स्थिर काष्ठा xkb_keymap_serialize_परिणाम *:          \
        माप(काष्ठा xkb_keymap_serialize_परिणाम_v1),   \
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *:      \
        माप(काष्ठा xkb_keymap_कुंजी_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#घोषणा xkb_versioned_struct_आकार_min(x) _Generic(       \
    (x),                                                 \
    स्थिर काष्ठा xkb_keymap_serialize_config *:          \
        माप(काष्ठा xkb_keymap_serialize_config_v1),   \
    स्थिर काष्ठा xkb_keymap_serialize_परिणाम *:          \
        माप(काष्ठा xkb_keymap_serialize_परिणाम_v1),   \
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *:      \
        माप(काष्ठा xkb_keymap_कुंजी_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#घोषणा xkb_versioned_struct_reserved_offset(x) _Generic(\
    (x),                                                 \
    स्थिर काष्ठा xkb_keymap_serialize_config *:          \
        माप(काष्ठा xkb_keymap_serialize_config),      \
    स्थिर काष्ठा xkb_keymap_serialize_परिणाम *:          \
        माप(काष्ठा xkb_keymap_serialize_परिणाम),      \
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *:      \
        माप(काष्ठा xkb_keymap_कुंजी_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_आकार_v1(((स्थिर काष्ठा xkb_keymap_serialize_config *)शून्य)) <=
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_serialize_config *)शून्य)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((स्थिर काष्ठा xkb_keymap_serialize_परिणाम *)शून्य)) <=
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_serialize_परिणाम *)शून्य)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *)शून्य)) <=
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *)शून्य)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_serialize_config *)शून्य)) <=
    माप(स्थिर काष्ठा xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_serialize_परिणाम *)शून्य)) <=
    माप(स्थिर काष्ठा xkb_keymap_serialize_परिणाम),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config *)शून्य)) <=
    माप(स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config),
    ""
);

#घोषणा xkb_check_keymap_abi(x) xkb_check_versioned_struct_आकार_2( \
    xkb_versioned_struct_आकार_v1(x),                                               \
    xkb_versioned_struct_आकार_min(x),                                              \
    xkb_versioned_struct_reserved_offset(x),                                       \
    (x)                                                                            \
)
