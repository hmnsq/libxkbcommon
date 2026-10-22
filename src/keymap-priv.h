/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdalign.h>
#अन्तर्भाव <stddef.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_keymap_serialize_config_v1 {
    uint32_t आकार;
    uint32_t flags;
    uint32_t format;
    xkb_layout_आवरणम्_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_keymap_serialize_config, आकार, flags);
assert_no_padding(संरचना xkb_keymap_serialize_config, flags, format);
assert_no_padding(संरचना xkb_keymap_serialize_config, format, layouts);
assert_no_padding(संरचना xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_keymap_serialize_config) ==
              परिमाण(संरचना xkb_keymap_serialize_config_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_config, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_config, _v1, flags);
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_config, _v1, format);
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_keymap_serialize_config) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_keymap_serialize_परिणाम_v1 {
    uint32_t आकार;
    xkb_layout_आवरणम्_t layouts;
    वर्ण *serialized;
    आकार_t दीर्घता;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_keymap_serialize_परिणाम, आकार, layouts);
assert_no_padding(संरचना xkb_keymap_serialize_परिणाम, layouts, serialized);
assert_no_padding(संरचना xkb_keymap_serialize_परिणाम, serialized, दीर्घता);
assert_no_padding(संरचना xkb_keymap_serialize_परिणाम, दीर्घता);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_keymap_serialize_परिणाम) ==
              परिमाण(संरचना xkb_keymap_serialize_परिणाम_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_परिणाम, _v1, आकार);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_परिणाम, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_परिणाम, _v1, दीर्घता);
assert_same_क्षेत्रम्(संरचना xkb_keymap_serialize_परिणाम, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_keymap_serialize_परिणाम) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_keymap_कुञ्चिका_iterator_config_v1 {
    uint32_t आकार;
    uint32_t flags;
    xkb_keycode_t start;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_keymap_कुञ्चिका_iterator_config, आकार, flags);
assert_no_padding(संरचना xkb_keymap_कुञ्चिका_iterator_config, flags, start);
assert_no_padding(संरचना xkb_keymap_कुञ्चिका_iterator_config, start);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config) ==
              परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_keymap_कुञ्चिका_iterator_config, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_keymap_कुञ्चिका_iterator_config, _v1, flags);
assert_same_क्षेत्रम्(संरचना xkb_keymap_कुञ्चिका_iterator_config, _v1, start);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#परिभाषय xkb_versioned_struct_आकार_v1(x) _Generic(        \
    (x),                                                 \
    ध्रुव संरचना xkb_keymap_serialize_config *:          \
        परिमाण(संरचना xkb_keymap_serialize_config_v1),   \
    ध्रुव संरचना xkb_keymap_serialize_परिणाम *:          \
        परिमाण(संरचना xkb_keymap_serialize_परिणाम_v1),   \
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *:      \
        परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#परिभाषय xkb_versioned_struct_आकार_min(x) _Generic(       \
    (x),                                                 \
    ध्रुव संरचना xkb_keymap_serialize_config *:          \
        परिमाण(संरचना xkb_keymap_serialize_config_v1),   \
    ध्रुव संरचना xkb_keymap_serialize_परिणाम *:          \
        परिमाण(संरचना xkb_keymap_serialize_परिणाम_v1),   \
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *:      \
        परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#परिभाषय xkb_versioned_struct_reserved_विस्थापनम्(x) _Generic(\
    (x),                                                 \
    ध्रुव संरचना xkb_keymap_serialize_config *:          \
        परिमाण(संरचना xkb_keymap_serialize_config),      \
    ध्रुव संरचना xkb_keymap_serialize_परिणाम *:          \
        परिमाण(संरचना xkb_keymap_serialize_परिणाम),      \
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *:      \
        परिमाण(संरचना xkb_keymap_कुञ्चिका_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_keymap_serialize_config *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_serialize_config *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_keymap_serialize_परिणाम *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_serialize_परिणाम *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *)रिक्त)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_serialize_config *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_serialize_परिणाम *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_keymap_serialize_परिणाम),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config),
    ""
);

#परिभाषय xkb_check_keymap_abi(x) xkb_check_versioned_struct_आकार_2( \
    xkb_versioned_struct_आकार_v1(x),                                               \
    xkb_versioned_struct_आकार_min(x),                                              \
    xkb_versioned_struct_reserved_विस्थापनम्(x),                                       \
    (x)                                                                            \
)
