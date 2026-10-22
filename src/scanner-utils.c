/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"

ರಚನೆ scanner_loc
scanner_token_location(ರಚನೆ scanner *s)
{
    /*
     * The following article and accompying code compares some algorithms:
     * https://lemire.me/blog/2017/02/14/how-fast-can-you-count-lines/
     * https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2017/02/14/newlines.c
     * This is the fastest portable one for me, and it's simple.
     *
     * To avoid rescanning on each call, cache the previously found position
     * and start from that on the next call. This is effective as long as the
     * tokens go forward.
     */
    ಗಾತ್ರ_t ಸಾಲು, ಸ್ತಂಭ;
    ಗಾತ್ರ_t ಸಾಲು_pos = 0;
    ಆದರೆ (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ಸಾಲು = s->cached_loc.ಸ್ತಂಭ = 1;
    }
    ಸಾಲು = s->cached_loc.ಸಾಲು;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ptr = s->s + s->cached_pos;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ = s->s + s->token_pos;
    ಆಗ ((ptr = memchr(ptr, '\n', (ಗಾತ್ರ_t)(ಕೊನೆಯ - ptr)))) {
        ಸಾಲು++;
        ptr++;
        ಸಾಲು_pos = (ಗಾತ್ರ_t)(ptr - s->s);
    }
    ಆದರೆ (ಸಾಲು == s->cached_loc.ಸಾಲು) {
        ಸ್ತಂಭ = s->cached_loc.ಸ್ತಂಭ + (s->token_pos - s->cached_pos);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಸ್ತಂಭ = s->token_pos - ಸಾಲು_pos + 1;
    }
    ರಚನೆ scanner_loc loc = {.ಸಾಲು = ಸಾಲು, .ಸ್ತಂಭ = ಸ್ತಂಭ};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ಹಿಂತಿರುಗಿ loc;
}
