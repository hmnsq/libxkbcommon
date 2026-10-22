/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "action.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "vmod.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"

ଗଣନା si_field {
    SI_FIELD_VIRTUAL_MOD = (1 << 0),
    SI_FIELD_ACTION = (1 << 1),
    SI_FIELD_AUTO_REPEAT = (1 << 2),
    SI_FIELD_LEVEL_ONE_ONLY = (1 << 3),
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଗଣନା si_field defined;
    ଗଣନା merge_mode merge;

    ସଂରଚନା xkb_sym_interpret interp;
} SymInterpInfo;

ଗଣନା led_field {
    LED_FIELD_MODS = (1 << 0),
    LED_FIELD_GROUPS = (1 << 1),
    LED_FIELD_CTRLS = (1 << 2),
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଗଣନା led_field defined;
    ଗଣନା merge_mode merge;

    ସଂରଚନା xkb_led led;
} LedInfo;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth;
    SymInterpInfo default_interp;
    darray(SymInterpInfo) interps;
    LedInfo default_led;
    LedInfo leds[XKB_MAX_LEDS];
    xkb_led_index_t ଅଙ୍କ_leds;
    ActionsInfo default_actions;
    ସଂରଚନା xkb_mod_set mods;

    ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info;
    ସଂରଚନା xkb_context *ctx;
} CompatInfo;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
siText(SymInterpInfo *si, CompatInfo *info)
{
    ଅକ୍ଷର *buf = xkb_context_get_buffer(info->ctx, 128);

    ଯଦି (si == &info->default_interp)
        ଫେରନ୍ତୁ "default";

    snprintf(buf, 128, "%s+%s(%s)",
             KeysymText(info->ctx, si->interp.sym),
             SIMatchText(si->interp.match),
             ModMaskText(info->ctx, MOD_BOTH, &info->mods, si->interp.mods));

    ଫେରନ୍ତୁ buf;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportSINotArray(CompatInfo *info, SymInterpInfo *si, ଅଚଳ ଅକ୍ଷର *field)
{
    ଫେରନ୍ତୁ ReportNotArray(info->ctx, "symbol interpretation", field,
                          siText(si, info));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportSIBadType(CompatInfo *info, SymInterpInfo *si, ଅଚଳ ଅକ୍ଷର *field,
                ଅଚଳ ଅକ୍ଷର *wanted)
{
    ଫେରନ୍ତୁ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "symbol interpretation", field,
                         siText(si, info), wanted);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*
LEDText(CompatInfo *info, LedInfo *ledi)
{
    ଯଦି (ledi == &info->default_led) {
        ପ୍ରତିଜ୍ଞା(xkb_atom_text(info->ctx, ledi->led.name) == ଖାଲି);
        ଫେରନ୍ତୁ "default";
    } ଅନ୍ୟଥା {
        ପ୍ରତିଜ୍ଞା(xkb_atom_text(info->ctx, ledi->led.name) != ଖାଲି);
        ଫେରନ୍ତୁ xkb_atom_text(info->ctx, ledi->led.name);
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportLedBadType(CompatInfo *info, LedInfo *ledi, ଅଚଳ ଅକ୍ଷର *field,
                 ଅଚଳ ଅକ୍ଷର *wanted)
{
    ଫେରନ୍ତୁ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                         "indicator map",
                         field, LEDText(info, ledi), wanted);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportLedNotArray(CompatInfo *info, LedInfo *ledi, ଅଚଳ ଅକ୍ଷର *field)
{
    ଫେରନ୍ତୁ ReportNotArray(info->ctx, "indicator map", field,
                          LEDText(info, ledi));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
InitInterp(SymInterpInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
    info->interp.virtual_mod = XKB_MOD_INVALID;
    static_assert(!DEFAULT_ଚାବି_VMODMAP, "invalid initial value");
    static_assert(!DEFAULT_ଚାବି_REPEAT, "invalid initial value");
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
InitLED(LedInfo *info)
{
    info->merge = MERGE_DEFAULT; /* Unused */
}

ସ୍ଥିର ଶୂନ୍ୟ
InitCompatInfo(CompatInfo *info, ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
               unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth, ଅଚଳ ସଂରଚନା xkb_mod_set *mods)
{
    memset(info, 0, ଆକାର(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    InitInterp(&info->default_interp);
    InitLED(&info->default_led);
}

ସ୍ଥିର ଶୂନ୍ୟ
ClearCompatInfo(CompatInfo *info)
{
    ମୁକ୍ତ(info->name);
    darray_free(info->interps);
}

ସ୍ଥିର SymInterpInfo *
FindMatchingInterp(CompatInfo *info, SymInterpInfo *new)
{
    SymInterpInfo *old;

    darray_foreach(old, info->interps)
        ଯଦି (old->interp.sym == new->interp.sym &&
            old->interp.mods == new->interp.mods &&
            old->interp.match == new->interp.match)
            ଫେରନ୍ତୁ old;

    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର bool
UseNewInterpField(ଗଣନା si_field field, ଗଣନା si_field old, ଗଣନା si_field new,
                  bool clobber, bool report, ଗଣନା si_field *collide)
{
    ଯଦି (!(old & field))
        ଫେରନ୍ତୁ (new & field);

    ଯଦି (new & field) {
        ଯଦି (report)
            *collide |= field;

        ଫେରନ୍ତୁ clobber;
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
MergeInterp(CompatInfo *info, SymInterpInfo *old, SymInterpInfo *new,
            bool same_file)
{
    ଅଚଳ bool clobber = (new->merge != MERGE_AUGMENT);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = xkb_context_get_log_verbosity(info->ctx);
    ଅଚଳ bool report = (same_file && verbosity > 0) || verbosity > 9;
    ଗଣନା si_field collide = 0;

    ଯଦି (new->merge == MERGE_REPLACE) {
        ଯଦି (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions for \"%s\"; "
                     "Earlier interpretation ignored\n",
                     siText(new, info));
        *old = *new;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.virtual_mod = new->interp.virtual_mod;
        old->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ଯଦି (UseNewInterpField(SI_FIELD_ACTION, old->defined, new->defined,
                          clobber, report, &collide)) {
        ଯଦି (old->interp.ଅଙ୍କ_actions > 1) {
                ମୁକ୍ତ(old->interp.a.actions);
            }
            old->interp.ଅଙ୍କ_actions = new->interp.ଅଙ୍କ_actions;
            ଯଦି (new->interp.ଅଙ୍କ_actions > 1) {
                old->interp.a.actions = new->interp.a.actions;
                new->interp.a.action =
                    (ସଂଘ xkb_action) { .type = ACTION_TYPE_NONE };
                new->interp.ଅଙ୍କ_actions = 0;
            } ଅନ୍ୟଥା {
                old->interp.a.action = new->interp.a.action;
            }
        old->defined |= SI_FIELD_ACTION;
    }
    ଯଦି (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.repeat = new->interp.repeat;
        old->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ଯଦି (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old->defined, new->defined,
                          clobber, report, &collide)) {
        old->interp.level_one_only = new->interp.level_one_only;
        old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }

    ଯଦି (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple interpretations of \"%s\"; "
                 "Using %s definition for duplicate fields\n",
                 siText(old, info),
                 (clobber ? "last" : "first"));
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    ଯଦି (old)
        ଫେରନ୍ତୁ MergeInterp(info, old, new, same_file);

    darray_append(info->interps, *new);
    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

// TODO: more lenient?
ସ୍ଥିର bool
ResolveStateAndPredicate(ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, ଗଣନା xkb_match_operation *pred_rtrn,
                         xkb_mod_mask_t *mods_rtrn, CompatInfo *info)
{
    ଯଦି (ପଦସମଷ୍ଟି == ଖାଲି) {
        *pred_rtrn = MATCH_ANY_OR_NONE;
        *mods_rtrn = MOD_REAL_MASK_ALL;
        ଫେରନ୍ତୁ true;
    }

    *pred_rtrn = MATCH_EXACTLY;
    ଯଦି (ପଦସମଷ୍ଟି->common.type == STMT_ପଦସମଷ୍ଟି_ACTION_DECL) {
        ଅଚଳ ଅକ୍ଷର *pred_txt = xkb_atom_text(info->ctx, ପଦସମଷ୍ଟି->action.name);
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pred = 0;
        ଯଦି (!LookupString(symInterpretMatchMaskNames, pred_txt, &pred) ||
            !ପଦସମଷ୍ଟି->action.args || ପଦସମଷ୍ଟି->action.args->common.next) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Illegal modifier predicate \"%s\"; Ignored\n", pred_txt);
            ଫେରନ୍ତୁ false;
        }
        *pred_rtrn = (ଗଣନା xkb_match_operation) pred;
        ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି->action.args;
    }
    ଅନ୍ୟଥା ଯଦି (ପଦସମଷ୍ଟି->common.type == STMT_ପଦସମଷ୍ଟି_IDENT) {
        ଅଚଳ ଅକ୍ଷର *pred_txt = xkb_atom_text(info->ctx, ପଦସମଷ୍ଟି->ident.ident);
        ଯଦି (pred_txt && istreq(pred_txt, "any")) {
            *pred_rtrn = MATCH_ANY;
            *mods_rtrn = MOD_REAL_MASK_ALL;
            ଫେରନ୍ତୁ true;
        }
    }

    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, ପଦସମଷ୍ଟି, MOD_REAL, &info->mods,
                              mods_rtrn);
}

/***====================================================================***/

ସ୍ଥିର bool
UseNewLEDField(ଗଣନା led_field field, ଗଣନା led_field old, ଗଣନା led_field new,
               bool clobber, bool report, ଗଣନା led_field *collide)
{
    ଯଦି (!(old & field))
        ଫେରନ୍ତୁ (new & field);

    ଯଦି (new & field) {
        ଯଦି (report)
            *collide |= field;

        ଫେରନ୍ତୁ clobber;
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
MergeLedMap(CompatInfo *info, LedInfo *old, LedInfo *new, bool same_file)
{
    ଗଣନା led_field collide;
    ଅଚଳ bool clobber = (new->merge != MERGE_AUGMENT);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = xkb_context_get_log_verbosity(info->ctx);
    ଅଚଳ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ଯଦି (old->led.mods.mods == new->led.mods.mods &&
        old->led.pending_groups == new->led.pending_groups &&
        old->led.groups == new->led.groups &&
        old->led.ctrls == new->led.ctrls &&
        old->led.which_mods == new->led.which_mods &&
        old->led.which_groups == new->led.which_groups) {
        old->defined |= new->defined;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (new->merge == MERGE_REPLACE) {
        ଯଦି (report)
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Map for indicator %s redefined; "
                     "Earlier definition ignored\n",
                     LEDText(info, old));
        *old = *new;
        ଫେରନ୍ତୁ true;
    }

    collide = 0;
    ଯଦି (UseNewLEDField(LED_FIELD_MODS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_mods = new->led.which_mods;
        old->led.mods = new->led.mods;
        old->defined |= LED_FIELD_MODS;
    }
    ଯଦି (UseNewLEDField(LED_FIELD_GROUPS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.which_groups = new->led.which_groups;
        old->led.groups = new->led.groups;
        old->led.pending_groups = new->led.pending_groups;
        old->defined |= LED_FIELD_GROUPS;
    }
    ଯଦି (UseNewLEDField(LED_FIELD_CTRLS, old->defined, new->defined,
                       clobber, report, &collide)) {
        old->led.ctrls = new->led.ctrls;
        old->defined |= LED_FIELD_CTRLS;
    }

    ଯଦି (collide) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Map for indicator %s redefined; "
                 "Using %s definition for duplicate fields\n",
                 LEDText(info, old), (clobber ? "last" : "first"));
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    ପାଇଁ (xkb_led_index_t i = 0; i < info->ଅଙ୍କ_leds; i++) {
        LedInfo *old = &info->leds[i];

        ଯଦି (old->led.name != new->led.name)
            ଜାରି;

        ଫେରନ୍ତୁ MergeLedMap(info, old, new, same_file);
    }

    ଯଦି (info->ଅଙ୍କ_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Too many LEDs defined (maximum %"PRIu32")\n",
                XKB_MAX_LEDS);
        ଫେରନ୍ତୁ false;
    }
    info->leds[info->ଅଙ୍କ_leds++] = *new;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
MergeIncludedCompatMaps(CompatInfo *into, CompatInfo *from,
                        ଗଣନା merge_mode merge)
{
    ଯଦି (from->error_ଗଣନା > 0) {
        into->error_ଗଣନା += from->error_ଗଣନା;
        ଫେରନ୍ତୁ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ଯଦି (into->name == ଖାଲି) {
        into->name = steal(&from->name);
    }

    ଯଦି (darray_empty(into->interps)) {
        into->interps = from->interps;
        darray_init(from->interps);
    }
    ଅନ୍ୟଥା {
        SymInterpInfo *si;
        darray_foreach(si, from->interps) {
            si->merge = merge;
            ଯଦି (!AddInterp(into, si, false))
                into->error_ଗଣନା++;
        }
    }

    ଯଦି (into->ଅଙ୍କ_leds == 0) {
        memcpy(into->leds, from->leds, ଆକାର(*from->leds) * from->ଅଙ୍କ_leds);
        into->ଅଙ୍କ_leds = from->ଅଙ୍କ_leds;
        from->ଅଙ୍କ_leds = 0;
    }
    ଅନ୍ୟଥା {
        ପାଇଁ (xkb_led_index_t i = 0; i < from->ଅଙ୍କ_leds; i++) {
            LedInfo *ledi = &from->leds[i];
            ledi->merge = merge;
            ଯଦି (!AddLedMap(into, ledi, false))
                into->error_ଗଣନା++;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleCompatMapFile(CompatInfo *info, XkbFile *file);

ସ୍ଥିର bool
HandleIncludeCompatMap(CompatInfo *info, IncludeStmt *include)
{
    CompatInfo included;

    ଯଦି (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ଗଣନା += 10;
        ଫେରନ୍ତୁ false;
    }

    InitCompatInfo(&included, info->keymap_info, info->include_depth + 1,
                   &info->mods);
    included.name = steal(&include->stmt);

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ପାଇଁ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        CompatInfo next_incl;
        XkbFile *file;

        ଅକ୍ଷର path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_COMPAT,
                                  path, ଆକାର(path));
        ଯଦି (!file) {
            info->error_ଗଣନା += 10;
            ClearCompatInfo(&included);
            ଫେରନ୍ତୁ false;
        }

        InitCompatInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                       &included.mods);
        next_incl.default_interp = info->default_interp;
        next_incl.default_led = info->default_led;

        HandleCompatMapFile(&next_incl, file);

        MergeIncludedCompatMaps(&included, &next_incl, stmt->merge);

        ClearCompatInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedCompatMaps(info, &included, include->merge);
    ClearCompatInfo(&included);

    ଫେରନ୍ତୁ (info->error_ଗଣନା == 0);
}

ସ୍ଥିର bool
SetInterpField(CompatInfo *info, SymInterpInfo *si, ଅଚଳ ଅକ୍ଷର *field,
               ପଦସମଷ୍ଟି_Def *arrayNdx, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    ଯଦି (istreq(field, "action")) {
        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportSINotArray(info, si, field);

        ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_ACTION_LIST) {
            unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_actions = 0;
            ପାଇଁ (ପଦସମଷ୍ଟି_Def *act = ମୂଲ୍ୟ->actions.actions;
                 act; act = (ପଦସମଷ୍ଟି_Def *) act->common.next)
                 ଅଙ୍କ_actions++;

            ଯଦି (ଅଙ୍କ_actions > MAX_ACTIONS_PER_LEVEL) {
                log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Interpret %s has too many actions; "
                        "expected max %u, got: %u\n",
                        siText(si, info), MAX_ACTIONS_PER_LEVEL, ଅଙ୍କ_actions);
                ଫେରନ୍ତୁ false;
            }

            si->interp.ଅଙ୍କ_actions = 0;
            si->interp.a.action.type = ACTION_TYPE_NONE;

            /* Parse actions and add only defined actions */
            darray(ସଂଘ xkb_action) actions = darray_new();
            ପାଇଁ (ପଦସମଷ୍ଟି_Def *act = ମୂଲ୍ୟ->actions.actions;
                 act; act = (ପଦସମଷ୍ଟି_Def *) act->common.next) {
                ସଂଘ xkb_action toAct = { 0 };
                ବାଛ (HandleActionDef(info->keymap_info,
                                        &info->default_actions,
                                        &info->mods, act, &toAct)) {
                ସ୍ଥିତି PARSER_RECOVERABLE_ERROR:
                    toAct.type = ACTION_TYPE_NONE;
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି PARSER_FATAL_ERROR:
                    darray_free(actions);
                    ଫେରନ୍ତୁ false;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    ;
                }
                ଯଦି (toAct.type == ACTION_TYPE_NONE) {
                    /* Drop action */
                } ଅନ୍ୟଥା ଯଦି (likely(ଅଙ୍କ_actions == 1)) {
                    /* Only one action: do not allocate */
                    si->interp.ଅଙ୍କ_actions = 1;
                    si->interp.a.action = toAct;
                } ଅନ୍ୟଥା {
                    darray_append(actions, toAct);
                }
            }
            ବାଛ (darray_size(actions)) {
            ସ୍ଥିତି 0:
                /* No action or exactly one action: already processed */
                ପ୍ରତିଜ୍ଞା(si->interp.ଅଙ୍କ_actions <= 1);
                ଭାଙ୍ଗ;
            ସ୍ଥିତି 1:
                /* One action: some actions were dropped */
                si->interp.ଅଙ୍କ_actions = 1;
                si->interp.a.action = darray_item(actions, 1);
                darray_free(actions);
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                /* Multiple actions; no NoAction() left */
                darray_shrink(actions);
                si->interp.ଅଙ୍କ_actions =
                    (xkb_action_ଗଣନା_t) darray_size(actions);
                darray_steal(actions, &si->interp.a.actions, ଖାଲି);
            }
        }
        ଅନ୍ୟଥା {
            ବାଛ (HandleActionDef(info->keymap_info,
                                    &info->default_actions,
                                    &info->mods, ମୂଲ୍ୟ, &si->interp.a.action)) {
            ସ୍ଥିତି PARSER_RECOVERABLE_ERROR:
                si->interp.a.action.type = ACTION_TYPE_NONE;
                si->interp.ଅଙ୍କ_actions = 0;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି PARSER_FATAL_ERROR:
                ଫେରନ୍ତୁ false;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                si->interp.ଅଙ୍କ_actions =
                    (si->interp.a.action.type != ACTION_TYPE_NONE);
            }
        }

        si->defined |= SI_FIELD_ACTION;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportSINotArray(info, si, field);

        xkb_mod_index_t ndx = 0;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod(info->ctx, ମୂଲ୍ୟ, MOD_VIRT, &info->mods, &ndx))
            ଫେରନ୍ତୁ ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "repeat")) {
        bool set = false;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportSINotArray(info, si, field);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Boolean(info->ctx, ମୂଲ୍ୟ, &set))
            ଫେରନ୍ତୁ ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "locking")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        uint32_t val = 0;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportSINotArray(info, si, field);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Enum(info->ctx, ମୂଲ୍ୟ, &val, use_Mod_Map_ମୂଲ୍ୟ_Names))
            ଫେରନ୍ତୁ ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    ଅନ୍ୟଥା {
        ReportBadField(info->ctx, "symbol interpretation", field,
                       siText(si, info));
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_INTERPRET_FIELDS);
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
SetLedMapField(CompatInfo *info, LedInfo *ledi, ଅଚଳ ଅକ୍ଷର *field,
               ପଦସମଷ୍ଟି_Def *arrayNdx, ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ପଦସମଷ୍ଟି_Def * restrict ଅଚଳ ମୂଲ୍ୟ = *ମୂଲ୍ୟ_ptr;

    ଯଦି (istreq(field, "modifiers") || istreq(field, "mods")) {
        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportLedNotArray(info, ledi, field);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, ମୂଲ୍ୟ, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            ଫେରନ୍ତୁ ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "groups")) {
        xkb_layout_mask_t mask = 0;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportLedNotArray(info, ledi, field);

        bool pending = false;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Group_Mask(info->keymap_info, ମୂଲ୍ୟ, &mask, &pending)) {
            ଯଦି (pending) {
                ledi->led.pending_groups = true;
                ଅଚଳ darray_size_t pending_index =
                    darray_size(*info->keymap_info->pending_computations);
                darray_append(
                    *info->keymap_info->pending_computations,
                    (ସଂରଚନା pending_computation) {
                        .ପଦସମଷ୍ଟି = *ମୂଲ୍ୟ_ptr,
                        .computed = false,
                        .ମୂଲ୍ୟ = 0,
                    }
                );
                *ମୂଲ୍ୟ_ptr = ଖାଲି;
                static_assert(ଆକାର(pending_index) == ଆକାର(mask),
                              "Cannot save pending computation");
                mask = pending_index;
            } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ ReportLedBadType(info, ledi, field, "group mask");
            }
        } ଅନ୍ୟଥା {
            ledi->led.pending_groups = false;
        }

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "controls") || istreq(field, "ctrls")) {
        uint32_t mask = 0;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportLedNotArray(info, ledi, field);

        ଅଚଳ uint8_t offset = info->keymap_info->features.controls_name_offset;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mask(info->ctx, ମୂଲ୍ୟ, &mask, ctrlMaskNames + offset))
            ଫେରନ୍ତୁ ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        uint32_t mask = 0;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportLedNotArray(info, ledi, field);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mask(info->ctx, ମୂଲ୍ୟ, &mask,
                             modComponentMaskNames))
            ଫେରନ୍ତୁ ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "whichgroupstate")) {
        uint32_t mask = 0;

        ଯଦି (arrayNdx)
            ଫେରନ୍ତୁ ReportLedNotArray(info, ledi, field);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mask(info->ctx, ମୂଲ୍ୟ, &mask,
                             groupComponentMaskNames))
            ଫେରନ୍ତୁ ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    ଅନ୍ୟଥା {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Unknown field \"%s\" in map for %s indicator; "
                "Definition ignored\n",
                field, LEDText(info, ledi));
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_LED_FIELDS);
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *ndx;
    bool ret;

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &ndx)) {
        ret = false;
    }
    ଅନ୍ୟଥା ଯଦି (elem && istreq(elem, "interpret")) {
        SymInterpInfo temp = {0};
        InitInterp(&temp);
        /* Do not replace the whole interpret, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetInterpField(info, &temp, field, ndx, stmt->ମୂଲ୍ୟ);
        ଯଦି (ret)
            MergeInterp(info, &info->default_interp, &temp, true);
    }
    ଅନ୍ୟଥା ଯଦି (elem && istreq(elem, "indicator")) {
        LedInfo temp = {0};
        InitLED(&temp);
        /* Do not replace the whole LED, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetLedMapField(info, &temp, field, ndx, &stmt->ମୂଲ୍ୟ);
        ଯଦି (ret)
            MergeLedMap(info, &info->default_led, &temp, true);
    }
    ଅନ୍ୟଥା ଯଦି (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, ndx,
                                     &stmt->ମୂଲ୍ୟ, stmt->merge) !=
               PARSER_FATAL_ERROR);
    }
    ଅନ୍ୟଥା {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS);
    }
    ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର bool
HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *arrayNdx;

    ପାଇଁ (; def; def = (VarDef *) def->common.next) {
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, def->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ଜାରି;
        }
        ଯଦି (elem) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set a global default value for \"%s\" element from "
                    "within an interpret statement; "
                    "Move assignment to \"%s.%s\" to the global file scope\n",
                    elem, elem, field);
            ok = false;
            ଜାରି;
        }
        ଯଦି (!SetInterpField(info, si, field, arrayNdx, def->ମୂଲ୍ୟ))
            ok = false;
    }

    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
HandleInterpDef(CompatInfo *info, InterpDef *def)
{
    ଗଣନା xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    ଯଦି (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        ଫେରନ୍ତୁ false;
    }

    si = info->default_interp;
    si.merge = def->merge;
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    ଯଦି (!HandleInterpBody(info, def->def, &si)) {
        info->error_ଗଣନା++;
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (!AddInterp(info, &si, true)) {
        info->error_ଗଣନା++;
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleLedMapDef(CompatInfo *info, LedMapDef *def)
{
    LedInfo ledi = info->default_led;
    ledi.merge = def->merge;
    ledi.led.name = def->name;

    bool ok = true;
    ପାଇଁ (VarDef *var = def->body; var != ଖାଲି; var = (VarDef *) var->common.next) {
        ଅଚଳ ଅକ୍ଷର *elem, *field;
        ପଦସମଷ୍ଟି_Def *arrayNdx;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            ଜାରି;
        }

        ଯଦି (elem) {
            log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        ଅନ୍ୟଥା {
            ଯଦି (!SetLedMapField(info, &ledi, field, arrayNdx, &var->ମୂଲ୍ୟ))
                ok = false;
        }
    }

    ଫେରନ୍ତୁ ok && AddLedMap(info, &ledi, true);
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleCompatMapFile(CompatInfo *info, XkbFile *file)
{
    bool ok;

    ମୁକ୍ତ(info->name);
    info->name = strdup_safe(file->name);

    ପାଇଁ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ବାଛ (stmt->type) {
        ସ୍ଥିତି STMT_INCLUDE:
            ok = HandleIncludeCompatMap(info, (IncludeStmt *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_INTERP:
            ok = HandleInterpDef(info, (InterpDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_GROUP_COMPAT:
            log_dbg(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The \"group\" statement in compat is unsupported; "
                    "Ignored\n");
            ok = true;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_LED_MAP:
            ok = HandleLedMapDef(info, (LedMapDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_UNKNOWN_DECLARATION:
        ସ୍ଥିତି STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported compatibility %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Compat files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ଭାଙ୍ଗ;
        }

        ଯଦି (!ok)
            info->error_ଗଣନା++;

        ଯଦି (info->error_ଗଣନା > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning compatibility map \"%s\"\n",
                    safe_map_name(file->name));
            ଭାଙ୍ଗ;
        }
    }
}

/* Temporary struct for CopyInterps. */
ସଂରଚନା collect {
    darray(ସଂରଚନା xkb_sym_interpret) sym_interprets;
};

ସ୍ଥିର ଶୂନ୍ୟ
CopyInterps(CompatInfo *info, bool need_ପ୍ରତୀକ, ଗଣନା xkb_match_operation pred,
            ସଂରଚନା collect *collect)
{
    SymInterpInfo *si;

    darray_foreach(si, info->interps)
        ଯଦି (si->interp.match == pred &&
            (si->interp.sym != XKB_ଚାବି_No_ପ୍ରତୀକ) == need_ପ୍ରତୀକ)
            darray_append(collect->sym_interprets, si->interp);
}

ସ୍ଥିର ଶୂନ୍ୟ
CopyLedMapDefsToKeymap(ସଂରଚନା xkb_keymap *keymap, CompatInfo *info)
{
    ପାଇଁ (xkb_led_index_t idx = 0; idx < info->ଅଙ୍କ_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        ସଂରଚନା xkb_led *led;

        /*
         * Find the LED with the given name, if it was already declared
         * in keycodes.
         */
        xkb_leds_enumerate(i, led, keymap)
            ଯଦି (led->name == ledi->led.name)
                ଭାଙ୍ଗ;

        /* Not previously declared; create it with next free index. */
        ଯଦି (i >= keymap->ଅଙ୍କ_leds) {
            log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Indicator name \"%s\" was not declared in the keycodes "
                    "section; Adding new indicator\n",
                    LEDText(info, ledi));

            xkb_leds_enumerate(i, led, keymap)
                ଯଦି (led->name == XKB_ATOM_NONE)
                    ଭାଙ୍ଗ;

            ଯଦି (i >= keymap->ଅଙ୍କ_leds) {
                /* Not place to put it; ignore. */
                ଯଦି (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                            "Too many indicators (maximum is %"PRIu32"); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS, LEDText(info, ledi));
                    ଜାରି;
                }

                /* Add a new LED. */
                led = &keymap->leds[keymap->ଅଙ୍କ_leds++];
            }
        }

        *led = ledi->led;
        /* Assume pending `groups` computation does not result in 0 */
        ଯଦି (led->which_groups == 0 && (led->groups != 0 || led->pending_groups))
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        ଯଦି (led->which_mods == 0 && led->mods.mods != 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}

ସ୍ଥିର bool
CopyCompatToKeymap(ସଂରଚନା xkb_keymap *keymap, CompatInfo *info)
{
    keymap->compat_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->compat_section_name);

    keymap->mods = info->mods;

    ଯଦି (!darray_empty(info->interps)) {
        ସଂରଚନା collect collect;
        darray_init(collect.sym_interprets);

        /* Most specific to least specific. */
        CopyInterps(info, true, MATCH_EXACTLY, &collect);
        CopyInterps(info, true, MATCH_ALL, &collect);
        CopyInterps(info, true, MATCH_NONE, &collect);
        CopyInterps(info, true, MATCH_ANY, &collect);
        CopyInterps(info, true, MATCH_ANY_OR_NONE, &collect);
        CopyInterps(info, false, MATCH_EXACTLY, &collect);
        CopyInterps(info, false, MATCH_ALL, &collect);
        CopyInterps(info, false, MATCH_NONE, &collect);
        CopyInterps(info, false, MATCH_ANY, &collect);
        CopyInterps(info, false, MATCH_ANY_OR_NONE, &collect);

        darray_steal(collect.sym_interprets,
                     &keymap->sym_interprets, &keymap->ଅଙ୍କ_sym_interprets);
    }

    CopyLedMapDefsToKeymap(keymap, info);

    ଫେରନ୍ତୁ true;
}

bool
CompileCompatMap(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info)
{
    CompatInfo info;

    InitCompatInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ଯଦି (file != ଖାଲି)
        HandleCompatMapFile(&info, file);

    ଯଦି (info.error_ଗଣନା != 0)
        ଯାଅ err_info;

    ଯଦି (!CopyCompatToKeymap(&keymap_info->keymap, &info))
        ଯାଅ err_info;

    ClearCompatInfo(&info);
    ଫେରନ୍ତୁ true;

err_info:
    ClearCompatInfo(&info);
    ଫେରନ୍ତୁ false;
}
