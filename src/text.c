/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "text.h"

bool
LookupString(స్థిరరాశి Lookup_నమోదు tab[], స్థిరరాశి అక్షరం *string,
              గుర్తులేని పూర్ణాంకం *విలువ_rtrn)
{
    ఒకవేళ (!string)
        తిరిగి false;

    కొరకు (స్థిరరాశి Lookup_నమోదు *నమోదు_2 = tab; నమోదు_2->name; నమోదు_2++) {
        ఒకవేళ (istreq(నమోదు_2->name, string)) {
            *విలువ_rtrn = నమోదు_2->విలువ;
            తిరిగి true;
        }
    }

    తిరిగి false;
}

స్థిరరాశి అక్షరం *
Lookup_విలువ(స్థిరరాశి Lookup_నమోదు tab[], గుర్తులేని పూర్ణాంకం విలువ)
{
    కొరకు (స్థిరరాశి Lookup_నమోదు *నమోదు_2 = tab; నమోదు_2->name; నమోదు_2++)
        ఒకవేళ (నమోదు_2->విలువ == విలువ)
            తిరిగి నమోదు_2->name;

    తిరిగి ఖాళీ;
}

స్థిరరాశి Lookup_నమోదు ctrl_ముసుగు_పేర్లు[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_పేర్లు_MIN_V1_సూచీ] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_కీలకాలు },
    { "MouseKeys", CONTROL_MOUSE_కీలకాలు },
    { "MouseKeysAccel", CONTROL_MOUSE_కీలకాలు_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_సమూహం_తాళం },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు mod_Component_ముసుగు_పేర్లు[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు సమూహం_Component_ముసుగు_పేర్లు[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు సమూహం_ముసుగు_పేర్లు[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు button_పేర్లు[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు use_Mod_Map_విలువ_పేర్లు[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ఖాళీ, 0 }
};

స్థిరరాశి Lookup_నమోదు action_Type_పేర్లు[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_సమితి },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_తాళం },
    { "SetGroup", ACTION_TYPE_సమూహం_సమితి },
    { "LatchGroup", ACTION_TYPE_సమూహం_LATCH },
    { "LockGroup", ACTION_TYPE_సమూహం_తాళం },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_తాళం },
    { "LockPtrButton", ACTION_TYPE_PTR_తాళం },
    { "LockPointerButton", ACTION_TYPE_PTR_తాళం },
    { "LockPointerBtn", ACTION_TYPE_PTR_తాళం },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_సమితి },
    { "LockControls", ACTION_TYPE_CTRL_తాళం },
    { "RedirectKey", ACTION_TYPE_REDIRECT_కీలకం },
    { "Redirect", ACTION_TYPE_REDIRECT_కీలకం },
    { "Private", ACTION_TYPE_వ్యక్తిగత },
    /* deprecated actions below here - unused */
    { "ISOLock", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "ActionMessage", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "MessageAction", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "Message", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDeviceBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDevBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDevButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDeviceButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceValuator", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevVal", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceVal", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevValuator", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { ఖాళీ, 0 },
};

స్థిరరాశి Lookup_నమోదు sym_Interpret_సరిపోలిక_ముసుగు_పేర్లు[] = {
    { "NoneOf", సరిపోలిక_NONE },
    { "AnyOfOrNone", సరిపోలిక_ANY_OR_NONE },
    { "AnyOf", సరిపోలిక_ANY },
    { "AllOf", సరిపోలిక_ALL },
    { "Exactly", సరిపోలిక_EXACTLY },
    { ఖాళీ, 0 },
};

స్థిరరాశి అక్షరం *
Mod_సూచీ_పాఠ్యం(నిర్మాణం xkb_context *ctx, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
             xkb_mod_సూచీ_t ndx)
{
    ఒకవేళ (ndx == XKB_MOD_INVALID)
        తిరిగి "none";

    ఒకవేళ (ndx == XKB_MOD_NONE)
        తిరిగి "None";

    ఒకవేళ (ndx >= mods->అంకె_mods)
        తిరిగి ఖాళీ;

    తిరిగి xkb_atom_పాఠ్యం(ctx, mods->mods[ndx].name);
}

స్థిరరాశి అక్షరం *
Action_Type_పాఠ్యం(గణన xkb_action_type type)
{
    స్థిరరాశి అక్షరం *name = Lookup_విలువ(action_Type_పేర్లు, type);
    తిరిగి name ? name : "Private";
}

స్థిరరాశి అక్షరం *
Keysym_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_keysym_t sym)
{
    అక్షరం *బఫరు = xkb_context_get_బఫరు(ctx, XKB_KEYSYM_NAME_MAX_కొలత);
    xkb_keysym_get_name(sym, బఫరు, XKB_KEYSYM_NAME_MAX_కొలత);
    తిరిగి బఫరు;
}

స్థిరరాశి అక్షరం *
కీలకం_Name_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_atom_t name)
{
    స్థిరరాశి అక్షరం *sname = xkb_atom_పాఠ్యం(ctx, name);
    కొలత_t len = strlen_సురక్షిత(sname) + 3;
    అక్షరం *buf = xkb_context_get_బఫరు(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    తిరిగి buf;
}

స్థిరరాశి అక్షరం *
SIMatch_పాఠ్యం(గణన xkb_సరిపోలిక_operation type)
{
    తిరిగి Lookup_విలువ(sym_Interpret_సరిపోలిక_ముసుగు_పేర్లు, type);
}

స్థిరరాశి అక్షరం *
Mod_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, గణన mod_type type,
            స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, xkb_mod_ముసుగు_t ముసుగు)
{
    అక్షరం buf[1024] = {0};
    కొలత_t pos = 0;
    స్థిరరాశి నిర్మాణం xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ప్రతిజ్ఞా(type == MOD_REAL || type == MOD_BOTH);

    ఒకవేళ (ముసుగు == 0)
        తిరిగి "none";

    ఒకవేళ (ముసుగు == MOD_REAL_ముసుగు_ALL)
        తిరిగి "all";

    ఒకవేళ ((type == MOD_REAL && (ముసుగు & ~MOD_REAL_ముసుగు_ALL)) ||
        unlikely(ముసుగు & ~((UINT64_C(1) << mods->అంకె_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        స్థిరరాశి పూర్ణాంకం ret = snprintf(buf, పరిమాణం(buf), "0x%"PRIx32, ముసుగు);
        static_assert(పరిమాణం(ముసుగు) == 4 && పరిమాణం(buf) >= పరిమాణం("0xffffffff"),
                      "Buffer too small");
        ప్రతిజ్ఞా(ret >= 0 && (కొలత_t) ret < పరిమాణం(buf));
        pos = (కొలత_t) ret;
    } లేకపోతే {
        /* Print known mods */
        xkb_mods_ముసుగు_foreach(ముసుగు, mod, mods) {
            పూర్ణాంకం ret = snprintf(buf + pos, పరిమాణం(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_పాఠ్యం(ctx, mod->name));
            ఒకవేళ (ret <= 0 || pos + (కొలత_t)ret >= పరిమాణం(buf))
                ఆపు;
            లేకపోతే
                pos += (కొలత_t)ret;
        }
    }
    pos++;

    తిరిగి జ్ఞాపకం_నకలు(xkb_context_get_బఫరు(ctx, pos), buf, pos);
}

స్థిరరాశి అక్షరం *
Led_State_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, స్థిరరాశి Lookup_నమోదు *lookup,
                 గణన xkb_state_component ముసుగు)
{
    అక్షరం buf[1024];
    కొలత_t pos = 0;

    ఒకవేళ (ముసుగు == 0)
        తిరిగి "0";

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; ముసుగు; i++) {
        పూర్ణాంకం ret;

        ఒకవేళ (!(ముసుగు & (1u << i)))
            కొనసాగు;

        ముసుగు &= ~(1u << i);

        స్థిరరాశి అక్షరం* స్థిరరాశి ముసుగు_పాఠ్యం = Lookup_విలువ(lookup, 1u << i);
        ప్రతిజ్ఞా(ముసుగు_పాఠ్యం != ఖాళీ);
        ret = snprintf(buf + pos, పరిమాణం(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ముసుగు_పాఠ్యం);
        ఒకవేళ (ret <= 0 || pos + (కొలత_t)ret >= పరిమాణం(buf))
            ఆపు;
        లేకపోతే
            pos += (కొలత_t)ret;
    }
    pos++;

    తిరిగి జ్ఞాపకం_నకలు(xkb_context_get_బఫరు(ctx, pos), buf, pos);
}

స్థిరరాశి అక్షరం *
Control_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, గణన xkb_keymap_format format,
                గణన xkb_action_controls ముసుగు)
{
    అక్షరం buf[1024];
    కొలత_t pos = 0;

    స్థిరరాశి గణన xkb_action_controls all_ctrls = format_boolean_controls(format);
    ముసుగు &= all_ctrls;

    ఒకవేళ (ముసుగు == 0)
        తిరిగి "none";

    ఒకవేళ (ముసుగు == all_ctrls)
        తిరిగి "all";

    స్థిరరాశి uint8_t control_పేర్లు_స్థానభ్రంశం = format_control_పేర్లు_స్థానభ్రంశం(format);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; ముసుగు; i++) {
        పూర్ణాంకం ret;

        ఒకవేళ (!(ముసుగు & (1u << i)))
            కొనసాగు;

        ముసుగు &= ~(1u << i);

        స్థిరరాశి అక్షరం* స్థిరరాశి ముసుగు_పాఠ్యం =
            Lookup_విలువ(ctrl_ముసుగు_పేర్లు + control_పేర్లు_స్థానభ్రంశం, 1u << i);
        ప్రతిజ్ఞా(ముసుగు_పాఠ్యం);
        ret = snprintf(buf + pos, పరిమాణం(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ముసుగు_పాఠ్యం);
        ఒకవేళ (ret <= 0 || pos + (కొలత_t)ret >= పరిమాణం(buf))
            ఆపు;
        లేకపోతే
            pos += (కొలత_t)ret;
    }
    pos++;

    తిరిగి జ్ఞాపకం_నకలు(xkb_context_get_బఫరు(ctx, pos), buf, pos);
}
