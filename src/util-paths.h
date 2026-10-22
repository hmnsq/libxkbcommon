/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "utils.h"

#यदि_परिभाषितम् _WIN32
#परिभाषय मार्गः_SEPARATOR '\\'
#परिभाषय ALT_मार्गः_SEPARATOR '/'
#परिभाषय is_मार्गः_separator(s) \
    ((s) == मार्गः_SEPARATOR || (s) == ALT_मार्गः_SEPARATOR)
#अन्यथा
#परिभाषय मार्गः_SEPARATOR '/'
#परिभाषय is_मार्गः_separator(s) ((s) == मार्गः_SEPARATOR)
#यदि_अन्तः

XKB_EXPORT_वैयक्तिकम् bool
is_absolute_मार्गः(ध्रुव वर्ण *मार्गः);
