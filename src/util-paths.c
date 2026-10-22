/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    ಹಿಂತಿರುಗಿ is_ಮಾರ್ಗ_separator(ಮಾರ್ಗ[0]) ||
           (strlen_ಸುರಕ್ಷಿತ(ಮಾರ್ಗ) >= 3 && ಮಾರ್ಗ[1] == ':' && is_ಮಾರ್ಗ_separator(ಮಾರ್ಗ[2]));
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ is_ಮಾರ್ಗ_separator(ಮಾರ್ಗ[0]);
#ಆದರೆ_ಅಂತ್ಯ
}
