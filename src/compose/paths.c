/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <locale.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-paths.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "paths.h"

গণনা resolve_name_direction {
    বাম_TO_ডান,
    ডান_TO_বাম,
};

ধ্রুব বর্ণ *
get_xlocaledir_path(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    যদি (!dir)
        dir = XLOCALEDIR;
    প্রত্যাগম dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
স্থির বর্ণ *
resolve_name(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *filename,
             গণনা resolve_name_direction direction, ধ্রুব বর্ণ *name)
{
    পূর্ণাঙ্ক ret;
    bool ok;
    ধ্রুব বর্ণ *xlocaledir;
    বর্ণ path[512];
    FILE *file;
    বর্ণ *string;
    size_t string_size;
    ধ্রুব বর্ণ *end;
    ধ্রুব বর্ণ *s, *বাম, *ডান;
    বর্ণ *match;
    size_t বাম_len, ডান_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, পরিমাণ(path), "%s/%s", xlocaledir, filename);
    যদি (ret < 0 || (size_t) ret >= পরিমাণ(path))
        প্রত্যাগম রিক্ত;

    file = fopen(path, "rb");
    যদি (!file)
        প্রত্যাগম রিক্ত;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    যদি (!ok)
        প্রত্যাগম রিক্ত;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = রিক্ত;

    যাবত্ (s < end) {
        /* Skip spaces. */
        যাবত্ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        যদি (s < end && *s == '#') {
            যাবত্ (s < end && *s != '\n')
                s++;
            অনুবর্ত;
        }

        /* Get the left value. */
        বাম = s;
        যাবত্ (s < end && !is_space(*s) && *s != ':')
            s++;
        বাম_len = (size_t)(s - বাম);

        /* There's an optional colon between left and right. */
        যদি (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        যাবত্ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        ডান = s;
        যাবত্ (s < end && !is_space(*s))
            s++;
        ডান_len = (size_t)(s - ডান);

        /* Discard rest of line. */
        যাবত্ (s < end && *s != '\n')
            s++;

        যদি (direction == বাম_TO_ডান) {
            যদি (বাম_len == name_len && memcmp(বাম, name, বাম_len) == 0) {
                match = strndup(ডান, ডান_len);
                ভঙ্গ;
            }
        } অন্যথা যদি (direction == ডান_TO_বাম &&
                   ডান_len == name_len &&
                   memcmp(ডান, name, ডান_len) == 0) {
            match = strndup(বাম, বাম_len);
            ভঙ্গ;
        }
    }

    unmap_file(string, string_size);
    প্রত্যাগম match;
}

বর্ণ *
resolve_locale(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *locale)
{
    বর্ণ *alias = resolve_name(ctx, "locale.alias", বাম_TO_ডান, locale);
    প্রত্যাগম alias ? alias : strdup(locale);
}

বর্ণ *
get_xcomposefile_path(সংরচনা xkb_context *ctx)
{
    প্রত্যাগম strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

বর্ণ *
get_xdg_xcompose_file_path(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *xdg_config_home;
    ধ্রুব বর্ণ *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    যদি (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        যদি (!home)
            প্রত্যাগম রিক্ত;
        প্রত্যাগম asprintf_safe("%s/.config/XCompose", home);
    }

    প্রত্যাগম asprintf_safe("%s/XCompose", xdg_config_home);
}

বর্ণ *
get_home_xcompose_file_path(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *home;

    home = xkb_context_getenv(ctx, "HOME");
    যদি (!home)
        প্রত্যাগম রিক্ত;

    প্রত্যাগম asprintf_safe("%s/.XCompose", home);
}

বর্ণ *
get_locale_compose_file_path(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *locale)
{
    বর্ণ *resolved;
    বর্ণ *path;

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
    স্থির ধ্রুব বর্ণ * ধ্রুব fallback = "en_US.UTF-8";
    স্থির ধ্রুব বর্ণ * ধ্রুব registry = "compose.dir";
    যদি (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ডান_TO_বাম, locale);
    যদি (!resolved) {
#যদি HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        যদি (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            প্রত্যাগম রিক্ত;
        } অন্যথা {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ডান_TO_বাম, fallback);
            যদি (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                প্রত্যাগম রিক্ত;
            } অন্যথা {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#অন্যথা
        প্রত্যাগম রিক্ত;
#endif
    }

    যদি (is_absolute_path(resolved)) {
        path = resolved;
    }
    অন্যথা {
        ধ্রুব বর্ণ *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        মুক্ত(resolved);
    }

    প্রত্যাগম path;
}
