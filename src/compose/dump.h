/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "table.h"

bool
print_compose_table_entry(FILE *file, ഘടന xkb_compose_table_entry *entry);

bool
xkb_compose_table_dump(FILE *file, ഘടന xkb_compose_table *table);
