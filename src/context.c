/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <sys/stat.h>

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    #অন্তর্ভাৱ <limits.h>
    #যদি HAVE_UNISTD_H && HAVE_DIRENT_H
        #অন্তর্ভাৱ <dirent.h>
        #অন্তর্ভাৱ <unistd.h>
    #endif
#endif

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "context.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "features/enums.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
স্থির পূর্ণাঙ্ক
context_include_path_append(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *path)
{
    পূর্ণাঙ্ক err = ENOMEM;

    ৱর্ণ *tmp = strdup(path);
    যদি (!tmp)
        গচ্ছ err;

    সংরচনা stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        গচ্ছ err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        গচ্ছ err;
    }

    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        গচ্ছ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ঘূৰাই 1;

err:
    যদি (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ঘূৰাই 0;
}

/**
 * Append one directory to the context’s include path.
 */
পূর্ণাঙ্ক
xkb_context_include_path_append(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *path)
{
    ঘূৰাই (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_extra_path(সংরচনা xkb_context *ctx)
{
    ধ্রুৱ ৱর্ণ * ধ্রুৱ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ঘূৰাই extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_unversioned_extensions_path(সংরচনা xkb_context *ctx)
{
    ধ্রুৱ ৱর্ণ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ঘূৰাই ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#অন্যথা
        : রিক্ত;
#endif
}

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_versioned_extensions_path(সংরচনা xkb_context *ctx)
{
    ধ্রুৱ ৱর্ণ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ঘূৰাই ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#অন্যথা
        : রিক্ত;
#endif
}

স্থির পূর্ণাঙ্ক
compare_str(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ঘূৰাই strcmp(*(ৱর্ণ **)a, *(ৱর্ণ **) b);
}

স্থির পূর্ণাঙ্ক
add_direct_subdirectories(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *path,
                          darray_string *extensions,
                          darray_size_t versioned_গুনতি,
                          size_t versioned_path_length)
{
    পূর্ণাঙ্ক ret = 0;
    পূর্ণাঙ্ক err = ENOMEM;
    DIR *dir = রিক্ত;

    /* Check extensions parent directory */
    সংরচনা stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        গচ্ছ err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        গচ্ছ err;
    }
    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        গচ্ছ err;
    }

    dir = opendir(path);
    যদি (dir == রিক্ত) {
        err = EACCES;
        গচ্ছ err;
    }

    সংরচনা dirent *entry;
    ৱর্ণ path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    যেতিয়ালৈ ((entry = readdir(dir)) != রিক্ত) {
        ধ্রুৱ ৱর্ণ *name = entry->d_name;

        /* Skip special entries */
        যদি (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            অনুৱর্ত;

        যদি (!snprintf_safe(path_buf, পরিমাণ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            গচ্ছ err;
        }
        যদি (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            অনুৱর্ত;

        /* Skip if the corresponding versioned directory is already included */
        বাবে (darray_size_t i = 0; i < versioned_গুনতি; i++) {
            ধ্রুৱ ৱর্ণ * ধ্রুৱ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            যদি (strcmp(name, prev_name) == 0)
                গচ্ছ next;
        }

        ৱর্ণ *ext_path = strdup_safe(path_buf);
        যদি (!ext_path) {
            err = ENOMEM;
            গচ্ছ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    যদি (darray_size(*extensions) > versioned_গুনতি) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_বস্তুসমূহ(*extensions) + versioned_গুনতি,
              darray_size(*extensions) - versioned_গুনতি,
              পরিমাণ(*darray_বস্তুসমূহ(*extensions)), &compare_str);
        ৱর্ণ **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_গুনতি) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ঘূৰাই ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    যদি (dir)
        closedir(dir);

    ঘূৰাই ret;
}
#endif

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_system_path(সংরচনা xkb_context *ctx)
{
    ধ্রুৱ ৱর্ণ * ধ্রুৱ মূল = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ঘূৰাই মূল ? মূল : DFLT_XKB_CONFIG_মূল;
}

/**
 * Append the default include directories to the context.
 */
পূর্ণাঙ্ক
xkb_context_include_path_append_default(সংরচনা xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    ৱর্ণ *user_path;
    পূর্ণাঙ্ক ret = 0;

    ধ্রুৱ ৱর্ণ * ধ্রুৱ home = xkb_context_getenv(ctx, "HOME");
    ধ্রুৱ ৱর্ণ * ধ্রুৱ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    যদি (xdg != রিক্ত) {
        user_path = asprintf_safe("%s/xkb", xdg);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    } অন্যথা যদি (home != রিক্ত) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    }

    যদি (home != রিক্ত) {
        user_path = asprintf_safe("%s/.xkb", home);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    }

    ধ্রুৱ ৱর্ণ * ধ্রুৱ extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ধ্রুৱ ৱর্ণ *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    যদি (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    যদি (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ৱর্ণ **ext_path;
    darray_foreach(ext_path, extensions) {
        মুক্ত(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ধ্রুৱ ৱর্ণ * ধ্রুৱ মূল = xkb_context_include_path_get_system_path(ctx);
    ধ্রুৱ bool has_মূল = context_include_path_append(ctx, মূল);
    ret |= has_মূল;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    যদি (!has_মূল && মূল[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_মূল, মূল);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_মূল);
    }

    ঘূৰাই ret;
}

/**
 * Remove all entries in the context's include path.
 */
শূন্য
xkb_context_include_path_clear(সংরচনা xkb_context *ctx)
{
    ৱর্ণ **path;

    darray_foreach(path, ctx->includes)
        মুক্ত(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        মুক্ত(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
পূর্ণাঙ্ক
xkb_context_include_path_reset_defaults(সংরচনা xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ঘূৰাই xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned পূর্ণাঙ্ক
xkb_context_অঙ্ক_include_paths(সংরচনা xkb_context *ctx)
{
    ঘূৰাই (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get(সংরচনা xkb_context *ctx, unsigned পূর্ণাঙ্ক idx)
{
    যদি (idx >= xkb_context_অঙ্ক_include_paths(ctx))
        ঘূৰাই রিক্ত;

    ঘূৰাই darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
সংরচনা xkb_context *
xkb_context_ref(সংরচনা xkb_context *ctx)
{
    প্রতিজ্ঞা(ctx->refcnt > 0);
    ctx->refcnt++;
    ঘূৰাই ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
শূন্য
xkb_context_unref(সংরচনা xkb_context *ctx)
{
    প্রতিজ্ঞা(!ctx || ctx->refcnt > 0);
    যদি (!ctx || --ctx->refcnt > 0)
        ঘূৰাই;

    মুক্ত(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    মুক্ত(ctx);
}

স্থির ধ্রুৱ ৱর্ণ *
log_level_to_prefix(গণনা xkb_log_level level)
{
    ৱিকল্প (level) {
    পক্ষ XKB_LOG_LEVEL_DEBUG:
        ঘূৰাই "xkbcommon: DEBUG: ";
    পক্ষ XKB_LOG_LEVEL_INFO:
        ঘূৰাই "xkbcommon: INFO: ";
    পক্ষ XKB_LOG_LEVEL_WARNING:
        ঘূৰাই "xkbcommon: WARNING: ";
    পক্ষ XKB_LOG_LEVEL_ERROR:
        ঘূৰাই "xkbcommon: ERROR: ";
    পক্ষ XKB_LOG_LEVEL_CRITICAL:
        ঘূৰাই "xkbcommon: CRITICAL: ";
    স্ৱভাৱ:
        ঘূৰাই রিক্ত;
    }
}

ATTR_PRINTF(3, 0) স্থির শূন্য
default_log_fn(সংরচনা xkb_context *ctx, গণনা xkb_log_level level,
               ধ্রুৱ ৱর্ণ *fmt, va_list args)
{
    ধ্রুৱ ৱর্ণ *prefix = log_level_to_prefix(level);

    যদি (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

স্থির গণনা xkb_log_level
log_level(ধ্রুৱ ৱর্ণ *level) {
    errno = 0;
    ৱর্ণ *endptr = রিক্ত;
    ধ্রুৱ long lvl = strtol(level, &endptr, 10);
    যদি (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ঘূৰাই (গণনা xkb_log_level)lvl;
    যদি (istreq_prefix("crit", level))
        ঘূৰাই XKB_LOG_LEVEL_CRITICAL;
    যদি (istreq_prefix("err", level))
        ঘূৰাই XKB_LOG_LEVEL_ERROR;
    যদি (istreq_prefix("warn", level))
        ঘূৰাই XKB_LOG_LEVEL_WARNING;
    যদি (istreq_prefix("info", level))
        ঘূৰাই XKB_LOG_LEVEL_INFO;
    যদি (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ঘূৰাই XKB_LOG_LEVEL_DEBUG;

    ঘূৰাই XKB_LOG_LEVEL_ERROR;
}

স্থির পূর্ণাঙ্ক
log_verbosity(ধ্রুৱ ৱর্ণ *verbosity) {
    errno = 0;
    ধ্রুৱ long পূর্ণাঙ্ক v = strtol(verbosity, রিক্ত, 10);
    যদি (errno == 0) {
        /* Truncate */
        ঘূৰাই (পূর্ণাঙ্ক) v;
    }

    ঘূৰাই XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
সংরচনা xkb_context *
xkb_context_new(গণনা xkb_context_flags flags)
{
    ধ্রুৱ ৱর্ণ *env;
    সংরচনা xkb_context *ctx = calloc(1, পরিমাণ(*ctx));

    যদি (!ctx)
        ঘূৰাই রিক্ত;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ধ্রুৱ গণনা xkb_context_flags invalid_flags =
        (flags & ~(গণনা xkb_context_flags)XKB_CONTEXT_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        মুক্ত(ctx);
        ঘূৰাই রিক্ত;
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
    যদি (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    যদি (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    যদি (!ctx->atom_table) {
        xkb_context_unref(ctx);
        ঘূৰাই রিক্ত;
    }

    ctx->x11_atom_cache = রিক্ত;

    ঘূৰাই ctx;
}

শূন্য
xkb_context_set_log_fn(সংরচনা xkb_context *ctx,
                       শূন্য (*log_fn)(সংরচনা xkb_context *ctx,
                                      গণনা xkb_log_level level,
                                      ধ্রুৱ ৱর্ণ *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

গণনা xkb_log_level
xkb_context_get_log_level(সংরচনা xkb_context *ctx)
{
    ঘূৰাই ctx->log_level;
}

শূন্য
xkb_context_set_log_level(সংরচনা xkb_context *ctx, গণনা xkb_log_level level)
{
    ctx->log_level = level;
}

পূর্ণাঙ্ক
xkb_context_get_log_verbosity(সংরচনা xkb_context *ctx)
{
    ঘূৰাই ctx->log_verbosity;
}

শূন্য
xkb_context_set_log_verbosity(সংরচনা xkb_context *ctx, পূর্ণাঙ্ক verbosity)
{
    ctx->log_verbosity = verbosity;
}

শূন্য *
xkb_context_get_user_data(সংরচনা xkb_context *ctx)
{
    যদি (ctx)
        ঘূৰাই ctx->user_data;
    ঘূৰাই রিক্ত;
}

শূন্য
xkb_context_set_user_data(সংরচনা xkb_context *ctx, শূন্য *user_data)
{
    ctx->user_data = user_data;
}
