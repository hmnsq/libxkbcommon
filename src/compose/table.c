/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "constants.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "paths.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "table.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਸਥਿਰ ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_ਸਾਰਣੀ_new(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *func,
                      ਅਚਲ ਅੱਖਰ *locale,
                      ਗਿਣਤੀ xkb_compose_format format,
                      ਗਿਣਤੀ xkb_compose_compile_flags flags)
{
    ਅਚਲ ਗਿਣਤੀ xkb_compose_compile_flags invalid_flags = (
        flags & ~(ਗਿਣਤੀ xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_ਮੁੱਲਾਂ
    );
    ਜੇ (invalid_flags) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (format != XKB_COMPOSE_FORMAT_ਪਾਠ_V1) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਅੱਖਰ *resolved_locale;
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ;
    ਬਣਤਰ compose_ਗੰਢ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ਜੇ (!resolved_locale)
        ਵਾਪਸ ਨਿਰਾ;

    ਸਾਰਣੀ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*ਸਾਰਣੀ));
    ਜੇ (!ਸਾਰਣੀ) {
        ਮੁਕਤ(resolved_locale);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਸਾਰਣੀ->refcnt = 1;
    ਸਾਰਣੀ->ctx = xkb_context_ref(ctx);

    ਸਾਰਣੀ->locale = resolved_locale;
    ਸਾਰਣੀ->format = format;
    ਸਾਰਣੀ->flags = flags;

    darray_init(ਸਾਰਣੀ->nodes);
    darray_init(ਸਾਰਣੀ->utf8);

    dummy.keysym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    dummy.ਪੱਤੀ.is_ਪੱਤੀ = true;
    dummy.ਪੱਤੀ.utf8 = 0;
    dummy.ਪੱਤੀ.keysym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    darray_append(ਸਾਰਣੀ->nodes, dummy);

    darray_append(ਸਾਰਣੀ->utf8, '\0');

    ਵਾਪਸ ਸਾਰਣੀ;
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_ਸਾਰਣੀ_ref(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(ਸਾਰਣੀ->refcnt > 0);
    ਸਾਰਣੀ->refcnt++;
    ਵਾਪਸ ਸਾਰਣੀ;
}

ਖਾਲੀ
xkb_compose_ਸਾਰਣੀ_unref(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!ਸਾਰਣੀ || ਸਾਰਣੀ->refcnt > 0);
    ਜੇ (!ਸਾਰਣੀ || --ਸਾਰਣੀ->refcnt > 0)
        ਵਾਪਸ;
    ਮੁਕਤ(ਸਾਰਣੀ->locale);
    darray_free(ਸਾਰਣੀ->nodes);
    darray_free(ਸਾਰਣੀ->utf8);
    xkb_context_unref(ਸਾਰਣੀ->ctx);
    ਮੁਕਤ(ਸਾਰਣੀ);
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_ਸਾਰਣੀ_new_from_file(ਬਣਤਰ xkb_context *ctx,
                                FILE *file,
                                ਅਚਲ ਅੱਖਰ *locale,
                                ਗਿਣਤੀ xkb_compose_format format,
                                ਗਿਣਤੀ xkb_compose_compile_flags flags)
{
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ * ਅਚਲ ਸਾਰਣੀ =
        xkb_compose_ਸਾਰਣੀ_new(ctx, __func__, locale, format, flags);
    ਜੇ (!ਸਾਰਣੀ)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (!parse_file(ਸਾਰਣੀ, file, "(unknown file)")) {
        xkb_compose_ਸਾਰਣੀ_unref(ਸਾਰਣੀ);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ਸਾਰਣੀ;
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_ਸਾਰਣੀ_new_from_ਬਫਰ(ਬਣਤਰ xkb_context *ctx,
                                  ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਲੰਮਾਈ,
                                  ਅਚਲ ਅੱਖਰ *locale,
                                  ਗਿਣਤੀ xkb_compose_format format,
                                  ਗਿਣਤੀ xkb_compose_compile_flags flags)
{
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ * ਅਚਲ ਸਾਰਣੀ =
        xkb_compose_ਸਾਰਣੀ_new(ctx, __func__, locale, format, flags);
    ਜੇ (!ਸਾਰਣੀ)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (!parse_string(ਸਾਰਣੀ, ਬਫਰ, ਲੰਮਾਈ, "(input string)")) {
        xkb_compose_ਸਾਰਣੀ_unref(ਸਾਰਣੀ);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ਸਾਰਣੀ;
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_ਸਾਰਣੀ_new_from_locale(ਬਣਤਰ xkb_context *ctx,
                                  ਅਚਲ ਅੱਖਰ *locale,
                                  ਗਿਣਤੀ xkb_compose_compile_flags flags)
{
    ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ xkb_compose_format format = XKB_COMPOSE_FORMAT_ਪਾਠ_V1;
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ * ਅਚਲ ਸਾਰਣੀ =
        xkb_compose_ਸਾਰਣੀ_new(ctx, __func__, locale, format, flags);
    ਜੇ (!ਸਾਰਣੀ)
        ਵਾਪਸ ਨਿਰਾ;

    ਅੱਖਰ *ਰਸਤਾ = get_xcomposefile_ਰਸਤਾ(ctx);
    FILE *file = open_file(ਰਸਤਾ);
    ਜੇ (file)
        ਜਾਓ ਮਿਲਿਆ_ਰਸਤਾ;
    ਮੁਕਤ(ਰਸਤਾ);

    ਰਸਤਾ = get_xdg_xcompose_file_ਰਸਤਾ(ctx);
    file = open_file(ਰਸਤਾ);
    ਜੇ (file)
        ਜਾਓ ਮਿਲਿਆ_ਰਸਤਾ;
    ਮੁਕਤ(ਰਸਤਾ);

    ਰਸਤਾ = get_home_xcompose_file_ਰਸਤਾ(ctx);
    file = open_file(ਰਸਤਾ);
    ਜੇ (file)
        ਜਾਓ ਮਿਲਿਆ_ਰਸਤਾ;
    ਮੁਕਤ(ਰਸਤਾ);

    ਰਸਤਾ = get_locale_compose_file_ਰਸਤਾ(ctx, ਸਾਰਣੀ->locale);
    file = open_file(ਰਸਤਾ);
    ਜੇ (file)
        ਜਾਓ ਮਿਲਿਆ_ਰਸਤਾ;
    ਮੁਕਤ(ਰਸਤਾ);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, ਸਾਰਣੀ->locale);
    xkb_compose_ਸਾਰਣੀ_unref(ਸਾਰਣੀ);
    ਵਾਪਸ ਨਿਰਾ;

ਮਿਲਿਆ_ਰਸਤਾ:
    {} /* Label followed by a declaration is a C23 extension */
    ਅਚਲ bool ok = parse_file(ਸਾਰਣੀ, file, ਰਸਤਾ);
    fclose(file);
    ਜੇ (!ok) {
        ਮੁਕਤ(ਰਸਤਾ);
        xkb_compose_ਸਾਰਣੀ_unref(ਸਾਰਣੀ);
        ਵਾਪਸ ਨਿਰਾ;
    }

    log_dbg(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "created compose table from locale %s with path %s\n",
            ਸਾਰਣੀ->locale, ਰਸਤਾ);

    ਮੁਕਤ(ਰਸਤਾ);
    ਵਾਪਸ ਸਾਰਣੀ;
}

ਅਚਲ xkb_keysym_t *
xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_sequence(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
                                 ਮਾਪ_t *sequence_ਲੰਮਾਈ)
{
    *sequence_ਲੰਮਾਈ = ਇੰਦਰਾਜ->sequence_ਲੰਮਾਈ;
    ਵਾਪਸ ਇੰਦਰਾਜ->sequence;
}

xkb_keysym_t
xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_keysym(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਵਾਪਸ ਇੰਦਰਾਜ->keysym;
}

ਅਚਲ ਅੱਖਰ *
xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ_utf8(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਵਾਪਸ ਇੰਦਰਾਜ->utf8;
}

#ਜੇ MAX_COMPOSE_NODES_LOG2 > 31
#ਗਲਤੀ "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#ਜੇ_ਅੰਤ

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator_pending_ਗੰਢ {
    uint32_t ਵਿੱਥ:31;
    bool processed:1;
};

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator {
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ;
    /* Current entry */
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
    /* Stack of pending nodes to process */
    darray(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator_pending_ਗੰਢ) pending_nodes;
};

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator *
xkb_compose_ਸਾਰਣੀ_iterator_new(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator *iter;
    xkb_keysym_t *sequence;

    iter = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*iter));
    ਜੇ (!iter) {
        ਵਾਪਸ ਨਿਰਾ;
    }
    iter->ਸਾਰਣੀ = xkb_compose_ਸਾਰਣੀ_ref(ਸਾਰਣੀ);
    sequence = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(COMPOSE_MAX_LHS_LEN, ਆਕਾਰ(xkb_keysym_t));
    ਜੇ (!sequence) {
        ਮੁਕਤ(iter);
        ਵਾਪਸ ਨਿਰਾ;
    }
    iter->ਇੰਦਰਾਜ.sequence = sequence;
    iter->ਇੰਦਰਾਜ.sequence_ਲੰਮਾਈ = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ਜੇ (darray_ਮਾਪ(ਸਾਰਣੀ->nodes) == 1) {
        ਵਾਪਸ iter;
    }
    /* Add root node */
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator_pending_ਗੰਢ pending = {
        .ਵਿੱਥ = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes,
                                                   pending.ਵਿੱਥ);

    /* Find the first left-most node and store intermediate nodes as pending */
    ਜਦੋਂ (ਗੰਢ->lokid) {
        pending.ਵਿੱਥ = ਗੰਢ->lokid;
        darray_append(iter->pending_nodes, pending);
        ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes, pending.ਵਿੱਥ);
    };

    ਵਾਪਸ iter;
}

ਖਾਲੀ
xkb_compose_ਸਾਰਣੀ_iterator_free(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator *iter)
{
    xkb_compose_ਸਾਰਣੀ_unref(iter->ਸਾਰਣੀ);
    darray_free(iter->pending_nodes);
    ਮੁਕਤ(iter->ਇੰਦਰਾਜ.sequence);
    ਮੁਕਤ(iter);
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_ਇੰਦਰਾਜ *
xkb_compose_ਸਾਰਣੀ_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator *iter)
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

    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator_pending_ਗੰਢ *pending;
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ;

    /* Iterator is empty if there is no pending nodes */
    ਜੇ (unlikely(darray_ਖਾਲੀ_ਥਾਵਾਂ(iter->pending_nodes))) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_ਵਸਤੂ(iter->pending_nodes,
                           darray_ਮਾਪ(iter->pending_nodes) - 1);
    ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes, pending->ਵਿੱਥ);

    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ_iterator_pending_ਗੰਢ new = {
        .processed = false,
        .ਵਿੱਥ = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ਜਦੋਂ (pending->processed) {
        /* Remove last keysym */
        iter->ਇੰਦਰਾਜ.sequence_ਲੰਮਾਈ--;
        ਜੇ (ਗੰਢ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->ਵਿੱਥ = ਗੰਢ->hikid;
            /* Process child’s left arrow */
            ਜਾਓ ਗੰਢ_ਖੱਬਾ;
        } ਨਹੀਂਤਾਂ {
            /* Remove processed node */
            darray_remove_ਆਖਰੀ(iter->pending_nodes);
            ਜੇ (unlikely(darray_ਖਾਲੀ_ਥਾਵਾਂ(iter->pending_nodes))) {
                /* No more nodes to process */
                ਵਾਪਸ ਨਿਰਾ;
            }
            /* Get parent node */
            pending = &darray_ਵਸਤੂ(iter->pending_nodes,
                                   darray_ਮਾਪ(iter->pending_nodes) - 1);
            ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes, pending->ਵਿੱਥ);
        }
    }

    ਜਦੋਂ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->ਇੰਦਰਾਜ.sequence_ਲੰਮਾਈ++;
        iter->ਇੰਦਰਾਜ.sequence[iter->ਇੰਦਰਾਜ.sequence_ਲੰਮਾਈ - 1] = ਗੰਢ->keysym;
        ਜੇ (ਗੰਢ->is_ਪੱਤੀ) {
            /* Leaf: return entry */
            iter->ਇੰਦਰਾਜ.keysym = ਗੰਢ->ਪੱਤੀ.keysym;
            iter->ਇੰਦਰਾਜ.utf8 = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->utf8, ਗੰਢ->ਪੱਤੀ.utf8);
            ਵਾਪਸ &iter->ਇੰਦਰਾਜ;
        }
        /* Not a leaf: process child node */
        new.ਵਿੱਥ = ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_ਵਸਤੂ(iter->pending_nodes,
                               darray_ਮਾਪ(iter->pending_nodes) - 1);
ਗੰਢ_ਖੱਬਾ:
        ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes, pending->ਵਿੱਥ);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ਜਦੋਂ (ਗੰਢ->lokid) {
            /* Follow left arrow */
            new.ਵਿੱਥ = ਗੰਢ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_ਵਸਤੂ(iter->pending_nodes,
                                   darray_ਮਾਪ(iter->pending_nodes) - 1);
            ਗੰਢ = &darray_ਵਸਤੂ(iter->ਸਾਰਣੀ->nodes, new.ਵਿੱਥ);
        }
    }
}
