/*
 * Copyright © 2024 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

/* Check if a char is the start of a UTF-8 sequence */
#నిర్వచించు is_utf8_ప్రారంభం(c) (((c) & 0xc0) != 0x80)
#నిర్వచించు INVALID_UTF8_సంకేతం_బిందువు UINT32_MAX

uint8_t
utf8_sequence_నిడివి(స్థిరరాశి అక్షరం *s);

uint32_t
utf8_తదుపరి_సంకేతం_బిందువు(స్థిరరాశి అక్షరం *s, కొలత_t max_కొలత, కొలత_t *కొలత_బహిర్గతం);
