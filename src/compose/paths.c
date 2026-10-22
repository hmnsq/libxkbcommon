/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <locale.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-paths.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "paths.h"

गणना resolve_name_direction {
    बायाँ_TO_दायाँ,
    दायाँ_TO_बायाँ,
};

ध्रुव वर्ण *
get_xlocaledir_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    यदि (!dir)
        dir = XLOCALEDIR;
    प्रत्यागम dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
स्थिर वर्ण *
resolve_name(संरचना xkb_context *ctx, ध्रुव वर्ण *filename,
             गणना resolve_name_direction direction, ध्रुव वर्ण *name)
{
    पूर्णाङ्क ret;
    bool ok;
    ध्रुव वर्ण *xlocaledir;
    वर्ण path[512];
    खाता *file;
    वर्ण *string;
    माप_प्रकार string_size;
    ध्रुव वर्ण *end;
    ध्रुव वर्ण *s, *बायाँ, *दायाँ;
    वर्ण *match;
    माप_प्रकार बायाँ_len, दायाँ_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, परिमाण(path), "%s/%s", xlocaledir, filename);
    यदि (ret < 0 || (माप_प्रकार) ret >= परिमाण(path))
        प्रत्यागम रिक्त;

    file = ख_खोलो(path, "rb");
    यदि (!file)
        प्रत्यागम रिक्त;

    ok = map_file(file, &string, &string_size);
    ख_बंद(file);
    यदि (!ok)
        प्रत्यागम रिक्त;

    s = string;
    end = string + string_size;
    name_len = म_माप(name);
    match = रिक्त;

    यावत् (s < end) {
        /* Skip spaces. */
        यावत् (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        यदि (s < end && *s == '#') {
            यावत् (s < end && *s != '\n')
                s++;
            अनुवर्त;
        }

        /* Get the left value. */
        बायाँ = s;
        यावत् (s < end && !is_space(*s) && *s != ':')
            s++;
        बायाँ_len = (माप_प्रकार)(s - बायाँ);

        /* There's an optional colon between left and right. */
        यदि (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        यावत् (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        दायाँ = s;
        यावत् (s < end && !is_space(*s))
            s++;
        दायाँ_len = (माप_प्रकार)(s - दायाँ);

        /* Discard rest of line. */
        यावत् (s < end && *s != '\n')
            s++;

        यदि (direction == बायाँ_TO_दायाँ) {
            यदि (बायाँ_len == name_len && स_भेद(बायाँ, name, बायाँ_len) == 0) {
                match = strndup(दायाँ, दायाँ_len);
                भङ्ग;
            }
        } अन्यथा यदि (direction == दायाँ_TO_बायाँ &&
                   दायाँ_len == name_len &&
                   स_भेद(दायाँ, name, दायाँ_len) == 0) {
            match = strndup(बायाँ, बायाँ_len);
            भङ्ग;
        }
    }

    unmap_file(string, string_size);
    प्रत्यागम match;
}

वर्ण *
resolve_locale(संरचना xkb_context *ctx, ध्रुव वर्ण *locale)
{
    वर्ण *alias = resolve_name(ctx, "locale.alias", बायाँ_TO_दायाँ, locale);
    प्रत्यागम alias ? alias : strdup(locale);
}

वर्ण *
get_xcomposefile_path(संरचना xkb_context *ctx)
{
    प्रत्यागम strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

वर्ण *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *xdg_config_home;
    ध्रुव वर्ण *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    यदि (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        यदि (!home)
            प्रत्यागम रिक्त;
        प्रत्यागम asprintf_safe("%s/.config/XCompose", home);
    }

    प्रत्यागम asprintf_safe("%s/XCompose", xdg_config_home);
}

वर्ण *
get_home_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *home;

    home = xkb_context_getenv(ctx, "HOME");
    यदि (!home)
        प्रत्यागम रिक्त;

    प्रत्यागम asprintf_safe("%s/.XCompose", home);
}

वर्ण *
get_locale_compose_file_path(संरचना xkb_context *ctx, ध्रुव वर्ण *locale)
{
    वर्ण *resolved;
    वर्ण *path;

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
    स्थिर ध्रुव वर्ण * ध्रुव fallback = "en_US.UTF-8";
    स्थिर ध्रुव वर्ण * ध्रुव registry = "compose.dir";
    यदि (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, दायाँ_TO_बायाँ, locale);
    यदि (!resolved) {
#यदि HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        यदि (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            प्रत्यागम रिक्त;
        } अन्यथा {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, दायाँ_TO_बायाँ, fallback);
            यदि (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                प्रत्यागम रिक्त;
            } अन्यथा {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#अन्यथा
        प्रत्यागम रिक्त;
#endif
    }

    यदि (is_absolute_path(resolved)) {
        path = resolved;
    }
    अन्यथा {
        ध्रुव वर्ण *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        मुक्त(resolved);
    }

    प्रत्यागम path;
}
