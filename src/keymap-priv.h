/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdalign.h>
#అన్తర్భావ <stddef.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "abi-check.h"

/******************************************************************************
 * xkb_keymap_serialize_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_config`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_keymap_serialize_config_v1 {
    uint32_t కొలత;
    uint32_t flags;
    uint32_t format;
    xkb_layout_ముసుగు_t layouts;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_keymap_serialize_config, కొలత, flags);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_config, flags, format);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_config, format, layouts);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_config, layouts);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_serialize_config) ==
              పరిమాణం(నిర్మాణం xkb_keymap_serialize_config_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_config, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_config, _v1, flags);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_config, _v1, format);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_config, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_serialize_config) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_keymap_serialize_result
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_serialize_result`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_keymap_serialize_ఫలితం_v1 {
    uint32_t కొలత;
    xkb_layout_ముసుగు_t layouts;
    అక్షరం *serialized;
    కొలత_t నిడివి;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_keymap_serialize_ఫలితం, కొలత, layouts);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_ఫలితం, layouts, serialized);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_ఫలితం, serialized, నిడివి);
assert_no_padding(నిర్మాణం xkb_keymap_serialize_ఫలితం, నిడివి);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం) ==
              పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_ఫలితం, _v1, కొలత);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_ఫలితం, _v1, serialized);
// NOLINTEND(bugprone-sizeof-expression)
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_ఫలితం, _v1, నిడివి);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_serialize_ఫలితం, _v1, layouts);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_keymap_key_iterator_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_keymap_key_iterator_config`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_keymap_కీలకం_iterator_config_v1 {
    uint32_t కొలత;
    uint32_t flags;
    xkb_keycode_t ప్రారంభం;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_keymap_కీలకం_iterator_config, కొలత, flags);
assert_no_padding(నిర్మాణం xkb_keymap_కీలకం_iterator_config, flags, ప్రారంభం);
assert_no_padding(నిర్మాణం xkb_keymap_కీలకం_iterator_config, ప్రారంభం);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config) ==
              పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_కీలకం_iterator_config, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_కీలకం_iterator_config, _v1, flags);
assert_same_క్షేత్రం(నిర్మాణం xkb_keymap_కీలకం_iterator_config, _v1, ప్రారంభం);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#నిర్వచించు xkb_versioned_struct_కొలత_v1(x) _Generic(        \
    (x),                                                 \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_config_v1),   \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం_v1),   \
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *:      \
        పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config_v1)\
)

/** Minimal *current* valid size of the struct */
#నిర్వచించు xkb_versioned_struct_కొలత_min(x) _Generic(       \
    (x),                                                 \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_config_v1),   \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం_v1),   \
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *:      \
        పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config_v1)\
)

/** Offset of the last meaningful field of the current struct */
#నిర్వచించు xkb_versioned_struct_reserved_స్థానభ్రంశం(x) _Generic(\
    (x),                                                 \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_config),      \
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *:          \
        పరిమాణం(నిర్మాణం xkb_keymap_serialize_ఫలితం),      \
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *:      \
        పరిమాణం(నిర్మాణం xkb_keymap_కీలకం_iterator_config)   \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *)ఖాళీ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config),
    ""
);

#నిర్వచించు xkb_check_keymap_abi(x) xkb_check_versioned_struct_కొలత_2( \
    xkb_versioned_struct_కొలత_v1(x),                                               \
    xkb_versioned_struct_కొలత_min(x),                                              \
    xkb_versioned_struct_reserved_స్థానభ్రంశం(x),                                       \
    (x)                                                                            \
)
