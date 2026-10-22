/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <locale.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-paths.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "paths.h"

எண்ணிக்கை resolve_name_direction {
    இடது_TO_வலது,
    வலது_TO_இடது,
};

மாறிலி எழுத்து *
get_xlocaledir_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    எனில் (!dir)
        dir = XLOCALEDIR;
    திரும்பு dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
நிலையான எழுத்து *
resolve_name(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *filename,
             எண்ணிக்கை resolve_name_direction direction, மாறிலி எழுத்து *name)
{
    முழு ret;
    bool ok;
    மாறிலி எழுத்து *xlocaledir;
    எழுத்து path[512];
    FILE *file;
    எழுத்து *string;
    size_t string_size;
    மாறிலி எழுத்து *end;
    மாறிலி எழுத்து *s, *இடது, *வலது;
    எழுத்து *match;
    size_t இடது_len, வலது_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, அளவு(path), "%s/%s", xlocaledir, filename);
    எனில் (ret < 0 || (size_t) ret >= அளவு(path))
        திரும்பு வெற்று;

    file = fopen(path, "rb");
    எனில் (!file)
        திரும்பு வெற்று;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    எனில் (!ok)
        திரும்பு வெற்று;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = வெற்று;

    போது (s < end) {
        /* Skip spaces. */
        போது (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        எனில் (s < end && *s == '#') {
            போது (s < end && *s != '\n')
                s++;
            தொடர்;
        }

        /* Get the left value. */
        இடது = s;
        போது (s < end && !is_space(*s) && *s != ':')
            s++;
        இடது_len = (size_t)(s - இடது);

        /* There's an optional colon between left and right. */
        எனில் (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        போது (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        வலது = s;
        போது (s < end && !is_space(*s))
            s++;
        வலது_len = (size_t)(s - வலது);

        /* Discard rest of line. */
        போது (s < end && *s != '\n')
            s++;

        எனில் (direction == இடது_TO_வலது) {
            எனில் (இடது_len == name_len && memcmp(இடது, name, இடது_len) == 0) {
                match = strndup(வலது, வலது_len);
                நிறுத்து;
            }
        } இல்லையெனில் எனில் (direction == வலது_TO_இடது &&
                   வலது_len == name_len &&
                   memcmp(வலது, name, வலது_len) == 0) {
            match = strndup(இடது, இடது_len);
            நிறுத்து;
        }
    }

    unmap_file(string, string_size);
    திரும்பு match;
}

எழுத்து *
resolve_locale(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *locale)
{
    எழுத்து *alias = resolve_name(ctx, "locale.alias", இடது_TO_வலது, locale);
    திரும்பு alias ? alias : strdup(locale);
}

எழுத்து *
get_xcomposefile_path(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

எழுத்து *
get_xdg_xcompose_file_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *xdg_config_home;
    மாறிலி எழுத்து *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    எனில் (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        எனில் (!home)
            திரும்பு வெற்று;
        திரும்பு asprintf_safe("%s/.config/XCompose", home);
    }

    திரும்பு asprintf_safe("%s/XCompose", xdg_config_home);
}

எழுத்து *
get_home_xcompose_file_path(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *home;

    home = xkb_context_getenv(ctx, "HOME");
    எனில் (!home)
        திரும்பு வெற்று;

    திரும்பு asprintf_safe("%s/.XCompose", home);
}

எழுத்து *
get_locale_compose_file_path(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *locale)
{
    எழுத்து *resolved;
    எழுத்து *path;

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
    நிலையான மாறிலி எழுத்து * மாறிலி fallback = "en_US.UTF-8";
    நிலையான மாறிலி எழுத்து * மாறிலி registry = "compose.dir";
    எனில் (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, வலது_TO_இடது, locale);
    எனில் (!resolved) {
#எனில் HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        எனில் (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            திரும்பு வெற்று;
        } இல்லையெனில் {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, வலது_TO_இடது, fallback);
            எனில் (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                திரும்பு வெற்று;
            } இல்லையெனில் {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#இல்லையெனில்
        திரும்பு வெற்று;
#endif
    }

    எனில் (is_absolute_path(resolved)) {
        path = resolved;
    }
    இல்லையெனில் {
        மாறிலி எழுத்து *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        விடுவி(resolved);
    }

    திரும்பு path;
}
