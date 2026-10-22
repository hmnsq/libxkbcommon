/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ಅನ್ತರ್ಭಾವ <limits.h>
    #ಯದಿ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ಅನ್ತರ್ಭಾವ <dirent.h>
        #ಅನ್ತರ್ಭಾವ <unistd.h>
    #endif
#endif

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
context_include_path_append(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path)
{
    ಪೂರ್ಣಾಙ್ಕ err = ENOMEM;

    ವರ್ಣ *tmp = strdup(path);
    ಯದಿ (!tmp)
        ಗಚ್ಛ err;

    ಸಂರಚನಾ stat stat_buf;
    err = stat(path, &stat_buf);
    ಯದಿ (err != 0) {
        err = errno;
        ಗಚ್ಛ err;
    }
    ಯದಿ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ಗಚ್ಛ err;
    }

    ಯದಿ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ಗಚ್ಛ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ಪ್ರತ್ಯಾಗಮ 1;

err:
    ಯದಿ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Append one directory to the context’s include path.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_context_include_path_append(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path)
{
    ಪ್ರತ್ಯಾಗಮ (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ಧ್ರುವ ವರ್ಣ *
xkb_context_include_path_get_extra_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಪ್ರತ್ಯಾಗಮ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES
ಧ್ರುವ ವರ್ಣ *
xkb_context_include_path_get_unversioned_extensions_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ಪ್ರತ್ಯಾಗಮ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#ಅನ್ಯಥಾ
        : ರಿಕ್ತ;
#endif
}

ಧ್ರುವ ವರ್ಣ *
xkb_context_include_path_get_versioned_extensions_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ಪ್ರತ್ಯಾಗಮ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#ಅನ್ಯಥಾ
        : ರಿಕ್ತ;
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_str(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಪ್ರತ್ಯಾಗಮ strcmp(*(ವರ್ಣ **)a, *(ವರ್ಣ **) b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
add_direct_subdirectories(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path,
                          darray_string *extensions,
                          darray_size_t versioned_ಎಣಿಕೆ,
                          size_t versioned_path_length)
{
    ಪೂರ್ಣಾಙ್ಕ ret = 0;
    ಪೂರ್ಣಾಙ್ಕ err = ENOMEM;
    DIR *dir = ರಿಕ್ತ;

    /* Check extensions parent directory */
    ಸಂರಚನಾ stat stat_buf;
    err = stat(path, &stat_buf);
    ಯದಿ (err != 0) {
        err = errno;
        ಗಚ್ಛ err;
    }
    ಯದಿ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ಗಚ್ಛ err;
    }
    ಯದಿ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ಗಚ್ಛ err;
    }

    dir = opendir(path);
    ಯದಿ (dir == ರಿಕ್ತ) {
        err = EACCES;
        ಗಚ್ಛ err;
    }

    ಸಂರಚನಾ dirent *entry;
    ವರ್ಣ path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ಯಾವತ್ ((entry = readdir(dir)) != ರಿಕ್ತ) {
        ಧ್ರುವ ವರ್ಣ *name = entry->d_name;

        /* Skip special entries */
        ಯದಿ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ಅನುವರ್ತ;

        ಯದಿ (!snprintf_safe(path_buf, ಪರಿಮಾಣ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            ಗಚ್ಛ err;
        }
        ಯದಿ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ಅನುವರ್ತ;

        /* Skip if the corresponding versioned directory is already included */
        ಪ್ರತಿ (darray_size_t i = 0; i < versioned_ಎಣಿಕೆ; i++) {
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ಯದಿ (strcmp(name, prev_name) == 0)
                ಗಚ್ಛ next;
        }

        ವರ್ಣ *ext_path = strdup_safe(path_buf);
        ಯದಿ (!ext_path) {
            err = ENOMEM;
            ಗಚ್ಛ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ಯದಿ (darray_size(*extensions) > versioned_ಎಣಿಕೆ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ವಸ್ತುಗಳು(*extensions) + versioned_ಎಣಿಕೆ,
              darray_size(*extensions) - versioned_ಎಣಿಕೆ,
              ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(*extensions)), &compare_str);
        ವರ್ಣ **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ಎಣಿಕೆ) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ಪ್ರತ್ಯಾಗಮ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ಯದಿ (dir)
        closedir(dir);

    ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

ಧ್ರುವ ವರ್ಣ *
xkb_context_include_path_get_system_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ಬೇರು = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಪ್ರತ್ಯಾಗಮ ಬೇರು ? ಬೇರು : DFLT_XKB_CONFIG_ಬೇರು;
}

/**
 * Append the default include directories to the context.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_context_include_path_append_default(ಸಂರಚನಾ xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    ವರ್ಣ *user_path;
    ಪೂರ್ಣಾಙ್ಕ ret = 0;

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ home = xkb_context_getenv(ctx, "HOME");
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ಯದಿ (xdg != ರಿಕ್ತ) {
        user_path = asprintf_safe("%s/xkb", xdg);
        ಯದಿ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ಮುಕ್ತ(user_path);
        }
    } ಅನ್ಯಥಾ ಯದಿ (home != ರಿಕ್ತ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        ಯದಿ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ಮುಕ್ತ(user_path);
        }
    }

    ಯದಿ (home != ರಿಕ್ತ) {
        user_path = asprintf_safe("%s/.xkb", home);
        ಯದಿ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ಮುಕ್ತ(user_path);
        }
    }

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ಧ್ರುವ ವರ್ಣ *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    ಯದಿ (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    ಯದಿ (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ವರ್ಣ **ext_path;
    darray_foreach(ext_path, extensions) {
        ಮುಕ್ತ(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ಬೇರು = xkb_context_include_path_get_system_path(ctx);
    ಧ್ರುವ bool has_ಬೇರು = context_include_path_append(ctx, ಬೇರು);
    ret |= has_ಬೇರು;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ಯದಿ (!has_ಬೇರು && ಬೇರು[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ಬೇರು, ಬೇರು);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ಬೇರು);
    }

    ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Remove all entries in the context's include path.
 */
ಶೂನ್ಯ
xkb_context_include_path_clear(ಸಂರಚನಾ xkb_context *ctx)
{
    ವರ್ಣ **path;

    darray_foreach(path, ctx->includes)
        ಮುಕ್ತ(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        ಮುಕ್ತ(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_context_include_path_reset_defaults(ಸಂರಚನಾ xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ಪ್ರತ್ಯಾಗಮ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned ಪೂರ್ಣಾಙ್ಕ
xkb_context_ಅಂಕೆ_include_paths(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ಧ್ರುವ ವರ್ಣ *
xkb_context_include_path_get(ಸಂರಚನಾ xkb_context *ctx, unsigned ಪೂರ್ಣಾಙ್ಕ idx)
{
    ಯದಿ (idx >= xkb_context_ಅಂಕೆ_include_paths(ctx))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ಸಂರಚನಾ xkb_context *
xkb_context_ref(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತಿಜ್ಞಾ(ctx->refcnt > 0);
    ctx->refcnt++;
    ಪ್ರತ್ಯಾಗಮ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
ಶೂನ್ಯ
xkb_context_unref(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತಿಜ್ಞಾ(!ctx || ctx->refcnt > 0);
    ಯದಿ (!ctx || --ctx->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    ಮುಕ್ತ(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    ಮುಕ್ತ(ctx);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
log_level_to_prefix(ಗಣನಾ xkb_log_level level)
{
    ವಿಕಲ್ಪ (level) {
    ಪಕ್ಷ XKB_LOG_LEVEL_DEBUG:
        ಪ್ರತ್ಯಾಗಮ "xkbcommon: DEBUG: ";
    ಪಕ್ಷ XKB_LOG_LEVEL_INFO:
        ಪ್ರತ್ಯಾಗಮ "xkbcommon: INFO: ";
    ಪಕ್ಷ XKB_LOG_LEVEL_WARNING:
        ಪ್ರತ್ಯಾಗಮ "xkbcommon: WARNING: ";
    ಪಕ್ಷ XKB_LOG_LEVEL_ERROR:
        ಪ್ರತ್ಯಾಗಮ "xkbcommon: ERROR: ";
    ಪಕ್ಷ XKB_LOG_LEVEL_CRITICAL:
        ಪ್ರತ್ಯಾಗಮ "xkbcommon: CRITICAL: ";
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
}

ATTR_PRINTF(3, 0) ಸ್ಥಿರ ಶೂನ್ಯ
default_log_fn(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_log_level level,
               ಧ್ರುವ ವರ್ಣ *fmt, va_list args)
{
    ಧ್ರುವ ವರ್ಣ *prefix = log_level_to_prefix(level);

    ಯದಿ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ಸ್ಥಿರ ಗಣನಾ xkb_log_level
log_level(ಧ್ರುವ ವರ್ಣ *level) {
    errno = 0;
    ವರ್ಣ *endptr = ರಿಕ್ತ;
    ಧ್ರುವ long lvl = strtol(level, &endptr, 10);
    ಯದಿ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ಪ್ರತ್ಯಾಗಮ (ಗಣನಾ xkb_log_level)lvl;
    ಯದಿ (istreq_prefix("crit", level))
        ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_CRITICAL;
    ಯದಿ (istreq_prefix("err", level))
        ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_ERROR;
    ಯದಿ (istreq_prefix("warn", level))
        ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_WARNING;
    ಯದಿ (istreq_prefix("info", level))
        ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_INFO;
    ಯದಿ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_DEBUG;

    ಪ್ರತ್ಯಾಗಮ XKB_LOG_LEVEL_ERROR;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
log_verbosity(ಧ್ರುವ ವರ್ಣ *verbosity) {
    errno = 0;
    ಧ್ರುವ long ಪೂರ್ಣಾಙ್ಕ v = strtol(verbosity, ರಿಕ್ತ, 10);
    ಯದಿ (errno == 0) {
        /* Truncate */
        ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) v;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ಸಂರಚನಾ xkb_context *
xkb_context_new(ಗಣನಾ xkb_context_flags flags)
{
    ಧ್ರುವ ವರ್ಣ *env;
    ಸಂರಚನಾ xkb_context *ctx = calloc(1, ಪರಿಮಾಣ(*ctx));

    ಯದಿ (!ctx)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ಧ್ರುವ ಗಣನಾ xkb_context_flags invalid_flags =
        (flags & ~(ಗಣನಾ xkb_context_flags)XKB_CONTEXT_FLAGS_ಮೌಲ್ಯಗಳು);
    ಯದಿ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        ಮುಕ್ತ(ctx);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
    ಯದಿ (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ಯದಿ (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    ಯದಿ (!ctx->atom_table) {
        xkb_context_unref(ctx);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ctx->x11_atom_cache = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ ctx;
}

ಶೂನ್ಯ
xkb_context_set_log_fn(ಸಂರಚನಾ xkb_context *ctx,
                       ಶೂನ್ಯ (*log_fn)(ಸಂರಚನಾ xkb_context *ctx,
                                      ಗಣನಾ xkb_log_level level,
                                      ಧ್ರುವ ವರ್ಣ *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ಗಣನಾ xkb_log_level
xkb_context_get_log_level(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ ctx->log_level;
}

ಶೂನ್ಯ
xkb_context_set_log_level(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_log_level level)
{
    ctx->log_level = level;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_context_get_log_verbosity(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ ctx->log_verbosity;
}

ಶೂನ್ಯ
xkb_context_set_log_verbosity(ಸಂರಚನಾ xkb_context *ctx, ಪೂರ್ಣಾಙ್ಕ verbosity)
{
    ctx->log_verbosity = verbosity;
}

ಶೂನ್ಯ *
xkb_context_get_user_data(ಸಂರಚನಾ xkb_context *ctx)
{
    ಯದಿ (ctx)
        ಪ್ರತ್ಯಾಗಮ ctx->user_data;
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಶೂನ್ಯ
xkb_context_set_user_data(ಸಂರಚನಾ xkb_context *ctx, ಶೂನ್ಯ *user_data)
{
    ctx->user_data = user_data;
}
