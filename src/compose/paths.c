/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <locale.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-paths.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "paths.h"

ଗଣନା resolve_name_direction {
    ବାମ_TO_ଡାହାଣ,
    ଡାହାଣ_TO_ବାମ,
};

ଅଚଳ ଅକ୍ଷର *
get_xlocaledir_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ଯଦି (!dir)
        dir = XLOCALEDIR;
    ଫେରନ୍ତୁ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ସ୍ଥିର ଅକ୍ଷର *
resolve_name(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *filename,
             ଗଣନା resolve_name_direction direction, ଅଚଳ ଅକ୍ଷର *name)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    bool ok;
    ଅଚଳ ଅକ୍ଷର *xlocaledir;
    ଅକ୍ଷର path[512];
    FILE *file;
    ଅକ୍ଷର *string;
    size_t string_size;
    ଅଚଳ ଅକ୍ଷର *end;
    ଅଚଳ ଅକ୍ଷର *s, *ବାମ, *ଡାହାଣ;
    ଅକ୍ଷର *match;
    size_t ବାମ_len, ଡାହାଣ_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, ଆକାର(path), "%s/%s", xlocaledir, filename);
    ଯଦି (ret < 0 || (size_t) ret >= ଆକାର(path))
        ଫେରନ୍ତୁ ଖାଲି;

    file = fopen(path, "rb");
    ଯଦି (!file)
        ଫେରନ୍ତୁ ଖାଲି;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    ଯଦି (!ok)
        ଫେରନ୍ତୁ ଖାଲି;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = ଖାଲି;

    ଯେତେବେଳେ (s < end) {
        /* Skip spaces. */
        ଯେତେବେଳେ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        ଯଦି (s < end && *s == '#') {
            ଯେତେବେଳେ (s < end && *s != '\n')
                s++;
            ଜାରି;
        }

        /* Get the left value. */
        ବାମ = s;
        ଯେତେବେଳେ (s < end && !is_space(*s) && *s != ':')
            s++;
        ବାମ_len = (size_t)(s - ବାମ);

        /* There's an optional colon between left and right. */
        ଯଦି (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        ଯେତେବେଳେ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        ଡାହାଣ = s;
        ଯେତେବେଳେ (s < end && !is_space(*s))
            s++;
        ଡାହାଣ_len = (size_t)(s - ଡାହାଣ);

        /* Discard rest of line. */
        ଯେତେବେଳେ (s < end && *s != '\n')
            s++;

        ଯଦି (direction == ବାମ_TO_ଡାହାଣ) {
            ଯଦି (ବାମ_len == name_len && memcmp(ବାମ, name, ବାମ_len) == 0) {
                match = strndup(ଡାହାଣ, ଡାହାଣ_len);
                ଭାଙ୍ଗ;
            }
        } ଅନ୍ୟଥା ଯଦି (direction == ଡାହାଣ_TO_ବାମ &&
                   ଡାହାଣ_len == name_len &&
                   memcmp(ଡାହାଣ, name, ଡାହାଣ_len) == 0) {
            match = strndup(ବାମ, ବାମ_len);
            ଭାଙ୍ଗ;
        }
    }

    unmap_file(string, string_size);
    ଫେରନ୍ତୁ match;
}

ଅକ୍ଷର *
resolve_locale(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *locale)
{
    ଅକ୍ଷର *alias = resolve_name(ctx, "locale.alias", ବାମ_TO_ଡାହାଣ, locale);
    ଫେରନ୍ତୁ alias ? alias : strdup(locale);
}

ଅକ୍ଷର *
get_xcomposefile_path(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

ଅକ୍ଷର *
get_xdg_xcompose_file_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *xdg_config_home;
    ଅଚଳ ଅକ୍ଷର *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ଯଦି (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ଯଦି (!home)
            ଫେରନ୍ତୁ ଖାଲି;
        ଫେରନ୍ତୁ asprintf_safe("%s/.config/XCompose", home);
    }

    ଫେରନ୍ତୁ asprintf_safe("%s/XCompose", xdg_config_home);
}

ଅକ୍ଷର *
get_home_xcompose_file_path(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *home;

    home = xkb_context_getenv(ctx, "HOME");
    ଯଦି (!home)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ asprintf_safe("%s/.XCompose", home);
}

ଅକ୍ଷର *
get_locale_compose_file_path(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *locale)
{
    ଅକ୍ଷର *resolved;
    ଅକ୍ଷର *path;

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
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ fallback = "en_US.UTF-8";
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ registry = "compose.dir";
    ଯଦି (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ଡାହାଣ_TO_ବାମ, locale);
    ଯଦି (!resolved) {
#ଯଦି HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ଯଦି (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ଫେରନ୍ତୁ ଖାଲି;
        } ଅନ୍ୟଥା {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ଡାହାଣ_TO_ବାମ, fallback);
            ଯଦି (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ଫେରନ୍ତୁ ଖାଲି;
            } ଅନ୍ୟଥା {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ଅନ୍ୟଥା
        ଫେରନ୍ତୁ ଖାଲି;
#endif
    }

    ଯଦି (is_absolute_path(resolved)) {
        path = resolved;
    }
    ଅନ୍ୟଥା {
        ଅଚଳ ଅକ୍ଷର *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        ମୁକ୍ତ(resolved);
    }

    ଫେରନ୍ତୁ path;
}
