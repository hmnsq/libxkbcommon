/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "ast.h"
#అన్తర్భావ "keymap.h"

శూన్య
InitVMods(నిర్మాణం xkb_mod_సమితి *info, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, bool reset);

శూన్య
విలీనం_Mod_Sets(నిర్మాణం xkb_context *ctx, నిర్మాణం xkb_mod_సమితి *into,
             స్థిరరాశి నిర్మాణం xkb_mod_సమితి *from, గణన విలీనం_బహుళకం విలీనం);

bool
HandleVModDef(నిర్మాణం xkb_context *ctx, నిర్మాణం xkb_mod_సమితి *mods, VModDef *stmt);
