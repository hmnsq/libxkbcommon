/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES
    #அந்தர்பாவ <limits.h>
    #எனில் HAVE_UNISTD_H && HAVE_DIRENT_H
        #அந்தர்பாவ <dirent.h>
        #அந்தர்பாவ <unistd.h>
    #endif
#endif

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
நிலையான முழு
context_include_path_append(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *path)
{
    முழு err = ENOMEM;

    எழுத்து *tmp = strdup(path);
    எனில் (!tmp)
        செல் err;

    கட்டமைப்பு stat stat_buf;
    err = stat(path, &stat_buf);
    எனில் (err != 0) {
        err = errno;
        செல் err;
    }
    எனில் (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        செல் err;
    }

    எனில் (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        செல் err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    திரும்பு 1;

err:
    எனில் (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    திரும்பு 0;
}

/**
 * Append one directory to the context’s include path.
 */
முழு
xkb_context_include_path_append(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *path)
{
    திரும்பு (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

மாறிலி எழுத்து *
xkb_context_include_path_get_extra_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து * மாறிலி extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    திரும்பு extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES
மாறிலி எழுத்து *
xkb_context_include_path_get_unversioned_extensions_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    திரும்பு ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#இல்லையெனில்
        : வெற்று;
#endif
}

மாறிலி எழுத்து *
xkb_context_include_path_get_versioned_extensions_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    திரும்பு ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#இல்லையெனில்
        : வெற்று;
#endif
}

நிலையான முழு
compare_str(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    திரும்பு strcmp(*(எழுத்து **)a, *(எழுத்து **) b);
}

நிலையான முழு
add_direct_subdirectories(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *path,
                          darray_string *extensions,
                          darray_size_t versioned_மொத்தம்,
                          size_t versioned_path_length)
{
    முழு ret = 0;
    முழு err = ENOMEM;
    DIR *dir = வெற்று;

    /* Check extensions parent directory */
    கட்டமைப்பு stat stat_buf;
    err = stat(path, &stat_buf);
    எனில் (err != 0) {
        err = errno;
        செல் err;
    }
    எனில் (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        செல் err;
    }
    எனில் (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        செல் err;
    }

    dir = opendir(path);
    எனில் (dir == வெற்று) {
        err = EACCES;
        செல் err;
    }

    கட்டமைப்பு dirent *entry;
    எழுத்து path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    போது ((entry = readdir(dir)) != வெற்று) {
        மாறிலி எழுத்து *name = entry->d_name;

        /* Skip special entries */
        எனில் (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            தொடர்;

        எனில் (!snprintf_safe(path_buf, அளவு(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            செல் err;
        }
        எனில் (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            தொடர்;

        /* Skip if the corresponding versioned directory is already included */
        க்கு (darray_size_t i = 0; i < versioned_மொத்தம்; i++) {
            மாறிலி எழுத்து * மாறிலி prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            எனில் (strcmp(name, prev_name) == 0)
                செல் next;
        }

        எழுத்து *ext_path = strdup_safe(path_buf);
        எனில் (!ext_path) {
            err = ENOMEM;
            செல் err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    எனில் (darray_size(*extensions) > versioned_மொத்தம்) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_பொருட்கள்(*extensions) + versioned_மொத்தம்,
              darray_size(*extensions) - versioned_மொத்தம்,
              அளவு(*darray_பொருட்கள்(*extensions)), &compare_str);
        எழுத்து **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_மொத்தம்) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    திரும்பு ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    எனில் (dir)
        closedir(dir);

    திரும்பு ret;
}
#endif

மாறிலி எழுத்து *
xkb_context_include_path_get_system_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து * மாறிலி வேர் = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    திரும்பு வேர் ? வேர் : DFLT_XKB_CONFIG_வேர்;
}

/**
 * Append the default include directories to the context.
 */
முழு
xkb_context_include_path_append_default(கட்டமைப்பு xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    எழுத்து *user_path;
    முழு ret = 0;

    மாறிலி எழுத்து * மாறிலி home = xkb_context_getenv(ctx, "HOME");
    மாறிலி எழுத்து * மாறிலி xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    எனில் (xdg != வெற்று) {
        user_path = asprintf_safe("%s/xkb", xdg);
        எனில் (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            விடுவி(user_path);
        }
    } இல்லையெனில் எனில் (home != வெற்று) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        எனில் (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            விடுவி(user_path);
        }
    }

    எனில் (home != வெற்று) {
        user_path = asprintf_safe("%s/.xkb", home);
        எனில் (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            விடுவி(user_path);
        }
    }

    மாறிலி எழுத்து * மாறிலி extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    மாறிலி எழுத்து *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    எனில் (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    எனில் (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    எழுத்து **ext_path;
    darray_foreach(ext_path, extensions) {
        விடுவி(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    மாறிலி எழுத்து * மாறிலி வேர் = xkb_context_include_path_get_system_path(ctx);
    மாறிலி bool has_வேர் = context_include_path_append(ctx, வேர்);
    ret |= has_வேர்;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    எனில் (!has_வேர் && வேர்[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_வேர், வேர்);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_வேர்);
    }

    திரும்பு ret;
}

/**
 * Remove all entries in the context's include path.
 */
வெறுமை
xkb_context_include_path_clear(கட்டமைப்பு xkb_context *ctx)
{
    எழுத்து **path;

    darray_foreach(path, ctx->includes)
        விடுவி(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        விடுவி(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
முழு
xkb_context_include_path_reset_defaults(கட்டமைப்பு xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    திரும்பு xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned முழு
xkb_context_இலக்கம்_include_paths(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
மாறிலி எழுத்து *
xkb_context_include_path_get(கட்டமைப்பு xkb_context *ctx, unsigned முழு idx)
{
    எனில் (idx >= xkb_context_இலக்கம்_include_paths(ctx))
        திரும்பு வெற்று;

    திரும்பு darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
கட்டமைப்பு xkb_context *
xkb_context_ref(கட்டமைப்பு xkb_context *ctx)
{
    ப்ரதிஜ்ஞா(ctx->refcnt > 0);
    ctx->refcnt++;
    திரும்பு ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
வெறுமை
xkb_context_unref(கட்டமைப்பு xkb_context *ctx)
{
    ப்ரதிஜ்ஞா(!ctx || ctx->refcnt > 0);
    எனில் (!ctx || --ctx->refcnt > 0)
        திரும்பு;

    விடுவி(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    விடுவி(ctx);
}

நிலையான மாறிலி எழுத்து *
log_level_to_prefix(எண்ணிக்கை xkb_log_level level)
{
    தேர்வு (level) {
    நிலை XKB_LOG_LEVEL_DEBUG:
        திரும்பு "xkbcommon: DEBUG: ";
    நிலை XKB_LOG_LEVEL_INFO:
        திரும்பு "xkbcommon: INFO: ";
    நிலை XKB_LOG_LEVEL_WARNING:
        திரும்பு "xkbcommon: WARNING: ";
    நிலை XKB_LOG_LEVEL_ERROR:
        திரும்பு "xkbcommon: ERROR: ";
    நிலை XKB_LOG_LEVEL_CRITICAL:
        திரும்பு "xkbcommon: CRITICAL: ";
    இயல்பு:
        திரும்பு வெற்று;
    }
}

ATTR_PRINTF(3, 0) நிலையான வெறுமை
default_log_fn(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_log_level level,
               மாறிலி எழுத்து *fmt, va_list args)
{
    மாறிலி எழுத்து *prefix = log_level_to_prefix(level);

    எனில் (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

நிலையான எண்ணிக்கை xkb_log_level
log_level(மாறிலி எழுத்து *level) {
    errno = 0;
    எழுத்து *endptr = வெற்று;
    மாறிலி long lvl = strtol(level, &endptr, 10);
    எனில் (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        திரும்பு (எண்ணிக்கை xkb_log_level)lvl;
    எனில் (istreq_prefix("crit", level))
        திரும்பு XKB_LOG_LEVEL_CRITICAL;
    எனில் (istreq_prefix("err", level))
        திரும்பு XKB_LOG_LEVEL_ERROR;
    எனில் (istreq_prefix("warn", level))
        திரும்பு XKB_LOG_LEVEL_WARNING;
    எனில் (istreq_prefix("info", level))
        திரும்பு XKB_LOG_LEVEL_INFO;
    எனில் (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        திரும்பு XKB_LOG_LEVEL_DEBUG;

    திரும்பு XKB_LOG_LEVEL_ERROR;
}

நிலையான முழு
log_verbosity(மாறிலி எழுத்து *verbosity) {
    errno = 0;
    மாறிலி long முழு v = strtol(verbosity, வெற்று, 10);
    எனில் (errno == 0) {
        /* Truncate */
        திரும்பு (முழு) v;
    }

    திரும்பு XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
கட்டமைப்பு xkb_context *
xkb_context_new(எண்ணிக்கை xkb_context_flags flags)
{
    மாறிலி எழுத்து *env;
    கட்டமைப்பு xkb_context *ctx = calloc(1, அளவு(*ctx));

    எனில் (!ctx)
        திரும்பு வெற்று;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    மாறிலி எண்ணிக்கை xkb_context_flags invalid_flags =
        (flags & ~(எண்ணிக்கை xkb_context_flags)XKB_CONTEXT_FLAGS_மதிப்புகள்);
    எனில் (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        விடுவி(ctx);
        திரும்பு வெற்று;
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
    எனில் (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    எனில் (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    எனில் (!ctx->atom_table) {
        xkb_context_unref(ctx);
        திரும்பு வெற்று;
    }

    ctx->x11_atom_cache = வெற்று;

    திரும்பு ctx;
}

வெறுமை
xkb_context_set_log_fn(கட்டமைப்பு xkb_context *ctx,
                       வெறுமை (*log_fn)(கட்டமைப்பு xkb_context *ctx,
                                      எண்ணிக்கை xkb_log_level level,
                                      மாறிலி எழுத்து *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

எண்ணிக்கை xkb_log_level
xkb_context_get_log_level(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு ctx->log_level;
}

வெறுமை
xkb_context_set_log_level(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_log_level level)
{
    ctx->log_level = level;
}

முழு
xkb_context_get_log_verbosity(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு ctx->log_verbosity;
}

வெறுமை
xkb_context_set_log_verbosity(கட்டமைப்பு xkb_context *ctx, முழு verbosity)
{
    ctx->log_verbosity = verbosity;
}

வெறுமை *
xkb_context_get_user_data(கட்டமைப்பு xkb_context *ctx)
{
    எனில் (ctx)
        திரும்பு ctx->user_data;
    திரும்பு வெற்று;
}

வெறுமை
xkb_context_set_user_data(கட்டமைப்பு xkb_context *ctx, வெறுமை *user_data)
{
    ctx->user_data = user_data;
}
