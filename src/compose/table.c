/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "constants.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "paths.h"
#ಅನ್ತರ್ಭಾವ "table.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಸ್ಥಿರ ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_ಕೋಷ್ಟಕ_new(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale,
                      ಎಣಿಕೆ xkb_compose_format format,
                      ಎಣಿಕೆ xkb_compose_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_compose_compile_flags invalid_flags = (
        flags & ~(ಎಣಿಕೆ xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು
    );
    ಆದರೆ (invalid_flags) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (format != XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಅಕ್ಷರ *resolved_locale;
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ;
    ರಚನೆ compose_ಗಂಟು dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ಆದರೆ (!resolved_locale)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಕೋಷ್ಟಕ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*ಕೋಷ್ಟಕ));
    ಆದರೆ (!ಕೋಷ್ಟಕ) {
        ಬಿಡುಗಡೆ(resolved_locale);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಕೋಷ್ಟಕ->refcnt = 1;
    ಕೋಷ್ಟಕ->ctx = xkb_context_ref(ctx);

    ಕೋಷ್ಟಕ->locale = resolved_locale;
    ಕೋಷ್ಟಕ->format = format;
    ಕೋಷ್ಟಕ->flags = flags;

    darray_init(ಕೋಷ್ಟಕ->nodes);
    darray_init(ಕೋಷ್ಟಕ->utf8);

    dummy.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    dummy.ಎಲೆ.is_ಎಲೆ = true;
    dummy.ಎಲೆ.utf8 = 0;
    dummy.ಎಲೆ.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    darray_append(ಕೋಷ್ಟಕ->nodes, dummy);

    darray_append(ಕೋಷ್ಟಕ->utf8, '\0');

    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_ಕೋಷ್ಟಕ_ref(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ಪ್ರತಿಜ್ಞಾ(ಕೋಷ್ಟಕ->refcnt > 0);
    ಕೋಷ್ಟಕ->refcnt++;
    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ಶೂನ್ಯ
xkb_compose_ಕೋಷ್ಟಕ_unref(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ಪ್ರತಿಜ್ಞಾ(!ಕೋಷ್ಟಕ || ಕೋಷ್ಟಕ->refcnt > 0);
    ಆದರೆ (!ಕೋಷ್ಟಕ || --ಕೋಷ್ಟಕ->refcnt > 0)
        ಹಿಂತಿರುಗಿ;
    ಬಿಡುಗಡೆ(ಕೋಷ್ಟಕ->locale);
    darray_free(ಕೋಷ್ಟಕ->nodes);
    darray_free(ಕೋಷ್ಟಕ->utf8);
    xkb_context_unref(ಕೋಷ್ಟಕ->ctx);
    ಬಿಡುಗಡೆ(ಕೋಷ್ಟಕ);
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_ಕೋಷ್ಟಕ_new_from_file(ರಚನೆ xkb_context *ctx,
                                FILE *file,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale,
                                ಎಣಿಕೆ xkb_compose_format format,
                                ಎಣಿಕೆ xkb_compose_compile_flags flags)
{
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ * ಸ್ಥಿರಾಂಕ ಕೋಷ್ಟಕ =
        xkb_compose_ಕೋಷ್ಟಕ_new(ctx, __func__, locale, format, flags);
    ಆದರೆ (!ಕೋಷ್ಟಕ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (!parse_file(ಕೋಷ್ಟಕ, file, "(unknown file)")) {
        xkb_compose_ಕೋಷ್ಟಕ_unref(ಕೋಷ್ಟಕ);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_ಕೋಷ್ಟಕ_new_from_ಬಫರ್(ರಚನೆ xkb_context *ctx,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಉದ್ದಳತೆ,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale,
                                  ಎಣಿಕೆ xkb_compose_format format,
                                  ಎಣಿಕೆ xkb_compose_compile_flags flags)
{
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ * ಸ್ಥಿರಾಂಕ ಕೋಷ್ಟಕ =
        xkb_compose_ಕೋಷ್ಟಕ_new(ctx, __func__, locale, format, flags);
    ಆದರೆ (!ಕೋಷ್ಟಕ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (!parse_string(ಕೋಷ್ಟಕ, ಬಫರ್, ಉದ್ದಳತೆ, "(input string)")) {
        xkb_compose_ಕೋಷ್ಟಕ_unref(ಕೋಷ್ಟಕ);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_ಕೋಷ್ಟಕ_new_from_locale(ರಚನೆ xkb_context *ctx,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale,
                                  ಎಣಿಕೆ xkb_compose_compile_flags flags)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_compose_format format = XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1;
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ * ಸ್ಥಿರಾಂಕ ಕೋಷ್ಟಕ =
        xkb_compose_ಕೋಷ್ಟಕ_new(ctx, __func__, locale, format, flags);
    ಆದರೆ (!ಕೋಷ್ಟಕ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಅಕ್ಷರ *ಮಾರ್ಗ = get_xcomposefile_ಮಾರ್ಗ(ctx);
    FILE *file = open_file(ಮಾರ್ಗ);
    ಆದರೆ (file)
        ಹೋಗು ಸಿಕ್ಕಿತು_ಮಾರ್ಗ;
    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);

    ಮಾರ್ಗ = get_xdg_xcompose_file_ಮಾರ್ಗ(ctx);
    file = open_file(ಮಾರ್ಗ);
    ಆದರೆ (file)
        ಹೋಗು ಸಿಕ್ಕಿತು_ಮಾರ್ಗ;
    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);

    ಮಾರ್ಗ = get_home_xcompose_file_ಮಾರ್ಗ(ctx);
    file = open_file(ಮಾರ್ಗ);
    ಆದರೆ (file)
        ಹೋಗು ಸಿಕ್ಕಿತು_ಮಾರ್ಗ;
    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);

    ಮಾರ್ಗ = get_locale_compose_file_ಮಾರ್ಗ(ctx, ಕೋಷ್ಟಕ->locale);
    file = open_file(ಮಾರ್ಗ);
    ಆದರೆ (file)
        ಹೋಗು ಸಿಕ್ಕಿತು_ಮಾರ್ಗ;
    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, ಕೋಷ್ಟಕ->locale);
    xkb_compose_ಕೋಷ್ಟಕ_unref(ಕೋಷ್ಟಕ);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

ಸಿಕ್ಕಿತು_ಮಾರ್ಗ:
    {} /* Label followed by a declaration is a C23 extension */
    ಸ್ಥಿರಾಂಕ bool ok = parse_file(ಕೋಷ್ಟಕ, file, ಮಾರ್ಗ);
    fclose(file);
    ಆದರೆ (!ok) {
        ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
        xkb_compose_ಕೋಷ್ಟಕ_unref(ಕೋಷ್ಟಕ);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    log_dbg(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "created compose table from locale %s with path %s\n",
            ಕೋಷ್ಟಕ->locale, ಮಾರ್ಗ);

    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
}

ಸ್ಥಿರಾಂಕ xkb_keysym_t *
xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_sequence(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು,
                                 ಗಾತ್ರ_t *sequence_ಉದ್ದಳತೆ)
{
    *sequence_ಉದ್ದಳತೆ = ನಮೂದು->sequence_ಉದ್ದಳತೆ;
    ಹಿಂತಿರುಗಿ ನಮೂದು->sequence;
}

xkb_keysym_t
xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_keysym(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು)
{
    ಹಿಂತಿರುಗಿ ನಮೂದು->keysym;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು_utf8(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು)
{
    ಹಿಂತಿರುಗಿ ನಮೂದು->utf8;
}

#ಆದರೆ MAX_COMPOSE_NODES_LOG2 > 31
#ದೋಷ "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator_pending_ಗಂಟು {
    uint32_t ಸ್ಥಾನಾಂತರ:31;
    bool processed:1;
};

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator {
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ;
    /* Current entry */
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು ನಮೂದು;
    /* Stack of pending nodes to process */
    darray(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator_pending_ಗಂಟು) pending_nodes;
};

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator *
xkb_compose_ಕೋಷ್ಟಕ_iterator_new(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator *iter;
    xkb_keysym_t *sequence;

    iter = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*iter));
    ಆದರೆ (!iter) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    iter->ಕೋಷ್ಟಕ = xkb_compose_ಕೋಷ್ಟಕ_ref(ಕೋಷ್ಟಕ);
    sequence = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(COMPOSE_MAX_LHS_LEN, ಗಾತ್ರ(xkb_keysym_t));
    ಆದರೆ (!sequence) {
        ಬಿಡುಗಡೆ(iter);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    iter->ನಮೂದು.sequence = sequence;
    iter->ನಮೂದು.sequence_ಉದ್ದಳತೆ = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ಆದರೆ (darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->nodes) == 1) {
        ಹಿಂತಿರುಗಿ iter;
    }
    /* Add root node */
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator_pending_ಗಂಟು pending = {
        .ಸ್ಥಾನಾಂತರ = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes,
                                                   pending.ಸ್ಥಾನಾಂತರ);

    /* Find the first left-most node and store intermediate nodes as pending */
    ಆಗ (ಗಂಟು->lokid) {
        pending.ಸ್ಥಾನಾಂತರ = ಗಂಟು->lokid;
        darray_append(iter->pending_nodes, pending);
        ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes, pending.ಸ್ಥಾನಾಂತರ);
    };

    ಹಿಂತಿರುಗಿ iter;
}

ಶೂನ್ಯ
xkb_compose_ಕೋಷ್ಟಕ_iterator_free(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator *iter)
{
    xkb_compose_ಕೋಷ್ಟಕ_unref(iter->ಕೋಷ್ಟಕ);
    darray_free(iter->pending_nodes);
    ಬಿಡುಗಡೆ(iter->ನಮೂದು.sequence);
    ಬಿಡುಗಡೆ(iter);
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_ನಮೂದು *
xkb_compose_ಕೋಷ್ಟಕ_iterator_ಮುಂದಿನ(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator *iter)
{
    /* Traversal algorithm (simplified):
     * 1. Resume last pending node from the stack as the current pending node.
     * 2. If the node is not yet processed, go to 5.
     * 3. Remove node from the stack and remove last keysym from the entry.
     * 4. If there is a right arrow, set it as the current pending node
     *    (unprocessed) and go to 6; else go to 1.
     * 5. Follow down arrow: set the pending node as processed, then:
     *      a) if it is a leaf, complete the entry and return it.
     *      b) else append the child node to the stack and set it as the current
     *         pending node.
     * 6. Find the next left-most arrow and store intermediate pending nodes.
     * 7. Go to 5.
     */

    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator_pending_ಗಂಟು *pending;
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *ಗಂಟು;

    /* Iterator is empty if there is no pending nodes */
    ಆದರೆ (unlikely(darray_ಖಾಲಿಗಳು(iter->pending_nodes))) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_ಅಂಶ(iter->pending_nodes,
                           darray_ಗಾತ್ರ(iter->pending_nodes) - 1);
    ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes, pending->ಸ್ಥಾನಾಂತರ);

    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ_iterator_pending_ಗಂಟು new = {
        .processed = false,
        .ಸ್ಥಾನಾಂತರ = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ಆಗ (pending->processed) {
        /* Remove last keysym */
        iter->ನಮೂದು.sequence_ಉದ್ದಳತೆ--;
        ಆದರೆ (ಗಂಟು->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->ಸ್ಥಾನಾಂತರ = ಗಂಟು->hikid;
            /* Process child’s left arrow */
            ಹೋಗು ಗಂಟು_ಎಡ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Remove processed node */
            darray_remove_ಕೊನೆಯ(iter->pending_nodes);
            ಆದರೆ (unlikely(darray_ಖಾಲಿಗಳು(iter->pending_nodes))) {
                /* No more nodes to process */
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
            /* Get parent node */
            pending = &darray_ಅಂಶ(iter->pending_nodes,
                                   darray_ಗಾತ್ರ(iter->pending_nodes) - 1);
            ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes, pending->ಸ್ಥಾನಾಂತರ);
        }
    }

    ಆಗ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->ನಮೂದು.sequence_ಉದ್ದಳತೆ++;
        iter->ನಮೂದು.sequence[iter->ನಮೂದು.sequence_ಉದ್ದಳತೆ - 1] = ಗಂಟು->keysym;
        ಆದರೆ (ಗಂಟು->is_ಎಲೆ) {
            /* Leaf: return entry */
            iter->ನಮೂದು.keysym = ಗಂಟು->ಎಲೆ.keysym;
            iter->ನಮೂದು.utf8 = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->utf8, ಗಂಟು->ಎಲೆ.utf8);
            ಹಿಂತಿರುಗಿ &iter->ನಮೂದು;
        }
        /* Not a leaf: process child node */
        new.ಸ್ಥಾನಾಂತರ = ಗಂಟು->ಆಂತರಿಕ.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_ಅಂಶ(iter->pending_nodes,
                               darray_ಗಾತ್ರ(iter->pending_nodes) - 1);
ಗಂಟು_ಎಡ:
        ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes, pending->ಸ್ಥಾನಾಂತರ);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ಆಗ (ಗಂಟು->lokid) {
            /* Follow left arrow */
            new.ಸ್ಥಾನಾಂತರ = ಗಂಟು->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_ಅಂಶ(iter->pending_nodes,
                                   darray_ಗಾತ್ರ(iter->pending_nodes) - 1);
            ಗಂಟು = &darray_ಅಂಶ(iter->ಕೋಷ್ಟಕ->nodes, new.ಸ್ಥಾನಾಂತರ);
        }
    }
}
