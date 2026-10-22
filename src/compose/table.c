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

ಸ್ಥಿರ ಸಂರಚನಾ xkb_compose_table *
xkb_compose_table_new(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *func,
                      ಧ್ರುವ ವರ್ಣ *locale,
                      ಗಣನಾ xkb_compose_format format,
                      ಗಣನಾ xkb_compose_compile_flags flags)
{
    ಧ್ರುವ ಗಣನಾ xkb_compose_compile_flags invalid_flags = (
        flags & ~(ಗಣನಾ xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು
    );
    ಯದಿ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ವರ್ಣ *resolved_locale;
    ಸಂರಚನಾ xkb_compose_table *table;
    ಸಂರಚನಾ compose_ಗಂಟು dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ಯದಿ (!resolved_locale)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    table = calloc(1, ಪರಿಮಾಣ(*table));
    ಯದಿ (!table) {
        ಮುಕ್ತ(resolved_locale);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    ಪ್ರತ್ಯಾಗಮ table;
}

ಸಂರಚನಾ xkb_compose_table *
xkb_compose_table_ref(ಸಂರಚನಾ xkb_compose_table *table)
{
    ಪ್ರತಿಜ್ಞಾ(table->refcnt > 0);
    table->refcnt++;
    ಪ್ರತ್ಯಾಗಮ table;
}

ಶೂನ್ಯ
xkb_compose_table_unref(ಸಂರಚನಾ xkb_compose_table *table)
{
    ಪ್ರತಿಜ್ಞಾ(!table || table->refcnt > 0);
    ಯದಿ (!table || --table->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;
    ಮುಕ್ತ(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    ಮುಕ್ತ(table);
}

ಸಂರಚನಾ xkb_compose_table *
xkb_compose_table_new_from_file(ಸಂರಚನಾ xkb_context *ctx,
                                FILE *file,
                                ಧ್ರುವ ವರ್ಣ *locale,
                                ಗಣನಾ xkb_compose_format format,
                                ಗಣನಾ xkb_compose_compile_flags flags)
{
    ಸಂರಚನಾ xkb_compose_table * ಧ್ರುವ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ಯದಿ (!table)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ table;
}

ಸಂರಚನಾ xkb_compose_table *
xkb_compose_table_new_from_buffer(ಸಂರಚನಾ xkb_context *ctx,
                                  ಧ್ರುವ ವರ್ಣ *buffer, size_t length,
                                  ಧ್ರುವ ವರ್ಣ *locale,
                                  ಗಣನಾ xkb_compose_format format,
                                  ಗಣನಾ xkb_compose_compile_flags flags)
{
    ಸಂರಚನಾ xkb_compose_table * ಧ್ರುವ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ಯದಿ (!table)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ table;
}

ಸಂರಚನಾ xkb_compose_table *
xkb_compose_table_new_from_locale(ಸಂರಚನಾ xkb_context *ctx,
                                  ಧ್ರುವ ವರ್ಣ *locale,
                                  ಗಣನಾ xkb_compose_compile_flags flags)
{
    ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    ಸಂರಚನಾ xkb_compose_table * ಧ್ರುವ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ಯದಿ (!table)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ವರ್ಣ *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    ಯದಿ (file)
        ಗಚ್ಛ found_path;
    ಮುಕ್ತ(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    ಯದಿ (file)
        ಗಚ್ಛ found_path;
    ಮುಕ್ತ(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    ಯದಿ (file)
        ಗಚ್ಛ found_path;
    ಮುಕ್ತ(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    ಯದಿ (file)
        ಗಚ್ಛ found_path;
    ಮುಕ್ತ(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ಧ್ರುವ bool ok = parse_file(table, file, path);
    fclose(file);
    ಯದಿ (!ok) {
        ಮುಕ್ತ(path);
        xkb_compose_table_unref(table);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    ಮುಕ್ತ(path);
    ಪ್ರತ್ಯಾಗಮ table;
}

ಧ್ರುವ xkb_keysym_t *
xkb_compose_table_entry_sequence(ಸಂರಚನಾ xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    ಪ್ರತ್ಯಾಗಮ entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(ಸಂರಚನಾ xkb_compose_table_entry *entry)
{
    ಪ್ರತ್ಯಾಗಮ entry->keysym;
}

ಧ್ರುವ ವರ್ಣ *
xkb_compose_table_entry_utf8(ಸಂರಚನಾ xkb_compose_table_entry *entry)
{
    ಪ್ರತ್ಯಾಗಮ entry->utf8;
}

#ಯದಿ MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

ಸಂರಚನಾ xkb_compose_table_iterator_pending_ಗಂಟು {
    uint32_t offset:31;
    bool processed:1;
};

ಸಂರಚನಾ xkb_compose_table_iterator {
    ಸಂರಚನಾ xkb_compose_table *table;
    /* Current entry */
    ಸಂರಚನಾ xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(ಸಂರಚನಾ xkb_compose_table_iterator_pending_ಗಂಟು) pending_nodes;
};

ಸಂರಚನಾ xkb_compose_table_iterator *
xkb_compose_table_iterator_new(ಸಂರಚನಾ xkb_compose_table *table)
{
    ಸಂರಚನಾ xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, ಪರಿಮಾಣ(*iter));
    ಯದಿ (!iter) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, ಪರಿಮಾಣ(xkb_keysym_t));
    ಯದಿ (!sequence) {
        ಮುಕ್ತ(iter);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ಯದಿ (darray_size(table->nodes) == 1) {
        ಪ್ರತ್ಯಾಗಮ iter;
    }
    /* Add root node */
    ಸಂರಚನಾ xkb_compose_table_iterator_pending_ಗಂಟು pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    ಯಾವತ್ (ಗಂಟು->lokid) {
        pending.offset = ಗಂಟು->lokid;
        darray_append(iter->pending_nodes, pending);
        ಗಂಟು = &darray_item(iter->table->nodes, pending.offset);
    };

    ಪ್ರತ್ಯಾಗಮ iter;
}

ಶೂನ್ಯ
xkb_compose_table_iterator_free(ಸಂರಚನಾ xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    ಮುಕ್ತ(iter->entry.sequence);
    ಮುಕ್ತ(iter);
}

ಸಂರಚನಾ xkb_compose_table_entry *
xkb_compose_table_iterator_next(ಸಂರಚನಾ xkb_compose_table_iterator *iter)
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

    ಸಂರಚನಾ xkb_compose_table_iterator_pending_ಗಂಟು *pending;
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು;

    /* Iterator is empty if there is no pending nodes */
    ಯದಿ (unlikely(darray_empty(iter->pending_nodes))) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    ಗಂಟು = &darray_item(iter->table->nodes, pending->offset);

    ಸಂರಚನಾ xkb_compose_table_iterator_pending_ಗಂಟು new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ಯಾವತ್ (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        ಯದಿ (ಗಂಟು->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = ಗಂಟು->hikid;
            /* Process child’s left arrow */
            ಗಚ್ಛ ಗಂಟು_ಎಡ;
        } ಅನ್ಯಥಾ {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            ಯದಿ (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ಗಂಟು = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    ಯಾವತ್ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = ಗಂಟು->keysym;
        ಯದಿ (ಗಂಟು->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = ಗಂಟು->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, ಗಂಟು->leaf.utf8);
            ಪ್ರತ್ಯಾಗಮ &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = ಗಂಟು->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
ಗಂಟು_ಎಡ:
        ಗಂಟು = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ಯಾವತ್ (ಗಂಟು->lokid) {
            /* Follow left arrow */
            new.offset = ಗಂಟು->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ಗಂಟು = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
