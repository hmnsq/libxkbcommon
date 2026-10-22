/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "ast.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
రకనిర్వచనం నిర్మాణం {
    సంఘం xkb_action actions[ACTION_TYPE_అంకె_ENTRIES];
} ActionsInfo;

శూన్య
InitActionsInfo(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, ActionsInfo *info);

గణన xkb_parser_error
HandleActionDef(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info, ActionsInfo *info,
                స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, సమాసం_Def *def,
                సంఘం xkb_action *action);

గణన xkb_parser_error
సమితి_Default_Action_క్షేత్రం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                      ActionsInfo *info, నిర్మాణం xkb_mod_సమితి *mods,
                      స్థిరరాశి అక్షరం *elem, స్థిరరాశి అక్షరం *క్షేత్రం_2, సమాసం_Def *array_ndx,
                      సమాసం_Def **విలువ_ptr, గణన విలీనం_బహుళకం విలీనం);
