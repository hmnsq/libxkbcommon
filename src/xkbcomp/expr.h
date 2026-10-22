/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "xkbcomp-priv.h"

bool
व्यञ्जकम्_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **क्षेत्रम्_rtrn,
               व्यञ्जकम्_Def **सूचकांक_rtrn);

bool
व्यञ्जकम्_Resolve_Mod_आवरणम्(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                   xkb_mod_आवरणम्_t *आवरणम्_rtrn);

bool
व्यञ्जकम्_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
               xkb_mod_सूचकांक_t *ndx_rtrn);

bool
व्यञ्जकम्_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   bool *समुच्चयः_rtrn);

bool
व्यञ्जकम्_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   int64_t *val_rtrn);

bool
व्यञ्जकम्_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                 xkb_level_सूचकांक_t *level_rtrn);

गणना xkb_parser_error
व्यञ्जकम्_Resolve_गणः(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, bool absolute,
                 xkb_layout_सूचकांक_t *गणः_rtrn, bool *pending_rtrn);

bool
व्यञ्जकम्_Resolve_गणः_आवरणम्(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, xkb_layout_आवरणम्_t *गणः_rtrn,
                     bool *pending_rtrn);

bool
व्यञ्जकम्_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  int64_t *btn_rtrn);

bool
व्यञ्जकम्_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  xkb_atom_t *val_rtrn);

bool
व्यञ्जकम्_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *val_rtrn, ध्रुव Lookup_प्रविष्टिः *मानानि);

bool
व्यञ्जकम्_Resolve_आवरणम्(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *आवरणम्_rtrn, ध्रुव Lookup_प्रविष्टिः *मानानि);
