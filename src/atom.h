/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/** Atoms are indexes into a `darray` */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray_ਮਾਪ_t xkb_atom_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_ATOM_NONE 0

ਬਣਤਰ atom_ਸਾਰਣੀ;

XKB_EXPORT_ਨਿੱਜੀ ਬਣਤਰ atom_ਸਾਰਣੀ *
atom_ਸਾਰਣੀ_new(ਖਾਲੀ);

XKB_EXPORT_ਨਿੱਜੀ ਖਾਲੀ
atom_ਸਾਰਣੀ_free(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ);

XKB_EXPORT_ਨਿੱਜੀ darray_ਮਾਪ_t
atom_ਸਾਰਣੀ_ਮਾਪ(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ);

XKB_EXPORT_ਨਿੱਜੀ xkb_atom_t
atom_intern(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len, bool ਜੋੜੋ);

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
atom_ਪਾਠ(ਬਣਤਰ atom_ਸਾਰਣੀ *ਸਾਰਣੀ, xkb_atom_t atom);
