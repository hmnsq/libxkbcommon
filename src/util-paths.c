/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "utils.h"
#ضمن "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_رستہ(ثابت حرف *رستہ)
{
#جے_تعریف_شدہ _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    أرجع is_رستہ_separator(رستہ[0]) ||
           (strlen_محفوظ(رستہ) >= 3 && رستہ[1] == ':' && is_رستہ_separator(رستہ[2]));
#وإلا
    أرجع is_رستہ_separator(رستہ[0]);
#جے_انت
}
