/*
 * Copyright © 2021 Ran Benita <ran@unusedvar.com>
 * Copyright © 2023 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "table.h"

bool
print_compose_table_entry(FILE *file, ಸಂರಚನಾ xkb_compose_table_entry *entry);

bool
xkb_compose_table_dump(FILE *file, ಸಂರಚನಾ xkb_compose_table *table);
