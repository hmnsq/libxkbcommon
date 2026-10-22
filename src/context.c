/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #അന്തര്ഭാവ <limits.h>
    #എങ്കിൽ HAVE_UNISTD_H && HAVE_DIRENT_H
        #അന്തര്ഭാവ <dirent.h>
        #അന്തര്ഭാവ <unistd.h>
    #endif
#endif

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
context_include_path_append(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *path)
{
    പൂർണ്ണസംഖ്യ err = ENOMEM;

    അക്ഷരം *tmp = strdup(path);
    എങ്കിൽ (!tmp)
        പോകുക err;

    ഘടന stat stat_buf;
    err = stat(path, &stat_buf);
    എങ്കിൽ (err != 0) {
        err = errno;
        പോകുക err;
    }
    എങ്കിൽ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        പോകുക err;
    }

    എങ്കിൽ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        പോകുക err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    തിരികെ 1;

err:
    എങ്കിൽ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    തിരികെ 0;
}

/**
 * Append one directory to the context’s include path.
 */
പൂർണ്ണസംഖ്യ
xkb_context_include_path_append(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *path)
{
    തിരികെ (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_extra_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം * സ്ഥിരം extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    തിരികെ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES
സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_unversioned_extensions_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    തിരികെ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#അല്ലെങ്കിൽ
        : ശൂന്യ;
#endif
}

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_versioned_extensions_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    തിരികെ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#അല്ലെങ്കിൽ
        : ശൂന്യ;
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_str(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    തിരികെ strcmp(*(അക്ഷരം **)a, *(അക്ഷരം **) b);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
add_direct_subdirectories(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *path,
                          darray_string *extensions,
                          darray_size_t versioned_എണ്ണം,
                          size_t versioned_path_length)
{
    പൂർണ്ണസംഖ്യ ret = 0;
    പൂർണ്ണസംഖ്യ err = ENOMEM;
    DIR *dir = ശൂന്യ;

    /* Check extensions parent directory */
    ഘടന stat stat_buf;
    err = stat(path, &stat_buf);
    എങ്കിൽ (err != 0) {
        err = errno;
        പോകുക err;
    }
    എങ്കിൽ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        പോകുക err;
    }
    എങ്കിൽ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        പോകുക err;
    }

    dir = opendir(path);
    എങ്കിൽ (dir == ശൂന്യ) {
        err = EACCES;
        പോകുക err;
    }

    ഘടന dirent *entry;
    അക്ഷരം path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ആകുമ്പോൾ ((entry = readdir(dir)) != ശൂന്യ) {
        സ്ഥിരം അക്ഷരം *name = entry->d_name;

        /* Skip special entries */
        എങ്കിൽ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            തുടരുക;

        എങ്കിൽ (!snprintf_safe(path_buf, വലുപ്പം(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            പോകുക err;
        }
        എങ്കിൽ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            തുടരുക;

        /* Skip if the corresponding versioned directory is already included */
        വേണ്ടി (darray_size_t i = 0; i < versioned_എണ്ണം; i++) {
            സ്ഥിരം അക്ഷരം * സ്ഥിരം prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            എങ്കിൽ (strcmp(name, prev_name) == 0)
                പോകുക next;
        }

        അക്ഷരം *ext_path = strdup_safe(path_buf);
        എങ്കിൽ (!ext_path) {
            err = ENOMEM;
            പോകുക err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    എങ്കിൽ (darray_size(*extensions) > versioned_എണ്ണം) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ഇനങ്ങൾ(*extensions) + versioned_എണ്ണം,
              darray_size(*extensions) - versioned_എണ്ണം,
              വലുപ്പം(*darray_ഇനങ്ങൾ(*extensions)), &compare_str);
        അക്ഷരം **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_എണ്ണം) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    തിരികെ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    എങ്കിൽ (dir)
        closedir(dir);

    തിരികെ ret;
}
#endif

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_system_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം * സ്ഥിരം വേര് = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    തിരികെ വേര് ? വേര് : DFLT_XKB_CONFIG_വേര്;
}

/**
 * Append the default include directories to the context.
 */
പൂർണ്ണസംഖ്യ
xkb_context_include_path_append_default(ഘടന xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    അക്ഷരം *user_path;
    പൂർണ്ണസംഖ്യ ret = 0;

    സ്ഥിരം അക്ഷരം * സ്ഥിരം home = xkb_context_getenv(ctx, "HOME");
    സ്ഥിരം അക്ഷരം * സ്ഥിരം xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    എങ്കിൽ (xdg != ശൂന്യ) {
        user_path = asprintf_safe("%s/xkb", xdg);
        എങ്കിൽ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            സ്വതന്ത്രം(user_path);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (home != ശൂന്യ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        എങ്കിൽ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            സ്വതന്ത്രം(user_path);
        }
    }

    എങ്കിൽ (home != ശൂന്യ) {
        user_path = asprintf_safe("%s/.xkb", home);
        എങ്കിൽ (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            സ്വതന്ത്രം(user_path);
        }
    }

    സ്ഥിരം അക്ഷരം * സ്ഥിരം extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    സ്ഥിരം അക്ഷരം *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    എങ്കിൽ (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    എങ്കിൽ (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    അക്ഷരം **ext_path;
    darray_foreach(ext_path, extensions) {
        സ്വതന്ത്രം(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    സ്ഥിരം അക്ഷരം * സ്ഥിരം വേര് = xkb_context_include_path_get_system_path(ctx);
    സ്ഥിരം bool has_വേര് = context_include_path_append(ctx, വേര്);
    ret |= has_വേര്;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    എങ്കിൽ (!has_വേര് && വേര്[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_വേര്, വേര്);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_വേര്);
    }

    തിരികെ ret;
}

/**
 * Remove all entries in the context's include path.
 */
ശൂന്യം
xkb_context_include_path_clear(ഘടന xkb_context *ctx)
{
    അക്ഷരം **path;

    darray_foreach(path, ctx->includes)
        സ്വതന്ത്രം(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        സ്വതന്ത്രം(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
പൂർണ്ണസംഖ്യ
xkb_context_include_path_reset_defaults(ഘടന xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    തിരികെ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned പൂർണ്ണസംഖ്യ
xkb_context_അക്കം_include_paths(ഘടന xkb_context *ctx)
{
    തിരികെ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get(ഘടന xkb_context *ctx, unsigned പൂർണ്ണസംഖ്യ idx)
{
    എങ്കിൽ (idx >= xkb_context_അക്കം_include_paths(ctx))
        തിരികെ ശൂന്യ;

    തിരികെ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ഘടന xkb_context *
xkb_context_ref(ഘടന xkb_context *ctx)
{
    പ്രതിജ്ഞാ(ctx->refcnt > 0);
    ctx->refcnt++;
    തിരികെ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
ശൂന്യം
xkb_context_unref(ഘടന xkb_context *ctx)
{
    പ്രതിജ്ഞാ(!ctx || ctx->refcnt > 0);
    എങ്കിൽ (!ctx || --ctx->refcnt > 0)
        തിരികെ;

    സ്വതന്ത്രം(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    സ്വതന്ത്രം(ctx);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
log_level_to_prefix(എണ്ണൽ xkb_log_level level)
{
    തിരഞ്ഞെടുപ്പ് (level) {
    അവസ്ഥ XKB_LOG_LEVEL_DEBUG:
        തിരികെ "xkbcommon: DEBUG: ";
    അവസ്ഥ XKB_LOG_LEVEL_INFO:
        തിരികെ "xkbcommon: INFO: ";
    അവസ്ഥ XKB_LOG_LEVEL_WARNING:
        തിരികെ "xkbcommon: WARNING: ";
    അവസ്ഥ XKB_LOG_LEVEL_ERROR:
        തിരികെ "xkbcommon: ERROR: ";
    അവസ്ഥ XKB_LOG_LEVEL_CRITICAL:
        തിരികെ "xkbcommon: CRITICAL: ";
    സ്ഥിരസ്ഥിതി:
        തിരികെ ശൂന്യ;
    }
}

ATTR_PRINTF(3, 0) സ്ഥിതം ശൂന്യം
default_log_fn(ഘടന xkb_context *ctx, എണ്ണൽ xkb_log_level level,
               സ്ഥിരം അക്ഷരം *fmt, va_list args)
{
    സ്ഥിരം അക്ഷരം *prefix = log_level_to_prefix(level);

    എങ്കിൽ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

സ്ഥിതം എണ്ണൽ xkb_log_level
log_level(സ്ഥിരം അക്ഷരം *level) {
    errno = 0;
    അക്ഷരം *endptr = ശൂന്യ;
    സ്ഥിരം long lvl = strtol(level, &endptr, 10);
    എങ്കിൽ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        തിരികെ (എണ്ണൽ xkb_log_level)lvl;
    എങ്കിൽ (istreq_prefix("crit", level))
        തിരികെ XKB_LOG_LEVEL_CRITICAL;
    എങ്കിൽ (istreq_prefix("err", level))
        തിരികെ XKB_LOG_LEVEL_ERROR;
    എങ്കിൽ (istreq_prefix("warn", level))
        തിരികെ XKB_LOG_LEVEL_WARNING;
    എങ്കിൽ (istreq_prefix("info", level))
        തിരികെ XKB_LOG_LEVEL_INFO;
    എങ്കിൽ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        തിരികെ XKB_LOG_LEVEL_DEBUG;

    തിരികെ XKB_LOG_LEVEL_ERROR;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
log_verbosity(സ്ഥിരം അക്ഷരം *verbosity) {
    errno = 0;
    സ്ഥിരം long പൂർണ്ണസംഖ്യ v = strtol(verbosity, ശൂന്യ, 10);
    എങ്കിൽ (errno == 0) {
        /* Truncate */
        തിരികെ (പൂർണ്ണസംഖ്യ) v;
    }

    തിരികെ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ഘടന xkb_context *
xkb_context_new(എണ്ണൽ xkb_context_flags flags)
{
    സ്ഥിരം അക്ഷരം *env;
    ഘടന xkb_context *ctx = calloc(1, വലുപ്പം(*ctx));

    എങ്കിൽ (!ctx)
        തിരികെ ശൂന്യ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    സ്ഥിരം എണ്ണൽ xkb_context_flags invalid_flags =
        (flags & ~(എണ്ണൽ xkb_context_flags)XKB_CONTEXT_FLAGS_മൂല്യങ്ങൾ);
    എങ്കിൽ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        സ്വതന്ത്രം(ctx);
        തിരികെ ശൂന്യ;
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
    എങ്കിൽ (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    എങ്കിൽ (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    എങ്കിൽ (!ctx->atom_table) {
        xkb_context_unref(ctx);
        തിരികെ ശൂന്യ;
    }

    ctx->x11_atom_cache = ശൂന്യ;

    തിരികെ ctx;
}

ശൂന്യം
xkb_context_set_log_fn(ഘടന xkb_context *ctx,
                       ശൂന്യം (*log_fn)(ഘടന xkb_context *ctx,
                                      എണ്ണൽ xkb_log_level level,
                                      സ്ഥിരം അക്ഷരം *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

എണ്ണൽ xkb_log_level
xkb_context_get_log_level(ഘടന xkb_context *ctx)
{
    തിരികെ ctx->log_level;
}

ശൂന്യം
xkb_context_set_log_level(ഘടന xkb_context *ctx, എണ്ണൽ xkb_log_level level)
{
    ctx->log_level = level;
}

പൂർണ്ണസംഖ്യ
xkb_context_get_log_verbosity(ഘടന xkb_context *ctx)
{
    തിരികെ ctx->log_verbosity;
}

ശൂന്യം
xkb_context_set_log_verbosity(ഘടന xkb_context *ctx, പൂർണ്ണസംഖ്യ verbosity)
{
    ctx->log_verbosity = verbosity;
}

ശൂന്യം *
xkb_context_get_user_data(ഘടന xkb_context *ctx)
{
    എങ്കിൽ (ctx)
        തിരികെ ctx->user_data;
    തിരികെ ശൂന്യ;
}

ശൂന്യം
xkb_context_set_user_data(ഘടന xkb_context *ctx, ശൂന്യം *user_data)
{
    ctx->user_data = user_data;
}
