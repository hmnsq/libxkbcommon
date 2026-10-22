/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES
    #अन्तर्भाव <limits.h>
    #जर HAVE_UNISTD_H && HAVE_DIRENT_H
        #अन्तर्भाव <dirent.h>
        #अन्तर्भाव <unistd.h>
    #endif
#endif

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"


/**
 * Append one directory to the context’s include path.
 */
स्थिर पूर्णांक
context_include_path_append(संरचना xkb_context *ctx, स्थिरांक अक्षर *path)
{
    पूर्णांक err = ENOMEM;

    अक्षर *tmp = strdup(path);
    जर (!tmp)
        जा err;

    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    जर (err != 0) {
        err = त्रुटि_सं;
        जा err;
    }
    जर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जा err;
    }

    जर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जा err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    परत 1;

err:
    जर (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    परत 0;
}

/**
 * Append one directory to the context’s include path.
 */
पूर्णांक
xkb_context_include_path_append(संरचना xkb_context *ctx, स्थिरांक अक्षर *path)
{
    परत (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

स्थिरांक अक्षर *
xkb_context_include_path_get_extra_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर * स्थिरांक extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    परत extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES
स्थिरांक अक्षर *
xkb_context_include_path_get_unversioned_extensions_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    परत ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : रिक्त;
#endif
}

स्थिरांक अक्षर *
xkb_context_include_path_get_versioned_extensions_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    परत ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : रिक्त;
#endif
}

स्थिर पूर्णांक
compare_str(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    परत म_भेद(*(अक्षर **)a, *(अक्षर **) b);
}

स्थिर पूर्णांक
add_direct_subdirectories(संरचना xkb_context *ctx, स्थिरांक अक्षर *path,
                          darray_string *extensions,
                          darray_size_t versioned_मोजणी,
                          माप_प्रकार versioned_path_length)
{
    पूर्णांक ret = 0;
    पूर्णांक err = ENOMEM;
    सूची *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    जर (err != 0) {
        err = त्रुटि_सं;
        जा err;
    }
    जर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जा err;
    }
    जर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जा err;
    }

    dir = सूची_खोलो(path);
    जर (dir == रिक्त) {
        err = EACCES;
        जा err;
    }

    संरचना dirent *entry;
    अक्षर path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    पर्यंत ((entry = सूची_पढ़ो(dir)) != रिक्त) {
        स्थिरांक अक्षर *name = entry->d_name;

        /* Skip special entries */
        जर (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            सुरूठेवा;

        जर (!snprintf_safe(path_buf, आकार(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            जा err;
        }
        जर (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            सुरूठेवा;

        /* Skip if the corresponding versioned directory is already included */
        साठी (darray_size_t i = 0; i < versioned_मोजणी; i++) {
            स्थिरांक अक्षर * स्थिरांक prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            जर (म_भेद(name, prev_name) == 0)
                जा next;
        }

        अक्षर *ext_path = strdup_safe(path_buf);
        जर (!ext_path) {
            err = ENOMEM;
            जा err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    जर (darray_size(*extensions) > versioned_मोजणी) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुसंच(*extensions) + versioned_मोजणी,
              darray_size(*extensions) - versioned_मोजणी,
              आकार(*darray_वस्तुसंच(*extensions)), &compare_str);
        अक्षर **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_मोजणी) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    परत ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    जर (dir)
        बंद_सूची(dir);

    परत ret;
}
#endif

स्थिरांक अक्षर *
xkb_context_include_path_get_system_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर * स्थिरांक मूळ = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    परत मूळ ? मूळ : DFLT_XKB_CONFIG_मूळ;
}

/**
 * Append the default include directories to the context.
 */
पूर्णांक
xkb_context_include_path_append_default(संरचना xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    अक्षर *user_path;
    पूर्णांक ret = 0;

    स्थिरांक अक्षर * स्थिरांक home = xkb_context_getenv(ctx, "HOME");
    स्थिरांक अक्षर * स्थिरांक xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    जर (xdg != रिक्त) {
        user_path = asprintf_safe("%s/xkb", xdg);
        जर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    } अन्यथा जर (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        जर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    जर (home != रिक्त) {
        user_path = asprintf_safe("%s/.xkb", home);
        जर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    स्थिरांक अक्षर * स्थिरांक extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    स्थिरांक अक्षर *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    माप_प्रकार versioned_path_length = 0;
    जर (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    जर (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    अक्षर **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    स्थिरांक अक्षर * स्थिरांक मूळ = xkb_context_include_path_get_system_path(ctx);
    स्थिरांक bool has_मूळ = context_include_path_append(ctx, मूळ);
    ret |= has_मूळ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    जर (!has_मूळ && मूळ[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूळ, मूळ);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_मूळ);
    }

    परत ret;
}

/**
 * Remove all entries in the context's include path.
 */
शून्य
xkb_context_include_path_clear(संरचना xkb_context *ctx)
{
    अक्षर **path;

    darray_foreach(path, ctx->includes)
        मुक्त(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        मुक्त(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
पूर्णांक
xkb_context_include_path_reset_defaults(संरचना xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    परत xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
अचिन्हित पूर्णांक
xkb_context_अंक_include_paths(संरचना xkb_context *ctx)
{
    परत (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
स्थिरांक अक्षर *
xkb_context_include_path_get(संरचना xkb_context *ctx, अचिन्हित पूर्णांक idx)
{
    जर (idx >= xkb_context_अंक_include_paths(ctx))
        परत रिक्त;

    परत darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
संरचना xkb_context *
xkb_context_ref(संरचना xkb_context *ctx)
{
    प्रतिज्ञा(ctx->refcnt > 0);
    ctx->refcnt++;
    परत ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
शून्य
xkb_context_unref(संरचना xkb_context *ctx)
{
    प्रतिज्ञा(!ctx || ctx->refcnt > 0);
    जर (!ctx || --ctx->refcnt > 0)
        परत;

    मुक्त(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    मुक्त(ctx);
}

स्थिर स्थिरांक अक्षर *
log_level_to_prefix(गणना xkb_log_level level)
{
    निवड (level) {
    स्थिती XKB_LOG_LEVEL_DEBUG:
        परत "xkbcommon: DEBUG: ";
    स्थिती XKB_LOG_LEVEL_INFO:
        परत "xkbcommon: INFO: ";
    स्थिती XKB_LOG_LEVEL_WARNING:
        परत "xkbcommon: WARNING: ";
    स्थिती XKB_LOG_LEVEL_ERROR:
        परत "xkbcommon: ERROR: ";
    स्थिती XKB_LOG_LEVEL_CRITICAL:
        परत "xkbcommon: CRITICAL: ";
    पूर्वनिर्धारित:
        परत रिक्त;
    }
}

ATTR_PRINTF(3, 0) स्थिर शून्य
default_log_fn(संरचना xkb_context *ctx, गणना xkb_log_level level,
               स्थिरांक अक्षर *fmt, बहु_सूची args)
{
    स्थिरांक अक्षर *prefix = log_level_to_prefix(level);

    जर (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

स्थिर गणना xkb_log_level
log_level(स्थिरांक अक्षर *level) {
    त्रुटि_सं = 0;
    अक्षर *endptr = रिक्त;
    स्थिरांक दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    जर (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        परत (गणना xkb_log_level)lvl;
    जर (istreq_prefix("crit", level))
        परत XKB_LOG_LEVEL_CRITICAL;
    जर (istreq_prefix("err", level))
        परत XKB_LOG_LEVEL_ERROR;
    जर (istreq_prefix("warn", level))
        परत XKB_LOG_LEVEL_WARNING;
    जर (istreq_prefix("info", level))
        परत XKB_LOG_LEVEL_INFO;
    जर (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        परत XKB_LOG_LEVEL_DEBUG;

    परत XKB_LOG_LEVEL_ERROR;
}

स्थिर पूर्णांक
log_verbosity(स्थिरांक अक्षर *verbosity) {
    त्रुटि_सं = 0;
    स्थिरांक दीर्घ पूर्णांक v = म_से_दीर्घ(verbosity, रिक्त, 10);
    जर (त्रुटि_सं == 0) {
        /* Truncate */
        परत (पूर्णांक) v;
    }

    परत XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
संरचना xkb_context *
xkb_context_new(गणना xkb_context_flags flags)
{
    स्थिरांक अक्षर *env;
    संरचना xkb_context *ctx = सुस्मृति(1, आकार(*ctx));

    जर (!ctx)
        परत रिक्त;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    स्थिरांक गणना xkb_context_flags invalid_flags =
        (flags & ~(गणना xkb_context_flags)XKB_CONTEXT_FLAGS_मूल्ये);
    जर (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        मुक्त(ctx);
        परत रिक्त;
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
    जर (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    जर (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    जर (!ctx->atom_table) {
        xkb_context_unref(ctx);
        परत रिक्त;
    }

    ctx->x11_atom_cache = रिक्त;

    परत ctx;
}

शून्य
xkb_context_set_log_fn(संरचना xkb_context *ctx,
                       शून्य (*log_fn)(संरचना xkb_context *ctx,
                                      गणना xkb_log_level level,
                                      स्थिरांक अक्षर *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

गणना xkb_log_level
xkb_context_get_log_level(संरचना xkb_context *ctx)
{
    परत ctx->log_level;
}

शून्य
xkb_context_set_log_level(संरचना xkb_context *ctx, गणना xkb_log_level level)
{
    ctx->log_level = level;
}

पूर्णांक
xkb_context_get_log_verbosity(संरचना xkb_context *ctx)
{
    परत ctx->log_verbosity;
}

शून्य
xkb_context_set_log_verbosity(संरचना xkb_context *ctx, पूर्णांक verbosity)
{
    ctx->log_verbosity = verbosity;
}

शून्य *
xkb_context_get_user_data(संरचना xkb_context *ctx)
{
    जर (ctx)
        परत ctx->user_data;
    परत रिक्त;
}

शून्य
xkb_context_set_user_data(संरचना xkb_context *ctx, शून्य *user_data)
{
    ctx->user_data = user_data;
}
