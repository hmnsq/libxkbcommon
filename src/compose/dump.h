/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن "table.h"

bool
print_compose_table_entry(FILE *file, بنية xkb_compose_table_entry *entry);

bool
xkb_compose_table_dump(FILE *file, بنية xkb_compose_table *table);
