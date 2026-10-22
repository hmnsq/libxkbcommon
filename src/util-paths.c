/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "utils.h"
#ضمن "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_رستو(ثابت حرف *رستو)
{
#جيڪڏهن_وصف_ٿيل _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    أرجع is_رستو_separator(رستو[0]) ||
           (strlen_محفوظ(رستو) >= 3 && رستو[1] == ':' && is_رستو_separator(رستو[2]));
#وإلا
    أرجع is_رستو_separator(رستو[0]);
#جيڪڏهن_پورو
}
