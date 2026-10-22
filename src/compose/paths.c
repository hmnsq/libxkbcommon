/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <locale.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "messages-codes.h"
#समावेश "utils.h"
#समावेश "util-paths.h"
#समावेश "context.h"
#समावेश "paths.h"

क्रमागत resolve_name_direction {
    बायाँ_TO_दायाँ,
    दायाँ_TO_बायाँ,
};

स्थिर अक्षर *
get_xlocaledir_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    अगर (!dir)
        dir = XLOCALEDIR;
    वापस dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
अटल अक्षर *
resolve_name(काष्ठा xkb_context *ctx, स्थिर अक्षर *filename,
             क्रमागत resolve_name_direction direction, स्थिर अक्षर *name)
{
    पूर्णांक ret;
    bool ok;
    स्थिर अक्षर *xlocaledir;
    अक्षर path[512];
    खाता *file;
    अक्षर *string;
    माप_प्रकार string_आकार;
    स्थिर अक्षर *end;
    स्थिर अक्षर *s, *बायाँ, *दायाँ;
    अक्षर *match;
    माप_प्रकार बायाँ_len, दायाँ_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, माप(path), "%s/%s", xlocaledir, filename);
    अगर (ret < 0 || (माप_प्रकार) ret >= माप(path))
        वापस शून्य;

    file = ख_खोलो(path, "rb");
    अगर (!file)
        वापस शून्य;

    ok = map_file(file, &string, &string_आकार);
    ख_बंद(file);
    अगर (!ok)
        वापस शून्य;

    s = string;
    end = string + string_आकार;
    name_len = म_माप(name);
    match = शून्य;

    जबतक (s < end) {
        /* Skip spaces. */
        जबतक (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        अगर (s < end && *s == '#') {
            जबतक (s < end && *s != '\n')
                s++;
            जारी;
        }

        /* Get the left value. */
        बायाँ = s;
        जबतक (s < end && !is_space(*s) && *s != ':')
            s++;
        बायाँ_len = (माप_प्रकार)(s - बायाँ);

        /* There's an optional colon between left and right. */
        अगर (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        जबतक (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        दायाँ = s;
        जबतक (s < end && !is_space(*s))
            s++;
        दायाँ_len = (माप_प्रकार)(s - दायाँ);

        /* Discard rest of line. */
        जबतक (s < end && *s != '\n')
            s++;

        अगर (direction == बायाँ_TO_दायाँ) {
            अगर (बायाँ_len == name_len && स_भेद(बायाँ, name, बायाँ_len) == 0) {
                match = strndup(दायाँ, दायाँ_len);
                अवरोध;
            }
        } अन्यथा अगर (direction == दायाँ_TO_बायाँ &&
                   दायाँ_len == name_len &&
                   स_भेद(दायाँ, name, दायाँ_len) == 0) {
            match = strndup(बायाँ, बायाँ_len);
            अवरोध;
        }
    }

    unmap_file(string, string_आकार);
    वापस match;
}

अक्षर *
resolve_locale(काष्ठा xkb_context *ctx, स्थिर अक्षर *locale)
{
    अक्षर *alias = resolve_name(ctx, "locale.alias", बायाँ_TO_दायाँ, locale);
    वापस alias ? alias : strdup(locale);
}

अक्षर *
get_xcomposefile_path(काष्ठा xkb_context *ctx)
{
    वापस strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

अक्षर *
get_xdg_xcompose_file_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *xdg_config_home;
    स्थिर अक्षर *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    अगर (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        अगर (!home)
            वापस शून्य;
        वापस asprintf_safe("%s/.config/XCompose", home);
    }

    वापस asprintf_safe("%s/XCompose", xdg_config_home);
}

अक्षर *
get_home_xcompose_file_path(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *home;

    home = xkb_context_getenv(ctx, "HOME");
    अगर (!home)
        वापस शून्य;

    वापस asprintf_safe("%s/.XCompose", home);
}

अक्षर *
get_locale_compose_file_path(काष्ठा xkb_context *ctx, स्थिर अक्षर *locale)
{
    अक्षर *resolved;
    अक्षर *path;

    /*
     * WARNING: Random workaround ahead.
     *
     * We currently do not support non-UTF-8 Compose files.  The C/POSIX
     * locale is specified to be the default fallback locale with an
     * ASCII charset.  But for some reason the compose.dir points the C
     * locale to the iso8859-1/Compose file, which is not ASCII but
     * ISO8859-1.  Since this is bound to happen a lot, and since our API
     * is UTF-8 based, and since 99% of the time a C locale is really just
     * a misconfiguration for UTF-8, let's do the most helpful thing.
     */
    अटल स्थिर अक्षर * स्थिर fallback = "en_US.UTF-8";
    अटल स्थिर अक्षर * स्थिर registry = "compose.dir";
    अगर (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, दायाँ_TO_बायाँ, locale);
    अगर (!resolved) {
#अगर HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        अगर (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            वापस शून्य;
        } अन्यथा {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, दायाँ_TO_बायाँ, fallback);
            अगर (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                वापस शून्य;
            } अन्यथा {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#अन्यथा
        वापस शून्य;
#पूर्ण_अगर
    }

    अगर (is_absolute_path(resolved)) {
        path = resolved;
    }
    अन्यथा {
        स्थिर अक्षर *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        मुक्त(resolved);
    }

    वापस path;
}
