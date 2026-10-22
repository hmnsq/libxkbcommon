/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "utils.h"

#నిర్వచితమైతే _WIN32
#నిర్వచించు మార్గం_SEPARATOR '\\'
#నిర్వచించు ALT_మార్గం_SEPARATOR '/'
#నిర్వచించు is_మార్గం_separator(s) \
    ((s) == మార్గం_SEPARATOR || (s) == ALT_మార్గం_SEPARATOR)
#లేకపోతే
#నిర్వచించు మార్గం_SEPARATOR '/'
#నిర్వచించు is_మార్గం_separator(s) ((s) == మార్గం_SEPARATOR)
#అయితే_ముగింపు

XKB_EXPORT_వ్యక్తిగత bool
is_absolute_మార్గం(స్థిరరాశి అక్షరం *మార్గం);
