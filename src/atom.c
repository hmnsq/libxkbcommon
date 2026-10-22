/*
 * For MIT-open-group:
 * Copyright 1987, 1998  The Open Group
 *
 * For HPND:
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 * Copyright 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * SPDX-License-Identifier: MIT-open-group AND HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "atom.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
స్థిర అంతఃవరుస uint32_t
hash_buf(స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    కొరకు (కొలత_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    తిరిగి hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
నిర్మాణం atom_పట్టిక {
    xkb_atom_t *సూచీ;
    కొలత_t సూచీ_కొలత;
    darray(అక్షరం *) అక్షరశ్రేణులు;
};

నిర్మాణం atom_పట్టిక *
atom_పట్టిక_new(శూన్య)
{
    నిర్మాణం atom_పట్టిక *పట్టిక = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*పట్టిక));
    ఒకవేళ (!పట్టిక)
        తిరిగి ఖాళీ;

    darray_init(పట్టిక->అక్షరశ్రేణులు);
    darray_append(పట్టిక->అక్షరశ్రేణులు, ఖాళీ);
    పట్టిక->సూచీ_కొలత = 4;
    పట్టిక->సూచీ = సున్నా_జ్ఞాపకం_ఇవ్వు(పట్టిక->సూచీ_కొలత, పరిమాణం(*పట్టిక->సూచీ));

    తిరిగి పట్టిక;
}

శూన్య
atom_పట్టిక_free(నిర్మాణం atom_పట్టిక *పట్టిక)
{
    ఒకవేళ (!పట్టిక)
        తిరిగి;

    అక్షరం **string;
    darray_foreach(string, పట్టిక->అక్షరశ్రేణులు)
        విడుదల(*string);
    darray_free(పట్టిక->అక్షరశ్రేణులు);
    విడుదల(పట్టిక->సూచీ);
    విడుదల(పట్టిక);
}

darray_కొలత_t
atom_పట్టిక_కొలత(నిర్మాణం atom_పట్టిక *పట్టిక)
{
    తిరిగి darray_కొలత(పట్టిక->అక్షరశ్రేణులు);
}

స్థిరరాశి అక్షరం *
atom_పాఠ్యం(నిర్మాణం atom_పట్టిక *పట్టిక, xkb_atom_t atom)
{
    ప్రతిజ్ఞా(atom < darray_కొలత(పట్టిక->అక్షరశ్రేణులు));
    తిరిగి darray_అంశం(పట్టిక->అక్షరశ్రేణులు, atom);
}

xkb_atom_t
atom_intern(నిర్మాణం atom_పట్టిక *పట్టిక, స్థిరరాశి అక్షరం *string, కొలత_t len, bool కూడు)
{
    /* len(string) > 0.8 * index_size */
    ఒకవేళ (darray_కొలత(పట్టిక->అక్షరశ్రేణులు) > (పట్టిక->సూచీ_కొలత / 5) * 4) {
        పట్టిక->సూచీ_కొలత *= 2;
        xkb_atom_t *tmp = realloc(పట్టిక->సూచీ,
                                  పట్టిక->సూచీ_కొలత * పరిమాణం(*పట్టిక->సూచీ));
        ఒకవేళ (!tmp)
            తిరిగి XKB_ATOM_NONE;
        పట్టిక->సూచీ = tmp;
        memset(పట్టిక->సూచీ, 0, పట్టిక->సూచీ_కొలత * పరిమాణం(*పట్టిక->సూచీ));
        కొరకు (darray_కొలత_t j = 1; j < darray_కొలత(పట్టిక->అక్షరశ్రేణులు); j++) {
            స్థిరరాశి అక్షరం *s = darray_అంశం(పట్టిక->అక్షరశ్రేణులు, j);
            uint32_t hash = hash_buf(s, అక్షరశ్రేణి_పొడవు(s));
            కొరకు (కొలత_t i = 0; i < పట్టిక->సూచీ_కొలత; i++) {
                కొలత_t సూచీ_pos = (hash + i) & (పట్టిక->సూచీ_కొలత - 1);
                ఒకవేళ (సూచీ_pos == 0)
                    కొనసాగు;

                xkb_atom_t atom = పట్టిక->సూచీ[సూచీ_pos];
                ఒకవేళ (atom == XKB_ATOM_NONE) {
                    పట్టిక->సూచీ[సూచీ_pos] = j;
                    ఆపు;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    కొరకు (కొలత_t i = 0; i < పట్టిక->సూచీ_కొలత; i++) {
        కొలత_t సూచీ_pos = (hash + i) & (పట్టిక->సూచీ_కొలత - 1);
        ఒకవేళ (సూచీ_pos == 0)
            కొనసాగు;

        xkb_atom_t existing_atom = పట్టిక->సూచీ[సూచీ_pos];
        ఒకవేళ (existing_atom == XKB_ATOM_NONE) {
            ఒకవేళ (కూడు) {
                xkb_atom_t new_atom = darray_కొలత(పట్టిక->అక్షరశ్రేణులు);
                అక్షరం *s = strndup(string, len);
                ఒకవేళ (!s)
                    తిరిగి XKB_ATOM_NONE;
                darray_append(పట్టిక->అక్షరశ్రేణులు, s);
                పట్టిక->సూచీ[సూచీ_pos] = new_atom;
                తిరిగి new_atom;
            } లేకపోతే {
                తిరిగి XKB_ATOM_NONE;
            }
        }

        స్థిరరాశి అక్షరం *existing_విలువ = darray_అంశం(పట్టిక->అక్షరశ్రేణులు, existing_atom);
        ఒకవేళ (అక్షరశ్రేణి_పోలిక(existing_విలువ, string, len) == 0 && existing_విలువ[len] == '\0')
            తిరిగి existing_atom;
    }

    ప్రతిజ్ఞా(!"couldn't find an empty slot during probing");
    తిరిగి XKB_ATOM_NONE;
}
