/* /////////////////////////////////////////////////////////////////////////
 * File:    src/backends/bec.WindowsConsole.cpp
 *
 * Purpose: Implementation of the Pantheios ANSI-Console Stock Back-end API.
 *
 * Created: 20th October 2024
 * Updated: 25th January 2025
 *
 * Home:    http://www.pantheios.org/
 *
 * Copyright (c) 2024-2025, Matthew Wilson and Synesis Information Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Information Systems
 *   nor the names of any contributors may be used to endorse or promote
 *   products derived from this software without specific prior written
 *   permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


/* Pantheios header files */
#include <pantheios/pantheios.h>

#include <pantheios/internal/nox.h>
#define PANTHEIOS_BE_INIT_NO_CPP_STRUCT_INIT
#include <pantheios/backends/bec.AnsiConsole.h>
#include <pantheios/internal/safestr.h>

#include <pantheios/util/backends/arguments.h>

#include <stlsoft/stlsoft.h>
#include <platformstl/system/console_functions.h>

#include <stdio.h>


PANTHEIOS_CALL(void)
pantheios_be_AnsiConsole_getDefaultAppInit(
    pan_be_AnsiConsole_init_t* init
)
{
    PANTHEIOS_CONTRACT_ENFORCE_PRECONDITION_PARAMS_API(NULL != init, "initialisation structure pointer may not be null");

    init->version   =   PANTHEIOS_VER;
    init->flags     =   0;

#if 0

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(init->colours); ++i)
    {
        size_t const    index_from  =   i;
        size_t const    index_to    =   i;

        init->colours[index_to] = s_infos[index_from].consoleAttributes;
    }}

    { for (size_t i = 0; i != STLSOFT_NUM_ELEMENTS(init->colours2); ++i)
    {
        size_t const    index_from  =   i + STLSOFT_NUM_ELEMENTS(init->colours);
        size_t const    index_to    =   i;

        init->colours2[index_to] = s_infos[index_from].consoleAttributes;
    }}
#endif
}


PANTHEIOS_CALL(int)
pantheios_be_AnsiConsole_init(
    PAN_CHAR_T const*                   processIdentity
,   int                                 id
,   pan_be_AnsiConsole_init_t const*    init
,   void*                               reserved
,   void**                              ptoken
)
{
    ((void)&processIdentity);
    ((void)&id);
    ((void)&init);
    ((void)&reserved);
    ((void)&ptoken);


    return -1;
}

PANTHEIOS_CALL(void)
pantheios_be_AnsiConsole_uninit(void* token)
{
    ((void)&token);


}

PANTHEIOS_CALL(int)
pantheios_be_AnsiConsole_logEntry(
    void*               feToken
,   void*               beToken
,   int                 severity
,   PAN_CHAR_T const*   entry
,   size_t              cchEntry
)
{
#if !defined(PANTHEIOS_NO_NAMESPACE)
    using pantheios::pantheios_getStockSeverityStringSlice;
#endif /* !PANTHEIOS_NO_NAMESPACE */
#if !defined(PLATFORMSTL_NO_NAMESPACE)
    using platformstl::platformstl_C_isatty_stm;
#endif /* !PLATFORMSTL_NO_NAMESPACE */

    ((void)&feToken);
    ((void)&beToken);


    FILE*       stm =   stderr;
    auto const  sss =   pantheios_getStockSeverityStringSlice(severity);

    // There are three points at

    char const* pre;
    char const* post;

    if (platformstl_C_isatty_stm(stderr))
    {
        switch (severity & 0xf)
        {
        case  0: pre = "\033[1;34;41;5m"; break;
        case  1: pre = "\033[1;33;41;5m"; break;
        case  2: pre = "\033[1;33;41m"; break;
        case  3: pre = "\033[1;31m"; break;
        case  4: pre = "\033[1;33m"; break;
        case  5: pre = "\033[1;30m"; break;
        case  6: pre = "\033[1;30m"; break;
        case  7: pre = "\033[1;34m"; break;
        case  8: pre = "\034[1;34m"; break;
        case  9: pre = "\034[0;34m"; break;
        case 10: pre = "\034[0;34m"; break;
        case 11: pre = "\034[0;34m"; break;
        case 12: pre = "\034[0;34m"; break;
        case 13: pre = "\034[0;34m"; break;
        case 14: pre = "\034[0;34m"; break;
        case 15: pre = "\034[0;34m"; break;
        }

        post = "\033[0m";
    }
    else
    {
        pre = "";
        post = "";
    }

    fprintf(
        stm
    ,   "[%s%.*s%s]: %.*s\n"
    ,   pre
    ,   int(sss.len), sss.ptr
    ,   post
    ,   int(cchEntry), entry
    );

    return 0;
}

PANTHEIOS_CALL(int)
pantheios_be_AnsiConsole_parseArgs(
    size_t                      numArgs
,   pantheios_slice_t           args[]
,   pan_be_AnsiConsole_init_t*  init
)
{
    PANTHEIOS_CONTRACT_ENFORCE_PRECONDITION_PARAMS_API((NULL != args || 0 == numArgs), "argument pointer must be non-null, or number of arguments must be 0");
    PANTHEIOS_CONTRACT_ENFORCE_PRECONDITION_PARAMS_API(NULL != init, "initialisation structure pointer may not be null");

    pantheios_be_AnsiConsole_getDefaultAppInit(init);

    // 1. Parse the stock arguments
    int res = pantheios_be_parseStockArgs(numArgs, args, &init->flags);
    int r;

    if (res >= 0)
    {
        // 2. Parse the custom argument: "showColours"
        r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("noColours"), true, PANTHEIOS_BE_ANSICONSOLE_F_NO_COLOURS, &init->flags);

        if (0 == r)
        {
            r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("noColors"), true, PANTHEIOS_BE_ANSICONSOLE_F_NO_COLOURS, &init->flags);
        }

        if (r < 0)
        {
            res = r;
        }
        else
        {
            res += r;
        }
    }

    if (res >= 0)
    {
        // Parse the custom argument: "forceAnsiEscapeSequences"
        r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("forceAnsiEscapeSequences"), false, PANTHEIOS_BE_ANSICONSOLE_F_FORCE_ANSI_ESCAPE_SEQUENCES, &init->flags);

        if (r < 0)
        {
            res = r;
        }
        else
        {
            res += r;
        }
    }

    if (res >= 0)
    {
        // Parse the custom argument: "colourWholePrefix"
        r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("colourWholePrefix"), false, PANTHEIOS_BE_ANSICONSOLE_F_COLOUR_WHOLE_PREFIX, &init->flags);

        if (0 == r)
        {
            r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("colorWholePrefix"), false, PANTHEIOS_BE_ANSICONSOLE_F_COLOUR_WHOLE_PREFIX, &init->flags);
        }

        if (r < 0)
        {
            res = r;
        }
        else
        {
            res += r;
        }
    }

    if (res >= 0)
    {
        // Parse the custom argument: "colourMessage"
        r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("colourMessage"), false, PANTHEIOS_BE_ANSICONSOLE_F_COLOUR_MESSAGE, &init->flags);

        if (0 == r)
        {
            r = pantheios_be_parseBooleanArg(numArgs, args, PANTHEIOS_LITERAL_STRING("colorMessage"), false, PANTHEIOS_BE_ANSICONSOLE_F_COLOUR_MESSAGE, &init->flags);
        }

        if (r < 0)
        {
            res = r;
        }
        else
        {
            res += r;
        }
    }

    return res;
}



/* ///////////////////////////// end of file //////////////////////////// */

