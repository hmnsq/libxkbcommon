// NOTE: This file has been generated automatically by “update-message-registry.py”.
//       Do not edit manually!
#رهنمود once

#ضمن "config.h"

#ضمن <stdint.h>

/*
 * Macro sorcery: PREPEND_MESSAGE_ID enables the log functions to format messages
 * with the message ID only if the ID is not 0 (XKB_LOG_MESSAGE_NO_ID).
 * This avoid checking the ID value at run time.
 *
 * The trick resides in CHECK_ID:
 * • CHECK_ID(0) expands to:
 *   ‣ SECOND(MATCH0, WITH_ID, unused)
 *   ‣ SECOND(unused,WITHOUT_ID, WITH_ID, unused)
 *   ‣ WITHOUT_ID
 * • CHECK_ID(123) expands to:
 *   ‣ SECOND(MATCH123, WITH_ID, unused)
 *   ‣ WITH_ID
*/
#تعریف_کن EXPAND(...)              __VA_ARGS__ /* needed for MSVC compatibility */

#تعریف_کن JOIN_EXPAND(a, b)        a##b
#تعریف_کن JOIN(a, b)               JOIN_EXPAND(a, b)

#تعریف_کن دومی_EXPAND(a, b, ...) b
#تعریف_کن دومی(...)              EXPAND(دومی_EXPAND(__VA_ARGS__))

#تعریف_کن MATCH0                   استفادهنشده,WITHOUT_ID
#تعریف_کن CHECK_ID(مقدار)          دومی(JOIN(تطابق_2, مقدار), WITH_ID, استفادهنشده)

#تعریف_کن FORMAT_پیام_WITHOUT_ID(id, fmt) fmt
#تعریف_کن FORMAT_پیام_WITH_ID(id, fmt)    "[XKB-%03d] " fmt, id
#تعریف_کن PREPEND_پیام_ID(id, fmt) JOIN(FORMAT_پیام, CHECK_ID(id))(id, fmt)

/**
 * Set of verbosity levels
 */
تعداد xkb_log_verbosity {
    XKB_LOG_VERBOSITY_SILENT = -1,
    XKB_LOG_VERBOSITY_کمینه = 0,
    XKB_LOG_VERBOSITY_BRIEF = 1,
    XKB_LOG_VERBOSITY_DETAILED = 5,
    XKB_LOG_VERBOSITY_VERBOSE = 10,
    XKB_LOG_VERBOSITY_COMPREHENSIVE = 11,
    XKB_LOG_VERBOSITY_DEFAULT = XKB_LOG_VERBOSITY_کمینه,
};

/**
 * Special case when no message identifier is defined.
 */
#تعریف_کن XKB_LOG_پیام_NO_ID 0

/**
 * @name Codes of the log messages
 */
تعداد xkb_پیام_کد {
    XKB_LOG_پیام_MIN_کد = 21,
    /** XKB section is deprecated */
    XKB_WARNING_DEPRECATED_SECTION = 21,
    /** Warn on malformed number literals */
    XKB_ERROR_MALFORMED_عدد_LITERAL = 34,
    /** Conflicting “preserve” entries in a key type */
    XKB_WARNING_CONFLICTING_کلید_TYPE_PRESERVE_ENTRIES = 43,
    /** The result of the operation is not mathematically correct */
    XKB_ERROR_INTEGER_سرریز = 52,
    /** Unsupported modifier mask */
    XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک_2 = 60,
    /** Expected an array entry, but the index is missing */
    XKB_ERROR_EXPECTED_ARRAY_مدخل = 77,
    /** Invalid numeric keysym */
    XKB_ERROR_INVALID_NUMERIC_KEYSYM = 82,
    /** Unsupported `xkb_state_mode` value */
    XKB_ERROR_UNSUPPORTED_STATE_نما_2 = 86,
    /** Illegal keycode alias with the name of a real key */
    XKB_WARNING_ILLEGAL_KEYCODE_ALIAS = 101,
    /** Warn on unrecognized keysyms */
    XKB_WARNING_UNRECOGNIZED_KEYSYM = 107,
    /** Event getter does not match the event’s type */
    XKB_ERROR_EVENT_TYPE_MISMATCH_ = 113,
    /** A virtual modifier is used before being declared */
    XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER = 123,
    /** A buffer has an insufficient size */
    XKB_ERROR_INSUFFICIENT_میانگیر_اندازه = 134,
    /** Unsupported `xkb_keymap_key_iterator_flags` value */
    XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS_2 = 138,
    /** The type of the statement is not allowed in the context */
    XKB_ERROR_WRONG_STATEMENT_TYPE = 150,
    /** The given path is invalid */
    XKB_ERROR_INVALID_مسیر = 161,
    /** Geometry sections are not supported */
    XKB_WARNING_UNSUPPORTED_GEOMETRY_SECTION = 172,
    /** Warn if no key type can be inferred */
    XKB_WARNING_CANNOT_INFER_کلید_TYPE = 183,
    /** Invalid escape sequence in a string */
    XKB_WARNING_INVALID_ESCAPE_SEQUENCE = 193,
    /** The result of a key type “preserve” entry must be a subset of its input modifiers. */
    XKB_WARNING_ILLEGAL_کلید_TYPE_PRESERVE_نتیجه = 195,
    /** Syntax error in the include statement */
    XKB_ERROR_INVALID_INCLUDE_STATEMENT = 203,
    /** A modmap entry is invalid */
    XKB_ERROR_INVALID_MODMAP_مدخل = 206,
    /** The given layout out-of-range policy is not supported */
    XKB_ERROR_UNSUPPORTED_LAYOUT_بیرون_از_RANGE_POLICY_2 = 214,
    /** The statement is unknown */
    XKB_ERROR_UNKNOWN_STATEMENT = 222,
    /** Layout index is not supported */
    XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2 = 237,
    /** The name of a key type level is defined multiple times. */
    XKB_WARNING_CONFLICTING_کلید_TYPE_LEVEL_نامها = 239,
    /** Invalid statement setting default values */
    XKB_ERROR_INVALID_مجموعه_DEFAULT_STATEMENT = 254,
    /** Conflicting “map” entries in type definition */
    XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_مدخل = 266,
    /** Unsupported keymap format */
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_ = 277,
    /** Unrecognized RMLVO value ignored */
    XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار = 282,
    /** Warn if using an undefined key type */
    XKB_WARNING_UNDEFINED_کلید_TYPE = 286,
    /** A keysym has been deprecated: consider using an alternative keysym */
    XKB_WARNING_DEPRECATED_KEYSYM = 301,
    /** A keysym name has been deprecated: use the corresponding canonical name instead */
    XKB_WARNING_DEPRECATED_KEYSYM_NAME = 302,
    /** Warn if a group name was defined for group other than the first one */
    XKB_WARNING_NON_پایه_گروه_NAME = 305,
    /** Warn when a shift level is not supported */
    XKB_ERROR_UNSUPPORTED_لغزش_LEVEL = 312,
    /** Could not find a file used in an include statement */
    XKB_ERROR_INCLUDED_FILE_NOT_یافت = 338,
    /** Use of an operator that is unknown and thus unsupported */
    XKB_ERROR_UNKNOWN_OPERATOR = 345,
    /** Overlapping overlays are not supported */
    XKB_ERROR_OVERLAPPING_OVERLAY = 355,
    /** Use of a legacy X11 action that is not supported */
    XKB_WARNING_UNSUPPORTED_LEGACY_ACTION = 362,
    /** Layout count limit exceeded for the corresponding keymap format */
    XKB_ERROR_LAYOUT_شمار_حد_EXCEEDED_2 = 366,
    /** Unsupported `xkb_a11y_flags` value */
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_ = 371,
    /** An entry is duplicated and will be ignored */
    XKB_WARNING_DUPLICATE_مدخل = 378,
    /** Included files form cycle */
    XKB_ERROR_RECURSIVE_INCLUDE = 386,
    /** A `xkb_state` object has an unexpected `xkb_state_mode` */
    XKB_ERROR_UNEXPECTED_STATE_نما_2 = 399,
    /** Conflicting definitions of a key type */
    XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS = 407,
    /** A global defaults statement is in a wrong scope and should be moved */
    XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE = 428,
    /** Missing default section in included file */
    XKB_WARNING_MISSING_DEFAULT_SECTION = 433,
    /** ABI struct size check failed */
    XKB_ERROR_ABI_INVALID_STRUCT_اندازه_2 = 450,
    /** Invalid real modifier mask in modifier map definition */
    XKB_ERROR_INVALID_MODIFIER_MAP_ماسک = 453,
    /** Warn if there are conflicting keysyms while merging keys */
    XKB_WARNING_CONFLICTING_کلید_نماد = 461,
    /** The operation is invalid in the context */
    XKB_ERROR_INVALID_OPERATION = 478,
    /** Warn on numeric keysym (other than 0-9) */
    XKB_WARNING_NUMERIC_KEYSYM = 489,
    /** TODO: add description */
    XKB_WARNING_EXTRA_SYMBOLS_IGNORED = 516,
    /** Conflicting definitions of a key name or alias */
    XKB_WARNING_CONFLICTING_کلید_NAME = 523,
    /** Invalid keycode */
    XKB_ERROR_INVALID_KEYCODE_ = 538,
    /** Invalid file encoding */
    XKB_ERROR_INVALID_FILE_ENCODING = 542,
    /** Cannot allocate memory */
    XKB_ERROR_تخصیص_FAILURE_2 = 550,
    /** Unknown or unsupported action field */
    XKB_ERROR_INVALID_ACTION_فیلد = 563,
    /** Warn when a field has not the expected type */
    XKB_ERROR_WRONG_فیلد_TYPE = 578,
    /** Overlay index is not in the supported range */
    XKB_ERROR_UNSUPPORTED_OVERLAY_شاخص = 588,
    /** Cannot resolve a given (Rules, Model, Layout, Variant, Options) configuration */
    XKB_ERROR_CANNOT_RESOLVE_RMLVO = 595,
    /** Invalid Unicode escape sequence */
    XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE = 607,
    /** Unsupported `xkb_events_flags` value */
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_ = 611,
    /** Invalid _real_ modifier */
    XKB_ERROR_INVALID_REAL_MODIFIER = 623,
    /** Unable to add any default include path */
    XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_مسیر = 632,
    /** Cannot set default value to a an unknown field */
    XKB_ERROR_UNKNOWN_DEFAULT_فیلد = 639,
    /** Warn on unknown escape sequence in string literal */
    XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE = 645,
    /** The target file of an include statement could not be processed */
    XKB_ERROR_INVALID_INCLUDED_FILE = 661,
    /** Invalid locale for Compose */
    XKB_ERROR_INVALID_COMPOSE_LOCALE = 679,
    /** The Compose file syntax is invalid and the entry cannot be parsed */
    XKB_ERROR_INVALID_COMPOSE_SYNTAX = 685,
    /** A level has a different number of keysyms and actions */
    XKB_ERROR_INCOMPATIBLE_ACTIONS_AND_KEYSYMS_شمار = 693,
    /** Warn if a key defines multiple groups at once */
    XKB_WARNING_مضرب_GROUPS_AT_ONCE = 700,
    /** A legacy X11 symbol field is not supported */
    XKB_WARNING_UNSUPPORTED_SYMBOLS_فیلد = 711,
    /** The keymap has features unsupported in the target format */
    XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT = 742,
    /** Unsupported `xkb_machine_flags` value */
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_ = 755,
    /** Cannot expand `%%i`: missing layout or variant in MLVO rule fields */
    XKB_ERROR_قواعد_INVALID_LAYOUT_شاخص_PERCENT_EXPANSION = 762,
    /** The XKB syntax is invalid and the file cannot be parsed */
    XKB_ERROR_INVALID_XKB_SYNTAX = 769,
    /** Reference to an undefined keycode */
    XKB_WARNING_UNDEFINED_KEYCODE = 770,
    /** An expression has not the expected type */
    XKB_ERROR_INVALID_عبارت_TYPE = 784,
    /** A value is invalid and will be ignored */
    XKB_ERROR_INVALID_مقدار = 796,
    /** Warn if there are conflicting modmap definitions */
    XKB_WARNING_CONFLICTING_MODMAP = 800,
    /** A field is unknown and will be ignored */
    XKB_ERROR_UNKNOWN_فیلد = 812,
    /** Keymap compilation failed */
    XKB_ERROR_KEYMAP_COMPILATION_FAILED = 822,
    /** Unsupported keymap serialization flags */
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_ = 829,
    /** Unknown action type */
    XKB_ERROR_UNKNOWN_ACTION_TYPE = 844,
    /** ABI forward-compatibility check failed */
    XKB_ERROR_ABI_FORWARD_COMPAT_ = 876,
    /** Warn if there are conflicting actions while merging keys */
    XKB_WARNING_CONFLICTING_کلید_ACTION = 883,
    /** Warn if there are conflicting key types while merging groups */
    XKB_WARNING_CONFLICTING_کلید_TYPE_MERGING_GROUPS = 893,
    /** Unsupported `xkb_machine_builder_flags` value */
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_ = 899,
    /** Conflicting symbols entry for a key */
    XKB_ERROR_CONFLICTING_کلید_SYMBOLS_مدخل = 901,
    /** Missing group index in a group name entry */
    XKB_WARNING_MISSING_SYMBOLS_گروه_NAME_شاخص = 903,
    /** ABI backward-compatibility check failed */
    XKB_ERROR_ABI_BACKWARD_COMPAT_ = 914,
    /** Invalid section flag */
    XKB_ERROR_UNKNOWN_SECTION_FLAG = 917,
    /** Warn if there are conflicting fields in the key definition or while merging keys */
    XKB_WARNING_CONFLICTING_کلید_FIELDS = 935,
    /** An identifier is used but is not built-in */
    XKB_ERROR_INVALID_IDENTIFIER = 949,
    /** Warn if using a symbol not defined in the keymap */
    XKB_WARNING_UNRESOLVED_KEYMAP_نماد = 965,
    /** The rules syntax is invalid and the file cannot be parsed */
    XKB_ERROR_INVALID_قواعد_SYNTAX = 967,
    /** Some modifiers used in a key type “map” or “preserve” entry are not declared */
    XKB_WARNING_UNDECLARED_MODIFIERS_درون_کلید_TYPE = 971,
    XKB_LOG_پیام_MAX_کد = 971
};
