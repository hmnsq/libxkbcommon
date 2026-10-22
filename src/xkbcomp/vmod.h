/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "ast.h"
#अन्तर्भाव "keymap.h"

शून्य
InitVMods(संरचना xkb_mod_समुच्चयः *info, ध्रुव संरचना xkb_mod_समुच्चयः *mods, bool reset);

शून्य
MergeModSets(संरचना xkb_context *ctx, संरचना xkb_mod_समुच्चयः *into,
             ध्रुव संरचना xkb_mod_समुच्चयः *from, गणना merge_बहुलकम् merge);

bool
HandleVModDef(संरचना xkb_context *ctx, संरचना xkb_mod_समुच्चयः *mods, VModDef *stmt);
