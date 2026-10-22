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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
hash_buf(ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    ਲਈ (ਮਾਪ_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ਵਾਪਸ hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ਬਣਤਰ atom_ਸਾਰਣੀ {
    xkb_atom_t *ਸੂਚਕਾਂਕ;
    ਮਾਪ_t ਸੂਚਕਾਂਕ_ਮਾਪ;
    darray(ਅੱਖਰ *) ਲੜੀਆਂ;
};

ਬਣਤਰ atom_ਸਾਰਣੀ *
atom_ਸਾਰਣੀ_new(ਖਾਲੀ)
{
    ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*ਸਾਰਣੀ));
    ਜੇ (!ਸਾਰਣੀ)
        ਵਾਪਸ ਨਿਰਾ;

    darray_init(ਸਾਰਣੀ->ਲੜੀਆਂ);
    darray_append(ਸਾਰਣੀ->ਲੜੀਆਂ, ਨਿਰਾ);
    ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ = 4;
    ਸਾਰਣੀ->ਸੂਚਕਾਂਕ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ, ਆਕਾਰ(*ਸਾਰਣੀ->ਸੂਚਕਾਂਕ));

    ਵਾਪਸ ਸਾਰਣੀ;
}

ਖਾਲੀ
atom_ਸਾਰਣੀ_free(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਜੇ (!ਸਾਰਣੀ)
        ਵਾਪਸ;

    ਅੱਖਰ **string;
    darray_foreach(string, ਸਾਰਣੀ->ਲੜੀਆਂ)
        ਮੁਕਤ(*string);
    darray_free(ਸਾਰਣੀ->ਲੜੀਆਂ);
    ਮੁਕਤ(ਸਾਰਣੀ->ਸੂਚਕਾਂਕ);
    ਮੁਕਤ(ਸਾਰਣੀ);
}

darray_ਮਾਪ_t
atom_ਸਾਰਣੀ_ਮਾਪ(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਵਾਪਸ darray_ਮਾਪ(ਸਾਰਣੀ->ਲੜੀਆਂ);
}

ਅਚਲ ਅੱਖਰ *
atom_ਪਾਠ(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ, xkb_atom_t atom)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(atom < darray_ਮਾਪ(ਸਾਰਣੀ->ਲੜੀਆਂ));
    ਵਾਪਸ darray_ਵਸਤੂ(ਸਾਰਣੀ->ਲੜੀਆਂ, atom);
}

xkb_atom_t
atom_intern(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len, bool ਜੋੜੋ)
{
    /* len(string) > 0.8 * index_size */
    ਜੇ (darray_ਮਾਪ(ਸਾਰਣੀ->ਲੜੀਆਂ) > (ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ / 5) * 4) {
        ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ *= 2;
        xkb_atom_t *tmp = realloc(ਸਾਰਣੀ->ਸੂਚਕਾਂਕ,
                                  ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ * ਆਕਾਰ(*ਸਾਰਣੀ->ਸੂਚਕਾਂਕ));
        ਜੇ (!tmp)
            ਵਾਪਸ XKB_ATOM_NONE;
        ਸਾਰਣੀ->ਸੂਚਕਾਂਕ = tmp;
        memset(ਸਾਰਣੀ->ਸੂਚਕਾਂਕ, 0, ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ * ਆਕਾਰ(*ਸਾਰਣੀ->ਸੂਚਕਾਂਕ));
        ਲਈ (darray_ਮਾਪ_t j = 1; j < darray_ਮਾਪ(ਸਾਰਣੀ->ਲੜੀਆਂ); j++) {
            ਅਚਲ ਅੱਖਰ *s = darray_ਵਸਤੂ(ਸਾਰਣੀ->ਲੜੀਆਂ, j);
            uint32_t hash = hash_buf(s, ਲੜੀ_ਲੰਬਾਈ(s));
            ਲਈ (ਮਾਪ_t i = 0; i < ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ; i++) {
                ਮਾਪ_t ਸੂਚਕਾਂਕ_pos = (hash + i) & (ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ - 1);
                ਜੇ (ਸੂਚਕਾਂਕ_pos == 0)
                    ਜਾਰੀ;

                xkb_atom_t atom = ਸਾਰਣੀ->ਸੂਚਕਾਂਕ[ਸੂਚਕਾਂਕ_pos];
                ਜੇ (atom == XKB_ATOM_NONE) {
                    ਸਾਰਣੀ->ਸੂਚਕਾਂਕ[ਸੂਚਕਾਂਕ_pos] = j;
                    ਰੋਕੋ;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    ਲਈ (ਮਾਪ_t i = 0; i < ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ; i++) {
        ਮਾਪ_t ਸੂਚਕਾਂਕ_pos = (hash + i) & (ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_ਮਾਪ - 1);
        ਜੇ (ਸੂਚਕਾਂਕ_pos == 0)
            ਜਾਰੀ;

        xkb_atom_t existing_atom = ਸਾਰਣੀ->ਸੂਚਕਾਂਕ[ਸੂਚਕਾਂਕ_pos];
        ਜੇ (existing_atom == XKB_ATOM_NONE) {
            ਜੇ (ਜੋੜੋ) {
                xkb_atom_t new_atom = darray_ਮਾਪ(ਸਾਰਣੀ->ਲੜੀਆਂ);
                ਅੱਖਰ *s = strndup(string, len);
                ਜੇ (!s)
                    ਵਾਪਸ XKB_ATOM_NONE;
                darray_append(ਸਾਰਣੀ->ਲੜੀਆਂ, s);
                ਸਾਰਣੀ->ਸੂਚਕਾਂਕ[ਸੂਚਕਾਂਕ_pos] = new_atom;
                ਵਾਪਸ new_atom;
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ XKB_ATOM_NONE;
            }
        }

        ਅਚਲ ਅੱਖਰ *existing_ਮੁੱਲ = darray_ਵਸਤੂ(ਸਾਰਣੀ->ਲੜੀਆਂ, existing_atom);
        ਜੇ (ਲੜੀ_ਤੁਲਨਾ(existing_ਮੁੱਲ, string, len) == 0 && existing_ਮੁੱਲ[len] == '\0')
            ਵਾਪਸ existing_atom;
    }

    ਪ੍ਰਤਿਜ੍ਞਾ(!"couldn't find an empty slot during probing");
    ਵਾਪਸ XKB_ATOM_NONE;
}
