/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ଅନ୍ତର୍ଭାଵ <limits.h>
    #ଯଦି HAVE_UNISTD_H && HAVE_DIRENT_H
        #ଅନ୍ତର୍ଭାଵ <dirent.h>
        #ଅନ୍ତର୍ଭାଵ <unistd.h>
    #endif
#endif

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
context_include_path_append(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = ENOMEM;

    ଅକ୍ଷର *tmp = strdup(path);
    ଯଦି (!tmp)
        ଯାଅ err;

    ସଂରଚନା stat stat_buf;
    err = stat(path, &stat_buf);
    ଯଦି (err != 0) {
        err = errno;
        ଯାଅ err;
    }
    ଯଦି (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ଯାଅ err;
    }

    ଯଦି (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ଯାଅ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ଫେରନ୍ତୁ 1;

err:
    ଯଦି (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ଫେରନ୍ତୁ 0;
}

/**
 * Append one directory to the context’s include path.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_context_include_path_append(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path)
{
    ଫେରନ୍ତୁ (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ଅଚଳ ଅକ୍ଷର *
xkb_context_include_path_get_extra_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ଫେରନ୍ତୁ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES
ଅଚଳ ଅକ୍ଷର *
xkb_context_include_path_get_unversioned_extensions_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ଫେରନ୍ତୁ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#ଅନ୍ୟଥା
        : ଖାଲି;
#endif
}

ଅଚଳ ଅକ୍ଷର *
xkb_context_include_path_get_versioned_extensions_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ଫେରନ୍ତୁ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#ଅନ୍ୟଥା
        : ଖାଲି;
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_str(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଫେରନ୍ତୁ strcmp(*(ଅକ୍ଷର **)a, *(ଅକ୍ଷର **) b);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
add_direct_subdirectories(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path,
                          darray_string *extensions,
                          darray_size_t versioned_ଗଣନା,
                          size_t versioned_path_length)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = ENOMEM;
    DIR *dir = ଖାଲି;

    /* Check extensions parent directory */
    ସଂରଚନା stat stat_buf;
    err = stat(path, &stat_buf);
    ଯଦି (err != 0) {
        err = errno;
        ଯାଅ err;
    }
    ଯଦି (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ଯାଅ err;
    }
    ଯଦି (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ଯାଅ err;
    }

    dir = opendir(path);
    ଯଦି (dir == ଖାଲି) {
        err = EACCES;
        ଯାଅ err;
    }

    ସଂରଚନା dirent *entry;
    ଅକ୍ଷର path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ଯେତେବେଳେ ((entry = readdir(dir)) != ଖାଲି) {
        ଅଚଳ ଅକ୍ଷର *name = entry->d_name;

        /* Skip special entries */
        ଯଦି (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ଜାରି;

        ଯଦି (!snprintf_safe(path_buf, ଆକାର(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            ଯାଅ err;
        }
        ଯଦି (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ଜାରି;

        /* Skip if the corresponding versioned directory is already included */
        ପାଇଁ (darray_size_t i = 0; i < versioned_ଗଣନା; i++) {
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ଯଦି (strcmp(name, prev_name) == 0)
                ଯାଅ next;
        }

        ଅକ୍ଷର *ext_path = strdup_safe(path_buf);
        ଯଦି (!ext_path) {
            err = ENOMEM;
            ଯାଅ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ଯଦି (darray_size(*extensions) > versioned_ଗଣନା) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(*extensions) + versioned_ଗଣନା,
              darray_size(*extensions) - versioned_ଗଣନା,
              ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(*extensions)), &compare_str);
        ଅକ୍ଷର **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ଗଣନା) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    ଫେରନ୍ତୁ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ଯଦି (dir)
        closedir(dir);

    ଫେରନ୍ତୁ ret;
}
#endif

ଅଚଳ ଅକ୍ଷର *
xkb_context_include_path_get_system_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ ମୂଳ = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ଫେରନ୍ତୁ ମୂଳ ? ମୂଳ : DFLT_XKB_CONFIG_ମୂଳ;
}

/**
 * Append the default include directories to the context.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_context_include_path_append_default(ସଂରଚନା xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    ଅକ୍ଷର *user_path;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ home = xkb_context_getenv(ctx, "HOME");
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ଯଦି (xdg != ଖାଲି) {
        user_path = asprintf_safe("%s/xkb", xdg);
        ଯଦି (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ମୁକ୍ତ(user_path);
        }
    } ଅନ୍ୟଥା ଯଦି (home != ଖାଲି) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        ଯଦି (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ମୁକ୍ତ(user_path);
        }
    }

    ଯଦି (home != ଖାଲି) {
        user_path = asprintf_safe("%s/.xkb", home);
        ଯଦି (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            ମୁକ୍ତ(user_path);
        }
    }

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ଅଚଳ ଅକ୍ଷର *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    ଯଦି (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    ଯଦି (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ଅକ୍ଷର **ext_path;
    darray_foreach(ext_path, extensions) {
        ମୁକ୍ତ(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ ମୂଳ = xkb_context_include_path_get_system_path(ctx);
    ଅଚଳ bool has_ମୂଳ = context_include_path_append(ctx, ମୂଳ);
    ret |= has_ମୂଳ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ଯଦି (!has_ମୂଳ && ମୂଳ[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ମୂଳ, ମୂଳ);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ମୂଳ);
    }

    ଫେରନ୍ତୁ ret;
}

/**
 * Remove all entries in the context's include path.
 */
ଶୂନ୍ୟ
xkb_context_include_path_clear(ସଂରଚନା xkb_context *ctx)
{
    ଅକ୍ଷର **path;

    darray_foreach(path, ctx->includes)
        ମୁକ୍ତ(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        ମୁକ୍ତ(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_context_include_path_reset_defaults(ସଂରଚନା xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    ଫେରନ୍ତୁ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_context_ଅଙ୍କ_include_paths(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ଅଚଳ ଅକ୍ଷର *
xkb_context_include_path_get(ସଂରଚନା xkb_context *ctx, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
    ଯଦି (idx >= xkb_context_ଅଙ୍କ_include_paths(ctx))
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ସଂରଚନା xkb_context *
xkb_context_ref(ସଂରଚନା xkb_context *ctx)
{
    ପ୍ରତିଜ୍ଞା(ctx->refcnt > 0);
    ctx->refcnt++;
    ଫେରନ୍ତୁ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
ଶୂନ୍ୟ
xkb_context_unref(ସଂରଚନା xkb_context *ctx)
{
    ପ୍ରତିଜ୍ଞା(!ctx || ctx->refcnt > 0);
    ଯଦି (!ctx || --ctx->refcnt > 0)
        ଫେରନ୍ତୁ;

    ମୁକ୍ତ(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    ମୁକ୍ତ(ctx);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
log_level_to_prefix(ଗଣନା xkb_log_level level)
{
    ବାଛ (level) {
    ସ୍ଥିତି XKB_LOG_LEVEL_DEBUG:
        ଫେରନ୍ତୁ "xkbcommon: DEBUG: ";
    ସ୍ଥିତି XKB_LOG_LEVEL_INFO:
        ଫେରନ୍ତୁ "xkbcommon: INFO: ";
    ସ୍ଥିତି XKB_LOG_LEVEL_WARNING:
        ଫେରନ୍ତୁ "xkbcommon: WARNING: ";
    ସ୍ଥିତି XKB_LOG_LEVEL_ERROR:
        ଫେରନ୍ତୁ "xkbcommon: ERROR: ";
    ସ୍ଥିତି XKB_LOG_LEVEL_CRITICAL:
        ଫେରନ୍ତୁ "xkbcommon: CRITICAL: ";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ ଖାଲି;
    }
}

ATTR_PRINTF(3, 0) ସ୍ଥିର ଶୂନ୍ୟ
default_log_fn(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_log_level level,
               ଅଚଳ ଅକ୍ଷର *fmt, va_list args)
{
    ଅଚଳ ଅକ୍ଷର *prefix = log_level_to_prefix(level);

    ଯଦି (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ସ୍ଥିର ଗଣନା xkb_log_level
log_level(ଅଚଳ ଅକ୍ଷର *level) {
    errno = 0;
    ଅକ୍ଷର *endptr = ଖାଲି;
    ଅଚଳ long lvl = strtol(level, &endptr, 10);
    ଯଦି (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ଫେରନ୍ତୁ (ଗଣନା xkb_log_level)lvl;
    ଯଦି (istreq_prefix("crit", level))
        ଫେରନ୍ତୁ XKB_LOG_LEVEL_CRITICAL;
    ଯଦି (istreq_prefix("err", level))
        ଫେରନ୍ତୁ XKB_LOG_LEVEL_ERROR;
    ଯଦି (istreq_prefix("warn", level))
        ଫେରନ୍ତୁ XKB_LOG_LEVEL_WARNING;
    ଯଦି (istreq_prefix("info", level))
        ଫେରନ୍ତୁ XKB_LOG_LEVEL_INFO;
    ଯଦି (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ଫେରନ୍ତୁ XKB_LOG_LEVEL_DEBUG;

    ଫେରନ୍ତୁ XKB_LOG_LEVEL_ERROR;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
log_verbosity(ଅଚଳ ଅକ୍ଷର *verbosity) {
    errno = 0;
    ଅଚଳ long ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = strtol(verbosity, ଖାଲି, 10);
    ଯଦି (errno == 0) {
        /* Truncate */
        ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) v;
    }

    ଫେରନ୍ତୁ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ସଂରଚନା xkb_context *
xkb_context_new(ଗଣନା xkb_context_flags flags)
{
    ଅଚଳ ଅକ୍ଷର *env;
    ସଂରଚନା xkb_context *ctx = calloc(1, ଆକାର(*ctx));

    ଯଦି (!ctx)
        ଫେରନ୍ତୁ ଖାଲି;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ଅଚଳ ଗଣନା xkb_context_flags invalid_flags =
        (flags & ~(ଗଣନା xkb_context_flags)XKB_CONTEXT_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଯଦି (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        ମୁକ୍ତ(ctx);
        ଫେରନ୍ତୁ ଖାଲି;
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
    ଯଦି (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ଯଦି (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    ଯଦି (!ctx->atom_table) {
        xkb_context_unref(ctx);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ctx->x11_atom_cache = ଖାଲି;

    ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ
xkb_context_set_log_fn(ସଂରଚନା xkb_context *ctx,
                       ଶୂନ୍ୟ (*log_fn)(ସଂରଚନା xkb_context *ctx,
                                      ଗଣନା xkb_log_level level,
                                      ଅଚଳ ଅକ୍ଷର *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ଗଣନା xkb_log_level
xkb_context_get_log_level(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ ctx->log_level;
}

ଶୂନ୍ୟ
xkb_context_set_log_level(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_log_level level)
{
    ctx->log_level = level;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_context_get_log_verbosity(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ ctx->log_verbosity;
}

ଶୂନ୍ୟ
xkb_context_set_log_verbosity(ସଂରଚନା xkb_context *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity)
{
    ctx->log_verbosity = verbosity;
}

ଶୂନ୍ୟ *
xkb_context_get_user_data(ସଂରଚନା xkb_context *ctx)
{
    ଯଦି (ctx)
        ଫେରନ୍ତୁ ctx->user_data;
    ଫେରନ୍ତୁ ଖାଲି;
}

ଶୂନ୍ୟ
xkb_context_set_user_data(ସଂରଚନା xkb_context *ctx, ଶୂନ୍ୟ *user_data)
{
    ctx->user_data = user_data;
}
