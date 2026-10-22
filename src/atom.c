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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t
hash_buf(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ಹಿಂತಿರುಗಿ hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ರಚನೆ atom_ಕೋಷ್ಟಕ {
    xkb_atom_t *ಸೂಚ್ಯಂಕ;
    ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ_ಗಾತ್ರ;
    darray(ಅಕ್ಷರ *) ಅಕ್ಷರಸರಣಿಗಳು;
};

ರಚನೆ atom_ಕೋಷ್ಟಕ *
atom_ಕೋಷ್ಟಕ_new(ಶೂನ್ಯ)
{
    ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*ಕೋಷ್ಟಕ));
    ಆದರೆ (!ಕೋಷ್ಟಕ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    darray_init(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು);
    darray_append(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು, ಖಾಲಿ);
    ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ = 4;
    ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ, ಗಾತ್ರ(*ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ));

    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ಶೂನ್ಯ
atom_ಕೋಷ್ಟಕ_free(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ಆದರೆ (!ಕೋಷ್ಟಕ)
        ಹಿಂತಿರುಗಿ;

    ಅಕ್ಷರ **string;
    darray_foreach(string, ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು)
        ಬಿಡುಗಡೆ(*string);
    darray_free(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು);
    ಬಿಡುಗಡೆ(ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ);
    ಬಿಡುಗಡೆ(ಕೋಷ್ಟಕ);
}

darray_ಗಾತ್ರ_t
atom_ಕೋಷ್ಟಕ_ಗಾತ್ರ(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ಹಿಂತಿರುಗಿ darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
atom_ಪಠ್ಯ(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, xkb_atom_t atom)
{
    ಪ್ರತಿಜ್ಞಾ(atom < darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು));
    ಹಿಂತಿರುಗಿ darray_ಅಂಶ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು, atom);
}

xkb_atom_t
atom_intern(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len, bool ಕೂಡಿಸು)
{
    /* len(string) > 0.8 * index_size */
    ಆದರೆ (darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು) > (ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ / 5) * 4) {
        ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ *= 2;
        xkb_atom_t *tmp = realloc(ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ,
                                  ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ * ಗಾತ್ರ(*ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ));
        ಆದರೆ (!tmp)
            ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;
        ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ = tmp;
        memset(ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ, 0, ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ * ಗಾತ್ರ(*ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ));
        ಗಾಗಿ (darray_ಗಾತ್ರ_t j = 1; j < darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು); j++) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = darray_ಅಂಶ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು, j);
            uint32_t hash = hash_buf(s, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s));
            ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ; i++) {
                ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ_pos = (hash + i) & (ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ - 1);
                ಆದರೆ (ಸೂಚ್ಯಂಕ_pos == 0)
                    ಮುಂದುವರಿ;

                xkb_atom_t atom = ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ[ಸೂಚ್ಯಂಕ_pos];
                ಆದರೆ (atom == XKB_ATOM_NONE) {
                    ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ[ಸೂಚ್ಯಂಕ_pos] = j;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ; i++) {
        ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ_pos = (hash + i) & (ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ_ಗಾತ್ರ - 1);
        ಆದರೆ (ಸೂಚ್ಯಂಕ_pos == 0)
            ಮುಂದುವರಿ;

        xkb_atom_t existing_atom = ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ[ಸೂಚ್ಯಂಕ_pos];
        ಆದರೆ (existing_atom == XKB_ATOM_NONE) {
            ಆದರೆ (ಕೂಡಿಸು) {
                xkb_atom_t new_atom = darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು);
                ಅಕ್ಷರ *s = strndup(string, len);
                ಆದರೆ (!s)
                    ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;
                darray_append(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು, s);
                ಕೋಷ್ಟಕ->ಸೂಚ್ಯಂಕ[ಸೂಚ್ಯಂಕ_pos] = new_atom;
                ಹಿಂತಿರುಗಿ new_atom;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;
            }
        }

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *existing_ಮೌಲ್ಯ = darray_ಅಂಶ(ಕೋಷ್ಟಕ->ಅಕ್ಷರಸರಣಿಗಳು, existing_atom);
        ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(existing_ಮೌಲ್ಯ, string, len) == 0 && existing_ಮೌಲ್ಯ[len] == '\0')
            ಹಿಂತಿರುಗಿ existing_atom;
    }

    ಪ್ರತಿಜ್ಞಾ(!"couldn't find an empty slot during probing");
    ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;
}
