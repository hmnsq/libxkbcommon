/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "keymap.h"

/** LUT entry */
రకనిర్వచనం నిర్మాణం {
    స్థిరరాశి అక్షరం *name;
    uint32_t విలువ;
} Lookup_నమోదు;

bool
LookupString(స్థిరరాశి Lookup_నమోదు tab[], స్థిరరాశి అక్షరం *string,
              గుర్తులేని పూర్ణాంకం *విలువ_rtrn);

స్థిరరాశి అక్షరం *
Lookup_విలువ(స్థిరరాశి Lookup_నమోదు tab[], గుర్తులేని పూర్ణాంకం విలువ);

#నిర్వచించు సమూహం_చివరి_సూచీ_NAME "last"
గణన {
    CONTROL_పేర్లు_MIN_V1_సూచీ = 7,
    CONTROL_పేర్లు_MIN_V2_సూచీ = 0,
};

స్థిర అంతఃవరుస uint8_t
format_control_పేర్లు_స్థానభ్రంశం(గణన xkb_keymap_format format)
{
    తిరిగి (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
        ? CONTROL_పేర్లు_MIN_V1_సూచీ
        : CONTROL_పేర్లు_MIN_V2_సూచీ;
}

బాహ్య స్థిరరాశి Lookup_నమోదు ctrl_ముసుగు_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు mod_Component_ముసుగు_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు సమూహం_Component_ముసుగు_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు సమూహం_ముసుగు_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు button_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు use_Mod_Map_విలువ_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు action_Type_పేర్లు[];
బాహ్య స్థిరరాశి Lookup_నమోదు sym_Interpret_సరిపోలిక_ముసుగు_పేర్లు[];

స్థిరరాశి అక్షరం *
Mod_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, గణన mod_type type,
            స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, xkb_mod_ముసుగు_t ముసుగు);

స్థిరరాశి అక్షరం *
Mod_సూచీ_పాఠ్యం(నిర్మాణం xkb_context *ctx, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
             xkb_mod_సూచీ_t ndx);

స్థిరరాశి అక్షరం *
Action_Type_పాఠ్యం(గణన xkb_action_type type);

స్థిరరాశి అక్షరం *
Keysym_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_keysym_t sym);

స్థిరరాశి అక్షరం *
కీలకం_Name_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_atom_t name);

స్థిరరాశి అక్షరం *
SIMatch_పాఠ్యం(గణన xkb_సరిపోలిక_operation type);

స్థిరరాశి అక్షరం *
Led_State_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, స్థిరరాశి Lookup_నమోదు *lookup,
                 గణన xkb_state_component ముసుగు);

స్థిరరాశి అక్షరం *
Control_ముసుగు_పాఠ్యం(నిర్మాణం xkb_context *ctx, గణన xkb_keymap_format format,
                గణన xkb_action_controls ముసుగు);
