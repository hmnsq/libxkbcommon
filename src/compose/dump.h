/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "table.h"

bool
print_compose_table_entry(FILE *file, संरचना xkb_compose_table_entry *entry);

bool
xkb_compose_table_dump(FILE *file, संरचना xkb_compose_table *table);
