/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#nếu HAVE_XKB_EXTENSIONS_DIRECTORIES
    #include <limits.h>
    #nếu HAVE_UNISTD_H && HAVE_DIRENT_H
        #include <dirent.h>
        #include <unistd.h>
    #endif
#endif

#include "xkbcommon/xkbcommon.h"
#include "context.h"
#include "darray.h"
#include "features/enums.h"
#include "messages-codes.h"
#include "utils.h"


/**
 * Append one directory to the context’s include path.
 */
static số_nguyên
context_include_path_append(struct xkb_context *ctx, const char *path)
{
    số_nguyên err = ENOMEM;

    char *tmp = strdup(path);
    nếu (!tmp)
        goto err;

    struct stat stat_buf;
    err = stat(path, &stat_buf);
    nếu (err != 0) {
        err = errno;
        goto err;
    }
    nếu (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }

    nếu (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    trả_về 1;

err:
    nếu (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    trả_về 0;
}

/**
 * Append one directory to the context’s include path.
 */
số_nguyên
xkb_context_include_path_append(struct xkb_context *ctx, const char *path)
{
    trả_về (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

const char *
xkb_context_include_path_get_extra_path(struct xkb_context *ctx)
{
    const char * const extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    trả_về extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#nếu HAVE_XKB_EXTENSIONS_DIRECTORIES
const char *
xkb_context_include_path_get_unversioned_extensions_path(struct xkb_context *ctx)
{
    const char *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    trả_về ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#không_thì
        : NULL;
#endif
}

const char *
xkb_context_include_path_get_versioned_extensions_path(struct xkb_context *ctx)
{
    const char *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    trả_về ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#không_thì
        : NULL;
#endif
}

static số_nguyên
compare_str(const void *a, const void *b)
{
    trả_về strcmp(*(char **)a, *(char **) b);
}

static số_nguyên
add_direct_subdirectories(struct xkb_context *ctx, const char *path,
                          darray_string *extensions,
                          darray_size_t versioned_count,
                          size_t versioned_path_length)
{
    số_nguyên ret = 0;
    số_nguyên err = ENOMEM;
    DIR *dir = NULL;

    /* Check extensions parent directory */
    struct stat stat_buf;
    err = stat(path, &stat_buf);
    nếu (err != 0) {
        err = errno;
        goto err;
    }
    nếu (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }
    nếu (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    dir = opendir(path);
    nếu (dir == NULL) {
        err = EACCES;
        goto err;
    }

    struct dirent *entry;
    char path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    trong_khi ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        /* Skip special entries */
        nếu (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        nếu (!snprintf_safe(path_buf, sizeof(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            goto err;
        }
        nếu (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            continue;

        /* Skip if the corresponding versioned directory is already included */
        cho (darray_size_t i = 0; i < versioned_count; i++) {
            const char * const prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            nếu (strcmp(name, prev_name) == 0)
                goto next;
        }

        char *ext_path = strdup_safe(path_buf);
        nếu (!ext_path) {
            err = ENOMEM;
            goto err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    nếu (darray_size(*extensions) > versioned_count) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_items(*extensions) + versioned_count,
              darray_size(*extensions) - versioned_count,
              sizeof(*darray_items(*extensions)), &compare_str);
        char **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_count) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    trả_về ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    nếu (dir)
        closedir(dir);

    trả_về ret;
}
#endif

const char *
xkb_context_include_path_get_system_path(struct xkb_context *ctx)
{
    const char * const root = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    trả_về root ? root : DFLT_XKB_CONFIG_ROOT;
}

/**
 * Append the default include directories to the context.
 */
số_nguyên
xkb_context_include_path_append_default(struct xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    char *user_path;
    số_nguyên ret = 0;

    const char * const home = xkb_context_getenv(ctx, "HOME");
    const char * const xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    nếu (xdg != NULL) {
        user_path = asprintf_safe("%s/xkb", xdg);
        nếu (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    } không_thì nếu (home != NULL) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        nếu (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    nếu (home != NULL) {
        user_path = asprintf_safe("%s/.xkb", home);
        nếu (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    const char * const extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#nếu HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    const char *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    nếu (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    nếu (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    char **ext_path;
    darray_foreach(ext_path, extensions) {
        free(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    const char * const root = xkb_context_include_path_get_system_path(ctx);
    const bool has_root = context_include_path_append(ctx, root);
    ret |= has_root;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    nếu (!has_root && root[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ROOT, root);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ROOT);
    }

    trả_về ret;
}

/**
 * Remove all entries in the context's include path.
 */
void
xkb_context_include_path_clear(struct xkb_context *ctx)
{
    char **path;

    darray_foreach(path, ctx->includes)
        free(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        free(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
số_nguyên
xkb_context_include_path_reset_defaults(struct xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    trả_về xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned số_nguyên
xkb_context_num_include_paths(struct xkb_context *ctx)
{
    trả_về (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
const char *
xkb_context_include_path_get(struct xkb_context *ctx, unsigned số_nguyên idx)
{
    nếu (idx >= xkb_context_num_include_paths(ctx))
        trả_về NULL;

    trả_về darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
struct xkb_context *
xkb_context_ref(struct xkb_context *ctx)
{
    assert(ctx->refcnt > 0);
    ctx->refcnt++;
    trả_về ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
void
xkb_context_unref(struct xkb_context *ctx)
{
    assert(!ctx || ctx->refcnt > 0);
    nếu (!ctx || --ctx->refcnt > 0)
        trả_về;

    free(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    free(ctx);
}

static const char *
log_level_to_prefix(enum xkb_log_level level)
{
    switch (level) {
    case XKB_LOG_LEVEL_DEBUG:
        trả_về "xkbcommon: DEBUG: ";
    case XKB_LOG_LEVEL_INFO:
        trả_về "xkbcommon: INFO: ";
    case XKB_LOG_LEVEL_WARNING:
        trả_về "xkbcommon: WARNING: ";
    case XKB_LOG_LEVEL_ERROR:
        trả_về "xkbcommon: ERROR: ";
    case XKB_LOG_LEVEL_CRITICAL:
        trả_về "xkbcommon: CRITICAL: ";
    default:
        trả_về NULL;
    }
}

ATTR_PRINTF(3, 0) static void
default_log_fn(struct xkb_context *ctx, enum xkb_log_level level,
               const char *fmt, va_list args)
{
    const char *prefix = log_level_to_prefix(level);

    nếu (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

static enum xkb_log_level
log_level(const char *level) {
    errno = 0;
    char *endptr = NULL;
    const long lvl = strtol(level, &endptr, 10);
    nếu (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        trả_về (enum xkb_log_level)lvl;
    nếu (istreq_prefix("crit", level))
        trả_về XKB_LOG_LEVEL_CRITICAL;
    nếu (istreq_prefix("err", level))
        trả_về XKB_LOG_LEVEL_ERROR;
    nếu (istreq_prefix("warn", level))
        trả_về XKB_LOG_LEVEL_WARNING;
    nếu (istreq_prefix("info", level))
        trả_về XKB_LOG_LEVEL_INFO;
    nếu (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        trả_về XKB_LOG_LEVEL_DEBUG;

    trả_về XKB_LOG_LEVEL_ERROR;
}

static số_nguyên
log_verbosity(const char *verbosity) {
    errno = 0;
    const long số_nguyên v = strtol(verbosity, NULL, 10);
    nếu (errno == 0) {
        /* Truncate */
        trả_về (số_nguyên) v;
    }

    trả_về XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
struct xkb_context *
xkb_context_new(enum xkb_context_flags flags)
{
    const char *env;
    struct xkb_context *ctx = calloc(1, sizeof(*ctx));

    nếu (!ctx)
        trả_về NULL;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    const enum xkb_context_flags invalid_flags =
        (flags & ~(enum xkb_context_flags)XKB_CONTEXT_FLAGS_VALUES);
    nếu (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        free(ctx);
        trả_về NULL;
    }

    ctx->use_environment_names = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_NAMES);
    ctx->use_secure_getenv = !(flags & XKB_CONTEXT_NO_SECURE_GETENV);

    /*
     * Default includes paths are delayed and added only if necessary.
     *
     * It is more efficient for most clients, which only get the keymap from the
     * server: it avoids unnecessary allocations and file system queries.
     *
     * It also avoid the corner case where a containerized app lacks access to
     * the XKB directories.
     *
     * There might be an issue in case the environment variables relevant to the
     * include paths change between the context initialization and the call to
     * `xkb_context_include_path_append_default()`. However this seems very
     * unlikely and would have already triggered issues when the `%`-expansion
     * is used.
     */
    ctx->pending_default_includes = !(flags & XKB_CONTEXT_NO_DEFAULT_INCLUDES);
    darray_init(ctx->includes);
    darray_init(ctx->failed_includes);

    /* Environment overwrites defaults. */
    env = xkb_context_getenv(ctx, "XKB_LOG_LEVEL");
    nếu (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    nếu (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    nếu (!ctx->atom_table) {
        xkb_context_unref(ctx);
        trả_về NULL;
    }

    ctx->x11_atom_cache = NULL;

    trả_về ctx;
}

void
xkb_context_set_log_fn(struct xkb_context *ctx,
                       void (*log_fn)(struct xkb_context *ctx,
                                      enum xkb_log_level level,
                                      const char *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

enum xkb_log_level
xkb_context_get_log_level(struct xkb_context *ctx)
{
    trả_về ctx->log_level;
}

void
xkb_context_set_log_level(struct xkb_context *ctx, enum xkb_log_level level)
{
    ctx->log_level = level;
}

số_nguyên
xkb_context_get_log_verbosity(struct xkb_context *ctx)
{
    trả_về ctx->log_verbosity;
}

void
xkb_context_set_log_verbosity(struct xkb_context *ctx, số_nguyên verbosity)
{
    ctx->log_verbosity = verbosity;
}

void *
xkb_context_get_user_data(struct xkb_context *ctx)
{
    nếu (ctx)
        trả_về ctx->user_data;
    trả_về NULL;
}

void
xkb_context_set_user_data(struct xkb_context *ctx, void *user_data)
{
    ctx->user_data = user_data;
}
