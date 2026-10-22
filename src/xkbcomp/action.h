/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include "xkbcommon/xkbcommon.h"
#include "ast.h"
#include "keymap.h"
#include "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
typedef struct {
    union xkb_action actions[ACTION_TYPE_नुमेरो_ENTRIES];
} ActionsInfo;

void
InitActionsInfo(const struct xkb_keymap *keymap, ActionsInfo *info);

enum xkb_parser_error
HandleActionDef(const struct xkb_keymap_info *keymap_info, ActionsInfo *info,
                const struct xkb_mod_set *mods, पाहायाग_Def *def,
                union xkb_action *action);

enum xkb_parser_error
SetDefaultActionField(const struct xkb_keymap_info *keymap_info,
                      ActionsInfo *info, struct xkb_mod_set *mods,
                      const char *elem, const char *field, पाहायाग_Def *array_ndx,
                      पाहायाग_Def **हालागा_ptr, enum merge_mode merge);
