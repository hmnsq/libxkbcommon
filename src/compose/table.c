/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "constants.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "paths.h"
#ଅନ୍ତର୍ଭାଵ "table.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ସ୍ଥିର ସଂରଚନା xkb_compose_table *
xkb_compose_table_new(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *func,
                      ଅଚଳ ଅକ୍ଷର *locale,
                      ଗଣନା xkb_compose_format format,
                      ଗଣନା xkb_compose_compile_flags flags)
{
    ଅଚଳ ଗଣନା xkb_compose_compile_flags invalid_flags = (
        flags & ~(ଗଣନା xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ
    );
    ଯଦି (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଅକ୍ଷର *resolved_locale;
    ସଂରଚନା xkb_compose_table *table;
    ସଂରଚନା compose_ଗଣ୍ଠି dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ଯଦି (!resolved_locale)
        ଫେରନ୍ତୁ ଖାଲି;

    table = calloc(1, ଆକାର(*table));
    ଯଦି (!table) {
        ମୁକ୍ତ(resolved_locale);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_ଚାବି_No_ପ୍ରତୀକ;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_ଚାବି_No_ପ୍ରତୀକ;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    ଫେରନ୍ତୁ table;
}

ସଂରଚନା xkb_compose_table *
xkb_compose_table_ref(ସଂରଚନା xkb_compose_table *table)
{
    ପ୍ରତିଜ୍ଞା(table->refcnt > 0);
    table->refcnt++;
    ଫେରନ୍ତୁ table;
}

ଶୂନ୍ୟ
xkb_compose_table_unref(ସଂରଚନା xkb_compose_table *table)
{
    ପ୍ରତିଜ୍ଞା(!table || table->refcnt > 0);
    ଯଦି (!table || --table->refcnt > 0)
        ଫେରନ୍ତୁ;
    ମୁକ୍ତ(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    ମୁକ୍ତ(table);
}

ସଂରଚନା xkb_compose_table *
xkb_compose_table_new_from_file(ସଂରଚନା xkb_context *ctx,
                                FILE *file,
                                ଅଚଳ ଅକ୍ଷର *locale,
                                ଗଣନା xkb_compose_format format,
                                ଗଣନା xkb_compose_compile_flags flags)
{
    ସଂରଚନା xkb_compose_table * ଅଚଳ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ଯଦି (!table)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ table;
}

ସଂରଚନା xkb_compose_table *
xkb_compose_table_new_from_buffer(ସଂରଚନା xkb_context *ctx,
                                  ଅଚଳ ଅକ୍ଷର *buffer, size_t length,
                                  ଅଚଳ ଅକ୍ଷର *locale,
                                  ଗଣନା xkb_compose_format format,
                                  ଗଣନା xkb_compose_compile_flags flags)
{
    ସଂରଚନା xkb_compose_table * ଅଚଳ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ଯଦି (!table)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ table;
}

ସଂରଚନା xkb_compose_table *
xkb_compose_table_new_from_locale(ସଂରଚନା xkb_context *ctx,
                                  ଅଚଳ ଅକ୍ଷର *locale,
                                  ଗଣନା xkb_compose_compile_flags flags)
{
    ସ୍ଥିର ଅଚଳ ଗଣନା xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    ସଂରଚନା xkb_compose_table * ଅଚଳ table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ଯଦି (!table)
        ଫେରନ୍ତୁ ଖାଲି;

    ଅକ୍ଷର *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    ଯଦି (file)
        ଯାଅ found_path;
    ମୁକ୍ତ(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    ଯଦି (file)
        ଯାଅ found_path;
    ମୁକ୍ତ(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    ଯଦି (file)
        ଯାଅ found_path;
    ମୁକ୍ତ(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    ଯଦି (file)
        ଯାଅ found_path;
    ମୁକ୍ତ(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    ଫେରନ୍ତୁ ଖାଲି;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ଅଚଳ bool ok = parse_file(table, file, path);
    fclose(file);
    ଯଦି (!ok) {
        ମୁକ୍ତ(path);
        xkb_compose_table_unref(table);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    ମୁକ୍ତ(path);
    ଫେରନ୍ତୁ table;
}

ଅଚଳ xkb_keysym_t *
xkb_compose_table_entry_sequence(ସଂରଚନା xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    ଫେରନ୍ତୁ entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(ସଂରଚନା xkb_compose_table_entry *entry)
{
    ଫେରନ୍ତୁ entry->keysym;
}

ଅଚଳ ଅକ୍ଷର *
xkb_compose_table_entry_utf8(ସଂରଚନା xkb_compose_table_entry *entry)
{
    ଫେରନ୍ତୁ entry->utf8;
}

#ଯଦି MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

ସଂରଚନା xkb_compose_table_iterator_pending_ଗଣ୍ଠି {
    uint32_t offset:31;
    bool processed:1;
};

ସଂରଚନା xkb_compose_table_iterator {
    ସଂରଚନା xkb_compose_table *table;
    /* Current entry */
    ସଂରଚନା xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(ସଂରଚନା xkb_compose_table_iterator_pending_ଗଣ୍ଠି) pending_nodes;
};

ସଂରଚନା xkb_compose_table_iterator *
xkb_compose_table_iterator_new(ସଂରଚନା xkb_compose_table *table)
{
    ସଂରଚନା xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, ଆକାର(*iter));
    ଯଦି (!iter) {
        ଫେରନ୍ତୁ ଖାଲି;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, ଆକାର(xkb_keysym_t));
    ଯଦି (!sequence) {
        ମୁକ୍ତ(iter);
        ଫେରନ୍ତୁ ଖାଲି;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ଯଦି (darray_size(table->nodes) == 1) {
        ଫେରନ୍ତୁ iter;
    }
    /* Add root node */
    ସଂରଚନା xkb_compose_table_iterator_pending_ଗଣ୍ଠି pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    ଯେତେବେଳେ (ଗଣ୍ଠି->lokid) {
        pending.offset = ଗଣ୍ଠି->lokid;
        darray_append(iter->pending_nodes, pending);
        ଗଣ୍ଠି = &darray_item(iter->table->nodes, pending.offset);
    };

    ଫେରନ୍ତୁ iter;
}

ଶୂନ୍ୟ
xkb_compose_table_iterator_free(ସଂରଚନା xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    ମୁକ୍ତ(iter->entry.sequence);
    ମୁକ୍ତ(iter);
}

ସଂରଚନା xkb_compose_table_entry *
xkb_compose_table_iterator_next(ସଂରଚନା xkb_compose_table_iterator *iter)
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

    ସଂରଚନା xkb_compose_table_iterator_pending_ଗଣ୍ଠି *pending;
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି;

    /* Iterator is empty if there is no pending nodes */
    ଯଦି (unlikely(darray_empty(iter->pending_nodes))) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    ଗଣ୍ଠି = &darray_item(iter->table->nodes, pending->offset);

    ସଂରଚନା xkb_compose_table_iterator_pending_ଗଣ୍ଠି new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ଯେତେବେଳେ (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        ଯଦି (ଗଣ୍ଠି->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = ଗଣ୍ଠି->hikid;
            /* Process child’s left arrow */
            ଯାଅ ଗଣ୍ଠି_ବାମ;
        } ଅନ୍ୟଥା {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            ଯଦି (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                ଫେରନ୍ତୁ ଖାଲି;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ଗଣ୍ଠି = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    ଯେତେବେଳେ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = ଗଣ୍ଠି->keysym;
        ଯଦି (ଗଣ୍ଠି->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = ଗଣ୍ଠି->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, ଗଣ୍ଠି->leaf.utf8);
            ଫେରନ୍ତୁ &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = ଗଣ୍ଠି->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
ଗଣ୍ଠି_ବାମ:
        ଗଣ୍ଠି = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ଯେତେବେଳେ (ଗଣ୍ଠି->lokid) {
            /* Follow left arrow */
            new.offset = ଗଣ୍ଠି->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            ଗଣ୍ଠି = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
