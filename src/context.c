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

#ຖ້າ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #include <limits.h>
    #ຖ້າ HAVE_UNISTD_H && HAVE_DIRENT_H
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
static ຈຳນວນເຕັມ
context_include_path_append(sa_nracanaa xkb_context *ctx, const var_na *path)
{
    ຈຳນວນເຕັມ err = ENOMEM;

    var_na *tmp = strdup(path);
    ຖ້າ (!tmp)
        goto err;

    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ຖ້າ (err != 0) {
        err = errno;
        goto err;
    }
    ຖ້າ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }

    ຖ້າ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ສົ່ງຄືນ 1;

err:
    ຖ້າ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ສົ່ງຄືນ 0;
}

/**
 * Append one directory to the context’s include path.
 */
ຈຳນວນເຕັມ
xkb_context_include_path_append(sa_nracanaa xkb_context *ctx, const var_na *path)
{
    ສົ່ງຄືນ (xkb_context_init_includes(ctx))
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
    ສົ່ງຄືນ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#ຖ້າ HAVE_XKB_EXTENSIONS_DIRECTORIES
const var_na *
xkb_context_include_path_get_unversioned_extensions_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ສົ່ງຄືນ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#ບໍ່ດັ່ງນັ້ນ
        : NULL;
#endif
}

const var_na *
xkb_context_include_path_get_versioned_extensions_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ສົ່ງຄືນ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#ບໍ່ດັ່ງນັ້ນ
        : NULL;
#endif
}

static ຈຳນວນເຕັມ
compare_str(const void *a, const void *b)
{
    ສົ່ງຄືນ strcmp(*(var_na **)a, *(var_na **) b);
}

static ຈຳນວນເຕັມ
add_direct_subdirectories(sa_nracanaa xkb_context *ctx, const var_na *path,
                          darray_string *extensions,
                          darray_size_t versioned_count,
                          size_t versioned_path_length)
{
    ຈຳນວນເຕັມ ret = 0;
    ຈຳນວນເຕັມ err = ENOMEM;
    DIR *dir = NULL;

    /* Check extensions parent directory */
    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ຖ້າ (err != 0) {
        err = errno;
        goto err;
    }
    ຖ້າ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }
    ຖ້າ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    dir = opendir(path);
    ຖ້າ (dir == NULL) {
        err = EACCES;
        goto err;
    }

    sa_nracanaa dirent *entry;
    var_na path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ໃນຂະນະທີ່ ((entry = readdir(dir)) != NULL) {
        const var_na *name = entry->d_name;

        /* Skip special entries */
        ຖ້າ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        ຖ້າ (!snprintf_safe(path_buf, parimaa_na(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            goto err;
        }
        ຖ້າ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            continue;

        /* Skip if the corresponding versioned directory is already included */
        ສຳລັບ (darray_size_t i = 0; i < versioned_count; i++) {
            const var_na * const prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ຖ້າ (strcmp(name, prev_name) == 0)
                goto next;
        }

        var_na *ext_path = strdup_safe(path_buf);
        ຖ້າ (!ext_path) {
            err = ENOMEM;
            goto err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ຖ້າ (darray_size(*extensions) > versioned_count) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_items(*extensions) + versioned_count,
              darray_size(*extensions) - versioned_count,
              parimaa_na(*darray_items(*extensions)), &compare_str);
        var_na **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_count) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ສົ່ງຄືນ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ຖ້າ (dir)
        closedir(dir);

    ສົ່ງຄືນ ret;
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
    ສົ່ງຄືນ root ? root : DFLT_XKB_CONFIG_ROOT;
}

/**
 * Append the default include directories to the context.
 */
ຈຳນວນເຕັມ
xkb_context_include_path_append_default(sa_nracanaa xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    var_na *user_path;
    ຈຳນວນເຕັມ ret = 0;

    const var_na * const home = xkb_context_getenv(ctx, "HOME");
    const var_na * const xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ຖ້າ (xdg != NULL) {
        user_path = asprintf_safe("%s/xkb", xdg);
        ຖ້າ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (home != NULL) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        ຖ້າ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    ຖ້າ (home != NULL) {
        user_path = asprintf_safe("%s/.xkb", home);
        ຖ້າ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            free(user_path);
        }
    }

    const var_na * const extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#ຖ້າ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    const var_na *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    ຖ້າ (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    ຖ້າ (extensions_path) {
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
    ຖ້າ (!has_root && root[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ROOT, root);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ROOT);
    }

    ສົ່ງຄືນ ret;
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
ຈຳນວນເຕັມ
xkb_context_include_path_reset_defaults(sa_nracanaa xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ສົ່ງຄືນ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned ຈຳນວນເຕັມ
xkb_context_num_include_paths(sa_nracanaa xkb_context *ctx)
{
    ສົ່ງຄືນ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
const var_na *
xkb_context_include_path_get(sa_nracanaa xkb_context *ctx, unsigned ຈຳນວນເຕັມ idx)
{
    ຖ້າ (idx >= xkb_context_num_include_paths(ctx))
        ສົ່ງຄືນ NULL;

    ສົ່ງຄືນ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
sa_nracanaa xkb_context *
xkb_context_ref(sa_nracanaa xkb_context *ctx)
{
    assert(ctx->refcnt > 0);
    ctx->refcnt++;
    ສົ່ງຄືນ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
void
xkb_context_unref(sa_nracanaa xkb_context *ctx)
{
    assert(!ctx || ctx->refcnt > 0);
    ຖ້າ (!ctx || --ctx->refcnt > 0)
        ສົ່ງຄືນ;

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
        ສົ່ງຄືນ "xkbcommon: DEBUG: ";
    pak_sa XKB_LOG_LEVEL_INFO:
        ສົ່ງຄືນ "xkbcommon: INFO: ";
    pak_sa XKB_LOG_LEVEL_WARNING:
        ສົ່ງຄືນ "xkbcommon: WARNING: ";
    pak_sa XKB_LOG_LEVEL_ERROR:
        ສົ່ງຄືນ "xkbcommon: ERROR: ";
    pak_sa XKB_LOG_LEVEL_CRITICAL:
        ສົ່ງຄືນ "xkbcommon: CRITICAL: ";
    default:
        ສົ່ງຄືນ NULL;
    }
}

ATTR_PRINTF(3, 0) static void
default_log_fn(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level,
               const var_na *fmt, va_list args)
{
    const var_na *prefix = log_level_to_prefix(level);

    ຖ້າ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

static ga_nanaa xkb_log_level
log_level(const var_na *level) {
    errno = 0;
    var_na *endptr = NULL;
    const long lvl = strtol(level, &endptr, 10);
    ຖ້າ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ສົ່ງຄືນ (ga_nanaa xkb_log_level)lvl;
    ຖ້າ (istreq_prefix("crit", level))
        ສົ່ງຄືນ XKB_LOG_LEVEL_CRITICAL;
    ຖ້າ (istreq_prefix("err", level))
        ສົ່ງຄືນ XKB_LOG_LEVEL_ERROR;
    ຖ້າ (istreq_prefix("warn", level))
        ສົ່ງຄືນ XKB_LOG_LEVEL_WARNING;
    ຖ້າ (istreq_prefix("info", level))
        ສົ່ງຄືນ XKB_LOG_LEVEL_INFO;
    ຖ້າ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ສົ່ງຄືນ XKB_LOG_LEVEL_DEBUG;

    ສົ່ງຄືນ XKB_LOG_LEVEL_ERROR;
}

static ຈຳນວນເຕັມ
log_verbosity(const var_na *verbosity) {
    errno = 0;
    const long ຈຳນວນເຕັມ v = strtol(verbosity, NULL, 10);
    ຖ້າ (errno == 0) {
        /* Truncate */
        ສົ່ງຄືນ (ຈຳນວນເຕັມ) v;
    }

    ສົ່ງຄືນ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
sa_nracanaa xkb_context *
xkb_context_new(ga_nanaa xkb_context_flags flags)
{
    const var_na *env;
    sa_nracanaa xkb_context *ctx = calloc(1, parimaa_na(*ctx));

    ຖ້າ (!ctx)
        ສົ່ງຄືນ NULL;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    const ga_nanaa xkb_context_flags invalid_flags =
        (flags & ~(ga_nanaa xkb_context_flags)XKB_CONTEXT_FLAGS_VALUES);
    ຖ້າ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        free(ctx);
        ສົ່ງຄືນ NULL;
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
    ຖ້າ (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ຖ້າ (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    ຖ້າ (!ctx->atom_table) {
        xkb_context_unref(ctx);
        ສົ່ງຄືນ NULL;
    }

    ctx->x11_atom_cache = NULL;

    ສົ່ງຄືນ ctx;
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
    ສົ່ງຄືນ ctx->log_level;
}

void
xkb_context_set_log_level(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level)
{
    ctx->log_level = level;
}

ຈຳນວນເຕັມ
xkb_context_get_log_verbosity(sa_nracanaa xkb_context *ctx)
{
    ສົ່ງຄືນ ctx->log_verbosity;
}

void
xkb_context_set_log_verbosity(sa_nracanaa xkb_context *ctx, ຈຳນວນເຕັມ verbosity)
{
    ctx->log_verbosity = verbosity;
}

void *
xkb_context_get_user_data(sa_nracanaa xkb_context *ctx)
{
    ຖ້າ (ctx)
        ສົ່ງຄືນ ctx->user_data;
    ສົ່ງຄືນ NULL;
}

void
xkb_context_set_user_data(sa_nracanaa xkb_context *ctx, void *user_data)
{
    ctx->user_data = user_data;
}
