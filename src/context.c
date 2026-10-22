/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <errno.h>
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <sys/stat.h>

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES
    #समावेश <limits.h>
    #अगर HAVE_UNISTD_H && HAVE_DIRENT_H
        #समावेश <dirent.h>
        #समावेश <unistd.h>
    #पूर्ण_अगर
#पूर्ण_अगर

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "features/enums.h"
#समावेश "messages-codes.h"
#समावेश "utils.h"


/**
 * Append one directory to the context’s include path.
 */
अटल पूर्णांक
context_include_path_append(काष्ठा xkb_context *ctx, स्थिर अक्षर *path)
{
    पूर्णांक err = ENOMEM;

    अक्षर *tmp = strdup(path);
    अगर (!tmp)
        जाओ err;

    काष्ठा stat stat_buf;
    err = stat(path, &stat_buf);
    अगर (err != 0) {
        err = त्रुटि_सं;
        जाओ err;
    }
    अगर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जाओ err;
    }

    अगर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जाओ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    वापस 1;

err:
    अगर (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    वापस 0;
}

/**
 * Append one directory to the context’s include path.
 */
पूर्णांक
xkb_context_include_path_append(काष्ठा xkb_context *ctx, स्थिर अक्षर *path)
{
    वापस (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

स्थिर अक्षर *
xkb_context_include_path_get_extra_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर * स्थिर extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    वापस extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES
स्थिर अक्षर *
xkb_context_include_path_get_unversioned_extensions_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    वापस ext
        ? ext
#अगर_घोषित DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : शून्य;
#पूर्ण_अगर
}

स्थिर अक्षर *
xkb_context_include_path_get_versioned_extensions_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    वापस ext
        ? ext
#अगर_घोषित DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : शून्य;
#पूर्ण_अगर
}

अटल पूर्णांक
compare_str(स्थिर व्योम *a, स्थिर व्योम *b)
{
    वापस म_भेद(*(अक्षर **)a, *(अक्षर **) b);
}

अटल पूर्णांक
add_direct_subdirectories(काष्ठा xkb_context *ctx, स्थिर अक्षर *path,
                          darray_string *extensions,
                          darray_आकार_t versioned_गिनती,
                          माप_प्रकार versioned_path_लंबाई)
{
    पूर्णांक ret = 0;
    पूर्णांक err = ENOMEM;
    सूची *dir = शून्य;

    /* Check extensions parent directory */
    काष्ठा stat stat_buf;
    err = stat(path, &stat_buf);
    अगर (err != 0) {
        err = त्रुटि_सं;
        जाओ err;
    }
    अगर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जाओ err;
    }
    अगर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जाओ err;
    }

    dir = सूची_खोलो(path);
    अगर (dir == शून्य) {
        err = EACCES;
        जाओ err;
    }

    काष्ठा dirent *entry;
    अक्षर path_buf[PATH_MAX] = "";
    versioned_path_लंबाई++; /* Additional final ‘/’ */
    जबतक ((entry = सूची_पढ़ो(dir)) != शून्य) {
        स्थिर अक्षर *name = entry->d_name;

        /* Skip special entries */
        अगर (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            जारी;

        अगर (!snprintf_safe(path_buf, माप(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            जाओ err;
        }
        अगर (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            जारी;

        /* Skip if the corresponding versioned directory is already included */
        क्रम (darray_आकार_t i = 0; i < versioned_गिनती; i++) {
            स्थिर अक्षर * स्थिर पूर्व_name = darray_item(*extensions, i)
                                         + versioned_path_लंबाई;
            अगर (म_भेद(name, पूर्व_name) == 0)
                जाओ अग्र;
        }

        अक्षर *ext_path = strdup_safe(path_buf);
        अगर (!ext_path) {
            err = ENOMEM;
            जाओ err;
        }

        darray_append(*extensions, ext_path);
अग्र:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    अगर (darray_आकार(*extensions) > versioned_गिनती) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुएँ(*extensions) + versioned_गिनती,
              darray_आकार(*extensions) - versioned_गिनती,
              माप(*darray_वस्तुएँ(*extensions)), &compare_str);
        अक्षर **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_गिनती) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    वापस ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    अगर (dir)
        बंद_सूची(dir);

    वापस ret;
}
#पूर्ण_अगर

स्थिर अक्षर *
xkb_context_include_path_get_system_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर * स्थिर मूल = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    वापस मूल ? मूल : DFLT_XKB_CONFIG_मूल;
}

/**
 * Append the default include directories to the context.
 */
पूर्णांक
xkb_context_include_path_append_default(काष्ठा xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    अक्षर *user_path;
    पूर्णांक ret = 0;

    स्थिर अक्षर * स्थिर home = xkb_context_getenv(ctx, "HOME");
    स्थिर अक्षर * स्थिर xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    अगर (xdg != शून्य) {
        user_path = asprintf_safe("%s/xkb", xdg);
        अगर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    } अन्यथा अगर (home != शून्य) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        अगर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    अगर (home != शून्य) {
        user_path = asprintf_safe("%s/.xkb", home);
        अगर (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    स्थिर अक्षर * स्थिर extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    स्थिर अक्षर *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    माप_प्रकार versioned_path_लंबाई = 0;
    अगर (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_लंबाई = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    अगर (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_आकार(extensions),
                                         versioned_path_लंबाई);
    }

    अक्षर **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#पूर्ण_अगर

    /* Canonical XKB root */
    स्थिर अक्षर * स्थिर मूल = xkb_context_include_path_get_system_path(ctx);
    स्थिर bool has_मूल = context_include_path_append(ctx, मूल);
    ret |= has_मूल;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    अगर (!has_मूल && मूल[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूल, मूल);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_मूल);
    }

    वापस ret;
}

/**
 * Remove all entries in the context's include path.
 */
व्योम
xkb_context_include_path_clear(काष्ठा xkb_context *ctx)
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
xkb_context_include_path_reset_defaults(काष्ठा xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    वापस xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
अचिन्हित पूर्णांक
xkb_context_अंक_include_paths(काष्ठा xkb_context *ctx)
{
    वापस (xkb_context_init_includes(ctx))
        ? darray_आकार(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
स्थिर अक्षर *
xkb_context_include_path_get(काष्ठा xkb_context *ctx, अचिन्हित पूर्णांक idx)
{
    अगर (idx >= xkb_context_अंक_include_paths(ctx))
        वापस शून्य;

    वापस darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
काष्ठा xkb_context *
xkb_context_ref(काष्ठा xkb_context *ctx)
{
    निश्चित(ctx->refcnt > 0);
    ctx->refcnt++;
    वापस ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
व्योम
xkb_context_unref(काष्ठा xkb_context *ctx)
{
    निश्चित(!ctx || ctx->refcnt > 0);
    अगर (!ctx || --ctx->refcnt > 0)
        वापस;

    मुक्त(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    मुक्त(ctx);
}

अटल स्थिर अक्षर *
log_level_to_prefix(क्रमागत xkb_log_level level)
{
    चयन (level) {
    हाल XKB_LOG_LEVEL_DEBUG:
        वापस "xkbcommon: DEBUG: ";
    हाल XKB_LOG_LEVEL_INFO:
        वापस "xkbcommon: INFO: ";
    हाल XKB_LOG_LEVEL_WARNING:
        वापस "xkbcommon: WARNING: ";
    हाल XKB_LOG_LEVEL_ERROR:
        वापस "xkbcommon: ERROR: ";
    हाल XKB_LOG_LEVEL_CRITICAL:
        वापस "xkbcommon: CRITICAL: ";
    शेष:
        वापस शून्य;
    }
}

ATTR_PRINTF(3, 0) अटल व्योम
default_log_fn(काष्ठा xkb_context *ctx, क्रमागत xkb_log_level level,
               स्थिर अक्षर *fmt, बहु_सूची args)
{
    स्थिर अक्षर *prefix = log_level_to_prefix(level);

    अगर (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

अटल क्रमागत xkb_log_level
log_level(स्थिर अक्षर *level) {
    त्रुटि_सं = 0;
    अक्षर *endptr = शून्य;
    स्थिर दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    अगर (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        वापस (क्रमागत xkb_log_level)lvl;
    अगर (istreq_prefix("crit", level))
        वापस XKB_LOG_LEVEL_CRITICAL;
    अगर (istreq_prefix("err", level))
        वापस XKB_LOG_LEVEL_ERROR;
    अगर (istreq_prefix("warn", level))
        वापस XKB_LOG_LEVEL_WARNING;
    अगर (istreq_prefix("info", level))
        वापस XKB_LOG_LEVEL_INFO;
    अगर (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        वापस XKB_LOG_LEVEL_DEBUG;

    वापस XKB_LOG_LEVEL_ERROR;
}

अटल पूर्णांक
log_verbosity(स्थिर अक्षर *verbosity) {
    त्रुटि_सं = 0;
    स्थिर दीर्घ पूर्णांक v = म_से_दीर्घ(verbosity, शून्य, 10);
    अगर (त्रुटि_सं == 0) {
        /* Truncate */
        वापस (पूर्णांक) v;
    }

    वापस XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
काष्ठा xkb_context *
xkb_context_new(क्रमागत xkb_context_flags flags)
{
    स्थिर अक्षर *env;
    काष्ठा xkb_context *ctx = सुस्मृति(1, माप(*ctx));

    अगर (!ctx)
        वापस शून्य;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    स्थिर क्रमागत xkb_context_flags invalid_flags =
        (flags & ~(क्रमागत xkb_context_flags)XKB_CONTEXT_FLAGS_मूल्यावली);
    अगर (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        मुक्त(ctx);
        वापस शून्य;
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
    अगर (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    अगर (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    अगर (!ctx->atom_table) {
        xkb_context_unref(ctx);
        वापस शून्य;
    }

    ctx->x11_atom_cache = शून्य;

    वापस ctx;
}

व्योम
xkb_context_set_log_fn(काष्ठा xkb_context *ctx,
                       व्योम (*log_fn)(काष्ठा xkb_context *ctx,
                                      क्रमागत xkb_log_level level,
                                      स्थिर अक्षर *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

क्रमागत xkb_log_level
xkb_context_get_log_level(काष्ठा xkb_context *ctx)
{
    वापस ctx->log_level;
}

व्योम
xkb_context_set_log_level(काष्ठा xkb_context *ctx, क्रमागत xkb_log_level level)
{
    ctx->log_level = level;
}

पूर्णांक
xkb_context_get_log_verbosity(काष्ठा xkb_context *ctx)
{
    वापस ctx->log_verbosity;
}

व्योम
xkb_context_set_log_verbosity(काष्ठा xkb_context *ctx, पूर्णांक verbosity)
{
    ctx->log_verbosity = verbosity;
}

व्योम *
xkb_context_get_user_तथ्य(काष्ठा xkb_context *ctx)
{
    अगर (ctx)
        वापस ctx->user_तथ्य;
    वापस शून्य;
}

व्योम
xkb_context_set_user_तथ्य(काष्ठा xkb_context *ctx, व्योम *user_तथ्य)
{
    ctx->user_तथ्य = user_तथ्य;
}
