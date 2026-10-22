/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "ast.h"
#अन्तर्भाव "keymap.h"

शून्य
InitVMods(संरचना xkb_mod_set *info, स्थिरांक संरचना xkb_mod_set *mods, bool reset);

शून्य
MergeModSets(संरचना xkb_context *ctx, संरचना xkb_mod_set *into,
             स्थिरांक संरचना xkb_mod_set *from, गणना merge_mode merge);

bool
HandleVModDef(संरचना xkb_context *ctx, संरचना xkb_mod_set *mods, VModDef *stmt);
