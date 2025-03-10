/* /////////////////////////////////////////////////////////////////////////
 * File:    pantheios/util/severity/syslog.h
 *
 * Purpose: Translations from Pantheios stock severity to ACE priority.
 *
 * Created: 13th November 2007
 * Updated: 7th February 2024
 *
 * Home:    http://www.pantheios.org/
 *
 * Copyright (c) 2019-2024, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2007-2019, Matthew Wilson and Synesis Software
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
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the
 *   names of any contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
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


/** \file pantheios/util/severity/syslog.h
 *
 * [C, C++] Translations from Pantheios stock severity to ACE priority.
 */

#ifndef PANTHEIOS_INCL_PANTHEIOS_UTIL_SEVERITY_H_ACE
#define PANTHEIOS_INCL_PANTHEIOS_UTIL_SEVERITY_H_ACE


/* /////////////////////////////////////////////////////////////////////////
 * version information
 */

#ifndef PANTHEIOS_DOCUMENTATION_SKIP_SECTION
# define PANTHEIOS_VER_PANTHEIOS_UTIL_SEVERITY_H_ACE_MAJOR      1
# define PANTHEIOS_VER_PANTHEIOS_UTIL_SEVERITY_H_ACE_MINOR      2
# define PANTHEIOS_VER_PANTHEIOS_UTIL_SEVERITY_H_ACE_REVISION   2
# define PANTHEIOS_VER_PANTHEIOS_UTIL_SEVERITY_H_ACE_EDIT       17
#endif /* !PANTHEIOS_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#ifndef PANTHEIOS_INCL_PANTHEIOS_H_PANTHEIOS
# include <pantheios/pantheios.h>
#endif /* !PANTHEIOS_INCL_PANTHEIOS_H_PANTHEIOS */
#ifndef PANTHEIOS_INCL_PANTHEIOS_QUALITY_H_CONTRACT
# include <pantheios/quality/contract.h>
#endif /* !PANTHEIOS_INCL_PANTHEIOS_QUALITY_H_CONTRACT */

#ifndef PANTHEIOS_INCL_H_SYSLOG
# define PANTHEIOS_INCL_H_SYSLOG
# include <syslog.h>
#endif /* !PANTHEIOS_INCL_H_SYSLOG */


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(PANTHEIOS_NO_NAMESPACE)
namespace pantheios
{

#endif /* !PANTHEIOS_NO_NAMESPACE */


/* /////////////////////////////////////////////////////////////////////////
 * functions
 */

/** Converts a Pantheios severity level to a SysLog severity level
 *
 * \param severity The Pantheios \link pantheios::pan_severity_t severity level\endlink
 *
 * \return The SysLog severity level
 */
#ifdef __cplusplus
inline
#else /* ? __cplusplus */
static
#endif /* __cplusplus */
       int pantheios_severity_to_syslog_severity(int severity)
{
    int slsev;

    PANTHEIOS_CONTRACT_ENFORCE_PRECONDITION_PARAMS_API((severity >= 0 && severity < 16), "invalid severity");

    switch (severity)
    {
        case    PANTHEIOS_SEV_EMERGENCY:        slsev = LOG_EMERG;      break;
        case    PANTHEIOS_SEV_ALERT:            slsev = LOG_ALERT;      break;
        case    PANTHEIOS_SEV_CRITICAL:         slsev = LOG_CRIT;       break;
        case    PANTHEIOS_SEV_ERROR:            slsev = LOG_ERR;        break;
        case    PANTHEIOS_SEV_WARNING:          slsev = LOG_WARNING;    break;
        case    PANTHEIOS_SEV_NOTICE:           slsev = LOG_NOTICE;     break;
        case    PANTHEIOS_SEV_INFORMATIONAL:    slsev = LOG_INFO;       break;
        default:
        case    PANTHEIOS_SEV_DEBUG:            slsev = LOG_DEBUG;      break;
    }

    return slsev;
}


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(PANTHEIOS_NO_NAMESPACE)
} /* namespace pantheios */
#endif /* !PANTHEIOS_NO_NAMESPACE */


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !PANTHEIOS_INCL_PANTHEIOS_UTIL_SEVERITY_H_ACE */


/* ///////////////////////////// end of file //////////////////////////// */

