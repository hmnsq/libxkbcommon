/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "utils.h"
#شامل "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_راستہ(ثابت حرف *راستہ)
{
#اگر_تعریف_شدہ _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    واپس is_راستہ_separator(راستہ[0]) ||
           (strlen_محفوظ(راستہ) >= 3 && راستہ[1] == ':' && is_راستہ_separator(راستہ[2]));
#ورنہ
    واپس is_راستہ_separator(راستہ[0]);
#اگر_ختم
}
