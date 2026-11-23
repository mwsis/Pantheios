/* /////////////////////////////////////////////////////////////////////////
 * File:    test/performance/core/log_dispatch/main.cpp
 *
 * Purpose: Time the Pantheios core log paths, one case at a time.
 *
 * Created: 23rd November 2026
 * Updated: 23rd November 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* Pantheios header files */
#include <pantheios/pantheios.hpp>
#include <pantheios/frontends/fe.simple.h>

/* STLSoft header files */
#include <platformstl/performance/performance_counter.hpp>
#include <stlsoft/stlsoft.h>

/* Standard C++ header files */
#include <exception>
#include <new>

/* Standard C header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(_MSC_VER) && \
    defined(_DEBUG)
# include <crtdbg.h>
#endif /* _MSC_VER) && _DEBUG */


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#ifdef _DEBUG

const int       ITERATIONS_LIGHT    =   1000;
const int       ITERATIONS_HEAVY    =   100;
#else /* ? _DEBUG */

const int       ITERATIONS_LIGHT    =   1000000;
const int       ITERATIONS_HEAVY    =   100000;
#endif /* _DEBUG */

const int       NUM_WARMUPS         =   2;
const int       NUM_SAMPLES         =   3;

/* Longer than the core's internal stack buffer (2048, including NUL). */
const size_t    OVERSIZE_LENGTH     =   3000;

/* Longer than the core's printf stack buffer (4096, including NUL). */
const size_t    PRINTF_LONG_LENGTH  =   5000;


/* /////////////////////////////////////////////////////////////////////////
 * macros
 */

 #define PSTR(x)                                             PANTHEIOS_LITERAL_STRING(x)

#ifdef PANTHEIOS_USE_WIDE_STRINGS

# define PCHAR(ch)                                          L ## ch
#else /* ? PANTHEIOS_USE_WIDE_STRINGS */

# define PCHAR(ch)                                          ch
#endif /* PANTHEIOS_USE_WIDE_STRINGS */


/* /////////////////////////////////////////////////////////////////////////
 * globals
 */

PANTHEIOS_EXTERN PAN_CHAR_T const PANTHEIOS_FE_PROCESS_IDENTITY[] = PANTHEIOS_LITERAL_STRING("test.performance.core.log_dispatch");

static unsigned long long   s_sink;
static pantheios_slice_t    s_short[4];
static pantheios_slice_t    s_wide[32];
static pantheios_slice_t    s_oversize[1];
static PAN_CHAR_T           s_oversize_text[OVERSIZE_LENGTH + 1];
static PAN_CHAR_T           s_printf_long[PRINTF_LONG_LENGTH + 1];


/* /////////////////////////////////////////////////////////////////////////
 * cases
 */

static void prepare_emit_()
{
    pantheios_fe_simple_setSeverityCeiling(PANTHEIOS_SEV_DEBUG);
}

static void prepare_filter_()
{
    pantheios_fe_simple_setSeverityCeiling(PANTHEIOS_SEV_ERROR);
}

static int once_filtered_()
{
    return pantheios::log(PANTHEIOS_SEV_DEBUG, PSTR("filtered"));
}

static int once_severity_()
{
    return pantheios::pantheios_isSeverityLogged(PANTHEIOS_SEV_DEBUG);
}

static int once_short_()
{
    return pantheios::core::pantheios_log_n(PANTHEIOS_SEV_NOTICE, STLSOFT_NUM_ELEMENTS(s_short), s_short);
}

static int once_wide_()
{
    return pantheios::core::pantheios_log_n(PANTHEIOS_SEV_NOTICE, STLSOFT_NUM_ELEMENTS(s_wide), s_wide);
}

static int once_oversize_()
{
    return pantheios::core::pantheios_log_n(PANTHEIOS_SEV_NOTICE, STLSOFT_NUM_ELEMENTS(s_oversize), s_oversize);
}

static int once_appl_()
{
    return pantheios::log(PANTHEIOS_SEV_NOTICE, PSTR("alpha "), PSTR("beta "), PSTR("gamma "), PSTR("delta"));
}

static int once_printf_()
{
    return pantheios::pantheios_logprintf(PANTHEIOS_SEV_NOTICE, PSTR("%s %d"), PSTR("core"), 17);
}

static int once_printf_long_()
{
    return pantheios::pantheios_logprintf(PANTHEIOS_SEV_NOTICE, PSTR("%s"), s_printf_long);
}

struct case_t
{
    char const*     name;
    void          (*prepare)();
    int           (*once)();
    int             iterations;
};

static case_t const s_cases[] =
{
    {   "filtered",     prepare_filter_,    once_filtered_,     ITERATIONS_LIGHT    },
    {   "severity",     prepare_emit_,      once_severity_,     ITERATIONS_LIGHT    },
    {   "short",        prepare_emit_,      once_short_,        ITERATIONS_LIGHT    },
    {   "wide",         prepare_emit_,      once_wide_,         ITERATIONS_LIGHT    },
    {   "oversize",     prepare_emit_,      once_oversize_,     ITERATIONS_HEAVY    },
    {   "appl",         prepare_emit_,      once_appl_,         ITERATIONS_LIGHT    },
    {   "printf",       prepare_emit_,      once_printf_,       ITERATIONS_LIGHT    },
    {   "printf-long",  prepare_emit_,      once_printf_long_,  ITERATIONS_HEAVY    },
};


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

static void fill_char_(PAN_CHAR_T* p, size_t n, PAN_CHAR_T ch)
{
    { for (size_t i = 0; i != n; ++i)
    {
        p[i] = ch;
    }}
    p[n] = PAN_CHAR_T();
}

static size_t pan_strlen_(PAN_CHAR_T const* s)
{
#ifdef PANTHEIOS_USE_WIDE_STRINGS

    return ::wcslen(s);
#else /* ? PANTHEIOS_USE_WIDE_STRINGS */

    return ::strlen(s);
#endif /* PANTHEIOS_USE_WIDE_STRINGS */
}

static pantheios_slice_t make_slice_(PAN_CHAR_T const* s)
{
    pantheios_slice_t slice;

    slice.ptr = s;
    slice.len = pan_strlen_(s);

    return slice;
}

static int case_selected_(int argc, char* argv[], char const* name)
{
    if (argc < 2)
    {
        return 1;
    }

    { for (int i = 1; i != argc; ++i)
    {
        if (0 == ::strcmp(argv[i], name))
        {
            return 1;
        }
    }}

    return 0;
}

static int is_known_case_(char const* name)
{
    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(s_cases); ++i)
    {
        if (0 == ::strcmp(s_cases[i].name, name))
        {
            return 1;
        }
    }}

    return 0;
}

static void usage_()
{
    fprintf(stderr, "USAGE: test.performance.core.log_dispatch [<case> ...]\n");
    fprintf(stderr, "cases:");

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(s_cases); ++i)
    {
        fprintf(stderr, " %s", s_cases[i].name);
    }}

    fprintf(stderr, "\n");
}

static unsigned long time_case_(case_t const& c)
{
    platformstl::performance_counter counter;
    unsigned long long sink = 0;

    c.prepare();

    { for (int w = 0; w != NUM_WARMUPS; ++w)
    {
        { for (int i = 0; i != c.iterations; ++i)
        {
            sink += c.once();
        }}
    }}

    unsigned long best = ~0ul;

    { for (int sample = 0; sample != NUM_SAMPLES; ++sample)
    {
        counter.start();
        { for (int i = 0; i != c.iterations; ++i)
        {
            sink += c.once();
        }}
        counter.stop();

        unsigned long const us = static_cast<unsigned long>(counter.get_microseconds());

        if (us < best)
        {
            best = us;
        }
    }}

    s_sink += sink;

    return best;
}

static void prepare_inputs_()
{
    PAN_CHAR_T const* const short_text[] =
    {
        PSTR("alpha "),
        PSTR("beta "),
        PSTR("gamma "),
        PSTR("delta"),
    };

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(s_short); ++i)
    {
        s_short[i] = make_slice_(short_text[i]);
    }}

    PAN_CHAR_T const* const wide_text = PSTR("slice ");

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(s_wide); ++i)
    {
        s_wide[i] = make_slice_(wide_text);
    }}

    fill_char_(s_oversize_text, OVERSIZE_LENGTH, PCHAR('X'));
    s_oversize[0].ptr = s_oversize_text;
    s_oversize[0].len = OVERSIZE_LENGTH;

    fill_char_(s_printf_long, PRINTF_LONG_LENGTH, PCHAR('Y'));
}


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

static int main_(int argc, char* argv[])
{
    { for (int i = 1; i != argc; ++i)
    {
        if (0 == ::strcmp(argv[i], "-h") || 0 == ::strcmp(argv[i], "--help"))
        {
            usage_();

            return EXIT_SUCCESS;
        }

        if (!is_known_case_(argv[i]))
        {
            fprintf(stderr, "unknown case: %s\n", argv[i]);
            usage_();

            return EXIT_FAILURE;
        }
    }}

    prepare_inputs_();

    fprintf(stdout, "be.null; minimum of %d samples after %d warm-ups\n", NUM_SAMPLES, NUM_WARMUPS);
    fprintf(stdout, "%-12s %10s %12s %12s\n", "case", "iterations", "us", "ns/call");

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(s_cases); ++i)
    {
        if (!case_selected_(argc, argv, s_cases[i].name))
        {
            continue;
        }

        unsigned long const us = time_case_(s_cases[i]);
        double const ns = (static_cast<double>(us) * 1000.0) / static_cast<double>(s_cases[i].iterations);

        fprintf(stdout, "%-12s %10d %12lu %12.1f\n", s_cases[i].name, s_cases[i].iterations, us, ns);
    }}

    return (s_sink == ~0ull) ? 100 : EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    int             res;

#if defined(_MSC_VER) && \
    defined(_DEBUG)
    _CrtMemState    memState;

    _CrtMemCheckpoint(&memState);
#endif /* _MSC_VER && _DEBUG */

    try
    {
        res = main_(argc, argv);
    }
    catch (std::bad_alloc&)
    {
        fprintf(stderr, "out of memory\n");

        res = EXIT_FAILURE;
    }
    catch (std::exception& x)
    {
        fprintf(stderr, "exception: %s\n", x.what());

        res = EXIT_FAILURE;
    }
    catch (...)
    {
        fprintf(stderr, "unknown exception\n");

        res = EXIT_FAILURE;
    }

#if defined(_MSC_VER) && \
    defined(_DEBUG)
    _CrtMemDumpAllObjectsSince(&memState);
#endif /* _MSC_VER && _DEBUG */

    return res;
}

/* ///////////////////////////// end of file //////////////////////////// */

