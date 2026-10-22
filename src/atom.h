/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"

/** Atoms are indexes into a `darray` */
రకనిర్వచనం darray_కొలత_t xkb_atom_t;

#నిర్వచించు XKB_ATOM_NONE 0

నిర్మాణం atom_పట్టిక;

XKB_EXPORT_వ్యక్తిగత నిర్మాణం atom_పట్టిక *
atom_పట్టిక_new(శూన్య);

XKB_EXPORT_వ్యక్తిగత శూన్య
atom_పట్టిక_free(నిర్మాణం atom_పట్టిక *పట్టిక);

XKB_EXPORT_వ్యక్తిగత darray_కొలత_t
atom_పట్టిక_కొలత(నిర్మాణం atom_పట్టిక *పట్టిక);

XKB_EXPORT_వ్యక్తిగత xkb_atom_t
atom_intern(నిర్మాణం atom_పట్టిక *పట్టిక, స్థిరరాశి అక్షరం *string, కొలత_t len, bool కూడు);

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
atom_పాఠ్యం(నిర్మాణం atom_పట్టిక *పట్టిక, xkb_atom_t atom);
