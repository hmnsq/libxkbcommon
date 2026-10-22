/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

/** Atoms are indexes into a `darray` */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray_ಗಾತ್ರ_t xkb_atom_t;

#ವ್ಯಾಖ್ಯಾನಿಸು XKB_ATOM_NONE 0

ರಚನೆ atom_ಕೋಷ್ಟಕ;

XKB_EXPORT_ಖಾಸಗಿ ರಚನೆ atom_ಕೋಷ್ಟಕ *
atom_ಕೋಷ್ಟಕ_new(ಶೂನ್ಯ);

XKB_EXPORT_ಖಾಸಗಿ ಶೂನ್ಯ
atom_ಕೋಷ್ಟಕ_free(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ);

XKB_EXPORT_ಖಾಸಗಿ darray_ಗಾತ್ರ_t
atom_ಕೋಷ್ಟಕ_ಗಾತ್ರ(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ);

XKB_EXPORT_ಖಾಸಗಿ xkb_atom_t
atom_intern(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len, bool ಕೂಡಿಸು);

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
atom_ಪಠ್ಯ(ರಚನೆ atom_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, xkb_atom_t atom);
