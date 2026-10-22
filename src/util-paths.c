/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    ਵਾਪਸ is_ਰਸਤਾ_separator(ਰਸਤਾ[0]) ||
           (strlen_ਸੁਰੱਖਿਅਤ(ਰਸਤਾ) >= 3 && ਰਸਤਾ[1] == ':' && is_ਰਸਤਾ_separator(ਰਸਤਾ[2]));
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ is_ਰਸਤਾ_separator(ਰਸਤਾ[0]);
#ਜੇ_ਅੰਤ
}
