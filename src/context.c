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

#ᤀᤡ᤺ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #include <limits.h>
    #ᤀᤡ᤺ HAVE_UNISTD_H && HAVE_DIRENT_H
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
static puur_naa_nka
context_include_path_append(sa_nracanaa xkb_context *ctx, const var_na *path)
{
    puur_naa_nka err = ENOMEM;

    var_na *tmp = strdup(path);
    ᤀᤡ᤺ (!tmp)
        goto err;

    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ᤀᤡ᤺ (err != 0) {
        err = errno;
        goto err;
    }
    ᤀᤡ᤺ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }

    ᤀᤡ᤺ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ᤀᤡᤴ 1;

err:
    ᤀᤡ᤺ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ᤀᤡᤴ 0;
}

/**
 * Append one directory to the context’s include path.
 */
puur_naa_nka
xkb_context_include_path_append(sa_nracanaa xkb_context *ctx, const var_na *path)
{
    ᤀᤡᤴ (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

const var_na *
xkb_context_include_path_get_extra_path(sa_nracanaa xkb_context *ctx)
{
    const var_na * const extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ᤀᤡᤴ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#ᤀᤡ᤺ HAVE_XKB_EXTENSIONS_DIRECTORIES
const var_na *
xkb_context_include_path_get_unversioned_extensions_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ᤀᤡᤴ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#ᤀᤡ᤺ᤆᤡ
        : NULL;
#endif
}

const var_na *
xkb_context_include_path_get_versioned_extensions_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ᤀᤡᤴ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#ᤀᤡ᤺ᤆᤡ
        : NULL;
#endif
}

static puur_naa_nka
compare_str(const void *a, const void *b)
{
    ᤀᤡᤴ strcmp(*(var_na **)a, *(var_na **) b);
}

static puur_naa_nka
add_direct_subdirectories(sa_nracanaa xkb_context *ctx, const var_na *path,
                          darray_string *extensions,
                          darray_size_t versioned_count,
                          size_t versioned_path_length)
{
    puur_naa_nka ret = 0;
    puur_naa_nka err = ENOMEM;
    DIR *dir = NULL;

    /* Check extensions parent directory */
    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ᤀᤡ᤺ (err != 0) {
        err = errno;
        goto err;
    }
    ᤀᤡ᤺ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }
    ᤀᤡ᤺ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    dir = opendir(path);
    ᤀᤡ᤺ (dir == NULL) {
        err = EACCES;
        goto err;
    }

    sa_nracanaa dirent *entry;
    var_na path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ᤀᤠ᤺ ((entry = readdir(dir)) != NULL) {
        const var_na *name = entry->d_name;

        /* Skip special entries */
        ᤀᤡ᤺ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        ᤀᤡ᤺ (!snprintf_safe(path_buf, parimaa_na(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            goto err;
        }
        ᤀᤡ᤺ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            continue;

        /* Skip if the corresponding versioned directory is already included */
        for (darray_size_t i = 0; i < versioned_count; i++) {
            const var_na * const prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ᤀᤡ᤺ (strcmp(name, prev_name) == 0)
                goto next;
        }

        var_na *ext_path = strdup_safe(path_buf);
        ᤀᤡ᤺ (!ext_path) {
            err = ENOMEM;
            goto err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ᤀᤡ᤺ (darray_size(*extensions) > versioned_count) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_items(*extensions) + versioned_count,
              darray_size(*extensions) - versioned_count,
              parimaa_na(*darray_items(*extensions)), &compare_str);
        var_na **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_count) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ᤀᤡᤴ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ᤀᤡ᤺ (dir)
        closedir(dir);

    ᤀᤡᤴ ret;
}
#endif

const var_na *
xkb_context_include_path_get_system_path(sa_nracanaa xkb_context *ctx)
{
    const var_na * const root = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ᤀᤡᤴ root ? root : DFLT_XKB_CONFIG_ROOT;
}

/**
 * Append the default include directories to the context.
 */
puur_naa_nka
xkb_context_include_path_append_default(sa_nracanaa xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    var_na *user_path;
    puur_naa_nka ret = 0;

    const var_na * const home = xkb_context_getenv(ctx, "HOME");
    const var_na * const xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ᤀᤡ᤺ (xdg != NULL) {
        user_path = asprintf_safe("%s/xkb", xdg);
        ᤀᤡ᤺ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (home != NULL) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        ᤀᤡ᤺ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    ᤀᤡ᤺ (home != NULL) {
        user_path = asprintf_safe("%s/.xkb", home);
        ᤀᤡ᤺ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    const var_na * const extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#ᤀᤡ᤺ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    const var_na *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    ᤀᤡ᤺ (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    ᤀᤡ᤺ (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    var_na **ext_path;
    darray_foreach(ext_path, extensions) {
        free(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    const var_na * const root = xkb_context_include_path_get_system_path(ctx);
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
    ᤀᤡ᤺ (!has_root && root[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ROOT, root);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ROOT);
    }

    ᤀᤡᤴ ret;
}

/**
 * Remove all entries in the context's include path.
 */
void
xkb_context_include_path_clear(sa_nracanaa xkb_context *ctx)
{
    var_na **path;

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
puur_naa_nka
xkb_context_include_path_reset_defaults(sa_nracanaa xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ᤀᤡᤴ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned puur_naa_nka
xkb_context_num_include_paths(sa_nracanaa xkb_context *ctx)
{
    ᤀᤡᤴ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
const var_na *
xkb_context_include_path_get(sa_nracanaa xkb_context *ctx, unsigned puur_naa_nka idx)
{
    ᤀᤡ᤺ (idx >= xkb_context_num_include_paths(ctx))
        ᤀᤡᤴ NULL;

    ᤀᤡᤴ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
sa_nracanaa xkb_context *
xkb_context_ref(sa_nracanaa xkb_context *ctx)
{
    assert(ctx->refcnt > 0);
    ctx->refcnt++;
    ᤀᤡᤴ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
void
xkb_context_unref(sa_nracanaa xkb_context *ctx)
{
    assert(!ctx || ctx->refcnt > 0);
    ᤀᤡ᤺ (!ctx || --ctx->refcnt > 0)
        ᤀᤡᤴ;

    free(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    free(ctx);
}

static const var_na *
log_level_to_prefix(ga_nanaa xkb_log_level level)
{
    switch (level) {
    pak_sa XKB_LOG_LEVEL_DEBUG:
        ᤀᤡᤴ "xkbcommon: DEBUG: ";
    pak_sa XKB_LOG_LEVEL_INFO:
        ᤀᤡᤴ "xkbcommon: INFO: ";
    pak_sa XKB_LOG_LEVEL_WARNING:
        ᤀᤡᤴ "xkbcommon: WARNING: ";
    pak_sa XKB_LOG_LEVEL_ERROR:
        ᤀᤡᤴ "xkbcommon: ERROR: ";
    pak_sa XKB_LOG_LEVEL_CRITICAL:
        ᤀᤡᤴ "xkbcommon: CRITICAL: ";
    default:
        ᤀᤡᤴ NULL;
    }
}

ATTR_PRINTF(3, 0) static void
default_log_fn(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level,
               const var_na *fmt, va_list args)
{
    const var_na *prefix = log_level_to_prefix(level);

    ᤀᤡ᤺ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

static ga_nanaa xkb_log_level
log_level(const var_na *level) {
    errno = 0;
    var_na *endptr = NULL;
    const long lvl = strtol(level, &endptr, 10);
    ᤀᤡ᤺ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ᤀᤡᤴ (ga_nanaa xkb_log_level)lvl;
    ᤀᤡ᤺ (istreq_prefix("crit", level))
        ᤀᤡᤴ XKB_LOG_LEVEL_CRITICAL;
    ᤀᤡ᤺ (istreq_prefix("err", level))
        ᤀᤡᤴ XKB_LOG_LEVEL_ERROR;
    ᤀᤡ᤺ (istreq_prefix("warn", level))
        ᤀᤡᤴ XKB_LOG_LEVEL_WARNING;
    ᤀᤡ᤺ (istreq_prefix("info", level))
        ᤀᤡᤴ XKB_LOG_LEVEL_INFO;
    ᤀᤡ᤺ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ᤀᤡᤴ XKB_LOG_LEVEL_DEBUG;

    ᤀᤡᤴ XKB_LOG_LEVEL_ERROR;
}

static puur_naa_nka
log_verbosity(const var_na *verbosity) {
    errno = 0;
    const long puur_naa_nka v = strtol(verbosity, NULL, 10);
    ᤀᤡ᤺ (errno == 0) {
        /* Truncate */
        ᤀᤡᤴ (puur_naa_nka) v;
    }

    ᤀᤡᤴ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
sa_nracanaa xkb_context *
xkb_context_new(ga_nanaa xkb_context_flags flags)
{
    const var_na *env;
    sa_nracanaa xkb_context *ctx = calloc(1, parimaa_na(*ctx));

    ᤀᤡ᤺ (!ctx)
        ᤀᤡᤴ NULL;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    const ga_nanaa xkb_context_flags invalid_flags =
        (flags & ~(ga_nanaa xkb_context_flags)XKB_CONTEXT_FLAGS_VALUES);
    ᤀᤡ᤺ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        free(ctx);
        ᤀᤡᤴ NULL;
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
    ᤀᤡ᤺ (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ᤀᤡ᤺ (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    ᤀᤡ᤺ (!ctx->atom_table) {
        xkb_context_unref(ctx);
        ᤀᤡᤴ NULL;
    }

    ctx->x11_atom_cache = NULL;

    ᤀᤡᤴ ctx;
}

void
xkb_context_set_log_fn(sa_nracanaa xkb_context *ctx,
                       void (*log_fn)(sa_nracanaa xkb_context *ctx,
                                      ga_nanaa xkb_log_level level,
                                      const var_na *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ga_nanaa xkb_log_level
xkb_context_get_log_level(sa_nracanaa xkb_context *ctx)
{
    ᤀᤡᤴ ctx->log_level;
}

void
xkb_context_set_log_level(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level)
{
    ctx->log_level = level;
}

puur_naa_nka
xkb_context_get_log_verbosity(sa_nracanaa xkb_context *ctx)
{
    ᤀᤡᤴ ctx->log_verbosity;
}

void
xkb_context_set_log_verbosity(sa_nracanaa xkb_context *ctx, puur_naa_nka verbosity)
{
    ctx->log_verbosity = verbosity;
}

void *
xkb_context_get_user_data(sa_nracanaa xkb_context *ctx)
{
    ᤀᤡ᤺ (ctx)
        ᤀᤡᤴ ctx->user_data;
    ᤀᤡᤴ NULL;
}

void
xkb_context_set_user_data(sa_nracanaa xkb_context *ctx, void *user_data)
{
    ctx->user_data = user_data;
}
