/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    #অন্তর্ভাব <limits.h>
    #যদি HAVE_UNISTD_H && HAVE_DIRENT_H
        #অন্তর্ভাব <dirent.h>
        #অন্তর্ভাব <unistd.h>
    #endif
#endif

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"


/**
 * Append one directory to the context’s include path.
 */
স্থির পূর্ণসংখ্যা
context_include_path_append(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *path)
{
    পূর্ণসংখ্যা err = ENOMEM;

    অক্ষর *tmp = strdup(path);
    যদি (!tmp)
        যাও err;

    কাঠামো stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        যাও err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        যাও err;
    }

    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        যাও err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ফেরত 1;

err:
    যদি (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ফেরত 0;
}

/**
 * Append one directory to the context’s include path.
 */
পূর্ণসংখ্যা
xkb_context_include_path_append(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *path)
{
    ফেরত (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ধ্রুবক অক্ষর *
xkb_context_include_path_get_extra_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর * ধ্রুবক extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ফেরত extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
ধ্রুবক অক্ষর *
xkb_context_include_path_get_unversioned_extensions_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ফেরত ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#অন্যথা
        : ফাঁকা;
#endif
}

ধ্রুবক অক্ষর *
xkb_context_include_path_get_versioned_extensions_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ফেরত ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#অন্যথা
        : ফাঁকা;
#endif
}

স্থির পূর্ণসংখ্যা
compare_str(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
    ফেরত strcmp(*(অক্ষর **)a, *(অক্ষর **) b);
}

স্থির পূর্ণসংখ্যা
add_direct_subdirectories(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *path,
                          darray_string *extensions,
                          darray_আকার_t versioned_গুনতি,
                          আকার_t versioned_path_দৈর্ঘ্য)
{
    পূর্ণসংখ্যা ret = 0;
    পূর্ণসংখ্যা err = ENOMEM;
    DIR *dir = ফাঁকা;

    /* Check extensions parent directory */
    কাঠামো stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        যাও err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        যাও err;
    }
    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        যাও err;
    }

    dir = opendir(path);
    যদি (dir == ফাঁকা) {
        err = EACCES;
        যাও err;
    }

    কাঠামো dirent *entry;
    অক্ষর path_buf[PATH_MAX] = "";
    versioned_path_দৈর্ঘ্য++; /* Additional final ‘/’ */
    যখন ((entry = readdir(dir)) != ফাঁকা) {
        ধ্রুবক অক্ষর *name = entry->d_name;

        /* Skip special entries */
        যদি (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            চালু;

        যদি (!snprintf_safe(path_buf, আকার(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            যাও err;
        }
        যদি (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            চালু;

        /* Skip if the corresponding versioned directory is already included */
        জন্য (darray_আকার_t i = 0; i < versioned_গুনতি; i++) {
            ধ্রুবক অক্ষর * ধ্রুবক পূর্ব_name = darray_item(*extensions, i)
                                         + versioned_path_দৈর্ঘ্য;
            যদি (strcmp(name, পূর্ব_name) == 0)
                যাও পরবর্তী;
        }

        অক্ষর *ext_path = strdup_safe(path_buf);
        যদি (!ext_path) {
            err = ENOMEM;
            যাও err;
        }

        darray_append(*extensions, ext_path);
পরবর্তী:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    যদি (darray_আকার(*extensions) > versioned_গুনতি) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_বস্তুসমূহ(*extensions) + versioned_গুনতি,
              darray_আকার(*extensions) - versioned_গুনতি,
              আকার(*darray_বস্তুসমূহ(*extensions)), &compare_str);
        অক্ষর **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_গুনতি) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ফেরত ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    যদি (dir)
        closedir(dir);

    ফেরত ret;
}
#endif

ধ্রুবক অক্ষর *
xkb_context_include_path_get_system_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর * ধ্রুবক মূল = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ফেরত মূল ? মূল : DFLT_XKB_CONFIG_মূল;
}

/**
 * Append the default include directories to the context.
 */
পূর্ণসংখ্যা
xkb_context_include_path_append_default(কাঠামো xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    অক্ষর *user_path;
    পূর্ণসংখ্যা ret = 0;

    ধ্রুবক অক্ষর * ধ্রুবক home = xkb_context_getenv(ctx, "HOME");
    ধ্রুবক অক্ষর * ধ্রুবক xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    যদি (xdg != ফাঁকা) {
        user_path = asprintf_safe("%s/xkb", xdg);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    } অন্যথা যদি (home != ফাঁকা) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    }

    যদি (home != ফাঁকা) {
        user_path = asprintf_safe("%s/.xkb", home);
        যদি (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            মুক্ত(user_path);
        }
    }

    ধ্রুবক অক্ষর * ধ্রুবক extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ধ্রুবক অক্ষর *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    আকার_t versioned_path_দৈর্ঘ্য = 0;
    যদি (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_দৈর্ঘ্য = strlen(extensions_path);
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
                                         darray_আকার(extensions),
                                         versioned_path_দৈর্ঘ্য);
    }

    অক্ষর **ext_path;
    darray_foreach(ext_path, extensions) {
        মুক্ত(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ধ্রুবক অক্ষর * ধ্রুবক মূল = xkb_context_include_path_get_system_path(ctx);
    ধ্রুবক bool has_মূল = context_include_path_append(ctx, মূল);
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

    ফেরত ret;
}

/**
 * Remove all entries in the context's include path.
 */
শূন্য
xkb_context_include_path_clear(কাঠামো xkb_context *ctx)
{
    অক্ষর **path;

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
পূর্ণসংখ্যা
xkb_context_include_path_reset_defaults(কাঠামো xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ফেরত xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned পূর্ণসংখ্যা
xkb_context_অঙ্ক_include_paths(কাঠামো xkb_context *ctx)
{
    ফেরত (xkb_context_init_includes(ctx))
        ? darray_আকার(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ধ্রুবক অক্ষর *
xkb_context_include_path_get(কাঠামো xkb_context *ctx, unsigned পূর্ণসংখ্যা idx)
{
    যদি (idx >= xkb_context_অঙ্ক_include_paths(ctx))
        ফেরত ফাঁকা;

    ফেরত darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
কাঠামো xkb_context *
xkb_context_ref(কাঠামো xkb_context *ctx)
{
    প্রতিজ্ঞা(ctx->refcnt > 0);
    ctx->refcnt++;
    ফেরত ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
শূন্য
xkb_context_unref(কাঠামো xkb_context *ctx)
{
    প্রতিজ্ঞা(!ctx || ctx->refcnt > 0);
    যদি (!ctx || --ctx->refcnt > 0)
        ফেরত;

    মুক্ত(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    মুক্ত(ctx);
}

স্থির ধ্রুবক অক্ষর *
log_level_to_prefix(গণনা xkb_log_level level)
{
    নির্বাচন (level) {
    ক্ষেত্র XKB_LOG_LEVEL_DEBUG:
        ফেরত "xkbcommon: DEBUG: ";
    ক্ষেত্র XKB_LOG_LEVEL_INFO:
        ফেরত "xkbcommon: INFO: ";
    ক্ষেত্র XKB_LOG_LEVEL_WARNING:
        ফেরত "xkbcommon: WARNING: ";
    ক্ষেত্র XKB_LOG_LEVEL_ERROR:
        ফেরত "xkbcommon: ERROR: ";
    ক্ষেত্র XKB_LOG_LEVEL_CRITICAL:
        ফেরত "xkbcommon: CRITICAL: ";
    পূর্বনির্ধারিত:
        ফেরত ফাঁকা;
    }
}

ATTR_PRINTF(3, 0) স্থির শূন্য
default_log_fn(কাঠামো xkb_context *ctx, গণনা xkb_log_level level,
               ধ্রুবক অক্ষর *fmt, va_list args)
{
    ধ্রুবক অক্ষর *prefix = log_level_to_prefix(level);

    যদি (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

স্থির গণনা xkb_log_level
log_level(ধ্রুবক অক্ষর *level) {
    errno = 0;
    অক্ষর *endptr = ফাঁকা;
    ধ্রুবক long lvl = strtol(level, &endptr, 10);
    যদি (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ফেরত (গণনা xkb_log_level)lvl;
    যদি (istreq_prefix("crit", level))
        ফেরত XKB_LOG_LEVEL_CRITICAL;
    যদি (istreq_prefix("err", level))
        ফেরত XKB_LOG_LEVEL_ERROR;
    যদি (istreq_prefix("warn", level))
        ফেরত XKB_LOG_LEVEL_WARNING;
    যদি (istreq_prefix("info", level))
        ফেরত XKB_LOG_LEVEL_INFO;
    যদি (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ফেরত XKB_LOG_LEVEL_DEBUG;

    ফেরত XKB_LOG_LEVEL_ERROR;
}

স্থির পূর্ণসংখ্যা
log_verbosity(ধ্রুবক অক্ষর *verbosity) {
    errno = 0;
    ধ্রুবক long পূর্ণসংখ্যা v = strtol(verbosity, ফাঁকা, 10);
    যদি (errno == 0) {
        /* Truncate */
        ফেরত (পূর্ণসংখ্যা) v;
    }

    ফেরত XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
কাঠামো xkb_context *
xkb_context_new(গণনা xkb_context_flags flags)
{
    ধ্রুবক অক্ষর *env;
    কাঠামো xkb_context *ctx = calloc(1, আকার(*ctx));

    যদি (!ctx)
        ফেরত ফাঁকা;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ধ্রুবক গণনা xkb_context_flags invalid_flags =
        (flags & ~(গণনা xkb_context_flags)XKB_CONTEXT_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        মুক্ত(ctx);
        ফেরত ফাঁকা;
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
        ফেরত ফাঁকা;
    }

    ctx->x11_atom_cache = ফাঁকা;

    ফেরত ctx;
}

শূন্য
xkb_context_set_log_fn(কাঠামো xkb_context *ctx,
                       শূন্য (*log_fn)(কাঠামো xkb_context *ctx,
                                      গণনা xkb_log_level level,
                                      ধ্রুবক অক্ষর *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

গণনা xkb_log_level
xkb_context_get_log_level(কাঠামো xkb_context *ctx)
{
    ফেরত ctx->log_level;
}

শূন্য
xkb_context_set_log_level(কাঠামো xkb_context *ctx, গণনা xkb_log_level level)
{
    ctx->log_level = level;
}

পূর্ণসংখ্যা
xkb_context_get_log_verbosity(কাঠামো xkb_context *ctx)
{
    ফেরত ctx->log_verbosity;
}

শূন্য
xkb_context_set_log_verbosity(কাঠামো xkb_context *ctx, পূর্ণসংখ্যা verbosity)
{
    ctx->log_verbosity = verbosity;
}

শূন্য *
xkb_context_get_user_তথ্য(কাঠামো xkb_context *ctx)
{
    যদি (ctx)
        ফেরত ctx->user_তথ্য;
    ফেরত ফাঁকা;
}

শূন্য
xkb_context_set_user_তথ্য(কাঠামো xkb_context *ctx, শূন্য *user_তথ্য)
{
    ctx->user_তথ্য = user_তথ্য;
}
