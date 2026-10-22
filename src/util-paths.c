/*
 * Copyright © 2024 Pierre Le Marre
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-paths.h"


/* Caller must ensure that the input is not NULL or empty */
bool
is_absolute_మార్గం(స్థిరరాశి అక్షరం *మార్గం)
{
#నిర్వచితమైతే _WIN32
    /*
     * A file name is relative to the current directory if it does not begin with
     * one of the following:
     * - A UNC name of any format, which always start with two backslash characters ("\\").
     * - A disk designator with a backslash, for example "C:\" or "d:\".
     * - A single backslash, for example, "\directory" or "\file.txt".
     * See: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
     */
    తిరిగి is_మార్గం_separator(మార్గం[0]) ||
           (strlen_సురక్షిత(మార్గం) >= 3 && మార్గం[1] == ':' && is_మార్గం_separator(మార్గం[2]));
#లేకపోతే
    తిరిగి is_మార్గం_separator(మార్గం[0]);
#అయితే_ముగింపు
}
