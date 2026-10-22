/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#అన్తర్భావ "ast.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "xkbcomp-priv.h"

bool
సమాసం_Resolve_Lhs(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
               స్థిరరాశి అక్షరం **elem_rtrn, స్థిరరాశి అక్షరం **క్షేత్రం_rtrn,
               సమాసం_Def **సూచీ_rtrn);

bool
సమాసం_Resolve_Mod_ముసుగు(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   గణన mod_type mod_type, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                   xkb_mod_ముసుగు_t *ముసుగు_rtrn);

bool
సమాసం_Resolve_Mod(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *def,
               గణన mod_type mod_type, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
               xkb_mod_సూచీ_t *ndx_rtrn);

bool
సమాసం_Resolve_Boolean(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   bool *సమితి_rtrn);

bool
సమాసం_Resolve_Integer(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   int64_t *val_rtrn);

bool
సమాసం_Resolve_Level(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                 xkb_level_సూచీ_t *level_rtrn);

గణన xkb_parser_error
సమాసం_Resolve_సమూహం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                 స్థిరరాశి సమాసం_Def *సమాసం, bool absolute,
                 xkb_layout_సూచీ_t *సమూహం_rtrn, bool *pending_rtrn);

bool
సమాసం_Resolve_సమూహం_ముసుగు(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                     స్థిరరాశి సమాసం_Def *సమాసం, xkb_layout_ముసుగు_t *సమూహం_rtrn,
                     bool *pending_rtrn);

bool
సమాసం_Resolve_Button(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                  int64_t *btn_rtrn);

bool
సమాసం_Resolve_String(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                  xkb_atom_t *val_rtrn);

bool
సమాసం_Resolve_Enum(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                uint32_t *val_rtrn, స్థిరరాశి Lookup_నమోదు *విలువలు);

bool
సమాసం_Resolve_ముసుగు(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                uint32_t *ముసుగు_rtrn, స్థిరరాశి Lookup_నమోదు *విలువలు);
