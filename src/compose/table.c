/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "constants.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "paths.h"
#அந்தர்பாவ "table.h"
#அந்தர்பாவ "utils.h"

நிலையான கட்டமைப்பு xkb_compose_table *
xkb_compose_table_new(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *func,
                      மாறிலி எழுத்து *locale,
                      எண்ணிக்கை xkb_compose_format format,
                      எண்ணிக்கை xkb_compose_compile_flags flags)
{
    மாறிலி எண்ணிக்கை xkb_compose_compile_flags invalid_flags = (
        flags & ~(எண்ணிக்கை xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_மதிப்புகள்
    );
    எனில் (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        திரும்பு வெற்று;
    }

    எனில் (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        திரும்பு வெற்று;
    }

    எழுத்து *resolved_locale;
    கட்டமைப்பு xkb_compose_table *table;
    கட்டமைப்பு compose_முனை dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    எனில் (!resolved_locale)
        திரும்பு வெற்று;

    table = calloc(1, அளவு(*table));
    எனில் (!table) {
        விடுவி(resolved_locale);
        திரும்பு வெற்று;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_சாவி_No_குறியீடு;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_சாவி_No_குறியீடு;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    திரும்பு table;
}

கட்டமைப்பு xkb_compose_table *
xkb_compose_table_ref(கட்டமைப்பு xkb_compose_table *table)
{
    ப்ரதிஜ்ஞா(table->refcnt > 0);
    table->refcnt++;
    திரும்பு table;
}

வெறுமை
xkb_compose_table_unref(கட்டமைப்பு xkb_compose_table *table)
{
    ப்ரதிஜ்ஞா(!table || table->refcnt > 0);
    எனில் (!table || --table->refcnt > 0)
        திரும்பு;
    விடுவி(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    விடுவி(table);
}

கட்டமைப்பு xkb_compose_table *
xkb_compose_table_new_from_file(கட்டமைப்பு xkb_context *ctx,
                                FILE *file,
                                மாறிலி எழுத்து *locale,
                                எண்ணிக்கை xkb_compose_format format,
                                எண்ணிக்கை xkb_compose_compile_flags flags)
{
    கட்டமைப்பு xkb_compose_table * மாறிலி table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    எனில் (!table)
        திரும்பு வெற்று;

    எனில் (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        திரும்பு வெற்று;
    }

    திரும்பு table;
}

கட்டமைப்பு xkb_compose_table *
xkb_compose_table_new_from_buffer(கட்டமைப்பு xkb_context *ctx,
                                  மாறிலி எழுத்து *buffer, size_t length,
                                  மாறிலி எழுத்து *locale,
                                  எண்ணிக்கை xkb_compose_format format,
                                  எண்ணிக்கை xkb_compose_compile_flags flags)
{
    கட்டமைப்பு xkb_compose_table * மாறிலி table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    எனில் (!table)
        திரும்பு வெற்று;

    எனில் (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        திரும்பு வெற்று;
    }

    திரும்பு table;
}

கட்டமைப்பு xkb_compose_table *
xkb_compose_table_new_from_locale(கட்டமைப்பு xkb_context *ctx,
                                  மாறிலி எழுத்து *locale,
                                  எண்ணிக்கை xkb_compose_compile_flags flags)
{
    நிலையான மாறிலி எண்ணிக்கை xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    கட்டமைப்பு xkb_compose_table * மாறிலி table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    எனில் (!table)
        திரும்பு வெற்று;

    எழுத்து *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    எனில் (file)
        செல் found_path;
    விடுவி(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    எனில் (file)
        செல் found_path;
    விடுவி(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    எனில் (file)
        செல் found_path;
    விடுவி(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    எனில் (file)
        செல் found_path;
    விடுவி(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    திரும்பு வெற்று;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    மாறிலி bool ok = parse_file(table, file, path);
    fclose(file);
    எனில் (!ok) {
        விடுவி(path);
        xkb_compose_table_unref(table);
        திரும்பு வெற்று;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    விடுவி(path);
    திரும்பு table;
}

மாறிலி xkb_keysym_t *
xkb_compose_table_entry_sequence(கட்டமைப்பு xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    திரும்பு entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(கட்டமைப்பு xkb_compose_table_entry *entry)
{
    திரும்பு entry->keysym;
}

மாறிலி எழுத்து *
xkb_compose_table_entry_utf8(கட்டமைப்பு xkb_compose_table_entry *entry)
{
    திரும்பு entry->utf8;
}

#எனில் MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

கட்டமைப்பு xkb_compose_table_iterator_pending_முனை {
    uint32_t offset:31;
    bool processed:1;
};

கட்டமைப்பு xkb_compose_table_iterator {
    கட்டமைப்பு xkb_compose_table *table;
    /* Current entry */
    கட்டமைப்பு xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(கட்டமைப்பு xkb_compose_table_iterator_pending_முனை) pending_nodes;
};

கட்டமைப்பு xkb_compose_table_iterator *
xkb_compose_table_iterator_new(கட்டமைப்பு xkb_compose_table *table)
{
    கட்டமைப்பு xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, அளவு(*iter));
    எனில் (!iter) {
        திரும்பு வெற்று;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, அளவு(xkb_keysym_t));
    எனில் (!sequence) {
        விடுவி(iter);
        திரும்பு வெற்று;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    எனில் (darray_size(table->nodes) == 1) {
        திரும்பு iter;
    }
    /* Add root node */
    கட்டமைப்பு xkb_compose_table_iterator_pending_முனை pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    மாறிலி கட்டமைப்பு compose_முனை *முனை = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    போது (முனை->lokid) {
        pending.offset = முனை->lokid;
        darray_append(iter->pending_nodes, pending);
        முனை = &darray_item(iter->table->nodes, pending.offset);
    };

    திரும்பு iter;
}

வெறுமை
xkb_compose_table_iterator_free(கட்டமைப்பு xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    விடுவி(iter->entry.sequence);
    விடுவி(iter);
}

கட்டமைப்பு xkb_compose_table_entry *
xkb_compose_table_iterator_next(கட்டமைப்பு xkb_compose_table_iterator *iter)
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

    கட்டமைப்பு xkb_compose_table_iterator_pending_முனை *pending;
    மாறிலி கட்டமைப்பு compose_முனை *முனை;

    /* Iterator is empty if there is no pending nodes */
    எனில் (unlikely(darray_empty(iter->pending_nodes))) {
        திரும்பு வெற்று;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    முனை = &darray_item(iter->table->nodes, pending->offset);

    கட்டமைப்பு xkb_compose_table_iterator_pending_முனை new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    போது (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        எனில் (முனை->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = முனை->hikid;
            /* Process child’s left arrow */
            செல் முனை_இடது;
        } இல்லையெனில் {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            எனில் (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                திரும்பு வெற்று;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            முனை = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    போது (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = முனை->keysym;
        எனில் (முனை->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = முனை->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, முனை->leaf.utf8);
            திரும்பு &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = முனை->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
முனை_இடது:
        முனை = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        போது (முனை->lokid) {
            /* Follow left arrow */
            new.offset = முனை->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            முனை = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
