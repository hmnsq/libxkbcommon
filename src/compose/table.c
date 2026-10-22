/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "constants.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "parser.h"
#અન્તર્ભાવ "paths.h"
#અન્તર્ભાવ "table.h"
#અન્તર્ભાવ "utils.h"

સ્થિર સંરચના xkb_compose_table *
xkb_compose_table_new(સંરચના xkb_context *ctx, અચલ અક્ષર *func,
                      અચલ અક્ષર *locale,
                      ગણના xkb_compose_format format,
                      ગણના xkb_compose_compile_flags flags)
{
    અચલ ગણના xkb_compose_compile_flags invalid_flags = (
        flags & ~(ગણના xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_કિંમતો
    );
    જો (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        પાછા ખાલી;
    }

    જો (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        પાછા ખાલી;
    }

    અક્ષર *resolved_locale;
    સંરચના xkb_compose_table *table;
    સંરચના compose_ગાંઠ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    જો (!resolved_locale)
        પાછા ખાલી;

    table = calloc(1, કદ(*table));
    જો (!table) {
        મુક્ત(resolved_locale);
        પાછા ખાલી;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_ચાવી_No_પ્રતીક;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_ચાવી_No_પ્રતીક;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    પાછા table;
}

સંરચના xkb_compose_table *
xkb_compose_table_ref(સંરચના xkb_compose_table *table)
{
    પ્રતિજ્ઞા(table->refcnt > 0);
    table->refcnt++;
    પાછા table;
}

શૂન્ય
xkb_compose_table_unref(સંરચના xkb_compose_table *table)
{
    પ્રતિજ્ઞા(!table || table->refcnt > 0);
    જો (!table || --table->refcnt > 0)
        પાછા;
    મુક્ત(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    મુક્ત(table);
}

સંરચના xkb_compose_table *
xkb_compose_table_new_from_file(સંરચના xkb_context *ctx,
                                FILE *file,
                                અચલ અક્ષર *locale,
                                ગણના xkb_compose_format format,
                                ગણના xkb_compose_compile_flags flags)
{
    સંરચના xkb_compose_table * અચલ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    જો (!table)
        પાછા ખાલી;

    જો (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        પાછા ખાલી;
    }

    પાછા table;
}

સંરચના xkb_compose_table *
xkb_compose_table_new_from_buffer(સંરચના xkb_context *ctx,
                                  અચલ અક્ષર *buffer, size_t length,
                                  અચલ અક્ષર *locale,
                                  ગણના xkb_compose_format format,
                                  ગણના xkb_compose_compile_flags flags)
{
    સંરચના xkb_compose_table * અચલ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    જો (!table)
        પાછા ખાલી;

    જો (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        પાછા ખાલી;
    }

    પાછા table;
}

સંરચના xkb_compose_table *
xkb_compose_table_new_from_locale(સંરચના xkb_context *ctx,
                                  અચલ અક્ષર *locale,
                                  ગણના xkb_compose_compile_flags flags)
{
    સ્થિર અચલ ગણના xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    સંરચના xkb_compose_table * અચલ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    જો (!table)
        પાછા ખાલી;

    અક્ષર *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    જો (file)
        જાઓ found_path;
    મુક્ત(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    જો (file)
        જાઓ found_path;
    મુક્ત(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    જો (file)
        જાઓ found_path;
    મુક્ત(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    જો (file)
        જાઓ found_path;
    મુક્ત(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    પાછા ખાલી;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    અચલ bool ok = parse_file(table, file, path);
    fclose(file);
    જો (!ok) {
        મુક્ત(path);
        xkb_compose_table_unref(table);
        પાછા ખાલી;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    મુક્ત(path);
    પાછા table;
}

અચલ xkb_keysym_t *
xkb_compose_table_entry_sequence(સંરચના xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    પાછા entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(સંરચના xkb_compose_table_entry *entry)
{
    પાછા entry->keysym;
}

અચલ અક્ષર *
xkb_compose_table_entry_utf8(સંરચના xkb_compose_table_entry *entry)
{
    પાછા entry->utf8;
}

#જો MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

સંરચના xkb_compose_table_iterator_pending_ગાંઠ {
    uint32_t offset:31;
    bool processed:1;
};

સંરચના xkb_compose_table_iterator {
    સંરચના xkb_compose_table *table;
    /* Current entry */
    સંરચના xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(સંરચના xkb_compose_table_iterator_pending_ગાંઠ) pending_nodes;
};

સંરચના xkb_compose_table_iterator *
xkb_compose_table_iterator_new(સંરચના xkb_compose_table *table)
{
    સંરચના xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, કદ(*iter));
    જો (!iter) {
        પાછા ખાલી;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, કદ(xkb_keysym_t));
    જો (!sequence) {
        મુક્ત(iter);
        પાછા ખાલી;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    જો (darray_size(table->nodes) == 1) {
        પાછા iter;
    }
    /* Add root node */
    સંરચના xkb_compose_table_iterator_pending_ગાંઠ pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    અચલ સંરચના compose_ગાંઠ *ગાંઠ = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    જ્યારે (ગાંઠ->lokid) {
        pending.offset = ગાંઠ->lokid;
        darray_append(iter->pending_nodes, pending);
        ગાંઠ = &darray_item(iter->table->nodes, pending.offset);
    };

    પાછા iter;
}

શૂન્ય
xkb_compose_table_iterator_free(સંરચના xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    મુક્ત(iter->entry.sequence);
    મુક્ત(iter);
}

સંરચના xkb_compose_table_entry *
xkb_compose_table_iterator_next(સંરચના xkb_compose_table_iterator *iter)
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

    સંરચના xkb_compose_table_iterator_pending_ગાંઠ *pending;
    અચલ સંરચના compose_ગાંઠ *ગાંઠ;

    /* Iterator is empty if there is no pending nodes */
    જો (unlikely(darray_empty(iter->pending_nodes))) {
        પાછા ખાલી;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    ગાંઠ = &darray_item(iter->table->nodes, pending->offset);

    સંરચના xkb_compose_table_iterator_pending_ગાંઠ new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    જ્યારે (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        જો (ગાંઠ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = ગાંઠ->hikid;
            /* Process child’s left arrow */
            જાઓ ગાંઠ_ડાબું;
        } નહીંતર {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            જો (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                પાછા ખાલી;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ગાંઠ = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    જ્યારે (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = ગાંઠ->keysym;
        જો (ગાંઠ->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = ગાંઠ->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, ગાંઠ->leaf.utf8);
            પાછા &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = ગાંઠ->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
ગાંઠ_ડાબું:
        ગાંઠ = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        જ્યારે (ગાંઠ->lokid) {
            /* Follow left arrow */
            new.offset = ગાંઠ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ગાંઠ = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
