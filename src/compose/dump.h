/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "table.h"

bool
print_compose_table_entry(FILE *file, கட்டமைப்பு xkb_compose_table_entry *entry);

bool
xkb_compose_table_dump(FILE *file, கட்டமைப்பு xkb_compose_table *table);
