/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "constants.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "paths.h"
#अन्तर्भाव "table.h"
#अन्तर्भाव "utils.h"

स्थिर संरचना xkb_compose_table *
xkb_compose_table_new(संरचना xkb_context *ctx, ध्रुव वर्ण *func,
                      ध्रुव वर्ण *locale,
                      गणना xkb_compose_format format,
                      गणना xkb_compose_compile_flags flags)
{
    ध्रुव गणना xkb_compose_compile_flags invalid_flags = (
        flags & ~(गणना xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_मूल्ये
    );
    यदि (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        प्रत्यागम रिक्त;
    }

    यदि (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        प्रत्यागम रिक्त;
    }

    वर्ण *resolved_locale;
    संरचना xkb_compose_table *table;
    संरचना compose_गाठ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    यदि (!resolved_locale)
        प्रत्यागम रिक्त;

    table = calloc(1, परिमाण(*table));
    यदि (!table) {
        मुक्त(resolved_locale);
        प्रत्यागम रिक्त;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_किल्ली_No_चिन्ह;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_किल्ली_No_चिन्ह;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    प्रत्यागम table;
}

संरचना xkb_compose_table *
xkb_compose_table_ref(संरचना xkb_compose_table *table)
{
    प्रतिज्ञा(table->refcnt > 0);
    table->refcnt++;
    प्रत्यागम table;
}

शून्य
xkb_compose_table_unref(संरचना xkb_compose_table *table)
{
    प्रतिज्ञा(!table || table->refcnt > 0);
    यदि (!table || --table->refcnt > 0)
        प्रत्यागम;
    मुक्त(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    मुक्त(table);
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_file(संरचना xkb_context *ctx,
                                FILE *file,
                                ध्रुव वर्ण *locale,
                                गणना xkb_compose_format format,
                                गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_table * ध्रुव table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    यदि (!table)
        प्रत्यागम रिक्त;

    यदि (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम table;
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_buffer(संरचना xkb_context *ctx,
                                  ध्रुव वर्ण *buffer, size_t length,
                                  ध्रुव वर्ण *locale,
                                  गणना xkb_compose_format format,
                                  गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_table * ध्रुव table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    यदि (!table)
        प्रत्यागम रिक्त;

    यदि (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम table;
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_locale(संरचना xkb_context *ctx,
                                  ध्रुव वर्ण *locale,
                                  गणना xkb_compose_compile_flags flags)
{
    स्थिर ध्रुव गणना xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    संरचना xkb_compose_table * ध्रुव table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    यदि (!table)
        प्रत्यागम रिक्त;

    वर्ण *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    यदि (file)
        गच्छ found_path;
    मुक्त(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    यदि (file)
        गच्छ found_path;
    मुक्त(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    यदि (file)
        गच्छ found_path;
    मुक्त(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    यदि (file)
        गच्छ found_path;
    मुक्त(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    प्रत्यागम रिक्त;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ध्रुव bool ok = parse_file(table, file, path);
    fclose(file);
    यदि (!ok) {
        मुक्त(path);
        xkb_compose_table_unref(table);
        प्रत्यागम रिक्त;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    मुक्त(path);
    प्रत्यागम table;
}

ध्रुव xkb_keysym_t *
xkb_compose_table_entry_sequence(संरचना xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    प्रत्यागम entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(संरचना xkb_compose_table_entry *entry)
{
    प्रत्यागम entry->keysym;
}

ध्रुव वर्ण *
xkb_compose_table_entry_utf8(संरचना xkb_compose_table_entry *entry)
{
    प्रत्यागम entry->utf8;
}

#यदि MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

संरचना xkb_compose_table_iterator_pending_गाठ {
    uint32_t offset:31;
    bool processed:1;
};

संरचना xkb_compose_table_iterator {
    संरचना xkb_compose_table *table;
    /* Current entry */
    संरचना xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(संरचना xkb_compose_table_iterator_pending_गाठ) pending_nodes;
};

संरचना xkb_compose_table_iterator *
xkb_compose_table_iterator_new(संरचना xkb_compose_table *table)
{
    संरचना xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, परिमाण(*iter));
    यदि (!iter) {
        प्रत्यागम रिक्त;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, परिमाण(xkb_keysym_t));
    यदि (!sequence) {
        मुक्त(iter);
        प्रत्यागम रिक्त;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    यदि (darray_size(table->nodes) == 1) {
        प्रत्यागम iter;
    }
    /* Add root node */
    संरचना xkb_compose_table_iterator_pending_गाठ pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ध्रुव संरचना compose_गाठ *गाठ = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    यावत् (गाठ->lokid) {
        pending.offset = गाठ->lokid;
        darray_append(iter->pending_nodes, pending);
        गाठ = &darray_item(iter->table->nodes, pending.offset);
    };

    प्रत्यागम iter;
}

शून्य
xkb_compose_table_iterator_free(संरचना xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    मुक्त(iter->entry.sequence);
    मुक्त(iter);
}

संरचना xkb_compose_table_entry *
xkb_compose_table_iterator_next(संरचना xkb_compose_table_iterator *iter)
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

    संरचना xkb_compose_table_iterator_pending_गाठ *pending;
    ध्रुव संरचना compose_गाठ *गाठ;

    /* Iterator is empty if there is no pending nodes */
    यदि (unlikely(darray_empty(iter->pending_nodes))) {
        प्रत्यागम रिक्त;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    गाठ = &darray_item(iter->table->nodes, pending->offset);

    संरचना xkb_compose_table_iterator_pending_गाठ new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    यावत् (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        यदि (गाठ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = गाठ->hikid;
            /* Process child’s left arrow */
            गच्छ गाठ_डावा;
        } अन्यथा {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            यदि (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                प्रत्यागम रिक्त;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            गाठ = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    यावत् (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = गाठ->keysym;
        यदि (गाठ->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = गाठ->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, गाठ->leaf.utf8);
            प्रत्यागम &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = गाठ->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
गाठ_डावा:
        गाठ = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        यावत् (गाठ->lokid) {
            /* Follow left arrow */
            new.offset = गाठ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            गाठ = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
