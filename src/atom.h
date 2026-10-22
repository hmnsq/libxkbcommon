/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"

/** Atoms are indexes into a `darray` */
प्रकारनिर्देश darray_आकार_t xkb_atom_t;

#परिभाषय XKB_ATOM_NONE 0

संरचना atom_सारणी;

XKB_EXPORT_वैयक्तिकम् संरचना atom_सारणी *
atom_सारणी_new(शून्य);

XKB_EXPORT_वैयक्तिकम् शून्य
atom_सारणी_free(संरचना atom_सारणी *सारणी);

XKB_EXPORT_वैयक्तिकम् darray_आकार_t
atom_सारणी_आकार(संरचना atom_सारणी *सारणी);

XKB_EXPORT_वैयक्तिकम् xkb_atom_t
atom_intern(संरचना atom_सारणी *सारणी, ध्रुव वर्ण *string, आकार_t len, bool सङ्कलय);

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
atom_text(संरचना atom_सारणी *सारणी, xkb_atom_t atom);
